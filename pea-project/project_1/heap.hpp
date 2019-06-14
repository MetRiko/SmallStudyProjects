#pragma once

#include <iostream>
#include <iomanip>

namespace sdizo {
	struct MinHeap { // Max type
		using Type = int;
		
		Type *tab = nullptr;
		int size = 0;
		
		~MinHeap() {
			clear();
		}
		
		void clear() {
			delete[] tab;
			tab = nullptr;
			size = 0;
		}			

		Type* find(int value) {
			if(!tab) return nullptr;
			Type* itr = tab;
			Type* end = tab+size;
			while(itr<end) {
				if(*itr == value) return itr;
				++itr;
			}
			return nullptr;
		}
		
		template<typename T>
		void swap(T& a, T& b) {
			T x = a;
			a = b;
			b = x;
		}

		void push(Type element) {
			if(tab) {
				// push_back
				Type *newTab = new Type[size+1];
				
				Type *itr = tab;
				Type *itr_2 = newTab;
				int counter = size;
				while(counter--) {
					*itr_2 = *itr;
					++itr;
					++itr_2;
				}				
				*itr_2 = element;
				delete[] tab;
				tab = newTab;
				
				// insert
				int curr = size+1;
				int next = 0;
				while(curr>1) {
					next = curr/2;
					if(tab[curr-1]<tab[next-1]) swap(tab[curr-1], tab[next-1]);
					//else break;
					curr = next;
				}
				
				++size;
			}
			else {
				tab = new Type[1];
				size = 1;
				tab[0] = element;
			}
		}
		
		Type top() {
			if(tab) return *tab;
			return 0;
		}
		
		Type pop() {
			if(tab) {
				Type ret = *tab;
				
				--size;
				if(size == 0) {
					delete[] tab;
					tab = nullptr;
					return ret;
				}
				tab[0] = tab[size];
				Type *newTab = new Type[size];
				for(int i=0; i<size; ++i) {
					newTab[i] = tab[i];
				}
				delete[] tab;
				tab = newTab;
				
				Type *curr = tab;
				Type *end = tab+size;
				Type *left = nullptr;
				Type *right = nullptr;
				
				while(true) {
					left = curr + (curr-tab+1);
					right = left + 1;
					if(left<end) {
						if(right<end) {
							if(*left>=*curr && *right>=*curr) break;
							if(*left<*right) {
								std::swap(*curr, *left); 
								curr = left;
							}
							else {
								std::swap(*curr, *right);
								curr = right;
							}							
						}
						else {
							if(*left>*curr) break;
							std::swap(*curr, *left);
							curr = left;
						}
					}
					else break;
				}				
				return ret;			
			}
			return 0;
		}
		
		bool empty() {
			return size == 0;
		}

	// Iterators
		
		int get_size() {
			return size;
		}		
		Type* begin() {
			return tab;
		}
		Type* end() {
			return tab+size;
		}
		Type* rbegin() {
			return tab+size-1;
		}
		Type* rend() {
			return tab-1;
		}
		
	// Display
	
		void set_stream(std::ostream& stream) {			
			int i = 1;
			int next = 2;
			Type *end = this->end();
			for(auto itr = begin(); itr!=end; ++itr) {
				if(i == next) {
					next *= 2;
					stream<<'\n';
				}
				++i;				
				stream<<std::right<<std::setw(3)<<*itr<<' ';
			}
			stream<<'\n';
		}		
	};
}