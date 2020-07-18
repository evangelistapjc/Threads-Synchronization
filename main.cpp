#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"
#include <time.h>

struct timeval t_global_start;

// Room Initialization, People creation vector
Restroom room;
int men, women;
std::vector<Person> person;

int main(int argc, char** argv) {
	// This is to set the global start time
	int             status = 0;
	int             work = 0;

	gettimeofday(&t_global_start, NULL);
	if (argc == 2 && atoi(argv[1]) > 0) {
		men = women = atoi(argv[1]);

		// Sets gender for every person
		for (int i = 0; i < men*2; i++) {
			Person p;
			p.set_gender( (i % 2) ? 0 : 1 );
			person.push_back(p);
		}

		// Randomizes the order of all people in vector
		srand(time(NULL));
		for (int i = 0; i < person.size(); i++) {
			int j = i + rand() % (person.size() - i);
			std::swap(person[i], person[j]);
		}

		// Begins Threading
		pthread_t tid[2];
		pthread_create(&tid[0], NULL, input_to_queue, NULL);
		pthread_create(&tid[1], NULL, input_to_restroom, NULL);

		for (int i = 0; i < 2; i++) {
			if (pthread_join(tid[i], NULL))
				fprintf(stderr, "Error joining thread\n");
		}

		} else {
		printf("[ERROR] Expecting 1 argument, but got %s.\n", argv[1]);
		printf("[USAGE] p2_exec <number>\n");
	}

	return 0;
}
