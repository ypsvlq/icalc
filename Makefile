.POSIX:

CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g -MMD -MP
LD = $(CC)
LDFLAGS =
LIBS =

src = src/main.c src/menu.c src/input.c
obj = $(src:.c=.o)
dep = $(src:.c=.d)

.PHONY: all clean run

all: icalc

clean:
	rm -f $(obj) $(dep) icalc

run: icalc
	./icalc

icalc: $(obj)
	$(LD) $(LDFLAGS) -o $@ $(obj) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(dep)
