#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <memory>
#include <utility>
#include <random>
#include <chrono>
#include <cmath>


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

struct SimAnn {
	
	struct State {
		virtual ~State() {}
		virtual void init() = 0;
		virtual std::unique_ptr<State> next() = 0; 
		virtual float energy() = 0; 
		virtual void print(bool showDetails){}
	};

	float T; // temperature
	float initT; // initial temperature
	float a; // coefficient a
	float time; // Time with ms
	std::unique_ptr<State> c; // current state
	float bestResultDuration = 0.0;

	SimAnn(std::unique_ptr<State> newState, float initT, float a, float time) :
		c(std::move(newState)), initT(initT), a(a), time(time) {
		c->init();
		T = initT;
	}
	
	void start() {

    	using Clock = std::chrono::high_resolution_clock;
    	namespace Chrono = std::chrono;

    	Chrono::time_point<Clock> beforeTime;
    	float duration = 0.f;

    	bool printed = false;

		beforeTime = Clock::now();

		while(duration < time) {

			// Print %
			if(std::fmod(duration, time*0.1f) <= 50.f) {
				//if(not printed) std::cout<< int(duration/time * 100.0)<<"% ";
				printed = true;
			}
			else printed = false;			

			// Algorithm

			T *= a;

			float Ec = c->energy();
			auto n = c->next();
			float En = n->energy();
			float dE = En - Ec;
			if(dE > 0) {
				c = std::move(n);
				bestResultDuration = duration;
			}
			else if(std::exp(dE/T) > Rand::real()) {
				c = std::move(n);
				bestResultDuration = duration;
			}

			duration = Chrono::duration_cast<Chrono::milliseconds>(Chrono::duration<float>(Clock::now() - beforeTime)).count();
		}
		//std::cout<<'\n';
	}

	float getBestResultTime() {
		return bestResultDuration;
	}
	float getCurrentTemperature() {
		return T;
	}
	float getInitTemperature() {
		return initT;
	}
	float getCoe() {
		return a;
	}
	std::unique_ptr<State>& getState() {
		return c;
	}

	void printResult(bool showPath = true) {
		std::cout<<"Time for best result: "<<bestResultDuration<<" ms\n";
		std::cout<<"Init/Current temperature: "<<initT<<" / "<<std::floor(T*10000.f)*0.0001f<<'\n';
		c->print(showPath);
	}
};

struct SimAnnStateTSP : public SimAnn::State {

	Graph *graph;
	int *tab;
	int size;

	SimAnnStateTSP(Graph& graph) {
		this->graph = &graph;
		this->size = graph.getSize();
		tab = new int[size];
	}

	SimAnnStateTSP(const SimAnnStateTSP& obj) : 
		SimAnnStateTSP(*obj.graph) {

		for(int i=0; i<size; ++i) {
			tab[i] = obj.tab[i];
		}
	}

	~SimAnnStateTSP() {
		delete[] tab;
	}

	void init() {
		for(int i=0; i<size; ++i) {
			tab[i] = i;
		}
		for(int i=0; i<size; ++i) {
			int *a = tab + Rand::integer(size);
			int *b = tab + Rand::integer(size);
			std::swap(*a, *b);
		}
	}

	std::unique_ptr<State> next() {
		auto n = std::make_unique<SimAnnStateTSP>(*this);
		// std::unique_ptr<SimAnnStateTSP> n(new SimAnnStateTSP(*this));
		std::swap(n->tab[Rand::integer(size)], n->tab[Rand::integer(size)]);
		return n;
	}

	float distance() {		
		int curr = 0;
		for(int i=1; i<size; ++i) {
			curr += graph->getDistance(tab[i-1], tab[i]);
		}
		curr += graph->getDistance(tab[size-1], tab[0]);
		return curr;
	}

	float energy() {
		return -distance();
	}

	std::string getPath() {
		std::string path;
		for(int i=0; i<size; ++i) {
			path += std::to_string(tab[i]) + " ";
		}
		return path;
	}

	void print(bool printPath) {
		std::cout<<"Distance: "<<distance()<<'\n';
		if(printPath == true) {
			std::cout<<"Path: ";
			for(int i=0; i<size; ++i) {
				std::cout<<tab[i]<<' ';
			}
			std::cout<<'\n';
		}
	}
};

