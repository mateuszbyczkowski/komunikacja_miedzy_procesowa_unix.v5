//DANE typu string, maksymalna ilosc przesylanych danych wynosi 3, dlugsc FIFO = 4 
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "structiwezel.h"
#include <conio.h>
#include <string>
#include <stdlib.h>
#include "procesy.h"
#include <fstream>
#include <windows.h>
using namespace std;

bool reading = false;
bool writing = false;

void fifoinit()//inicjuje strukture iwezel, pipe_inod_info
{
	node.start = 0;//pocz¹tek pliku
	node.koniec = 0;//koniec pliku
	node.reader = 0;//brak czytelnikow
	node.writer = 0;//brak pisarzy
	node.lock = -1;//-1 zamkniete, 1 otwarte do pisania, 0 otwarte do czytania
}

class PIPE
{
protected:
	char x;//sprawdza kto otworzy³ ³¹cze aby rozró¿niæ operacje open-zapis i open-odczyt
	fstream plik1;
public:

	PIPE() :x(0)
	{
		fifoinit();//wypelnienie struktury pipe_inode_info 
		cout << "Lacze utworzone pomyslnie" << endl;
	}

	void zamknijw()//zamyka lacze writera
	{
		plik1.close();
		node.lock = 0;//zamknieto i umozliwiono dostep do czytania
		node.writer = 0;//mozna czytac
		cout << "Zamykanie ze strony pisarza zakonczone sukcesem, dane gotowe do odczytu \nLacze zostalo odblokowane, liczba procesow piszacych wynosi " << node.writer << endl;
		cout << "/*--------------------------------------------------------*/" << endl;
		cout << endl;
		cout << "Dowolny klawisz aby kontynuowac..." << endl;
		getchar();
		getchar();
	}
	void zapis(string kom)
	{
		plik1 << kom << endl;
		cout << "| Pomyslnie zapisano dane do lacza |" << endl;
	}
	void open(string komunikat, char operacja)//do zapisu
	{

		if (operacja == 'w' && node.reader == 0)
		{
			string tmp1, tmp2, tmp3, tmp4;//zmienne sluzace do dzielenia komunikatu procesu na rozkazy
			node.koniec = 11;//od 0 do 11, 3*4
			node.lock = 1;
			node.writer = 1;//ustawia noda
			plik1.open("kom.txt", ios::out);//otwiera do zapisu
			cout << "Lacze zostalo otworzone do zapisu " << endl;

			int dlugosc = komunikat.size();
			if (dlugosc <= 12)
			{
				if (dlugosc == 3)
				{
					zapis(komunikat);//zapisuje jeden komunikat
				}
				else if (dlugosc == 6)
				{
					tmp1 = komunikat.substr(0, 3);//zapisuje dwa komunikaty
					tmp2 = komunikat.substr(3, 3);
					zapis(tmp1);
					zapis(tmp2);
				}
				else if (dlugosc == 9)
				{
					tmp1 = komunikat.substr(0, 3);//zapisuje trzy komunikaty
					tmp2 = komunikat.substr(3, 3);
					tmp3 = komunikat.substr(6, 3);
					zapis(tmp1);
					zapis(tmp2);
					zapis(tmp3);
				}
				else if (dlugosc == 12)
				{
					tmp1 = komunikat.substr(0, 3);//zapisuje cztery komunikaty maksymalna dlugosc to 12, 4*3znaki
					tmp2 = komunikat.substr(3, 3);
					tmp3 = komunikat.substr(6, 3);
					tmp4 = komunikat.substr(9, 3);
					zapis(tmp1);
					zapis(tmp2);
					zapis(tmp3);
					zapis(tmp4);
				}
				else { cout << "Blad: niepoprawna dlugosc danych" << endl; }
			}
			else { cout << "Blad: niepoprawna dlugosc danych" << endl; }

			zamknijw();//zamyka-odblokowywuje do odczytu
		}
		else if (operacja != 'w')
		{
			cout << " Blad: niepoprawna operacja " << endl;
		}
	}
	string odczyt(string kom1)
	{
		int nr_wiersza = 0;
		string wiersz = "";
		while (!plik1.eof())  //petla wykona sie dodatkowo o 1 obrot wiecej bo eof
		{
			getline(plik1, wiersz);
			nr_wiersza++;
			kom1.append(wiersz);
		}
		cout << "Poprawnie odczytano cala zawartosc lacza, jest ona nastepujaca: " << kom1 << endl;
		//TUTAJ BEDZIE WPISANIE DO PROCESU CO ZOSTA£O ODCZYTANE a nie wyswietlanie tylko, starczy zwracanie do funkcji i przekazanie do zmiennej
		return kom1;
	}
	void zamknijr()
	{
		node.lock = -1;
		node.reader = 0;
		plik1.close();
		plik1.open("kom.txt", ios::out|ios::trunc);
		plik1.close();
		cout << "Pomyslnie zamknieto lacze, liczba procesow czytajacych wynosi " << node.reader << endl;
		cout << "/*--------------------------------------------------------*/" << endl;
		cout << endl;
		cout << "Dowolny klawisz aby kontynuowac..." << endl;
		getchar();
		getchar();
	}
	string open(char operacja)
	{
		string komunikatdocelowy = "";
		if (operacja == 'r')
		{
			if (node.lock = 0 || node.writer == 0)//jesli mozna odczytywac, odczytuj
			{
				plik1.open("kom.txt", ios::in);//otwiera do odczytu
				cout << "Lacze zostalo otworzone do odczytu" << endl;
				node.reader = 1;
				komunikatdocelowy = odczyt(komunikatdocelowy);
				node.koniec = 0;
				zamknijr();
				return komunikatdocelowy;
			}
			cout << "Blad: niepoprawna operacja" << endl; return komunikatdocelowy;
		}
		else { cout << "Blad: niepoprawna operacja" << endl; return komunikatdocelowy; }
		return komunikatdocelowy;
	}
	//-----
	static void unlink()
	{
		fifoinit();
		if (remove("kom.txt") == 0)
		{
			cout << "Lacze zostalo usuniete poprawnie" << endl;
			cout << "/*--------------------------------------------------------*/" << endl;
			cout << endl;
		}
		else
		{
			cout << "Blad: usuwanie lacza nie powiodlo sie." << endl;
			cout << "/*--------------------------------------------------------*/" << endl;
			cout << endl;
		}
	}
};

