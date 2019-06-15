#include <iostream>
#include <thread>
#include <chrono>
#include <type_traits>
#include <mutex>
#include <functional>
#include <memory>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <random>
#include <atomic>
#include <array>
#include <condition_variable>

#ifdef NCURSES
#include <ncurses.h>
#endif // NCURSES

// --------------------- Tools --------------------- 

class Rand {

	static Rand *ptr;
	std::mt19937 eng;

public: // ---- Constructors ---- 

	Rand() : eng(seed()) {
		ptr = this;
	}

private: // ---- Seed ---- 

	std::uint32_t seed() {
		using Clock = std::chrono::high_resolution_clock;
		return static_cast<std::uint32_t>(Clock::now().time_since_epoch().count());
	}

public: // ---- Range ---- 

	struct ReturnReal{};
	struct ReturnInt{};

	template <typename T>
	static T rangeTyped(T from, T to, ReturnInt) {
		std::uniform_int_distribution<T> dis(from, to);
		return dis(ptr->eng );
	}

	template <typename T>
	static T rangeTyped(T from, T to, ReturnReal) {
		std::uniform_real_distribution<T> dis(from, to);
		return dis(ptr->eng);
	}

	std::mutex rangeMutex;
	template <typename T>
	static T range(T from, T to) {
		std::lock_guard<std::mutex> lock(ptr->rangeMutex);
		return rangeTyped(from, to, typename std::conditional<std::is_integral<T>::value, ReturnInt, ReturnReal>::type());
	}

} __Rand;
Rand *Rand::ptr = nullptr;

namespace chrono {
	using Timestamp = decltype(std::chrono::high_resolution_clock::now());

	std::mutex nowMutex;
	auto now() {
		std::lock_guard<std::mutex> lock(nowMutex);
		return std::chrono::high_resolution_clock::now();
	}

	std::mutex differenceMutex;
	float difference(const auto &b, const auto &a) {
		std::lock_guard<std::mutex> lock(differenceMutex);
		return std::chrono::duration_cast<std::chrono::milliseconds>(b-a).count();
	}
}

class Timer {
	chrono::Timestamp lastTs;
	float timeElapsed = 0.0;
	float waitTime = 1000.0;
	bool isActive = false;
	bool oneShot = false;

	std::function<void()> timeoutCallback;

public: // ---- Main ----

	void update() {
		if (isActive == true) {
			timeElapsed = chrono::difference(chrono::now(), lastTs);
			if (timeElapsed >= waitTime) {
				if (timeoutCallback) timeoutCallback();
				if (oneShot) stop();
				else lastTs = chrono::now();
			}			
		}
	}

public: // ---- Control ----

	void start(float waitTime = -1.0, bool oneShoot = false) {
		lastTs = chrono::now();
		timeElapsed = 0.0;
		isActive = true;

		this->oneShot = oneShot; 
		if (waitTime > 0) {
			this->waitTime = waitTime;
		}
	}

	void stop() {
		timeElapsed = waitTime;
		isActive = false;
	}

public: // ---- Setters ----

	void setCallback(std::function<void()> callback) {
		timeoutCallback = callback;
	}

	void setWaitTime(float waitTime) {
		this->waitTime = waitTime;
	}

	void setOneShot(bool flag) {
		oneShot = flag;
	}

public: // ---- Check ----

	bool stopped() {
		return isActive == false;
	}

public: // ---- Getters ----

	float getElapsed() {
		return timeElapsed; 
	}

	float getLeft() {
		return waitTime - timeElapsed;
	}

	float getProgress() {
		return timeElapsed/waitTime;
	}
};

class StateThread {

	chrono::Timestamp lastTs;

	float stateDuration = 0.0;
	float timeElapsed = 0.0;

	std::thread thread;
	std::string stateName;

	std::atomic<bool> finished {false};
	std::atomic<bool> infState {false};
	bool isAnyState = false;

	std::function<void()> stateFunction;
	std::function<void()> callbackStateFunction;

public: // ---- Constructors ----

	StateThread() 
		: thread(&StateThread::main, this) {
	}	

private: // ---- Thread's main ----

