#include "p2_threads.h"
#include "types_p2.h"
#include "utils.h"

using namespace std;

extern struct timeval t_global_start;
extern class Restroom room;
extern class Restroom queue;

extern pthread_mutex_t person_lock;
extern pthread_mutex_t restroom_lock;

extern std::vector<Person> person;
extern int men, women;

void *input_to_queue(void *parm) {
	struct timeval t_now;
	int status;

	// Inputs people into Queue
	for (int i = 0; i < men*2; i++) {
		int random = rand() % 5 + 1;
		int stayTime = rand() % 8 + 3;

		usleep(MSEC(random));
		// Sets order for being sent into queue and time to stay in bathroom
		person[i].set_order(i+1);
		person[i].set_time(stayTime);
		string sex = (person[i].get_gender() == 0) ? "Man" : "Woman";

		pthread_mutex_lock(&person_lock);
		room.add_person_queue(person[i]);
		gettimeofday(&t_now, NULL);
		printf("[%lu ms][Input] A person (%s) goes into the queue.\n", get_elasped_time(t_global_start, t_now), sex.c_str());
		pthread_mutex_unlock(&person_lock);
	}
	pthread_exit(0);
}

void *input_to_restroom(void *parm) {
	// cout << "Input to Restroom Came Here" << endl;
	int amount_left = 0;
	while (amount_left < men*2) {
		// assert(room.get_status() >= EMPTY || room.get_status() <= MENPRESENT);
		// Adding Women to Restroom
		if (room.get_status() == EMPTY || room.get_status() == WOMENPRESENT) {
			// if (room.get_status() == EMPTY)
			// 	room.set_place("Queue");
			// else
			// 	room.set_place("Restroom");

			for (int i = 0; i < room.people.size(); i++) {
				if (room.people[i].get_gender() == 1)
					room.woman_wants_to_enter(i);
			}
		}

		// Adding Men to Restroom
		if (room.get_status() == EMPTY || room.get_status() == MENPRESENT) {
			for (int i = 0; i < room.people.size(); i++) {
				if (room.people[i].get_gender() == 0)
					room.man_wants_to_enter(i);
			}
		}

		// Removing People from Restroom
		for (int i = 0; i < room.enter.size(); i++) {
			// Remove Women when ready to leave
			if (room.enter[i].get_gender() == 1 && room.enter[i].ready_to_leave() ) {
				room.woman_leaves(i);
				amount_left++;
			} 
			// Remove Men when ready to leave
			else if (room.enter[i].get_gender() == 0 && room.enter[i].ready_to_leave() ) {
				room.man_leaves(i);
				amount_left++;
			} else;
		}
	}

	pthread_exit(0);
}
