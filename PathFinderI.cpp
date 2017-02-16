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
#include "PathFinderI.h"

#define ROW 120
#define COL 160

using namespace std;


void findPathI(char letterMap[][COL], int xStart, int yStart, int xGoal, int yGoal, double we, int mNum, int inNum, double we2)
{
	Fringe * allFringe[4];
	for (int i = 0; i < 4; i++) {

		allFringe[i] = new Fringe;
	}
	Fringe * fringe0 = new Fringe;
	node * startNode0 = new node;
	node * goalNode0 = new node;
	ClosedList * cl0 = new ClosedList;
	ClosedList * clI = new ClosedList;
	node * startNode[4];
	node * goalNode[4];


	goalNode0->setGoal(letterMap[xGoal][yGoal], xGoal, yGoal, we);
	startNode0->setStart(letterMap[xStart][yStart], xStart, yStart, goalNode0, we, 1);
	startNode0->gVal = 0;
	startNode0->calcH(goalNode0, we, 1);
	startNode0->prev = startNode0;
	node * sI = new node;
	sI->setI(startNode0->xCord, startNode0->yCord, startNode0);
	cl0->insertI(sI);

		clI->insertI(sI);
	
	fringe0->insert(startNode0, startNode0->getG() + startNode0->getH());
	for (int i = 0; i < 4; i++) {
		startNode[i] = new node;
		goalNode[i] = new node;
		goalNode[i]->setGoal(letterMap[xGoal][yGoal], xGoal, yGoal, we);
		startNode[i]->setStart(letterMap[xStart][yStart], xStart, yStart, goalNode[i], we, i + 2);
		startNode[i]->gVal = 0;
		startNode[i]->calcH(goalNode[i], we, i + 2);
		startNode[i]->prev = startNode[i];
		allFringe[i]->insert(startNode[i], startNode[i]->getG() + startNode[i]->getH());
	}

	node * curS0 = new node;
	node * curSi = new node;
	cout << "Finding Path..." << endl;
	while (fringe0->peek() < 100000) {

		for (int i = 0; i < 4; i++) {
			if (allFringe[i]->peek() <= fringe0->peek()*we2) {
				curSi = allFringe[i]->pop();
				if (curSi->xCord == goalNode[i]->xCord && curSi->yCord == goalNode[i]->yCord) {


					goalNode[i]->prev = curSi;
					goalNode[i]->calcC();
					goalNode[i]->calcG();
					goalNode[i]->calcF();
					node * ptr = goalNode[i]->prev;
					ptr->calcH(goalNode[i], we, i + 2);


					cout << "**path found**" << endl;
					/*cout << "(" << goalNode[i]->xCord << ", " << goalNode[i]->yCord <<") f: "<< goalNode[i]->fVal<<" g: "<< goalNode[i]->gVal<<endl;
					do {
					ptr = ptr->prev;
					cout << "(" << ptr->xCord << ", " << ptr->yCord << ") f: " << ptr->fVal << " g: " << ptr->gVal << endl;

					} while (ptr != startNode[i]);*/
					cout << "** path found ! **" << endl;
					cout << "Printing to path file..." << endl;

					ofstream pathFile;
					string pfile = "pathS_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(i + 2);
					pathFile.open(pfile + ".txt");
					pathFile << goalNode[i]->xCord << " " << goalNode[i]->yCord << " " << goalNode[i]->fVal << " " << goalNode[i]->gVal << " 0" << endl;
					do {
						ptr = ptr->prev;
						pathFile << ptr->xCord << " " << ptr->yCord << " " << ptr->fVal << " " << ptr->gVal << " " << ptr->hVal << endl;

					} while (ptr != startNode[i]);
					pathFile.close();
					cout << "Path File created." << endl;

					node * ptr1 = allFringe[i]->head;
					ptr1->calcH(goalNode[i], we, i + 1);
					ofstream fringeFile;
					string ffile = "fringeS_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(i + 2);;
					fringeFile.open(ffile + ".txt");
					cout << "Printing to fringe file..." << endl;
					fringeFile << ptr1->xCord << " " << ptr1->yCord << " " << ptr1->fVal << " " << ptr1->gVal << " " << ptr1->hVal << endl;
					while (ptr1->next != NULL) {
						ptr1 = ptr1->next;
						fringeFile << ptr1->xCord << " " << ptr1->yCord << " " << ptr1->fVal << " " << ptr1->gVal << " " << ptr1->hVal << endl;

					};
					fringeFile.close();
					cout << "Fringe File created." << endl;

					node * ptr2 = clI->headI;
					ptr2->calcH(goalNode[i], we, i + 2);
					ofstream cliFile;
					string clifile = "openlistS_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(i + 2);;
					cliFile.open(clifile + ".txt");
					cout << "Printing to closed list file..." << endl;
					cliFile << ptr2->prev->xCord << " " << ptr2->prev->yCord << " " << ptr2->prev->fVal << " " << ptr2->prev->gVal << " " << ptr2->prev->hVal << endl;
					while (ptr2->nextI != NULL) {
						ptr2 = ptr2->nextI;
						ptr2->calcH(goalNode[i], we, i + 2);
						cliFile << ptr2->prev->xCord << " " << ptr2->prev->yCord << " " << ptr2->prev->fVal << " " << ptr2->prev->gVal << " " << ptr2->prev->hVal << endl;

					};
					cliFile.close();
					cout << "Closed list File created." << endl;

					cout << "Path Finder closing." << endl;

					return;
				}
				else {
					if (letterMap[curSi->xCord + 1][curSi->yCord] != '0' &&curSi->xCord + 1 <= ROW - 1) {                                        // |--
						node * rtI = new node;
						rtI->setI(curSi->xCord + 1, curSi->yCord, curSi);
						if (!clI->isOn(rtI)) {
							node * rtNode = new node;
							rtNode->prev = curSi;
							rtI->prev = curSi;
							rtNode->setNode(letterMap[curSi->xCord + 1][curSi->yCord], curSi->xCord + 1, curSi->yCord, goalNode[i], we, i + 2);
							clI->insertI(rtI);
							allFringe[i]->insert(rtNode, rtNode->getG() + rtNode->getH());
						}
						else {
							if (rtI->updateVer()) {
								allFringe[i]->remove(rtI);
								allFringe[i]->insert(rtI, rtI->getG() + rtI->getH());
							}

						}
					}
					if (letterMap[curSi->xCord + 1][curSi->yCord - 1] != '0'&&curSi->xCord + 1 <= ROW - 1 && curSi->yCord - 1 >= 0) {                                     //  "\_"
						node * rtdwI = new node;
						rtdwI->setI(curSi->xCord + 1, curSi->yCord - 1, curSi);
						if (!clI->isOn(rtdwI)) {
							node * rtdwNode = new node;
							rtdwNode->prev = curSi;
							rtdwI->prev = curSi;
							rtdwNode->setNode(letterMap[curSi->xCord + 1][curSi->yCord - 1], curSi->xCord + 1, curSi->yCord - 1, goalNode[i], we, i + 2);
							clI->insertI(rtdwI);
							allFringe[i]->insert(rtdwNode, rtdwNode->getG() + rtdwNode->getH());
						}
						else {
							if (rtdwI->updateVer()) {
								allFringe[i]->remove(rtdwI);
								allFringe[i]->insert(rtdwI, rtdwI->getG() + rtdwI->getH());
							}
						}
					}

					if (letterMap[curSi->xCord][curSi->yCord - 1] != '0' && curSi->yCord - 1 >= 0) {                                       // |_
						node * dwI = new node;
						dwI->setI(curSi->xCord, curSi->yCord - 1, curSi);
						if (!clI->isOn(dwI)) {
							node * dwNode = new node;
							dwNode->prev = curSi;
							dwI->prev = curSi;
							dwNode->setNode(letterMap[curSi->xCord][curSi->yCord - 1], curSi->xCord, curSi->yCord - 1, goalNode[i], we, i + 2);
							clI->insertI(dwI);
							allFringe[i]->insert(dwNode, dwNode->getG() + dwNode->getH());
						}
						else {
							if (dwI->updateVer()) {
								allFringe[i]->remove(dwI);
								allFringe[i]->insert(dwI, dwI->getG() + dwI->getH());
							}
						}
					}

					if (letterMap[curSi->xCord - 1][curSi->yCord - 1] != '0'&&curSi->xCord - 1 >= 0 && curSi->yCord - 1 >= 0) {                                   // _/
						node * lfdwI = new node;
						lfdwI->setI(curSi->xCord - 1, curSi->yCord - 1, curSi);
						if (!clI->isOn(lfdwI)) {
							node * lfdwNode = new node;
							lfdwNode->prev = curSi;
							lfdwI->prev = curSi;
							lfdwNode->setNode(letterMap[curSi->xCord - 1][curSi->yCord - 1], curSi->xCord - 1, curSi->yCord - 1, goalNode[i], we, i + 2);
							clI->insertI(lfdwI);
							allFringe[i]->insert(lfdwNode, lfdwNode->getG() + lfdwNode->getH());

						}
						else {
							if (lfdwI->updateVer()) {
								allFringe[i]->remove(lfdwI);
								allFringe[i]->insert(lfdwI, lfdwI->getG() + lfdwI->getH());
							}
						}
					}

					if (letterMap[curSi->xCord - 1][curSi->yCord] != '0'&&curSi->xCord - 1 >= 0) {                                      // --|.
						node * lfI = new node;
						lfI->setI(curSi->xCord - 1, curSi->yCord, curSi);
						if (!clI->isOn(lfI)) {
							node * lfNode = new node;
							lfNode->prev = curSi;
							lfI->prev = curSi;
							lfNode->setNode(letterMap[curSi->xCord - 1][curSi->yCord], curSi->xCord - 1, curSi->yCord, goalNode[i], we, i + 2);
							clI->insertI(lfI);
							allFringe[i]->insert(lfNode, lfNode->getG() + lfNode->getH());

						}
						else {
							if (lfI->updateVer()) {
								allFringe[i]->remove(lfI);
								allFringe[i]->insert(lfI, lfI->getG() + lfI->getH());
							}
						}
					}

					if (letterMap[curSi->xCord - 1][curSi->yCord + 1] != '0'&& curSi->xCord - 1 >= 0 && curSi->yCord + 1 <= COL - 1) {                             // --"\"
						node * lftpI = new node;
						lftpI->setI(curSi->xCord - 1, curSi->yCord + 1, curSi);
						if (!clI->isOn(lftpI)) {
							node * lftpNode = new node;
							lftpNode->prev = curSi;
							lftpI->prev = curSi;
							lftpNode->setNode(letterMap[curSi->xCord - 1][curSi->yCord + 1], curSi->xCord - 1, curSi->yCord + 1, goalNode[i], we, i + 2);
							clI->insertI(lftpI);
							allFringe[i]->insert(lftpNode, lftpNode->getG() + lftpNode->getH());

						}
						else {
							if (lftpI->updateVer()) {
								allFringe[i]->remove(lftpI);
								allFringe[i]->insert(lftpI, lftpI->getG() + lftpI->getH());
							}
						}
					}

					if (letterMap[curSi->xCord][curSi->yCord + 1] != '0'&& curSi->yCord + 1 <= COL - 1) {                            //  ^|^
						node * tpI = new node;
						tpI->setI(curSi->xCord, curSi->yCord + 1, curSi);
						if (!clI->isOn(tpI)) {
							node * tpNode = new node;
							tpNode->prev = curSi;
							tpI->prev = curSi;
							tpNode->setNode(letterMap[curSi->xCord][curSi->yCord + 1], curSi->xCord, curSi->yCord + 1, goalNode[i], we, i + 2);
							clI->insertI(tpI);
							allFringe[i]->insert(tpNode, tpNode->getG() + tpNode->getH());

						}
						else {
							if (tpI->updateVer()) {
								allFringe[i]->remove(tpI);
								allFringe[i]->insert(tpI, tpI->getG() + tpI->getH());
							}
						}
					}

					if (letterMap[curSi->xCord + 1][curSi->yCord + 1] != '0'&&curSi->xCord + 1 <= ROW - 1 && curSi->yCord + 1 <= COL - 1) {                                  // /--
						node * rttpI = new node;
						rttpI->setI(curSi->xCord + 1, curSi->yCord + 1, curSi);
						if (!clI->isOn(rttpI)) {
							node * rttpNode = new node;
							rttpNode->prev = curSi;
							rttpI->prev = curSi;
							rttpNode->setNode(letterMap[curSi->xCord + 1][curSi->yCord + 1], curSi->xCord + 1, curSi->yCord + 1, goalNode[i], we, i + 2);
							clI->insertI(rttpI);
							allFringe[i]->insert(rttpNode, rttpNode->getG() + rttpNode->getH());
						}
						else {
							if (rttpI->updateVer()) {
								allFringe[i]->remove(rttpI);
								allFringe[i]->insert(rttpI, rttpI->getG() + rttpI->getH());
							}
						}
					}


				}


			}
			else {
				curS0 = fringe0->pop();
				if (curS0->xCord == goalNode0->xCord && curS0->yCord == goalNode0->yCord) {


					goalNode0->prev = curS0;
					goalNode0->calcC();
					goalNode0->calcG();
					goalNode0->calcF();
					node * ptr = goalNode0->prev;
					ptr->calcH(goalNode0, we, 1);


					cout << "**path found**" << endl;
					/*cout << "(" << goalNode0->xCord << ", " << goalNode0->yCord <<") f: "<< goalNode0->fVal<<" g: "<< goalNode0->gVal<<endl;
					do {
					ptr = ptr->prev;
					cout << "(" << ptr->xCord << ", " << ptr->yCord << ") f: " << ptr->fVal << " g: " << ptr->gVal << endl;

					} while (ptr != startNode[i]);*/
					cout << "** path found ! **" << endl;
					cout << "Printing to path file..." << endl;

					ofstream pathFile;
					string pfile = "path_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(1);
					pathFile.open(pfile + ".txt");
					pathFile << goalNode0->xCord << " " << goalNode0->yCord << " " << goalNode0->fVal << " " << goalNode0->gVal << " 0" << endl;
					do {
						ptr = ptr->prev;
						pathFile << ptr->xCord << " " << ptr->yCord << " " << ptr->fVal << " " << ptr->gVal << " " << ptr->hVal << endl;

					} while (ptr != startNode[i]);
					pathFile.close();
					cout << "Path File created." << endl;

					node * ptr1 = fringe0->head;
					ptr1->calcH(goalNode0, we, 1);
					ofstream fringeFile;
					string ffile = "fringe_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(1);;
					fringeFile.open(ffile + ".txt");
					cout << "Printing to fringe file..." << endl;
					fringeFile << ptr1->xCord << " " << ptr1->yCord << " " << ptr1->fVal << " " << ptr1->gVal << " " << ptr1->hVal << endl;
					while (ptr1->next != NULL) {
						ptr1 = ptr1->next;
						fringeFile << ptr1->xCord << " " << ptr1->yCord << " " << ptr1->fVal << " " << ptr1->gVal << " " << ptr1->hVal << endl;

					};
					fringeFile.close();
					cout << "Fringe File created." << endl;

					node * ptr2 = cl0->headI;
					ptr2->calcH(goalNode0, we, 1);
					ofstream cl0File;
					string cl0file = "openlist_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(1);;
					cl0File.open(cl0file + ".txt");
					cout << "Printing to closed list file..." << endl;
					cl0File << ptr2->prev->xCord << " " << ptr2->prev->yCord << " " << ptr2->prev->fVal << " " << ptr2->prev->gVal << " " << ptr2->prev->hVal << endl;
					while (ptr2->nextI != NULL) {
						ptr2 = ptr2->nextI;
						ptr2->calcH(goalNode0, we, 1);
						cl0File << ptr2->prev->xCord << " " << ptr2->prev->yCord << " " << ptr2->prev->fVal << " " << ptr2->prev->gVal << " " << ptr2->prev->hVal << endl;

					};
					cl0File.close();
					cout << "Closed list File created." << endl;

					cout << "Path Finder closing." << endl;

					return;
				}
				else {
					if (letterMap[curS0->xCord + 1][curS0->yCord] != '0' &&curS0->xCord + 1 <= ROW - 1) {                                        // |--
						node * rtI = new node;
						rtI->setI(curS0->xCord + 1, curS0->yCord, curS0);
						if (!cl0->isOn(rtI)) {
							node * rtNode = new node;
							rtNode->prev = curS0;
							rtI->prev = curS0;
							rtNode->setNode(letterMap[curS0->xCord + 1][curS0->yCord], curS0->xCord + 1, curS0->yCord, goalNode0, we, 1);
							cl0->insertI(rtI);
							fringe0->insert(rtNode, rtNode->getG() + rtNode->getH());
						}
						else {
							if (rtI->updateVer()) {
								fringe0->remove(rtI);
								fringe0->insert(rtI, rtI->getG() + rtI->getH());
							}

						}
					}
					if (letterMap[curS0->xCord + 1][curS0->yCord - 1] != '0'&&curS0->xCord + 1 <= ROW - 1 && curS0->yCord - 1 >= 0) {                                     //  "\_"
						node * rtdwI = new node;
						rtdwI->setI(curS0->xCord + 1, curS0->yCord - 1, curS0);
						if (!cl0->isOn(rtdwI)) {
							node * rtdwNode = new node;
							rtdwNode->prev = curS0;
							rtdwI->prev = curS0;
							rtdwNode->setNode(letterMap[curS0->xCord + 1][curS0->yCord - 1], curS0->xCord + 1, curS0->yCord - 1, goalNode0, we, 1);
							cl0->insertI(rtdwI);
							fringe0->insert(rtdwNode, rtdwNode->getG() + rtdwNode->getH());
						}
						else {
							if (rtdwI->updateVer()) {
								fringe0->remove(rtdwI);
								fringe0->insert(rtdwI, rtdwI->getG() + rtdwI->getH());
							}
						}
					}

					if (letterMap[curS0->xCord][curS0->yCord - 1] != '0' && curS0->yCord - 1 >= 0) {                                       // |_
						node * dwI = new node;
						dwI->setI(curS0->xCord, curS0->yCord - 1, curS0);
						if (!cl0->isOn(dwI)) {
							node * dwNode = new node;
							dwNode->prev = curS0;
							dwI->prev = curS0;
							dwNode->setNode(letterMap[curS0->xCord][curS0->yCord - 1], curS0->xCord, curS0->yCord - 1, goalNode0, we, 1);
							cl0->insertI(dwI);
							fringe0->insert(dwNode, dwNode->getG() + dwNode->getH());
						}
						else {
							if (dwI->updateVer()) {
								fringe0->remove(dwI);
								fringe0->insert(dwI, dwI->getG() + dwI->getH());
							}
						}
					}

					if (letterMap[curS0->xCord - 1][curS0->yCord - 1] != '0'&&curS0->xCord - 1 >= 0 && curS0->yCord - 1 >= 0) {                                   // _/
						node * lfdwI = new node;
						lfdwI->setI(curS0->xCord - 1, curS0->yCord - 1, curS0);
						if (!cl0->isOn(lfdwI)) {
							node * lfdwNode = new node;
							lfdwNode->prev = curS0;
							lfdwI->prev = curS0;
							lfdwNode->setNode(letterMap[curS0->xCord - 1][curS0->yCord - 1], curS0->xCord - 1, curS0->yCord - 1, goalNode0, we, 1);
							cl0->insertI(lfdwI);
							fringe0->insert(lfdwNode, lfdwNode->getG() + lfdwNode->getH());

						}
						else {
							if (lfdwI->updateVer()) {
								fringe0->remove(lfdwI);
								fringe0->insert(lfdwI, lfdwI->getG() + lfdwI->getH());
							}
						}
					}

					if (letterMap[curS0->xCord - 1][curS0->yCord] != '0'&&curS0->xCord - 1 >= 0) {                                      // --|.
						node * lfI = new node;
						lfI->setI(curS0->xCord - 1, curS0->yCord, curS0);
						if (!cl0->isOn(lfI)) {
							node * lfNode = new node;
							lfNode->prev = curS0;
							lfI->prev = curS0;
							lfNode->setNode(letterMap[curS0->xCord - 1][curS0->yCord], curS0->xCord - 1, curS0->yCord, goalNode0, we, 1);
							cl0->insertI(lfI);
							fringe0->insert(lfNode, lfNode->getG() + lfNode->getH());

						}
						else {
							if (lfI->updateVer()) {
								fringe0->remove(lfI);
								fringe0->insert(lfI, lfI->getG() + lfI->getH());
							}
						}
					}

					if (letterMap[curS0->xCord - 1][curS0->yCord + 1] != '0'&& curS0->xCord - 1 >= 0 && curS0->yCord + 1 <= COL - 1) {                             // --"\"
						node * lftpI = new node;
						lftpI->setI(curS0->xCord - 1, curS0->yCord + 1, curS0);
						if (!cl0->isOn(lftpI)) {
							node * lftpNode = new node;
							lftpNode->prev = curS0;
							lftpI->prev = curS0;
							lftpNode->setNode(letterMap[curS0->xCord - 1][curS0->yCord + 1], curS0->xCord - 1, curS0->yCord + 1, goalNode0, we, 1);
							cl0->insertI(lftpI);
							fringe0->insert(lftpNode, lftpNode->getG() + lftpNode->getH());

						}
						else {
							if (lftpI->updateVer()) {
								fringe0->remove(lftpI);
								fringe0->insert(lftpI, lftpI->getG() + lftpI->getH());
							}
						}
					}

					if (letterMap[curS0->xCord][curS0->yCord + 1] != '0'&& curS0->yCord + 1 <= COL - 1) {                            //  ^|^
						node * tpI = new node;
						tpI->setI(curS0->xCord, curS0->yCord + 1, curS0);
						if (!cl0->isOn(tpI)) {
							node * tpNode = new node;
							tpNode->prev = curS0;
							tpI->prev = curS0;
							tpNode->setNode(letterMap[curS0->xCord][curS0->yCord + 1], curS0->xCord, curS0->yCord + 1, goalNode0, we, 1);
							cl0->insertI(tpI);
							fringe0->insert(tpNode, tpNode->getG() + tpNode->getH());

						}
						else {
							if (tpI->updateVer()) {
								fringe0->remove(tpI);
								fringe0->insert(tpI, tpI->getG() + tpI->getH());
							}
						}
					}

					if (letterMap[curS0->xCord + 1][curS0->yCord + 1] != '0'&&curS0->xCord + 1 <= ROW - 1 && curS0->yCord + 1 <= COL - 1) {                                  // /--
						node * rttpI = new node;
						rttpI->setI(curS0->xCord + 1, curS0->yCord + 1, curS0);
						if (!cl0->isOn(rttpI)) {
							node * rttpNode = new node;
							rttpNode->prev = curS0;
							rttpI->prev = curS0;
							rttpNode->setNode(letterMap[curS0->xCord + 1][curS0->yCord + 1], curS0->xCord + 1, curS0->yCord + 1, goalNode0, we, 1);
							cl0->insertI(rttpI);
							fringe0->insert(rttpNode, rttpNode->getG() + rttpNode->getH());
						}
						else {
							if (rttpI->updateVer()) {
								fringe0->remove(rttpI);
								fringe0->insert(rttpI, rttpI->getG() + rttpI->getH());
							}
						}
					}
					//				cout << "expanded" << endl;
					/*node * ptr = fringe->head;
					cout << "**current fringe**" << endl;
					do {
					cout << " node-> " << ptr->xCord << ", " << ptr->yCord << " f value: "<< ptr->fVal << endl;
					ptr = ptr->next;
					} while (ptr->next != NULL);*/
				}
			}
		}
	}
	cout << "No path found" << endl;
	return;
}


