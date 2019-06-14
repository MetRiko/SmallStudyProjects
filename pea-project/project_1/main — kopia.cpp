#include <iostream>
#include <fstream>
#include <cmath>
#include <type_traits>
#include <limits>
#include <vector>
#include <utility>
#include <queue>
#include <functional>
#include <cstdlib>
#include <chrono>
#include <ctime>

struct Matrix {

	int inf = std::numeric_limits<int>::max();
	int **mat = nullptr;
	int size = 0;

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

template <bool SHOW_RESULT = false, bool DEBUG_MODE = false>
struct BruteForceTSP {
	Matrix mat;
	int size;

	int currentMin = -1;
	int *minPath = nullptr;
	int count = 1;
	int maxCount = 1;

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

template <bool SHOW_RESULT = false>
struct BxBTSP {
	Matrix baseMat;
	int size;
	std::priority_queue<int, std::vector<int>, std::greater<int>> mainQueue;
	std::vector<std::pair<int,int>> bannedConnections;
	std::vector<int> path;
	int minCost = 0;

	BxBTSP(const std::string& filename) {
		baseMat.load(filename);
		baseMat.setDiagonalInf();
		size = baseMat.getSize();
	}

	BxBTSP(const Matrix& source) {
		baseMat.set(source);
		baseMat.setDiagonalInf();
		size = baseMat.getSize();
	}

	bool reset() {

		mainQueue = std::priority_queue<int, std::vector<int>, std::greater<int>>();
		path.clear();

		Matrix mat(baseMat);
		int prevCost = mat.reduce();

		int vertex = 0;
		path.push_back(vertex);

		std::vector<int> queue;
		for(int i=1; i<size; ++i) {
			queue.push_back(i);
		}

		while(!queue.empty()) {
			vertex = _calculateLowMatrix(mat, vertex, queue, prevCost);
			if(vertex != -1) {
				path.push_back(vertex);
			}
			else {
				// baseMat.print();

				bannedConnections.emplace_back(path[path.size()-2], path.back());

				// while(!mainQueue.empty()) {
				// 	std::cout<<mainQueue.top()<<' ';
				// 	mainQueue.pop();
				// }
				// std::cout<<'\n';
				return false;
			}
			// std::cout<<"Low mat for: "<<vertex<<'\n';
			// mat.print();
		}

		return true;
	}

	void start() {

		bannedConnections.clear();

		while(!reset());

		path.push_back(path.front());

		minCost = 0;
		if constexpr(SHOW_RESULT == true) {
			std::cout<<"Sciezka: ";
		}

		for(int i=1; i<path.size(); ++i) {
			if constexpr(SHOW_RESULT == true) {
				std::cout<<path[i-1]<<' ';
			}
			minCost += baseMat[path[i-1]][path[i]];
		}

		if constexpr(SHOW_RESULT == true) {
			std::cout<<path[0];
			std::cout<<"\nKoszt: "<<minCost<<'\n';
			std::cout<<'\n';
		}
		
	}

	// return best vertex and replace target on reduced matrix with pair with best vertex and remove the vertex from vector
	int _calculateLowMatrix(Matrix &target, int firstVertex, std::vector<int>& secondVertices, int &prevCost) {		

		std::priority_queue<int, std::vector<int>, std::greater<int>> costs;
		int minLowCost = std::numeric_limits<int>::max();
		Matrix minLowMat;
		int minVertex = secondVertices.front();
		int minIndex = 0;

		int index = 0;
		for(auto secondVertex : secondVertices) {
			// Matrix lowMat(target);
			Matrix lowMat;
			lowMat.set(target);
			if(secondVertex == 1) {
				// std::cout<<"-------before-------\n";
				// lowMat.print();
			}
			int lowCost = _calculateReducedMatrixForPair(lowMat, firstVertex, secondVertex) + target[firstVertex][secondVertex] + prevCost;

			for(auto& pair : bannedConnections) {
				if(pair.first == firstVertex and pair.second == secondVertex) {
					// std::cout<<pair.first<<' '<<pair.second<<'\n';
					lowCost = minLowMat.getInf();
				}
			}

			costs.push(lowCost);

			// 	std::cout<<"v: "<<secondVertex<<" cost: "<<lowCost<<'\n';
			// if((firstVertex == 3 and secondVertex == 1) || (firstVertex == 3 and secondVertex == 2) || (firstVertex == 7 and secondVertex == 5)) {
			// 	lowCost = 100000;
			// 	// lowMat.print();
			// }
			// std::cout<<"-------- "<<secondVertex<<" -------\n";
			// lowMat.print();
			// std::cout<<"cost: "<<lowCost<<'\n';
			if(minLowCost > lowCost) {
				minVertex = secondVertex;
				minLowCost = lowCost;
				minLowMat.set(std::move(lowMat));
				minIndex = index;
			}
			++index;
		}
		// std::cout<<'\n';

		target.set(std::move(minLowMat));
		secondVertices.erase(secondVertices.begin() + minIndex);

		if(!mainQueue.empty() and minLowCost > mainQueue.top()) {
			return -1;
		}
		else {
			mainQueue = std::priority_queue<int, std::vector<int>, std::greater<int>>();
			costs.pop();
			while(!costs.empty()) {
				mainQueue.push(costs.top());
				costs.pop();
			}
			prevCost = minLowCost;
		}

		return minVertex;
	}

	// Add infs -> reduce -> return cost
	int _calculateReducedMatrixForPair(Matrix& mat, int from, int to) {
		for(int i=0; i<size; ++i) {
			mat.setInf(i, from);
			mat.setInf(to, i);
		}
		mat.setInf(from, to);

		return mat.reduce();
	}
};


int main() {
	
	while(true) {

		std::string filename;
		std::cout<<"---- TSP ----\n";
		std::cout<<"1. Brute force\n";
		std::cout<<"2. Branch and bound\n";
		std::cout<<"3. Wyjscie\n\n";

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
			BxBTSP<true> tsp2(filename);
			tsp2.start();
		}
		else if(filename[0] == '3') {
			break;
		}
		std::cout<<"Nacisnij [Enter] aby kontynuowac...\n";
		std::cin.get();
		std::cin.get();
	}


	// using namespace std::chrono;
	// std::srand(std::time(nullptr));

	// for(int n=6; n<=12; ++n) {

	// 	Matrix mat;
	// 	mat.generateRandomMatrix(n);
	// 	// BruteForceTSP tsp(mat);
	// 	BxBTSP tsp(mat);

	// 	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	// 	tsp.start();
	//     high_resolution_clock::time_point t2 = high_resolution_clock::now();

	//     auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();

	//     std::cout << n << ": " << duration << '\n';
	// }

}