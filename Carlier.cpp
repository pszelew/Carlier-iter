#include "Carlier.h"



using namespace std;

Carlier_var::Carlier_var() : UB(INT_MAX), LB(0), U(0), r_k(INT_MAX), flag(0)
{

}


struct CompareR {
	bool operator()(zadanie const& p1, zadanie const& p2)
	{
		if (p1.R < p2.R) return false;
		if (p1.R == p2.R)
		{
			if (p1.id < p2.id) return false;
			else return true;
		}
		if (p1.R > p2.R) return true;
	}
};

struct CompareQ {
	bool operator()(zadanie const& p1, zadanie const& p2)
	{
		//return p1.Q < p2.Q;
		if (p1.Q < p2.Q) return true;
		if (p1.Q == p2.Q)
		{
			if (p1.id < p2.id) return false;
			else return true;
		}
		if (p1.Q > p2.Q) return false;
	}
};

struct CompareLB {
	bool operator()(Carlier_var const& p1, Carlier_var const& p2)
	{
		if (p1.sLB < p2.sLB) return true;
		if (p1.sLB == p2.sLB)
		{
			if (p1.id < p2.id) return true;
			else return false;
		}
		if (p1.sLB > p2.sLB) return false;
	}
};


int Carlier_var::check_time()
{
	int m = 0;
	int c = 0;
	int i = 0;

	for (int i = 0; i < n; i++)
	{
		m = max(m, lista_zadan[i].R) + lista_zadan[i].P;
		c = max(m + lista_zadan[i].Q, c);
	}
	return c;
}

int Carlier_var::check_time_stop_working(int k)
{
	int m = 0;
	int c = 0;
	int i = 0;

	for (int i = 0; i < k; i++)
	{
		m = max(m, lista_zadan[i].R) + lista_zadan[i].P;
		c = max(m + lista_zadan[i].Q, c);
	}
	return m;
}



int Carlier_var::schrage_normal() 
{
	//chciałbym otrzymać dane w vectorze
	vector<zadanie> N;
	for (int i = 0; i < n; i++)
	{
		N.push_back(lista_zadan[i]);
	}
	vector<zadanie> G;
	vector<zadanie> schr;
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	int pos = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	//list<zadanie> lista_zadan;
	//zadanie *tab_res = new zadanie[99];
	//odtworzenie kopca	

	std::make_heap(N.begin(), N.end(), CompareR()); //kopiec, w ktorym minimalizujemy R
	while (!G.empty() || !N.empty())
	{
		//odswiez zbior zadan gotowych
		while (!N.empty() && N[0].R <= t)
		{
			e = N[0];
			pe = N[0].P;
			G.push_back(e);
			push_heap(G.begin(), G.end(), CompareQ());
			pop_heap(N.begin(), N.end(), CompareR());
			N.pop_back();
		}
		//wyznacz t_next
		{
			if (!N.empty())
				t_next = N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (G.empty())
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania. Tutaj bedzie roznica w algorytmie
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G[0];
			//pe to czas wykonania nastepnego zadania
			pe = G[0].P;
			//wykonujemy cale zadanie
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				pop_heap(G.begin(), G.end(), CompareQ());
				G.pop_back();
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);
				t = t + pe;
			}
			schr.push_back(e);
		}
	}

	lista_zadan.clear();
	lista_zadan = schr;
	schr.clear();

	CMax = c;
	return c;
}


