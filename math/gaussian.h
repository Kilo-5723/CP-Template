
const ll mod = 1e9 + 7, pri = 5;
const array<int, pri> inv = {0, 1, 3, 2, 4};
ll qpow(ll a, ll b, ll m = mod) {
  ll res = 1;
  while (b) {
    if (b & 1) res = res * a % m;
    a = a * a % m, b >>= 1;
  }
  return res;
}
vector<int> read() {
  string s;
  cin >> s;
  vector<int> a(s.size());
  for (int i = 0; i < s.size(); i++) a[i] = s[i] - 'a';
  return a;
}
void flip(vector<vector<int>> &a) {
  int n = a.size(), m = a[0].size();
  vector b(m, vector<int>(n));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) b[j][i] = a[i][j];
  swap(a, b);
}
int gaussian(vector<vector<int>> &a) {
  int n = a.size(), m = a[0].size();
  int r = 0;
  for (int j = 0; j < m; j++) {
    int p = -1;
    for (int i = r; i < n; i++)
      if (a[i][j]) p = i;
    if (p == -1) continue;
    swap(a[r], a[p]);
    int t = inv[a[r][j]];
    for (int k = 0; k < m; k++) a[r][k] = a[r][k] * t % pri;
    for (int i = 0; i < n; i++)
      if (i != r) {
        int t = a[i][j];
        for (int k = 0; k < m; k++)
          a[i][k] = ((a[i][k] - a[r][k] * t) % pri + pri) % pri;
      }
    r++;
  }
  a.resize(r);
  return r;
}
bool solve(vector<vector<int>> &a, vector<int> &b) {
  int n = a.size(), m = a[0].size();
  for (int i = 0; i < n; i++) {
    int p = 0;
    while (!a[i][p]) p++;
    int t = b[p];
    for (int j = 0; j < m; j++) b[j] = ((b[j] - a[i][j] * t) % pri + pri) % pri;
  }
  for (auto v : b)
    if (v % pri) return false;
  return true;
}