const ll mod = 998244353;
typedef vector<ll> poly;

ll qpow(ll a, ll b) {
  ll res = 1;
  while (b) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod;
    b >>= 1;
  }
  return res;
}
ll inv(ll n) { return qpow(n, mod - 2); }

const auto pi = acosl(-1);
const int len = 15, mask = (1 << len) - 1;

vector<ll> get_root(int n) {
  n = 1 << 32 - __builtin_clz(n);
  int mul = qpow(3, (mod - 1) / n);
  vector<ll> res{1};
  res[0] = 1;
  for (int i = 1; i < n; i++) res.push_back(res.back() * mul % mod);
  return res;
}

void fft(vector<ll> &p, const vector<ll> &w) {
  int n = w.size();
  for (int i = 1, j = 0; i < n - 1; ++i) {
    int s = n;
    do {
      s >>= 1;
      j ^= s;
    } while (~j & s);
    if (i < j) swap(p[i], p[j]);
  }
  for (int d = 0; (1 << d) < n; ++d) {
    int m = 1 << d, m2 = m * 2, rm = n >> (d + 1);
    for (int i = 0; i < n; i += m2)
      for (int j = 0; j < m; ++j) {
        auto &p1 = p[i + j + m], &p2 = p[i + j];
        p1 %= mod, p2 %= mod;
        auto t = w[rm * j] * p1 % mod;
        p1 = (p2 - t) % mod;
        p2 = (p2 + t) % mod;
      }
  }
}
poly operator+(const poly &a, const poly &b) {
  poly c(max(a.size(), b.size()));
  for (int i = 0; i < a.size(); i++) c[i] += a[i];
  for (int i = 0; i < b.size(); i++) c[i] += b[i];
  for (auto &v : c) v %= mod;
  return c;
}
poly operator-(poly b) {
  for (auto &v : b) v = v ? mod - v : 0;
  return b;
}
poly operator-(const poly &a, const poly &b) { return a + -b; }
poly operator*(poly a, poly b) {
  int m = a.size() + b.size() - 1;
  vector<ll> w = get_root(m);
  int n = w.size();
  a.resize(n), b.resize(n);
  fft(a, w), fft(b, w);
  for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % mod;
  fft(a, w);
  reverse(a.begin() + 1, a.end());
  a.resize(m);
  ll invn = inv(n);
  for (auto &v : a) v = (v * invn % mod + mod) % mod;
  return a;
}