int Carlier_var::schrage_inter()
{
	//chciałbym otrzymać dane w vectorze
	vector<zadanie> N;
	for (int i = 0; i < n; i++)
	{
		N.push_back(lista_zadan[i]);
	}
	vector<zadanie> G;
	//zmienne do obliczenia czasu dzialania programu
	int m = 0;
	int c = 0;
	//reszta zmiennych programu
	int t = 0;
	zadanie e;
	int pe = 0;
	int t_next = 0;
	//musimy tutaj dac liste zadan do wykonania, zeby mozna ich bylo dodac nieskonczenie wiele
	//list<zadanie> lista_zadan;
	//odtworzenie kopca	

	std::make_heap(N.begin(), N.end(), CompareR()); //kopiec, w ktorym minimalizujemy R
	while (!G.empty() || !N.empty())
	{
		//odswiez zbior zadan gotowych
		while (!N.empty() && N[0].R <= t)
		{
			e = N[0];
			pe = N[0].P;
			G.push_back(e);
			push_heap(G.begin(), G.end(), CompareQ());
			pop_heap(N.begin(), N.end(), CompareR());
			N.pop_back();
		}
		//wyznacz t_next
		{
			if (!N.empty())
				t_next = N[0].R;
			else
				t_next = INT_MAX;
		}
		//jesli nie ma zadan gotowych, to przesuwamy sie do momentu gdy bedzie gotowe zadanie do realizacji
		if (G.empty())
		{
			t = t_next;
		}
		//jezeli mamy jakies zadania do wykonania. Tutaj bedzie roznica w algorytmie
		else
		{
			//e to nastepne zadanie do wykonania. w pseudokodzie to "j".
			e = G[0];
			//pe to czas wykonania nastepnego zadania
			pe = G[0].P;
			//wykonujemy zadanie j w czesci
			if (t_next < t + pe)
			{
				//aktualizacja czasu zadania
				pe = pe - (t_next - t);
				e.P = pe;
				pop_heap(G.begin(), G.end(), CompareQ());
				G.pop_back();
				G.push_back(e);
				push_heap(G.begin(), G.end(), CompareQ());
				//przechodzimy do nastepnej chwili czasowej
				t = t_next;
			}
			//wykonujemy cale zadanie j
			else
			{
				//usuwamy zadanie z kolejki gotowych do wykonania
				pop_heap(G.begin(), G.end(), CompareQ());
				G.pop_back();
				//przesuwamy sie do opdowiedniej chwili czasowej

				//aktualizujemy czas wykonania zadania
				m = max(m, t) + e.P;
				c = max(m + e.Q, c);
				t = t + pe;
			}
		}
	}
	return c;
}


void Carlier_var::findAB()
{
	//znalezeienie zadania b. Zacznijmy szukac od konca uszeregowania. Jesli czas zakonczenia go na maszynie + jego q== Cmax
	int c_max = check_time();

	for (int j = n; j > 0; --j)
	{
		if (check_time_stop_working(j) + lista_zadan[j - 1].Q == c_max)
		{
			b = j - 1;
			break;
		}
	}

	//znalezienie zadania a. Zacznijmy szukac go od poczatku
	for (int j = 0; j < n; ++j)
	{
		int temp_1 = 0;

		for (int s = j; s <= b; ++s)
		{
			temp_1 += lista_zadan[s].P;
		}

		int temp_final = lista_zadan[j].R + temp_1 + lista_zadan[b].Q;

		if (temp_final == c_max)
		{
			a = j;
			break;
		}

	}
}

bool Carlier_var::findC() 
{
	//cout << "BQ: " << lista_zadan[b].Q;
	//znalezienie zadania c
	for (int j = b; j >= a; --j)
	{
		if (lista_zadan[j].Q < lista_zadan[b].Q)
		{
			//cout << "Dla a: i b: " << a<< " " << b  << endl;
			c = j;
			//cout << " CQ: " << lista_zadan[j].Q << endl;
			return true;
			
		}
	}

	return false;
}

int Carlier_var::rMinK()
{
	return rMin(c + 1, b); // zaczynamy od zadania c+1, konczymy na ostatnim
}

int Carlier_var::qMinK()
{
	return qMin(c+1, b);
}

int Carlier_var::sPK()
{
	return sP(c + 1, b);
}

int Carlier_var::fastLB()
{
	sLB = max(h(c + 1, b), h(c, b));
	return sLB;
}


int Carlier_var::rMin(int a_t, int b_t)
{
	int r_min = INT_MAX;

	for (int i = a_t; i <= b_t; ++i)
	{
		if (lista_zadan.at(i).R < r_min)
			r_min = lista_zadan.at(i).R;
	}

	return r_min;
}


