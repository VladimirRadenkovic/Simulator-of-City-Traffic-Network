#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <string>
using namespace std;
class InvalidStationId : public exception {
public:
	InvalidStationId(int id) : exception("Ne postoji stanica sa unetom sifrom: "), id_(id) {}
	int getId() {
		return id_;
	}
private: 
	int id_;
};

class InvalidLineId : public exception {
public:
	InvalidLineId(const string& id) : exception("Ne postoji linija sa unetom oznakom: "), id_(id) {}
	string getId() {
		return id_;
	}
private:
	string id_;
};

class NoPathBetweenTwoLines : public exception{
public:
	NoPathBetweenTwoLines(int startId, int endId) : exception("Ne postoji putanja izmedju unetih linija: "), startId_(startId), endId_(endId) {}
	int getStartId() {
		return startId_;
	}
	int getEndId() {
		return endId_;
	}
private:
	int startId_;
	int endId_;
};
#endif
