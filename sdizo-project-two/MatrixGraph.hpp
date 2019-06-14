#pragma once

// class MatrixGraph {
// public:
// 	int x = 1;
// };

#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include "Heap.hpp"
#include "Vector.hpp"
#include "FastVector.hpp"

class MatrixGraph {
public:

	struct GraphNode {
		int v1 = 0, value = 0, v2 = 0;
		GraphNode(int v1, int value, int v2) : v1(v1), value(value), v2(v2) {}
		GraphNode(const GraphNode& node) : v1(node.v1), value(node.value), v2(node.v2) {}
		GraphNode() {}
		bool operator<(GraphNode& node) { return value<node.value; }
		bool operator>(GraphNode& node) { return value>node.value; }
		bool operator<=(GraphNode& node) { return value<=node.value; }
		bool operator>=(GraphNode& node) { return value>=node.value; }
		friend std::ostream& operator<<(std::ostream& out, GraphNode& node) {
			out<<'('<<node.v1<<"- "<<node.value<<" ->"<<node.v2<<')';
			return out;
		}
	};

private:

	int **matrix = nullptr;

	int vs = 0; // number of vertices

	bool doubleSide = true;

public:

	~MatrixGraph() {
		clear();
	}

	void clear() {		
		if(matrix) {
			for(int i=0; i<vs; ++i) {
				delete[] matrix[i];
			}
			delete[] matrix;
			matrix = nullptr;
			vs = 0;
		}
	}

	void setDoubleSide(bool flag) {
		doubleSide = flag;
	}

	void clearEdges() {		
		if(matrix) {
			for(int i=0; i<vs; ++i) {				
				for(int j=0; j<vs; ++j) {
					matrix[i][j] = 0;
				}
			}
		}
	}

	/// For 1,2,3... vertices (not zero)
	void addEdge(int v1, int value, int v2) {
		if(v1<=0 || v2<=0) return;
		addVertex(v1);
		addVertex(v2);
		--v1;
		--v2; 
		matrix[v1][v2] = value;
		if(doubleSide) {
			matrix[v2][v1] = value;
		}
	}

	/// For 1,2,3... vertices (not zero)
	void removeEdge(int v1, int v2) {
		if(v1<=0 || v2<=0) return;
		--v1;
		--v2;
		if(matrix && v1<vs && v2<vs) {
			matrix[v1][v2] = 0;
			if(doubleSide) {
				matrix[v2][v1] = 0;
			}
		}
	}

	/// For 1,2,3... vertices (not zero)
	void addVertex(int v) {
		--v;
		if(matrix == nullptr || v>=vs) {
			int newVs = v+1;
			int **mat = new int*[newVs];
			for(int i=0; i<newVs; ++i) {
				mat[i] = new int[newVs]{};
			}
			for(int i=0; i<vs; ++i) {
				for(int j=0; j<vs; ++j) {
					mat[i][j] = matrix[i][j];
				}
			}
			clear();
			matrix = mat;
			vs = newVs;
		}
	}

	void loadFromFile(const std::string& path) {
		std::fstream file;
		file.open(path, std::ios::in);
		if(file.good()) {
			clear();
			int e, v;
			file>>doubleSide>>e>>v;
			addVertex(v);
			int v1,value,v2;
			for(int i=0; i<e; ++i) {
				file>>v1>>value>>v2;
				addEdge(v1,value,v2);
			}
		}
	}

	void printEdges() {
		if(matrix == nullptr) return;
		int c = 1;
		std::stringstream ss;
		if(doubleSide == false) {
			for(int i=0; i<vs; ++i) {
				for(int j=0; j<vs; ++j) {
					if(matrix[i][j] != 0) {
						ss<<c<<": "<<i+1<<" - "<<matrix[i][j]<<" -> "<<j+1<<'\n';
						++c;					
					}
				}
			}	
		}
		else {
			for(int i=0; i<vs; ++i) {
				for(int j=i; j<vs; ++j) {
					if(matrix[i][j] != 0) {
						ss<<c<<": "<<i+1<<" <- "<<matrix[i][j]<<" -> "<<j+1<<'\n';
						++c;					
					}
				}
			}
		}
		std::cout<<ss.str();
	}


	void printMatrix() {
		if(matrix == nullptr) return;
		std::stringstream ss;
		for(int i=0; i<vs; ++i) {
			ss<<std::setw(2)<<std::left<<i+1<<" >^ ";
			for(int j=0; j<vs; ++j) {
				if(matrix[i][j]) {
					ss<<std::setw(3)<<matrix[i][j]<<' ';
				}
				else {
					ss<<" +  ";
				}
			}
			ss<<'\n';
		}
		std::cout<<ss.str();
	}