	void updateState() {
		if (isAnyState == true) {
			timeElapsed = chrono::difference(chrono::now(), lastTs);
			if (timeElapsed < stateDuration) {
				// std::cout<<chrono::difference(chrono::now(), lastTs)<<std::endl;
				if(stateFunction) stateFunction();
			}
			else {
				isAnyState = false;
				timeElapsed = stateDuration;
				if(callbackStateFunction) callbackStateFunction();
			}
		}
	}

	void main() {
		init();

		while (not finished) {
			updateState();
			process();
		}
	}

protected: // ---- Thread's methods ----

	virtual void process() = 0;
	virtual void init() = 0;
	
public: // ---- Control ----

	std::mutex changeStateMutex;
	void changeState(const std::string& stateName, auto function) {
		std::lock_guard<std::mutex> lock(changeStateMutex);
		changeState(stateName, 100000000.0, function, nullptr);
		infState = true;
	}

	std::mutex changeState2Mutex;
	void changeState(const std::string& stateName, float stateDuration, auto function, auto callback) {
		std::lock_guard<std::mutex> lock(changeState2Mutex);
		infState = false;
		isAnyState = true;
		lastTs = chrono::now();
		timeElapsed = 0.0;
		this->stateDuration = stateDuration;
		this->stateName = stateName;
		stateFunction = function;
		callbackStateFunction = callback; 
	}

	void finish() {
		finished = true;
	}

	void join() {
		thread.join();
	}

public: // ---- Check ----

	bool isInfState() {
		return infState;
	}

public: // ---- Getters ----
	
	std::mutex getStateNameMutex;
	const std::string& getStateName() {
		std::lock_guard<std::mutex> lock(getStateNameMutex);
		return stateName;
	}

	std::mutex getTimeLeftMutex;
	float getTimeLeft() {
		std::lock_guard<std::mutex> lock(getTimeLeftMutex);
		return stateDuration - timeElapsed;
	}

	std::mutex getTimeElpsedMutex;
	float getTimeElpsed() {
		std::lock_guard<std::mutex> lock(getTimeElpsedMutex);
		return timeElapsed;
	}

	std::mutex getStateDurationMutex;
	float getStateDuration() {
		std::lock_guard<std::mutex> lock(getStateDurationMutex);
		return stateDuration;
	}

	std::mutex getCurrentProgressMutex;
	float getCurrentProgress() {
		std::lock_guard<std::mutex> lock(getCurrentProgressMutex);
		return timeElapsed/stateDuration;
	}

	std::mutex getCurrentProgressWithPercentsMutex;
	float getCurrentProgressWithPercents() {
		std::lock_guard<std::mutex> lock(getCurrentProgressWithPercentsMutex);
		return std::floor(100.0 * timeElapsed/stateDuration);
	}

	std::mutex getThreadMutex;
	std::thread& getThread() {		
		std::lock_guard<std::mutex> lock(getThreadMutex);
		return thread;
	}
};

// --------------------- World ---------------------

int WORLD_SIZE = 1000;

class DayNightCycle : public StateThread {
	std::atomic<bool> day {true};

private: // ---- Thread's methods ----

	void process() {

	}

	void init() {
		startDay();
	}

private: // ---- Day/night switching (callbacks) ----

	void startDay() {
		day = true;
		changeState("day", 30000.0, nullptr, [&](){startNight();});
	}

	void startNight() {
		day = false;
		changeState("night", 15000.0, nullptr, [&](){startDay();});
	}

public: // ---- Check ----

	bool isNight() {
		return not day;
	}

	bool isDay() {
		return day;
	}

public: // ---- Getters ----

	std::mutex getStringStatusMutex;
	std::string getStringStatus() {
		std::lock_guard<std::mutex> lock(getStringStatusMutex);

		std::string ret = "[Day-night cycle]\n";
		ret += day ? "Day " : "Night ";
		ret += std::to_string(int(getCurrentProgressWithPercents())) + " %";
		return ret;
	}
};

class Tree {
	float healthPerWood = 20.0;
	float health = healthPerWood;

	std::atomic<int> wood {Rand::range(1, 3)};
	std::atomic<int> workers {0};
	int workersIdx[2]{0, 0};

