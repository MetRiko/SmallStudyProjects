#pragma once

namespace sdizo {
	template <typename T = int>
	struct UnorderedVector {
		using Type = T;

		int capacity = 0;
		int size = 0;
		Type *tab = nullptr;

	
		~UnorderedVector() {
			clear();
		}
		
		void clear() {
			if(tab != nullptr) {
				delete[] tab;
				tab = nullptr;
				capacity = 0;
				size = 0;
			}
		}

		void push_back(const Type& obj) {
			if(tab == nullptr) {
				capacity = 8;
				tab = new Type[capacity];
			}
			else if(size == capacity) {
				capacity *= 2;
				Type* newTab = new Type[capacity];
				for(int i=0; i<size; ++i) {
					newTab[i] = tab[i];
				}
				delete[] tab;
				tab = newTab;
			}
			tab[size] = obj;
			++size;
		}

		Type front() {
			return tab[0];
		}

		Type back() {
			return tab[size-1];
		}

		bool empty() {
			return size == 0;
		}

		int getSize() {
			return size;
		}

		void pop_back() {
			if(size > 0) {
				--size;
			}
			else {
				clear();
			}
		}

		void pop_front() {
			if(size > 0) {
				--size;
				tab[0] = tab[size];
			}
			else {
				clear();
			}
		}

		void erase(int index) {
			if(size > 0) {
				--size;
				tab[index] = tab[size];
			}
			else {
				clear();
			}
		}

		Type& operator[](int index) {
			return tab[index];
		}

		Type* begin() {
			return tab;
		}

		Type* end() {
			return tab+size;
		}
	};
}