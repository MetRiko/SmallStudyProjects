#pragma once

namespace sdizo {
	struct List {
		using Type = int;
		struct Node {
			Type data;
			Node *prev = nullptr;
			Node *next = nullptr;
			Node(Type data,	Node *prev, Node *next) : data(data), prev(prev), next(next) {;}
			Node(Type newData) : data(newData) {;}
			Node() {;}
		};
		
		Node *root = nullptr;
		Node *last = nullptr;
		int size = 0;
		
		~List() {
			clear();
		}
		
	// *etters
		int get_size() {
			return size;
		}

	// Modifiers
		
		void clear() {
			Node *node = root;
			while(node) {
				Node *toRemove = node;
				node = node->next;
				delete toRemove;
			}
			size = 0;
			root = nullptr;
			last = nullptr;
		}
		
		void push(Type element, int pos) {
			if(root) {
				if(pos<=0) 
				{
					Node *newNode = new Node(element, nullptr, root);
					root->prev = newNode;
					root = newNode;
				}
				else if(pos >= size) {					
					Node *newNode = new Node(element, last, nullptr);
					last->next = newNode;
					last = newNode;
				}
				else {
					Node *newNode = new Node(element);
					Node *node = root;
					while(pos--) {
						node = node->next;
					}
					Node *left = node->prev;
					Node *right = node;
					left->next = newNode;
					right->prev = newNode;
					newNode->prev = left;
					newNode->next = right;
				}
			}
			else {
				root = new Node(element);
				last = root;
			}
			++size;
		}
		
		void push_back(Type element) {
			push(element, size);
		}
		
		void push_front(Type element) {
			push(element, 0);
		}
		
		void pop(int pos) {
			if(!root) return;
			if(size == 1) {
				delete root;
				root = nullptr;
				last = nullptr;
			}
			else if(pos<=0) {
				Node *toRemove = root;
				root = root->next;
				root->prev = nullptr;
				if(root == nullptr) last = nullptr;
				delete toRemove;
			}
			else if(pos>=size-1) {					
				Node *toRemove = last;
				std::cout<<'a';
				last = last->prev;
				std::cout<<'b';
				last->next = nullptr;
				std::cout<<'c';
				if(last == nullptr) root = nullptr;
				std::cout<<'d';
				delete toRemove;
				std::cout<<'e';
			}
			else {
				Node *node = root;
				while(pos--) {
					node = root->next;
				}					
				Node *left = node->prev;
				Node *right = node->next;
				left->next = right;
				right->prev = left;
				delete node;					
			}
			--size;
		}
		
		void pop_back() {
			pop(size-1);
		}
		
		void pop_front() {
			pop(0);
		}
		
	// Iterators
		
		
		struct Iterator
        {
			Iterator(Node *ptr) : ptr(ptr) { }
			Iterator operator++() { Iterator ret = *this; ptr = ptr->next; return ret; }
			Iterator operator++(int junk) { ptr = ptr->next; return *this; }
			Type& operator*() { return ptr->data; }
			Type* operator->() { return &ptr->data; }
			bool operator==(const Iterator& itr) { return ptr == itr.ptr; }
			bool operator!=(const Iterator& itr) { return ptr != itr.ptr; }
			bool is_null() {return ptr == nullptr;}
		private:
			Node* ptr;
        };
		using iterator = Iterator;
		
		Iterator begin() {
			return Iterator(root);
		}
		Iterator end() {
			return Iterator(nullptr);
		}
		Iterator rbegin() {
			return Iterator(last);
		}
		Iterator rend() {
			return Iterator(nullptr);
		}		

	// Search
		Iterator find(Type value) {
			if(!root) return Iterator(nullptr);
			Node* itr = root;
			while(itr) {
				if(itr->data == value) return Iterator(itr);
				itr = itr->next;
			}
			return Iterator(nullptr);
		}

	// Display
	
		void set_stream(std::ostream& stream) {		
			Iterator end = this->end();
			for(auto itr = begin(); itr!=end; ++itr) {
				stream<<*itr<<' ';
			}
			stream<<'\n';
		}	
	};
}