	float distance = Rand::range(80.0f, float(WORLD_SIZE));

public: // ---- Control ----

	std::mutex addWorkerMutex;
	bool addWorker(int idx) {
		std::lock_guard<std::mutex> lock(addWorkerMutex);

		if (canAddWorker()) {
			workersIdx[workers] = idx; 
			++workers;
			return true;
		}
		else return false;
	}

	std::mutex removeWorkerMutex;
	void removeWorker(int idx) {
		std::lock_guard<std::mutex> lock(removeWorkerMutex);

		if (workers > 0) {
			--workers;
			if (workersIdx[0] == idx) workersIdx[0] = 0;
			else if(workersIdx[1] == idx) workersIdx[1] = 0;
		}
	}

	// states: 0-nothing, 1-wood-1
	std::mutex damageMutex;
	bool damage(float value) { 
		std::lock_guard<std::mutex> lock(damageMutex);

		health -= value;
		if (health <= 0.0) {
			if(wood > 0) {
				health += healthPerWood;
				--wood;
				if (wood == 0) {
					health = 0.0;
				}
				return true;
			}
			else {
				health = 0.0;
				return false;
			}
		}
		return false;
	}

public: // ---- Check ----

	bool canAddWorker() {
		return workers < 2 and wood > 0;
	}

	bool hasWorker(int workerIdx) {
		return workersIdx[0] == workerIdx or workersIdx[1] == workerIdx;
	}

	bool hasWood() {
		return wood > 0;
	}

	bool hasWorkers() {
		return workers > 0;
	}

public: // ---- Getters ----

	std::mutex getDistanceMutex;
	float getDistance() {
		std::lock_guard<std::mutex> lock(getDistanceMutex);
		return distance;
	}

	std::mutex getStringStatusMutex;
	std::string getStringStatus() {
		std::lock_guard<std::mutex> lock(getStringStatusMutex);

		std::string ret = "[Tree] HP ";
		ret += std::to_string(int(health)) + "/" + std::to_string(wood) + " | ";

		for(auto& idx : workersIdx) {
			ret += '[';
			if (idx > 0) ret += std::to_string(idx) + "]";
			else ret += " ]";
		}

		return ret;
	}
};

class Forest {
	std::vector<std::unique_ptr<Tree>> trees;

	DayNightCycle& timeCycle;
	bool dayState = true;
	bool waitOnRemoveTrees = false;

	std::atomic<int> workers {0};

public: // ---- Constructors ----

	Forest(DayNightCycle& timeCycle) : timeCycle(timeCycle) {
		dayState = timeCycle.isDay();
	}

public: // ---- Main ----
	
	void update() {

		if (dayState != timeCycle.isDay()) {
			dayState = timeCycle.isDay();
			if (timeCycle.isDay()) { // Day started
				generate();
			}
			else { // Night started
				waitOnRemoveTrees = true;
			}
		}
		if (waitOnRemoveTrees == true) {
			if (areAllTreesFree()) {
				waitOnRemoveTrees = false;
				removeEmptyTrees();
			}
		}
	}

public: // ---- Control ---- @TODO
	
	std::mutex generateMutex;
	void generate() {
	std::lock_guard<std::mutex> lock(generateMutex);

		for (int i = trees.size(); i < 10; ++i) {
			trees.emplace_back(new Tree);
		}
	}

	std::mutex removeEmptyTreesMutex;
	void removeEmptyTrees() {
		std::lock_guard<std::mutex> lock(removeEmptyTreesMutex);
		auto itr = trees.begin();
		while(itr != trees.end())
		{
		    if(not itr->get()->hasWood()) itr = trees.erase(itr);
		    else ++itr;
		}
	}

	// return false if there is no free trees
	std::mutex addWorkerToTreeMutex;
	Tree* addWorkerToTree(int workerIdx) {
		std::lock_guard<std::mutex> lock(addWorkerToTreeMutex);
		for(auto& tree : trees) {
			if (tree->canAddWorker()) {
				tree->addWorker(workerIdx);
				++workers;

				return tree.get();
			}
		}
		return nullptr;
	}

