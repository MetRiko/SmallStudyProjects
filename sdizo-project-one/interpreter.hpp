#include <iostream>
#include <sstream>

#include "table.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "red_black_tree.hpp"

struct Interpreter {

	sdizo::Table table;
	sdizo::MaxHeap heap;
	sdizo::List list;
	sdizo::RedBlackTree rbt;

	int currentStruct = 0;
	bool isRunning = false;
	bool autoShow = true;

	void run() {
		isRunning = true;
		std::cout<<"SDiZO - Data structures interpreter.\n";
		std::cout<<"Available structures:\n";
		std::cout<<"   1. dynamic table [table]\n";
		std::cout<<"   2. double-linked list [list]\n";
		std::cout<<"   3. max-heap [heap]\n";
		std::cout<<"   4. red-black tree [rbt]\n\n";
		std::cout<<"Type 'help' for information about available commands.\n";

		std::string line;
		while(isRunning) {
			std::cout<<"\n>>> ";
			std::getline(std::cin, line);
			std::cout<<'\n';
			run_command(line);
		}
		std::cout<<"[Interpeter closed]\n";
	}

	void load_vector_from_file(const std::string& filename, std::vector<int>& vec) {
		std::ifstream file(filename);
		if(file.good()) {
			int value;
			while(file>>value) {
				vec.push_back(value);
			}
			file.close();
		}
		else {
			std::cout<<"[Error]\n";
		}
	}

	void print_help() {
		std::stringstream stream;
		stream<<"----- Help -----\n";
		stream<<"Usage: [option]\n";
		stream<<"   help\t\t\t\tShow information about available commands\n";
		stream<<"   close\t\t\tClose interpreter\n";
		stream<<"   auto_show [flag=1|0]\t\tAlways show structure after any operation.\n\n";
		stream<<"Usage: switch [option]\n";
		stream<<"   table\t\t\tChange data structure on dynamic table\n";
		stream<<"   list\t\t\t\tChange data structure on double-linked list\n";
		stream<<"   heap\t\t\t\tChange data structure on max-heap\n";
		stream<<"   rbt\t\t\t\tChange data structure on red-black tree\n\n";
		stream<<"When [table]/[list] is switched:\nUsage: [option]\n";
		stream<<"   push_back [value]\t\tPush [value] on back\n";
		stream<<"   push_front [value]\t\tPush [value] on front\n";
		stream<<"   push [value] [pos]\t\tPush [value] on position [pos]\n";
		stream<<"   pop_back\t\t\tPop element from back\n";
		stream<<"   pop_front\t\t\tPop element from front\n";
		stream<<"   pop [pos]\t\t\tPop element from position [pos]\n";
		stream<<"   find [value]\t\t\tSearch [value] in structure\n";
		stream<<"   clear\t\t\tRemove all elements from structure\n";
		stream<<"   show\t\t\t\tDisplay structure content\n";
		stream<<"   load [file=*.txt|*.*]\tAdd all values from file to structure\n\n";
		stream<<"When [heap] is switched:\nUsage: [option]\n";
		stream<<"   insert [value]\t\tInsert [value] to heap\n";
		stream<<"   extract_max\t\t\tRemove the maximum element and return its value\n";
		stream<<"   find [value]\t\t\tSearch [value] in structure\n";
		stream<<"   clear\t\t\tRemove all elements from structure\n";
		stream<<"   show\t\t\t\tDisplay structure content\n";
		stream<<"   load [file=*.txt|*.*]\tAdd all values from file to structure\n\n";
		stream<<"When [rbt] is switched:\nUsage: [option]\n";
		stream<<"   insert [value]\t\tInsert [value] to heap\n";
		stream<<"   remove [value]\t\tRemove [value] from structure\n";
		stream<<"   find [value]\t\t\tSearch [value] in structure\n";
		stream<<"   clear\t\t\tRemove all elements from structure\n";
		stream<<"   show\t\t\t\tDisplay structure content\n";
		stream<<"   load [file=*.txt|*.*]\tAdd all values from file to structure\n\n";
		stream<<"----------------\n";
		std::cout<<stream.str();
	}

