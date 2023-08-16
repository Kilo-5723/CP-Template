
struct cplx {
  ll x, y;
  double abs() { return sqrt(x * x + y * y); }
};
cplx operator+(cplx a, cplx b) { return {a.x + b.x, a.y + b.y}; }
cplx operator-(cplx a, cplx b) { return {a.x - b.x, a.y - b.y}; }
ll dot(cplx a, cplx b) { return a.x * b.x + a.y * b.y; }
ll det(cplx a, cplx b) { return a.x * b.y - a.y * b.x; }
cplx find(vector<cplx> &conv, cplx a) {
  int l = 0, r = conv.size() - 1;
  while (r - l) {
    int m = (l + r) / 2;
    if (det(conv[m + 1] - conv[m], a) < 0)
      r = m;
    else
      l = m + 1;
  }
  return conv[l];
}
ll solve(vector<ll> &a) {
  vector<ll> s = {0};
  for (auto v : a) s.push_back(s.back() + v);
  vector<cplx> conv;
  for (int i = 0; i < s.size(); i++) {
    cplx r = {i, s[i]};
    while (conv.size() >= 2) {
      cplx p = conv[conv.size() - 2], q = conv.back();
      if (det(q - p, r - q) > 0) break;
      conv.pop_back();
    }
    conv.push_back(r);
  }
  ll res = 0;
  for (int i = 0; i < a.size(); i++) {
    auto p = find(conv, {1, a[i]});
    res = max(res, s[i] - p.y + (p.x - i) * a[i]);
  }
  return res;
}