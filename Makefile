CFLAGS = -std=gnu11 -Wall -Werror -Wextra -Wno-unused-parameter
LDLIBS = -lm

.PHONY: all clean

all: scheduling_sim

clean:
	$(RM) scheduling_sim

scheduling_sim: main.c scheduling_sim.c scheduling_utility.c my_array.c
	$(CC) $(CFLAGS) $^ -o scheduling_sim $(LDLIBS)
