#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <stdbool.h>
#include <time.h>

#define PROJECT_NAME "litebuild"
#define DEFAULT_MAKEFILE "Litefile"

#ifdef _WIN32
typedef int pid_t;
#else
#include <sys/types.h>
#endif

#include "../utils/vector.h"
#include "../utils/hashmap.h"

enum {
    INITIAL_PID_CAPACITY = 4,
    INTERRUPT_TIMEOUT_SEC = 1,
    DEFAULT_COMPILE_FLAGS_COUNT = 3
};

typedef struct {
    char* makefileName;
    bool simulate;
    StringVector cliVarDefs;
    StringVector requestedTargets;
} CommandLineOptions;

typedef struct {
    char* name;
    StringVector values;
} Variable;

typedef struct {
    char* name;
    StringVector rawTokens;
    int lineNumber;
} VariableDef;

typedef enum {
    TARGET_FILE,
    TARGET_PHONY
} TargetType;

typedef struct {
    StringVector rawDeps;
    StringVector expandedDeps;
    bool depsExpanded;
    int lineNumber;
} FileTarget;

typedef struct {
    StringVector rawPipeline;
    StringVector expandedPipeline;
    bool pipelineExpanded;
    int lineNumber;
} PhonyTarget;

typedef struct {
    TargetType type;
    char* name;
    union {
        FileTarget fileTarget;
        PhonyTarget phonyTarget;
    } data;
} Target;

typedef struct {
    VariableDef* variableDefs;
    int variableDefCount;
    Target* targets;
    int targetCount;
    char* firstTargetName;
    HashMap* targetMap;
} BuildFileModel;

typedef struct {
    HashMap* map;
} VariableTable;

typedef struct {
    bool success;
    bool builtSomething;
    bool targetExistsAsFile;
    bool isPhony;
    struct timespec mtime;
} BuildResult;

typedef struct {
    bool interrupted;
    pid_t* runningPids;
    int runningCount;
    int runningCapacity;
} RuntimeState;

#endif /* CORE_TYPES_H */
