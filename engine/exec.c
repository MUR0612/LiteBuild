/** @file exec.c
 * @author Frank WU
 * @brief Command execution, pipeline execution and signal handling.
 */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "exec.h"

#include "../utils/logger.h"
#include "../utils/memory.h"

#define NANOSECONDS_IN_SECOND 1000000000L

static int execute_command(char** argv, RuntimeState* runtime, bool simulate);
static int interpret_child_status(int status);
static void record_running_pid(RuntimeState* runtime, pid_t pid);
static void remove_running_pid(RuntimeState* runtime, pid_t pid);

static void print_command(char** argv, int argc)
{
    for (int i = 0; i < argc; i++) {
        if (i > 0) {
            log_info(" ");
        }
        log_info("%s", argv[i]);
    }
    log_info("\n");
}

/**
 * Counts command arguments before the NULL marker.
 *
 * @param argv command argument list.
 * @return number of command arguments.
 */
static int count_command_args(char** argv)
{
    int count = 0;

    while (argv[count] != NULL) {
        count++;
    }

    return count;
}
/**
 * Creates the signal mask used while waiting for child processes.
 *
 * @param mask signal mask to fill in.
 */
static void make_wait_signal_mask(sigset_t* mask)
{
    sigemptyset(mask);
    sigaddset(mask, SIGINT);
    sigaddset(mask, SIGCHLD);
}

/**
 * Sets a one-second deadline from the current time.
 *
 * @param deadline timespec to update.
 */
static void reset_interrupt_deadline(struct timespec* deadline)
{
    clock_gettime(CLOCK_MONOTONIC, deadline);
    deadline->tv_sec += INTERRUPT_TIMEOUT_SEC;
}

/**
 * Calculates remaining time until a deadline.
 *
 * @param deadline absolute deadline time.
 * @param timeout output timeout value.
 * @return true if time remains before the deadline.
 */
static bool get_remaining_timeout(
        const struct timespec* deadline, struct timespec* timeout)
{
    /* Work out how much time is left before the interrupt deadline. */
    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);

    timeout->tv_sec = deadline->tv_sec - now.tv_sec;
    timeout->tv_nsec = deadline->tv_nsec - now.tv_nsec;

    if (timeout->tv_nsec < 0) {
        timeout->tv_sec--;
        timeout->tv_nsec += NANOSECONDS_IN_SECOND;
    }

    if (timeout->tv_sec < 0
            || (timeout->tv_sec == 0 && timeout->tv_nsec <= 0)) {
        timeout->tv_sec = 0;
        timeout->tv_nsec = 0;
        return false;
    }

    return true;
}

/**
 * Unblocks signals in a child before execvp is called.
 */
static void unblock_wait_signals(void)
{
    sigset_t mask;

    make_wait_signal_mask(&mask);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}
/**
 * Prints and executes one command, unless simulate mode is active.
 *
 * @param argv command argument list.
 * @param runtime runtime state used for signal and child tracking.
 * @param simulate true if the command should only be printed.
 * @return zero on success, or the command failure status.
 */