int Carlier_var::qMin(int a_t, int b_t)
{
	int q_min = INT_MAX;

	for (int i = a_t; i <= b_t; ++i)
	{
		if (lista_zadan.at(i).Q < q_min)
			q_min = lista_zadan.at(i).Q;
	}
	return q_min;
}

int Carlier_var::sP(int a_t, int b_t)
{
	int suma = 0;

	for (int i = a_t; i <= b_t; ++i)
	{
		suma += lista_zadan.at(i).P;
	}

	return suma;
}

int Carlier_var::h(int a_t, int b_t)
{
	return rMin(a_t, b_t) + sP(a_t, b_t) + qMin(a_t, b_t);
}


void Carlier_var::eliminacja(int UB)
{
	int h_K = h(c + 1, b); // ===> h dla bloku k
	for (int i = 0; i < (int)lista_zadan.size(); ++i) //przegladamy cala liste zadan
	{
		if ((i < a) || (i > b)) //jesli nie jestesmy teraz w bloku krytycznym
		{
			if (lista_zadan[i].P > UB - h_K)
			{
				if (lista_zadan.at(i).R + lista_zadan.at(i).P + sPK() + lista_zadan.at(b).Q >= UB)
				{
					lista_zadan.at(i).R = max(lista_zadan.at(i).R, rMinK() + sPK());
				}
				else if (rMinK() + lista_zadan.at(i).P + sPK() + lista_zadan.at(i).Q >= UB)
				{
					lista_zadan[i].Q = max(lista_zadan[i].Q, qMinK() + sPK());
				}
			}
		}
	}
}


Carlier_alg::Carlier_alg() : opt(false)
{

}


void Carlier_alg::load_data(string plik)
{
	ifstream data(plik);
	data >> n;
	perm_start.n = n;
	for (int i = 0; i < n; i++) {
		zadanie temp;
		data >> temp.R >> temp.P >> temp.Q; 
		temp.id = i+1;
		perm_start.lista_zadan.push_back(temp);
	}
	data.close();
}

int Carlier_alg::do_carlier_full()
{

	perm_start.sLB = 1;
	//popchniecie na stos
	UB = perm_start.schrage_normal();
	LB = perm_start.schrage_inter();
	
	heap.push_back(perm_start);
	push_heap(heap.begin(), heap.end(), CompareLB());

	optimum = perm_start;

	std::make_heap(heap.begin(), heap.end(), CompareLB());

	while (!heap.empty())
	{

		++count;
		acctual = heap.front();
		pop_heap(heap.begin(), heap.end(), CompareLB());
		heap.pop_back();

		LB = (acctual.schrage_inter());
		
		if (LB < UB)
		{

			acctual.schrage_normal();
			if (acctual.CMax < UB)
			{
				cout << "UB =: " << acctual.CMax<< endl;
				UB = acctual.CMax;
				optimum = acctual;
			}

			acctual.findAB();
			if (acctual.findC())
			{
				//cout << "a: " << acctual.a << endl;
				//cout << "b: " << acctual.b << endl;
				//cout << "c: " << acctual.c<< endl;
				acctual.eliminacja(UB);

				left = right = acctual;

				left.lista_zadan[left.c].R = left.rMinK() + left.sPK();
				right.lista_zadan[right.c].Q = right.qMinK() + left.sPK();

				left.fastLB();
				right.fastLB();

				left.sLB = max(left.sLB, LB);
				right.sLB = max(right.sLB, LB);

				left.id = id++;
				right.id = id++;

				//wrzucenie potomkow na kopiec
				if (left.sLB < UB)
				{
					heap.push_back(left);
					push_heap(heap.begin(), heap.end(), CompareLB());
				}
				if (right.sLB < UB)
				{
					heap.push_back(right);
					push_heap(heap.begin(), heap.end(), CompareLB());
				}
			}
			else
			{
				opt = true;
				cout << "Znaleziono lokalnie optimum: " << acctual.CMax << endl;
			}
		}
	}
	cout << "Cmax: " << optimum.CMax << endl;
	return optimum.CMax;
	
}


