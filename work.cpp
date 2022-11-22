#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;
typedef long long i64;

struct edge {
    int v, d;
};

int n, m, siz[2005];
i64 f[2005][2005];
vector<edge> G[2005];

void dp(int x, int fa) 
{
    siz[x] = 1; f[x][0] = f[x][1] = 0;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = G[x][i].v; i64 w = G[x][i].d;
        if (y != fa) {
            dp(y, x); siz[x] += siz[y];
            for (int j = min(m, siz[x]); j >= 0; --j) {
                if (f[x][j] != -1)
                    f[x][j] += f[y][0] + w * siz[y] * (n - m - siz[y]);
                for (int k = min(j, siz[y]); k >= 0; --k) {
                    if (f[x][j - k] == -1) continue;
                    i64 val = 1ll * k * (m - k) + 1ll * (siz[y] - k) * (n - m - (siz[y] - k));
                    f[x][j] = max(f[x][j], f[x][j - k] + f[y][k] + val * w);
                }
            }
        }
    }
}
                
int main(void)
{
    memset(f, -1, sizeof(f));
    scanf("%d%d", &n, &m);
    for (int i = 1; i < n; ++i) {
        int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        G[u].push_back({v, d});
        G[v].push_back({u, d});
    }
    dp(1, 0);
    printf("%lld\n", f[1][m]);
    return 0;
}