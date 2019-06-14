#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <utility>
#include <random>
#include <chrono>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

struct Rand {
	static Rand *ptr;

	std::mt19937 gen;

	Rand() : gen(genSeed()) {ptr = this;}

	std::uint32_t genSeed() {
		using Clock = std::chrono::high_resolution_clock;
		return static_cast<std::uint32_t>(Clock::now().time_since_epoch().count());
	}

	static int integer(int to) {
		std::uniform_int_distribution<int> dis(0, to-1);
		return dis(ptr->gen);
	} 

	static int integer(int from, int to) {
		std::uniform_int_distribution<int> dis(from, to-1);
		return dis(ptr->gen);
	} 

	static float real() {
		std::uniform_real_distribution<float> dis(0, 1);
		return dis(ptr->gen);
	}

};
Rand* Rand::ptr = nullptr;

struct Graph {

	int size = 1;
	int **tab  = nullptr;

	~Graph() {
		removeTab();
	}

	int getDistance(int from, int to) {
		return tab[from][to];
	}

	int getSize() {
		return size;
	}

	void removeTab() {		
		if(tab != nullptr) {
			for(int i=0; i<size; ++i) {
				delete[] tab[i];
			}
			delete[] tab;
		}
	}

	void initTab(int size) {
		initTab(size, size);
	}

	void initTab(int width, int height) {
		removeTab();
		tab = new int*[width];
		for(int i=0; i<width; ++i) {
			tab[i] = new int[height];
		}
	}

	int& getCell(int idx) {
		return tab[idx%size][idx/size];
	}

	void loadFromFile(const std::string& path) {
		std::ifstream file;
		file.open(path);

		std::string line;
		while(std::getline(file, line), !file.eof()) {
			
			if(line == "EDGE_WEIGHT_SECTION") {
				break;
			}
			else if(line.substr(0, 9) == "DIMENSION") {
				std::stringstream ss;
				ss << line;
				ss >> line; ss >> size;
				//std::cout<<"Size: "<<size<<'\n';
			}	
		} 

		initTab(size);

		std::stringstream ss;
		int counter = 0;
		while(std::getline(file, line), !file.eof()) {
			ss << line;
			if(line != "EOF") {
				while(ss >> line) {
					// std::cout<<value<<' ';
					getCell(counter) = std::stoi(line);
					++counter;
				}
			}
			ss.clear();
		}
	}

	void print() {		
		//std::cout<<"Size: "<<size<<'\n';
		for(int i=0; i<size; ++i) {
			for(int j=0; j<size; ++j) {
				std::cout<<tab[j][i]<<' ';
			}
			std::cout<<'\n';
		}
	}

};

template <typename Unit>
struct GeneticAlgorithm {

	// struct Unit {

	// 	Unit() {}

	// 	virtual void generate() = 0;

	// 	virtual void mutate() = 0;

	// 	// lower is better
	// 	virtual float value() = 0;

	// 	virtual std::unique_ptr<Unit> cross(std::unique_ptr<Unit> &unit) = 0;

	// virtual std::unique_ptr<Unit> duplicate() = 0;

	// 	virtual void print() {}
	// };

	// Amounts
	bool switchTimeIterations = false;
	float time = 1.0; // Time with seconds
	int iterations = 10; 
	float populationAmount; // population size
	float crossesAmount;
	float mutationsAmount;

	std::vector<std::unique_ptr<Unit>> population;
	Unit& firstUnit;

	float lastCheckedTime = -1.0;
	float bestKnownResult = 1.0;
	int samplingTime = 5; // Time with seconds

	GeneticAlgorithm(Unit& firstUnit) : firstUnit(firstUnit) {

	}

	void setSamplingTime(int value) {
		samplingTime = value;
	}

	void setBestKnownResult(float value) {
		bestKnownResult = value;
	}

	void setIterations(int value) {
		iterations = value;
		switchTimeIterations = true;
	}

	void setTime(float value) {
		time = value;
		switchTimeIterations = false;
	}

	void setCoes(float populationAmount, float crossesCoe, float mutationsCoe) {
		this->populationAmount = populationAmount;
		this->crossesAmount = std::floor(populationAmount*crossesCoe);
		this->mutationsAmount = std::floor(populationAmount*mutationsCoe);
	}

	void setAmounts(float populationAmount, float crossesAmount, float mutationsAmount) {
		this->populationAmount = populationAmount;
		this->crossesAmount = crossesAmount;
		this->mutationsAmount = mutationsAmount;
	}

	void generateFirstPopulation() {
		population.clear();
		for(int i=0; i<populationAmount; ++i) {
			population.emplace_back(firstUnit.duplicate());
			population.back()->generate();
		}
	}

	void crossUnits(int idxA, int idxB) {
		auto newUnit = population[idxA]->cross(population[idxB]);
		population.emplace_back(newUnit.release());
	}

	void mutateUnit(int idx) {
		population[idx]->mutate();
	}

