#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
i64 fpow(i64 a, i64 t, i64 mod){
	i64 r = 1;
	for (; t; t >>= 1, a = (i128)a * a % mod) {
		if (t & 1) {
			r = (i128)r * a % mod;
		}
	}
	return r;
}
i64 gcd(i64 a, i64 b){
	#define ctz __builtin_ctzll
	int shift = ctz(a | b);
	b >>= ctz(b);
	while (a) {
		a >>= ctz(a);
		if (a < b) swap(a, b);
		a -= b;
	}
	return b << shift;
}
bool check_prime(i64 n){
	static const int jp[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	if (n == 1) return false;
	for (int p : jp) if (n % p == 0) return n == p;
	i64 r = n - 1, x, y;
	int e = 0;
	while (~r & 1) r >>= 1, ++e;
	for (int p : jp) {
		x = fpow(p, r, n);
		for (int t = 0; t < e && x > 1; ++t) {
			y = (i128)x * x % n;
			if (y == 1 && x != n - 1) return false;
			x = y;
		}
		if (x != 1) return false;
	}
	return true;
}
i64 find(i64 n){
	static const int step = 1 << 7;
	i64 x, y = rand() % n;
	int c = rand() % n;
	auto f = [=](i64 x){ return ((i128)x * x + c) % n; } ;
	for (int l = 1; ; l <<= 1) {
		x = y;
		for (int i = 0; i < l; ++i) y = f(y);
		for (int k = 0; k < l; k += step) {
			int e = std::min(step, l - k);
			i64 g = 1, z = y;
			for (int i = 0; i < e; ++i) g = (i128)g * ((y = f(y)) + n - x) % n;
			g = gcd(g, n);
			if (g == 1) continue;
			if (g == n) for (g = 1, y = z; g == 1; ) y = f(y), g = gcd(y + n - x, n);
			return g;
		}
	} //
}
void rho(i64 n, map<i64,int> &factor){
	while (~n & 1) n >>= 1, ++factor[2];
	if (n == 1) return ;
	if (check_prime(n)) {
		++factor[n];
		return ;
	}
	i64 d;
	for (d = find(n); d == n; d = find(d));
	rho(d, factor), rho(n / d, factor);
}
int T;
i64 n;
int main(){
	for (cin >> T; T; --T) {
		map<i64, int> f;
		cin >> n;
		rho(n, f);
		if (f.size() > 1 || (--f.end())->second > 1) {
			cout << (--f.end())->first << '\n';
		} else {
			cout << "Prime\n";
		}
	}
}