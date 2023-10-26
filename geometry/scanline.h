#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
typedef __int128 li;
const int inf = 3e6;
struct frac {
  li a, b;
  frac(li _a = 0, li _b = 1) : a(_a), b(_b) {
    if (b < 0) a = -a, b = -b;
  }
};
// note that without gcd plus operation also enlarges the fractor.
frac operator+(frac a, frac b) { return {a.a * b.b + b.a * a.b, a.b * b.b}; }
frac operator-(frac a, frac b) { return {a.a * b.b - b.a * a.b, a.b * b.b}; }
frac operator*(frac a, frac b) { return {a.a * b.a, a.b * b.b}; }
frac operator/(frac a, frac b) { return {a.a * b.b, a.b * b.a}; }
bool operator<(frac a, frac b) { return a.a * b.b < b.a * a.b; }
bool operator>(frac a, frac b) { return b < a; }
bool operator>=(frac a, frac b) { return !(a < b); }
bool operator<=(frac a, frac b) { return !(b < a); }
bool operator!=(frac a, frac b) { return a < b || b < a; }
bool operator==(frac a, frac b) { return !(a != b); }
frac abs(frac a) { return {a.a < 0 ? -a.a : a.a, a.b}; }
struct cplx {
  frac x, y;
};
cplx operator+(const cplx &a, const cplx &b) { return {a.x + b.x, a.y + b.y}; }
cplx operator-(const cplx &a, const cplx &b) { return {a.x - b.x, a.y - b.y}; }
bool operator<(const cplx &a, const cplx &b) {
  return a.x == b.x ? a.y < b.y : a.x < b.x;
}
bool operator!=(const cplx &a, const cplx &b) { return a < b || b < a; }
bool operator==(const cplx &a, const cplx &b) { return !(a != b); }
struct sgmt {
  cplx a, b;
  sgmt(cplx _a, cplx _b) : a(_a), b(_b) {
    if (b < a) swap(a, b);
  }
  frac cut(frac x) const {
    if (x == a.x) return a.y;
    if (x == b.x) return b.y;
    auto t1 = (b.x - x) * a.y, t2 = (x - a.x) * b.y;
    t1.a += t2.a;
    auto t3 = b.x - a.x;
    auto res = t1 / t3;
    return res;
  }
};
bool operator<(const sgmt &a, const sgmt &b) {
  if (a.a != b.a) return a.a < b.a;
  return a.b < b.b;
}
map<sgmt, vector<cplx>> solve(vector<sgmt> line, vector<cplx> vtx) {
  sort(line.begin(), line.end(), [&](sgmt a, sgmt b) { return a.a < b.a; });
  auto cmp1 = [&](sgmt a, sgmt b) {
    if (a.b != b.b) return a.b < b.b;
    return a.a < b.a;
  };
  set<sgmt, decltype(cmp1)> curl(cmp1);
  frac cx;
  auto cmp2 = [&](sgmt a, sgmt b) {
    frac ay = a.cut(cx), by = b.cut(cx);
    if (ay == by) return a.cut(cx + 1) < b.cut(cx + 1);
    return ay < by;
  };
  set<sgmt, decltype(cmp2)> scan(cmp2);
  auto findc = [&](cplx v) {
    auto [x, y] = v;
    return *scan.lower_bound({{x, y}, {x + 1, y - inf}});
  };
  auto cmp3 = [&](sgmt a, sgmt b) { return a.a.y < b.a.y; };
  set<sgmt, decltype(cmp3)> hori(cmp3);
  auto findh = [&](cplx v) -> optional<sgmt> {
    auto [x, y] = v;
    auto it = hori.upper_bound({{x, y}, {x, y}});
    if (it == hori.begin()) return nullopt;
    it = prev(it);
    if (it->b.y < y) return nullopt;
    return *it;
  };
  reverse(line.begin(), line.end());
  reverse(vtx.begin(), vtx.end());
  map<sgmt, vector<cplx>> res;
  while (vtx.size()) {
    hori.clear();
    frac x = vtx.back().x;
    vector<cplx> curv;
    while (vtx.size() && vtx.back().x <= x)
      curv.push_back(vtx.back()), vtx.pop_back();
    while (curl.size() && curl.begin()->b.x <= x)
      scan.erase(*curl.begin()), curl.erase(curl.begin());
    cx = x;
    while (line.size() && line.back().a.x <= x) {
      auto l = line.back();
      line.pop_back();
      if (l.b.x < x) continue;
      if (l.a.x == l.b.x)
        hori.insert(l);
      else
        scan.insert(l), curl.insert(l);
    }
    for (auto v : curv)
      if (auto t = findh(v))
        res[*t].push_back(v);
      else
        res[findc(v)].push_back(v);
  }
  return res;
}
const int k = 20;
struct graph {
  vector<vector<int>> e;
  graph(int n) : e(n + 1) {}
  void adde(int u, int v) {
    e[u].push_back(v);
    e[v].push_back(u);
  }
  vector<array<int, k>> f;
  vector<int> d;
  void makef(int u, int w) {
    f[u][0] = w;
    for (int i = 1; i < k; i++) f[u][i] = f[f[u][i - 1]][i - 1];
    d[u] = d[w] + 1;
    for (auto &v : e[u])
      if (v == w) {
        v = e[u].back(), e[u].pop_back();
        break;
      }
    for (auto v : e[u]) makef(v, u);
  }
  int lca(int u, int v) {
    if (d[u] < d[v]) swap(u, v);
    for (int i = 0; i < k; i++)
      if (d[u] - d[v] >> i & 1) u = f[u][i];
    if (u == v) return u;
    for (int i = k - 1; i >= 0; i--)
      if (f[u][i] != f[v][i]) u = f[u][i], v = f[v][i];
    return f[u][0];
  }
  vector<int> s, c;
  void addup(int u) {
    for (auto v : e[u]) s[v] += s[u], addup(v), c[u] += c[v];
  }
  vector<int> solve(vector<pair<int, int>> q) {
    int n = e.size();
    f.resize(n);
    d.assign(n, 0);
    makef(1, 0);
    s.assign(n, 0);
    c.assign(n, 0);
    for (auto [u, v] : q) {
      auto w = lca(u, v);
      s[w]++;
      c[u]++, c[v]++, c[w]--, c[f[w][0]]--;
    }
    addup(1);
    vector<int> res;
    for (auto [u, v] : q) {
      auto w = lca(u, v);
      res.push_back(s[u] + s[v] - 2 * s[w] + c[w] - 1);
    }
    return res;
  }
};
li read() {
  ll n;
  cin >> n;
  return n;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(3);
  int n, m;
  cin >> n >> m;
  vector<cplx> a(n);
  for (auto &[x, y] : a) x.a = read(), y.a = read();
  a.insert(a.begin(), {0, 0});
  vector<pair<int, int>> edge(n - 1);
  for (auto &[u, v] : edge) cin >> u >> v;
  vector<sgmt> line;
  for (auto [u, v] : edge) line.push_back(sgmt(a[u], a[v]));
  vector<pair<cplx, cplx>> qry(m);
  for (auto &[a, b] : qry)
    a.x.a = read(), a.x.b = read(), a.y.a = read(), a.y.b = read(),
    b.x.a = read(), b.x.b = read(), b.y.a = read(), b.y.b = read();
  map<cplx, int> id;
  for (int i = 1; i <= n; i++) id[a[i]] = i;
  vector<cplx> vtx;
  for (auto [a, b] : qry)
    for (auto v : {a, b})
      if (!id.count(v)) vtx.push_back(v);
  sort(vtx.begin(), vtx.end());
  vtx.resize(unique(vtx.begin(), vtx.end()) - vtx.begin());
  for (int i = 0; i < vtx.size(); i++) id[vtx[i]] = n + 1 + i;
  auto vol = solve(line, vtx);
  graph g(id.size());
  for (const auto &[p, q] : line) {
    auto arr = vol[{p, q}];
    arr.insert(arr.begin(), p), arr.push_back(q);
    for (int i = 1; i < arr.size(); i++) g.adde(id[arr[i - 1]], id[arr[i]]);
  }
  vector<pair<int, int>> q;
  for (auto &[a, b] : qry) q.push_back({id[a], id[b]});
  vector<int> res = g.solve(q);
  for (auto v : res) cout << v << ' ';
  cout << el;
  return 0;
}