#include "InfoSystem.h"

InfoSystem::InfoSystem(const string& stationFileName, const string& lineFileName){
	createStationList(stationFileName);
	createLineList(lineFileName);
	createAdjList();
}

InfoSystem::InfoSystem() {
	// Konstruktor kreira liste stanica i stajalista i listu susednosti.
	createStationList();
	createLineList();
	createAdjList();
}

InfoSystem::~InfoSystem(){
	for (auto i : stations_) 
		delete i;
	for (auto i : lines_) 
		delete i;
	for (auto i : adjList_) 
		delete i;
}

void InfoSystem::printSystem(){
	for (auto i : stations_) 
		i->printStation();
	for (auto i : lines_) 
		i->printLine();
}

void InfoSystem::printStationInfo(int id) {
	getStation(id)->printStation();
}

void InfoSystem::printLineInfo(const string& id){
	getLine(id)->printLine();
}

void InfoSystem::lineStats(const string& id) {
	Line* targetLine = getLine(id);
	ofstream outputFile("statistika_" + id + ".txt", ios::out);
	outputFile << targetLine->getId() << endl;
	int max = 0;
	list<Line*> maxLine;
	for (auto i : lines_) 
		if (i != targetLine)
			if (targetLine->checkLine(maxLine, max, i))
				outputFile << i->getId() << " ";
	if (max > 0) {
		outputFile << endl;
		for (auto t : maxLine) {
			outputFile << t->getId();
			if (t != maxLine.back()) outputFile << ", ";
		}
	}
	cout  << "Generisan je fajl statistika_" << id << ".txt " << "sa statisti?kim informacijama o liniji" <<  endl;
	outputFile.close();
}

void InfoSystem::printAdjList(){
	for (auto i : adjList_) {
		cout << "Stajaliste:" << i->getStation()->getId() << endl;
		for (auto j : i->getList()) 
			cout << "[" << j->getDest()->getStation()->getId() << ", " << j->getNodeLine()->getId() << "], ";
		cout << endl << endl;
	}
}
void InfoSystem::findShortestRoute(int startId, int endId) {
	/* Primenjuje se modifikovana verzija Breadth-first search algoritma u kome se cuva niz prethodnih cvorova trenutnim cvorovima.
	 Kada se pri prolasku kroz graf dodje do destinacije zavrsava se BFS algoritam i rekonstruise se putanja od pocetnog cvorado destinacionog cvora.
	 Koristi se mapa kako bi se omogucio rad sa nizovima. Celi brojevi od 0 do broja cvorova u grafu se koriste za predstavljanje cvorova u nizovima koji se koriste u BFS
	 algoritmu.
	*/
		AdjNode* startNode = getAdjNode(startId);
		AdjNode* endNode = getAdjNode(endId);
		vector<int> pred; // niz prethodnih cvorova.
		vector<bool> visited;
		vector<int> distance;
		vector<vector<int>> adj;
		map<int, AdjNode*> map = createMap(startNode, pred, visited, distance, adj); // kreira se mapa i inicijalizuju se vektori potrebni za BFS algoritam.
		initialiseAdjVect(map, adj); // Incijalizuje se vektor koji predstavlja listu susednosti.
		int end = getMapElem(map, endNode);
		int start = getMapElem(map, startNode);
		findRoute(end, start, pred, visited, distance, adj); // BFS algoritam
		vector <int> path = initialisePathVect(pred, end); // Niz koji predstavlja putanju od krajnjeg do pocetnog cvora.
		printShortestRoute(map, path, start, end, startNode->getStation()->getId(), endNode->getStation()->getId());
}

void InfoSystem::findLeastLineRoute(int startId, int endId){
	/*Rekurzivnim prolaskom kroz liste linija i stanica dolazi se do stanica na kojima je potrebno izvrsiti presedanje i potrebnim linijama. Metoda createLineRoute se poziva rekurzivno.*/
	Station* startStation = getStation(startId);
	Station* endStation = getStation(endId);
	list<Station*> visitedStations; // Lista posecenih stajalista, koristi se kako ne bi dolazilo do beskonacnih rekurzija.
	deque<Line*> lineDeque; // Dek sa potrebnim linijama.
	deque<Station*> stationDeque; // Dtek sa potrebnim stajalistima.
	queue<Station*> stationsToVisit; // Na ovom steku se cuvaju stajalista koja je potrebno posetiti u rekurziji.
	stationDeque.push_back(startStation);
	if (createLineRoute(startStation, endStation, visitedStations, lineDeque, stationDeque,stationsToVisit))stationDeque.push_back(endStation);
	else throw NoPathBetweenTwoLines(startId, endId);
	if (startStation != stationDeque.front()) { // Ukoliko pocetna stanica nije dodata u dek sa stajalistima potrebno je dodati pocetno stajaliste i odgovarajucu liniju od pocetnog stajalista do sledeceg.
		findRoute(startStation, stationDeque.front(), lineDeque, stationDeque); 
		stationDeque.push_front(startStation);
		lineDeque.push_front(lineDeque.back());
		lineDeque.pop_back();
	}
	printLeastLinesRoute(lineDeque, stationDeque);
}

