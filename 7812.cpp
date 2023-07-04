#include <bits/stdc++.h>
using namespace std; 
typedef long long i64; 

int n; i64 ans; 
int a[1005], p[1005]; 
mt19937 Rand(19120623); 

int rndint(int l, int r) { return uniform_int_distribution<>(l, r)(Rand); }
inline double rnddb(double l, double r) { return uniform_real_distribution<>(l, r)(Rand); }

void calc(int x, int y) {
    int A = p[(x - 2 + n) % n], B = p[(x - 1 + n) % n], &C = p[x], D = p[(x + 1) % n], E = p[(x + 2) % n]; 
    ans -= 1ll * a[A] * a[B] * a[C] * (B + 1) + 1ll * a[B] * a[C] * a[D] * (C + 1) + 1ll * a[C] * a[D] * a[E] * (D + 1); 
    C = y; 
    ans -= 1ll * a[A] * a[B] * a[C] * (B + 1) + 1ll * a[B] * a[C] * a[D] * (C + 1) + 1ll * a[C] * a[D] * a[E] * (D + 1); 
}

void SA(double T, const double ET, const double delta) {
    for (int i = 0; i < n; ++i) calc(i, i); 
    while (T > ET) {
        int x = rndint(0, n - 1), y = rndint(0, n - 1); 
        i64 res = ans; 
        int px = p[x], py = p[y]; 
        calc(x, py); calc(y, px); 
        if (ans > res || exp((ans - res) / T) > rnddb(0, 1)) ; 
        else ans = res, swap(p[x], p[y]); 
        T *= delta; 
    }
    for (int i = 0; i < n; ++i) printf("%d ", p[i] + 1);
    putchar('\n');  
}

int main(void) {
    scanf("%d", &n); 
    for (int i = 0; i < n; ++i) scanf("%d", a + i); 
    SA(1e10, -1e10, 0.99); 
    return 0; 
}