const int det = 100;
const ll mod = 1e9 + 7;
template <typename T>
struct min25 {
  ll n, m;
  vector<ll> p;
  vector<T> s;
  void sieve(ll n) {
    p = {1}, s = {T()};
    vector<int> minf(n);
    for (ll i = 2; i < n; i++) {
      if (!minf[i]) {
        minf[i] = p.size();
        p.push_back(i);
        s.push_back(s.back() + T::init(i));
      }
      for (ll j = 1; j <= minf[i] && i * p[j] < n; j++) minf[i * p[j]] = j;
    }
  }
  vector<ll> lis;
  vector<int> le, ge;
  vector<T> g;
  void init(ll n) {
    le.resize(m + det), ge.resize(m + det);
    for (ll i = 1, j; i <= n; i = n / j + 1) {
      j = n / i;
      int k = lis.size();
      lis.push_back(j);
      (j <= m ? le[j] : ge[n / j]) = k;
      g.push_back(T::plug(j));
    }
  }
  int id(ll v) { return v <= m ? le[v] : ge[n / v]; }
  vector<ll> f;
  void calcp() {
    for (int k = 1; k < p.size(); k++) {
      ll pk = p[k], sp = pk * pk;
      for (int i = 0; i < lis.size() && lis[i] >= sp; i++) {
        int j = id(lis[i] / pk);
        g[i] = g[i] - (g[j] - s[k - 1]) * T::item(pk);
      }
    }
    for (auto &v : g) f.push_back(v.val());
  }
  min25(ll n) : n(n) {
    m = sqrt(n);
    sieve(m + det);
    init(n);
    calcp();
  }
  ll query(ll n, int k = 1) {
    if (n < p[k] || n <= 1) return 0;
    const int i = id(n);
    ll ans = f[i] - s[k - 1].val();
    for (int i = k; i < p.size() && 1ll * p[i] * p[i] <= n; i++) {
      ll mul = p[i];
      for (int c = 1; mul * p[i] <= n; c++, mul *= p[i])
        ans += T::f(p[i], c) * query(n / mul, i + 1) % mod + T::f(p[i], c + 1);
    }
    return ans % mod;
  }
};
struct ez {
  ll a, b;
  ez(ll a = 0, ll b = 0) : a(a % mod), b(b % mod) {}
  ez operator+(ez r) { return {(a + r.a) % mod, (b + r.b) % mod}; }
  ez operator-(ez r) { return {(a - r.a) % mod, (b - r.b) % mod}; }
  ez operator*(ez r) { return {a * r.a % mod, b * r.b % mod}; }
  ll val() { return (a + b) % mod; }
  static ll f(ll p, ll c) { return p ^ c; }
  static ez init(ll p) { return {-1, p}; }
  static ez plug(ll p) {
    p %= mod;
    return {-p + 1, (p + 2) * (p - 1) % mod * (mod + 1 >> 1) % mod};
  }
  static ez item(ll p) { return {1, p}; }
};