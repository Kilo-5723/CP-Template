unsigned int gcd(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0) return v;
  if (v == 0) return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    unsigned m;
    v >>= __builtin_ctz(v);
    v -= u;
    m = (int)v >> 31;
    u += v & m;
    v = (v + m) ^ m;
  } while (v != 0);
  return u * u << shift * 2;
}