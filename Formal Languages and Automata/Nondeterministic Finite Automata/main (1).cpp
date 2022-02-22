#include <fstream>
#include <map>
#include <vector>
#include <queue>
using namespace std;
ifstream fin("nfa.in");
ofstream fout("nfa.out");


vector<pair<int, char> > adj[10000];
int N, M;
int S;    ///stare initiala
vector <int> f;   ///stari finale
int nrF, NrCuv;
vector <pair<int, int>> dr;  ///(stare, lungime cuv)


bool NFA(string cuv)
{
    pair <int, int> x;
    queue <pair<int, int>> C;
    C.push(make_pair(S, 0));
    dr.clear();

    while (C.size() > 0)   ///BFS
    {
        x = C.front();   ///preluam primul elem din coada
        C.pop();
        dr.push_back(make_pair(x.first, x.second));    //adaugam nodul si momentul la care a fost parcurs in vectorul in care avem TOT drumul parcurs cu BFS ul
        if (x.second == cuv.size())
        {
            for (int i = 0; i < nrF; i++)     ///cautam stare finala
                if (x.first == f[i]) return 1;
        }
        for (auto j = adj[x.first].begin(); j != adj[x.first].end(); j++)    ///cautam urmatorul nod de bagat in coada
            //in variabila j o sa avem o pereche de forma (y, l) care este vecina cu x
            //in j->first avem y in j->second o sa avem l
            if (cuv[x.second] == j->second)    ///daca litera curenta e acceptata
                C.push(make_pair(j->first, x.second + 1));    ///o adaugam in coada
    }
    return 0;
}

void Lant(string cuv)
{
    int n = dr[dr.size() - 1].first;
    int p = dr[dr.size() - 1].second;
    vector <int> fi;   ///vector drum final este in ordine inversa
    fi.push_back(n);

    for (auto j = dr.end() - 1; j != dr.begin(); j--)   ///parcurgem invers vectorul dr
    //vectorul dr e format din perechid de forma  (nod, mom) si stim ca la momentul "mom" BFS-ul a trecut prin nodul "nod"
    //elementele se acceseaza cu j->first care "adreseaza" nodului nod si cu j->second care "adreseaza" momentul mom la care a fost parcurs
        if (j->second == p - 1)     ///daca pozitiile sunt convenabile   (sa corespunda pozitia literei in cuvant cu momentul in care a fost gasita litera cu BFS-ul)
            for (auto k = adj[j->first].begin(); k != adj[j->first].end(); k++)    ///cautam "tatal"
                if (k->first == n && cuv[p - 1] == k->second)   ///avem muchie+tranzitie
                {
                    n = j->first;      ///trecem la tatal nodului curent
                    p--;
                    fi.push_back(n);    ///il adaugam in vectorul final
                    break;
                }
    for (int i = fi.size() - 1; i >= 0; i--) fout << fi[i] << " ";    ///afisam de la cap la coada
}

void Citeste()
{
    int x, y; char l;
    string cuv;
    fin >> N;
    fin >> M;
    for (int i = 0; i < M; ++i)
    {
        fin >> x >> y >> l;
        adj[x].push_back(make_pair(y, l));    ///cream lista de adiac cu ponderi
    }
    fin >> S;            //stare initiala
    fin >> nrF;          //nr de stari finale
    for (int i = 0; i < nrF; ++i) { fin >> x; f.push_back(x); }     //stari finale
    fin >> NrCuv;        //numar de cuvinte
    for (int i = 0; i < NrCuv; ++i)
    {
        fin >> cuv;      //cuvinte
        if (NFA(cuv))
        {
            fout << "DA\n" << S <<" ";
            Lant(cuv);
            fout << "\n";
        }
        else fout << "NU\n";
    }
}

int main()
{
    Citeste();
    return 0;
}
