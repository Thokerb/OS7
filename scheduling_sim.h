#ifndef SCHEDULING_SIM_H
#define SCHEDULING_SIM_H

#include "my_array.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_TIMESTEPS 1000

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

typedef struct {
	int num_processes;
	process_t* procs;
} simulation_data_t;

typedef struct {
	int num_timesteps;
	char proc_names[MAX_TIMESTEPS];
	char* name;
} schedule_t;

typedef process_t* (*scheduling_function)(const int timestep, my_array* list);

#define compute_schedule(procs, sched) compute_schedule_(procs, sched, #sched)

schedule_t compute_schedule_(simulation_data_t* data, scheduling_function scheduler,
                             char* scheduler_name);

int scheduler_rand();

#endif // !SCHEDULING_SIM_H
