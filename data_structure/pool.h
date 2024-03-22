struct node {
  int l, r;
  node *ls, *rs;
  int tag;
  node() : l(0), r(0), ls(nullptr), rs(nullptr), tag(0) {}
  static void* operator new(size_t count) {
    static node *begin = nullptr, *end = nullptr;
    if (begin == end) begin = (node*)malloc(count * 1000), end = begin + 1000;
    return begin++;
  }
};