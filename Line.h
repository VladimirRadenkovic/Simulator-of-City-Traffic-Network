#ifndef LINE_H
#define LINE_H

#include "Station.h"
#include<fstream>
#include<list>
#include<string>
#include<vector>

using namespace std;
class Station;
class Line {
	// Klasa linija sadrzi ime linije i stajalista kroz koja prolazi linija u oba smera.
public:
	Line(list<Station*> stations, const string& id = "", const string& routes = "");
	
	string getId() const;
	list<Station*>& getRoute(char r);
	Station* getStart() const;
	Station* getEnd() const;
	
	void printLine(); 
	bool checkLine(list<Line*>& maxline, int& max, Line* current); // Pomocna funckija za pronalazenje statistike linije
private:
	string id_; 
	list<Station*> routeA_; //Stajalista kroz koje prolazi linija pocetne stanice do okretnice
	list<Station*> routeB_; // Stajalista kroz koje prolazi linija od okretnice do pocetne stanice

	int readStation(const string& routes, int& i, int& j);
	Station* findStation(list<Station*> stations, int id);

	void compareLines(Station* current, list<Station*> cmpRoute, int& n, bool& f);

};
#endif