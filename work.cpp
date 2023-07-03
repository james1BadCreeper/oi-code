#include <bits/stdc++.h>
using namespace std; 
typedef long long i64; 

int n; i64 ans; 
int a[1005], p[1005]; 
mt19937 Rand(19120623); 

int rndint(int l, int r) { return uniform_int_distribution<>(l, r)(Rand); }
inline double rnddb(double l, double r) { return uniform_real_distribution<>(l, r)(Rand); }

void calc(int x, int y) {

}

void SA(double T, const double ET, const double delta) {
    for (int i = 1; i <= n; ++i) calc(i, i); 
    while (T > ET) {
        int x = rndint(1, n), y = rndint(1, n); 
        i64 res = ans; 
        int px = p[x], py = p[y]; 
        calc(x, py); calc(y, px); 
        if (res > ans || exp((ans - res) / T) <= rnddb(0, 1)) ; 
        else ans = res, swap(p[x], p[y]); 
        T *= delta; 
    }
    for (int i = 1; i <= n; ++i) printf("%d ", p[i]);
    putchar('\n');  
}

int main(void) {
    scanf("%d", &n); 
    for (int i = 1; i <= n; ++i) scanf("%d", a + i); 
    SA(1e15, -1e15, 0.999999); 
    return 0; 
}