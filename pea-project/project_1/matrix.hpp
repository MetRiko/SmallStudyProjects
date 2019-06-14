#pragma once

#include <limits>

class Matrix {

	int inf = std::numeric_limits<int>::max();
	int **mat = nullptr;
	int size = 0;

public:

	Matrix() {
	}

	Matrix(const Matrix& source) {
		set(source);
	} 

	Matrix(Matrix& source) {
		set(source);
	} 

	Matrix(Matrix&& source) {
		set(std::move(source));
	}

	~Matrix() {
		clear();
	}

	void generateRandomMatrix(int size) {
		resize(size);
		for(int i=0; i<size; ++i) {
			for(int j=0; j<size; ++j) {
				mat[i][j] = rand()%100;
			}
		}
	}

	int getInf() {
		return inf;
	}

	int* operator[](int index) {
		return mat[index];
	}

	const int* operator[](int index) const {
		return mat[index];
	}

	void load(const std::string& filename) {		
		std::ifstream file(filename);
		if(file.good()) {
			file >> size;
			mat = new int*[size]{};
			for(int i=0; i<size; ++i) {
				mat[i] = new int[size];
				for(int j=0; j<size; ++j) {
					file >> mat[i][j];
				}
			}
		}
		else {
			std::cout<<"ERROR: Loading file error!\n";
		}
	}

	void resize(int newSize) {
		clear();
		size = newSize;
		mat = new int*[size]{};
		for(int i=0; i<size; ++i) {
			mat[i] = new int[size]{};
		}
	}

	void set(const Matrix &source) {
		resize(source.size);
		for(int i=0; i<size; ++i) {
			for(int j=0; j<size; ++j) {
				mat[i][j] = source[i][j];
			}
		}
	}

	void set(Matrix &&source) {
		mat = source.mat;
		size = source.size;
		source.mat = nullptr;
		source.size = 0;
	}

	void copy(Matrix &target) {
		target.set(*this);
	}

	void setDiagonalInf() {
		for(int i=0; i<size; ++i) {
			setInf(i, i);
		}
	}

	void setInf(int x, int y) {
		mat[y][x] = inf;
	}

	bool checkInf(int x, int y) {
		return mat[y][x] == inf;
	}

	// Reduce matrix and return minimum cost
	int reduce() {
		int cost = 0;

		for(int y=0; y<size; ++y) {
			int min = findMinInRow(y);
			if(min != 0 && min != inf) {
				cost += min;
				for(int x=0; x<size; ++x) {
					if(mat[y][x] != inf) {
						mat[y][x] -= min;
					} 
				}
			}
		}

		for(int x=0; x<size; ++x) {
			int min = findMinInColumn(x);
			if(min != 0 && min != inf) {
				cost += min;
				for(int y=0; y<size; ++y) {
					if(mat[y][x] != inf) {
						mat[y][x] -= min;
					} 
				}
			}
		}

		return cost;
	}

	// Return 0 if there is any 0
	int findMinInRow(int row) {
		int min = inf;
		for(int i=0; i<size; ++i) {
			if(mat[row][i] < min) {
				min = mat[row][i];
				if(min == 0) {
					return 0;
				}
			}
		}
		return min;
	}

	// Return 0 if there is any 0
	int findMinInColumn(int column) {
		int min = inf;
		for(int i=0; i<size; ++i) {
			if(mat[i][column] < min) {
				min = mat[i][column];
				if(min == 0) {
					return 0;
				}
			}
		}
		return min;
	}

	int getSize() {
		return size;
	}

	void print() {
		std::cout<<size<<'\n';
		for(int i=0; i<size; ++i) {
			for(int j=0; j<size; ++j) {
				if(mat[i][j] != inf) {
					std::cout<<mat[i][j]<<'\t';
				}
				else {
					std::cout<<"-\t";
				}
			}
			std::cout<<'\n';
		}
	}

	void clear() {
		if(mat != nullptr) {
			for(int i=0; i<size; ++i) {
				delete[] mat[i];
			}
			delete[] mat;
			mat = nullptr;
		}
	}
};