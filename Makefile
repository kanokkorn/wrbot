# Compiler and flags
CC      ?= gcc
CFLAGS  += -Wall -Wextra -g -std=c99
LDFLAGS +=
LDLIBS  += -lm
PREFIX  ?= /usr/local

# Project files
PROG = wrbot
SRCS = main.c navigate.c pathd.c msgd.c comm.c
OBJS = ${SRCS:.c=.o}

# Targets
all: ${PROG}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LDLIBS}

# Phony targets
.PHONY: all clean install uninstall

clean:
	rm -f ${PROG} ${OBJS}

install: all
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 ${PROG} ${DESTDIR}${PREFIX}/bin

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${PROG}
