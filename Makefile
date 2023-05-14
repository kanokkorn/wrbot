OPENMP = 0

CC = clang
LDFLAGS = -fuse-ld=lld
LDLIBS = -lm
CFLAGS = -std=c99 -Wall -Wextra -Wconversion -Wshadow\
         -Wdouble-promotion -fno-math-errno -pedantic\
				 -fdelete-null-pointer-checks -fno-common -g \
				 -pipe -march=native -mtune=native
BIN = wrbot
SRC != ls *.c
OBJ = ${SRC:.c=.o}

.ifdef OPENMP
CFLAGS += -fopenmp
.endif

all: ${OBJ} ${BIN}
.PHONY: all clean

${OBJ}: ${SRC}
	${CC} ${CFLAGS} -c $<
${BIN} : ${OBJ}
	${CC} ${LDFLAGS} ${OBJ} -o $@ ${LDLIBS}
clean:
	rm -rf *.o ${BIN}
