# Exercise 8

This week we are revisiting _scheduling algorithms_. Instead of computing
a schedule using pen & paper however, the different algorithms are to be
implemented in C. To this end, we provide a _scheduler simulation framework_,
which will be explained in more detail in the next section.

**IMPORTANT**: Read the entire exercise carefully before proceeding. Part of
the goal is to learn programming within the confines of a given framework,
while adhering to a specified behavior.

## Scheduler Simulation Framework

The scheduler simulation framework is provided in the files alongside this
documentation. Run `make && ./scheduling_sim input.csv` to try it out.

The basic operation of the simulation framework is as follows:

- First, a list of processes (including all their attributes, such as arrival
  time, priority, and so on) is read from a given CSV file.
- The framework begins its simulation at time step 0, and runs until all
  processes have been serviced.
- For each time step, the framework calls a
  **user provided scheduling function** that decides which process should run
  for the next time step.
  - Only processes that can already be executed and have not finished
    are provided to the scheduling function, i.e.
    `time step >= process.arrival_time` and `process.remaining_time > 0`.
- The process that was returned has its remaining time decremented by 1.
- The simulation stops once all processes have no more time remaining.

The first file you should inspect is [main.c](main.c). From here, the
simulation framework is called using the different scheduling algorithms.
**This is also the place where you should implement your own schedulers**.
It is **NOT ALLOWED** to modify any other files.

We've provided reference implementations of two scheduling algorithms for
you: **first come first serve** scheduling, and **round robin** (with quantum
= 1) scheduling. Make sure you fully understand these two implementations
before you proceed to implementing your own.

A scheduling function has the following signature:

```c
process_t *(*scheduling_function)(int timestep, my_array* processes)
```

For each time step, the scheduling function receives the current `timestep`,
as well as a list of all viable (= ready to be run) processes at this point
in time. The framework then expects one of the processes specified in
`processes` to be returned from the function.

Processes are described by the following struct, defined in
[scheduling_sim.h](scheduling_sim.h):

```c
typedef struct {
  const char name;
  const int arrival_time;
  const int service_time;
  const int priority;
  const int remaining_time;

  int user1;
  int user2;
  int user3;
} process_t;
```

A dynamic array (`my_array`) that stores elements of type `process_t` is
provided to your scheduling function. See [my_array.h](my_array.h) to get an
idea of how it can be used. Generally, you must not delete processes from
this list, unless you insert them again before returning from your scheduling
function. **There is however an exception**: If you want to have your
_currently scheduled_ process to be re-appended at the very back of the list,
i.e., after any potential new arrivals, you may delete it from the array. See
the provided implementation of `round_robin` for an example of this.

The `process_t` struct describes a single process. Most of its fields should
be self-explanatory, however there are a few that warrant further explanation.
First, note that each process has a single character `name`, which is used
during the output and which might also be useful for debugging.

Most of the member variables of `process_t` are marked as `const`, indicating
that they should not be modified by your scheduler. However, there are three
non-const `int` variables `user1, user2, user3` that can be used to store
**anything you want**. This is often times useful, when you need to retain
state between calls to your scheduler function. You can assume these
variables to be initialized to zero.

## Example Output

In the file [example_output.log](example_output.log) you can find the output
produced by our own reference implementations for each algorithm. Feel free to
use this as a guide for what output your system might produce. Note that
algorithms that use randomness (i.e., lottery scheduling) might produce
different results on your machine (depending on your OS, C standard library
version and so on).

Here you can see the example output produced for our FCFS implementation:

```
===================================        fcfs        ===================================

    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
A:  x   x   x   x   x   x   x
B:              *               x   x   x
C:                  *                       x   x
D:                          *                       x   x   x   x   x
E:                                  *                                   x   x
```

For each time step, the letter indicates the process name (`process_t.name`)
that was returned by the scheduling function. An `x` means that a process has
been scheduled at this time step. A `*` indicates that a process has arrived
at this time step (if it is scheduled immediately, an `x` will be printed
instead).

**DISCLAIMER**: We do not want to guarantee the correctness of our own
implementations. There might very well be cases where your implementation
produces different results (for example, due to some ambiguity in the
algorithm's specification). Please take note of any such cases and try to
justify why your output is correct.

## Additional Notes

- If two processes are equally eligible for scheduling (e.g., have the same
  priority), fall back to first-come-first-served.
- All state (if any) should be stored using the user data fields, i.e., your
  schedulers must not use any global variables.

---

## Task 1

- Implement **shortest remaining time next** scheduling.
- Implement **round robin** scheduling with a quantum of 4.

## Task 2

- Implement **priority** scheduling **with preemption**.
- Implement **priority** scheduling **without preemption**.

## Task 3

- Implement **lottery** scheduling, as described in the lecture.
  - Each process receives `30 / service_time` tickets.
  - Assume a quantum of 3.
  - Use the `scheduler_rand(a, b)` function to generate random numbers in the
    range [a,b]. You must NOT use `rand()` or any other random number generator.
  - For each time step, decide with 50% probability (again using
    `scheduler_rand()`) whether a process will yield early, i.e., not use up
    its entire quantum. In this case, make sure to compensate the process
    that yielded early with _compensation tickets_, as described in the
    lecture. Note that non-integer results should be correctly rounded (using
    `roundf()`).
