
const int k = 17, n = 1 << k;
const int mod = 1e9 + 7, inv2 = mod / 2 + 1;
struct modint {
  int n;
  modint(int n = 0) : n(n) {}
};
modint operator+(modint a, modint b) {
  return (a.n += b.n) >= mod ? a.n - mod : a.n;
}
modint operator-(modint a, modint b) {
  return (a.n -= b.n) < 0 ? a.n + mod : a.n;
}
modint operator*(modint a, modint b) { return 1ll * a.n * b.n % mod; }
vector<modint> fib(n);
vector<modint> fwt(vector<modint> a, void (*opr)(modint &, modint &)) {
  // cout << 1 << el;
  int n = a.size();
  for (int i = 0; 1 << i < n; i++)
    for (int j = 0; j < n; j++)
      if (j >> i & 1) opr(a[j - (1 << i)], a[j]);
  // cout << 2 << el;
  return a;
}
void fwtand(modint &a, modint &b) { a = a + b; }
void revand(modint &a, modint &b) { a = a - b; }
void fwtor(modint &a, modint &b) { b = b + a; }
void revor(modint &a, modint &b) { b = b - a; }
void fwtxor(modint &a, modint &b) {
  modint x = a + b, y = a - b;
  a = x, b = y;
}
void revxor(modint &a, modint &b) {
  modint x = a + b, y = a - b;
  a = x * inv2, b = y * inv2;
}
vector<modint> add(vector<modint> a, vector<modint> b) {
  for (int i = 0; i < a.size(); i++) a[i] = a[i] + b[i];
  return a;
}
vector<modint> mul(vector<modint> a, vector<modint> b) {
  for (int i = 0; i < a.size(); i++) a[i] = a[i] * b[i];
  return a;
}
vector<int> popc(n);
vector<modint> filter(vector<modint> a, int k) {
  for (int i = 0; i < n; i++)
    if (popc[i] != k) a[i] = 0;
  return a;
}
int main() {
  init();
  fib[0] = 0, fib[1] = 1;
  for (int i = 2; i < n; i++) fib[i] = fib[i - 1] + fib[i - 2];
  popc[0] = 0;
  for (int i = 1; i < n; i++) popc[i] = popc[i >> 1] + (i & 1);
  int m;
  cin >> m;
  vector<int> s(m);
  for (auto &v : s) cin >> v;
  vector<modint> a(n);
  for (auto v : s) a[v] = a[v] + 1;
  auto mid = mul(a, fib),
       rhs = mul(fwt(mul(fwt(a, fwtxor), fwt(a, fwtxor)), revxor), fib);
  vector b(k + 1, vector<modint>());
  vector c(k + 1, vector<modint>(n));
  vector<modint> lhs(n);
  for (int i = 0; i <= k; i++) b[i] = fwt(filter(a, i), fwtor);
  for (int i = 0; i <= k; i++)
    for (int j = 0; i + j <= k; j++) c[i + j] = add(c[i + j], mul(b[i], b[j]));
  for (int i = 0; i <= k; i++)
    lhs = add(lhs, filter(mul(fwt(c[i], revor), fib), i));
  auto res = fwt(mul(mul(fwt(lhs, fwtand), fwt(mid, fwtand)), fwt(rhs, fwtand)),
                 revand);
  modint ans;
  for (int i = 0; i < k; i++) ans = ans + res[1 << i];
  cout << ans.n << el;
  return 0;
}