	std::mutex removeWorkerFromTreeMutex;
	void removeWorkerFromTree(int workerIdx) {
		std::lock_guard<std::mutex> lock(removeWorkerFromTreeMutex);
		for(auto& tree : trees) {
			if (tree->hasWorker(workerIdx)) {
				tree->removeWorker(workerIdx);
				--workers;
				return;
			}
		}
	}

public: // ---- Check ----

	std::mutex areAllTreesFreeMutex;
	bool areAllTreesFree() {
		std::lock_guard<std::mutex> lock(areAllTreesFreeMutex);

		for (auto& tree : trees) {
			if (tree->hasWorkers()) return false;
		}
		return true;
	}

public: // ---- Getters ----

	std::string getStringStatus() {
		std::stringstream ss;
        
        int idx = 1;
		for(auto& tree : trees) {
			ss << idx << ' ' << tree->getStringStatus() << '\n';
			++idx;
		}
		
		return ss.str();
	}

	std::string getMap() {
		std::string map(100, '-');

		for(auto& tree : trees) {
			if (tree->hasWood()) {				
				int mapIdx = int(tree->getDistance()/WORLD_SIZE*100.0);
				map[mapIdx] = 'T';
			}
		}

		return map;
	}
};

class EatingRoom {
	std::atomic<int> workers {0};
	const int maxPlaces = 4;
	std::atomic<int> workersIdx[4] {};

	float distance = 60.0;

public: // ---- Control ----

	void addWorker(int idx) {
		if (hasPlace()) {
			workersIdx[workers] = idx;
			++workers;
		}
	}

	std::mutex removeWorkerMutex;
	void removeWorker(int idx) {
		std::lock_guard<std::mutex> lock(removeWorkerMutex);
		for(auto& worker : workersIdx) {
			if (worker == idx) {
				worker = 0;
				return;
			}
		}
	}

public: // ---- Check ----

	bool hasPlace() {
		return workers < maxPlaces;
	}

public: // ---- Getters ----
	
	std::mutex getStringStatusMutex;
	std::string getStringStatus() {
		std::lock_guard<std::mutex> lock(getStringStatusMutex);

		std::string ret = "[EatingRoom] Places: ";
		for(auto& idx : workersIdx) {
			ret += '[';
			if (idx > 0) ret += std::to_string(idx) + "]";
			else ret += " ]";
		}
		ret += std::string("\n");
        return ret;
	}

	std::mutex getDistanceMutex;
	float getDistance() {
		std::lock_guard<std::mutex> lock(getDistanceMutex);
		return distance;
	}

};

class Storage : public StateThread {
	Timer timer;
	DayNightCycle& timeCycle;
	bool timeState = false;

	int wood = 0;
	int processedWood = 0;
	int saws = 3;

	float distance = 30.0; 

public: // ---- Constructors ----

	Storage(DayNightCycle& timeCycle) : timeCycle(timeCycle) {
		timeState = timeCycle.isDay();
	}

private: // ---- Thread's methods ----

	void init() {
		startProcessingWood();
	}

	void process() {
		timer.update();
		if (timeState != timeCycle.isDay()) {
			timeState = timeCycle.isDay();
			if (timeCycle.isDay()) {
				startProcessingWood();
			}
			else {
				stopProcessingWood();
			}
		}
	}

public: // ---- Control ----

	std::mutex startProcessingWoodMutex;
	void startProcessingWood() {
		std::lock_guard<std::mutex> lock(startProcessingWoodMutex);

		if (wood > 0) {
			timer.setCallback([&](){
				if (wood > 0) {
					--wood;
					++processedWood;
					if (wood == 0) waitForWood();
				}
			});
			timer.start(1500.0);
			changeState("przerabia drzewo", nullptr);
		}
		else {
			waitForWood();
		}
	}

	std::mutex waitForWoodMutex;
	void waitForWood() {
		std::lock_guard<std::mutex> lock(waitForWoodMutex);
		timer.stop();
		changeState("czeka na drewno", nullptr);		
	}

