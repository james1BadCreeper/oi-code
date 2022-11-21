#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;
const int MOD = 45989;

int N;
struct Matrix {
    int a[125][125];
    Matrix() { memset(a, 0, sizeof(a)); }
    void build(void) { for (int i = 0; i < N; ++i) a[i][i] = 1; }
    Matrix operator < (const Matrix &b) const {
        Matrix c;
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k) {
                int r = a[i][k];
                for (int j = 0; j < N; ++j)    
                    c.a[i][j] = (c.a[i][j] + r * b.a[k][j]) % MOD;
            }
        return c;
    }
};

Matrix poww(Matrix a, int b) {
    Matrix res; res.build();
    for (; b; b >>= 1, a = a * a) 
        if (b & 1) res = res * a;
    return res;
}

struct edge { int u, v; };
int n, m, t, a, b;
vector<edge> edges;
vector<int> G[55];
inline void addedge(int u, int v) {
    edges.emplace_back({u, v});
    G[u].emplace_back(edges.size() - 1);
}

int main(void)
{
    scanf("%d%d%d%d%d", &n, &m, &t, &a, &b);
    edges.push_back(-1);
    addedge(0, a);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v); addedge(v, u);
    }
    Matrix f;
    for (int i = 0; i < edges.size(); ++i) {
        int u = edges[i].u;
        for (int j = 0; j < G[u].size(); ++j) {
            if (i == (j ^ 1)) continue;
            f.a[i][j] = 1;   
        }
    }
    f = poww(f, t);
    int ans = 0;
    for (int i = 0; i < edges.size(); ++i)
        if (edges[i].v == b) ans = (ans + f.a[1][i]) % MOD;
    printf("%d\n", ans);
    return 0;
}