void wait(PCB *p)
{
	p->setstanprocesu("wait");
	cout << "Proces zostal uspiony" << endl;
}
void wait2(PCB *p)
{
	p->setstanprocesu("wait");
}
void wakeup(PCB *p)
{
	string run = "running";
	p->setstanprocesu(run);
	cout << "Proces zostal wybudzony, przyznano mu procesor" << endl;
	cout << "/*--------------------------------------------------------*/" << endl;
	cout << endl;
}

int main()
{
	cout << "************************************************************" << endl;
	cout << "********** KOMUNIKACJA MIEDZYPROCESOWA - UNIX v.5 **********" << endl;
	cout << "************************************************************" << endl;
	cout << endl;
	char k = 'l';
	cout << "/*--------------------------------------------------------*/" << endl;
	cout << endl;
	while (true)
	{
	cout << "Czy chcesz przeprowadzic test? (y)es/(n)o" << endl;
	cin >> k;
	cout << "/*--------------------------------------------------------*/" << endl;
	cout << endl;
	if (k == 'y')
	{
	PIPE rura;
	char x = 'a';
	char y = 'a';
	char z = 'a';
	string komunikat;
	string running = "running";
	string waiting = "wait";
	cout << "Stworzono dwa procesy testowe: " << endl;
	PCB *p1 = new PCB("proces1", "new", "aaabbbcccddd");//dwa procesy
	wait2(p1);//uspione oba procesy
	PCB *p2 = new PCB("proces2", "new", "dddcccbbb");
	wait2(p2);
	/*najpierw sprawdza dla procesu nr.1 jak jest r to wait jak jest w to zapisuje i daje procesowi nr.2 running */
	/*sprawdza dla procesu nr.2 jestli jest r to odczytuje bo dostal running, jesli jest w to konflikt nie mozna wpisac lacze jest pelne*/

	/*PROCES NR 1*/
	cout << "Podaj tryb operacji (w)rite/(r)ead dla procesu nr.1" << endl;
	while (true)
	{
		cin >> x;
		if (x == 'r' || x == 'w')//podajemy dla niego tryb otwarcia
		{
			break;
		}
		cout << "Bledny tryb, podaj jeszcze raz" << endl;
	}
	if (x == 'r')//dla odczytu usypiamy go czeka na wybudzenie przez drugi proces
	{
		wait(p1);//uspienie procesu.1
	}
	else if (x == 'w')//dla zapisu budzimy i zapisujemy 
	{
		wakeup(p1);
		if (p1->getstanprocesu() == "running")
		{
			cout << "Proces nr.1 w stanie running moze rozpoczaci przesylanie komunikatu" << endl;
			cout << "Czy chcesz uzyc wlasnego komunikatu do przeslania? (y)es/(n)o" << endl;
			while (true)
			{
				cin >> z;
				if (z == 'y')//wpisanie czegokolwiek niz Y spowoduje wybranie komunikatu domyslnego
				{
					cout << "Podaj komunikat do przekazania max. 12 znakow czyli 4 rozkazy po 3 znaki" << endl;
					while (true)
					{
						cin >> komunikat;
						if (komunikat.size() <= 12)
						{
							p1->setkomunikat(komunikat);
							break;
						}
						cout << "Blad: za dlugi komunikat podaj ponownie" << endl;
					}
				}
				else
				{
					cout << "Wybrano przekazanie domyslnego komunikatu" << endl;
					cout << "/*--------------------------------------------------------*/" << endl;
					cout << endl;
					komunikat = p1->getkomunikat();//pobiera komunikat od procesu nr.1
					break;
				}
				break;
			}
			rura.open(komunikat, x);//wywolanie otwierania i zapisu z parametrami
			wakeup(p2);//budzenie procesu nr.2 czyli przekazywanie mu procesora i umozliwienie odczytu
		}
	}
	/*PROCES NR 2*/
	cout << "Podaj tryb operacji (w)rite/(r)ead dla procesu nr.2" << endl;
	while (true)//petla sprawdza czy to nie jest inny znak niz r/w
	{
		cin >> y;
		if (y == 'r' || y == 'w')//podajemy dla niego tryb otwarcia
		{
			if (y != x)
			{
				break;
			}
			cout << "Blad: lacze zostalo juz uprzednio otworzone w tym trybie, wprowadz inny tryb" << endl;
		}
		cout << "Blad: nieprawidlowy tryb, podaj jeszcze raz" << endl;
	}

	if (y == 'r')//dla odczytu, moze czytac bo proces nr.1 ustawiony do pisania juz zapisal i przekazal procesor
	{
		if (p2->getstanprocesu() == "running")//TUTAJ ODCZYTUJE BO PIERWSZY JUZ NAPISAL, TERAZ MOZEMY ODCZYTAC
		{
			wait(p1);
			p2->setkomunikat(rura.open(y));
			cout << "Komunikat w procesie nr.1 identyczny: " << p1->getkomunikat() << endl;
			cout<< "do komunikatu w procesie nr.2: " << p2->getkomunikat() << endl;
		}
	}
	else if (y == 'w')//dla zapisu budzimy i zapisujemy bo pierwszy ŒPI, trzeba zapisac i obudzic PIERWSZEGO I OD NOWA MU OTWORZYC OPEN 
	{																							//¿eby to wgl dzia³a³o :P w dwie strony
		wakeup(p2);
		if (p2->getstanprocesu() == "running")
		{
			cout << "Proces nr.2 w stanie running moze rozpoczac przesylanie komunikatu" << endl;
			char z;
			cout << "Czy chcesz uzyc wlasnego komunikatu do przeslania? (y)es/(n)o" << endl;
			while (true)
			{
				cin >> z;
				if (z == 'y')
				{
					cout << "Podaj komunikat do przekazania max. 12 znakow czyli 4 rozkazy" << endl;
					while (true)
					{
						cin >> komunikat;
						if (komunikat.size() <= 12)
						{
							p2->setkomunikat(komunikat);
							break;
						}
						cout << "Blad: za dlugi komunikat podaj ponownie" << endl;
					}
				}
				else
				{
					cout << "Wybrano przekazanie domyslnego komunikatu" << endl;
					cout << "/*--------------------------------------------------------*/" << endl;
					cout << endl;
					komunikat = p2->getkomunikat();
					break;
				}
				break;
			}
			rura.open(komunikat, y);
			wakeup(p1);
		}
		if (x == 'r')//dla odczytu, moze czytac bo proces nr.2 ustawiony do pisania juz zapisal i przekazal procesor
		{
			if (p1->getstanprocesu() == "running")
			{
				p1->setkomunikat(rura.open(x));//ODCZYTYWANIE 
				cout << "Komunikat w procesie nr.1 identyczny: " << p1->getkomunikat() << endl;
				cout << "do komunikatu w procesie nr.2: " << p2->getkomunikat() << endl;
			}
		}
	}
	cout << "Trwa usuwanie lacza..." << endl;
	rura.unlink();
		}
		else
		{
			cout << "Trwa zamykanie programu..." << endl;
			break;
		}
	}
	cout << "Dowolny klawisz aby kontynuowac..." << endl;
	getchar();
	getchar();
	return 0;
}