	std::mutex stopProcessingWoodMutex;
	void stopProcessingWood() {		
		std::lock_guard<std::mutex> lock(stopProcessingWoodMutex);

		timer.stop();
		changeState("nic nie robi", nullptr);
	}

	std::mutex addWoodMutex;
	void addWood(int value) {
		std::lock_guard<std::mutex> lock(addWoodMutex);

		if (wood == 0) {			
			wood += value;
			startProcessingWood();
		}
		else {	
			wood += value;			
		}
	}

	std::mutex takeSawMutex;
	bool takeSaw() {
		std::lock_guard<std::mutex> lock(takeSawMutex);

		if (hasSaw()) {
			--saws;
			return true;
		}
		return false;
	}

	std::mutex takeOffSawMutex;
	void takeOffSaw() {
		std::lock_guard<std::mutex> lock(takeOffSawMutex);

		++saws;
	}

public: // ---- Check ----

	std::mutex hasSawMutex;
	bool hasSaw() {
		std::lock_guard<std::mutex> lock(hasSawMutex);

		return saws > 0;
	}

public: // ---- Getters ----

	std::mutex getDistanceMutex;
	float getDistance() {
		std::lock_guard<std::mutex> lock(getDistanceMutex);

		return distance;
	}

	std::mutex getStringStatusMutex;
	std::string getStringStatus() {
		std::lock_guard<std::mutex> lock(getStringStatusMutex);

        std::stringstream ss;
        
		ss << "[Storage]: " << getStateName() << ' ';
		if (isInfState() == true)
			ss << " --- %";
		else
			ss << int(getCurrentProgressWithPercents()) << " %";

		ss << " | Saws: " << saws << " | Wood: " << wood << " -> " << processedWood << '\n';
		
		return ss.str();
	}
};

class Employee : public StateThread {

	Forest& forest;
	Storage& storage;
	Timer timer;
	EatingRoom& eatingRoom;

	std::atomic<int> idx {0};
	int wood = 0;
	int maxWood = 2;
	int tools = 0; // 0 - nothing, 1 - axe, 2 - axe+saw
	float hungry = 0.0; // max 100.0
	bool finishedJob = false;

	Tree* targetedTree = nullptr;

	std::string place = "home";
	
	float distance = 0.0;
	float prevDistance = 0.0;
	float nextTargetDistance = 0.0;

public: // ---- Constructors ----

	Employee(int idx, Forest& forest, Storage& storage, EatingRoom& eatingRoom) 
		: idx(idx), forest(forest), storage(storage), eatingRoom(eatingRoom) {}

private: // ---- Thread's methods ----

	void init() {
		goToWork();
	}

	void process() {
		timer.update();

		distance = prevDistance + (nextTargetDistance - prevDistance) * getCurrentProgress();
	}

private: // ---- Distance ----

	void setNextTargetDistance(float nextTargetDistance) {
		prevDistance = distance;
		this->nextTargetDistance = nextTargetDistance;
	}

	float calculateTravelTime() {
		return abs(prevDistance - nextTargetDistance) * 8.0 + 1.0;
	}

public: // ---- Moving ----

	void goToWork() {	
		finishedJob = false;
		place = "outside";
		setNextTargetDistance(storage.getDistance());
		changeState("idzie do pracy", calculateTravelTime(), nullptr, [&](){
			goToStorage();
		});
	}

private: // ---- Moving ----

	void justWait() {
		setNextTargetDistance(getCurrentDistance());
		changeState("czeka", Rand::range(1000.0, 3000.0), nullptr, [&](){
			if (place == "storage") goToStorage();
			else if (place == "eating room") goToEatingRoom();
			else goToStorage();
		});	
	}

	void goToStorage() {
		setNextTargetDistance(storage.getDistance());
		float time = 1.0;
		if (place != "storage") {
			place = "outside";
			time = calculateTravelTime();
		}
		changeState("idzie do magaz.", time, nullptr, [&](){
			place = "storage";

			// Inside the storage
			takeOffWood();

			if (finishedJob == false) {

				if (not isHungry()) {
					takeTools(); // Take axe (and saw) 
					findAndGoToFreeTree(); // Searching free tree
				}
				else {
					takeOffTools(); // Cannot eat with tools
					goToEatingRoom();
				}
			}
			else goHome();
		});
	}