bool InfoSystem::createLineRoute(Station* currentStation, Station* endStation, list<Station*>& visitedStations, deque<Line*>& lineDeque, deque<Station*>& stationDeque, queue<Station*>& stationsToVisit){
	if (findRoute(currentStation, endStation, lineDeque, stationDeque)) {
		return true;
	}
	visitedStations.push_back(currentStation);
	for (auto i : currentStation->getLineList()) {
		lineDeque.push_back(i);
		list<Station*> traverseList = getTraverseList(i, currentStation);
		for (auto j : traverseList) {
			bool visited = false;
			for (auto k : visitedStations) {
				if (j == k) {
					visited = true;
					break;
				}
			}
			if (!visited) stationsToVisit.push(j);
			stationDeque.push_back(j);
			if (findRoute(j, endStation, lineDeque, stationDeque)) return true;
			stationDeque.pop_back();
		}
		lineDeque.pop_back();
	}
	stationDeque.pop_back();
	if (!stationsToVisit.empty()) {
		currentStation = stationsToVisit.front();
		stationsToVisit.pop();
		stationDeque.push_back(currentStation);
		if(createLineRoute(currentStation, endStation, visitedStations, lineDeque, stationDeque, stationsToVisit)) return true;
	}
	return false;
}


Station* InfoSystem::getStation(int id) {
		for (auto i : stations_)
			if (i->getId() == id) return i;
		throw InvalidStationId(id);
}

AdjNode* InfoSystem::getAdjNode(int id){
	for (auto i : adjList_) 
		if (id == i->getStation()->getId()) return i;
	throw InvalidStationId(id);
}

Line* InfoSystem::getLine(const string& id){
	for (auto i : lines_) 
		if (id == i->getId()) return i;
	throw InvalidLineId(id);
}

void InfoSystem::createStationList(const string& inputFile)
{
	fstream stationFile(inputFile, ios::in);
	while (stationFile.peek() != EOF) {
		string element;
		getline(stationFile, element);
		int i = element.find(" ");
		stations_.push_back(new Station(element.substr(i + 1), stoi(element.substr(0, i))));
	}
	stationFile.close();
}

void InfoSystem::createStationList(){
	fstream stationFile("stajalista.txt", ios::in);
	while (stationFile.peek() != EOF) {
		string element;
		getline(stationFile, element);
		int i = element.find(" ");
		stations_.push_back(new Station(element.substr(i + 1), stoi(element.substr(0, i))));
	}
	stationFile.close();
}

void InfoSystem::createLineList(const string& inputFile){
	fstream lineFile(inputFile, ios::in);
	while (lineFile.peek() != EOF) {
		string element;
		getline(lineFile, element);
		int i = element.find(" ");
		lines_.push_back(new Line(stations_, element.substr(0, i), element.substr(i + 1)));
	}
	lineFile.close();
}

void InfoSystem::createLineList(){
	fstream lineFile("linije.txt", ios::in);
	while (lineFile.peek() != EOF) {
		string element;
		getline(lineFile, element);
		int i = element.find(" ");
		lines_.push_back(new Line(stations_, element.substr(0, i), element.substr(i + 1)));
	}
	lineFile.close();
}


void InfoSystem::createAdjList(){
	// Kreira se graf. Prvo se na osnovu stanica kreiraju cvorovi, zatim se prolaskom kroz listu svih linija inicijalizuje lista susednosti svakog cvora.
	for (auto i : stations_) 
		adjList_.push_back(new AdjNode(i));

	for (auto i : adjList_) 
		for (auto j : lines_) {
			list<Station*> route;
			addAdjListNodes(i, j,'A');
			addAdjListNodes(i,j,'B');
	}
}

