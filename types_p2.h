#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2

class Person {

	int gender; // 0: male 1: female
	int place;
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	long get_time(void);
	int ready_to_leave(void);

	void start(void);
	void complete(void);

	void location(int x);
	int get_location(void);
};


// Class for the restroom
// You may need to add more class member variables and functions
class Restroom {
	int status;
	// std::string place;

	// You need to define the data structure to
    // save the information of people using the restroom
	// You can probably use Standard Template Library (STL) vector

public:
	Restroom(){
		status = EMPTY;
		// place = "";
	}
	std::vector<Person> people;
	std::vector<Person> enter;

	// You need to use this function to print the Restroom's status
	void print_status(void);
	int get_status(void);
	void set_status(int i);

	// void set_place(std::string x);
	// std::string get_place(void);

	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person_queue(Person& p);
	void add_person_restroom(Person& p);
	void woman_wants_to_enter(int x);
	void man_wants_to_enter(int x);
	void woman_leaves(int x);
	void man_leaves(int x);
};

#endif
