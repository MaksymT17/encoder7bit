CC = gcc  # C compiler
CFLAGS = -fPIC -Wall -Wextra -O2 -g  # C flags
LDFLAGS = -shared   # linking flags
TEXT_EXE_FLAGS = 
RM = rm -f   # rm command
TARGET_LIB = lib7bitcoder.so  # target lib

SRCS = 7bitcoder.c  # source files
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)

test: $(OBJ)
	$(CC) 7bitcoder_test.c 7bitcoder.c -o $@ $^ $(CFLAGS)
