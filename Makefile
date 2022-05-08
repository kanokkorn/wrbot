TARGET_HW  = 0
ADV_METHOD = 0

CC = clang
LD = -fuse-ld=lld --rtlib=compiler-rt
LDLIBS = -lm
CFLAGS = -std=c99 -Wall -Wextra -Wconversion -Wshadow\
         -Wdouble-promotion -fno-math-errno -pedantic\
				 -fdelete-null-pointer-checks -fno-common\
				 -g

BIN = wrbot
SRC = main.c
OBJ = $(SRC:.c=.o)

ifeq ($(TARGET_HW), 1)
  CFLAGS += -target aarch64 -mfpu=neon -mfloat-abi=softfp\
						-integrated-as -DTARGET_HW
endif
ifeq ($(ADV_METHOD), 1)
  CFLAGS += -DADV_METHOD
	SRC += kalman.c
endif

all: $(OBJ) $(BIN)
.PHONY: all clean

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $^ -c
$(BIN) : $(OBJ)
	$(CC) $(LD) $^ -o $@ $(LDLIBS)
clean:
	$(RM) *.o $(BIN)
