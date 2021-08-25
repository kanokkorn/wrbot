TARGET_HW  = 0
ADV_METHOD = 0

CC = cc
LIBS = -lm -lcurses
CFLAGS = -std=c89 -Wall -Wextra -Wconversion \
         -O3 -g -pedantic -fno-math-errno \
         -fdelete-null-pointer-checks
OBJ = main.o
SRC = main.c
BIN = wrbot

ifeq ($(TARGET_HW), 1)
  CFLAGS += -DTARGET_HW
	SRC += interface.c
	OBJ += interface.o
endif
ifeq ($(ADV_METHOD), 1)
  CFLAGS += -DADV_METHOD
	SRC += kalman.c
	OBJ += kalman.o
endif

ALL: $(OBJ) $(BIN)
.PHONY = ALl

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $^ -c
$(BIN) : $(OBJ)
	$(CC) $^ -o $@ $(LIBS)
clean:
	$(RM) *.o $(BIN)

.SILENT : clean
