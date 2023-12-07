for (ll l = 1; l <= n;) {
  ll r = n / (n / l);
  ...  // for l<=i<=r, n/i = n/l
  l = r + 1;
}