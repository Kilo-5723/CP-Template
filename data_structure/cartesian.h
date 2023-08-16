
struct node {
  array<node *, 2> ch;
  int val, idx;
  node(int val, int idx) : val(val), idx(idx) { ch[0] = ch[1] = NULL; }
};
node *build(vector<int> &a) {
  vector<node *> stk;
  for (int i = 0; i < a.size(); i++) {
    node *last = NULL;
    while (stk.size() && stk.back()->val > a[i]) {
      last = stk.back();
      stk.pop_back();
    }
    node *u = new node(a[i], i);
    if (stk.size()) stk.back()->ch[1] = u;
    if (last) u->ch[0] = last;
    stk.push_back(u);
  }
  return stk[0];
}