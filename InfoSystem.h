#ifndef INFO_SYSTEM_H
#define INFO_SYSTEM_H
#include "Station.h"
#include "Line.h"
#include "Exceptions.h"
#include<deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <vector>


class AdjNode;
class InfoSystem{
	/* Klasa predstavlja sistem stajalista i linija. 
	Klasa sadrzi lisu svih linija i svih stajalista, i graf kojim je predstavljen ceo sistem.
	* */
public:
	InfoSystem(const string& stationFileName, const string& lineFileName);
	InfoSystem();
	~InfoSystem();

	void printStationInfo(int id); // Ispis informacije o stanici.
	void printLineInfo(const string& id); // Ispis informacije o liniji
	void lineStats(const string& id); // Ispis statistike za liniju.

	void printSystem(); // Ispis svih stajalista i linija.
	void printAdjList(); // Ispis grafa.

	void findShortestRoute(int startId, int endId); // Strategija za pronalazenje najrakce putanje izmedju 2 stanice.
	void findLeastLineRoute(int startId, int endId); // Strategija za pronalazenje putanje sa najmanjim brojem presedanja izmedju 2 stranice.

private:
	list<Station*> stations_;
	list<Line*> lines_;
	list<AdjNode*> adjList_;

	Station* getStation(int id);
	AdjNode* getAdjNode(int id);
	Line* getLine(const string& id);

	void createStationList(const string& inputFile);
	void createStationList();
	void createLineList(const string& inputFile);
	void createLineList();
	void createAdjList(); // Metoda za kreiranje liste susednosti.
	void addAdjListNodes(AdjNode* currNode, Line* currLine, char r);

	// Pomocne metode za realizaciju strategije za pronalazenje najrakce putanje izmedju 2 stajalista.
	int getMapElem(map<int, AdjNode*>map, AdjNode* elem);
	map<int, AdjNode*> createMap(AdjNode* startNode, vector<int>& pred, vector<bool>& visited, vector<int>& distance, vector<vector<int>>& adj);
	void initialiseAdjVect(map<int, AdjNode*> map, vector<vector<int>>& adj);
	void findRoute(int end, int start, vector<int>& pred, vector<bool>& visited, vector<int>& distance, vector<vector<int>>& adj);
	vector<int> initialisePathVect(vector<int>& pred, int end);
	void printShortestRoute(map<int, AdjNode*> map, vector<int> path, int start, int end, int startStation, int endStation);

	// Pomocne metode za realizaciju strategije za pronalazenje putanje izmedju 2 stajalista sa najmanjim brojem presedanja.
	bool createLineRoute(Station* currentStation, Station* endStation, list<Station*>& visitedStations, deque<Line*>& lineDeque, deque<Station*>& stationDeque, queue<Station*>& stationsToVisit);
	bool compareCurEndLines(list<Line*> linesToVisit, Station* endStation, deque<Line*>& lineDeque);
	bool compareLineStations(list<Station*> traverseList, Line* currentLine, Line* currEndLine, deque<Line*>& lineDeque, deque<Station*>& stationDeque);
	list<Station*> getTraverseList(Line* currentLine, Station* currentStation);
	bool compareCurStation(Line* currLine, Line* currEndLine, Station* currStation, list<Station*> route, deque<Line*>& lineDeque, deque<Station*>& stationDeque);
	void printLeastLinesRoute(deque<Line*>& lineDeque, deque<Station*>& stationDeque);
	list<Line*> getLineList(Station* currentStation, Line* currentLine);
	bool findRoute(Station* currentStation, Station* endStation, deque<Line*>& lineDeque, deque<Station*>& stationDeque);

};

class AdjListNode;
class AdjNode {
	/* Klasa koja predstavlja listu  susednosti.
Jedan element sadrzi stajaliste  i listu susednih stanica na koje je moguce doci sa ovog elementa.
Element ove liste sadrzi stajaliste i liniju sa kojom se dolayi do te stanice.*/
public:
	AdjNode(Station* station = nullptr);
	~AdjNode();
	list<AdjListNode*>& getList();
	Station* getStation() const;
	void addNode(AdjListNode* newNode);

private:
	list<AdjListNode*> adjNodeList_; // lista susednih stanica sa linijama.
	Station* station_;
};
class AdjListNode {
	/* Klasa predstavlja element liste susednih stajaliste elementa liste susednosti.
	Klasa sadrzi stajaliste i liniju pomocu koje se dolazi do stajalista. */
public:
	AdjListNode(AdjNode* dest = nullptr, Line* line = nullptr);
	AdjNode* getDest();
	Line* getNodeLine();

private:
	AdjNode* dest_;
	Line* line_;

};

#endif


