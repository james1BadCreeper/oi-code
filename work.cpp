#include <iostream>
#include <cstdio>
#include <algorithm>
#define rep(i, l, r) for (int i = l; i <= r; ++i)

using namespace std;
inline int F(int x) { return x * x; }

int n, a, b;
int w[55], l[55];
int f[55][55][55][55], g[55][55];

int main(void)
{
    scanf("%d%d%d", &n, &a, &b);
    for (int i = 1; i <= n; ++i) 
        scanf("%d", w + i), l[i] = w[i];
    sort(l + 1, l + n + 1);
    int m = unique(l + 1, l + n + 1) - (l + 1);
    for (int i = 1; i <= n; ++i)
        w[i] = lower_bound(l + 1, l + m + 1) - l;
    memset(f, 0x3f, sizeof(f));
    memset(g, 0x3f, sizeof(g));
    for (int i = 1; i <= n; ++i)
        f[i][i][a[i]][a[i]] = 0, g[i][i] = a;
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            rep(x, 1, m) rep(y, x, m) {
                f[i][j][min(x, a[r])][max(y, a[r])] = f[i][j - 1][x][y];
                for (int k = l; k < r; ++k)
                    f[i][j][x][y] = min(f[i][j][x][y], f[i][k][x][y] + g[k + 1][r]);
            }
            rep(x, 1, m) rep(y, x, m)
                g[i][j] = min(g[i][j], f[i][j][x][y] + a + b * F(l[y] - l[x]));
        }
    printf("%d\n", g[1][n]);
    return 0;
}