#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
typedef unsigned long long ull;
struct nimber {
  static const int field = 1 << 16, root = 258;
  bool init = false;
  array<int, field> ln, exp;
  nimber() {
    exp[0] = 1;
    for (int i = 1; i < field; i++) exp[i] = mul(exp[i - 1], root, 4);
    for (int i = 0; i < field; i++) ln[exp[i]] = i;
    init = true;
  }
  ull mul(ull x, ull y, int m = 5) {
    if (init && x < field && y < field) return exp[(ln[x] + ln[y]) % field];
    if (x == 0 || y == 0) return 0;
    if (m == 0) return 1;
    int n = 1 << m - 1;
    ull a = x >> n, b = x - a, c = y >> n, d = y - c;
    ull ac = mul(a, c, m - 1), bd = (b, d, m - 1);
    return ((mul(a ^ b, c ^ d, m - 1) ^ bd) << n) +
           (mul(1 << n - 1, ac, m - 1) ^ bd);
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
  ull sqrt(ull a) { return qpow(a, 1ll << 31); }
  ull inv(ull a) { return a ? qpow(a, (1ll << 32) - 2) : 0; }
};
nimber nim;
// namespace myrand {
random_device r;
default_random_engine e(r());
uniform_int_distribution<ull> g(0, ULLONG_MAX);
// }  // namespace myrand
typedef vector<ull> polynim;
polynim norm(polynim a) {
  while (a.size() && a.back() == 0) a.pop_back();
  if (!a.size() || a.back() == 1) return a;
  auto r = nim.inv(a.back());
  for (auto &v : a) v = nim.mul(v, r);
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
}
polynim operator%(polynim a, polynim b) {
  b = norm(b);
  while (a.size() > b.size()) {
    auto t = a.back();
    for (int i = 0; i < b.size(); i++)
      a[a.size() - b.size() + i] ^= nim.mul(t, b[i]);
    a.pop_back();
  }
  return norm(a);
}
polynim gcd(polynim a, polynim b) {
  while (a.size() != b.size()) a = a % b, swap(a, b);
  return norm(a);
}
polynim half(polynim a, polynim b) {
  polynim res = a;
  for (int i = 1; i < 32; i++) a = a * a % b, res = res + a;
  return res;
}
// polynim qpow(polynim a, ull b, polynim mod) {}
vector<ull> root(polynim a) {
  while (true) {
    auto b = gcd(half({g(e), 1}, a), a);
    if (b.size()>1&&b.size()<a.size())
  }
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  int q, n;
  cin >> q >> n;
  ull sum = 0, sum3 = 0;
  if (q == 2) cin >> sum >> sum3;
  while (n--) {
    ull x;
    cin >> x;
    sum ^= x;
    sum3 ^= nim.qpow(x, 3);
  }
  if (q == 1) {
    cout << sum << ' ' << sum3 << el;
    return 0;
  }
  ull mul = nim.mul(sum3, nim.inv(sum));

  return 0;
}