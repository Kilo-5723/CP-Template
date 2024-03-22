#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
typedef double ld;
const ld pi = acos(-1);
const ld inf = 1e30, eps = 1e-14;
struct cplx {
  ld x, y;
  ld abs2() const { return x * x + y * y; }
  ld abs() const { return sqrt(abs2()); }
};
cplx operator+(const cplx &a, const cplx &b) { return {a.x + b.x, a.y + b.y}; }
cplx operator-(const cplx &a, const cplx &b) { return {a.x - b.x, a.y - b.y}; }
cplx operator*(const cplx &a, ld b) { return {a.x * b, a.y * b}; }
cplx operator/(const cplx &a, ld b) { return {a.x / b, a.y / b}; }
cplx one(const cplx &a) { return a / a.abs(); }
bool operator<(const cplx &a, const cplx &b) {
  return a.x != b.x ? a.x < b.x : a.y < b.y;
}
bool operator!=(const cplx &a, const cplx &b) { return a < b || b < a; }
bool operator==(const cplx &a, const cplx &b) { return !(a == b); }
ld dot(const cplx &a, const cplx &b) { return a.x * b.x + a.y * b.y; }
ld det(const cplx &a, const cplx &b) { return a.x * b.y - a.y * b.x; }
ld radius(vector<cplx> &a) {
  ld res = (a.back() - a[0]).abs();
  for (int i = 1; i < a.size(); i++) res += (a[i] - a[i - 1]).abs();
  return res;
}
ld area(vector<cplx> &b) {
  // for (auto [x, y] : b) cout << x << ' ' << y << " ; ";
  ld res = 0;
  for (int i = 2; i < b.size(); i++) res += det(b[i - 1] - b[0], b[i] - b[0]);
  // cout << abs(res / 2) << el;
  return abs(res / 2);
}
struct func {
  vector<cplx> a;
  int k;
  ld rad, stp;
  func(vector<cplx> a, int k) : a(a), k(k), rad(radius(a)), stp(rad / k) {}
  ld f(ld x) {
    x -= stp * (ll)(x / stp);
    if (x < 0) x += stp;
    vector<cplx> b;
    cplx c = a[0];
    int p = 0;
    for (int i = 0; i < k; i++) {
      while ((a[p] - c).abs() < x + eps) {
        x -= (a[p] - c).abs();
        c = a[p];
        p = (p + 1) % a.size();
      }
      c = c + one(a[p] - c) * x;
      x = stp;
      b.push_back(c);
    }
    return area(b);
  }
};
random_device r;
default_random_engine e(r());
uniform_real_distribution<> g(0.0, 1.0);
double runtime() { return 1.0 * clock() / CLOCKS_PER_SEC; }
bool intime() { return runtime() < 4; }
ld simu(func &f) {
  ld dx = f.stp;
  ld x = 0, y = f.f(x), res = y;
  int cnt = 0;
  while (dx > 1e-10 && intime()) {
    cnt++;
    ld x2 = x + dx * (g(e) * 2 - 1), y2 = f.f(x2);
    double det = y2 - y;
    res = min(res, y2);
    if (exp(-det / dx) > g(e)) x = x2, y = y2;
    dx *= 0.97;
  }
  for (int i = 1; i <= 100 && intime(); i++) {
    double x2 = x + dx * (g(e) * 2 - 1);
    res = min(res, f.f(x2));
  }
  return res;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  int n, k;
  cin >> n >> k;
  vector<cplx> a(n);
  for (auto &[x, y] : a) cin >> x >> y;
  func f(a, k);
  ld res = inf;
  while (intime()) res = min(res, simu(f));
  cout << res << el;
  return 0;
}
/*
https://qoj.ac/contest/868/problem/3246
https://ac.nowcoder.com/acm/contest/57364/E
*/