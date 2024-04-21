#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
random_device r;
default_random_engine e(r());
uniform_int_distribution<int> g(2, 9e8);
struct hsh {
  static const int k = 2;
  array<ll, k> a;
};
hsh init(int c) {
  hsh a;
  fill(a.a.begin(), a.a.end(), c);
  return a;
}
const hsh zero{0, 0}, one{1, 1}, mul{g(e), g(e)}, mod{998244353, 1000000007};
hsh operator+(hsh a, hsh b) {
  for (int i = 0; i < hsh::k; i++) a.a[i] = (a.a[i] + b.a[i]) % mod.a[i];
  return a;
}
hsh operator-(hsh a, hsh b) {
  for (int i = 0; i < hsh::k; i++)
    a.a[i] = (mod.a[i] + a.a[i] - b.a[i]) % mod.a[i];
  return a;
}
hsh operator*(hsh a, hsh b) {
  for (int i = 0; i < hsh::k; i++) a.a[i] = (a.a[i] * b.a[i]) % mod.a[i];
  return a;
}
bool operator!=(hsh a, hsh b) {
  for (int i = 0; i < hsh::k; i++)
    if (a.a[i] != b.a[i]) return true;
  return false;
}
hsh pmul(int n) {
  static vector<hsh> res = {{1, 1}};
  while (res.size() <= n) res.push_back(mul * res.back());
  return res[n];
}
struct hshstr {
  vector<hsh> h;
  hshstr(string s = "") {
    h.push_back({0, 0});
    for (auto c : s) h.push_back(h.back() * mul + init(c));
  }
  hsh hash(int l, int r) { return h[r] - h[l] * pmul(r - l); }  // [l,r)
};
// verified by:
// https://codeforces.com/contest/1951/problem/E