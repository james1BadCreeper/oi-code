---
title: YbtOJ 高效进阶 数据结构 第5章 LCA 问题
cover: false
plugins:
  - katex
abbrlink: d2cc35
date: 2022-09-21 21:35:16
categories:
    - 算法竞赛
    - 做题记录
tag:
    - LCA
status: done
sticky: -100
---

> 原来 LCA 是数据结构啊！

<!--more-->

## A. 树上距离

> 两点之间有边权，多次询问树上两点距离。

LCA 板子题。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

struct edge
{
    int v, d;
    edge(int v = 0, int d = 0) :
        v(v), d(d) {}
};
vector <edge> G[10005];
inline void addedge(int u, int v, int d) { G[u].push_back(edge(v, d)); }

int n, m, lg[10005];
int dep[10005], f[10005][15], w[10005][15];

void dfs(int x, int fa)
{
    f[x][0] = fa, dep[x] = dep[fa] + 1;
    for (int i = 1; i <= lg[n]; ++i)
        f[x][i] = f[f[x][i - 1]][i - 1], w[x][i] = w[x][i - 1] + w[f[x][i - 1]][i - 1];
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i].v, val = G[x][i].d;
        if (y != fa)
        {
            w[y][0] = val;
            dfs(y, x);
        }
    }
}

int LCA(int x, int y)
{
    int ans = 0;
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = lg[n]; i >= 0; --i)
        if (dep[f[x][i]] >= dep[y]) ans += w[x][i], x = f[x][i];
    if (x == y) return ans;
    for (int i = lg[n]; i >= 0; --i)
        if (f[x][i] != f[y][i]) ans += w[x][i] + w[y][i], x = f[x][i], y = f[y][i];
    return ans + w[x][0] + w[y][0];
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 1, u, v, d; i < n; ++i)
    {
        scanf("%d%d%d", &u, &v, &d);
        addedge(u, v, d), addedge(v, u, d);
    }
    dfs(1, 0);
    while (m--)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        printf("%d\n", LCA(x, y));
    }
    return 0;
}
```
{% endfolding %}

## B. 货车运输

[Portal](https://www.luogu.com.cn/problem/P1967).

经典问题。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;
const int INF = 1e9;

struct edge
{
    int u, v, d;
    edge(int u = 0, int v = 0, int d = 0) : u(u), v(v), d(d) {}
    bool operator < (const edge &a) const { return d > a.d; }
}a[50005];

int n, m, q;
int bin[10005];

int find(int x)
{
    if (bin[x] == x) return x;
    return bin[x] = find(bin[x]); 
}

vector <edge> G[10005];
inline void addedge(int u, int v, int d) { G[u].push_back(edge(u, v, d)); }

void Kruskal(void)
{
    sort(a + 1, a + m + 1);
    for (int i = 1; i <= n; ++i) bin[i] = i;
    for (int i = 1; i <= m; ++i)
    {
        int x = find(a[i].u), y = find(a[i].v);
        if (x == y) continue;
        bin[x] = y;
        addedge(a[i].u, a[i].v, a[i].d);
        addedge(a[i].v, a[i].u, a[i].d);
    }
}

bool v[10005];
int dep[10005], lg[10005];
int f[10005][15], w[10005][15];
void dfs(int x, int fa)
{
    v[x] = true;
    dep[x] = dep[fa] + 1;
    f[x][0] = fa;
    for (int i = 1; i <= lg[n]; ++i)
        f[x][i] = f[f[x][i - 1]][i - 1], w[x][i] = min(w[x][i - 1], w[f[x][i - 1]][i - 1]);
    for (int i = 0; i < G[x].size(); ++i)
        if (G[x][i].v != fa)
        {
            w[G[x][i].v][0] = G[x][i].d;
            dfs(G[x][i].v, x);
        }
}

int LCA(int x, int y)
{
    if (find(x) != find(y)) return -1;
    if (dep[x] < dep[y]) swap(x, y);
    int ans = INF;
    for (int i = lg[n]; i >= 0; --i)
        if (dep[f[x][i]] >= dep[y]) ans = min(ans, w[x][i]), x = f[x][i];
    if (x == y) return ans;
    for (int i = lg[n]; i >= 0; --i)
        if (f[x][i] != f[y][i]) ans = min({ans, w[x][i], w[y][i]}), x = f[x][i], y = f[y][i];
    return min({ans, w[x][0], w[y][0]});
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; ++i)
        scanf("%d%d%d", &a[i].u, &a[i].v, &a[i].d);
    Kruskal();
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 1; i <= n; ++i)
        if (!v[i]) dfs(i, 0);
    scanf("%d", &q);
    while (q--)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        printf("%d\n", LCA(x, y));
    }
    return 0;
}
```
{% endfolding %}