void InfoSystem::addAdjListNodes(AdjNode* currNode, Line* currLine, char r){
	Station* currStation = currNode->getStation();
	list<Station*> route = currLine->getRoute(r);
	for (auto it = route.begin(); it != prev(route.end()); it++)
		if ((*it) == currStation)
			currNode->addNode(new AdjListNode(getAdjNode((*next(it, 1))->getId()), currLine));

}

int InfoSystem::getMapElem(map<int, AdjNode*> map, AdjNode* elem){
	for (auto it = map.begin(); it != map.end(); it++) 
		if (it->second == elem) return it->first;
	return 0;
}


map<int, AdjNode*> InfoSystem::createMap(AdjNode* startNode, vector<int>& pred, vector<bool>& visited, vector<int>& distance, vector<vector<int>>& adj){
	map<int, AdjNode*> map;
	int i = 0;
	for (auto it = adjList_.begin(); it != adjList_.end(); it++, i++) {
		map.insert(pair<int, AdjNode*>(i, *it));
		vector<int> c;
		adj.push_back(c);
		pred.push_back(-1);
		if (!((*it) == startNode)) {
			visited.push_back(false);
			distance.push_back(INT_MAX);
		}
		else {
			visited.push_back(true);
			distance.push_back(0);
		}
	}
	return map;
}


void InfoSystem::initialiseAdjVect(map<int, AdjNode*> map, vector<vector<int>>& adj){
	for (auto it = map.begin(); it != map.end(); it++) 
		for (auto a = it->second->getList().begin(); a != it->second->getList().end(); a++) 
			adj[it->first].push_back(getMapElem(map, (*a)->getDest()));
}

void InfoSystem::findRoute(int end, int start, vector<int>& pred, vector<bool>& visited, vector<int>& distance, vector<vector<int>>& adj) {
	queue<int> queue;
	queue.push(start);
	while (!queue.empty()) {
		int u = queue.front();
		queue.pop();
		for (int i = 0; i < adj[u].size(); i++) {
			if (!visited[adj[u][i]]) {
				visited[adj[u][i]] = true;
				distance[adj[u][i]] = distance[u] + 1;
				queue.push(adj[u][i]);
				pred[adj[u][i]] = u;
				if (adj[u][i] == end) return;
			}
		}
	}
	throw NoPathBetweenTwoLines(start, end);
}

vector<int> InfoSystem::initialisePathVect(vector<int>& pred, int end) {
	vector <int> path;
	int crawl = end;
	path.push_back(crawl);
	while (pred[crawl] != -1) {
		path.push_back(pred[crawl]);
		crawl = pred[crawl];
	}
	return path;
}

void InfoSystem::printShortestRoute(map<int, AdjNode*> map, vector<int> path, int start, int end, int startStation, int endStation){
	ofstream outputFile("putanja_" + to_string(startStation) + "_" + to_string(endStation) + ".txt", ios::out);
	Line* currentLine = nullptr;
	Line* newLine = nullptr;
	for (int i = path.size() - 1; i >= 0; i--) {
		for (auto t : map[path[i]]->getList())
			if (i > 0)
				if (t->getDest() == map[path[i - 1]]) {
					newLine = t->getNodeLine();
					break;
				}
		if (currentLine != nullptr) {
			outputFile << map[path[i]]->getStation()->getId() << " ";
			if (currentLine != newLine) outputFile << endl << currentLine->getId() << "->" << newLine->getId() << endl << map[path[i]]->getStation()->getId() << " ";
		}
		else  outputFile << "->" << newLine->getId() << endl << map[path[i]]->getStation()->getId() << " ";
		currentLine = newLine;
	}
	cout << "Generisan je fajl " << "putanja_" << to_string(startStation) << "_" << to_string(endStation) << ".txt " << "sa najkra?om putanjom izme?u stajališta" << startStation << " " << endStation << endl;
	outputFile.close();
}

bool InfoSystem::compareCurEndLines(list<Line*> linesToVisit, Station* endStation, deque<Line*>& lineDeque){
	for (auto i : linesToVisit) 
		for (auto j : endStation->getLineList()) 
			if (i == j) {
				lineDeque.push_back(i);
				return true;
			}
	return false;
}

bool InfoSystem::compareLineStations(list<Station*> traverseList, Line* currentLine, Line* currEndLine, deque<Line*>& lineDeque, deque<Station*>& stationDeque){
	for (auto i : traverseList) {
		if (compareCurStation(currentLine, currEndLine, i, currEndLine->getRoute('A'), lineDeque, stationDeque)) return true;
		if (compareCurStation(currentLine, currEndLine, i, currEndLine->getRoute('B'), lineDeque, stationDeque)) return true;
	}
	return false;
}

