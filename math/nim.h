#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 li;
struct nimber {
  static const int field = 1 << 16, root = 258;
  bool init = false;
  array<int, field> ln, exp;
  nimber() {
    exp[0] = 1;
    for (int i = 1; i < field; i++) exp[i] = mul(exp[i - 1], root, 4);
    for (int i = 0; i < field - 1; i++) ln[exp[i]] = i;
    init = true;
  }
  ull mul(ull x, ull y, int m = 6) {
    if (!x || !y) return 0;
    if (!m) return 1;
    if (init && x < field && y < field)
      return exp[(ln[x] + ln[y]) % (field - 1)];
    int n = 1 << m - 1;
    ull k = ((ull)1 << n) - 1;
    ull a = x >> n, b = x & k, c = y >> n, d = y & k;
    ull ac = mul(a, c, m - 1), bd = mul(b, d, m - 1);
    auto res = ((mul(a ^ b, c ^ d, m - 1) ^ bd) << n) +
               (mul(1 << n - 1, ac, m - 1) ^ bd);
    return res;
  }
  ull qpow(ull a, ull b) {
    ull res = 1;
    while (b) {
      if (b & 1) res = mul(res, a);
      a = mul(a, a);
      b >>= 1;
    }
    return res;
  }
  ull sqrt(ull a) { return qpow(a, (ull)1 << 63); }
  ull inv(ull a) { return qpow(a, (ull)-2); }
} nim;
random_device r;
default_random_engine e(r());
uniform_int_distribution<ull> g(0, ULLONG_MAX);
typedef vector<ull> polynim;
polynim norm(polynim a) {
  while (a.size() && a.back() == 0) a.pop_back();
  if (!a.size() || a.back() == 1) return a;
  auto r = nim.inv(a.back());
  for (auto &v : a) v = nim.mul(v, r);
  assert(a.back() == 1);
  return a;
}
polynim operator+(polynim a, polynim b) {
  if (a.size() < b.size()) swap(a, b);
  for (int i = 0; i < b.size(); i++) a[i] ^= b[i];
  return a;
}
polynim operator*(polynim a, polynim b) {
  polynim c(a.size() + b.size() - 1);
  for (int i = 0; i < a.size(); i++)
    for (int j = 0; j < b.size(); j++) c[i + j] ^= nim.mul(a[i], b[j]);
  return c;
}
polynim operator/(polynim a, polynim b) {
  b = norm(b);
  polynim c;
  while (a.size() >= b.size()) {
    auto t = a.back();
    for (int i = 0; i < b.size(); i++)
      a[a.size() - b.size() + i] ^= nim.mul(t, b[i]);
    a.pop_back();
    c.push_back(t);
  }
  reverse(c.begin(), c.end());
  return c;
}
polynim operator%(polynim a, polynim b) {
  b = norm(b);
  if (!b.size()) return a;
  while (a.size() >= b.size()) {
    auto t = a.back();
    for (int i = 0; i < b.size(); i++)
      a[a.size() - b.size() + i] ^= nim.mul(t, b[i]);
    a.pop_back();
  }
  return a;
}
polynim qpow(polynim a, li b, polynim mod) {
  a = a % mod;
  polynim res = {1};
  while (b) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod;
    b >>= 1;
  }
  return norm(res);
}
polynim gcd(polynim a, polynim b) {
  a = norm(a), b = norm(b);
  while (b.size()) a = norm(a % b), swap(a, b);
  return norm(a);
}
polynim half(polynim a, polynim b) {
  norm(b);
  polynim res;
  a = a % b;
  for (int i = 0; i < 64; i++) res = res + a, a = a * a % b;
  return res;
}
vector<ull> decomp(polynim a) {
  if (a.size() == 2) return {a[0]};
  polynim d;
  while (d.size() < 2 || d.size() == a.size()) {
    d = gcd(half({g(e), g(e)}, a), a);
  }
  auto l = decomp(d), r = decomp(a / d);
  l.insert(l.end(), r.begin(), r.end());
  return l;
}
vector<ull> root(polynim a) {
  if (a.size() == 2) return {a[0]};
  a = gcd(a, qpow({0, 1}, (li)1 << 64, a) + (polynim){0, 1});
  return decomp(a);
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  vector<ull> a(100);
  while (true) {
    for (auto &v : a) v = g(e);
    polynim que = {1};
    for (auto v : a) que = que * polynim{v, 1};
    auto res = root(que);
    sort(res.begin(), res.end());
    sort(a.begin(), a.end());
    for (auto v : res) cout << v << ' ';
    cout << endl;
    for (auto v : a) cout << v << ' ';
    cout << endl;
  }
  return 0;
}