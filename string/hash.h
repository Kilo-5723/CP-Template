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

#include <bits/stdc++.h>
using namespace std;
const char el = '\n';
typedef long long ll;
const int alpha = 26;
const int thr = 500;
struct node {
  array<node *, alpha> ch;
  int cnt;
  node() : cnt(0) { ch.fill(NULL); }
};
void add(node *u, const string &s) {
  for (auto c : s) {
    c -= 'a';
    if (!u->ch[c]) u->ch[c] = new node();
    u = u->ch[c];
    u->cnt++;
  }
}
pair<node *, ll> run(node *u, const string &s) {
  ll res = 0;
  for (auto c : s) {
    c -= 'a';
    if (!u->ch[c]) return {NULL, res};
    u = u->ch[c];
    res += u->cnt;
  }
  return {u, res};
}
const ll mod = 1e9 + 7, mul = 307;
struct hstr {
  vector<ll> pre;
  static vector<ll> pow;
  hstr(string s = "") {
    pre = {0};
    for (auto c : s) pre.push_back((pre.back() * mul + c) % mod);
    while (pow.size() <= s.size()) pow.push_back(pow.back() * mul % mod);
  }
  ll hash(int l, int r) {
    return ((pre[r] - pre[l] * pow[r - l]) % mod + mod) % mod;
  }
  int size() { return pre.size() - 1; }
};
vector<ll> hstr::pow = {1};
pair<int, bool> calc(hstr &a, hstr &b, int p) {
  int q = p + b.size();
  if (q <= a.size() && a.hash(p, q) == b.pre.back()) return {q, true};
  int res;
  int l = 0, r = min(b.size(), a.size() + 1 - p);
  while (r - l > 1) {
    int m = (l + r) / 2;
    if (a.hash(p, p + m) != b.hash(0, m))
      r = m;
    else
      l = m;
  }
  return {p + l, false};
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout << setprecision(15);
  int n, m, q;
  cin >> n;
  vector<string> a(n);
  for (auto &v : a) cin >> v;
  cin >> m;
  vector<string> b(m);
  for (auto &v : b) cin >> v;
  node *rt = new node();
  vector<hstr> s, t;
  for (auto v : a)
    if (v.size() > thr)
      s.push_back(hstr(v));
    else
      add(rt, v);
  for (auto v : b) t.push_back(hstr(v));
  cin >> q;
  while (q--) {
    int k;
    cin >> k;
    vector<int> c(k);
    for (auto &v : c) cin >> v, v--;
    ll ans = 0;
    node *u = rt;
    for (auto v : c) {
      auto [ptr, val] = run(u, b[v]);
      u = ptr, ans += val;
      if (u == NULL) break;
    }
    for (auto &u : s) {
      int p = 0;
      for (auto &v : c) {
        auto [q, flg] = calc(u, t[v], p);
        p = q;
        if (!flg) break;
      }
      ans += p;
    }
    cout << ans << el;
  }
  return 0;
}