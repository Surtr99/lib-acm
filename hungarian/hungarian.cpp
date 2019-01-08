#include <bits/stdc++.h>
#define rep(i,a,b) for (int i=a; i<b; i++)
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define mitte (lewy+prawy)/2
#define debug //
using namespace std;
//TESTED
//Najtansze skojarzenie w grafie dwudzielnym - metoda wegierska
namespace hungarian //macierzy n x n (x-om przypisuje y)
{
	#define INF 1000000000
	int n;
	class matching //tutaj chcemy sobie matchowac linie
	{
		//konw:  [0, n) - x (prawa strona) [n, 2n) - y lewa strona
		private:
		vector <vector <int> > graf; // graf z dobrymi krawedziami
		vector <int> matched;
		vector <bool> used;
		vector <int> dist;
		queue <int> kol;
		bool dfs (int a) //zawsze jestem po lewej
		{
			if (used[a]) return false;
			used[a]=true;
			
			for (int s: graf[a]) if (matched[s]==-1)
			{
				matched[a]=s;
				matched[s]=a;
				return true;
			}
			for (int s: graf[a]) if (dist[s]==dist[a]+1)
			{
				if (dfs(matched[s]))
				{
					matched[a]=s;
					matched[s]=a;
					return true;
				}
			}
			return false;
		}
		public:
		void graftest()
		{
			debug ("graftest\n");
			rep(i,0,n*2)
			{
				debug ("%d: ", i);
				rep(j,0,(int)graf[i].size()) debug ("%d ", graf[i][j]);
				debug ("\n");
			}
			debug ("\n");
		}
		void init ()
		{
			graf.clear();
			graf.resize(n*2);
			matched.clear();
			matched.resize(n*2, -1); //wszystko od nowa - ale gra zostaje
			used.reserve(n*2);
			dist.reserve(n*2);
		}
		void edge (int a, int b) //add edge from y a to x b
		{
			graf[a].pb(b+n);
			graf[b+n].pb(a);
		}
		int match () 
		{
			int v, m=0;
			bool flag=true;
			while (flag)
			{
				flag=false;
				dist.clear();
				dist.resize(n*2, INF);
				used.clear();
				used.resize(n*2, false);
				
				rep(i,0,n) if (matched[i]==-1)
				{
					dist[i]=0;
					kol.push(i);
				}
				while (!kol.empty())
				{
					v=kol.front();
					kol.pop();
					if (used[v]) continue;
					used[v]=true;
					if (v>=n) //jestem prawy
					{
						if (matched[v]>=0)
						{
							if (dist[matched[v]]>dist[v]+1)
							{
								dist[matched[v]]=dist[v]+1;
								kol.push(matched[v]);
							}
						}
						continue;
					}
					for (int s: graf[v]) if (dist[s]>dist[v]+1)
					{
						dist[s]=dist[v]+1;
						kol.push(s);
					}
				}
				used.clear();
				used.resize(n*2, false);
				rep(l,0,n) if (matched[l]==-1) if (dfs(l))
				{
					flag=true;
					m++; //bo tu chce miec tylko moc - to wystarczy
				}
			}
			return m;
		}
		bool vert_cov (vector <bool> &cov) //wypelnij go vertex coverem - RUN ONLY AFTER MATCHING
		{
			int v;
			rep(i,0,n*2) cov[i]=false;
			//na kolejke rzucam ludzi, ktorych sasiedzi musza byc indepentent
			rep(i,0,n*2) if (matched[i]==-1)
			{
				kol.push(i);
			} 
			used.clear();
			used.resize(n*2, false);
			while (!kol.empty())
			{
				v=kol.front();
				kol.pop();
				if (used[v]) continue;
				used[v]=true;
				for (int s: graf[v]) if (!cov[s])
				{
					cov[s]=true; //wrzuc go - on na pewno jest zmachowany
					if (matched[s]==-1) return false;
					kol.push(matched[s]);
				}
			}
			rep(i,0,n) if (matched[i]>=0)
			{
				if (!cov[i] && !cov[matched[i]]) cov[i]=true; //zawsze z lewej
			}
			return true; //sukces
		}
		vector <int> give_matching () //wypisz wynik
		{
			vector <int> ret;
			ret.resize(n*2);
			
			rep(i,0,n*2) 
			{
				ret[i]=matched[i]; //to jest nasze najtansze przyporzadkowanie
			}
			return ret;
		}
	};
	matching M;
	void druk (vector <vector <int> > &tab)
	{
		debug ("our tab\n");
		rep(i,0,n)
		{
			rep(j,0,n) debug ("%d ", tab[i][j]);
			debug ("\n");
		}
	}
	vector <int> main (int _n, vector <vector <int> > tab) //macierz n x n
	{
		n=_n;
		int mini;
		rep(i,0,n)
		{
			mini=INF;
			rep(j,0,n) mini=min(mini, tab[i][j]);
			rep(j,0,n) tab[i][j]-=mini;
		}
		rep(j,0,n) 
		{
			mini=INF;
			rep(i,0,n) mini=min(mini, tab[i][j]);
			rep(i,0,n) tab[i][j]-=mini;
		}
		M.init();
		rep(i,0,n) rep(j,0,n) if (tab[i][j]==0) M.edge(i,j);
		
		
		vector <bool> crossed;
		crossed.resize(2*n, false); //skreslone - kolumny, wiersze
		while (M.match()<n)
		{
			M.vert_cov(crossed);
			mini=INF;
			rep(i,0,n) if (!crossed[i]) rep(j,0,n) if (!crossed[n+j]) mini=min(mini, tab[i][j]);
			
			//odejmij od wszystkich wierszy niezaznaczonych
			rep(i,0,n) if (!crossed[i]) rep(j,0,n) 
			{
				tab[i][j]-=mini;
			}
			rep(j,0,n) if (crossed[j+n]) rep(i,0,n) tab[i][j]+=mini;
			
			M.init();
			rep(i,0,n) rep(j,0,n) if (tab[i][j]==0) M.edge(i,j);
			
		}
		
		return M.give_matching(); //najtansze maksymalne skojarzenie
	}
}
vector <vector <int> > tab; //to tam hungarianowi
int main ()
{
	int n;
	scanf ("%d", &n); //tyle jest po jednej stronie
	tab.resize(n);
	rep(i,0,n) 
	{
		tab[i].resize(n);	
		rep(j,0,n) scanf ("%d", &tab[i][j]);
	}
	vector <int> res=hungarian::main(n, tab);
	int s=0;
	rep(i,0,n) s+=tab[i][res[i]-n];
	printf ("%d\n", s); //tyle kosztuje najtansze skojarzenie 
}
