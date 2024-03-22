for (ll l = 1, r; l <= n; l = r + 1) {
  r = n / (n / l);
  ...  // for l<=i<=r, n/i = n/l
}
/*
https://qoj.ac/contest/1103/problem/5503
*/