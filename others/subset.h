
for (int p = 0; p < n; p++)
  for (int q = p; q; q = q - 1 & p) {
    int k = ~p & (n - 1);
    c[k + q] = max(c[k + q], a[k] + b[q]);
  }