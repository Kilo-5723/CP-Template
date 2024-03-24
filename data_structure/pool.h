// ver 2.0
struct node {
  static void* operator new(size_t count) {
    static node *begin = nullptr, *end = nullptr;
    if (begin == end) begin = (node*)malloc(count * 100), end = begin + 100;
    return begin++;
  }
};