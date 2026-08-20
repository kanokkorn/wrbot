CC      ?= gcc
CFLAGS  += -Wall -Wextra -g -std=c99
LDLIBS  += -lm
PREFIX  ?= /usr/local

PROG = wrbot
SRCS = main.c navigate.c pathd.c msgd.c comm.c fsm.c
OBJS = ${SRCS:.c=.o}

all: ${PROG}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LDLIBS}

.PHONY: all clean install uninstall

clean:
	rm -f ${PROG} ${OBJS}

install: all
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 ${PROG} ${DESTDIR}${PREFIX}/bin

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${PROG}
