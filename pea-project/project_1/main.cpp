#include <iostream> // std::cout

#include <fstream> // std::ifstream
#include <limits> // For max value of 'int' type
#include <cmath> // std::floor()

#include <chrono> // Measuring time

#include <cstdlib> // std::rand()
#include <ctime> // std::time(nullptr) for std::srand(...)

#include "heap.hpp"
#include "unordered_vector.hpp"
#include "matrix.hpp"
#include "brute_force.hpp"
#include "branch_and_bound.hpp"


int main() {
	
	while(true) {

		std::string filename;
		std::cout<<"---- TSP ----\n";
		std::cout<<"1. Brute force\n";
		std::cout<<"2. Branch and bound\n";
		std::cout<<"3. BF oraz B&B\n";
		std::cout<<"4. Wyjscie\n\n";

		std::cin>>filename;
		if(filename[0] == '1') {
			std::cout<<"Podaj sciezke do pliku (bez spacji): ";
			std::cin>>filename;
			BruteForceTSP<true> tsp(filename);
			tsp.start();
		}
		else if(filename[0] == '2') {
			std::cout<<"Podaj sciezke do pliku (bez spacji): ";
			std::cin>>filename;
			BxBTSP<true> tsp(filename);
			tsp.start();
		}
		else if(filename[0] == '3') {
			std::cout<<"Podaj sciezke do pliku (bez spacji): ";
			std::cin>>filename;
			std::cout<<"Brute force:\n";
			BruteForceTSP<true> tsp1(filename);
			tsp1.start();
			std::cout<<"Branch and bound:\n";
			BxBTSP<true> tsp2(filename);
			tsp2.start();
		}
		else if(filename[0] == '4') {
			break;
		}
		std::cout<<"Nacisnij [Enter] aby kontynuowac...\n";
		std::cin.get();
		std::cin.get();
	}


	// using namespace std::chrono;
	// std::srand(std::time(nullptr));

	// for(int n=6; n<=12; ++n) {
	// 	float avg = 0;
	// 	for(int i=0; i<100; ++i) {


	// 		Matrix mat;
	// 		mat.generateRandomMatrix(n);
	// 		// BruteForceTSP tsp(mat);
	// 		BxBTSP tsp(mat);

	// 		high_resolution_clock::time_point t1 = high_resolution_clock::now();
	// 		tsp.start();
	// 	    high_resolution_clock::time_point t2 = high_resolution_clock::now();

	// 	    avg += duration_cast<nanoseconds>( t2 - t1 ).count();

	// 	}
	// 	std::cout << n << ": " << std::floor(avg/20.0*100.0)*0.01 << '\n';
	// }

}