	void sortUnits() {
		std::sort(population.begin(), population.end(), [](auto &unitA, auto &unitB) {
			return unitA->value() < unitB->value();
		});
	}

	void selectUnits() {
		population.resize(populationAmount);
	}

	void doIteration() {

		// Crosses 
		for(int i=0; i<crossesAmount; ++i) {
			int unitA = Rand::integer(population.size());
			int unitB = Rand::integer(population.size());
			if(unitA != unitB) {
				crossUnits(unitA, unitB);		
			}
			else {
				--i;
			}
		}

		// Mutations
		for(int i=0; i<mutationsAmount; ++i) {
			mutateUnit(Rand::integer(population.size()));
		}

		// Put units with best result on the top
		sortUnits();

		// Remove all above population amount
		selectUnits();
	}

	void iterationsLoop() {

		for(int i=0; i<iterations; ++i) {
			doIteration();
			std::cout<<std::floor(float(i)/iterations*10000.0)*0.01<<"%\n";
			// std::cout<<"After itr nr "<<i+1<<":\n";
			//print();
		}
	}

	void timeLoop() {

		auto start = std::chrono::steady_clock::now();
		float duration = 1.0f;

		while(duration > 0.0f) {
			float currTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-start).count();
			doIteration();
			duration = time - currTime;
			if(int(duration) != lastCheckedTime and int(duration)%samplingTime == 0) {
				lastCheckedTime = duration;
				std::cout<<"Time left - "<<std::floor((time-duration)*10000.0)*0.0001<<"s | ";
				std::cout<<"Best dis - "<<population.front()->value()<<" | ";
				float relErr = std::fabs(bestKnownResult-population.front()->value())/bestKnownResult;
				relErr = std::floor(relErr*10000.0)*0.01;
				std::cout<<"Rel err - "<<relErr<<"%\n";
			}

			// std::cout<<"After itr nr "<<i+1<<":\n";
			//print();
		}
	}

	void start() {

		generateFirstPopulation();
		sortUnits();

		std::cout<<"Before: \n";
		print(5);

		if(switchTimeIterations == true) {
			iterationsLoop();
		}
		else {
			timeLoop();
		}

		std::cout<<"Final result:\n";
		print(5, true);
	}

	void print(int size = 0, bool printBestPath = false) {
		if(size == 0)
			size = population.size();

		for(int i=0; i<size; ++i) {
			std::cout<<i+1<<". ";
			// population[i]->print();
			std::cout<<" ("<<population[i]->value()<<')';
			std::cout<<'\n';
		}
		std::cout<<'\n';

		if(printBestPath == true) {
			std::cout<<"Best path: ";
			population.front()->print();
			std::cout<<'\n';
		}
	}

};

struct GeneticAlgorithmUnit_Graph {

	using Unit = GeneticAlgorithmUnit_Graph;

	std::vector<int> tab;

	Graph *graph = nullptr;

	float dis = 0.0;

	GeneticAlgorithmUnit_Graph(Graph& gr) : graph(&gr) {			
		tab.resize(graph->getSize());
	}

	std::unique_ptr<Unit> duplicate() {
		return std::unique_ptr<Unit>(new Unit(*this));
	}

	void generate() {

		for(int i=0; i<tab.size(); ++i) {
			tab[i] = i;
		}

		for(int i=0; i<tab.size(); ++i) {
			std::swap(tab[Rand::integer(tab.size())], tab[Rand::integer(tab.size())]);
		}
		dis = calcValue();
	}

	bool findInRange(int find, int begin, int end) {
		for(int i=begin; i<end; ++i) {
			if(tab[i] == find) return true;
		}
		return false;
	}

	void mutate() {
		std::swap(tab[Rand::integer(tab.size())], tab[Rand::integer(tab.size())]);
		dis = calcValue();
	}

	// lower is better
	float calcValue() {
		float dis = 0.0f;
		for(int i=1; i<tab.size(); ++i) {
			dis += graph->getDistance(tab[i-1], tab[i]);
		}
		dis += graph->getDistance(tab.back(), tab.front());
		return dis;
	}

	float value() {
		return dis;
	}

	std::unique_ptr<Unit> cross(std::unique_ptr<Unit> &unit) {
		std::unique_ptr<Unit> newUnit(new Unit(*this));
		int *curr = newUnit->tab.data();
		int *end = newUnit->tab.data() + newUnit->tab.size();

		if(Rand::integer(2) == 0) {
			// take frist half of first unit
			for(int i=0; i<tab.size()/2; ++i) {
				*curr = tab[i];
				++curr;
			}
			
			//take rest of second unit
			for(auto& obj : unit->tab) {
				if(not findInRange(obj, 0, tab.size()/2)) {
					*curr = obj;
					++curr;
					if(curr == end) break;
				}
			}				
		}
		else {
			// take second half of first unit
			end = newUnit->tab.data() + newUnit->tab.size()/2;
			curr = end;

			for(int i=tab.size()/2; i<tab.size(); ++i) {
				*curr = tab[i];
				++curr;
			}
			curr = newUnit->tab.data();
			
			//take rest of second unit
			for(auto& obj : unit->tab) {
				if(not findInRange(obj, tab.size()/2, tab.size())) {
					*curr = obj;
					++curr;
					if(curr == end) break;
				}
			}
		}

		newUnit->dis = newUnit->calcValue();
		
		return newUnit;
	}

