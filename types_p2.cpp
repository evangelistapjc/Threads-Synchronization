#include "types_p2.h"
#include "utils.h"

pthread_mutex_t person_lock 	= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t restroom_lock 	= PTHREAD_MUTEX_INITIALIZER;
extern struct timeval t_global_start;

extern std::vector<Person> person;

void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::location(int x) 	{ place = x; }
int Person::get_location(void) 	{ return place; }

void Person::set_time(long data) { time_to_stay_ms = data; }
long Person::get_time(void) { return time_to_stay_ms; }

int Person::ready_to_leave(void) {
	struct timeval t_now;
	gettimeofday(&t_now, NULL);

	if (get_elasped_time(t_start, t_now) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}

// Queue to Restroom
void Person::start(void) {
	std::string name = (get_gender() == 0) ? "Man" : "Woman";
	gettimeofday(&t_start, NULL);
	printf("[%lu ms][Restroom] (%s) goes into the Restroom. ", get_elasped_time(t_global_start, t_start), name.c_str());
}

// Finished in Restroom
void Person::complete(void) {
	std::string name = (get_gender() == 0) ? "Man" : "Woman";
	gettimeofday(&t_end, NULL);
	printf("[%lu ms][Restroom] (%s) left the Restroom. ", get_elasped_time(t_global_start, t_end), name.c_str());
}

Person::Person() {
	gettimeofday(&t_create, NULL);
}

void Restroom::print_status(void) {
	int x = 0;
	int y = 0;
	// Determines size of either men or women
	for (int i = 0; i < enter.size(); i++) {
		if (enter[i].get_gender() == 0)
			x++;
		else
			y++;
	}
	std::string sex = (x > y) ? "MENPRESENT" : ( (x < y) ? "WOMENPRESENT" : "EMPTY");
	int total = (x > y) ? x : ( (x < y) ? y : 0);
	printf("State is: (%s) Total: %d (Men: %d, Women: %d)\n", sex.c_str(), total, x, y);
}

void Restroom::add_person_queue(Person& p) 	{ people.push_back(p); }
void Restroom::add_person_restroom(Person& p) 	{ enter.push_back(p); }
int Restroom::get_status(void) 			{ return status; }
void Restroom::set_status(int i) 		{ status = i; }

void Restroom::woman_wants_to_enter(int x) {
	// assert(people[x].get_gender() == 1);

	// Add Woman into Restroom
	pthread_mutex_lock(&restroom_lock);
	status = WOMENPRESENT;
	enter.push_back(people[x]);
	enter[enter.size() - 1].start();
	print_status();
	pthread_mutex_unlock(&restroom_lock);

	// Remove Woman from Queue
	pthread_mutex_lock(&person_lock);
	people.erase(people.begin());
	pthread_mutex_unlock(&person_lock);
}

void Restroom::man_wants_to_enter(int x) {
	// assert(people[x].get_gender() == 0);

	// Add Man into Restroom
	pthread_mutex_lock(&restroom_lock);
	status = MENPRESENT;
	enter.push_back(people[x]);
	enter[enter.size() - 1].start();
	print_status();
	pthread_mutex_unlock(&restroom_lock);

	// Remove Man from Queue
	pthread_mutex_lock(&person_lock);
	people.erase(people.begin());
	pthread_mutex_unlock(&person_lock);
}

void Restroom::woman_leaves(int x) {
	// Remove Woman from Restroom
	pthread_mutex_lock(&restroom_lock);
	// enter.erase(enter.begin() + x - 1);
	enter.erase(enter.begin() + x);
	enter[x].complete();
	if (enter.size() == 0)
		status = EMPTY;
	pthread_mutex_unlock(&restroom_lock);

	print_status();
}

void Restroom::man_leaves(int x) {
	// Remove Woman from Restroom
	pthread_mutex_lock(&restroom_lock);
	// enter.erase(enter.begin() + x - 1);
	enter.erase(enter.begin() + x);
	enter[x].complete();
	if (enter.size() == 0)
		status = EMPTY;
	pthread_mutex_unlock(&restroom_lock);
	
	print_status();
}
