# litebuild - A lightweight build system

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=gnu99 -g -O2
LDFLAGS = 

TARGET  = litebuild
SRCS    = main.c frontend/cli.c lexer.c frontend/parser.c core/variable.c engine/build.c engine/exec.c utils/memory.c utils/logger.c utils/vector.c utils/hashmap.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
