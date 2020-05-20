#include "scheduling_sim.h"
#include "my_array.h"
#include "scheduling_utility.h"

#include <assert.h>
#include <math.h>

static void init_processes(simulation_data_t* data) {
	assert(data);
	for(int i = 0; i < data->num_processes; ++i) {
		process_t* cur = data->procs + i;
		*(int*)(&cur->remaining_time) = cur->service_time;
		cur->user1 = 0;
		cur->user2 = 0;
		cur->user3 = 0;
	}
}

static int _sched_rng_state;

int scheduler_rand(int min, int max) {
	assert(max > min);
	srand(_sched_rng_state);
	float r = rand() / (float)RAND_MAX;
	return roundf(min + r * (max - min));
}

schedule_t compute_schedule_(simulation_data_t* data, scheduling_function scheduler,
                             char* scheduler_name) {
	assert(data);
	assert(scheduler);
	assert(scheduler_name);

	init_processes(data);
	schedule_t sched;
	int timestep = 0;

	my_array* list = my_array_init();
	process_t* prev_proc = NULL;

	process_t* current = NULL;
	for(; timestep < MAX_TIMESTEPS - 1; ++timestep) {
		_sched_rng_state = timestep;

		for(size_t i = 0; i < my_array_size(list); ++i) {
			process_t* p = my_array_get(list, i);
			if(!p->remaining_time) {
				my_array_delete(list, i--);
			}
		}
		for(int i = 0; i < data->num_processes; ++i) {
			process_t* p = data->procs + i;
			if(timestep == p->arrival_time) {
				my_array_push_back(list, p);
			}
		}
		if(prev_proc) {
			my_array_push_back(list, prev_proc);
			prev_proc = NULL;
		}

		// if nothing left to schedule we're done
		if(my_array_size(list) == 0) {
			break;
		}

		size_t size_pre = my_array_size(list);
		current = scheduler(timestep, list);
		assert(current != NULL);
		size_t size_post = my_array_size(list);

		int* rem_time = (int*)(&current->remaining_time);
		(*rem_time)--;

		// Do not add processes and do not remove more than one process!
		assert(size_post == size_pre || size_post + 1 == size_pre);

		if(size_pre == size_post + 1 && current->remaining_time > 0) {
			prev_proc = current;
#if !defined(NDEBUG)
			for(size_t i = 0; i < my_array_size(list); ++i) {
				assert(my_array_get(list, i) != current &&
				       "Only the scheduled process can be erased from the list");
			}
#endif
		}

		// record the schedule
		sched.proc_names[timestep] = current->name;
	}

	sched.num_timesteps = timestep;
	sched.proc_names[timestep] = '\0';
	sched.name = scheduler_name;

	my_array_free(list);
	return sched;
}