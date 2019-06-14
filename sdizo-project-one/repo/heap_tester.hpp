#pragma once

#include <chrono>
#include <random>

#include "random_number_generator.hpp"

template <typename Structure>
struct HeapTester {

	// Aliases
	using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;

	RandomNumberGenerator rng;
	int capacity = 100;
	Time begin;
	Time end;

	HeapTester() {
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
		}
		avg /= static_cast<float>(tests);
		return avg;
	}

	void run_test(int tests, int capacity, std::ostream& stream) {
		this->capacity = capacity;
		rng.set_range(0, capacity);
		stream<<"\t> insert: "<<get_average(insert, tests)<<" micro s \n";
		stream<<"\t> extract_max: "<<get_average(extract_max, tests)<<" micro s \n";
		stream<<"\t> find: "<<get_average(find, tests)<<" micro s \n";
	}

// push_back
	void insert(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.insert(rng.rand());
		int value = rng.rand();
		start();
		ds.insert(value);
		avg += finish();
	}
// pop_back
	void extract_max(float& avg) {
		Structure ds;
		for(int i=0; i<capacity; ++i) ds.insert(rng.rand());
		start();
		ds.extract_max();
		avg += finish();
	}
// find	
	void find(float& avg) {
		Structure ds;
		int firstPart = capacity/2;
		int secondPart = capacity - firstPart - 1;
		for(int i=0; i<firstPart; ++i) ds.insert(rng.rand());
		int value = rng.rand();
		ds.insert(value);
		for(int i=0; i<secondPart; ++i) ds.insert(rng.rand());
		start();
		ds.find(value);
		avg += finish();
	}
};
