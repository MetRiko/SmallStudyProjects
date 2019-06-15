#pragma once

template <class Type>
struct FastVector {
	// using Type = int;
	int capacity = 0;
	int _size = 0;
	Type *tab = nullptr;

	FastVector() {
		
	}

	FastVector(const FastVector& vec) {
		clear();
		capacity = vec.capacity;
		_size = vec._size;
		tab = new Type[capacity];
		for(int i=0; i<_size; ++i) {
			tab[i] = vec[i];
		}
	}

	FastVector(FastVector&& vec) {
		capacity = vec.capacity;
		_size = vec._size;
		tab = vec.tab;
		vec.capacity = 0;
		vec.tab = nullptr;
	}

	~FastVector() {
		clear();
	}

// *etters
	int size() {
		return _size;
	}

// Search
	int* find(Type value) {
		if(!tab) return nullptr;
		Type* end = tab+_size;
		Type* itr = tab;
		while(itr!=end) {
			if(*itr == value) return itr;
			++itr;
		}
		return nullptr;
	}

// Modifiers
	
	void clear() {
		delete[] tab;
		tab = nullptr;
		_size = 0;
		capacity = 0;
	}

	void create(int size) {
		if(tab) {
			delete[] tab;
		}
		tab = new Type[size];
		capacity = size;
		_size = 0;
	}

	void push_back(Type element) {
		if(tab == nullptr) {
			create(8);
		}
		if(_size>=capacity) {
			capacity *= 2;
			Type *newTab = new Type[capacity];
			for(int i=0; i<_size; ++i) {
				newTab[i] = tab[i];
			}
			delete[] tab;
			tab = newTab;
		}
		tab[_size] = element;
		++_size;
 	}
	
	void pop_back() {
		if(_size>0) {
			--_size;
		}
	}

	Type& operator[](int index) {
		return tab[index];
	}

	const Type& operator[](int index) const {
		return tab[index];
	}
	
// Iterators
	
	Type* begin() {
		return tab;
	}
	Type* end() {
		return tab+_size;
	}
	Type* rbegin() {
		return tab+_size-1;
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
