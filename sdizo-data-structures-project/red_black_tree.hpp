#pragma once

#include <iomanip>
#include <iostream>

// Website: http://math.uni.lodz.pl/~horzel/ZA_2008/3_drzewa_RB.pdf

namespace sdizo {
struct RedBlackTree {

	struct Node {
		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;
		int value;
		bool red = true;
		Node(int value) : value(value) {}
		void setRed() {red = true;}
		void setBlack() {red = false;}
		void recolor() {red = !red;}
		void recolor(bool color) {red = color;}
		void recolor(Node* node) {red = node->red;}
		bool isRed() {return red==true;}
		bool isBlack() {return red==false;}
		Node* brother() {
			if(parent) {
				if(parent->left == this) return parent->right;
				else return parent->left;
			}
			return nullptr;
		}
		bool isLeftChild() {return parent->left == this;}
		bool isRightChild() {return parent->right == this;}
		Node*& junction() {
			if(parent->left == this) return parent->left;
			return parent->right;
		}
		Node* firstChild() {
			if(left) return left;
			else if(right) return right;
			else return nullptr;
		}
	};

	Node* root = nullptr;
	Node* guardian = nullptr;

	// Latest added node
	Node* latest = nullptr;

	int size = 0;

	~RedBlackTree() {
		clear();
		delete guardian;
	}

	RedBlackTree() {
		guardian = new Node(0);
		guardian->setBlack();
	}

	// *etters

	int get_size() {
		return size;
	}

	// Modifiers

	void clear() {
		if(root) {
			rclear(root);
			root = nullptr;
			latest = nullptr;
			size = 0;
		}
	}

	void rclear(Node* node) {
		if(node->left) rclear(node->left);
		if(node->right) rclear(node->right);
		delete node;
	}

	void left_rotation(Node* node) {
		Node* x = node;
		Node* y = x->right;
		x->right = y->left;
		if(y->left) y->left->parent = x;
		y->parent = x->parent;
		if(x->parent == nullptr) {
			root = y;

		}
		else { 
			if(x == x->parent->left) {
				x->parent->left = y;
	
			}
			else {
				x->parent->right = y;
	
			}
		}
		y->left = x;
		x->parent = y;
	}
	void right_rotation(Node* node) {
		Node* x = node;
		Node* y = x->left;
		x->left = y->right;
		if(y->right) y->right->parent = x;
		y->parent = x->parent;
		if(x->parent == nullptr) {
			root = y;
		}
		else { 
			if(x == x->parent->right) {
				x->parent->right = y;
			}
			else {
				x->parent->left = y;
			}
		}
		y->right = x;
		x->parent = y;
	}

	void insert_fixup(Node* node) {

		root->setBlack();
		if(node->parent && node->parent->isRed()) {
			Node* uncle = node->parent->brother();
			if(uncle && uncle->isRed()) {
				// Case 1 - Red uncle
				// std::cout<<"[Case 1]\n";
				node->parent->setBlack();
				uncle->setBlack();
				node->parent->parent->setRed();
				insert_fixup(node->parent->parent);
			}
			else {
				// Case 2 - Black uncle
				if(node->parent->isLeftChild()) {
					if(node->isRightChild()) {
						// Case 2a - Parent is left + Node is right
						// std::cout<<"[Case 2a]\n";
						left_rotation(node->parent);
						node->setBlack();
						node->parent->setRed();
						right_rotation(node->parent);
					}
					else {		
						// Case 2b - Parent is left + Node is left 				
						// std::cout<<"[Case 2b]\n";
						node->parent->setBlack();
						node->parent->parent->setRed();
						right_rotation(node->parent->parent);	
					}
				}
				else {
					if(node->isLeftChild()) {
						// Case 2c - Parent is right + Node is left
						// std::cout<<"[Case 2c]\n";
						right_rotation(node->parent);
						node->setBlack();
						node->parent->setRed();
						left_rotation(node->parent);
					}
					else {		
						// Case 2d - Parent is right + Node is right 				
						// std::cout<<"[Case 2d]\n";
						node->parent->setBlack();
						node->parent->parent->setRed();
						left_rotation(node->parent->parent);	
					}
				}
			}
		}
	}

	Node* successor(Node* node) {
		if(node->left) {
			node = node->left;
			while(node->right) {
				node = node->right;
			}
			return node;
		}
		else if(node->right) {
			node = node->right;
			while(node->left) {
				node = node->left;
			}
			return node;
		}
		return nullptr;
	}

	void remove(int value) {
		remove(find(value));
	}

	void remove(Node* node);

