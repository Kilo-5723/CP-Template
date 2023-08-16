
unordered_map<ll, int> result;
struct status {
  array<int, 10> a;
  status() { fill(a.begin(), a.end(), 0); }
  status flip() {
    status b;
    for (int i = 0; i < 5; i++) b.a[i] = a[i + 5];
    for (int i = 0; i < 5; i++) b.a[i + 5] = a[i];
    return b;
  }
  ll num() {
    ll res = 0;
    for (int i = 0; i < 10; i++) res = (res << 5) + a[i];
    return res;
  }
  vector<status> from() {
    vector<status> res;
    for (int i = 0; i < 5; i++)
      if (a[i])
        for (int j = 0; j < 5; j++)
          if (a[j + 5])
            if (i && j - i) {
              auto b = *this;
              b.a[j + 5]--;
              b.a[(j + 5 - i) % 5 + 5]++;
              res.push_back(b.flip());
            }
    // cout << "from " << res.size() << el;
    return res;
  }
  vector<status> to() {
    vector<status> res;
    for (int i = 0; i < 5; i++)
      if (a[i])
        for (int j = 0; j < 5; j++)
          if (a[j + 5])
            if (i && j) {
              auto b = *this;
              b.a[i]--;
              b.a[(i + j) % 5]++;
              res.push_back(b.flip());
            }
    // cout << "to: " << res.size() << el;
    return res;
  }
  bool win() {
    if (result.count(num())) return result[num()] == 1;
    for (auto v : to())
      if (result.count(v.num()) && result[v.num()] == -1) return true;
    return false;
  }
  bool lose() {
    if (result.count(num())) return result[num()] == -1;
    for (auto v : to())
      if (!result.count(v.num()) || result[v.num()] != 1) return false;
    return true;
  }
};
bool operator<(const status &a, const status &b) { return a.a < b.a; }
status read() {
  status a;
  for (int i = 0; i < 8; i++) {
    int t;
    cin >> t;
    a.a[t]++;
  }
  for (int i = 0; i < 8; i++) {
    int t;
    cin >> t;
    a.a[t + 5]++;
  }
  return a;
}

set<status> losestat(int t) {
  if (!t) {
    status tmp;
    tmp.a[5] = 8;
    return {tmp};
  }
  set<status> res;
  auto tmp = losestat(t - 1);
  for (auto v : tmp) {
    for (int i = 0; i < 5; i++) {
      auto t = v;
      t.a[i]++;
      res.insert(t);
    }
  }
  return res;
}
set<status> winstat(int t) {
  if (!t) {
    status tmp;
    tmp.a[0] = 8;
    return {tmp};
  }
  set<status> res;
  auto tmp = winstat(t - 1);
  for (auto v : tmp) {
    for (int i = 0; i < 5; i++) {
      auto t = v;
      t.a[i + 5]++;
      res.insert(t);
    }
  }
  return res;
}
void preprocess() {
  auto wst = winstat(8), lst = losestat(8);
  vector<status> win, lose;
  for (auto v : wst) {
    win.push_back(v);
    result[v.num()] = 1;
  }
  for (auto v : lst) {
    lose.push_back(v);
    result[v.num()] = -1;
  }
  while (win.size() || lose.size()) {
    if (win.size()) {
      for (auto wn : win)
        for (auto ls : wn.from()) {
          if (!result.count(ls.num()) && ls.lose()) {
            lose.push_back(ls);
            result[ls.num()] = -1;
          }
        }
      win.clear();
    } else {
      for (auto ls : lose)
        for (auto wn : ls.from()) {
          if (!result.count(wn.num()) && wn.win()) {
            win.push_back(wn);
            result[wn.num()] = 1;
          }
        }
      lose.clear();
    }
  }
}