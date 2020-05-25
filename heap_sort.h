#pragma once
#include "result_shrage.h"


bool comparer(zadanie a, zadanie b)
{

	if (a.R < b.R) return false;
	if (a.R == b.R)
	{
		if (a.id < b.id) return false;
		else return true;
	}
	if (a.R > b.R) return true;
}

bool compareq(zadanie a, zadanie b)
{

	if ((a).Q < (b).Q) return true;
	if ((a).Q == (b).Q)
	{
		if ((a).id < (b).id) return false;
		else return true;
	}
	if ((a).Q > (b).Q) return false;
}



void zamien_elementy(zadanie *tablica, int pierwszy, int drugi)
{
	zadanie i = tablica[pierwszy];
	tablica[pierwszy] = tablica[drugi];
	tablica[drugi] = i;
}

void heapify(zadanie *tablica, int size, int node, bool compr)
{
	int maximum = node;
	int lewy = 2 * node + 1;
	int prawy = 2 * node + 2;
	int ostatni = size-1;
	if (compr)
	{
		if (lewy <= ostatni && !comparer(tablica[lewy], tablica[maximum]))
		{
			maximum = lewy;
		}
		if (prawy <= ostatni && !comparer(tablica[prawy], tablica[maximum]))
		{
			maximum = prawy;
		}

		if (node != maximum)
		{
			zamien_elementy(tablica, maximum, node);
			heapify(tablica, size, maximum, compr);
		}
	}
	else
	{
		if (lewy <= ostatni && !compareq(tablica[lewy], tablica[maximum]))
		{
			maximum = lewy;
		}
		if (prawy <= ostatni && !compareq(tablica[prawy], tablica[maximum]))
		{
			maximum = prawy;
		}

		if (node != maximum)
		{
			zamien_elementy(tablica, maximum, node);
			heapify(tablica, size, maximum, compr);
		}
	}
}

void make_heap(zadanie *tablica, int size, bool rosnaco)
{
	/*Z drzewa binarnego tworzymy kopiec */
	for (int i = ((size - 2) / 2); i >= 0; i--)
	{
		heapify(tablica, size, i, rosnaco);
	}
}

void heap_sort(zadanie *tablica, int size, bool rosnaco)
{
	int ostatni = size - 1;
	int pierwszy = 0; 
	/*Z drzewa binarnego tworzymy kopiec */
	for (int i = ((size - 2) / 2); i >= 0; i--)
	{
		heapify(tablica, size, i, rosnaco);
	}

	for (int i = ostatni; i >= pierwszy; i--)
	{
		zamien_elementy(tablica, pierwszy, i);
		/*Wywolanie kopcowania dla zredukowanego kopca*/
		heapify(tablica, i, pierwszy, rosnaco);
	}
}

void pop(zadanie *tablica, int &size, bool compr)
{
	zamien_elementy(tablica, 0, --size);
	//odtworzenie kopca
	make_heap(tablica, size, compr);
}

void push(zadanie *tablica, int &size, bool compr, zadanie value)
{
	tablica[size++] = value;
	//odtworzenie kopca
	make_heap(tablica, size, compr);
}