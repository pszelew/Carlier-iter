#pragma once

#include <limits>
#include "result_shrage.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>

class Carlier_var
{
	public:
		Carlier_var();
		std::vector<zadanie> lista_zadan; //wektor przechwoujacy liste zadan do wykonania
		int UB;
		int LB;
		int U;
		int a; //poczatek sciezki krytycznej
		int b; //koniec sciezki krytycznej 
		int c; //zadanie iterferencyjne
		int sLB; //szybki LB (lower band)
		int n; //ilosc zadan
		long long flag;
		int r_k;
		int p_k;
		int q_k;
		int CMax;
		int depth = 0;
		long long id=0;

		int schrage_inter(); //algorytm schrage z podzialem
		int schrage_normal(); //zwykly algorytm schrage
		int check_time();
		int h(int a_t, int b_t);
		int rMin(int a_t, int b_t); //najmniejsze r z przedzialu
		int qMin(int a_t, int b_t); // najmniejsze q z przedzialu
		int rMinK(); //Minimalne r bloku krytycznego
		int qMinK();//Minimalne q bloku krytycznego
		int sPK(); // suma p bloku krytycznego
		int sP(int a_t, int b_t); // suma p elementow o wybranych indeksach
		int check_time_stop_working(int k);

		void findAB(); //szuka poczatku oraz konca sciezki krytycznej

		bool findC(); // szuka zadania interfenencyjnego

		int fastLB(); // szybki schrage z podzialem

		void eliminacja(int UB); //szybkie testy eliminacyjne ==> nie wchodzimy tam gdzie nie ma potrzeby wchodzic 
};

class Carlier_alg
{
public:
	long long int id=0;
	long long int count=0;
	Carlier_alg();
	void load_data(std::string plik); //wczytaj dane
	int do_carlier_full();
	int do_carlier_notests();
	int do_carlier_nostrat();
	Carlier_var perm_start;
private:
	int LB, UB;
	int n; // dlugosc uszeregowania
	bool opt = false; //czy znaleziono juz rozwiazanie optymalne

	
	Carlier_var optimum;
	Carlier_var acctual;
	Carlier_var left;
	Carlier_var right;

	//kopiec zrobiony na wektorze z biblioteki STL
	std::vector<Carlier_var> heap;

};