	void run_command(const std::string& command) {
		std::stringstream ss;
		std::string cmd;
		ss<<command;
		ss>>cmd;

		if(cmd == "help") {
			print_help();
		}
		else if(cmd == "close") {
			isRunning = false;
		}
		else if(cmd == "auto_show") {
			int flag;
			ss>>flag;
			autoShow = flag;
			std::cout<<"Auto-show status: "<<(autoShow ? "on" : "off")<<'\n';
		}
		else if(cmd == "switch") {
			ss>>cmd;
			if(cmd == "table") { 
				currentStruct = 0;
				std::cout<<"Switched on [table]:\npush_back | push_front | push | pop_back | pop_front | pop | \nfind | clear | show | load\n";
			}
			else if(cmd == "list") { 
				currentStruct = 1;
				std::cout<<"Switched on [list]:\npush_back | push_front | push | pop_back | pop_front | pop | \nfind | clear | show | load\n";
			}
			else if(cmd == "heap") { 
				currentStruct = 2;
				std::cout<<"Switched on [heap]:\ninsert | extract_max | find | clear | show | load\n";
			}
			else if(cmd == "rbt") { 
				currentStruct = 3;
				std::cout<<"Switched on [rbt]:\ninsert | remove | find | clear | show | load\n";
			}
			else std::cout<<"[Error]\n";
		}
		else {			
		// table
			bool errorFlag = false;
			if(currentStruct == 0) {
				int a,b;
				if(cmd == "push_back") {
					ss>>a;
					table.push_back(a);
				}
				else if(cmd == "push_front") {
					ss>>a;
					table.push_front(a);
				}
				else if(cmd == "push") {
					ss>>a;
					ss>>b;
					table.push(a, b);
				}
				else if(cmd == "pop_back") {
					table.pop_back();				
				}
				else if(cmd == "pop_front") {	
					table.pop_front();
				}
				else if(cmd == "pop") {
					ss>>a;
					table.pop(a);
				}
				else if(cmd == "find") {
					ss>>a;
					auto* f = table.find(a);
					if(f) std::cout<<"Founded: "<<*f<<'\n';
					else std::cout<<"Could not be found.\n";
				}
				else if(cmd == "clear") {
					table.clear();
				}
				else if(cmd == "show") {
					std::cout<<"----- Table -----\n";
					table.set_stream(std::cout);
					std::cout<<"-----------------\n";
					errorFlag = true;
				}
				else if(cmd == "load") {
					ss>>cmd;
					std::vector<int> vec;
					load_vector_from_file(cmd, vec);
					for(auto& obj : vec) {
						table.push_back(obj);
					}
				}
				else {
					std::cout<<"[Error]\n";
					errorFlag = true;
				}
				if(!errorFlag && autoShow) {					
					std::cout<<"----- Table -----\n";
					table.set_stream(std::cout);
					std::cout<<"-----------------\n";
				}
			}
		// list
			else if(currentStruct == 1) {
				int a,b;
				if(cmd == "push_back") {
					ss>>a;
					list.push_back(a);
				}
				else if(cmd == "push_front") {
					ss>>a;
					list.push_front(a);
				}
				else if(cmd == "push") {
					ss>>a;
					ss>>b;
					list.push(a, b);
				}
				else if(cmd == "pop_back") {
					list.pop_back();				
				}
				else if(cmd == "pop_front") {	
					list.pop_front();
				}
				else if(cmd == "pop") {
					ss>>a;
					list.pop(a);
				}
				else if(cmd == "find") {
					ss>>a;
					auto f = list.find(a);
					if(!f.is_null()) std::cout<<"Founded: "<<*f<<'\n';
					else std::cout<<"Could not be found.\n";
				}
				else if(cmd == "clear") {
					list.clear();
				}
				else if(cmd == "show") {
					std::cout<<"----- List -----\n";
					list.set_stream(std::cout);
					std::cout<<"-----------------\n";
					errorFlag = true;
				}
				else if(cmd == "load") {
					ss>>cmd;
					std::vector<int> vec;
					load_vector_from_file(cmd, vec);
					for(auto& obj : vec) {
						list.push_back(obj);
					}
				}
				else {
					std::cout<<"[Error]\n";
					errorFlag = true;
				}
				if(!errorFlag && autoShow) {					
					std::cout<<"----- List -----\n";
					list.set_stream(std::cout);
					std::cout<<"-----------------\n";
				}
			}
		// heap
			else if(currentStruct == 2) {
				int a;
				if(cmd == "insert") {
					ss>>a;
					heap.insert(a);
				}
				else if(cmd == "extract_max") {
					if(heap.get_size()) std::cout<<"Extracted maximum: "<<heap.extract_max()<<'\n';
					else std::cout<<"Heap is empty.\n";
				}
				else if(cmd == "find") {
					ss>>a;
					auto* f = heap.find(a);
					if(f) std::cout<<"Founded: "<<*f<<'\n';
					else std::cout<<"Could not be found.\n";
				}
				else if(cmd == "clear") {
					heap.clear();
				}
				else if(cmd == "show") {
					std::cout<<"----- Max heap -----\n";
					heap.set_stream(std::cout);
					std::cout<<"-----------------\n";
					errorFlag = true;
				}
				else if(cmd == "load") {
					ss>>cmd;
					std::vector<int> vec;
					load_vector_from_file(cmd, vec);
					for(auto& obj : vec) {
						heap.insert(obj);
					}
				}
				else {
					std::cout<<"[Error]\n";
					errorFlag = true;
				}
				if(!errorFlag && autoShow) {					
					std::cout<<"----- Max heap -----\n";
					heap.set_stream(std::cout);
					std::cout<<"-----------------\n";
				}
			}
		// rbt
			else if(currentStruct == 3) {
				int a;
				if(cmd == "insert") {
					ss>>a;
					rbt.insert(a);
				}
				else if(cmd == "remove") {
					ss>>a;
					if(rbt.get_size()) rbt.remove(a);
					else std::cout<<"Tree is empty.\n";
				}
				else if(cmd == "find") {
					ss>>a;
					auto* f = rbt.find(a);
					if(f) std::cout<<"Founded: "<<f->value<<'\n';
					else std::cout<<"Could not be found.\n";
				}
				else if(cmd == "clear") {
					rbt.clear();
				}
				else if(cmd == "show") {
					std::cout<<"----- Red-black tree -----\n";
					rbt.set_stream(std::cout);
					std::cout<<"-----------------\n";
					errorFlag = true;
				}
				else if(cmd == "load") {
					ss>>cmd;
					std::vector<int> vec;
					load_vector_from_file(cmd, vec);
					for(auto& obj : vec) {
						rbt.insert(obj);
					}
				}
				else {
					std::cout<<"[Error]\n";
					errorFlag = true;
				}
				if(!errorFlag && autoShow) {					
					std::cout<<"----- Red-black tree -----\n";
					rbt.set_stream(std::cout);
					std::cout<<"-----------------\n";
				}
			}
		}
	}

};