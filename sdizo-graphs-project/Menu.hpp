#pragma once

#include <iostream>

#include "MatrixGraph.hpp"
#include "ListGraph.hpp"

class Menu {
private:

	MatrixGraph matrixGraph;
	ListGraph listGraph;

public:

	void printMenu() {
		std::cout<<"\n---------------------------\nDostepne opcje:\n";
		std::cout<<"1. Wczytaj dane z pliku.\n";
		std::cout<<"2. Wygeneruj grafy losowo.\n";
		std::cout<<"3. Wyswietl grafy na ekranie.\n";
		std::cout<<"4. Algorytm Prima z wyswietleniem wynikow.\n";
		std::cout<<"5. Algorytm Kruskala z wyswietleniem wynikow. \n";
		std::cout<<"6. Algorytm Dijkstry z wyswietleniem wynikow. \n";
		std::cout<<"7. Algorytm Forda-Bellmana z wyswietleniem wynikow. \n";
		std::cout<<"8. Wyswietl grafy na ekranie (bez macierzy polaczen).\n";
		std::cout<<"9. Zamknij program. \n\n";
	}

	void run() {
		char option;
		while(true) {
			printMenu();
			std::cout<<"Wybierz opcje: ";
			std::cin>>option;
			if(option == '1') {
				std::cout<<"Opcja: Wczytaj dane z pliku.\n\n";
				loadFromFile();
			}
			else if(option == '2') {
				std::cout<<"Opcja: Wygeneruj grafy losowo.\n\n";
				generate();
			}
			else if(option == '3') {
				std::cout<<"Opcja: Wyswietl grafy na ekranie.\n\n";
				show();
			}
			else if(option == '4') {
				std::cout<<"Opcja: Algorytm Prima z wyswietleniem wynikow.\n\n";
				prim();
			}
			else if(option == '5') {
				std::cout<<"Opcja: Algorytm Kruskala z wyswietleniem wynikow. \n\n";
				kruskal();
			}
			else if(option == '6') {
				std::cout<<"Opcja: Algorytm Dijkstry z wyswietleniem wynikow. \n\n";
				dijkstra();
			}
			else if(option == '7') {
				std::cout<<"Opcja: Algorytm Forda-Bellmana z wyswietleniem wynikow. \n\n";
				fordBellman();
			}
			else if(option == '8') {
				std::cout<<"Opcja: Wyswietl grafy na ekranie (bez macierzy polaczen). \n\n";
				showNoMatrix();
			}
			else if(option == '9') {
				std::cout<<"Opcja: Zamknij program. \n\n";
				break;
			}
		}
	}

	void loadFromFile() {
		std::cout<<"> Podaj nazwe pliku: ";
		std::string path;
		std::cin>>path;
		std::cout<<"[Wczytywanie grafow...]\n";
		matrixGraph.loadFromFile(path);
		listGraph.loadFromFile(path);
		std::cout<<"[Grafy wczytane.]\n";
	}

	void generate() {
		static unsigned times = 0;
		++times;
		int vs;
		float density; // in percents
		unsigned seed = std::time(nullptr)+times;
		std::cout<<"> Podaj ilosc wierzcholkow i gestosc (w %): ";
		std::cin>>vs>>density;
		std::cout<<"[Generowanie grafow...]\n";
		matrixGraph.generateDoubleGraph(vs, 100, density/100.f, seed);
		listGraph.generateDoubleGraph(vs, 100, density/100.f, seed);
		std::cout<<"[Grafy wygenerowane.]\n";
	}

	void show() {
		std::cout<<"-------- Graf listowy --------\n";
		std::cout<<"Lista polaczen:\n";
		listGraph.printEdges();
		std::cout<<'\n';
		std::cout<<"-------- Graf macierzowy --------\n";
		std::cout<<"Lista polaczen:\n";
		matrixGraph.printEdges();
		std::cout<<'\n';
		std::cout<<"Macierz polaczen:\n";
		matrixGraph.printMatrix();
		std::cout<<'\n';
	}

	void showNoMatrix() {
		std::cout<<"-------- Graf listowy --------\n";
		std::cout<<"Lista polaczen:\n";
		listGraph.printEdges();
		std::cout<<'\n';
		std::cout<<"-------- Graf macierzowy --------\n";
		std::cout<<"Lista polaczen:\n";
		matrixGraph.printEdges();
		std::cout<<'\n';
	}

	void prim() {
		std::cout<<"[Wykonywanie algorytmu Prima...]\n";
		matrixGraph.prim();
		listGraph.prim();
		std::cout<<"[Wykonywanie zakonczone.]\n";
		show();
	}

	void kruskal() {
		std::cout<<"[Wykonywanie algorytmu Kruskala...]\n";
		matrixGraph.kruskal();
		listGraph.kruskal();
		std::cout<<"[Wykonywanie zakonczone.]\n";
		show();
	}

	void dijkstra() {
		std::cout<<"> Podaj wierzcholek poczatkowy i koncowy: ";
		int src, dst;
		std::cin>>src>>dst;

		std::cout<<"[Wykonywanie algorytmu Dijkstry...]\n";
		Vector<int> p1(std::move(listGraph.dijkstra(src, dst))); 
		Vector<int> p2(std::move(matrixGraph.dijkstra(src, dst))); 
		std::cout<<"[Wykonywanie zakonczone.]\n\n";

		std::cout<<"-------- Graf listowy --------\n";
		std::cout<<"Najkrotsza sciezka: ";
		for(auto& obj : p1) {
			std::cout<<obj<<' ';
		}
		std::cout<<"\n\n";

		std::cout<<"-------- Graf macierzowy --------\n";
		std::cout<<"Najkrotsza sciezka: ";
		for(auto& obj : p2) {
			std::cout<<obj<<' ';
		}
		std::cout<<'\n';
	}

	void fordBellman() {
		std::cout<<"> Podaj wierzcholek poczatkowy i koncowy: ";
		int src, dst;
		std::cin>>src>>dst;

		std::cout<<"[Wykonywanie algorytmu Forda-Bellmana...]\n";
		Vector<int> p1(std::move(listGraph.bellmanFord(src, dst))); 
		Vector<int> p2(std::move(matrixGraph.bellmanFord(src, dst))); 
		std::cout<<"[Wykonywanie zakonczone.]\n\n";

		std::cout<<"-------- Graf listowy --------\n";
		std::cout<<"Najkrotsza sciezka: ";
		for(auto& obj : p1) {
			std::cout<<obj<<' ';
		}
		std::cout<<"\n\n";

		std::cout<<"-------- Graf macierzowy --------\n";
		std::cout<<"Najkrotsza sciezka: ";
		for(auto& obj : p2) {
			std::cout<<obj<<' ';
		}
		std::cout<<'\n';
	}

};