struct Tests {

	std::ostream& stream;

	// Consts
	std::string files[3] {"data/ftv47.atsp", "data/ftv170.atsp", "data/rbg403.atsp"};
	float coeA[3] {0.92, 0.9992, 0.999992};
	float bests[3] {1776, 2465, 2755};

	Tests(std::ostream& stream) : stream(stream) {}

	void runTest(const std::string& path, float T, float a, float time, int testsAmount, float bestKnownResult) {

		stream<<"================== NEW TEST ================== \n";
		stream<<"File: "<<path<<" | Coe: "<<a<<" | Max time: "<<time<<" ms | T: "<<T<<" | Best known: "<<bestKnownResult<<'\n';
		stream<<"Results: \nDistance | Time | Temperature | Relative error\n";

		std::unique_ptr<SimAnnStateTSP> bestResult(nullptr);
		float bestRelativeError = 0.0;

		Graph graph;
		graph.loadFromFile(path);

		for(int i=1; i<=testsAmount; ++i) {

			SimAnn sa(std::make_unique<SimAnnStateTSP>(graph), T, a, time);
			sa.start();

			stream<<"Test "<<i<<". ";
			float distance = static_cast<SimAnnStateTSP*>(sa.getState().get())->distance();
			stream<<distance<<" | ";
			stream<<sa.getBestResultTime()<<" | ";
			float cT = std::floor(sa.getCurrentTemperature() * 10000.f) * 0.0001;
			stream<<cT<<" | ";
			float relativeError = std::fabs(distance - bestKnownResult) / bestKnownResult;
			stream<<std::floor(relativeError * 10000.f) * 0.01<<"% \n";

			if(bestResult == nullptr or bestResult->distance() > distance) {
				bestResult.reset(static_cast<SimAnnStateTSP*>(sa.getState().release()));
				bestRelativeError = std::floor(relativeError * 10000.f) * 0.01;
			}
		}

		stream<<"Best result (distance): "<<bestResult->distance()<<'\n';
		stream<<"Best result (relative error): "<<bestRelativeError<<'\n';
		stream<<"Best result (path): "<<bestResult->getPath()<<'\n';
		stream<<'\n';
	}
};

struct Interface {

	std::string path;
	float initT;
	float a;
	float time;
	int testsAmount;
	float bestKnownResult;

	Tests tests;

	Interface() : tests(std::cout) {}

	void run() {
		std::cout<<"> Simulated Annealing for TSP \n";
		while(true) {
			std::cout<<"> Line syntax: file_path init_temp a_coe time(ms) tests_amount best_known_result\n";
			std::cout<<"> Exit: file_path == \"exit\"\n";
			std::cout<<"\n> Enter line: ";
			std::cin >> path;
			if(path == "exit" or path == "EXIT") {
				break;
			}
			std::cin >> initT;
			std::cin >> a;
			std::cin >> time;
			std::cin >> testsAmount;
			std::cin >> bestKnownResult;
			tests.runTest(path, initT, a, time, testsAmount, bestKnownResult);
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

	// ========== TESTS ==========

	// Tests tests(std::cout);
	// tests.runTest("data/rbg403.atsp", 403, 0.999999, 60000.f, 10, 2755);
	// tests.runTest("data/rbg403.atsp", 403, 0.9999, 60000.f, 10, 2755);
	// tests.runTest("data/rbg403.atsp", 403, 0.99, 60000.f, 10, 2755);

	// tests.runTest("data/ftv170.atsp", 171, 0.999999, 60000.f, 10, 2465);
	// tests.runTest("data/ftv170.atsp", 171, 0.9999, 60000.f, 10, 2465);
	// tests.runTest("data/ftv170.atsp", 171, 0.99, 60000.f, 10, 2465);

	// tests.runTest("data/ftv47.atsp", 48, 0.999999, 60000.f, 10, 1776);
	// tests.runTest("data/ftv47.atsp", 48, 0.9999, 60000.f, 10, 1776);
	// tests.runTest("data/ftv47.atsp", 48, 0.99, 60000.f, 10, 1776);
}