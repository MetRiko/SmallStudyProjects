#pragma once

#include <vector>
#include <chrono>
#include <random>

struct RandomNumberGenerator {

	using Time = std::chrono::time_point<std::chrono::high_resolution_clock >;

	// RNG generator
	std::mt19937 generator;
	std::uniform_int_distribution<int> dis;
	// std::random_device rd;
	
	// Values for tests
	std::vector<int> values;

	// Time points
	Time timeBegin;
	Time timeEnd;

	RandomNumberGenerator() 
		: generator(std::chrono::high_resolution_clock::now().time_since_epoch().count()), dis(0,1) {
	}

	void set_range(int from, int to) {
		dis = std::uniform_int_distribution<int>(from, to);
	}

	int rand() {
		return dis(generator);
	}

	int rand(int from, int to) {
		std::uniform_int_distribution<int> dis(from, to);
		return dis(generator);
	}

};
