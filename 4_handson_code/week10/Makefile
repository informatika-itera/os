CC      = gcc
CFLAGS  = -Wall -pthread -O0
TARGETS = 01_thread_basic 02_race_condition 03_mutex_fix 04_naive_flag

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
