struct node {
  array<node *, 2> ch;
  cplx val, sum;
  int rank;
  node(cplx val) : val(val) {
    ch[0] = ch[1] = NULL;
    sum = val;
    rank = rand();
  }
  void update() {
    sum = val;
    if (ch[0]) sum = ch[0]->sum + sum;
    if (ch[1]) sum = sum + ch[1]->sum;
  }
};
bool operator<(cplx a, cplx b) {
  auto d = det(a, b);
  if (d) return d < 0;
  if (a.x != b.x) return a.x < b.x;
  if (a.y != b.y) return a.y < b.y;
  if (a.z != b.z) return a.z < b.z;
  return false;
}
void rotate(node *&u, int c) {
  auto v = u->ch[c];
  u->ch[c] = v->ch[!c];
  v->ch[!c] = u;
  u->update();
  v->update();
  u = v;
}
node *insert(node *rt, cplx val) {
  if (!rt) return new node(val);
  auto c = rt->val < val;
  rt->ch[c] = insert(rt->ch[c], val);
  if (rt->ch[c]->rank < rt->rank) rotate(rt, c);
  rt->update();
  return rt;
}
void erase(node *&rt, cplx val) {
  if (val < rt->val) {
    erase(rt->ch[0], val);
    rt->update();
    return;
  }
  if (rt->val < val) {
    erase(rt->ch[1], val);
    rt->update();
    return;
  }
  auto tmp = rt;
  if (rt->ch[0]) {
    if (rt->ch[1]) {
      auto c = rt->ch[0]->rank > rt->ch[1]->rank;
      rotate(rt, c);
      erase(rt->ch[!c], val);
      rt->update();
    } else {
      rt = rt->ch[0];
      delete tmp;
    }
  } else {
    if (rt->ch[1]) {
      rt = rt->ch[1];
      delete tmp;
    } else {
      rt = NULL;
      delete tmp;
    }
  }
}


struct node {
  array<node *, 2> ch;
  ui p, n, avg;
  ui sl, sr, tag, sum;
  ui rank;
  node(int l, int r) {
    ch[0] = ch[1] = NULL;
    sl = p = l, sr = n = r;
    avg = tag = sum = 0;
    rank = rand();
  }
  void addtag(ui t) {
    tag += t, avg += t;
    sum += (sr - sl) * t;
  }
  void pushdown() {
    if (ch[0]) ch[0]->addtag(tag);
    if (ch[1]) ch[1]->addtag(tag);
    tag = 0;
  }
  void update() {
    sum = (n - p) * avg;
    sl = ch[0] ? ch[0]->sl : p;
    sr = ch[1] ? ch[1]->sr : n;
    if (ch[0]) sum += ch[0]->sum;
    if (ch[1]) sum += ch[1]->sum;
  }
  void add(ui l, ui r, ui v) {
    if (l >= sr || r <= sl) return;
    if (l <= sl && r >= sr) {
      addtag(v);
      return;
    }
    pushdown();
    if (l <= p && r >= n) avg += v;
    if (ch[0]) ch[0]->add(l, r, v);
    if (ch[1]) ch[1]->add(l, r, v);
    update();
  }
  ui qsum(ui l, ui r) {
    if (l >= sr || r <= sl) return 0;
    if (l <= sl && r >= sr) return sum;
    pushdown();
    ui res = 0;
    if (l <= p && r >= n) res += (n - p) * avg;
    if (ch[0]) res += ch[0]->qsum(l, r);
    if (ch[1]) res += ch[1]->qsum(l, r);
    return res;
  }
};
node *merge(node *u, node *v) {
  if (!u) return v;
  if (!v) return u;
  if (u->rank < v->rank) {
    u->pushdown();
    u->ch[1] = merge(u->ch[1], v);
    u->update();
    return u;
  } else {
    v->pushdown();
    v->ch[0] = merge(u, v->ch[0]);
    v->update();
    return v;
  }
}
pair<node *, node *> split(node *u, ui p) {
  if (!u) return {NULL, NULL};
  u->pushdown();
  if (u->p <= p) {
    auto [l, r] = split(u->ch[1], p);
    u->ch[1] = l;
    u->update();
    return {u, r};
  } else {
    auto [l, r] = split(u->ch[0], p);
    u->ch[0] = r;
    u->update();
    return {l, u};
  }
}
node *insert(node *u, ui p) {
  auto [l, r] = split(u, p);
  auto *v = l;
  while (v->ch[1]) v = v->ch[1];
  if (v->p == p) return merge(l, r);
  auto w = new node(p, v->n);
  v->n = p;
  w->avg = v->avg;
  v->update(), w->update();
  w = merge(l, w), w = merge(w, r);
  return w;
}