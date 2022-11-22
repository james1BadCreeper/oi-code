#include <iostream>
#include <cstdio>
#include <vector>
#include <bitset>

using namespace std;

int n, m;
int st[100005], tot = 0;
bool ins[100005];
int dfn[100005], low[100005], num = 0;
int cnt = 0, c[100005];
vector <int> G[100005];
bool flag[100005];

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    ins[st[++tot] = x] = true;
    for (int y : G[x])
        if (!dfn[y]) {
            tarjan(y);
            low[x] = min(low[x], low[y]);
        } else if (ins[y]) low[x] = min(low[x], dfn[y]);
    if (dfn[x] == low[x]) {
        ++cnt; int y;
        do {
            y = st[tot--]; ins[y] = false;
            c[y] = cnt; 
        } while (x != y);
    }
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i);
    for (int i = 1; i <= n; ++i) 
        for (int j : G[i])
            if (c[i] != c[j]) flag[c[j]] = true;
    int ans = 0;
    for (int i = 1; i <= cnt; ++i)
        ans += (flag[i] == 0);
    printf("%d\n", ans);
    return 0;
}