	void print() {
		for(int i=0; i<tab.size(); ++i) {
			//std::cout<<char('A' + tab[i])<<' ';
			std::cout<<tab[i]<<' ';
		}
		std::cout<<tab.front()<<' ';
	}
};

struct Test {

	std::ostream& stream;

	Test(std::ostream& stream) : stream(stream) {}

	void run(const std::string& path, float populationCoe, float crossesCoe, float mutationsCoe, float time, float samplingTime, float bestKnownResult) {

		Graph graph;
		graph.loadFromFile(path);
		float populationAmount = std::floor(populationCoe * graph.getSize());

		stream<<"================== NEW TEST ================== \n";
		stream<<"File: "<<path<<" | popAm: "<<populationAmount<<" | crossCoe: "<<crossesCoe<<" | mutCoe: "<<mutationsCoe<<" | Time: "<<time<<" | Sampling time: "<<samplingTime<<" | Best known: "<<bestKnownResult<<'\n';

		GeneticAlgorithmUnit_Graph unit(graph);
		GeneticAlgorithm<GeneticAlgorithmUnit_Graph> ga(unit);

		ga.setTime(time);
		ga.setSamplingTime(samplingTime);
		ga.setBestKnownResult(bestKnownResult);
		ga.setCoes(populationAmount, crossesCoe, mutationsCoe);
		ga.start();
		std::cout<<'\n';
	}
};

struct Interface {

	std::string path;
	float populationCoe;
	float crossesCoe;
	float mutationsCoe;
	float time;
	float samplingTime;
	float bestKnownResult;

	void run() {
		std::cout<<"> Genetic algorithm for TSP \n";
		while(true) {
			std::cout<<"> Line syntax: file_path pop_coe cross_coe mut_coe time sampling_time best_known_result\n";
			std::cout<<"> Exit: file_path == \"exit\"\n";
			std::cout<<"\n> Enter line: ";

			std::cin >> path;
			if(path == "exit" or path == "EXIT") {
				break;
			}

			std::cin >> populationCoe;
			std::cin >> crossesCoe;
			std::cin >> mutationsCoe;
			std::cin >> time;
			std::cin >> samplingTime;
			std::cin >> bestKnownResult;

			Test test(std::cout);
			test.run(path, populationCoe, crossesCoe, mutationsCoe, time, samplingTime, bestKnownResult);

			std::cout<<"> Press [Enter] to continue...";
			std::cin.get();
			std::cin.get();
		}
	}
};

int main() {

	Rand randomDevice;

	Interface interface;
	interface.run();

	// ===============================

	// Graph graph;
	// graph.loadFromFile("data/rbg403.atsp");

	// GeneticAlgorithmUnit_Graph unit(graph);
	// GeneticAlgorithm<GeneticAlgorithmUnit_Graph> ga(unit);

	// // ga.setIterations(10000);
	// ga.setTime(300.0);
	// ga.setSamplingTime(20.0);
	// ga.setBestKnownResult(2755.0);
	// // ga.setAmounts(10, 1, 1);
	// ga.setCoes(graph.getSize(), 0.8, 0.01);
	// // ga.setCoes(graph.getSize()*2.0, 0.8, 0.01);
	// // ga.setCoes(graph.getSize()*5.0, 0.8, 0.01);
	// ga.start();

	// ========== TESTS ==========

	// Test test(std::cout);

	// test.run("data/rbg403.atsp", 1.0, 0.8, 0.01, 300.0, 20.0, 2755.0);
	// test.run("data/rbg403.atsp", 2.0, 0.8, 0.01, 300.0, 20.0, 2755.0);
	// test.run("data/rbg403.atsp", 5.0, 0.8, 0.01, 300.0, 20.0, 2755.0);

	// test.run("data/ftv170.atsp", 1.0, 0.8, 0.01, 300.0, 20.0, 2465.0);
	// test.run("data/ftv170.atsp", 2.0, 0.8, 0.01, 300.0, 20.0, 2465.0);
	// test.run("data/ftv170.atsp", 5.0, 0.8, 0.01, 300.0, 20.0, 2465.0);

	// test.run("data/ftv47.atsp", 50.0, 0.8, 0.01, 300.0, 20.0, 1776.0);
	// test.run("data/ftv47.atsp", 20.0, 0.8, 0.01, 300.0, 20.0, 1776.0);
	// test.run("data/ftv47.atsp", 10.0, 0.8, 0.01, 300.0, 20.0, 1776.0);
	// test.run("data/ftv47.atsp", 500.0, 0.8, 0.01, 300.0, 20.0, 1776.0);
}