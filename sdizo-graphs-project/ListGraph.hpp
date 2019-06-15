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
#include "List.hpp"

class ListGraph {
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

	struct Edge {
		int v=0;
		int value=0;
		Edge() {}
		Edge(int v, int value) : v(v), value(value) {}
		Edge(const Edge&) = default;
	};

private:

	List<List<Edge>> list;
	int vs = 0; // number of vertices

	bool doubleSide = false;

public:

	void clear() {		
		list.clear();
		vs = 0;
	}

	void setDoubleSide(bool flag) {
		doubleSide = flag;
	}

	// For v = 1,2,3,...
	void addVertex(int v) {
		--v;
		if(v>=vs) {
			for(int i=vs; i<=v; ++i) {
				list.push_back(List<Edge>());				
			}
			vs = v+1;
		}
	}

	// For v1,v2 = 1,2,3,...
	// Edge* getEdge(int v1, int v2) {
	// 	--v1;
	// 	--v2
	// 	return getEdgeRI(v1,v2);
	// }

	// Get edge by Real Indexes of vertices
	Edge* getEdgeRI(int v1, int v2) {
		if(v1<vs && v2<vs) {
			for(auto& e : list[v1]) {
				if(e.v == v2) return &e;				
			}
		}
		return nullptr;
	}

	void addEdge(int v1, int value, int v2) {
		if(v1<=0 || v2<=0) return;

		if(doubleSide == false) {
			addSingleEdge(v1, value, v2);
		}
		else {
			addSingleEdge(v1, value, v2);
			if(v1!=v2) addSingleEdge(v2, value, v1);
		}
	}

	void addSingleEdge(int v1, int value, int v2) {
		// Resize list
		if(v1>v2) {
			addVertex(v1);
		}
		else { 
			addVertex(v2);
		}

		--v1;
		--v2;
		bool updateEdge = false;
		for(auto& edge : list[v1]) {
			if(edge.v == v2) {
				edge.value = value;
				updateEdge = true;
				break;
			}
		}
		if(updateEdge == false) {
			list[v1].push_back(Edge(v2, value));
		}
	}
	
	void printEdges() {
		if(vs == 0) return;
		std::stringstream ss;
		int vi=0;
		int ei=1;
		for(auto& v : list) {
			for(auto& e : v) {
				ss<<ei<<": "<<vi+1<<" - "<<e.value<<" -> "<<e.v+1<<'\n';
				++ei;
			}
			++vi;
		}
		std::cout<<ss.str();
	}

	void removeEdge(int v1, int v2) {
		if(v1<=0 || v2<=0) return;
		--v1;
		--v2;
		if(v1<vs && v2<vs) {
			int i=0;
			for(auto& e : list[v1]) {
				if(e.v == v2) {
					list[v1].pop(i);
					break;
				}
				++i;
			}
			if(doubleSide == true) {
				i=0;
				for(auto& e : list[v2]) {
					if(e.v == v1) {
						list[v2].pop(i);
						break;
					}
					++i;
				}
			}
		}
	}

	void clearEdges() {
		for(auto& v : list) {
			v.clear();
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

	// Algorithms

	void prim() {

		if(!doubleSide) return;

		Vector<int> visited;
		Vector<GraphNode> acceptedEdges;
		MinHeap<GraphNode> edges;

		visited.push_back(0);
		for(int i=0; i<vs; ++i) {
			if(auto* edge = getEdgeRI(0, i); edge != nullptr) {
				edges.insert(GraphNode(0,edge->value,i));
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
					if(auto* edge = getEdgeRI(node.v2, i); edge != nullptr) {
						edges.insert(GraphNode(node.v2,edge->value,i));
					}
				}
			}

			//DEBUG: Show accepted edges
			// std::cout<<"-------------------\n";
			// for(auto& edge : acceptedEdges) {
			// 	std::cout<<edge.v1<<' '<<edge.value<<' '<<edge.v2<<'\n';
			// }
		}

		// Create final graph
		clearEdges();
		for(auto& edge : acceptedEdges) {
			addEdge(edge.v1+1, edge.value, edge.v2+1); // Double side
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
				if(auto* edge = getEdgeRI(i,j); edge != nullptr && i!=j) {
					L.insert(GraphNode(i, edge->value, j));
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
			addEdge(edge.v1+1, edge.value, edge.v2+1); // Double side
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
				if(auto* edge = getEdgeRI(currentVertex, n); edge!=nullptr && currentVertex!=n) {
					if(tab[currentVertex].dis+edge->value<tab[n].dis) {
						tab[n].dis = tab[currentVertex].dis+edge->value;
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
					if(auto* edge = getEdgeRI(u,v); edge!=nullptr && u!=v) {
						// jeżeli d[v] > d[u] + w(u,v) to
						if(dis[u]!=std::numeric_limits<int>::max() && dis[v]>dis[u]+edge->value) {
							// d[v] = d[u] + w(u,v)
							dis[v] = dis[u]+edge->value;
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