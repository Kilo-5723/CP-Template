const vector<ll> test{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
// {2,7,61} for integers
li qpow(li a, li b, li mod) {
  li res = 1;
  while (b) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod, b >>= 1;
  }
  return res;
}
bool isprime(ll n) {
  if (n < 3 || n % 2 == 0) return n == 2;
  ll u = n - 1, t = 0;
  while (u % 2 == 0) u /= 2, ++t;
  for (auto a : test) {
    a = a % n;
    if (!a) continue;
    ll v = qpow(a, u, n);
    if (v == 1) continue;
    ll s;
    for (s = 0; s < t; ++s) {
      if (v == n - 1) break;
      v = (li)v * v % n;
    }
    if (s == t) return 0;
  }
  return 1;
}