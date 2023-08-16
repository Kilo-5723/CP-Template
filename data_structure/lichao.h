#include <optional>

#include "bits/stdc++.h"
using namespace std;
const char el = '\n';
typedef long long ll;

const ll inf = 1e18;
struct node {
  node *ls, *rs;
  int pl, pm, pr;
  int xl, xm, xr;
  ll k, b;
  int tag;
  node(int l, int r) {
    pl = l, pr = r, pm = l + (r - l >> 1);
    xl = xm = xr = 0;
    k = 0, b = 0;
    tag = 0;
    if (l == r) {
      ls = rs = NULL;
      return;
    }
    ls = new node(l, pm);
    rs = new node(pm + 1, r);
  }
  void addtag(int t) {
    tag += t;
    b -= k * t;
    xl += t, xm += t, xr += t;
  }
  void pushtag() {
    if (ls) ls->addtag(tag);
    if (rs) rs->addtag(tag);
    tag = 0;
  }
  void addfunc(int l, int r, ll _k, ll _b) {
    if (r < pl || l > pr) return;
    pushtag();
    if (l <= pl && r >= pr) {
      if (k * xm + b < _k * xm + _b) {
        swap(k, _k);
        swap(b, _b);
      }
      if (k * xl + b < _k * xl + _b && ls) ls->addfunc(l, r, _k, _b);
      if (k * xr + b < _k * xr + _b && rs) rs->addfunc(l, r, _k, _b);
      return;
    }
    if (ls) ls->addfunc(l, r, _k, _b);
    if (rs) rs->addfunc(l, r, _k, _b);
  }
  void addx(int p, int v) {
    if (pr < p) return;
    if (pl >= p) {
      addtag(v);
      return;
    }
    if (pl >= p) xl += v;
    if (pm >= p) xm += v;
    if (pr >= p) xr += v;
    if (ls) ls->addfunc(pl, pr, k, b);
    if (rs) rs->addfunc(pl, pr, k, b);
    pushtag();
    if (ls) ls->addx(p, v);
    if (rs) rs->addx(p, v);
    k = 0, b = 0;
  }
  optional<pair<int, ll>> query(int p) {
    if (p < pl || p > pr) return nullopt;
    if (pl == pr) return pair<int, ll>{xm, k * xm + b};
    pushtag();
    auto res = ls->query(p);
    if (!res) res = rs->query(p);
    auto [x, y] = *res;
    return pair<int, ll>{x, max(y, k * x + b)};
  }
};
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  int tt;
  cin >> tt;
  while (tt--) {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> a(m + 1);
    vector<int> c(m + 1);
    while (n--) {
      int l, r;
      cin >> l >> r;
      a[l].push_back(r);
      c[r]++;
    }
    vector<ll> p(m + 1);
    for (int i = 1; i <= m; i++) cin >> p[i];
    node *rt = new node(0, m + 1);
    for (int i = 1; i <= m; i++) {
      auto [x, y] = *rt->query(i);
      rt->addx(i, -c[i - 1]);
      sort(a[i].begin(), a[i].end());
      for (auto j : a[i]) rt->addx(j + 1, 1);
      rt->addfunc(0, m + 1, p[i], y);
    }
    cout << rt->query(m + 1)->second << el;
  }
  return 0;
}
// https://codeforces.com/contest/1830/problem/F