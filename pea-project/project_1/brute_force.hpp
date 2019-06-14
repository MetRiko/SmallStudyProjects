#pragma once

#include <iostream>
#include <string>

#include "matrix.hpp"

template <bool SHOW_RESULT = false, bool DEBUG_MODE = false>
class BruteForceTSP {
	Matrix mat;
	int size;

	int currentMin = -1;
	int *minPath = nullptr;
	int count = 1;
	int maxCount = 1;

public:

	BruteForceTSP(const std::string& filename) {
		mat.load(filename);
		size = mat.getSize();
	}

	BruteForceTSP(const Matrix& source) {
		mat.set(source);
		size = mat.getSize();
	}

	void clear() {
		if(minPath != nullptr) {
			delete[] minPath;
		}
	}

	void start() {
		currentMin = -1;
		count = 0;
		maxCount = 1;

		minPath = new int[size+1];
		int *tab = new int[size]{};
		for(int i=0; i<size; ++i) {
			tab[i] = i;
			maxCount *= i+1;
		}
		_perm(tab, 0, size-1);
		delete[] tab;

		if constexpr(SHOW_RESULT == true) {
			std::cout<<"Sciezka: ";
			for(int i=0; i<size+1; ++i) {
				std::cout<<minPath[i]<<' ';
			}
			std::cout<<"\nKoszt: "<<currentMin<<'\n';			
		}
	}

private:

	void _swap(int *a, int *b) {		
	    int temp = *a; 
	    *a = *b; 
	    *b = temp; 
	}

	void _perm(int *tab, int l, int r) { 
		if (l == r) {
			_calculateCost(tab, size);		
		}
		else { 
			for (int i = l; i <= r; i++) { 
				_swap(tab+l, tab+i); 
				_perm(tab, l+1, r); 
				_swap(tab+l, tab+i);
			}
   			if constexpr(DEBUG_MODE == true) {
				std::cout<<count<<" / "<<maxCount<<'\n';   	
			}	
		} 
	} 

	void _calculateCost(int* tab, int size) {	
		int cost = 0;
		for(int i = 1; i<size; ++i) {
   			// std::cout<<tab[i-1]<< ' ';
   			cost += mat[tab[i-1]][tab[i]];
		}
   		cost += mat[tab[size-1]][tab[0]];

   		// std::cout<<tab[size-1]<< ' ';
   		// std::cout<<tab[0]<< ' ';

   		if constexpr(DEBUG_MODE == true) {
	   		++count;	
   		}

   		if(cost < currentMin || currentMin == -1) {
   			currentMin = cost;
   			for(int i=0; i<size; ++i) {
   				minPath[i] = tab[i];
   			}
   			minPath[size] = tab[0];
   		}

   		// std::cout<<"cost: "<<cost<<'\n';
	}
};