	void prim() {

		if(!doubleSide) return;

		Vector<int> visited;
		Vector<GraphNode> acceptedEdges;
		MinHeap<GraphNode> edges;

		visited.push_back(0);
		for(int i=0; i<vs; ++i) {
			if(matrix[0][i]!=0) {
				edges.insert(GraphNode(0,matrix[0][i],i));
			}
		}

		while(visited.size()<vs) {
			auto node = edges.get_min();
			bool isVisited = false;
			for(auto& obj : visited) {
				if(obj == node.v2 || node.v1 == node.v2) {
					edges.extract_min();
					isVisited = true;
					break;
				}
			}
			if(isVisited == false) {
				visited.push_back(node.v2);
				acceptedEdges.push_back(node);
				for(int i=0; i<vs; ++i) {
					if(matrix[node.v2][i]!=0) {
						edges.insert(GraphNode(node.v2,matrix[node.v2][i],i));
					}
				}
			}
		}
		clearEdges();
		for(auto& edge : acceptedEdges) {
			matrix[edge.v1][edge.v2] = edge.value;
			matrix[edge.v2][edge.v1] = edge.value;
		}
	}

	void kruskal() {

		if(!doubleSide) return;

		MinHeap<GraphNode> L;
		Vector<std::pair<int, int>> V; // added vertices (group index, vertex iindex)
		Vector<GraphNode> T;
		int nextGroup = 1;
		int groupCounter = 1;

		for(int i=0; i<vs; ++i) {
			for(int j=i; j<vs; ++j) {
				if(matrix[i][j]!=0 && i!=j) {
					L.insert(GraphNode(i, matrix[i][j], j));
				}
			}
		}

		while(L.get_size()) {
			if(V.size() == vs && groupCounter == 2) {
				break;
			}

			// DEBUG:
			// std::cout<<"-----------------\n";
			// for(auto& obj : V) {
			// 	std::cout<<obj.first<<" : "<<groupCounter<<'\n';
			// }

			GraphNode node = L.extract_min();

			//Check vertices
			int groupLeft = 0;
			for(auto& vertex : V) {
				if(vertex.second == node.v1) {
					groupLeft = vertex.first;
					break;
				}
			}
			int groupRight = 0;
			for(auto& vertex : V) {
				if(vertex.second == node.v2) {
					groupRight = vertex.first;
					break;
				}
			}
			if(groupLeft != 0 && groupRight != 0) {
				if(groupLeft != groupRight) {
					for(auto& vertex : V) {
						if(vertex.first == groupRight) {
							vertex.first = groupLeft;
						}
					}					
					T.push_back(node);
					--groupCounter;
				}
			}
			else if(groupLeft != 0 && groupRight == 0) {
				V.push_back(std::make_pair(groupLeft, node.v2));
				T.push_back(node);
			}
			else if(groupLeft == 0 && groupRight != 0) {
				V.push_back(std::make_pair(groupRight, node.v1));
				T.push_back(node);
			}
			else if(groupLeft == 0 && groupRight == 0) {
				V.push_back(std::make_pair(nextGroup, node.v1));
				V.push_back(std::make_pair(nextGroup, node.v2));
				T.push_back(node);
				++nextGroup;
				++groupCounter;
			}
		}

		clearEdges();
		for(auto& edge : T) {
			matrix[edge.v1][edge.v2] = edge.value;
			matrix[edge.v2][edge.v1] = edge.value;
		}
	}

	// src, dest - vertices from -> to
	
	struct DijkstraNode {
		int v=-1, dis=std::numeric_limits<int>::max(), prev=-1;
		bool visited = false;
	};

