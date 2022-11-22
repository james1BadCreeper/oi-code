---
title: 生成树问题
plugins:
  - katex
cover: false
headimg: https://pic1.imgdb.cn/item/6348451c16f2c2beb1c328d9.jpg
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 图论
    - 生成树
    - 笔记
abbrlink: 9acb5cb3
date: 2022-10-14 16:55:16
hide: true
group: notes
status: working
---

> 在无向图中，生成树指一棵由全部顶点和组成的树，而当中边权之和最小的生成树称为最小生成树（Minimum Spanning Tree，MST）。本文会引导你学习 MST 的 Kruskal 和 Prim 算法，以及生成树问题的一些变种和扩展应用。

<!--more-->

## 最小生成树

常见的求解 MST 的方法有两种：Kruskal 和 Prim。   
[Portal](https://www.luogu.com.cn/problem/P3366).

### Kruskal

Kruskal 基于贪心的思想。Kruskal 先把 $m$ 条边进行排序，然后检查每条边 $u,v$，如果 $u$ 和 $v$ 在同一个连通分量中，那么加入后就会形成环，不能加入。若不在呢？那就直接加入，一定是最优的。证明可以使用反证法，这里略去。

实现上，排序直接用 `sort`，维护的过程可以采用并查集，参考代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

class UnionFind
{
    private:
        int bin[5005];
    public:
        inline void init(int n) {
            n += 2;
            for (int i = 1; i <= n; ++i)
                bin[i] = i;
        }
        int find(int x) {
            if (bin[x] == x) return x;
            return bin[x] = find(bin[x]);
        }
        inline void uni(int x, int y) {
            bin[find(x)] = find(y);
        }
        inline bool ask(int x, int y) {
            return find(x) == find(y);
        }
}U;

struct edge
{
    int u, v, d;
    edge(int from = 0, int to = 0, int dist = 0)
    {
        u = from;
        v = to;
        d = dist;
    }
    inline bool operator < (const edge a) const {
        return d < a.d;
    }
}G[200005];

int n, m;

inline void kruskal(void)
{
    sort(G + 1, G + m + 1);
    int ans = 0, now = 0;
    for (int i = 1; i <= m; ++i)
    {
        int a = U.find(G[i].u), b = U.find(G[i].v);
        if (a == b) continue;
        ans += G[i].d;
        U.uni(a, b);
        ++now;
        if (now == n-1) break;
    }
    if (now != n-1) puts("orz"); // 如果都遍历了一遍还是不连通，输出 orz
    else printf("%d\n", ans);
}

int main(void)
{
    n = read(), m = read();
    U.init(n);
    for (int i = 1; i <= m; ++i)
        G[i].u = read(), G[i].v = read(), G[i].d = read();
    kruskal();
    return 0;
}
```

时间复杂度 $\mathcal{O}(m\log n+n\log n)$（一般认为 $m > n$，所以写作 $\mathcal{O}(m\log n)$。

### Prim

Prim 同样基于贪心的思想，读者应该了解过 [Dijkstra](/2022/6bbbc5e9/#Dijkstra) 算法，Prim 和 Dijkstra 大概就是相同的原理。

Prim 算法维护的是 MST 的一部分。最初，Prim 确定 $1$ 号节点属于 MST（即将 $1$ 作为根节点）。  

设确定属于 MST 的点集为 $T$，未确定为 $S$。Prim 会找到边权最小的边 $(u,v),u\in T,v\in S$，然后将这条边加入 MST。

实现时开一个 $d$ 数组，当 $i\in S$ 时，$d[i]$ 代表与集合 $T$ 中节点之间权值最小的边的权值，最终答案就是 $\sum d$。

发没发现这一过程很像 Dijkstra？的确如此，代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m;
int G[5005][5005];
int d[5005];
bool v[5005];

inline void prim(void)
{
    memset(d, 0x3f, sizeof(d)); 
    d[1] = 0;
    for (int op = 1; op < n; ++op) // 进行 n - 1 轮
    {
        int x = 0;
        for (int i = 1; i <= n; ++i)
            if (v[i] == 0 && (x == 0 || d[i] < d[x])) x = i; // 找最小
        v[x] = 1;
        for (int i = 1; i <= n; ++i)
            if (v[i] == 0) d[i] = min(d[i], G[x][i]); // 更新
    }
}

int main(void)
{
    n = read(), m = read();
    memset(G, 0x3f, sizeof(G));
    for (int i = 1; i <= m; ++i)
    {
        int u = read(), v = read(), d = read();
        G[u][v] = G[v][u] = min(G[u][v], d); // 注意这里
    }   
    prim();
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        if (d[i] == 0x3f3f3f3f) // 不连通
        {
            puts("orz");
            return 0;
        }
        else ans += d[i];
    cout << ans << endl;
    return 0;
}
```

Prim 的复杂度是 $\mathcal{O}(n^2)$，虽然和 Dijkstra 一样可以用优先队列优化到 $\mathcal{O}(m\log n)$，但是这时就不如直接用 Kruskal。所以 Prim 用于稠密图（尤其是完全图）的 MST 求解。

## 其它生成树

生成树问题有一些变种，这里简单介绍一下：

### 最小瓶颈生成树

这类问题形如这样：给出一个带权无向图，求一棵生成树，使得最大边权值尽量小。   

怎么求呢？我们肯定要把所有边都排序，然后求解。等等，这不就是 Kruskal 算法吗？的确如此。原图的最小生成树就一定是最小瓶颈生成树（但要注意最小瓶颈生成树不一定是最小生成树）。

### 最小瓶颈路

求带权无向图 $u$ 和 $v$ 的一条路径，使得这条路径上的最大边权值最小。   

怎么做呢？可以使用二分 + BFS 来解决，但效率较低。可以求原图的 MST，然后所有路径必定在 MST 上（使用 DFS）。为什么可以这么做呢？可以用反证法，会证明出这样一个结论：如果存在一条路径不在 MST 上，那么这个 MST 一定是假的。   

这个方法还可以求任意节点的最小瓶颈路。先把 MST 转为有根树。设 $f(u,v)$ 为 $u$ 到 $v$ 的最小瓶颈路，从树根开始遍历，有 $f(x,u)=\max\{f(x,v),d(u,v)\}$，其中 $d(u,v)$ 指边 $(u,v)$ 的权值，$v$ 是 $u$ 的父亲。总时间复杂度为 $\mathcal{O}(n^2)$。

### 增量最小生成树

给定一个有 $n$ 个节点的空图，依次给定 $m$ 条边，求 $m$ 个图的最小生成树（不连通输出 `-1`）。

若每来一条边都计算一次最小生成树，排序采用二分插入的方式，


### 次小生成树

[模板](https://www.luogu.com.cn/problem/P4180)。

这里只讨论严格次小生成树，非严格的同理。

由于 Kruskal 算法的过程，可以证明次小生成树只和最小生成树有一条边差距。接下来考虑如何替换：

设最小生成树为 $T$，权值和为 $S$，那么遍历每一条边，加入一条边后树上会出现一个环，再断掉这个环中边权最大的边（若加入的也是最大的，那么需要断掉次大的，由于原来已经是 MST，显然加入的只能大于等于），对上述所有生成的答案取 $\min$ 之后就可以得到答案。

现在的问题就是，如何高校维护 $u,v$ 路径上的最大值呢？

采用树上倍增法，类似于 ST 表，存储每个点向上 $2^i$ 条边的最大值与次大值，查询的时候倍增查询即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;
typedef long long i64;
const int INF = 1e9;
const i64 INF64 = 2e18;

struct edge {
	int u, v, w;
	bool use;
	bool operator < (const edge &a) const {
		return w < a.w;
	}
} e[300005];

int n, m;
int bin[100005];
int find(int x) {
	if (bin[x] == x) return x;
	return bin[x] = find(bin[x]);
}

#define pii pair<int, int>
vector<pii> G[100005];

i64 ans0 = 0;
void Kruskal(void) 
{
	for (int i = 1; i <= n; ++i) bin[i] = i;
	sort(e + 1, e + m + 1);
    int tot = 0;
	for (int i = 1; i <= m; ++i) {
		int u = find(e[i].u), v = find(e[i].v);
		if (u != v) {
		    ans0 += e[i].w; bin[u] = v; e[i].use = true;
		    G[e[i].u].push_back({e[i].v, e[i].w});
		    G[e[i].v].push_back({e[i].u, e[i].w});
            ++tot;
        }
        if (tot == n - 1) break;
	}
}

int dep[100005], lg[100005];
int f[100005][20];
int mx[100005][20], mx2[100005][20];
void dfs(int x, int fa)
{
	dep[x] = dep[fa] + 1; f[x][0] = fa; mx2[x][0] = -INF;
	for (int i = 1; (1 << i) <= dep[x]; ++i) {
		f[x][i] = f[f[x][i - 1]][i - 1];
		int g[4] = {mx[x][i - 1], mx[f[x][i - 1]][i - 1], mx2[x][i - 1], mx2[f[x][i - 1]][i - 1]};
		sort(g, g + 4);
		mx[x][i] = g[3];
		int p = 2;
		while (p >= 0 && g[p] == g[3]) --p;
		mx2[x][i] = (p == -1 ? -INF : g[p]);
	}
	for (int i = 0; i < G[x].size(); ++i) {
		int y = G[x][i].first, w = G[x][i].second;
		if (y != fa) {
            mx[y][0] = w; 
            dfs(y, x);
        }
	}
}
int LCA(int x, int y) {
	if (dep[x] < dep[y]) swap(x, y);
	for (int i = lg[n]; i >= 0; --i)
		if (dep[f[x][i]] >= dep[y]) x = f[x][i];
	if (x == y) return x;
	for (int i = lg[n]; i >= 0; --i)
		if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
	return f[x][0];
}

i64 calc(int x, int y, int val)
{
	int res = -INF;
	for (int i = lg[n]; i >= 0; --i)
		if (dep[f[x][i]] >= dep[y]) {
            if (val != mx[x][i]) res = max(res, mx[x][i]);
            else res = max(res, mx2[x][i]);
            x = f[x][i];
        }
    return res;
}

int main(void)
{
    ios::sync_with_stdio(false);
	scanf("%d%d", &n, &m);
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= m; ++i) scanf("%d%d%d", &e[i].u, &e[i].v, &e[i].w);
	Kruskal();
	dfs(1, 0);
	i64 ans = INF64;
	for (int i = 1; i <= m; ++i)
		if (!e[i].use) {
            int lca = LCA(e[i].u, e[i].v);
            i64 tmpa = calc(e[i].u, lca, e[i].w);
            i64 tmpb = calc(e[i].v, lca, e[i].w);
            ans = min(ans, ans0 - max(tmpa, tmpb) + e[i].w);
        }
	if (ans != INF64) printf("%lld\n", ans);
    else puts("-1");
	return 0;
}
```
{% endfolding %}

{% divider rail %}

有向图中有一类生成树称为最小树形图。这个问题比较复杂，不在本文中讨论。感兴趣的同学可以自行了解。   

还有一类问题称为 k 小生成树，但是这种问题的做法笔者暂时没有了解。据闻在大神刘汝佳的《算法艺术与信息学竞赛》P300 中有说明，感兴趣的读者可以自行挑战{% emoji 滑稽 %}

## Kruskal 重构树

得益于 Kruskal 的美妙性质，Kruskal 重构树是解决一些复杂问题的强有力武器。

### 定义

在求解 Kruskal 的时候，我们会从小到大加入若干条边。开始时我们建立 $n$ 个集合，每个集合恰好有一个节点，点权为 $0$。

每次加边会合并两个集合，这时候我们新建一个节点，点权为加入的边的边权，同时将两个集合的根节点分别设为新建节点的左右儿子，这时这两个集合和新建点会并为一个集合，新建点为根。

当 Kruskal 完成时，我们就得到了一棵恰好包含 $n$ 个**叶子**的二叉树（总节点数为 $2n-1$），同时每个非叶子节点恰好有两个儿子，得到的这棵树就叫做 Kruskal 重构树。

### 性质

不难发现，

## Problemset

生成树的题目多种多样，其中还有一些经典的套路。这里是一些比较有趣的题目，它们有难有易，读者可以自行取用。

### 最小生成树

这是最基本的生成树问题。

#### [Luogu P1195] 口袋的天空

[Portal](https://www.luogu.com.cn/problem/P1195).

这个 $K$ 是什么？不要紧，我们还是使用 Kruskal 算法，不过不一定要连成一棵树，我们只要把这些云连成 $K$ 个即可。也就是说，只需要连 $N-K$ 条边。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, m, k;

struct edge
{
    int u, v, d;
    edge(int u = 0, int v = 0, int d = 0) :
        u(u), v(v), d(d) {}
    bool operator < (const edge &a) const
    {
        return d < a.d;
    }
}a[10005];

int fa[1005];
int find(int x)
{
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

int main(void)
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; ++i) fa[i] = i;
    for (int i = 1; i <= m; ++i)
        scanf("%d%d%d", &a[i].u, &a[i].v, &a[i].d);
    sort(a + 1, a + m + 1);
    int ans = 0, cnt = 0;
    for (int i = 1; i <= m; ++i)
    {
        int x = find(a[i].u), y = find(a[i].v);
        if (x == y) continue;
        fa[x] = y, ans += a[i].d, ++cnt;
        if (cnt == n - k)
        {
            printf("%d\n", ans);
            return 0;    
        }
    }   
    puts("No Answer");
    return 0;
}
```
{% endfolding %}

#### \[UVa 1395\] Slim Span

[Portal](https://www.luogu.com.cn/problem/UVA1395).

> 给定一个 $n(n\le 100)$ 个点的无向图，求最大边减最小边的值尽量小的生成树。

如果最小边确定，我们求出最小生成树，那么就可以求出这个值了。代码如下：

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int fa[105];
int find(int x) { return x == fa[x] ? x : fa[x] = find(fa[x]); }

struct edge
{
    int u, v, d;
    edge(int u, int v, int d) :
        u(u), v(v), d(d) {}
    inline bool operator < (const edge &a) const
    {
        return d < a.d;
    }
};
vector <edge> e;

int n, m;

inline int kruskal(void)
{
    sort(e.begin(), e.end());
    int ans = 0x7fffffff;
    for (int L = 0; L < m; ++L)
    {
        for (int i = 1; i <= n; ++i) fa[i] = i;
        int cnt = 0;
        for (int R = L; R < m; ++R)
        {
            int a = find(e[R].u), b = find(e[R].v);
            if (a == b) continue;
            fa[a] = b;
            if (++cnt == n - 1)
            {
                ans = min(ans, e[R].d - e[L].d);
                break;
            }
        }
    }
    if (ans == 0x7fffffff) return -1;
    return ans;
}

int main(void)
{
    while (scanf("%d%d", &n, &m) == 2 && n)
    {
        e.clear();
        for (int i = 0; i < m; ++i) 
        {
            int u = read(), v = read(), d = read();
            e.push_back(edge(u, v, d));
        }
        printf("%d\n", kruskal());
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P2700] 逐个击破

[Portal](https://www.luogu.com.cn/problem/P2700).

我们现假设需要摧毁所有的边，然后按边权从大到小排序。如果两个点都不是敌人节点就连边，注意父亲也要设置为敌人节点（如果连接的点有敌人）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

struct edge
{
    int u, v, d;
    edge(int u = 0, int v = 0, int d = 0) :
        u(u), v(v), d(d) {}
    bool operator < (const edge &a) const
    {
        return d > a.d;
    }
}a[100005];

int n, k, s[100005];
int fa[100005];

int find(int x)
{
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

int main(void)
{
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= k; ++i)
    {
        int x;
        scanf("%d", &x);
        s[x] = true;
    }
    long long ans = 0;
    for (int i = 1; i < n; ++i) 
    {
        scanf("%d%d%d", &a[i].u, &a[i].v, &a[i].d);
        ans += a[i].d;
    }
    for (int i = 1; i <= n; ++i) fa[i] = i;
    sort(a + 1, a + n + 1);
    for (int i = 1; i < n; ++i)
    {
        int x = find(a[i].u), y = find(a[i].v);
        if (s[x] && s[y]) continue;
        fa[x] = y;
        ans -= a[i].d;
        s[y] = (s[x] | s[y]);
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [USACO08OCT] Watering Hole G

[Portal](https://www.luogu.com.cn/problem/P1550).

我们只需要增设一个水井点 $0$，让每一个牧场都与 $0$ 连一条 $W_i$ 的边，然后使用 Prim（因为是完全图，开大数据范围即可杀死 Kruskal）求解最小生成树。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, d[305];
bool v[305];
int a[305][305];

void Prim(void)
{
    memset(d, 0x3f, sizeof(d));
    d[0] = 0;
    for (int i = 1; i <= n; ++i)
    {
        int x = -1;
        for (int j = 0; j <= n; ++j)
            if (!v[j] && (x == -1 || d[j] < d[x])) x = j;
        v[x] = true;
        for (int j = 0; j <= n; ++j)
            if (!v[j]) d[j] = min(d[j], a[x][j]);
    }
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        int w;
        scanf("%d", &w);
        a[0][i] = a[i][0] = w;
    }
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            scanf("%d", &a[i][j]);
    Prim();
    int ans = 0;
    for (int i = 0; i <= n; ++i)
        ans += d[i];
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [UVa 1151] Buy or Build

[Portal](https://www.luogu.com.cn/problem/UVA1151).
 
通过二维枚举，我们可以轻松的把这玩意转化成图。对着图使用 Kruskal，得到 $n-1$ 条边，就是可能成为最终答案的边。然后枚举购买哪些套餐即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int read(void)
{
    int x = 0, c = getchar(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar();
    return x * f;
}

int n, q, cost[8];
vector <int> sub[8];
int x[1005], y[1005];

struct edge
{
    int u, v, d;
    edge(int u = 0, int v = 0, int d = 0) :
        u(u), v(v), d(d) {}
    bool operator < (const edge &a) const
    {
        return d < a.d;
    }
};
vector <edge> e, es;

int fa[1005];
int find(int x)
{
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}
inline void UnionFind_init(void)
{
    for (int i = 1; i <= n; ++i)
        fa[i] = i;
}

int kruskal(int cnt, const vector <edge> &G, bool flag)
{
    if (cnt == 1) return 0;
    int ans = 0;
    for (int i = 0; i < G.size(); ++i)
    {
        int x = find(G[i].u), y = find(G[i].v);
        if (x == y) continue;
        fa[x] = y;
        ans += G[i].d;
        if (flag) es.push_back(G[i]);
        --cnt;
        if (cnt == 1) break;
    }
    return ans;
}

int main(void)
{
    int T = read();
    while (T--)
    {
        n = read(), q = read();
        for (int i = 0; i < q; ++i)
        {
            int m = read(); cost[i] = read();
            sub[i].clear();
            while (m--) sub[i].push_back(read());
        }
        for (int i = 1; i <= n; ++i)
            x[i] = read(), y[i] = read();
        e.clear(), es.clear();
        for (int i = 1; i < n; ++i)
            for (int j = i + 1; j <= n; ++j)
                e.push_back(edge(i, j, (x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j])));
        sort(e.begin(), e.end());

        UnionFind_init();
        int ans = kruskal(n, e, true);
        for (int i = 0; i < (1 << q); ++i)
        {
            UnionFind_init();
            int cnt = n, c = 0;
            for (int j = 0; j < q; ++j)
                if (i & (1 << j))
                {
                    c += cost[j];
                    for (int k = 1; k < sub[j].size(); ++k)
                    {
                        int x = find(sub[j][k]), y = find(sub[j][0]);
                        if (x != y) fa[x] = y, --cnt;
                    }
                }
            ans = min(ans, c + kruskal(cnt, es, false));
        }

        printf("%d\n", ans);
        if (T) putchar('\n');
    }   
    return 0;
}
```
{% endfolding %}