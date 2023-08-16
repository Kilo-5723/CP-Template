#include "bits/stdc++.h"
using namespace std;
const char el = '\n';
typedef long long ll;
const ll mod = 1e9 + 7, mul = 157;
vector<ll> hsh, rhs, powm;
int n;
string s;
ll subhsh(vector<ll> &hsh, int l, int r) {
  return ((hsh[r] - powm[r - l] * hsh[l]) % mod + mod) % mod;
}
bool comp(int p, int q) {
  int l = 0, r = q - p;
  while (r - l > 1) {
    int m = (l + r) / 2;
    if (subhsh(hsh, p, p + m) == subhsh(rhs, n - q, n - q + m))
      l = m;
    else
      r = m;
  }
  // cout << p << ' ' << q << ' ' << r << ' ' << s[p + l] << ' ' << s[q - r] << el;
  return s[p + l] < s[q - r];
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  cin >> n;
  cin >> s;
  powm = {1};
  for (auto ch : s) powm.push_back(powm.back() * mul % mod);
  hsh = {0};
  for (auto ch : s) hsh.push_back((hsh.back() * mul + ch) % mod);
  reverse(s.begin(), s.end());
  rhs = {0};
  for (auto ch : s) rhs.push_back((rhs.back() * mul + ch) % mod);
  reverse(s.begin(), s.end());
  int p = 0, q = n;
  while (p < q)
    if (comp(p, q))
      cout << s[p++];
    else
      cout << s[--q];
  cout << el;
  return 0;
}