	Vector<int> dijkstra(int src, int dest) {
		--src;
		--dest;
		if(src>=vs || dest>=vs || src<0 || dest<0) return Vector<int>();

		DijkstraNode tab[vs];

		for(int i=0; i<vs; ++i) {
			tab[i].v = i;
		}	

		tab[src].dis = 0;
		int currentVertex = src;
		int verticesCounter = 0; // Number of visited vertices

		// Algorithm

		tab[currentVertex].visited = true;
		while(verticesCounter<vs) {
			// Neighbours
			for(int n=0; n<vs; ++n) {
				if(matrix[currentVertex][n]!=0 && currentVertex!=n) {
					if(tab[currentVertex].dis+matrix[currentVertex][n]<tab[n].dis) {
						tab[n].dis = tab[currentVertex].dis+matrix[currentVertex][n];
						tab[n].prev = currentVertex;
					}
				}
			}
			// Take next vertex
			int min = std::numeric_limits<int>::max();
			for(int i=0; i<vs; ++i) {
				if(tab[i].dis<min && tab[i].visited == false) {
					min = tab[i].dis;
					currentVertex = i;
				}
			}
			tab[currentVertex].visited = true;
			++verticesCounter;
			

			// DEBUG: Show dijkstra table
			// std::cout<<"----------------------\n";
			// for(int i=0; i<vs; ++i) {
			// 	std::cout<<tab[i].v<<' '<<tab[i].dis<<' '<<tab[i].prev<<' '<<tab[i].visited<<'\n';
			// }
		}

		// Shortest path

		Vector<int> shortestPath;

		int current = dest;
		shortestPath.push_front(current+1);
		while(current != src) {
			current = tab[current].prev;			
			shortestPath.push_front(current+1);
		}
		return shortestPath;
	}

	// SOURCE: https://pl.wikipedia.org/wiki/Algorytm_Bellmana-Forda
	Vector<int> bellmanFord(int src, int dest) {
		--src;
		--dest;
		if(src>=vs || dest>=vs || src<0 || dest<0) return Vector<int>();

		// Bellman-Ford(G,w,s):

		int dis[vs];
		int prev[vs];

		// dla każdego wierzchołka v w V[G] wykonaj
		for(int i=0; i<vs; ++i) {
			//   d[v] = nieskończone
			dis[i] = std::numeric_limits<int>::max();
			//   poprzednik[v] = niezdefiniowane
			prev[i] = -1;
		}

		// d[s] = 0
		dis[src] = 0;

		// dla i od 1 do |V[G]| - 1 wykonaj
		for(int i=1; i<=vs; ++i) {
			// Nighbours
			// dla każdej krawędzi (u,v) w E[G] wykonaj
			for(int u=0; u<vs; ++u) {
				for(int v=0; v<vs; ++v) {
					if(matrix[u][v]!=0 && u!=v) {
						// jeżeli d[v] > d[u] + w(u,v) to
						if(dis[u]!=std::numeric_limits<int>::max() && dis[v]>dis[u]+matrix[u][v]) {
							// d[v] = d[u] + w(u,v)
							dis[v] = dis[u]+matrix[u][v];
							// poprzednik[v] = u
							prev[v] = u;

							// Check for negative-weight cycles
							if (i == vs) {
								// ERROR: Graph contains a negative-weight cycle
								return Vector<int>();
							}
						}
					}
				}
			}
		}

		// DEBUG: Show dis and prev tables
		// std::cout<<"------------------\n";
		// for(int i=0; i<vs; ++i) {
		// 	std::cout<<i<<' '<<dis[i]<<' '<<prev[i]<<'\n';
		// }

		// Shortest path
		Vector<int> shortestPath;
		int current = dest;
		shortestPath.push_front(current+1);
		while(current != src) {
			current = prev[current];			
			shortestPath.push_front(current+1);
		}
		return shortestPath;
	}

	// Generating graph
	void generateDoubleGraph(int vertices, int maxWeight, float density, unsigned seed) {
		if(vertices<=0) return;
		if(density<0.f) density = 0.f;
		else if(density>1.f) density = 1.f;
		std::srand(seed);

		clear();
		setDoubleSide(true);
		addVertex(vertices); // set vs=vertices

		// Form cohesion
		for(int i=1; i<vs; ++i) {
			addEdge(i, 1+rand()%maxWeight, i+1);
		}

		FastVector<std::pair<int,int>> pairs;
		for(int i=0; i<vs; ++i) {
			for(int j=i+2; j<vs; ++j) {
				pairs.push_back(std::make_pair(i,j));
			}
		}

		int maxEdges = vs*(vs-1)/2;
		int edges = density*maxEdges-(vs-1);

		if(edges>0) {
			// shuffle
			int a, b;
			std::pair<int,int> temp;
			for(int i=0; i<maxEdges; ++i) {
				a = rand()%pairs.size();
				b = rand()%pairs.size();
				temp = pairs[a];
				pairs[a] = pairs[b];
				pairs[b] = temp;
			}

			for(int i=0; i<edges; ++i) {
				auto& e = pairs[i];
				addEdge(e.first+1, 1+rand()%maxWeight, e.second+1);
			}
		}
	}
};
