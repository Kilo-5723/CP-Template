#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
typedef double ld;
const ld pi = acosl(-1);
const ld eps = 1e-9;
struct func {
  ll x, y, z;
  ld val(ld t) { return x * cos(t) + y * sin(t) + z; }
  pair<ld, ld> zero() {
    ld l = sqrt(x * x + y * y);
    if (l < abs(z) - eps) return {0, 2 * pi};
    ld a = atan2(y, x) + 2 * pi, b = acos(-z / l);
    ld p = a - b, q = a + b;
    while (p > 2 * pi) p -= 2 * pi;
    while (q > 2 * pi) q -= 2 * pi;
    if (p > q) swap(p, q);
    return {p, q};
  }
};
bool operator==(func a, func b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}
func operator+(func a, func b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
func operator-(func a, func b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
struct pcef {
  vector<ld> a;
  vector<func> f;
  ld val(ld x) {
    int p = upper_bound(a.begin(), a.end(), x - eps) - a.begin();
    p = max(min(p - 1, (int)f.size() - 1), 0);
    return f[p].val(x);
  }
  ld fnc(ld x) {
    return max(val(x) + val(x + pi), val(x + pi / 2) + val(x + pi * 3 / 2));
  }
  ld mxm(ld l, ld r) {
    ld res = min(fnc(l), fnc(r));
    while (r - l > eps) {
      ld m1 = (l + r) / 2, m2 = (m1 + r) / 2;
      ld v1 = fnc(m1), v2 = fnc(m2);
      res = min({res, v1, v2});
      if (v1 < v2)
        r = m2;
      else
        l = m1;
    }
    return res;
  }
};
pcef operator+(pcef a, pcef b) {
  pcef c = {{0}, {}};
  int p = 0, q = 0;
  ld xl = 0;
  while (p < a.f.size() && q < b.f.size()) {
    ld xr = min(a.a[p + 1], b.a[q + 1]);
    auto [x1, x2] = (a.f[p] - b.f[q]).zero();
    vector<ld> ax;
    if (x1 > xl && x1 < xr) ax.push_back(x1);
    if (x2 > xl && x2 < xr) ax.push_back(x2);
    ax.push_back(xr);
    for (auto xx : ax) {
      ld x = (xl + xx) / 2;
      auto f = a.f[p].val(x) > b.f[q].val(x) ? a.f[p] : b.f[q];
      if (c.f.size() && c.f.back() == f)
        c.a.back() = xx;
      else
        c.a.push_back(xx), c.f.push_back(f);
      xl = xx;
    }
    if (a.a[p + 1] < xr + eps) p++;
    if (b.a[q + 1] < xr + eps) q++;
  }
  return c;
};
pcef comb(const vector<func> &a, int l, int r) {
  if (r - l == 1) return pcef({{0, 2 * pi}, {a[l]}});
  int m = (l + r) / 2;
  return comb(a, l, m) + comb(a, m, r);
}