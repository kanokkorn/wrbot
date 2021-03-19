TARGET_HW  = 0
ADV_METHOD = 0

CC = cc
LIBS = -lm
CFLAGS = -std=c89 -Wall -Wextra -Os -s -ansi -pedantic -fno-math-errno -fdelete-null-pointer-checks
PROJECT = wrbot

ifeq ($(TARGET_HW), 1)
  CFLAGS += -DTARGET_HW
endif
ifeq ($(ADV_METHOD), 1)
  CFLAGS += -DADV_METHOD
endif

$(PROJECT): main.c
	$(CC) $(LIBS) $(CFLAGS) $< -o $@
clean:
	rm $(PROJECT)

.SILENT : clean
