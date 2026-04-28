CC      = gcc
CFLAGS  = -Wall -pthread -O0
TARGETS = race mutex spin

all: $(TARGETS)

race:  01_race.c
	$(CC) $(CFLAGS) -o $@ $<

mutex: 02_mutex.c
	$(CC) $(CFLAGS) -o $@ $<

spin:  03_spinlock.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
