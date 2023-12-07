struct lct {
  vector<array<int, 2>> ch;
  vector<int> fa;
  vector<int> rev;
  vector<int> siz;
  lct(int n)
      : ch(n + 1, {0, 0}), fa(n + 1, 0), rev(n + 1, false), siz(n + 1, 1) {
    siz[0] = 0;  // initialize nil
  }
  void update(int u) { siz[u] = siz[ch[u][0]] + siz[ch[u][1]] + 1; }
  void reverse(int u) {
    if (u) rev[u] ^= 1, swap(ch[0], ch[1]);
  }
  void pushdown(int u) {
    if (rev[u]) reverse(ch[u][0]), reverse(ch[u][1]);
    rev[u] = false;
  }
  int chid(int u) {
    if (ch[fa[u]][0] == u) return 0;
    if (ch[fa[u]][1] == u) return 1;
    return -1;
  }
  bool isroot(int u) { return chid(u) == -1; }
  void rotate(int u) {
    int v = fa[u], w = fa[v], k = chid(u), x = ch[u][!k];
    if (!isroot(v)) ch[w][chid(v)] = u;
    ch[u][!k] = v, ch[v][k] = x;
    if (x) fa[x] = v;
    fa[v] = u, fa[u] = w;
    update(v), update(u);
  }
  void clearup(int u) {
    if (!isroot(u)) clearup(fa[u]);
    pushdown(u);
  }
  void splay(int u) {
    clearup(u);
    while (!isroot(u)) {
      int f = fa[u];
      if (!isroot(f)) rotate(chid(u) == chid(f) ? f : u);
      rotate(u);
    }
  }
  int access(int u) {
    int v;
    for (v = 0; u; v = u, u = fa[u]) splay(u), ch[u][1] = v, update(u);
    return v;
  }
  void makeroot(int u) {
    u = access(u);
    reverse(u);
  }
  void link(int u, int v) {
    makeroot(u);
    splay(u);
    fa[u] = v;
  }
  void cut(int u,int v){
    makeroot(u);
    access(v);
    splay(v);
    ch[v][0]=fa[u]=0;
    update(v);
  }
};