	void findAndGoToFreeTree() {
		targetedTree = forest.addWorkerToTree(idx);
		if (targetedTree != nullptr) {
			// Found free tree 
			place = "outside";

			setNextTargetDistance(targetedTree->getDistance());
			changeState("idzie do drzewa", calculateTravelTime(), nullptr, [&](){startCutting();});
		}
		else {
			// No free tree in forest
			if (hasEmptyBackpack()) {
				justWait();		
			}
			else {
				goToStorage();	
			}
		}
	}

	void goToEatingRoom() {		
		setNextTargetDistance(eatingRoom.getDistance());
		float time = 1.0;
		if (place != "eating room") {
			place = "outside";
			time = calculateTravelTime();
		}
		changeState("idzie do jadalni", time, nullptr, [&](){ 
			place = "eating room";

			if (eatingRoom.hasPlace()) {
				eatingRoom.addWorker(idx);
				startEating();
			}
			else {
				justWait();
			}
		});
	}

	void goHome() {
		setNextTargetDistance(0.0);
		changeState("idzie do domu", calculateTravelTime(), nullptr, [&](){ 
			place = "home"; 
			hungry = 0;
			setNextTargetDistance(getCurrentDistance());
			changeState("jest w domu", nullptr);
		});
	}

private: // ---- Control ----

	void startCutting() {
		timer.setCallback([&](){cuttingTreeCallback();});
		timer.start(600.0);
		setNextTargetDistance(getCurrentDistance());
		changeState("scina drzewo", nullptr);
	}

	void stopCutting() {
		timer.stop();
		forest.removeWorkerFromTree(idx);
		// targetedTree->removeWorker(idx);
		targetedTree = nullptr;
	}

	std::mutex takeToolsMutex;
	void takeTools() {
		std::lock_guard<std::mutex> lock(takeToolsMutex);

		if (tools == 0) { // Take axe
			tools = 1;
		}
		if (tools == 1 and storage.hasSaw()) { // Take saw
			storage.takeSaw();
			tools = 2;
		}
	}

	std::mutex takeOffToolsMutex;
	void takeOffTools() {
		std::lock_guard<std::mutex> lock(takeOffToolsMutex);

		if (hasTools()) {
			if (hasSaw()) {
				storage.takeOffSaw();
			}
			tools = 0;
		}
	}

	std::mutex takeOffWoodMutex;
	void takeOffWood() {
		std::lock_guard<std::mutex> lock(takeOffWoodMutex);
		if (not hasEmptyBackpack()) {
			storage.addWood(wood);
			wood = 0;
		}
	}

	void startEating() {		
		timer.setCallback([&](){
			hungry -= 3.0;
			if (hungry < 0.0) {
				hungry = 0.0;
				stopEeating();
				goToStorage();
			} 
		});
		timer.start(500.0);
		changeState("spozywa posilek", nullptr);
	}

	void stopEeating() {
		timer.stop();
		eatingRoom.removeWorker(idx);
	}

	std::mutex cuttingTreeCallbackMutex;
	void cuttingTreeCallback() {
		std::lock_guard<std::mutex> lock(cuttingTreeCallbackMutex);

		if (targetedTree->hasWood()) {

			// Cutting tree
			bool gotNewWood = targetedTree->damage(getDamage());
			addHungry(Rand::range(4.0, 8.0));
			
			if (gotNewWood == true) {
				addWoodToBackpack();

				if (not hasPlaceInBackpack()) {
					// Go to storage
					stopCutting();
					goToStorage();
				}
				else if (not targetedTree->hasWood()) {
					// Find next tree
					stopCutting();
					findAndGoToFreeTree();
				}
			}
		
			if (isHungry()) {
				stopCutting();
				goToStorage();
			}
		}
		else {
			stopCutting();
			if (hasPlaceInBackpack()) findAndGoToFreeTree();
			else goToStorage();
		}
	}

	bool addWoodToBackpack() {
		if (hasPlaceInBackpack()) {
			++wood;
			return true;
		}
		else return false;
	}


