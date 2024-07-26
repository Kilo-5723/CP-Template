const ll mod = 1e9 + 7;
// inversion from 1 to n
vector<ll> invs(int n) {
  vector<ll> res(n + 1);
  res[1] = 1;
  for (int i = 2; i <= n; i++) res[i] = (mod - mod / i) * res[mod % i] % mod;
  return res;
}
ll qpow(ll a, ll b) {
  ll res = 1;
  while (b) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod, b >>= 1;
  }
  return res;
}
// inversion for arbitrary numbers
vector<ll> invs(const vector<ll> &a) {
  int n = a.size();
  vector<ll> s(n + 1);
  s[0] = 1;
  for (int i = 0; i < n; i++) s[i + 1] = s[i] * a[i] % mod;
  vector<ll> res(n + 1);
  res.back() = qpow(s.back(), mod - 2);
  for (int i = n; i; i--) res[i - 1] = res[i] * a[i - 1] % mod;
  for (int i = 1; i <= n; i++) res[i] = res[i] * s[i - 1] % mod;
  return res;
}