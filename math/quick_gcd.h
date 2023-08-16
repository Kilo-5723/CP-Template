
unsigned long long gcd(unsigned long long a, unsigned long long b) {
  int shift = __builtin_ctzll(a | b);
  b >>= __builtin_ctzll(b);
  while (a) {
    a >>= __builtin_ctzll(a);
    if (a < b) swap(a, b);
    a -= b;
  }
  return b << shift;
}