#include <iostream>

#include "MatrixGraph.hpp"
#include "ListGraph.hpp"
#include "Heap.hpp"
#include "Vector.hpp"
#include "FastVector.hpp"
#include "Menu.hpp"

#include <chrono>

std::chrono::high_resolution_clock::time_point getTime();

float getDuration(const auto& from, const auto& to);

template<typename Graph>
void runTest(int vs, float density);

int main() {

	Menu menu;
	menu.run();

// Measuring

	// std::cout<<"ListGraph:\n";
	// runTest<ListGraph>(10, 0.25);
	// runTest<ListGraph>(10, 0.5);
	// runTest<ListGraph>(10, 0.75);
	// runTest<ListGraph>(10, 0.99);
	// std::cout<<"----------------------\n\n";
	// runTest<ListGraph>(20, 0.25);
	// runTest<ListGraph>(20, 0.5);
	// runTest<ListGraph>(20, 0.75);
	// runTest<ListGraph>(20, 0.99);
	// std::cout<<"----------------------\n\n";
	// runTest<ListGraph>(30, 0.25);
	// runTest<ListGraph>(30, 0.5);
	// runTest<ListGraph>(30, 0.75);
	// runTest<ListGraph>(30, 0.99);
	// std::cout<<"----------------------\n\n";
	// runTest<ListGraph>(40, 0.25);
	// runTest<ListGraph>(40, 0.5);
	// runTest<ListGraph>(40, 0.75);
	// runTest<ListGraph>(40, 0.99);
	// std::cout<<"----------------------\n\n";
	// runTest<ListGraph>(50, 0.25);
	// runTest<ListGraph>(50, 0.5);
	// runTest<ListGraph>(50, 0.75);
	// runTest<ListGraph>(50, 0.99);
	// std::cout<<"----------------------\n\n";

// --Measuring


	// MatrixGraph g;
	// g.loadFromFile("dgraph.txt");
	// g.printEdges();
	// g.printMatrix();

	// std::cout<<"Prim:\n";
	// g.prim();
	// g.printEdges();
	// std::cout<<"Kruskal:\n";
	// g.kruskal();
	// g.printEdges();

	// Vector<int> shortestPathDijkstra(std::move(g.dijkstra(1, 3))); 
	// std::cout<<"Shortest path Dijkstra (1->3): ";
	// for(auto& obj : shortestPathDijkstra) {
	// 	std::cout<<obj<<' ';
	// }
	// std::cout<<'\n';

	// Vector<int> shortestPathBellmanFord(std::move(g.bellmanFord(1, 3))); 
	// std::cout<<"Shortest path Bellman-Ford (1->3): ";
	// for(auto& obj : shortestPathDijkstra) {
	// 	std::cout<<obj<<' ';
	// }
	// std::cout<<'\n';

	// Final test

	// for(int i=0; i<100; ++i) {
		// g.generateDoubleGraph(10, 20, 0.5f);
		// g.printEdges();
		// std::cout<<"--------------\n";

		// Vector<int> shortestPath(std::move(g.dijkstra(1, 10))); 
		// std::cout<<"Shortest path (1->5): ";
		// for(auto& obj : shortestPath) {
		// 	std::cout<<obj+1<<' ';
		// }
		// std::cout<<'\n';
	// }
}

std::chrono::high_resolution_clock::time_point getTime() {
	return std::chrono::high_resolution_clock::now();
}

float getDuration(const auto& from, const auto& to) {

	return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(to-from).count());
}

template<typename Graph>
void runTest(int vs, float density) {

	float p = 0;
	float k = 0;
	float d = 0;
	float bf = 0;

	Graph graph;

	std::cout<<"\nV = "<<vs<<" density = "<<density<<'\n';

	std::chrono::high_resolution_clock::time_point begin, end;
	for(int i=0; i<100; ++i) {

		// Prim
		graph.generateDoubleGraph(vs, 100, density, 2077);
		begin = getTime();
		graph.prim();
		end = getTime();
		p += getDuration(begin, end);

		// Kruskal
		graph.generateDoubleGraph(vs, 100, density, 2077);
		begin = getTime();
		graph.kruskal();
		end = getTime();
		k += getDuration(begin, end);

		// Dijkstra
		graph.generateDoubleGraph(vs, 100, density, 2077);
		begin = getTime();
		graph.dijkstra(1, vs);
		end = getTime();
		d += getDuration(begin, end);
		
		// Bellman-Ford
		graph.generateDoubleGraph(vs, 100, density, 2077);
		begin = getTime();
		graph.bellmanFord(1, vs);
		end = getTime();
		bf += getDuration(begin, end);
	}

	std::cout<<"Prim = "<<p/100.f<<" micro s\n";
	std::cout<<"Kruskal = "<<k/100.f<<" micro s\n";
	std::cout<<"Dijkstra = "<<d/100.f<<" micro s\n";
	std::cout<<"Bellman-Ford = "<<bf/100.f<<" micro s\n";

}
