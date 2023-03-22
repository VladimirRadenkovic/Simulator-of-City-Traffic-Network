#include "InfoSystem.h"
void startPrint(int& i) {
	cout << "1. U?itavanje podataka o mreži gradskog prevoza" << endl;
	cout << "0. Kraj rada" << endl;
	cin >> i;
	if (i != 0 && i != 1) {
		cout << "Nevalidan unos, molimo Vas ponovo izaberite opciju." << endl;
			startPrint(i);
	}
}
void printOptions(int&j) {
	cout << "1. Prikaz informacija o stajalištu" << endl;
	cout << "2. Prikaz osnovnih informacija o liniji gradskog prevoza" << endl;
	cout << "3. Prikaz statisti?kih informacija o liniji gradskog prevoza" << endl;
	cout << "4. Pronalazak putanje izme?u dva stajališta sa najmanjim brojem stajališta" << endl;
	cout << "5. Pronalazak putanje izme?u dva stajališta sa najmanjim brojem presedanja" << endl;
	cout << "0. Kraj rada" << endl;
	cin >> j;
}
int main() {
	cout << "Dobrodošli u simulator mreže gradskog prevoza. Molimo Vas, odaberite opciju:" << endl;
	int i;
	startPrint(i);
	while (i != 0) {
		string stationInputName, lineInputName;
		cout << "Molimo Vas, unesite putanju do fajla sa stajalištima:" << endl;
		cin >> stationInputName;
		cout << "Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:" << endl;
		cin >> lineInputName;
		InfoSystem* s = new InfoSystem(stationInputName, lineInputName);
		cout << "Mreža gradskog prevoza je uspešno u?itana. Molimo Vas, odaberite opciju:" << endl;
		int j;
		printOptions(j);
		int stationId1, stationId2;
		string lineId;
		while (j != 0) {
			try{
				switch (j) {
				case 1:
					cout << "Molimo Vas, unesite šifru stanice ?ije osnovne informacije želite da prikažete." << endl;
					cin >> stationId1;
					s->printStationInfo(stationId1);
					break;
				case 2:
					cout << "Molimo Vas, unesite oznaku linije ?ije osnovne informacije želite da prikažete." << endl;
					cin >> lineId;
					s->printLineInfo(lineId);
					break;
				case 3:
					cout << "Molimo Vas, unesite oznaku linije ?ije statisti?ke informacije želite da prikažete." << endl;
					cin >> lineId;
					s->lineStats(lineId);
					break;
				case 4:
					cout << "Molimo Vas, unesite šifre stajališta za pronalazak putanje." << endl;;
					cin >> stationId1;
					cin >> stationId2;
					s->findShortestRoute(stationId1, stationId2);
					break;
				case 5:
					cout << "Molimo Vas, unesite šifre stajališta za pronalazak putanje." << endl;
					cin >> stationId1;
					cin >> stationId2;
					s->findLeastLineRoute(stationId1, stationId2);
					break;
				case 0:  break;
				default:
					cout << "Nevalidan unos, molimo Vas ponovite unos";
					cin >> j;
					continue;
				}
			}
			catch (InvalidStationId& i) {
				cout << i.what() << i.getId() << ". Molimo Vas, ponovite unos." << endl;
				continue;
			}
			catch (InvalidLineId& i) {
				cout << i.what() << i.getId() << ". Molimo Vas, ponovite unos." << endl;
				continue;
			}
			catch (NoPathBetweenTwoLines& i) {
				cout << i.what() << i.getStartId() << " " << i.getEndId() << ". Molimo Vas, ponovite unos." << endl;
				int n;
				cin >> n;
				if (n != 0) continue;
				continue;
			}
			cout << endl;
			printOptions(j);
		}
		startPrint(i);
		if (i == 0) delete s;
	}
	return 0;
}