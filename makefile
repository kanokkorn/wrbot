TARGET_HW  = 0
ADV_METHOD = 0

cc = clang
ld = lld
libs = -lm -lcurses
cflags = -std=c89 -Wall -Wextra -Wconversion -Werror\
         -Wdouble-promotion -Wshadow  -fno-math-errno\
				 -fdelete-null-pointer-checks -fno-common\
				 -Os -g3 -pedantic

bin = wrbot
src = main.c
obj = $(patsubst %.c, %.o, $(src))

ifeq ($(TARGET_HW), 1)
  cflags += -DTARGET_HW
	src += interface.c
	obj += interface.o
endif
ifeq ($(ADV_METHOD), 1)
  cflags += -DADV_METHOD
	src += kalman.c
	obj += kalman.o
endif

all: $(obj) $(bin)
.PHONY: all clean

$(obj): $(src)
	$(cc) $(cflags) $^ -c
$(bin) : $(obj)
	$(cc) -fuse-ld=$(ld) $^ -o $@ $(libs)
clean:
	$(RM) *.o $(bin)