list<Station*> InfoSystem::getTraverseList(Line* currentLine, Station* currentStation){
	list<Station*> traverseList;
	for (auto it = currentLine->getRoute('A').begin(); it != currentLine->getRoute('A').end(); it++) 
		if (*it == currentStation) {
			traverseList.insert(traverseList.end(), it, currentLine->getRoute('A').end());
			traverseList.insert(traverseList.end(), next(currentLine->getRoute('B').begin(),1), currentLine->getRoute('B').end());
			if (it != currentLine->getRoute('A').begin())
				traverseList.insert(traverseList.end(), currentLine->getRoute('A').begin(), prev(it,1));
			return traverseList;
		}
	
	for (auto it = currentLine->getRoute('B').begin(); it != currentLine->getRoute('B').end(); it++) 
		if (*it == currentStation) {
			traverseList.insert(traverseList.end(), it, currentLine->getRoute('B').end());
			traverseList.insert(traverseList.end(), next(currentLine->getRoute('A').begin(),1), currentLine->getRoute('A').end());
			if (it != currentLine->getRoute('B').begin())
				traverseList.insert(traverseList.end(), currentLine->getRoute('B').begin(), prev(it,1) );
			return traverseList;
		}
}
bool InfoSystem::compareCurStation(Line* currLine, Line* currEndLine, Station* currStation, list<Station*> route, deque<Line*>& lineDeque, deque<Station*>& stationDeque){
	for (auto g : route)
		if (g == currStation) {
			stationDeque.push_back(g);
			lineDeque.push_back(currLine);
			lineDeque.push_back(currEndLine);
			return true;
		}
	return false;
}

void InfoSystem::printLeastLinesRoute(deque<Line*>& lineDeque, deque<Station*>& stationDeque){
	int start = stationDeque.front()->getId(), end = stationDeque.back()->getId();
	ofstream outputFile("putanja_" + to_string(start) + "_" + to_string(end) + ".txt", ios::out);
	Line* current = nullptr;
	while (!lineDeque.empty()) {
		Line* newLine = lineDeque.front();
		Station* currentStation = stationDeque.front();
		stationDeque.pop_front();
		lineDeque.pop_front();
		if (current == nullptr) outputFile << "->" << newLine->getId() << endl;
		else outputFile << current->getId() << "->" << newLine->getId() << endl;
		current = newLine;
		for (auto i : getTraverseList(current, currentStation)) {
			outputFile << i->getId() << " ";
			if (i == stationDeque.front()) break;
		}
		outputFile << endl;
	}
	outputFile.close();
	cout << "Generisan je fajl " << "putanja_" << start << "_" << end << ".txt " << "sa najkra?om putanjom izme?u stajališta" << start << " " << end << endl;
}

list<Line*> InfoSystem::getLineList(Station* currentStation, Line* currentLine)
{
	list<Line*> lineList = currentStation->getLineList();
	if (currentLine != nullptr) {
			for (auto current = lineList.begin(); current != lineList.end(); current++)
				if (currentLine == *current) {
					Line* curr = *current;
					lineList.erase(current);
					lineList.push_front(curr);
					return lineList;
				}
	}
	return lineList;
}

bool InfoSystem::findRoute(Station* currentStation, Station* endStation, deque<Line*>& lineDeque, deque<Station*>& stationDeque){
	for (auto i : currentStation->getLineList())
		if (compareCurEndLines(currentStation->getLineList(), endStation, lineDeque)) 	return true;
	for (auto i : currentStation->getLineList()) 
		for (auto j : endStation->getLineList())
			if (compareLineStations(getTraverseList(i, currentStation), i, j, lineDeque, stationDeque)) return true;
	return false;
}


AdjListNode::AdjListNode(AdjNode* dest, Line* line): dest_(dest), line_(line){}

AdjNode* AdjListNode::getDest(){
	return dest_;
}

Line* AdjListNode::getNodeLine(){
	return line_;
}

AdjNode::AdjNode(Station* station): station_(station){}

AdjNode::~AdjNode(){ 
	for (auto j : adjNodeList_)
		delete j;
}

list<AdjListNode*>& AdjNode::getList(){
	return adjNodeList_;
}

Station* AdjNode::getStation() const{
	return station_;
}

void AdjNode::addNode(AdjListNode* newNode){
	adjNodeList_.push_back(newNode);
}




