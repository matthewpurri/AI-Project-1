#include <iostream>
#include <string> // Naming files
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function
#include <time.h> // For time in srand function
#include <vector> 
#include <utility> // pair

#include "Coordinate.h"
#include "MapBuild.h"
#include "fringe.h"
#include "closedlist.h"
#include "PathFinder.h"
#include "PathFinderS.h"
#include "PathFinderI.h"

#define ROW 120
#define COL 160

using namespace std;


int main() {
	//makeMap();

	Coordinate map[ROW][COL];
	char letterMap[ROW][COL];
	int start_goal[2][2];
	int index, mapNumber, heur;
	double weight;
	double w2;
	char again;
	int h;
start:
	cout << "Select a map (1-5): " << endl;
	cin >> mapNumber;
	cout << "\n Select a start-goal pair (0-9)" << endl;
	cin >> index;
	cout << "Single Heauristic A* (1), Sequential Heuristic A*(2), Integrated Heuristic A*(3)?" << endl;
	cin >> h;
	if (h == 1) {
		cout << "Heuristic ? Manhattan(1), Euclidean(2), No Blocked Cells(3),"
			"Horizontal cells are 50% easier to traverse than vertical cells(4), Get close to the end point (5):" << endl;
		cin >> heur;
		cout << "Heuristic weight? Uniform(0), A*(0.25), A*Weighted( number greater than 0.25) :" << endl;
		cin >> weight;

		openMap(map, letterMap, start_goal, index, mapNumber);
		findPath(letterMap, start_goal[0][0], start_goal[0][1], start_goal[1][0], start_goal[1][1], weight, mapNumber, index, heur);
		cout << "Another map and path (y,n)" << endl;
		cin >> again;
		while (1) {
			if (again == 'y')
				goto start;
			else if (again == 'n') {
				cout << "Program Closing" << endl;
				return 0;
			}
			else {
				cout << "Incorrect input. Another map and path (y,n)" << endl;
				cin >> again;
			}
		}
	}
	else if (h == 2) {
		cout << "Heuristic weight ? (0.25<=):" << endl;
		cin >> weight;
		cout << "Prioritization of inadmissible heuristic weight? (1<=) :" << endl;
		cin >> w2;


		openMap(map, letterMap, start_goal, index, mapNumber);
		findPathS(letterMap, start_goal[0][0], start_goal[0][1], start_goal[1][0], start_goal[1][1], weight, mapNumber, index, w2);


		cout << "Another map and path (y,n)" << endl;
		cin >> again;
		while (1) {
			if (again == 'y')
				goto start;
			else if (again == 'n') {
				cout << "Program Closing" << endl;
				return 0;
			}
			else {
				cout << "Incorrect input. Another map and path (y,n)" << endl;
				cin >> again;
			}
		}
	}else {
		cout << "Heuristic weight ? (0.25<=):" << endl;
		cin >> weight;
		cout << "Prioritization of inadmissible heuristic weight? (1<=) :" << endl;
		cin >> w2;

		openMap(map, letterMap, start_goal, index, mapNumber);
		findPathI(letterMap, start_goal[0][0], start_goal[0][1], start_goal[1][0], start_goal[1][1], weight, mapNumber, index, 1);

		cout << "Another map and path (y,n)" << endl;
		cin >> again;
		while (1) {
			if (again == 'y')
				goto start;
			else if (again == 'n') {
				cout << "Program Closing" << endl;
				return 0;
			}
			else {
				cout << "Incorrect input. Another map and path (y,n)" << endl;
				cin >> again;
			}
		}
	}
	/*
	double we[3] = { 0.25, 1.25, 2 };


			for (int h = 1; h < 6; h++) {
				for (int q = 2; q < 3; q++) {

					ofstream  allfile;
					allfile.open("allAverage_" + to_string(h)+"_"+to_string(we[q])+".txt");
					
		for (int m = 1; m < 6; m++) {
		for (int i = 0; i < 10; i++) {
					openMap(map, letterMap, start_goal, i, m);
					findPath(letterMap, start_goal[0][0], start_goal[0][1], start_goal[1][0], start_goal[1][1], 0, m, i, h, allfile);
		
				}
			}
			allfile.close();
		}
	}*/
}