static int execute_command(char** argv, RuntimeState* runtime, bool simulate)
{
    int argc = count_command_args(argv);
    int status = 0;
    pid_t pid;
    sigset_t mask;

    /* Print the command before deciding whether to run it. */
    print_command(argv, argc);

    if (simulate) {
        return 0;
    }

    pid = fork();
    if (pid < 0) {
        log_error("fork failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        unblock_wait_signals();
        execvp(argv[0], argv);
        log_error("execvp failed (%s): %s\n", argv[0], strerror(errno));
        _exit(EXIT_FAILURE);
    }

    record_running_pid(runtime, pid);
    make_wait_signal_mask(&mask);

    /* Wait for either SIGINT or the child finishing. */
    while (true) {
        int signalNumber = sigtimedwait(&mask, NULL, NULL);

        if (signalNumber == SIGINT) {
            runtime->interrupted = true;
            handle_interrupt_shutdown(runtime);
        }

        if (signalNumber == SIGCHLD) {
            if (waitpid(pid, &status, WNOHANG) == pid) {
                remove_running_pid(runtime, pid);
                break;
            }
        }
    }

    return interpret_child_status(status);
}
/**
 * Converts a child wait status into the assignment exit status.
 *
 * @param status status value returned by waitpid.
 * @return child exit code, or signal failure status if signalled.
 */
static int interpret_child_status(int status)
{
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
/**
 * Executes a build command and reports whether it succeeded.
 *
 * @param argv command argument list.
 * @param runtime runtime state used for signal and child tracking.
 * @param simulate true if the command should only be printed.
 * @param targetName target being built by this command.
 */
bool execute_command_or_abort(char** argv, RuntimeState* runtime, bool simulate,
        const char* targetName)
{
    int result = execute_command(argv, runtime, simulate);

    if (result != 0) {
        log_error("cannot build target \"%s\" - aborting\n", targetName);
        return false;
    }
    return true;
}
/**
 * Checks whether a token is the pipeline separator.
 *
 * @param token token to check.
 * @return true if the token is the pipe token, false otherwise.
 */
static bool is_pipe_token(const char* token)
{
    return strcmp(token, "=>") == 0;
}
/**
 * Prints a command pipeline using the required output format.
 *
 * @param commands command list split into pipeline stages.
 * @param argcList number of arguments in each command.
 * @param cmdCount number of commands in the pipeline.
 */
static void print_pipeline(char*** commands, const int* argcList, int cmdCount)
{
    int i;
    int j;

    for (i = 0; i < cmdCount; i++) {
        if (i > 0) {
            log_info(" => ");
        }

        for (j = 0; j < argcList[i]; j++) {
            if (j > 0) {
                log_info(" ");
            }
            log_info("%s", commands[i][j]);
        }
    }

    log_info("\n");
    fflush(stdout);
}
/**
 * Waits for all pipeline child processes and checks pipeline failure rules.
 *
 * @param pids process IDs for the pipeline children.
 * @param cmdCount number of commands in the pipeline.
 * @param runtime runtime state used for child tracking.
 * @return zero if the pipeline succeeds, or pipeline failure status.
 */
static int wait_for_pipeline_children(
        const pid_t* pids, int cmdCount, RuntimeState* runtime)
{
    int waited = 0;
    int status;
    int result = 0;
    pid_t donePid;
    int i;
    sigset_t mask;

    make_wait_signal_mask(&mask);

    /* Reap children until every pipeline command has finished. */
    while (waited < cmdCount) {
        int signalNumber = sigtimedwait(&mask, NULL, NULL);

        if (signalNumber == SIGINT) {
            runtime->interrupted = true;
            handle_interrupt_shutdown(runtime);
        }

        if (signalNumber != SIGCHLD) {
            continue;
        }

        while ((donePid = waitpid(-1, &status, WNOHANG)) > 0) {
            remove_running_pid(runtime, donePid);

            /* Find which pipeline command this child belonged to. */
            for (i = 0; i < cmdCount; i++) {
                if (pids[i] == donePid) {
                    if (i == cmdCount - 1
                            && interpret_child_status(status) != 0) {
                        result = EXIT_FAILURE;
                    } else if (i != cmdCount - 1 && !WIFEXITED(status)) {
                        result = EXIT_FAILURE;
                    } else if (i != cmdCount - 1
                            && WEXITSTATUS(status) == EXIT_FAILURE) {
                        result = EXIT_FAILURE;
                    }
                    break;
                }
            }

            waited++;
        }
    }

    return result;
}
/**
 * Sets up stdin and stdout for one pipeline child.
 *
 * @param index command position in the pipeline.
 * @param cmdCount number of commands in the pipeline.
 * @param inputFd input file descriptor from the previous command.
 * @param pipeFd pipe file descriptors for the next command.
 * @param devNullFd file descriptor for /dev/null.
 */
static void setup_pipeline_child_fds(
        int index, int cmdCount, int inputFd, int pipeFd[2], int devNullFd)
{
    if (index == 0) {
        if (dup2(devNullFd, STDIN_FILENO) < 0) {
            log_error("dup2 failed for /dev/null: %s\n", strerror(errno));
            _exit(EXIT_FAILURE);
        }
    } else {
        if (dup2(inputFd, STDIN_FILENO) < 0) {
            log_error("dup2 failed for input pipe: %s\n", strerror(errno));
            _exit(EXIT_FAILURE);
        }
    }

    if (index < cmdCount - 1) {
        if (dup2(pipeFd[1], STDOUT_FILENO) < 0) {
            log_error("dup2 failed for output pipe: %s\n", strerror(errno));
            _exit(EXIT_FAILURE);
        }
    }
}

/**
 * Closes pipeline file descriptors that are not needed.
 *
 * @param inputFd input file descriptor from the previous command.
 * @param pipeFd pipe file descriptors for the next command.
 * @param devNullFd file descriptor for /dev/null.
 */
static void close_pipeline_fds(int inputFd, int pipeFd[2], int devNullFd)
{
    if (devNullFd >= 0) {
        close(devNullFd);
    }
    if (inputFd >= 0) {
        close(inputFd);
    }
    if (pipeFd[0] >= 0) {
        close(pipeFd[0]);
    }
    if (pipeFd[1] >= 0) {
        close(pipeFd[1]);
    }
}
/**
 * Records a pipeline child and updates parent-side file descriptors.
 *
 * @param runtime runtime state used for child tracking.
 * @param pid child process ID to record.
 * @param inputFd pointer to the current input file descriptor.
 * @param pipeFd pipe file descriptors for the next command.
 */
static void handle_pipeline_parent_after_fork(
        RuntimeState* runtime, pid_t pid, int* inputFd, int pipeFd[2])
{
    record_running_pid(runtime, pid);

    if (*inputFd >= 0) {
        close(*inputFd);
    }
    if (pipeFd[1] >= 0) {
        close(pipeFd[1]);
    }

    *inputFd = pipeFd[0];
}
/**
 * Starts all child processes in a command pipeline.
 *
 * @param commands command list split into pipeline stages.
 * @param cmdCount number of commands in the pipeline.
 * @param runtime runtime state used for child tracking.
 * @param pids process ID array to fill in.
 * @param devNullFd file descriptor for /dev/null.
 */
static int start_pipeline_children(char*** commands, int cmdCount,
        RuntimeState* runtime, pid_t* pids, int devNullFd)
{
    int i;
    int inputFd = -1;
    int pipeFd[2];
    pid_t pid;

    /* Iterate through each command and create a child process. */
    for (i = 0; i < cmdCount; i++) {
        if (runtime->interrupted) {
            handle_interrupt_shutdown(runtime);
        }

        /* Create a pipe for all but the last command. */
        if (i < cmdCount - 1) {
            if (pipe(pipeFd) < 0) {
                log_error("pipe failed: %s\n", strerror(errno));
                if (inputFd >= 0) {
                    close(inputFd);
                }
                return i;
            }
        } else {
            pipeFd[0] = -1;
            pipeFd[1] = -1;
        }

        /* Fork a child process to run this command. */
        pid = fork();
        if (pid < 0) {
            log_error("fork failed: %s\n", strerror(errno));
            if (inputFd >= 0) {
                close(inputFd);
            }
            if (pipeFd[0] >= 0) {
                close(pipeFd[0]);
            }
            if (pipeFd[1] >= 0) {
                close(pipeFd[1]);
            }
            return i;
        }

        if (pid == 0) {
            unblock_wait_signals();
            setup_pipeline_child_fds(i, cmdCount, inputFd, pipeFd, devNullFd);
            close_pipeline_fds(inputFd, pipeFd, devNullFd);
            /* Replace the child process with the command. */
            execvp(commands[i][0], commands[i]);
            log_error("execvp failed (%s): %s\n", commands[i][0],
                    strerror(errno));
            _exit(EXIT_FAILURE);
        }

        /* Parent records the child and prepares for the next stage. */
        pids[i] = pid;
        handle_pipeline_parent_after_fork(runtime, pid, &inputFd, pipeFd);
    }

    /* Close the final input file descriptor if needed. */
    if (inputFd >= 0) {
        close(inputFd);
    }
    return cmdCount;
}
/**
 * Prints and executes a command pipeline.
 *
 * @param commands command list split into pipeline stages.
 * @param argcList number of arguments in each command.
 * @param cmdCount number of commands in the pipeline.
 * @param runtime runtime state used for signal and child tracking.
 * @param simulate true if the pipeline should only be printed.
 * @return zero on success, or pipeline failure status.
 */
int execute_pipeline(
        char*** commands, int* argcList, int cmdCount, RuntimeState* runtime,
        bool simulate)
{
    int result;
    int devNullFd;
    pid_t* pids;
    int spawnedCount;

    /* Print the pipeline before execution or simulation. */
    print_pipeline(commands, argcList, cmdCount);

    /* In simulate mode, no execution is performed. */
    if (simulate) {
        return 0;
    }

    /* Allocate space for child process IDs and open /dev/null. */
    pids = xmalloc(sizeof(pid_t) * (size_t)cmdCount);
    devNullFd = open("/dev/null", O_RDONLY);
    if (devNullFd < 0) {
        log_error("failed to open /dev/null: %s\n", strerror(errno));
        free(pids);
        return EXIT_FAILURE;
    }

    /* Start all child processes in the pipeline. */
    spawnedCount = start_pipeline_children(commands, cmdCount, runtime, pids,
            devNullFd);

    /* Close the /dev/null descriptor in the parent. */
    if (devNullFd >= 0) {
        close(devNullFd);
    }

    /* Wait for all children and determine pipeline result. */
    result = wait_for_pipeline_children(pids, spawnedCount, runtime);
    free(pids);

    if (spawnedCount < cmdCount) {
        return EXIT_FAILURE;
    }

    return result;
}
/**
 * Checks whether an expanded command pipeline is valid.
 *
 * @param tokens expanded pipeline tokens.
 * @param tokenCount number of expanded tokens.
 * @return true if the pipeline is valid, false otherwise.
 */
bool is_valid_pipeline_after_expansion(char** tokens, int tokenCount)
{
    int i;
    bool needCommand = true;

    if (tokenCount <= 0) {
        return false;
    }

    /* A pipe token cannot appear before a command or after another pipe. */
    for (i = 0; i < tokenCount; i++) {
        if (is_pipe_token(tokens[i])) {
            if (needCommand) {
                return false;
            }
            needCommand = true;
        } else {
            needCommand = false;
        }
    }

    return !needCommand;
}
/**
 * Splits expanded pipeline tokens into separate command arrays.
 *
 * @param tokens expanded pipeline tokens.
 * @param tokenCount number of expanded tokens.
 * @param argcListOut output pointer for argument counts.
 * @param cmdCountOut output pointer for command count.
 * @return command arrays split by pipeline separators.
 */
char*** split_pipeline_into_commands(
        char** tokens, int tokenCount, int** argcListOut, int* cmdCountOut)
{
    int i;
    int commandIndex = 0;
    int argIndex = 0;
    int cmdCount = 1;
    char*** commands;
    int* argcList;

    /* First count how many commands are separated by pipe tokens. */
    for (i = 0; i < tokenCount; i++) {
        if (is_pipe_token(tokens[i])) {
            cmdCount++;
        }
    }

    commands = xmalloc(sizeof(char**) * (size_t)cmdCount);
    argcList = xmalloc(sizeof(int) * (size_t)cmdCount);
    memset(commands, 0, sizeof(char**) * (size_t)cmdCount);
    memset(argcList, 0, sizeof(int) * (size_t)cmdCount);

    /* Count arguments for each command before allocating arrays. */
    for (i = 0; i < tokenCount; i++) {
        if (is_pipe_token(tokens[i])) {
            commandIndex++;
            argIndex = 0;
        } else {
            argcList[commandIndex]++;
        }
    }

    for (i = 0; i < cmdCount; i++) {
        commands[i] = xmalloc(sizeof(char*) * ((size_t)argcList[i] + 1));
        memset(commands[i], 0, sizeof(char*) * ((size_t)argcList[i] + 1));
    }

    commandIndex = 0;
    argIndex = 0;
    /* Copy token pointers into each command array. */
    for (i = 0; i < tokenCount; i++) {
        if (is_pipe_token(tokens[i])) {
            commandIndex++;
            argIndex = 0;
        } else {
            commands[commandIndex][argIndex] = tokens[i];
            argIndex++;
        }
    }

    *argcListOut = argcList;
    *cmdCountOut = cmdCount;
    return commands;
}
/**
 * Creates an empty runtime state.
 *
 * @return initialised runtime state.
 */
RuntimeState init_runtime_state(void)
{
    RuntimeState runtime = {0};
    return runtime;
}
/**
 * Installs the SIGINT handler used by litebuild.
 *
 * @param runtime runtime state for the program.
 */
void install_signal_handlers(RuntimeState* runtime)
{
    sigset_t mask;

    (void)runtime;

    make_wait_signal_mask(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL);
}
/**
 * Records a running child process ID in the runtime state.
 *
 * @param runtime runtime state to update.
 * @param pid process ID to record.
 */
static void record_running_pid(RuntimeState* runtime, pid_t pid)
{
    pid_t* newPids;
    int newCapacity;

    if (runtime == NULL) {
        return;
    }

    /* Grow the pid array when there is no more space. */
    if (runtime->runningCount >= runtime->runningCapacity) {
        newCapacity = runtime->runningCapacity * 2;
        if (newCapacity == 0) {
            newCapacity = INITIAL_PID_CAPACITY;
        }

        newPids = xrealloc(runtime->runningPids,
                sizeof(pid_t) * (size_t)newCapacity);
        runtime->runningPids = newPids;
        runtime->runningCapacity = newCapacity;
    }

    runtime->runningPids[runtime->runningCount] = pid;
    runtime->runningCount++;
}
/**
 * Removes a child process ID from the runtime state.
 *
 * @param runtime runtime state to update.
 * @param pid process ID to remove.
 */
static void remove_running_pid(RuntimeState* runtime, pid_t pid)
{
    int i;

    if (runtime == NULL) {
        return;
    }

    for (i = 0; i < runtime->runningCount; i++) {
        if (runtime->runningPids[i] == pid) {
            runtime->runningPids[i]
                    = runtime->runningPids[runtime->runningCount - 1];
            runtime->runningCount--;
            return;
        }
    }
}
/**
 * Sends a signal to every running child process.
 *
 * @param runtime runtime state containing running child process IDs.
 * @param signalNumber signal number to send.
 */
static void signal_running_children(RuntimeState* runtime, int signalNumber)
{
    int i;

    for (i = 0; i < runtime->runningCount; i++) {
        kill(runtime->runningPids[i], signalNumber);
    }
}
/**
 * Reaps child processes that have already exited.
 *
 * @param runtime runtime state containing running child process IDs.
 */
static void reap_finished_children(RuntimeState* runtime)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        remove_running_pid(runtime, pid);
    }
}
/**
 * Reaps all remaining child processes using blocking waitpid.
 *
 * @param runtime runtime state containing running child process IDs.
 */
