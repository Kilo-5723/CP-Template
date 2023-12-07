#include "bits/stdc++.h"
using namespace std;
const char el = '\n';
typedef long long ll;
const ll mod1 = 1e9 + 7, mod2 = 1e9 + 9;
struct graph {
  vector<vector<pair<int, int>>> e;
  graph(int n) : e(n + 1) {}
  void adde(int u, int v, int w) { e[u].push_back({v, w}); }
  bool check() {
    vector<int> deg(e.size());
    for (auto &ed : e)
      for (auto &[to, wt] : ed) deg[to]++;
    for (int i = 1; i < e.size(); i++)
      if (deg[i] != e[i].size()) return false;
    return true;
  }
  void euler(int u, vector<int> &res) {
    while (e[u].size()) {
      auto [v, w] = e[u].back();
      e[u].pop_back();
      euler(v, res);
      res.push_back(w);
    }
  }
};
struct hsh {
  static const int k = 3;
  array<ll, k> a;
};
const hsh zero{0, 0, 0}, one{1, 1, 1}, mul{31, 57, 71},
    mod{998244353, 1e9 + 7, 1e9 + 9};

const int mul1 = 31, mul2 = 157;
pair<ll, ll> pmul(int n) {
  static vector<pair<ll, ll>> res = {{1, 1}};
  while (res.size() <= n) {
    auto [p, q] = res.back();
    res.push_back({p * mul1 % mod1, q * mul2 % mod2});
  }
  return res[n];
}
struct hshstr {
  vector<pair<ll, ll>> hsh;
  hshstr(string s = "") {
    hsh.push_back({0, 0});
    for (auto c : s) {
      auto [p, q] = hsh.back();
      hsh.push_back({(p * mul1 + c) % mod1, (q * mul2 + c) % mod2});
    }
  }
  pair<ll, ll> hash(int l, int r) {
    auto [p1, q1] = hsh[l];
    auto [p2, q2] = hsh[r];
    auto [mp, mq] = pmul(r - l);
    return {(p1 * mp - p2 + mod1) % mod1, (q1 * mq - q2 + mod2) % mod2};
  }
};
optional<pair<vector<int>, vector<int>>> solve(vector<hshstr> &a,
                                               vector<hshstr> &b, int n,
                                               int m) {
  map<pair<ll, ll>, int> id1, id2;
  int k = 0;
  for (auto &s : a) {
    pair<ll, ll> l = s.hash(0, m), r = s.hash(m, n);
    if (!id1.count(l)) id1[l] = ++k;
    if (!id2.count(r)) id2[r] = ++k;
  }
  for (auto &s : b) {
    pair<ll, ll> l = s.hash(0, n - m), r = s.hash(n - m, n);
    if (!id2.count(l)) return nullopt;
    if (!id1.count(r)) return nullopt;
  }
  graph g(k);
  int t = 0;
  for (auto &s : a) {
    pair<ll, ll> l = s.hash(0, m), r = s.hash(m, n);
    g.adde(id1[l], id2[r], ++t);
  }
  t = 0;
  for (auto &s : b) {
    pair<ll, ll> l = s.hash(0, n - m), r = s.hash(n - m, n);
    g.adde(id2[l], id1[r], ++t);
  }
  vector<int> res;
  if (!g.check()) return nullopt;
  g.euler(1, res);
  if (res.size() < 2 * a.size()) return nullopt;
  reverse(res.begin(), res.end());
  vector<int> p, q;
  for (int i = 0; i < res.size(); i += 2) p.push_back(res[i]);
  for (int i = 1; i < res.size(); i += 2) q.push_back(res[i]);
  return pair<vector<int>, vector<int>>{p, q};
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  int tt;
  cin >> tt;
  while (tt--) {
    int n, m;
    cin >> n >> m;
    vector<hshstr> a(n), b(n);
    for (auto &v : a) {
      string s;
      cin >> s;
      v = hshstr(s);
    }
    for (auto &v : b) {
      string s;
      cin >> s;
      v = hshstr(s);
    }
    bool flg = false;
    for (int i = 0; i < m; i++)
      if (auto res = solve(a, b, m, i)) {
        flg = true;
        for (auto v : res->first) cout << v << ' ';
        cout << el;
        for (auto v : res->second) cout << v << ' ';
        cout << el;
        break;
      }
    if (!flg) cout << -1 << el;
  }
  return 0;
}