	// void remove(Node* node) {
	// 	if(node == nullptr) return;
	// 	bool fixupFlag = false;
	// 	if(node->isBlack())
	// 	Node* succ = successor(node);
	// 	if(succ) {
	// 		// std::cout<<"succ\n";
	// 		node->value = succ->value;
	// 		Node* succChild = succ->firstChild();
	// 		if(succChild) succChild->parent = succ->parent;
	// 		succ->junction() = succChild;
	// 		delete succ;
	// 	} else {
	// 		if(node == root) {
	// 			root = nullptr;
	// 			delete node;
	// 		}
	// 		else {
	// 			node->junction() = nullptr;
	// 			delete node;
	// 		}
	// 	}
	// }

	void insert(int value) {
		if(root) {
			rinsert(root, value);
		}
		else {
			root = new Node(value);
			latest = root;
		}
		insert_fixup(latest);
		++size;
	}

	void rinsert(Node* node, int value) {
		if(value<node->value) {
			if(node->left) {
				rinsert(node->left, value);
			}
			else {
				Node* newNode = new Node(value);
				latest = newNode;
				newNode->parent = node;
				node->left = newNode;
			}
		}
		else {
			if(node->right) {
				rinsert(node->right, value);
			}
			else {				
				Node* newNode = new Node(value);
				latest = newNode;
				newNode->parent = node;
				node->right = newNode;
			}
		}
	}

	Node* find(int value) {
		return rfind(root, value);
	}

	Node* rfind(Node* node, int value) {
		if(node->value==value) return node;
		if(value<node->value && node->left) return rfind(node->left, value);
		if(value>node->value && node->right) return rfind(node->right, value);
		return nullptr;
	}

	void set_stream(std::ostream& stream) {
		if(root) {
			rset_stream(root, 0, stream);
		}
	}

	void rset_stream(Node* node, int lvl, std::ostream& stream) {
		if(node) rset_stream(node->right, lvl+1, stream);
		for(int i=0; i<lvl; ++i) {
			std::cout<<std::setw(3)<<std::left<<"     ";
		}
		if(node) { 
			std::cout<<node->value<<'|'<<(node->red?'R':'B')<<'\n';
			rset_stream(node->left, lvl+1, stream);
		}
		else std::cout<<"---\n";
	}
};

	void RedBlackTree::remove(Node* node)
	{
		Node *W, *Y, *Z;

		if(node->left == nullptr || node->right == nullptr) Y = node;
		else Y = successor(node);

		if(Y->left) Z = Y->left;
		else if(Y->right) Z = Y->right;
		else Z = guardian;

		Z->parent = Y->parent;

		if(Y->parent == nullptr) root = Z;
		else if(Y == Y->parent->left) Y->parent->left = Z;
		else Y->parent->right = Z;

		if(Y != node) node->value = Y->value;

		if(Y->isBlack()) while(Z != root && Z->isBlack()) {
			// For left side
			if(Z->isLeftChild()) {
				W = Z->parent->right;
	        
				// Case 1
				if(W->isRed()) {      
					W->setBlack();
					Z->parent->setRed();
					left_rotation(Z->parent);
					W = Z->parent->right;
				}

				// Case 2
				if((W->left == nullptr || W->left->isBlack()) && (W->right == nullptr || W->right->isBlack()))
				{              
					W->setRed();
					Z = Z->parent;
					continue;
				}

				// Case 3
				if(W->right == nullptr || W->right->isBlack())
				{              
					W->left->setBlack();
					W->setRed();
					right_rotation(W);
					W = Z->parent->right;
				}

				// Case 4
				W->recolor(Z->parent);
				Z->parent->setBlack();
				W->right->setBlack();
				left_rotation(Z->parent);
				Z = root;
			}
			// For right side
			else
			{
				W = Z->parent->left;

				// Case 1
				if(W->isRed())
				{              
					W->setBlack();
					Z->parent->setRed();
					right_rotation(Z->parent);
					W = Z->parent->left;
				}

				// Case 2
				if((W->left == nullptr || W->left->isBlack()) && (W->right == nullptr || W->right->isBlack()))
				{              
					W->setRed();
					Z = Z->parent;
					continue;
				}

				// Case 3
				if(W->left == nullptr || W->left->isBlack())
				{             
					W->right->setBlack();
					W->setRed();
					left_rotation(W);
					W = Z->parent->left;
				}

				// Case 4
				W->recolor(Z->parent);
				Z->parent->setBlack();
				W->left->setBlack();
				right_rotation(Z->parent);
				Z = root;
			}
		}

		if(guardian->parent) {
			guardian->junction() = nullptr;
			guardian->parent = nullptr;
		}
		else if(guardian == root) {
			root = nullptr;
		}

		Z->setBlack();
		delete Y;
		--size;
	}
}