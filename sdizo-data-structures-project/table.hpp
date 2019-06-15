#pragma once

namespace sdizo {
	struct Table {
		using Type = int;
		int capacity = 0;
		Type *tab = nullptr;

	
		~Table() {
			clear();
		}
	
	// *etters
		int get_size() {
			return capacity;
		}

	// Search
		int* find(Type value) {
			if(!tab) return nullptr;
			Type* end = tab+capacity;
			Type* itr = tab;
			while(itr<end) {
				if(*itr == value) return itr;
				++itr;
			}
			return nullptr;
		}

	// Modifiers

		void resize(int newSize) {
			if(newSize <= 0) {
				if(tab) {
					delete[] tab;
					tab = nullptr;
					capacity = 0;
				}
				return;
			}
			Type *newTab = new Type[newSize];
			if(tab) {
				if(newSize>capacity) {				
					for(int i=0; i<capacity; ++i) newTab[i] = tab[i];
				}
				else {
					for(int i=0; i<newSize; ++i) newTab[i] = tab[i];				
				}
				delete[] tab;
			}
			capacity = newSize;
			tab = newTab;
		}
		
		void clear() {
			resize(0);
		}
	
		void push(Type element, int pos) {
			if(tab) {
				if(pos < 0) pos = 0;
				else if(pos > capacity) pos = capacity;
				resize(capacity+1);
				for(int i = capacity-2; i>=pos; --i) {
					tab[i+1] = tab[i];
				}
				tab[pos] = element;
			}
			else {
				capacity = 1;
				tab = new Type[1];
				tab[0] = element;
			}
		}
		
		void push_back(Type element) {		
			if(tab) {
				resize(capacity+1);
				tab[capacity-1] = element;
			}
			else {
				capacity = 1;
				tab = new Type[1];
				tab[0] = element;
			}
		}
		
		void push_front(Type element) {
			push(element, 0);
		}
		
		void pop(int pos) {
			if(tab) {
				if(pos < 0) pos = 0;
				else if(pos >= capacity) pos = capacity-1;
				for(int i = pos+1; i<capacity; ++i) {
					tab[i-1] = tab[i];
				}
				resize(capacity-1);
			}
		}
		
		void pop_back() {
			resize(capacity-1);
		}
		
		void pop_front() {
			pop(0);
		}
		
	// Iterators
		
		Type* begin() {
			return tab;
		}
		Type* end() {
			return tab+capacity;
		}
		Type* rbegin() {
			return tab+capacity-1;
		}
		Type* rend() {
			return tab-1;
		}
		
	// Display
	
		void set_stream(std::ostream& stream) {		
			Type *end = this->end();
			for(auto itr = begin(); itr!=end; ++itr) {
				stream<<*itr<<' ';
			}
			stream<<'\n';
		}	
	};
}