static void reap_all_children(RuntimeState* runtime)
{
    int status;
    pid_t pid;

    while (runtime->runningCount > 0) {
        pid = waitpid(-1, &status, 0);
        if (pid > 0) {
            remove_running_pid(runtime, pid);
        }
    }
}
/**
 * Waits for interrupted children and kills them if the timeout expires.
 *
 * @param runtime runtime state containing running child process IDs.
 * @param mask signal mask used while waiting for child completion.
 * @param deadline interrupt timeout deadline.
 */
static void wait_for_interrupted_children(
        RuntimeState* runtime, sigset_t* mask, struct timespec* deadline)
{
    struct timespec timeout;

    /* Wait until all children exit or the timeout is reached. */
    while (runtime->runningCount > 0) {
        int beforeCount = runtime->runningCount;

        if (!get_remaining_timeout(deadline, &timeout)) {
            signal_running_children(runtime, SIGKILL);
            break;
        }

        if (sigtimedwait(mask, NULL, &timeout) == -1) {
            if (errno == EAGAIN) {
                signal_running_children(runtime, SIGKILL);
                break;
            } else if (errno != EINTR) {
                log_error("sigtimedwait failed: %s\n", strerror(errno));
                signal_running_children(runtime, SIGKILL);
                break;
            }
            continue;
        }

        reap_finished_children(runtime);
        if (runtime->runningCount < beforeCount) {
            reset_interrupt_deadline(deadline);
        }
    }
}
/**
 * Stops running child processes after litebuild receives SIGINT.
 *
 * @param runtime runtime state containing running child process IDs.
 */
void handle_interrupt_shutdown(RuntimeState* runtime)
{
    struct timespec deadline;
    sigset_t mask;
    bool sentInterrupt;

    if (runtime == NULL) {
        exit(EXIT_FAILURE);
    }

    /* Mark the runtime so no more commands should be started. */
    runtime->interrupted = true;
    sentInterrupt = runtime->runningCount > 0;

    if (!sentInterrupt) {
        exit(EXIT_FAILURE);
    }

    /* Send SIGINT and prepare to wait for child termination. */
    signal_running_children(runtime, SIGINT);
    make_wait_signal_mask(&mask);
    reset_interrupt_deadline(&deadline);
    reap_finished_children(runtime);

    wait_for_interrupted_children(runtime, &mask, &deadline);
    reap_all_children(runtime);

    log_error("execution interrupted by signal\n");
    exit(EXIT_FAILURE);
}
/**
 * Frees memory owned by the runtime state.
 *
 * @param runtime runtime state to free.
 */
void free_runtime_state(RuntimeState* runtime)
{
    if (runtime == NULL) {
        return;
    }

    free(runtime->runningPids);
    runtime->runningPids = NULL;
    runtime->runningCount = 0;
    runtime->runningCapacity = 0;
}
