#include <iostream>
#include <fstream>

#include "table.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "red_black_tree.hpp"

#include "vector_tester.hpp"
#include "heap_tester.hpp"
#include "tree_tester.hpp"

#include "interpreter.hpp"

void measure_time(int tests, std::initializer_list<int> capacities, std::ostream& stream) {
	VectorTester<sdizo::Table> tableTester;
	VectorTester<sdizo::List> listTester;
	HeapTester<sdizo::MaxHeap> heapTester;
	TreeTester<sdizo::RedBlackTree> rbtTester;

	for(auto& capacity : capacities) {
		stream<<"===== Capacity : "<<capacity<<" =====\n";
		// stream<<"   [Table]:\n";
		// tableTester.run_test(tests, capacity, stream);
		// stream<<"   [List]:\n";
		// listTester.run_test(tests, capacity, stream);
		stream<<"   [Heap]:\n";
		heapTester.run_test(tests, capacity, stream);
		stream<<"   [Red-black tree]:\n";
		rbtTester.run_test(tests, capacity, stream);
	}
}

int main() {

// Uncomment for interpreter
	Interpreter interpreter;
	interpreter.run();

// Uncomment for test
	// std::ofstream file("tree_results.txt");
	// file.clear();
	// measure_time(100, {500,1000,2000,5000/*,10000,20000*/}, file);
	// file.close();
}