int Carlier_alg::do_carlier_notests()
{

	perm_start.sLB = 1;
	//popchniecie na stos
	UB = perm_start.schrage_normal();
	LB = perm_start.schrage_inter();

	heap.push_back(perm_start);
	push_heap(heap.begin(), heap.end(), CompareLB());

	optimum = perm_start;

	std::make_heap(heap.begin(), heap.end(), CompareLB());

	while (!heap.empty())
	{

		++count;
		acctual = heap.front();
		pop_heap(heap.begin(), heap.end(), CompareLB());
		heap.pop_back();

		LB = (acctual.schrage_inter());

		if (LB < UB)
		{

			acctual.schrage_normal();
			if (acctual.CMax < UB)
			{
				cout << "UB =: " << acctual.CMax << endl;
				UB = acctual.CMax;
				optimum = acctual;
			}

			acctual.findAB();
			if (acctual.findC())
			{
				//cout << "a: " << acctual.a << endl;
				//cout << "b: " << acctual.b << endl;
				//cout << "c: " << acctual.c<< endl;
				//acctual.eliminacja(UB);

				left = right = acctual;

				left.lista_zadan[left.c].R = left.rMinK() + left.sPK();
				right.lista_zadan[right.c].Q = right.qMinK() + left.sPK();

				left.fastLB();
				right.fastLB();

				left.sLB = max(left.sLB, LB);
				right.sLB = max(right.sLB, LB);

				left.id = id++;
				right.id = id++;

				//wrzucenie potomkow na kopiec
				if (left.sLB < UB)
				{
					heap.push_back(left);
					push_heap(heap.begin(), heap.end(), CompareLB());
				}
				if (right.sLB < UB)
				{
					heap.push_back(right);
					push_heap(heap.begin(), heap.end(), CompareLB());
				}
			}
			else
			{
				opt = true;
				cout << "Znaleziono lokalnie optimum: " << acctual.CMax << endl;
			}
		}
	}
	cout << "Cmax: " << optimum.CMax << endl;
	return optimum.CMax;

}

int Carlier_alg::do_carlier_nostrat()
{

	perm_start.sLB = 1;
	//popchniecie na stos
	UB = perm_start.schrage_normal();
	LB = perm_start.schrage_inter();

	heap.push_back(perm_start);
	//push_heap(heap.begin(), heap.end(), CompareLB());

	optimum = perm_start;

	//std::make_heap(heap.begin(), heap.end(), CompareLB());

	while (!heap.empty())
	{

		++count;
		//acctual = heap.front();
		//rozwazamy zawsze ostatnio wlozony element
		acctual = heap.back();
		//pop_heap(heap.begin(), heap.end(), CompareLB());
		heap.pop_back();

		LB = (acctual.schrage_inter());

		if (LB < UB)
		{

			acctual.schrage_normal();
			if (acctual.CMax < UB)
			{
				cout << "UB =: " << acctual.CMax << endl;
				UB = acctual.CMax;
				optimum = acctual;
			}

			acctual.findAB();
			if (acctual.findC())
			{
				//cout << "a: " << acctual.a << endl;
				//cout << "b: " << acctual.b << endl;
				//cout << "c: " << acctual.c<< endl;
				acctual.eliminacja(UB);

				left = right = acctual;

				left.lista_zadan[left.c].R = left.rMinK() + left.sPK();
				right.lista_zadan[right.c].Q = right.qMinK() + left.sPK();

				left.fastLB();
				right.fastLB();

				left.sLB = max(left.sLB, LB);
				right.sLB = max(right.sLB, LB);

				left.id = id++;
				right.id = id++;

				//wrzucenie potomkow na kopiec
				if (left.sLB < UB)
				{
					heap.push_back(left);
					//push_heap(heap.begin(), heap.end(), CompareLB());
				}
				if (right.sLB < UB)
				{
					heap.push_back(right);
					//push_heap(heap.begin(), heap.end(), CompareLB());
				}
			}
			else
			{
				opt = true;
				cout << "Znaleziono lokalnie optimum: " << acctual.CMax << endl;
			}
		}
	}
	cout << "Cmax: " << optimum.CMax << endl;
	return optimum.CMax;

}


