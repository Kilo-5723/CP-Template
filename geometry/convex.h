#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
struct cplx {
  ll x, y;
  ll abs2() { return x * x + y * y; }
  ld abs() { return sqrt(abs2()); }
};
cplx operator+(const cplx &a, const cplx &b) { return {a.x + b.x, a.y + b.y}; }
cplx operator-(const cplx &a, const cplx &b) { return {a.x - b.x, a.y - b.y}; }
bool operator<(const cplx &a, const cplx &b) {
  return a.x != b.x ? a.x < b.x : a.y < b.y;
}
bool operator!=(const cplx &a, const cplx &b) { return a < b || b < a; }
bool operator==(const cplx &a, const cplx &b) { return !(a != b); }
ll dot(const cplx &a, const cplx &b) { return a.x * b.x + a.y * b.y; }
ll det(const cplx &a, const cplx &b) { return a.x * b.y - a.y * b.x; }
void diagsort(vector<cplx> &a, cplx o) {
  sort(a.begin(), a.end(), [&](const cplx &p, const cplx &q) {
    if ((o < p) != (o < q)) return o < p;
    auto d = det(p - o, q - o);
    if (d) return d > 0;
    return (p - o).abs2() > (q - o).abs2();
  });
  a.resize(unique(a.begin(), a.end(),
                  [&](const cplx &p, const cplx &q) {
                    return !det(p - o, q - o) && dot(p - o, q - o) > 0;
                  }) -
           a.begin());
}
vector<cplx> convex(vector<cplx> a) {
  sort(a.begin(), a.end());
  auto o = a[0];
  a.erase(a.begin());
  diagsort(a, o);
  vector<cplx> res = {o};
  for (auto v : a) {
    while (res.size() >= 2) {
      auto p = res[res.size() - 2], q = res[res.size() - 1];
      if (det(q - p, v - q) > 0) break;
      res.pop_back();
    }
    res.push_back(v);
  }
  return res;
}
double diameter(const vector<cplx> &a) {
  double res = (a.back() - a[0]).abs();
  for (int i = 1; i < a.size(); i++) res += (a[i] - a[i - 1]).abs();
  return res;
}

cplx find(vector<cplx> &conv, cplx a) {
  int l = 0, r = conv.size() - 1;
  while (r - l) {
    int m = (l + r) / 2;
    if (det(conv[m + 1] - conv[m], a) < 0)
      r = m;
    else
      l = m + 1;
  }
  return conv[l];
}
ll solve(vector<ll> &a) {
  vector<ll> s = {0};
  for (auto v : a) s.push_back(s.back() + v);
  vector<cplx> conv;
  for (int i = 0; i < s.size(); i++) {
    cplx r = {i, s[i]};
    while (conv.size() >= 2) {
      cplx p = conv[conv.size() - 2], q = conv.back();
      if (det(q - p, r - q) > 0) break;
      conv.pop_back();
    }
    conv.push_back(r);
  }
  ll res = 0;
  for (int i = 0; i < a.size(); i++) {
    auto p = find(conv, {1, a[i]});
    res = max(res, s[i] - p.y + (p.x - i) * a[i]);
  }
  return res;
}