	void addHungry(float value) {
		hungry += value;
	}

public: // ---- Control ----

	std::mutex finishJobMutex;
	void finishJob() {
		std::lock_guard<std::mutex> lock(finishJobMutex);

		if (isCuttingTree()) stopCutting();
		finishedJob = true;
		goToStorage();
	}

private: // ---- Check ----

	bool hasTools() {
		return tools > 0;
	}

	bool hasEmptyBackpack() {
		return wood == 0;
	}

	bool hasPlaceInBackpack() {
		return wood < maxWood;
	}

	bool hasSaw() {
		return tools == 2;
	}

	bool isCuttingTree() {
		return targetedTree != nullptr;
	}

	bool isHungry() {
		return hungry > 100.0;
	}

private: // ---- Getters ----

	float getDamage() {
		float factor = hasSaw() ? 1.5f : 1.0f;
		if (hungry < 25.0) return 4.0f * factor;
		else if (hungry < 50.0) return 3.0f * factor;
		else if (hungry < 75.0) return 2.0f * factor;
		else return 1.0f * factor;
	}

public: // ---- Getters ----

	int getIdx() {
		return idx;
	}

	std::mutex getCurrentDistanceMutex;
	float getCurrentDistance() {
		std::lock_guard<std::mutex> lock(getCurrentDistanceMutex);
		return distance;
	}

		// idz do roboty
		// idz do magazynu

		// idz do drzewa
		// tnij drzewo
		// wroc do magazynu
		// odloz drzewo

		// idz do drzewa 
		// ...

	std::mutex getStringStatusMutex;
	std::string getStringStatus() {
		std::lock_guard<std::mutex> lock(getStringStatusMutex);

		std::stringstream ss;
		ss << "[Employee]: " << getStateName() << ' ';
		if (isInfState() == false)
			ss << int(getCurrentProgressWithPercents()) << " % \t| Backpack: ";
		else
			ss << " --- % \t| Backpack: ";

		for(int i = 0; i < maxWood; ++i) {
			if (i < wood) ss << "[x]";
			else ss << "[ ]";
		}

		ss << " | Miejsce: " << place;

		ss << " | Hungry: " << int(hungry);

		ss << " | Tools: ";
		if (tools == 0) ss << "--- --- ";
		else if (tools == 1) ss << "AXE --- ";
		else if (tools == 2) ss << "AXE SAW ";

		ss << getCurrentDistance();

		if (isCuttingTree()) {
	 		ss << "\n   > " << targetedTree->getStringStatus();
		}
		else {			
	 		ss << "\n   > [NO TREE]";
		}

		return ss.str();
	}
};

class Sawmill {
	Forest& forest;
	DayNightCycle& timeCycle;
	EatingRoom eatingRoom;
	Storage storage;

	std::vector<std::unique_ptr<Employee>> employees;

	bool isChange = false;

public: // ---- Constructors ----

	Sawmill(Forest& forest, DayNightCycle& timeCycle) : forest(forest), timeCycle(timeCycle), storage(timeCycle) {} 

	void init() {		
		for (int i=0; i<5; ++i) {
			addEmployee();
			// emp->changeState("Yey", i*3000.0, nullptr, [emp](){emp->finish();});
		}
	}

public: // ---- Main ----

	void update() {
		if(timeCycle.isNight() and isChange == true) {
			finishWorkChange();
		}
		else if(timeCycle.isDay() and isChange == false) {
			startNewWorkChange();
		}
	}

public: // ---- Threads methods ----

	void joinAll() {		
		for (auto& employee : employees) {
			employee->join();
		}
		storage.join();
	}

public: // ---- Control ----

	void finishWorkChange() {
		isChange = false;
		for (auto& employee : employees) {
			employee->finishJob();
		}
	}

	void startNewWorkChange() {
		isChange = true;
		for (auto& employee : employees) {
			employee->goToWork();
		}
	}

	void addEmployee() {		
		employees.emplace_back(new Employee(employees.size()+1, forest, storage, eatingRoom));
	}

	void finish() {
		for (auto& employee : employees) {
			employee->finish();
		}
		storage.finish();
	}

public: // ---- Getters ----