## C. 运输计划

[Portal](https://www.luogu.com.cn/problem/P2680).

经典问题。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

struct edge
{
    int v, d;
    edge(int v = 0, int d = 0) :
        v(v), d(d) {}
};

int n, m, lg[300005];
int dep[300005], f[300005][20], dis[300005];
int s[300005], t[300005], lca[300005], dist[300005], C[300005];
int maxlen = 0, sum = 0, val[300005], ans;
vector <edge> G[300005];

void dfs(int x, int fa)
{
    dep[x] = dep[fa] + 1, f[x][0] = fa;
    for (int i = 1; i <= lg[n]; ++i)
        f[x][i] = f[f[x][i - 1]][i - 1];
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i].v, w = G[x][i].d;
        if (y == fa) continue;
        dis[y] = dis[x] + w, val[y] = w;
        dfs(y, x);
    }
}

int LCA(int x, int y)
{
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = lg[n]; i >= 0; --i)
        if (dep[f[x][i]] >= dep[y]) x = f[x][i];
    if (x == y) return x;
    for (int i = lg[n]; i >= 0; --i)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}


void get(int x, int fa)
{
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i].v;
        if (y == fa) continue;
        get(y, x);
        C[x] += C[y];
    }
}

bool P(int x) // 所有运输计划都不超过 x
{
    int cnt = 0, maxdis = 0;
    memset(C, 0, sizeof(C));
    for (int i = 1; i <= m; ++i)
        if (dist[i] > x)
        {
            C[s[i]]++, C[t[i]]++, C[lca[i]] -= 2;
            ++cnt;
        }
    if (cnt == 0) return true;
    get(1, 0);
    for (int i = 1; i <= n; ++i)
        if (C[i] == cnt) maxdis = max(maxdis, val[i]);
    if (maxlen - maxdis <= x) return true;
    return false;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1, u, v, d; i < n; ++i)
    {
        scanf("%d%d%d", &u, &v, &d);
        G[u].push_back(edge(v, d)), G[v].push_back(edge(u, d));
    }
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    dfs(1, 0);
    for (int i = 1; i <= m; ++i) 
    {
        scanf("%d%d", s + i, t + i);
        lca[i] = LCA(s[i], t[i]);
        dist[i] = dis[s[i]] + dis[t[i]] - 2 * dis[lca[i]];
        sum += dist[i];
        maxlen = max(maxlen, dist[i]);
    }
    int L = -1, R = 300000005;
    while (L + 1 != R)
    {
        int mid = L + R >> 1;
        if (P(mid)) R = mid;
        else L = mid;
    }
    printf("%d\n", R);
    return 0;
}

```
{% endfolding %}

## E. 祖孙询问

板子。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, m, root;
int f[40005], dfn[40005], num = 0;
int dep[40005], top[40005], son[40005], siz[40005];
vector <int> G[40005];

void dfs1(int x, int fa)
{
    f[x] = fa; dep[x] = dep[fa] + 1; siz[x] = 1;
    int max_part = -1;
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i];
        if (y != fa)
        {
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > max_part)
            {
                son[x] = y;
                max_part = siz[y];
            }
        }
    } 
}

void dfs2(int x, int topf)
{
    dfn[x] = ++num; top[x] = topf;
    if (son[x] == -1) return;
    dfs2(son[x], topf);
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i];
        if (y != f[x] && y != son[x]) dfs2(y, y);
    }
}

int LCA(int x, int y)
{
    while (top[x] != top[y])
    {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        x = f[top[x]];
    }   
    return dep[x] < dep[y] ? x : y;
}

int main(void)
{
    memset(son, 0xff, sizeof(son));
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        if (a == -1 || b == -1) root = max(a, b);
        else G[a].push_back(b), G[b].push_back(a);
    }
    dfs1(root, 0);
    dfs2(root, root);
    scanf("%d", &m);
    while (m--)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        int lca = LCA(x, y);
        if (x == lca) puts("1");
        else if (y == lca) puts("2");
        else puts("0");
    }
    return 0;
}
```
{% endfolding %}

## 小结

关于为什么折叠框没有颜色：笔者这种东西写的臭代码还是不要上配色了吧，怕太显眼。

垒球天下第一。