#include <cstdio>
#include <string>
#include <map>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

#define TRACE(x) cerr << #x << " = " << x << endl
#define REP(i, n) for (int i=0; i<n; i++)
#define FOR(i, a, b) for (int i=(a); i<(b); i++)

typedef long long ll;
typedef pair<int, int> P;
#define X first
#define Y second

vector <pair<string, string> > V; //name, jmbag
vector <string> Bitni;

string daj_rje(char *(&p))
{
  string ret = "";
  for (; *p && isspace(*p); p++);
  for (; *p && !isspace(*p); p++)
    ret.push_back(*p);

  return ret;
}

int is_slovo(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int is_broj(char c)
{
  return c >= '0' && c <= '9';
}

int is_name(string s)
{
  int sl = 0, sz = (int) s.size();

  REP(i, sz)
    sl += is_slovo(s[i]);

  return sl > sz - sl;
}

int is_jmbag(string s)
{
  int br = 0, sz = (int) s.size();
  REP(i, sz)
    br += is_broj(s[i]);

  return br > sz - br && sz > 7;
}

int rek_edit_dist(int dp[30][30], const string &a, const string &b, int asz, int bsz, int poza, int pozb)
{
  if (pozb == (int) b.size()) return asz - poza;
  if (dp[poza][pozb] != -1) return dp[poza][pozb];

  int ret = 1000;
  if (poza < asz) ret = min(ret, rek_edit_dist(dp, a, b, asz, bsz, poza + 1, pozb) + 1);
  if (pozb < bsz) ret = min(ret, rek_edit_dist(dp, a, b, asz, bsz, poza, pozb + 1) + 1);
  if (poza < asz && pozb < bsz && a[poza] == b[pozb])
    ret = min(ret, rek_edit_dist(dp, a, b, asz, bsz, poza + 1, pozb + 1));

  return dp[poza][pozb] = ret;
}

int edit_dist(const string &a, const string &b)
{
  assert(a.size() < 30 && b.size() < 30);
  int dp[30][30];
  memset(dp, -1, sizeof dp);

  return rek_edit_dist(dp, a, b, (int) a.size(), (int) b.size(), 0, 0);
}

int isti(const string &a, const string &b)
{
  double koef = .2;
  if (is_jmbag(a)) koef = 0;
  int tmp = edit_dist(a, b);
  return tmp <= ((int) a.size() + (int) b.size()) * koef;
}

void load_base()
{
  FILE *baza = fopen("baza.txt", "r");
  assert(baza);
  char red[2000];

  for (; ~fscanf(baza, "%[^\n]", red); ) {
    string rijec = "";
    string name = "";
    string num = "";
    char *p = red;

    for (; *p; ) {
      rijec = daj_rje(p);
      if (is_name(rijec)) {
        if (!name.empty()) name += " ";
        name += rijec;
      }
      if (is_jmbag(rijec))
        num = rijec;
    }

    REP(i, (int) Bitni.size())
      if (isti(Bitni[i], name)) {
        V.push_back({name, num});
        printf("PUSH %s %s\n", name.c_str(), num.c_str());
      }

    fscanf(baza, " ");
  }
}

void load_bitni()
{
  FILE *bitni = fopen("bitni.txt", "r");
  char s[100];
  assert(bitni);

  for (; ~fscanf(bitni, "%[^\n]", s); ) {
    Bitni.push_back((string) s);
    fscanf(bitni, " ");
    TRACE(s);
  }
}

string jel_bitan_jmbag(string s)
{
  REP(i, (int) V.size())
    if (isti(V[i].Y, s)) return V[i].X;

  return "";
}

void load_rez(char *pp)
{
  vector <pair<string, string> > R;
  
  FILE *f = fopen(pp, "r");
  char s[100];
  assert(f);

  string st = "";
  string rez = "";
  for (; ~fscanf(f, "%s", s); ) {
    string tmp = s;
    if (is_jmbag(tmp)) {
      string ime = jel_bitan_jmbag(st);
      if (!ime.empty())
	R.push_back({ime, rez});

      st = tmp;
      rez = "";
    }
    else
      rez += " " + tmp;

    fscanf(f, " ");
  }

  string ime = jel_bitan_jmbag(st);
  if (!ime.empty())
    R.push_back({ime, rez});

  for (auto it : Bitni)
    for (auto it2 : R)
      if (isti(it2.X, it))
	printf("%s -> %s\n", it2.X.c_str(), it2.Y.c_str());          
}

int main(int argc, char *argv[])
{
  assert(argc == 2);

  load_bitni();
  load_base();
  load_rez(argv[1]);

  return 0;
}

