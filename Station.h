#ifndef STATION_H
#define STATION_H

#include "Line.h"
#include<fstream>
#include<iostream>
#include<list>
#include<string>

using namespace std;

class Line;
class AdjNode;

class Station{
	// Klasa stajaliste sadrzi ime stanice, identifikacioni broj, i listu linija koje staju na stajalistu.
public:
	Station(const string& name = "", int id = 0);

	int getId() const;
	string getName() const;
	list<Line*> getLineList() const;

	void addLine(Line* line);
	Line* findLine(const string& id);
	void printStation();


private:
	string name_;
	int id_;
	list<Line*> lines_;
};
#endif
