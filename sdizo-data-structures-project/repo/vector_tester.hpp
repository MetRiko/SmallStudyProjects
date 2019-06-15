#pragma once

#include <chrono>
#include <random>

#include "random_number_generator.hpp"

template <typename Structure>
struct VectorTester {

	// Aliases
	using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;

	RandomNumberGenerator rng;
	int capacity = 100;
	Time begin;
	Time end;

	VectorTester() {
		rng.set_range(0, 10000);
	}
	void start() {
		begin = std::chrono::high_resolution_clock::now();
	}
	float finish() {
		end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/1000.f;
	}
	float get_average(auto testerFunc, int tests) {		
		float avg = 0;
		for(int i=0; i<tests; ++i) {
			(this->*testerFunc)(avg);
			// std::cout<<avg<<' ';
		}
		avg /= static_cast<float>(tests);
		return avg;
	}

	void run_test(int tests, int capacity, std::ostream& stream) {
		this->capacity = capacity;
		rng.set_range(0, capacity);
		stream<<"\t> push_back: "<<get_average(push_back, tests)<<" micro s \n";
		stream<<"\t> push_front: "<<get_average(push_front, tests)<<" micro s \n";
		stream<<"\t> push: "<<get_average(push, tests)<<" micro s \n";
		stream<<"\t> pop_back: "<<get_average(pop_back, tests)<<" micro s \n";
		stream<<"\t> pop_front: "<<get_average(pop_front, tests)<<" micro s \n";
		stream<<"\t> pop: "<<get_average(pop, tests)<<" micro s \n";
		stream<<"\t> find: "<<get_average(find, tests)<<" micro s \n";
	}

// push_back
	void push_back(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.push_back(rng.rand());
		int value = rng.rand();
		start();
		ds.push_back(value);
		avg += finish();
	}
// push_front
	void push_front(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.push_back(rng.rand());
		int value = rng.rand();
		start();
		ds.push_front(value);
		avg += finish();
	}
// push	
	void push(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.push_back(rng.rand());
		int value = rng.rand();
		int pos = rng.rand(0, ds.get_size()-1);
		start();
		ds.push(value, pos);
		avg += finish();
	}
// pop_back
	void pop_back(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.push_back(rng.rand());
		start();
		ds.pop_back();
		avg += finish();
	}
// pop_front
	void pop_front(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.push_back(rng.rand());
		start();
		ds.pop_front();
		avg += finish();
	}
// pop	
	void pop(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.push_back(rng.rand());
		int pos = rng.rand(0, ds.get_size()-1);
		start();
		ds.pop(pos);
		avg += finish();
	}
// find	
	void find(float& avg) {
		Structure ds;
		int firstPart = capacity/2;
		int secondPart = capacity - firstPart - 1;
		for(int i=0; i<firstPart; ++i) ds.push_back(rng.rand());
		int value = rng.rand();
		ds.push_back(value);
		for(int i=0; i<secondPart; ++i) ds.push_back(rng.rand());
		start();
		ds.find(value);
		avg += finish();
	}
};
