by#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <tuple>

const int ErrorState = -1;
const char Lambda = '.';
const char EmptyChar = 0;

#define DeltaType map<pair<pair<int, char>, char>, pair<int, string>>
// se mapeaza ((stare_curenta, simbol_curent_stiva), caracter) la (stare_noua, simboluri_de_pus_pe_stiva)

using namespace std;

class DPDA
{
	set<int> m_Q, m_F;
	set<char> m_Sigma, m_Gamma;
	int m_q0;
	char m_Z;
	DeltaType m_delta;
	stack<char> Stack;

public:
	DPDA()
	{
		m_q0 = 0;
		m_Z = 'Z';
		Stack.push(m_Z);
	}

	DPDA(set<int> Q, set<char> Sigma, int q0, set<int> F, set<char> Gamma, char Z, DeltaType delta)
	{
		m_Q = Q;
		m_Sigma = Sigma;
		m_q0 = q0;
		m_F = F;
		m_Gamma = Gamma;
		m_Z = Z;
		m_delta = delta;
		Stack.push(Z);
	}

	set<int> getQ() const { return m_Q; }
	set<char> getSigma() const { return m_Sigma; }
	int getQ0() const { return m_q0; }
	set<int> getF() const { return m_F; }
	set<char> getGamma() const { return m_Gamma; }
	char getZ() const { return m_Z; }
	DeltaType getDelta() const { return m_delta; }

	bool isFinalStateAndIsEmptyStack(int);
	int deltaStar(int, string);
};

bool DPDA::isFinalStateAndIsEmptyStack(int q)
{
	return (m_F.find(q) != m_F.end()) && Stack.top() == m_Z;
}

int DPDA::deltaStar(int q, string w)
{
	if (Stack.size() == 0)
		return ErrorState;

	if (w.length() == 0)
	{
		pair<int, string> transition = m_delta[{ {q, Stack.top()}, Lambda}];
		Stack.pop();
		for (char symbol : transition.second)
			Stack.push(symbol);
		return transition.first;
	}

	if (w.length() == 1)
	{
		pair<int, string> transition = m_delta[{ {q, Stack.top()}, (char)w[0]}];
		Stack.pop();
		for (char symbol : transition.second)
			Stack.push(symbol);
		return transition.first;
	}

	pair<int, string> transition = m_delta[{ {q, Stack.top()}, (char)w[0]}];
	Stack.pop();
	for (char symbol : transition.second)
		Stack.push(symbol);
	return deltaStar(transition.first, w.substr(1, w.length() - 1));
}

DPDA configureDPDA_1()
{
	set<int> Q = { 0, 1, 2 };
	set<char> Sigma = { 'a', 'b' };
	int q0 = 0;
	set<int> F = { 0, 2 };
	set<char> Gamma = { 'A' };
	char Z = 'Z';
	DeltaType delta;

	// initial, tranzitiile ajung in starea ErrorState (inclusiv tranzitiile vide sau cu lambda)
	// punem astfel pentru ca, altfel, implicit aceste tranzitii ajung in starea 0
	// iar daca starea 0 este finala, va accepta cuvinte care ar trebui sa fie respinse atunci cand nu va mai gasi tranzitii
	for (auto q : Q)
		for (auto s : Gamma)
			for (auto ch : Sigma)
			{
				delta[{ {q, s}, ch}] = { ErrorState, "" };
				delta[{ {q, s}, EmptyChar}] = { ErrorState, "" };
				delta[{ {q, s}, Lambda}] = { ErrorState, "" };
			}

	delta[{ {0, 'Z'}, 'a'}] = { 0, "ZA" };
	delta[{ {0, 'A'}, 'a'}] = { 0, "AA" };
	delta[{ {0, 'Z'}, Lambda}] = { 0, "Z" };
	delta[{ {0, 'A'}, 'b'}] = { 1, "" };
	delta[{ {1, 'A'}, 'b'}] = { 1, "" };
	delta[{ {1, 'Z'}, Lambda}] = { 2, "Z" };

	DPDA M(Q, Sigma, q0, F, Gamma, Z, delta);
	return M;
}

DPDA configureDPDA_2()
{
	set<int> Q = { 0, 1, 2, 3, 4 };
	set<char> Sigma = { 'a', 'b', 'c', 'd' };
	int q0 = 0;
	set<int> F = { 4 };
	set<char> Gamma = { 'A', 'B' };
	char Z = 'Z';
	DeltaType delta;

	// initial, tranzitiile ajung in starea ErrorState (inclusiv tranzitiile vide sau cu lambda)
	// punem astfel pentru ca, altfel, implicit aceste tranzitii ajung in starea 0
	// iar daca starea 0 este finala, va accepta cuvinte care ar trebui sa fie respinse atunci cand nu va mai gasi tranzitii
	for (auto q : Q)
		for (auto s : Gamma)
			for (auto ch : Sigma)
			{
				delta[{ {q, s}, ch}] = { ErrorState, "" };
				delta[{ {q, s}, EmptyChar}] = { ErrorState, "" };
				delta[{ {q, s}, Lambda}] = { ErrorState, "" };
			}

	delta[{ {0, 'Z'}, 'a'}] = { 0, "ZA" };
	delta[{ {0, 'A'}, 'a'}] = { 0, "AA" };
	delta[{ {0, 'A'}, 'b'}] = { 1, "AB" };

	delta[{ {1, 'B'}, 'b'}] = { 1, "BB" };
	delta[{ {1, 'B'}, 'c'}] = { 2, "" };

	delta[{ {2, 'B'}, 'c'}] = { 2, "" };
	delta[{ {2, 'A'}, 'd'}] = { 3, "" };

	delta[{ {3, 'A'}, 'd'}] = { 3, "" };
	delta[{ {3, 'Z'}, Lambda}] = { 4, "Z" };


	DPDA M(Q, Sigma, q0, F, Gamma, Z, delta);
	return M;
}

int main()
{
	DPDA M = configureDPDA_2();
	string word = "aabbbbccccdd.";

	if (M.isFinalStateAndIsEmptyStack(M.deltaStar(M.getQ0(), word)))
	{
		cout << "Cuvant acceptat";
	}
	else
	{
		cout << "Cuvant respins";
	}

	return 0;
}