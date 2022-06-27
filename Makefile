CC = clang
LDFLAGS = -fuse-ld=lld --rtlib=compiler-rt
LDLIBS = -lm
CFLAGS = -std=c99 -Wall -Wextra -Wconversion -Wshadow\
         -Wdouble-promotion -fno-math-errno -pedantic\
				 -fdelete-null-pointer-checks -fno-common -g \
				 -pipe -march=native -mtune=native
BIN = wrbot
SRC = main.c
OBJ = ${SRC:.c=.o}

all: ${OBJ} ${BIN}
.PHONY: all clean

${OBJ}: ${SRC}
	${CC} ${CFLAGS} -c $<
${BIN} : ${OBJ}
	${CC} ${LDFLAGS} ${OBJ} -o $@ ${LDLIBS}
clean:
	rm -rf *.o ${BIN}
