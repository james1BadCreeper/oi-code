#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m, s, p;
int low[500005], dfn[500005], num = 0;
int st[500005], tot = 0, f[500005];
int val[500005], sum[500005], c[500005], cnt = 0;
bool ins[500005];
vector<int> G[500005];

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    ins[st[++tot] = x] = true;
    for (int y : G[x])
        if (!dfn[y]) { tarjan(y); low[x] = min(low[x], low[y]); }
        else if (ins[y]) low[x] = min(low[x], dfn[y]);
    if (low[x] == dfn[x]) {
        int y; ++cnt;
        do {
            y = st[tot--]; ins[y] = false;
            c[y] = cnt; sum[cnt] += val[y];
        } while (x != y);
    }
}

int in[500005];
vector<int> G2[500005];

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].emplace_back(v);
    }
    for (int i = 1; i <= n; ++i) scanf("%d", val + i);
    scanf("%d%d", &s, &p);
    tarjan(s);
    for (int i = 1; i <= n; ++i)
        if (!c[i]) for (int j : G[i])
            if (c[i] != c[j]) {
                G2[i].push_back(c[i], c[j]);
                ++in[c[j]];
            }
    queue<int> q;
    q.push(c[s]); f[c[s]] = sum[c[s]];
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : G2[u].size()) {
            f[v] = max(f[v], f[u] + sum[v]);
            --in[v];
            if (in[v] == 0) q.push(v);
        }
    }
    int ans = 0;
    for (int i = 1; i <= p; ++i) {
        int x;
        scanf("%d", &x);
        ans = max(ans, f[c[x]]);
    }
    printf("%d\n", ans);
    return 0;
}