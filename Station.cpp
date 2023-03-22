#include "Station.h"

Station::Station(const string& name, int id): name_(name), id_(id) {}

void Station::addLine(Line* line){ // Dodavanje linije u sortiranu listu linija koje prolaze kroz stanicu.
	if (lines_.empty()) {
		lines_.push_back(line);
		return;
	}
	string newLineId = line->getId();
	for (auto t = lines_.begin(); t != lines_.end(); t++) {
		string currLineId = (*t)->getId();
		for (int j = 0; j < currLineId.length(); j++) {
			if (newLineId[j] < currLineId[j]) {
				lines_.insert(t, 1, line);
				return;
			}
			else if (newLineId[j] > currLineId[j]) break;
		}
	}
	lines_.push_back(line);
}

int Station::getId() const{
	return id_;
}

string Station::getName() const
{
	return name_;
}

list<Line*> Station::getLineList() const
{
	return lines_;
}

Line* Station::findLine(const string& id){
	for (auto i : lines_) 
		if (id == i->getId()) return i;
	return nullptr;
}


void Station::printStation(){
	ofstream outputFile(to_string(id_) + "_" + name_ + ".txt", ios::out);
	outputFile << id_ << " " << name_ << " [";
	for (auto t : lines_) {
		outputFile << t->getId();
		if (t != lines_.back()) outputFile << " ";
	}
	outputFile << "]";
	outputFile.close();
	cout << "Generisan je fajl " << id_ << "_" << name_ << ".txt " << "Sa osnovnim informacijama o stajalištu " << id_ << name_ << endl;
}




