
#include <iostream>
#include <random>
#include <time.h>

#define array_width 10
#define array_length 10
#define total_actions 4
#define Gamma 0.6
#define GOAL 100
#define DEAD -100

double R[array_width * array_length] = { DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,
										 DEAD,   1,   1,   1,   1,   1,   1,   1,   1,DEAD,
										 DEAD,   1,   1,   1,   1,DEAD,   1,   1,   1,DEAD,
										 DEAD,   1,   1,   1,   1,   1,DEAD,   1,   1,DEAD,
										 DEAD,   1,   1,   1,   1,   1,   1,   1,   1,DEAD,
							             DEAD,   1,   1,DEAD,DEAD,DEAD,DEAD,DEAD,   1,DEAD,
										 DEAD,   1,   1,   1,   1,   1,   1,   1,   1,DEAD,
										 DEAD,   1,DEAD,   1,DEAD,   1,DEAD,   1,   1,DEAD,
										 DEAD,   1,   1,   1,   1,   1,   1,   1,GOAL,DEAD,
										 DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD
										};

double Q[array_width * array_length][total_actions] = { 0 };


int init_state = array_length+1;
int current_state = init_state;
int action = 0; // 0 up, 1 down, 2 lef, 3 right

double get_MAX_Q(int next_state)
{
	double local_max_Q = 0;

	for (int loop = 0; loop < total_actions; loop++)
	{
		if (Q[next_state][loop] > local_max_Q)
			local_max_Q = Q[next_state][loop];
	}
	return local_max_Q;
}

int get_action_of_MAX_Q(int next_state) // actually the parameter should be curret_state, not next_state.
{
	double local_max_Q = 0;
	int local_action = -1;

	for (int loop = 0; loop < total_actions; loop++)
	{
		if (Q[next_state][loop] > local_max_Q)
		{
			local_max_Q = Q[next_state][loop];
			local_action = loop;
		}
		else if (Q[next_state][loop] == local_max_Q )
		{
			int r = (((double)rand()*10 / RAND_MAX));
			if (r > 5)
			{
				local_action = loop;
			}
		}
	}

	if (local_action > -1)
		return local_action;
	else
		return ((double)rand() / (RAND_MAX)) * total_actions;

}

void print_R_table()
{
	for (int y = 0; y < array_length; y++)
	{
		for (int x = 0; x < array_width; x++)
			printf("%5d", R[y * array_length + x]);
		printf("\n");
	}
}

void print_Q_table()
{
	for (int y = 0; y < array_length* array_width; y++)
	{
		printf("\nstate %3d (%3d,%3d) =",y,y / array_width, y % array_width);
		for (int x = 0; x < total_actions; x++)
			printf(" %10f ", Q[y][x]);
	}
}

void print_state()
{
	for (int y = 0; y < array_length; y++)
	{
		for (int x = 0; x < array_width; x++)
		{
			if ((y * array_length + x) == current_state)
				printf("X");
			else if (R[y * array_length + x] == DEAD)
				printf("#");
			else if (R[y * array_length + x] == GOAL)
				printf("$");
			else if (R[y * array_length + x] == 1)
				printf(" ");
		}
		printf("\n");
	}
}
int main()
{
	srand(time(0));
	double r = ((double)rand() / (RAND_MAX)) ;
	int used_step = 0;
	int WIN = 0;
	int round = 0;

	//print_R_table();
	//print_state();
	//print_Q_table();

	for (int step = 0; step < 400000; /*step++*/)
	{
		int old_state = current_state;
		int explore_or_experience = ((double)rand() / (RAND_MAX)) * 10;
		
		if (Q[old_state][0] == 0)
			action = 0;
		else if (Q[old_state][1] == 0)
			action = 1;
		else if (Q[old_state][2] == 0)
			action = 2;
		else if (Q[old_state][3] == 0)
			action = 3;
		else if (WIN > 0 || explore_or_experience > 1)
			action = get_action_of_MAX_Q(old_state);
		else
			action = ((double)rand() / (RAND_MAX)) * total_actions;

		used_step++;
		//if (WIN > 0)
		{
			//printf("Current:(%d,%d)\n", old_state/array_length, old_state%array_length);
			//print_state();
		}

		//print_Q_table();
	    //printf("\ncurrent_state=%d actoin = %d ", current_state, action);
		switch (action)
		{
		case 0:
			if (current_state - array_length > -1)
				current_state -= array_length;
			break;
		case 1:
			if (current_state + array_length < array_width * array_length)
				current_state += array_length;
			break;
		case 2:
			if ((current_state - 1 > -1) && (current_state % array_width != 0))
				current_state -= 1;
			break;
		case 3:
			if (((current_state + 1) % array_width != array_width-1) && (current_state + 1 < array_width * array_length))
				current_state += 1;
			break;
		}
		//print_state();

		// Q(state, action) = R(state, action) + Gamma * Max[Q(next state, all actions)]
		Q[old_state][action] = R[current_state] + Gamma * get_MAX_Q(current_state);

		if (R[current_state] == DEAD)
		{
			current_state = init_state;
			//used_step = 0;
			//printf(".......DEAD\n");
			//round++;
		}
			
		else if (R[current_state] == GOAL ) // When reach GOAL or DEAD, restart game
		{
			//printf("current_state=%d actoin = %d\n", current_state, action);
			current_state = init_state;
			if (round++ >500)
				break;
			
			//print_Q_table();
			printf("\nGoal %d, used step = %d ", round, used_step);
			if (used_step == 14)
			{
				//printf("Total tranning = %d \n ", step);;
				if(++WIN>5)
					break;
			}
			
			used_step = 0;
		}
	}
	print_Q_table();
}
