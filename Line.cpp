#include "Line.h"

Line::Line(list<Station*> stations, const string& id, const string& routes): id_(id){
	int i = 0;
	int j = routes.find("#");
	while (i != -1) {
		if (i < j)
			routeA_.push_back(findStation(stations, readStation(routes, i, j)));
		else
			routeB_.push_back(findStation(stations, readStation(routes, i, j)));
	}
}

string Line::getId() const {
	return id_;
}

list<Station*>& Line::getRoute(char r){
	if (r == 'A') return routeA_;
	if (r == 'B') return routeB_;
}

Station* Line::getStart() const {
	return routeA_.front();
}

Station* Line::getEnd() const {
	return routeB_.front();
}



void Line::printLine() {
	ofstream outputFile("linija_" + id_ + ".txt", ios::out);
	outputFile << id_ << " " << getStart()->getName() << "->" << getEnd()->getName() << endl;
	for (auto i : routeA_) 
		outputFile << i->getId() << " " << i->getName() << endl;
	for (auto i : routeB_) {
		outputFile << i->getId() << " " << i->getName();
		if(i != routeB_.back()) outputFile << endl;
	}
	cout << "Generisan je fajl linija_"  << id_ << ".txt" <<  "Sa osnonvim informacijama o liniji" << id_ << "." << endl;
	outputFile.close();
}

bool Line::checkLine(list<Line*>& maxline, int& max, Line* current){
	// metoda utvrdjuje da li dve linije imaju zajednicka stajalista i broj zajednickih stajalista.
	int n = 0;
	bool f = false;
	for (auto i : routeA_) {
			compareLines(i, current->routeA_, n, f); 
			compareLines(i, current->routeB_, n, f);
	}
	for (auto i : routeB_) 
		if (i != getEnd()) {
			compareLines(i, current->routeA_, n, f);
			compareLines(i, current->routeB_, n, f);
		}

	if (n == max) maxline.push_back(current);
	else if (n > max) {
		max = n;
		maxline.clear();
		maxline.push_back(current);
	}
	return f;
}


int Line::readStation(const string& routes, int& i, int& j){
	int k = routes.find(" ", i + 1);
	int a = (i == 0) ? i : i + 1;
	if (k > j && i < j) {
		i = j;
		return stoi(routes.substr(a, j - a));
	}
	else {
		i = k;
		if (i == -1) return stoi(routes.substr(a));
		else return stoi(routes.substr(a, k - a));
	}
}

Station* Line::findStation(list<Station*> stations, int id){
	for (auto i : stations) 
		if (id == i->getId()) {
			if (!i->findLine(id_)) i->addLine(this);
			return i;
		}
	return nullptr;
}

void Line::compareLines(Station* current, list<Station*> cmpRoute, int& n, bool& f){
	for (auto j : cmpRoute) 
		if (current->getId() == j->getId()) {
			n++;
			f = true;
		}
}




