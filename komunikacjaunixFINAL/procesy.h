#ifndef procesy_h
#define procesy_h
#include<iostream>
#include<string>
#include<ctime>
#include<cstdio>
#include<vector>
#include<list>
#include<queue>

using namespace std;
int licznik = 1;

class PCB
{
private:
	int PID; //identyfikator procesu
	string nazwaprocesu;//nazwa
	string stanprocesu; // stan procesu
	int priorytet; /// priorytet procesu -  poczatkowo 15;
	string komunikat;

public:
	PCB()
	{
	}
	//-----------------Konstruktor bloku kontrolnego procesora-------------//
	PCB(string name, string stanprocesu, string komunikat)
	{
		cout << "-----------------------------" << endl;
		cout << "Nowy proces: " << endl;
		nazwaprocesu = name;
		cout << "Nazwa: " << nazwaprocesu << endl;

		cout << "Identyfikator procesu: " << licznik << endl;

		priorytet = 15;
		cout << "Priorytet procesu: " << priorytet << endl;

		this->stanprocesu = stanprocesu;
		cout << "Stan procesu: " << stanprocesu << endl;

		this->komunikat = komunikat;
		cout << "komunikat: " << komunikat << endl;
		cout << "-----------------------------" << endl;
		licznik++;
	}
	//---------------------------------------------------Metody dostepu do zmiennych prywatnych
	void setkomunikat(string komunikat)
	{
		this->komunikat = komunikat;
	}

	void setstanprocesu(string stanprocesu)
	{
		this->stanprocesu = stanprocesu;
	}

	string getkomunikat()
	{
		return komunikat;
	}
	string getstanprocesu()
	{
		return stanprocesu;
	}
	string getnazwaprocesu()
	{
		return nazwaprocesu;
	}

	int getPID()
	{
		return PID;
	}
};
#endif
