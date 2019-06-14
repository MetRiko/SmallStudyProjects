#pragma once

#include <iostream>
#include <string>

#include "matrix.hpp"

template <bool SHOW_RESULT = false>
class BxBTSP {
	Matrix baseMat;
	Matrix reducedBaseMat;
	int size;
	sdizo::MinHeap mainQueue;
	sdizo::UnorderedVector<std::pair<int,int>> bannedConnections;
	sdizo::UnorderedVector<int> path;
	int minCost = 0;

public:

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

	void start() {

		bannedConnections.clear();

		while(!_reset());

		path.push_back(path.front());

		minCost = 0;
		if constexpr(SHOW_RESULT == true) {
			std::cout<<"Sciezka: ";
		}

		for(int i=1; i<path.getSize(); ++i) {
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

private:

	bool _reset() {

		mainQueue.clear();
		path.clear();

		Matrix mat(baseMat);
		int prevCost = mat.reduce();
		reducedBaseMat.set(mat);

		int vertex = 0;
		path.push_back(vertex);

		sdizo::UnorderedVector<int> queue;
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

				bannedConnections.push_back(std::pair<int,int>(path[path.getSize()-2], path.back()));

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

	// return best vertex and replace target on reduced matrix with pair with best vertex and remove the vertex from vector
	int _calculateLowMatrix(Matrix &target, int firstVertex, sdizo::UnorderedVector<int>& secondVertices, int &prevCost) {		

		sdizo::MinHeap costs;
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
			int lowCost = _calculateReducedMatrixForPair(lowMat, firstVertex, secondVertex) + reducedBaseMat[firstVertex][secondVertex] + prevCost;

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
		secondVertices.erase(minIndex);

		if(!mainQueue.empty() and minLowCost > mainQueue.top()) {
			return -1;
		}
		else {
			mainQueue.clear();
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
