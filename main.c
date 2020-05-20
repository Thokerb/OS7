#include <math.h>
#include <stdio.h>

#include "scheduling_sim.h"
#include "scheduling_utility.h"

// ---------------------------------------------------------------------------
// Example schedulers
// ---------------------------------------------------------------------------

process_t* fcfs(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);
	return selected;
}

process_t* round_robin(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);
	// We are allowed to delete the scheduled process from the list.
	// It will be re-appended at the end (after any newly arrived processes).
	my_array_delete(processes, 0);
	return selected;
}

// ---------------------------------------------------------------------------
// Implement your schedulers here
// ---------------------------------------------------------------------------
process_t* shortest_time_next(int timestep, my_array* processes){
	size_t count = my_array_size(processes);
	process_t* biggest = my_array_get(processes,0);
	process_t* selected;
	for (size_t i = 0; i < count; i++)
	{
		selected = my_array_get(processes,i);
		if(selected->remaining_time <  biggest->remaining_time){
			biggest = selected;
		}
		else
		{
			//we might not need this, because array is already ordered after arrival time....
			if(selected->remaining_time == biggest->remaining_time){
				if(selected->arrival_time < biggest->arrival_time){
					biggest = selected;
				}
			}
		}
	}
	return biggest;
}

process_t* round_robin4(int timestep, my_array* processes){
	process_t* selected = my_array_get(processes,0);

	if(selected->user1%4 == 0 && selected->user1 != 0){
		selected->user1 = 0;
		my_array_push_back(processes,selected);
		my_array_delete(processes,0);
		return my_array_get(processes,0);
	}
	selected->user1++;
	return selected;
}

process_t* priority_withPre(int timestep, my_array* processes){
	size_t count = my_array_size(processes);
	process_t* biggest = my_array_get(processes,0);
	process_t* selected;
	for (size_t i = 0; i < count; i++)
	{
		selected = my_array_get(processes,i);
		if(selected->priority >  biggest->priority){
			biggest = selected;
		}
		else
		{
			//we might not need this, because array is already ordered after arrival time....
			if(selected->remaining_time == biggest->remaining_time){
				if(selected->arrival_time < biggest->arrival_time){
					biggest = selected;
				}
			}
		}
	}
	return biggest;
}

process_t* priority_withoutPre(int timestep, my_array* processes){
	size_t count = my_array_size(processes);
	process_t* biggest = my_array_get(processes,0);
	for (size_t i = 0; i < count; i++){
		process_t* proc = my_array_get(processes,i);
		if(proc->user1){
			return proc;
		}
	}
	process_t* selected;
	for (size_t i = 0; i < count; i++)
	{
		selected = my_array_get(processes,i);
		if(selected->priority >  biggest->priority){
			biggest = selected;
		}
		else
		{
			//we might not need this, because array is already ordered after arrival time....
			if(selected->remaining_time == biggest->remaining_time){
				if(selected->arrival_time < biggest->arrival_time){
					biggest = selected;
				}
			}
		}
	}
	biggest->user1 = 1;
	return biggest;
}

process_t* lottery(int timestep, my_array* processes){	
	int shouldBreak = scheduler_rand(0,1);
	size_t numberProcesses = my_array_size(processes);
	int numberTickets = 0;
	int tickets = 0;
	int reCalculate = 0;
	int foundProcessor = 0;
	process_t* activeProc;

	for (size_t i = 0; i < numberProcesses; i++)
	{
		process_t* proc = my_array_get(processes,i);

		if(proc->user2 > 0){
			foundProcessor = 1;
			activeProc = proc;
			//reached end of quantum
			if(proc->user2 == 3){
				proc->user2 = 0;
				reCalculate = 1;
				break;
			}

		}
	}

	if(shouldBreak && foundProcessor == 1 && reCalculate == 0){
		activeProc->user3 = activeProc->user2;
		activeProc->user2 = 0;
		reCalculate = 1;
	}


	if(foundProcessor == 0 || reCalculate == 1){

	}
	else
	{
		printf("foundProc %d reCalc %d shouldBreak %d winner: %c \n",foundProcessor,reCalculate,shouldBreak,activeProc->name);
		activeProc->user2++;
		return activeProc;	
	}

	printf("lottery foundProc %d reCalc %d shouldBreak %d \t",foundProcessor,reCalculate,shouldBreak);

	//set tickets
	for (size_t i = 0; i < numberProcesses; i++)
	{
		process_t* proc = my_array_get(processes,i);
		tickets = roundf(30/proc->service_time); 
		if(proc->user3 != 0){
			//has some bonus tickets
			tickets += roundf((1.0/proc->user3) * tickets );
		}
		proc->user1 = tickets;
		numberTickets += tickets;
	}

	//draw ticket
	int winningNo = scheduler_rand(0,numberTickets);
	printf("winningNo: %d [0 - %d] \t",winningNo,numberTickets);

	//find winner
	for (size_t i = 0; i < numberProcesses; i++)
	{
		process_t* proc = my_array_get(processes,i);
		winningNo -= proc->user1;
		printf("%d,",winningNo);
		if(winningNo <= 0){
			//winner
			//set quantum number to 1
			proc->user2 = 1;
			printf("winner is %c \n",proc->name);
			proc->user3 = 0;
			return proc;
		}
	}
	printf("CRITICAL ERROR; NO WINNER FOUND");
	return my_array_get(processes,0);

}
// ---------------------------------------------------------------------------

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "Error: Usage: %s <filename>\nExample: %s input.csv\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	simulation_data_t* data = read_simulation_data(argv[1]);
	if(!data) {
		fprintf(stderr, "Error: Unable to open the file '%s' for reading!\n", argv[1]);
		return EXIT_FAILURE;
	}
	print_simulation_data(stdout, data);

	// -----------------------------------------------------------------------
	// Insert calls to your schedulers here
	// -----------------------------------------------------------------------

	// print_schedule(stdout, compute_schedule(data, fcfs), data);
	// print_schedule(stdout, compute_schedule(data, round_robin), data);
	// print_schedule(stdout, compute_schedule(data, shortest_time_next), data);
	// print_schedule(stdout, compute_schedule(data, round_robin4), data);
	// print_schedule(stdout, compute_schedule(data, priority_withPre), data);
	 print_schedule(stdout, compute_schedule(data, priority_withoutPre), data);
	// print_schedule(stdout, compute_schedule(data, lottery), data);


	// -----------------------------------------------------------------------

	free_simulation_data(data);

	return EXIT_SUCCESS;
}