	std::mutex getStringStatusMutex;
	std::string getStringStatus() {
		std::lock_guard<std::mutex> lock(getStringStatusMutex);

        std::stringstream ss;
        
		ss << storage.getStringStatus();
		ss << '\n';
		ss << eatingRoom.getStringStatus();
		ss << '\n';

		int idx = 1;
		for (auto& employee : employees) {
			ss << idx << ' ' << employee->getStringStatus() << '\n';
			++idx;
		}
		
		return ss.str();
	}

	std::mutex getMapMutex;
	std::string getMap() {
		std::lock_guard<std::mutex> lock(getMapMutex);

		std::string map(100, '-');

		int homeIdx = 0;
		map[homeIdx] = 'H';

		int storageIdx = int(storage.getDistance()/WORLD_SIZE*100.0);
		map[storageIdx] = 'S';

		int eatingRoomIdx = int(eatingRoom.getDistance()/WORLD_SIZE*100.0);
		map[eatingRoomIdx] = 'E';

		return map;
	}

	std::mutex getEmployeesMapMutex;
	std::string getEmployeesMap() {
		std::lock_guard<std::mutex> lock(getEmployeesMapMutex);

		std::string map(100, '-');

		for(auto& employee : employees) {
			int mapIdx = int(employee->getCurrentDistance()/WORLD_SIZE*100.0);
			map[mapIdx] = char(employee->getIdx()%10 + '0');
		}

		return map;
	}

};

struct WorldManager {

	// Main fields

	Timer mainTimer;
	Timer displayTimer;
	bool finished = false;

    int key = 0;
    
	// Resources

	Forest forest;
	Sawmill sawmill;
	DayNightCycle timeCycle;

	WorldManager() : forest(timeCycle), sawmill(forest, timeCycle) {}

#ifdef NCURSES
    void initNcurses() {
        initscr();
        curs_set(0);
        cbreak();
        nodelay(stdscr, true);
        keypad(stdscr, true);
    }
	
	void finishNcurses() {  
        endwin();
    }

#endif // NCURSES
	
	void init() {

#ifdef NCURSES
        initNcurses();
#endif // NCURSES
        
		forest.generate();
		sawmill.init();

		mainTimer.setCallback([&](){finish();});
		mainTimer.start(50000.0, true);

		displayTimer.setCallback([&](){displayStatus();});
		displayTimer.start(100.0);

	}

	void run() {
		init();
		while(not finished) update();
		displayStatus();
		joinAll();
#ifdef NCURSES
        finishNcurses();
#endif // NCURSES
	}

	void update() {
		mainTimer.update();
		displayTimer.update();

		sawmill.update();
		forest.update();
    
#ifdef NCURSES
		// Press [LEFT] to close app
        if(key != -1) printw(std::to_string(key).c_str());
        key = getch();
		if(key  == 260) {
			finish();
		}
#endif // NCURSES
	}

	void joinAll() {
		sawmill.joinAll();
		timeCycle.join();
	}

	void finish() {
		finished = true;
		sawmill.finish();
		timeCycle.finish();
	}

	void displayStatus() {

#ifndef NCURSES
		//system("cls");
		system("clear");
        std::cout << getStringStatus() << std::flush;
#endif // not NCURSES

#ifdef NCURSES
        clear();
        printw(getStringStatus().c_str());
        refresh();
#endif // NCURSES

    }
        
	std::string getStringStatus() {        
        std::stringstream ss;
        
		ss << "Progress: " << std::floor(100.0*mainTimer.getProgress()) << " %\n\n";

		ss << timeCycle.getStringStatus() << "\n\n";

		ss << sawmill.getStringStatus();

		ss << '\n';
		ss << forest.getStringStatus();

		ss << "\n\n ------ MAPS ------ \n\n";
		ss << sawmill.getMap() << '\n' << sawmill.getEmployeesMap() << '\n' << forest.getMap();
        
        return ss.str();
	}

};

// --------------------- Main ---------------------

int main() {

	WorldManager wm;
	wm.run();
    
	std::cout<<"\nFinished!"<<std::flush;


}
