CC = clang
LDLIBS = -lm
# SRC != ls *.c
# OBJ = ${SRC:.c=.o}
SRC = nmain.c
OBJ = nmain.o
BIN = wrbot

all: ${OBJ} ${BIN}
.PHONY: all clean

${OBJ}: ${SRC}
	${CC} -c $< -g
${BIN} : ${OBJ}
	${CC} ${LDFLAGS} ${OBJ} -o $@ ${LDLIBS} -static
release : ${OBJ}
	${CC} -c ${SRC} && ${CC} -fuse-ld=lld ${OBJ} -static -o ${BIN} ${LDLIBS}
clean:
	rm -rf *.o ${BIN}
