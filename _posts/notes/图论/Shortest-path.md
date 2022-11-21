---
title: 最短路问题
headimg: https://s1.ax1x.com/2022/08/04/vm9npV.jpg
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 图论
    - 最短路
    - 差分约束
group: notes
hide: true
abbrlink: 6bbbc5e9
date: 2022-08-01 09:31:35
status: working
---

> 最短路是图论中的常考问题，本文引导你学习最短路的 Floyd，BellmanFord 及 Djikstra 算法，以及它们的各种应用。还有一种基于最短路的系统：差分约束系统。

<!--more-->

{% folding blue::更新日志 %}

{% timeline %}

{% timenode 2022/08/01 %}

将最短路原文章搬移了过来，打算用新的代码风格重写代码，调整题目的顺序，增加例题和习题。

{% endtimenode %}

{% endtimeline %}

{% endfolding %}

## 概念

**松弛**。最短路的核心思想是松弛，即找一个点 C，如果从 A 到 B 的距离比从 A 到 C 再到 B 的距离长，那么更新最短路，这便是松弛操作。

**环**。要认识到一个事实。如果最短路存在，一定是个**不含环的最短路**。因为如果它是正环（边权和为正数），走它是自寻死路；如果它是零环，走它没有意义；如果它是负环，则不存在最短路了（一直转圈刷分，或者说存在长度为 `-INF` 的最短路{% emoji 滑稽 %}，当然，前提是这个负环能走到）。

## 多源最短路

指求任意两点间的最短路的问题，一般使用简单好写的 Floyd 算法（全称为 Floyd-Warshall）。

### Floyd

非常简单，用邻接矩阵存图，初始化边为距离，其它距离为无穷大，跑下面的代码（注意循环顺序！）：

```cpp
for (int k = 1; k <= n; ++k)
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
```

嗯，非常简单，但怎么证明呢{% emoji 滑稽 %}？

好，给点提示，这实际上是个 DP，而且数组的第一维被滚掉了{% emoji 滑稽 %}。

实际上，设 $D[k,i,j]$ 为经过若干编号不超过 $k$ 的节点的 $i$ 到 $j$ 的最短路长度。那么有两种选择：经过编号不超过 $k-1$ 的点，或者从 $i$ 到 $k$ 再到 $j$，也就是 $D[k,i,j]=\min\{D[k-1,i,j], D[k-1,i,k]+D[k-1,k,j]\}$

改变松弛操作，就可以求不同的路径。值得一提的是，如果求图的连通性，那么这一问题被称为**传递闭包**。代码是 `d[i][j] |= (d[i][k] & d[k][j])`。

Floyd 的时间复杂度是 $\Theta(n^3)$。

有的时候你会在网上看到一种判断 `k, i, j` 是否不相等的代码，但是没有必要。因为即使其中有变量相等，它的结果也会是正确的。

说个有趣的事情，即使循环顺序错误，只要连续跑三遍 Floyd，它的结果也是正确的{% emoji 滑稽 %}。

### Johnson

一种特殊的多源最短路算法，可以做到 $O(nm \log m)$ 的复杂度来求解（Dijkstra 的复杂度是 $O(m\log m)$，但实用性不高，感兴趣的读者可以自己了解学习。

### Floyd 的题目

我们来几道简单题。

#### [Luogu 1119] 灾后重建

[Portal](https://www.luogu.com.cn/problem/P1119).

直接使用 Floyd 算法。因为我们知道 Floyd 是个以“到达点的编号”为阶段的 DP 算法，这跟题目的要求正好符合。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar();
    return x;
}

int n, m;
int d[205][205];
int t[205];

int main(void) {
    n = read(), m = read();
    memset(d, 0x3f, sizeof(d));
    for (int i = 0; i < n; ++i)  {
        t[i] = read();
        d[i][i] = 0;
    }
    while (m--) {
        int x = read(), y = read(), v = read();
        d[x][y] = v;
        d[y][x] = v;
    }
    int q = read(), k = 0;
    while (q--) {
        int x = read(), y = read(), T = read();
        for (; t[k] <= T && k < n; ++k) // 核心
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        if (t[x] > T || t[y] > T || d[x][y] == 0x3f3f3f3f) puts("-1"); // 可能这个村庄依旧处于报废
        else printf("%d\n", d[x][y]);
    }
    return 0;
}
```
{% endfolding %}

时间复杂度 $O(n^3)$。

#### [UVa 10048] Audiophobia

[Portal](https://www.luogu.com.cn/problem/UVA10048)

也可以使用 Floyd。我们求的还是最短路，路径中的加法变成了 max 而已。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int c, s, q, kase = 0;
int d[105][105];

int main(void) {
    while (scanf("%d%d%d", &c, &s, &q) == 3 && c) {
        if (kase) putchar('\n');
        printf("Case #%d\n", ++kase);
        memset(d, 0x7f, sizeof(d));
        while (s--) {
            int x, y, val;
            scanf("%d%d%d", &x, &y, &val);
            d[x][y] = min(d[x][y], val);
            d[y][x] = min(d[y][x], val);
        }
        for (int i = 1; i <= c; ++i) d[i][i] = 0;
        for (int k = 1; k <= c; ++k)
            for (int i = 1; i <= c; ++i)
                for (int j = 1; j <= c; ++j)
                    d[i][j] = min(d[i][j], max(d[i][k], d[k][j]));
        while (q--) {
            int x, y;
            scanf("%d%d", &x, &y);
            if (d[x][y] == 0x7f7f7f7f) puts("no path");
            else printf("%d\n", d[x][y]);
        }
    }
    return 0;
}
```
{% endfolding %}

#### [UVa 247] Calling Circles

[Portal](https://www.luogu.com.cn/problem/UVA247).

用 Floyd 求出传递闭包即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

int n, m, kase;
vector <string> names;
bool f[30][30], vis[30];
char a[100], b[100];

int ID(const string &s) {
    for (int i = 0; i < names.size(); ++i)
        if (names[i] == s) return i;
    names.push_back(s);
    return names.size() - 1;
}

void dfs(int u) {
    vis[u] = true;
    for (int v = 0; v < n; ++v)
        if (!vis[v] && f[u][v] && f[v][u]) {
            cout << ", " << names[v];
            dfs(v);
        }
}

int main(void) {
    while (scanf("%d%d", &n, &m) == 2 && n) {
        names.clear();
        memset(f, 0, sizeof(f));
        for (int i = 0; i < n; ++i) f[i][i] = 1;

        while (m--) {
            scanf("%s%s", a, b);
            f[ID(a)][ID(b)] = 1;
        }

        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    f[i][j] |= f[i][k] & f[k][j];

        if (kase) putchar('\n');
        printf("Calling circles for data set %d:\n", ++kase);

        memset(vis, 0, sizeof(vis));
        for (int i = 0; i < n; ++i)
            if (!vis[i]) {
                cout << names[i];
                dfs(i);
                putchar('\n');
            }
    }
    return 0;
}
```
{% endfolding %}

### 负环

Floyd 可以判断是否有负环。如果从一个点绕了一圈回到了自己，正常情况下 $f(i,i)=0$，但如果跑了一个负环，它就会变成负数。

[[USACO06DEC] Wormholes G](https://www.luogu.com.cn/problem/P2850).

使用 Floyd 判断负环即可，需要吸氧（不吸氧需要使用后文介绍的 SPFA）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int read(void) {
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int n, m, w;
int f[505][505];

int main(void) {
    int T = read();
    while (T--) {
        memset(f, 0x3f, sizeof(f));
        n = read(), m = read(), w = read();
        while (m--) {
            int x = read(), y = read(), v = read();
            f[x][y] = min(f[x][y], v);
            f[y][x] = min(f[y][x], v);
        }
        while (w--) {
            int x = read(), y = read();
            f[x][y] = min(f[x][y], -read());
        }
        for (int k = 1; k <= n; ++k)
            for (int i = 1; i <= n; ++i)
                for (int j = 1; j <= n; ++j)
                    f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
        bool flag = true;
        for (int i = 1; i <= n; ++i)
            if (f[i][i] < 0) {
                puts("YES");
                flag = false;
                break;
            }
        if (flag) puts("NO");
    }
    return 0;
}
```
{% endfolding %}

### 最小环问题

给定一张图，求图中一个至少包含 $3$ 个点的环，环上的节点不重复，并且环上的边的长度之和最小。该问题称为图的最小环问题。

[模板](https://www.luogu.com.cn/problem/P6175)。虽然这是无向图，但是有向图也是可以用下面的方法做的。

可以发现，最小环的长度就是 $\min\limits_{1\le i<j<k}\{d[i,j]+a[i,k]+a[k,j]\}$，代表经过节点 $k$，由编号不超过 $k$ 的节点构成（$i,j,k$）的顺序是为了保证不重复经过节点。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#define i64 long long

using namespace std;

const i64 INF = 1e18;
int n, m, u, v;
i64 w;
i64 ans = INF;
i64 f[105][105], a[105][105];

int main(void)
{
    scanf("%d%d", &n, &m);
    memset(a, 0x1f, sizeof(a)); memset(f, 0x1f, sizeof(f));
    for (int i = 1; i <= n; ++i)
        a[i][i] = f[i][i] = 0;
    while (m--)
    {
        scanf("%d%d%lld", &u, &v, &w);
        f[u][v] = f[v][u] = a[u][v] = a[v][u] = min(f[u][v], w);
    }
    for (int k = 1; k <= n; k++)
    {
        for (int i = 1; i < k; i++)
            for (int j = i + 1; j < k; j++)
                ans = min(ans, f[i][j] + a[i][k] + a[k][j]);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
    }
    if (ans == INF) puts("No solution.");
    else printf("%lld\n", ans);
    return 0;
}
```

## Bellman-Ford

这是一种 $\Theta(nm)$ 的单源最短路算法，即可以计算出从 $s$ 到任意一点的最短路。

### 原理

基于之前说的有最短路，一定是不含环的。那么我们撑死只要迭代 $n-1$ 次，每次检查所有的边，便会没有更新的内容（环是无意义的），便可以得到正确的最短路，代码如下：

```cpp
memset(dis, 0x3f, sizeof(dis));
dis[s] = 0;
for (int op = 1; op < n; ++op) // 迭代 n-1 次
    for (int i = 0; i < edges.size(); ++i) // 检查所有边
        if (dis[edges[i].u] + edges[i].val < dis[edges[i].v])
            dis[edges[i].v] = dis[edges[i].u] + edges[i].val;
```

### SPFA

但是 Bellman-Ford 也太慢了吧！$\Theta(nm)$ 的复杂度，谁顶得住啊！   
解决方法是使用 SPFA（SPFA 是 Shortest Path Fast Algorithm 的缩写，在中国大陆使用这一称谓。由于这玩意的复杂度是错的，所以在国际上称之为“队列优化的 Bellman-Ford 算法”）。

Bellman-Ford 每次都要将所有点对应的所有边松弛一遍，这当中有很多浪费，因为不是每一条边都要松弛的！   
就是说，如果上一轮这个点的最短路没有发生变化，那么这一轮我们就不用对这一个点进行松弛了。   
那么我们开一个队列，如果节点在队列中，表示最短路发生改变，需要重新计算该点。

[模板](https://www.luogu.com.cn/problem/P3371)。

为方便，这里也给出封装好的数据结构，之后介绍 Dijkstra 时也会使用相同的数据结构，这里使用 vector，因为它很方便，而且在各大竞赛都普遍使用 O2 后，vector 甚至比链式前向星更快，可以看一下 [riteme 的这篇博客](https://riteme.blog.uoj.ac/blog/6692)，经笔者测试开 O2 基本是相同的效果。至于 vector 会开二倍空间，一般不会被卡。

代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

const int INF = (1ll << 31) - 1;

// 边，根据情况可以不记录 from
struct edge {
    int from, to, dist;
    edge(int from = 0, int to = 0, int dist = 0) :
        from(from), to(to), dist(dist) {}
};

int n, m, s;
int d[100005], inq[100005];
vector <edge> G[100005];

inline void addedge(int u, int v, int d) {
    G[u].push_back(edge(u, v, d));
}

void SPFA(void) {
    for (int i = 1; i <= n; ++i) d[i] = INF;
    d[s] = 0;
    inq[s] = 1;
    queue <int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); 
        q.pop(); 
        inq[u] = 0;

        for (int i = 0; i < G[u].size(); ++i) {
            edge &e = G[u][i];
            if (d[e.to] > d[u] + e.dist) {
                d[e.to] = d[u] + e.dist;
                if (!inq[e.to]) { // 被更新了，但是不在队列里
                    inq[e.to] = true; // 加入队列，需要重新计算
                    q.push(e.to);
                }
            }
        }
    }
}

int main(void) {
    scanf("%d%d%d", &n, &m, &s);
    while (m--) {
        int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        if (u != v) addedge(u, v, d);
    }
    SPFA();
    for (int i = 1; i <= n; ++i)
        printf("%d ", d[i]);
    putchar('\n');
    return 0;
}
```

### 干掉 SPFA

{% p center large, R.I.P. %}
{% p center small, SPFA %}
{% p center small, 1994~2018 %}

~~可怜的 SPFA 发明人——段凡丁~~.

{% noteblock bug red %}

2018 年 7 月 19 日，某位同学在 NOIDay1T1 归程 一题里非常熟练地使用了一个广为人知的算法求最短路。

然后呢？   
$100 \rightarrow 60$   
$\text{Ag} \rightarrow \text{Cu}$

最终，他因此没能与理想的大学达成契约。

{% image https://z3.ax1x.com/2021/08/19/f75810.jpg::alt=这是 NOI2018 的真实照片，讲题人公开宣称 SPFA 已死 %}

{% endnoteblock %}

SPFA 的复杂度是 $\Omega(kn)$。其中 $k$ 是一个较小的常数，**但是注意**！这里的时间复杂度符号不对，它的**下界**是较快的，一般在随机图中，SPFA 是最快的最短路算法，但如果构造特殊数据，SPFA 会达到 $O(nm)$ 的上界复杂度，即退化成 Bellman-Ford（~~而且常数更大~~）。

实际上 SPFA 在国际上通称队列优化的 Bellman-Ford 算法，因为段凡丁极其不负责任地搞出了一个错误的证明，给出它的复杂度是 $\Theta(kn)$。

怎么干掉 SPFA 呢？核心思路是：**让节点频繁入队**。可以看一下 [fstqwq 在知乎上的回答](https://www.zhihu.com/question/292283275/answer/484871888)。

### 负环

SPFA 也不是什么用也没有，它可以判负环（相比于 Floyd，它显然更快）。

当然 Bellman-Ford 也可以判断负环，如果 $n$ 轮之后还是有更新的内容，就说明有人在刷分，有负环。

SPFA 有三种方法判断负环。

第一种方法是 $cnt[v]$ 表示从 $s$ 到 $v$ 的最短路经过了多少个点，如果 `u->v` 松弛了 `s->v`，那么就让 $cnt[v]$ 更新为 $cnt[u]+1$。   
之前提到过，一个正常的最短路是不应该有超过 $n$ 个点的，因此当 $cnt[v]>n$ 的时候说明有内鬼，终止交易{% emoji sticker 滑稽喝水 %}。

第二种是这样的：还是说最短路最多经过 $n$ 个点，$cnt[v]$ 表示从 $v$ 入了多少次队，如果一个点超过了 $n$ 次入队，那么就说明还是有内鬼，就是有负环{% emoji tong ... %}！

第三种方式是 `dfs` 版的 SPFA 判环。如果一个点被松弛了，就直接递归进这个点去松弛别人就行了。而如果一个点递归一圈又回到自己了，那显然是有负环的。

一般第二种方法很慢（需要绕着环跑 $n$ 次才能找到负环），而第三种方法比第一种快。但是很少有第一种不够快的时候，而且第三种方法很可能会严重降低负环不存在时的最短路计算效率，还有可能有其它副作用，不建议使用。这里给出第一种方法的代码，时间复杂度为 $O(nm)$。

可以参考[模板](https://www.luogu.com.cn/problem/P3385)。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x * f;
}

struct edge
{
    int from, to, dist;
    edge(int from = 0, int to = 0, int dist = 0) :
        from(from), to(to), dist(dist) {}
};

int n, m;
int d[10005], inq[10005];
vector <edge> G[10005];
int cnt[100005];

inline void addedge(int u, int v, int d)
{
    G[u].push_back(edge(u, v, d));
}

bool SPFA()
{
    memset(d, 0x3f, sizeof(d));
    memset(cnt, 0, sizeof(cnt));
    memset(inq, 0, sizeof(inq));
    d[1] = 0;
    inq[1] = true;
    queue <int> q;
    q.push(1);
    while (!q.empty())
    {
        int u = q.front(); q.pop(); inq[u] = false;

        for (int i = 0; i < G[u].size(); ++i)
        {
            edge &e = G[u][i];
            if (d[e.to] > d[u] + e.dist)
            {
                d[e.to] = d[u] + e.dist;
                cnt[e.to] = cnt[u] + 1;
                if (cnt[e.to] > n) return true;
                if (!inq[e.to])
                {
                    inq[e.to] = 1;
                    q.push(e.to);
                }
            }
        }
    }
    return false;
}

int main(void)
{
    int T = read();
    while (T--)
    {
        n = read(), m = read();
        for (int i = 1; i <= n; ++i)
            G[i].clear();
        while (m--)
        {
            int u = read(), v = read(), w = read();
            addedge(u, v, w); 
            if (w >= 0) addedge(v, u, w);
        }
        puts(SPFA() ? "YES" : "NO");
    }
    return 0;
}
```
{% endfolding %}

要注意的是，如果图中有负环但是起点 $s$ 走不到这个负环，那么无法找到。解决方法是外挂一个节点，跟每一个点都连一条边，然后对这个点跑 SPFA。

## Dijkstra

既然 SPFA 都会被卡，那我们用什么？答案是：Dijkstra。

### 原理与实现

Dijkstra 基于贪心的思想。大致流程如下：

1. 初始化 $d[s]=0$，其余为 INF。   
2. 找出一个没有被访问的，$d[x]$ 最小的 $x$，然后访问 $x$。
3. 扫描 $x$ 所有的出边，进行松弛。
4. 迭代步骤 2~3 $n-1$ 次。

所以说，Dijkstra 的思路就是“我找**当前**最近的路走，一定能找到最短路”。**但这意味着，如果图中存在负权边，那么 Dijkstra 可能会鼠目寸光，忽略了前方还有很小的负权边的可能，所以 Dijkstra** {% span red bold::只可用于正权图！ %}

具体证明请读者自行查找（在 OI-wiki 上有）。

下面是代码：

```cpp
int v[5005], d[5050];
memset(d, 0x3f, sizeof(d));
memset(a, 0x3f, sizeof(a));
read_graph();
memset(v, 0, sizeof(v));
d[1] = 0;
for (int op = 1; op < n; ++op) // 迭代 n-1 次
{
    int x = 0;
    // 找 x
    for (int i = 1; i <= n; ++i)
        if (v[i] == 0 && (x == 0 || d[i] < d[x])) x = i;
    v[x] = 1;
    // 松弛
    for (int i = 1; i <= n; ++i)
        d[i] = min(d[i], d[x] + a[x][i]);
}
```

而且由于 Dijkstra 的贪心思想，使用前必须满足**当前的最优决策就是最终的最优决策**。

### 优化

然而还是很慢，上述代码的复杂度高达 $\Theta(n^2)$，$n = 10^5$ 的时候直接告辞，这也是为什么上述代码采用邻接矩阵的原因。

但是有优化（啊，小心在不同的数据范围下可能是负优化，比如 $m = n^2${% emoji 滑稽 %}）！

因为”找出一个没有被访问的，$d[x]$ 最小的 $x$，然后访问 $x$“这一操作实属费时费力，所以我们可以用数据结构代替{% emoji 滑稽 %}，使用优先队列优化时间复杂度为 $O((n+m)\log n)$。

由于除了优先队列是 STL 就有的，剩下的数据结构手写巨复杂，超级难写且容易爆炸{% emoji sticker 滑稽喝水 %}，~~而且效率差不了太多~~，所以我们还是用优先队列吧{% emoji sticker 滑稽狂汗 %}。

下面是代码，采用 SPFA 封装好的数据结构，这套代码可以通过 [模板](https://www.luogu.com.cn/problem/P4779)：

```cpp
#include <iostream>
#include <cstdio>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

const int INF = (1u << 31) - 1;

struct edge
{
    int from, to, dist;
    edge(int from = 0, int to = 0, int dist = 0) :
        from(from), to(to), dist(dist) {}
};

int n, m, s;
int d[100005];
bool v[100005];
vector <edge> G[100005];

void addedge(int u, int v, int d) {
    G[u].push_back(edge(u, v, d));
}

void Dijkstra(void)
{
    #define pii pair<int, int>
    priority_queue <pii, vector<pii>, greater<pii>> q; // 小根堆
    memset(d, 0x3f, sizeof(d));
    d[s] = 0;
    q.push(make_pair(0, s));
    while (!q.empty())
    {
        int u = q.top().second; q.pop();
        if (!v[u]) // 没有被访问过
        {
            v[u] = true;
            for (int i = 0; i < G[u].size(); ++i)
            {
                edge &e = G[u][i];
                if (d[e.to] > d[u] + e.dist)
                {
                    d[e.to] = d[u] + e.dist;
                    q.push(make_pair(d[e.to], e.to));
                }
            }
        }
    }
}

int main(void)
{
    scanf("%d%d%d", &n, &m, &s);
    while (m--)
    {
        int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        addedge(u, v, d);
    }
    Dijkstra();
    for (int i = 1; i <= n; ++i)
        printf("%d ", d[i]);
    putchar('\n');
    return 0;
}
```

值得注意的是这个入队的方式。正常来讲我们应该修改队列中的 $d[e.to]$，但是 STL 不支持这种操作。所以我们再入一个队，值小的自然会先出队，所以正确性是没有问题的。当然也可以判断当前到此的距离是否小于最短路，从而省掉一个 `v` 数组，接下来的次短路也用到了类似的思想。

## 我究竟用什么算法？

这个问题已经很显然了，这里列出表格给大家参考（绿色字体意为这类题目少见，初学阶段可不必学习；蓝色意为要求数据随机）：

|最短路|$n \le 300$| $n\le 10^3, m\le 10^4$| $n\le 10^5, m\le 10^6$|
|:--:|:--:|:--:|:--:|
|多源|Floyd|{% span green,Johnson %}|N/A|
|无负权单源|Floyd|Dijkstra|优先队列 Dijkstra|
|有负权单源|Floyd|Belman-Ford 或 SPFA|{% span blue,SPFA %}|

如果这题要求有负权单源最短路，而且没保证数据随机，则有两种可能：一是毒瘤出题人就是没写{% emoji 滑稽 %}，二是你真的建错模型了，这题可以转化为无负权单源最短路或者它压根就不是最短路{% emoji 滑稽 %}。

注意！如果一张图是稠密图（图的边数接近点数的平方，否则就是稀疏图），那么这时就使用不优化的 Dijkstra，反而比优化的好。

就是这么简单{% emoji 滑稽 %}。

## 最短路的技巧

虽然最短路的算法我们都学习完毕了，但是还远远不够。如果最短路只有以上那些内容，那么它很简单，但事实上并不是。最短路有很多技巧，我们来看几个经典的。

### 最短路的打印

嗯，最短路还可以打印。

怎么打？每次松弛的时候记录一下不就完事了{% emoji 滑稽 %}？

以 Dijkstra 为例：

```cpp
int p[100005];
void dijkstra(void)
{
    #define pii pair<int, int>
    priority_queue<pii, vector<pii>, greater<pii> > q;
    memset(d, 0x3f, sizeof(d));
    d[s] = 0;
    q.push(make_pair(0, s));
    while (!q.empty())
    {
        int u = q.top().second;
        q.pop();
        if (v[u] == 0)
        {
            v[u] = 1;
            for (int i = 0; i < G[u].size(); ++i)
            {
                edge &e = G[u][i];
                if (d[u] + e.dist < d[e.to])
                {
                    d[e.to] = d[u] + e.dist;
                    p[e.to] = u; // 记录从哪个点来
                    q.push(make_pair(d[e.to], e.to));
                }
            }
        }
    }
}

void print(int s, int t) // 打印 s 到 t 的最短路
{
    if (s == t) // 是起点
    {
        printf("%d ", s);
        return;
    }
    print(s, p[t]); // 打印前一个点
    printf("%d ", t); //打印当前的点
}
```

### 固定终点的最短路

[[Luogu 1629] 邮递员送信](https://www.luogu.com.cn/problem/P1629)。

可以看出题目要分别求固定起点（从 $1$ 走）和固定终点（走回 $1$）的最短路。前者可以直接使用 Dijkstra 算法，但后者怎么办？

想一想，正常的单源最短路求的是从一个起点到所有点的最短路，如果这是无向图，我们只需要把这个最短路反过来就是所有点到这个起点的最短路（就是路径是反着的）。

也就是说，从 $i$ 走到 $1$ 的最短路，就是从 $1$ 走到 $i$ 的最短路径倒着走。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

struct edge {
    int from, to, dist;
    edge(int from = 0, int to = 0, int dist = 0) :
        from(from), to(to), dist(dist) {}
};

struct solver {
    vector <edge> G[1005];
    int d[1005]; bool v[1005];
    inline void addedge(int u, int v, int d) {
        G[u].push_back(edge(u, v, d));
    }
    void Dijkstra(void)
    {
        memset(d, 0x3f, sizeof(d));
        d[1] = 0;
        #define pii pair<int, int>
        priority_queue <pii, vector<pii>, greater<pii>> q;
        q.push(make_pair(0, 1));
        while (!q.empty()) {
            int u = q.top().second; q.pop();
            if (!v[u]) {
                v[u] = true;
                for (int i = 0; i < G[u].size(); ++i) {
                    edge &e = G[u][i];
                    if (d[e.to] > d[u] + e.dist) {
                        d[e.to] = d[u] + e.dist;
                        q.push(make_pair(d[e.to], e.to));
                    }
                }
            }
        }
    }
}A, B;

int main(void)
{
    int n, m;
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        A.addedge(u, v, d);
        B.addedge(v, u, d);
    }
    A.Dijkstra();
    B.Dijkstra();
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans += A.d[i] + B.d[i];
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 分层图最短路

[[JLOI2011] 飞行路线](https://www.luogu.com.cn/problem/P4568)。

注意到 $k$ 的范围很小，所以可以使用分层图，下面介绍什么是分层图。

将原图复制 $k$ 次，原来编号为 $i$ 的节点复制为编号 $i+jn$ 的节点。然后第 $j$ 层和第 $j+1$ 层的对应节点也要连上边，边权值为 $0$，而且是单向边（最多免费乘坐 $k$ 次）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

struct edge {
    int to, dist;
    edge(int to = 0, int dist = 0) :
        to(to), dist(dist) {}
};

int n, m, k, s, t;
int d[110005];
bool v[110005];
vector <edge> G[110005];

inline void addedge(int u, int v, int d) {
    G[u].push_back(edge(v, d));
}

void Dijkstra(void)
{
    #define pii pair<int, int>
    priority_queue <pii, vector<pii>, greater<pii>> q;
    memset(d, 0x3f, sizeof(d));
    d[s] = 0;
    q.push(make_pair(0, s));
    while (!q.empty()) {
        int u = q.top().second; q.pop();
        if (!v[u]) {
            v[u] = true;
            for (int i = 0; i < G[u].size(); ++i) {
                edge &e = G[u][i];
                if (d[e.to] > d[u] + e.dist) {
                    d[e.to] = d[u] + e.dist;
                    q.push(make_pair(d[e.to], e.to));
                }
            }
        }
    }
}

int main(void)
{
    scanf("%d%d%d%d%d", &n, &m, &k, &s, &t);
    for (int i = 0, u, v, d; i < m; ++i) {
        scanf("%d%d%d", &u, &v, &d);
        addedge(u, v, d);
        addedge(v, u, d);
        for (int j = 1; j <= k; ++j) {
            addedge(u + j * n, v + j * n, d);
            addedge(v + j * n, u + j * n, d);
            addedge(u + (j - 1) * n, v + j * n, 0);
            addedge(v + (j - 1) * n, u + j * n, 0);
        }
    }
    Dijkstra();
    int ans = 0x7fffffff;
    for (int i = 0; i <= k; ++i)
        ans = min(ans, d[t + i * n]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 最短路的性质

最短路有一条重要的性质：最短路中的一部分依然是最短路。采用反证法，如果存在一条路径比那一部分更短，那么可以用这条路径替换那一部分，原先的整条路就不是最短路。

### 最短路树（SPT）

能形成最短路的路径构成了一棵树，求解的方法是在松弛时记录来的边（跟最短路的打印是一样的），然后把它当作父指针就可以求解出整棵最短路树。

或者也可以在 `dfs` 查找树的时候直接扫描所有边，虽然会慢一点，但是时间复杂度是没有变的。

{% noteblock %}
[[Codeforces 1076D] Edge Deletion](https://www.luogu.com.cn/problem/CF1076D).

给一个 $n$ 个点，$m$ 条边的无向简单带权连通图, 要求删边至最多剩余 $k$ 条边。定义"好点"是指删边后，$1$ 号节点到它的最短路长度仍然等于原图最短路长度的节点。最大化删边后的好点个数。
{% endnoteblock %}

显然要尽可能地保留最短路树，代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;
typedef long long i64;
typedef pair<i64, int> pii;

int n, m, k;

struct edge { int v, d, id; };

vector<edge> G[300005];
bool vis[300005];
i64 d[300005];

void Dijkstra(void) 
{
    memset(d, 0x3f, sizeof(d));
    d[1] = 0;
    priority_queue<pii, vector<pii>, greater<pii> > q;
    q.push({0, 1});
    while (!q.empty()) {
        int u = q.top().second; q.pop();
        if (!vis[u]) {
            vis[u] = true;
            for (int i = 0; i < G[u].size(); ++i) {
                int v = G[u][i].v, w = G[u][i].d;
                if (d[v] > d[u] + w) {
                    d[v] = d[u] + w;
                    q.push({d[v], v});
                }
            }
        }
    }
}

int ans[300005], tot = 0;
void dfs(int x) {
    if (tot >= k) return;
    vis[x] = true;
    for (int i = 0; i < G[x].size(); ++i) {
        int v = G[x][i].v, id = G[x][i].id, w = G[x][i].d;
        if (!vis[v] && d[v] == d[x] + w) {
            ans[++tot] = id;
            dfs(v);
            if (tot >= k) return;
        }
    }
}

int main(void)
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1, u, v, w; i <= m; ++i) {
        scanf("%d%d%d", &u, &v, &w);
        G[u].push_back({v, w, i});
        G[v].push_back({u, w, i});
    }
    printf("%d\n", k = min(n - 1, k));
    Dijkstra();
    memset(vis, 0, sizeof(vis));
    dfs(1);
    for (int i = 1; i <= k; ++i)
        printf("%d ", ans[i]);
    putchar('\n');
    return 0;
}
```
{% endfolding %}

### 次短路

[模板](https://www.luogu.com.cn/problem/P2865)。

直接套用 Dijkstra 算法，但是要记录两个 $d$ 数组，分别保存最短路和次短路。

具体一点，如果发现了一条新的 $1\rightarrow i$ 的最短路，那么存在以下几种可能：

- 这是最短路，则更新最短路，并将次短路替换为原来的最短路；
- 等于最短路，没用（因为是严格次短路）；
- 大于最短路但是小于次短路，则替换次短路；
- 否则没用。

无论如何，更新之后都要 `push`，因为接下来都可能需要更新（即使是次短路）。判断是否已经更新过的条件就是当前的距离是否已经比次短路还要大。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

struct edge {
    int from, to, dist;
    edge(int from = 0, int to = 0, int dist = 0) :
        from(from), to(to), dist(dist) {}
};

int n, m;
int d[2][5005];
vector <edge> edges;
vector <int> G[5005];

inline void addedge(int u, int v, int d)
{
    edges.push_back(edge(u, v, d));
    G[u].push_back(edges.size() - 1);
}

void Dijkstra2(void)
{
    #define pii pair<int, int>
    priority_queue <pii, vector<pii>, greater<pii>> q;
    memset(d, 0x3f, sizeof(d));
    d[0][1] = 0;
    q.push(make_pair(0, 1));
    while (!q.empty()) {
        int dis = q.top().first, u = q.top().second; q.pop();
        if (dis > d[1][u]) continue;
        for (int i = 0; i < G[u].size(); ++i) {
            edge &e = edges[G[u][i]];
            if (d[0][e.to] > dis + e.dist) {
                d[1][e.to] = d[0][e.to];
                d[0][e.to] = dis + e.dist; // 原来最短路变成次短路
                q.push(make_pair(d[0][e.to], e.to));
            } else if (d[1][e.to] > dis + e.dist && d[0][e.to] < dis + e.dist) {
                d[1][e.to] = dis + e.dist;
                q.push(make_pair(d[1][e.to], e.to)); // 次短路可能被用于接下来的次短路更新
            }
        }
    }
}

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        addedge(u, v, d);
        addedge(v, u, d);
    }
    Dijkstra2();
    printf("%d\n", d[1][n]);
    return 0;
}
```
{% endfolding %}

当然还有 k 短路，不过非常的麻烦，会在专门的地方进行讨论。

## 差分约束系统

还记得负环吗？负环能做什么呢？就在这里，差分约束系统。

[模板](https://www.luogu.com.cn/problem/P5960)。

这类问题是说，给定一个包含 $m$ 个不等式的不等式组，有 $n$ 个变量 $x_1\dots x_n$，判断是否有解，有解输出一组解。不等式组如下：

$$\begin{cases} x_{c_1}-x_{c'_1}\leq y_1 \\x_{c_2}-x_{c'_2} \leq y_2 \\ \cdots\\ x_{c_m} - x_{c'_m}\leq y_m\end{cases}$$

我们将一个不等式写作 $x_i-x_j\le y$，那么可以变形为 $x_i \le x_j+c_k$，这与单源最短路径中的不等式非常相似，因此可以连一条从 $j$ 到 $i$ 的长度为 $c_k$ 的有向边。我们利用之前所讲的外挂一个虚拟节点 $0$，作为起点，将它向所有点连一条长度为 $0$ 的有向边，用于方便下面求解整个图的负环。那么 $0$ 到 $i$ 的距离必定小于 $0$ 到 $j$ 再到 $i$，满足不等式。

如果图中不存在负环，那么可以令 $x_i=d_i$ 就是一组解。如果有负环，设负环对应的所有不等式是 $x_{a_i}-x_{a_{i+1}}\le y_{b_i}$，将所有不等式相加得到 $0\le \sum y_{b_i}$，但是这个是负环，$\sum y_{b_i} < 0$，矛盾，因此差分约束系统无解。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

struct edge {
    int to, dist;
    edge(int to = 0, int dist = 0) :
        to(to), dist(dist) {}
};

int n, m;
int d[5005], cnt[5005];
bool inq[5005];
vector <edge> G[5005];

inline void addedge(int u, int v, int d) {
    G[u].push_back(edge(v, d));
}

bool SPFA(void)
{
    memset(d, 0x3f, sizeof(d));
    d[0] = 0, cnt[0] = 1, inq[0] = true;
    queue <int> q;
    q.push(0);
    while (!q.empty()) {
        int u = q.front(); q.pop(); inq[u] = false;

        for (int i = 0; i < G[u].size(); ++i) {
            edge &e = G[u][i];
            if (d[e.to] > d[u] + e.dist) {
                d[e.to] = d[u] + e.dist;
                cnt[e.to] = cnt[u] + 1;
                if (cnt[e.to] > n + 1) return false; // 总共有 n+1 个点，超过它有负环
                if (!inq[e.to]) {
                    inq[e.to] = true;
                    q.push(e.to);
                }
            }
        }
    }
    return true;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--) {
        int i, j, c;
        scanf("%d%d%d", &i, &j, &c);
        addedge(j, i, c);
    }
    for (int i = 1; i <= n; ++i)
        addedge(0, i, 0);
    if (SPFA()) {
        for (int i = 1; i <= n; ++i)
            printf("%d ", d[i]);
        putchar('\n');
    }
    else puts("NO");
    return 0;
}
```

有的约束条件会出现为 $x_i-x_j \ge y$，那么只需要两边乘 $-1$ 得到 $x_j-x_i \le -y$。如果出现了相等，只需要将其拆分成 $x_i-x_j \ge y$ 和 $x_i-x_j \le y$ 即可。

如果要求正数解，我们只需要给所有值加上一个足够大的数即可，这样不等式组依然成立。

## Problemset

我们来看一些习题，它们正是最短路中最有趣的内容，其中有些题目会与图论的其它内容综合。

### 简单最短路

不需要什么转化，就是赤裸的模板。

#### [Cnoi2020] 雷雨

[Portal](https://www.luogu.com.cn/problem/P6833).

我们可以枚举这个中转点，利用 Dijkstra 分别求出三个点到所有点的最短路，在建模的时候将一个有序数对作为点的编号，然后这个点的答案就是 $d1(i,j)+d2(i,j)+d3(i,j)-r(i,j)\times 2$。推荐读者自行实现代码。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>

#define i64 long long

using namespace std;

const int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};
 
int n, m, a, b, c;
int R[1005][1005];
i64 d[3][1001][1001];
bool v[1005][1005];

struct node
{
    int x, y;
    i64 d;
    node(int x = 0, int y = 0, i64 d = 0) :
        x(x), y(y), d(d) {}
    bool operator < (const node &a) const
    {
        return d > a.d;
    }
};

void Dijkstra(int sx, int sy, int k)
{
    memset(v, 0, sizeof(v));
    priority_queue <node> q;
    q.push(node(sx, sy, R[sx][sy]));
    d[k][sx][sy] = R[sx][sy];
    while (!q.empty())
    {
        int x = q.top().x, y = q.top().y; q.pop();
        if (!v[x][y])
        {
            v[x][y] = true;
            for (int i = 0; i < 4; ++i)
            {
                int xx = x + dx[i], yy = y + dy[i];
                if (xx < 1 || xx > n || yy < 1 || yy > n) continue;
                if (d[k][xx][yy] > d[k][x][y] + R[xx][yy])
                {
                    d[k][xx][yy] = d[k][x][y] + R[xx][yy];
                    q.push(node(xx, yy, d[k][xx][yy]));
                }
            }
        }
    }
}

int main(void)
{
    scanf("%d%d%d%d%d", &n, &m, &a, &b, &c);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            scanf("%d", &R[i][j]);
    memset(d, 0x3f, sizeof(d));
    Dijkstra(1, a, 0);
    Dijkstra(n, b, 1);
    Dijkstra(n, c, 2);
    i64 ans = 2e18;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            ans = min(ans, d[0][i][j] + d[1][i][j] + d[2][i][j] - (R[i][j] << 1));
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1144] 最短路计数

[Portal](https://www.luogu.com.cn/problem/P1144).

记 $cnt[i]$ 代表走到 $i$ 的最短路数目，那么如果 $i$ 被 $u\rightarrow i$ 松弛，那么 $cnt[u]=cnt[i]+1$，如果恰好相等而不能被松弛，那么 $cnt[i]$ 要加上 $cnt[u]$。

{% folding cyan::查看代码 %}
```cpp
// 笔者为了练习而使用的 SPFA，正常不要使用
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int MOD = 100003;

int n, m;
int d[1000005], cnt[1000005];
bool inq[1000005];
vector <int> G[1000005];

void SPFA(void)
{
    memset(d, 0x3f, sizeof(d));
    d[1] = 0, cnt[1] = 1, inq[1] = true;
    queue <int> q;
    q.push(1);
    while (!q.empty())
    {
        int u = q.front(); q.pop(); inq[u] = false;

        for (int i = 0; i < G[u].size(); ++i)
            if (d[G[u][i]] > d[u] + 1)
            {
                d[G[u][i]] = d[u] + 1;
                cnt[G[u][i]] = cnt[u];
                if (!inq[G[u][i]])
                {
                    inq[G[u][i]] = true;
                    q.push(G[u][i]);
                }
            }
            else if (d[G[u][i]] == d[u] + 1)
                cnt[G[u][i]] = (cnt[G[u][i]] + cnt[u]) % MOD;
    }
}

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        G[x].push_back(y);
        G[y].push_back(x);
    }
    SPFA();
    for (int i = 1; i <= n; ++i)
        printf("%d\n", cnt[i]);
    return 0;
}
```
{% endfolding %}

然后可以做一下双倍经验：[[UVa 10917] Walk Through the Forest](https://www.luogu.com.cn/problem/UVA10917)。

#### [Luogu P1875] 佳佳的魔法药水

[Portal](https://www.luogu.com.cn/problem/P1875).

直接使用不加优化的 Dijkstra 算法，用一个二维数组记录边，在转移的时候要保证另一个点也已经被访问。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#define i64 long long

using namespace std;

int n;
int d[1005];
i64 cnt[1005];
int G[3005][3005];
bool v[1005];

int main(void)
{
    memset(G, 0xff, sizeof G);
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d", d + i), cnt[i] = 1;
    int a, b, c;
    while (scanf("%d%d%d", &a, &b, &c) == 3) G[a][b] = G[b][a] = c;
    for (int op = 1; op < n; ++op)
    {
        int x = 0;
        for (int i = 0; i < n; ++i)
            if (!v[i] && (x == 0 || d[i] < d[x])) x = i;
        v[x] = true;
        for (int y = 0; y < n; ++y)
            if (v[y] && G[x][y] != -1)
            {
                if (d[G[x][y]] > d[x] + d[y]) d[G[x][y]] = d[x] + d[y], cnt[G[x][y]] = cnt[x] * cnt[y];
                else if (d[G[x][y]] == d[x] + d[y]) cnt[G[x][y]] += cnt[x] * cnt[y];
            }
    }
    printf("%d %lld\n", d[0], cnt[0]);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1462] 通往奥格瑞玛的道路

[Portal](https://www.luogu.com.cn/problem/P1462).

我们可以二分需要交的最大过路费，在限定内才允许松弛，然后跑最短路判断血量是否允许。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

struct edge
{
    int to, dist;
    edge(int to = 0, int dist = 0) :
        to(to), dist(dist) {}
};
vector <edge> edges;
vector <int> G[10005];

inline void addedge(int u, int v, int d)
{
    edges.push_back(edge(v, d));
    G[u].push_back(edges.size() - 1);
}

int n, m, b;
int f[10005];
int d[10005];
bool v[10005];

bool Dijkstra(int maxx)
{
    if (f[1] > maxx) return false;
    #define pii pair<int, int>
    memset(d, 0x3f, sizeof(d));
    memset(v, 0, sizeof(v));
    d[1] = 0;
    priority_queue <pii, vector<pii>, greater<pii>> q;
    q.push(make_pair(0, 1));
    while (!q.empty())
    {
        int x = q.top().second; q.pop();
        if (!v[x])
        {
            v[x] = true;
            for (int i = 0; i < G[x].size(); ++i)
            {
                edge &e = edges[G[x][i]];
                if (d[e.to] > d[x] + e.dist && f[e.to] <= maxx)
                {
                    d[e.to] = d[x] + e.dist;
                    q.push(make_pair(d[e.to], e.to));
                }
            }
        }
    }
    return d[n] <= b;
}

int main(void)
{
    scanf("%d%d%d", &n, &m, &b);
    for (int i = 1; i <= n; ++i)
        scanf("%d", f + i);
    while (m--)
    {
        int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        addedge(u, v, d);
        addedge(v, u, d);
    }
    int L = 0, R = 1000000002;
    while (L + 1 != R) // 二分需要交的最多的过路费
    {
        int mid = L + R >> 1;
        if (Dijkstra(mid)) R = mid;
        else L = mid;
    }
    if (L == 1000000001) puts("AFK");
    else printf("%d\n", R);
    return 0;
}
```
{% endfolding %}

#### [BJWC2012] 冻结

[Portal](https://www.luogu.com.cn/problem/P4822).

直接使用分层图最短路，处理一下边权即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

struct edge
{
    int to, dist;
    edge(int to = 0, int dist = 0) :
        to(to), dist(dist) {}
};

int n, m, k;
int d[2555];
bool v[2555];
vector <edge> G[2555];

inline void addedge(int u, int v, int d)
{
    G[u].push_back(edge(v, d));
}

void Dijkstra(void)
{
    memset(d, 0x3f, sizeof(d));
    d[1] = 0;
    #define pii pair<int, int>
    priority_queue <pii, vector<pii>, greater<pii>> q;
    q.push(make_pair(0, 1));
    while (!q.empty())
    {
        int u = q.top().second; q.pop();
        if (!v[u])
        {
            v[u] = true;
            for (int i = 0; i < G[u].size(); ++i)
            {
                edge &e = G[u][i];
                if (d[e.to] > d[u] + e.dist)
                {
                    d[e.to] = d[u] + e.dist;
                    q.push(make_pair(d[e.to], e.to));
                }
            }
        }
    }
}

int main(void)
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1, u, v, d; i <= m; ++i)
    {
        scanf("%d%d%d", &u, &v, &d);
        addedge(u, v, d);
        addedge(v, u, d);
        for (int j = 1; j <= k; ++j)
        {
            addedge(u + j * n, v + j * n, d);
            addedge(v + j * n, u + j * n, d);
            addedge(u + (j - 1) * n, v + j * n, d >> 1);
            addedge(v + (j - 1) * n, u + j * n, d >> 1);
        }
    }
    Dijkstra();
    int ans = 0x7fffffff;
    for (int i = 0; i <= k; ++i)
        ans = min(ans, d[(i + 1) * n]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [UVa 11374] Airport Express

[Portal](https://www.luogu.com.cn/problem/UVA11374)。

注意到商业线只能坐一站，所以枚举坐哪个商业线即可。在起点和重点各跑一次最短路（因为是无向图）。代码较为复杂，推荐自行实现。注意本题有坑，输出的行末不能有空格。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

#define pii pair<int, int>
#define X first
#define Y second

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

struct edge
{
    int u, v, d;
    edge(int u, int v, int d) :
        u(u), v(v), d(d) {}
};

vector <edge> edges;
vector <int> G[505];

inline void addedge(int u, int v, int d)
{
    edges.push_back(edge(u, v, d));
    G[u].push_back(edges.size() - 1);
}

int n, s, e, m, k;
int dist[505], p[505];
bool vis[505];

inline void dijkstra(int S)
{
    memset(dist, 0x3f, sizeof(dist));
    memset(vis, 0, sizeof(vis));
    memset(p, 0, sizeof(p));
    dist[S] = 0;
    priority_queue <pii, vector<pii>, greater<pii> > q;
    q.push(make_pair(0, S));
    while (!q.empty())
    {
        int u = q.top().Y;
        q.pop();
        if (vis[u] == 0)
        {
            vis[u] = 1;
            for (int i = 0; i < G[u].size(); ++i)
            {
                edge &e = edges[G[u][i]];
                if (dist[u] + e.d < dist[e.v])
                {
                    dist[e.v] = dist[u] + e.d;
                    p[e.v] = G[u][i];
                    q.push(make_pair(dist[e.v], e.v));
                }
            }
        }
    }
}

int d1[505], d2[505];
vector <int> path1[505], path2[505];
inline void calcShortestPath(int S, int *d, vector<int> *paths)
{
    dijkstra(S);
    for (int i = 1; i <= n; ++i)
    {
        d[i] = dist[i];
        paths[i].clear();
        int t = i;
        paths[i].push_back(t);
        while (S != t)
        {
            paths[i].push_back(edges[p[t]].u);
            t = edges[p[t]].u;
        }
        reverse(paths[i].begin(), paths[i].end());
    }
}

int main(void)
{
    int kase = 0;
    while (scanf("%d%d%d", &n, &s, &e) == 3)
    {
        for (int i = 0; i <= n; ++i) G[i].clear();
        edges.clear();
        m = read();
        while (m--)
        {
            int u = read(), v = read(), d = read();
            addedge(u, v, d);
            addedge(v, u, d);
        }
        calcShortestPath(s, d1, path1);
        calcShortestPath(e, d2, path2);
        int ans = d1[e], midpoint = -1;
        vector <int> path = path1[e];
        k = read();
        while (k--)
        {
            int x = read(), y = read(), val = read();
            for (int op = 0; op < 2; ++op)
            {
                if (d1[x] + d2[y] + val < ans)
                {
                    ans = d1[x] + d2[y] + val;
                    midpoint = x;
                    path = path1[x];
                    for (int j = path2[y].size() - 1; j >= 0; --j)
                        path.push_back(path2[y][j]);
                }
                swap(x, y);
            }
        }
        if (kase) putchar('\n');
        ++kase;
        for (int i = 0; i < path.size() - 1; ++i)
            printf("%d ", path[i]);
        printf("%d\n", e);
        if (midpoint == -1) puts("Ticket Not Used");
        else printf("%d\n", midpoint);
        printf("%d\n", ans);
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P2829] 大逃离

[Portal](https://www.luogu.com.cn/problem/P2829).

根据最短路的性质，次短路还有另一种求法：跑出以 $1,n$ 为起点的最短路（当然有向图还需要建反图），然后枚举每一条边，以这一条边为次短路中的一条，两端的最短路是剩下的部分。按上述方式实现即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;
const int INF = 1e9;

struct edge {
    int u, v, d;
};

int n, m, k;
bool vis[5005];
int d1[5005], d2[5005], out[5005];
vector<edge> edges;
vector<int> G[5005];

inline void addedge(int u, int v, int w) {
    edges.push_back({u, v, w});
    G[u].push_back(edges.size() - 1);
}

void Dijkstra(int s, int *d) {
    #define pii pair<int, int>
    priority_queue<pii, vector<pii>, greater<pii> > q;
    memset(vis, 0, sizeof(vis));
    d[s] = 0;
    q.push({0, s});
    while (!q.empty()) {
        int u = q.top().second; q.pop();
        if (!vis[u]) {
            vis[u] = true;
            for (int i = 0; i < G[u].size(); ++i) {
                int v = edges[G[u][i]].v, w = edges[G[u][i]].d;
                if (d[v] > d[u] + w && out[v] >= k) {
                    d[v] = d[u] + w;
                    q.push({d[v], v});
                }
            }
        }
    }
}

int main(void)
{
    scanf("%d%d%d", &n, &m, &k);
    while (m--) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        addedge(u, v, w);
        addedge(v, u, w);
    }
    out[1] = out[n] = INF;
    for (int i = 2; i < n; ++i) {
        memset(vis, 0, sizeof(vis));
        for (int j = 0; j < G[i].size(); ++j) {
            int v = edges[G[i][j]].v;
            if (!vis[v]) {
                vis[v] = true;
                ++out[i];
            }
        }
    }
    memset(d1, 0x3f, sizeof(d1)); memset(d2, 0x3f, sizeof(d2));
    Dijkstra(1, d1);
    Dijkstra(n, d2);
    int ans = INF;
    for (int i = 0; i < edges.size(); ++i) {
        int u = edges[i].u, v = edges[i].v, w = edges[i].d;
        int len = d1[u] + d2[v] + w;
        if (len > d1[n]) ans = min(ans, len);
    }
    if (ans != INF) printf("%d\n", ans);
    else puts("-1");
    return 0;
}
```
{% endfolding %}

### 差分约束算法

差分约束系统很有用。

#### [Luogu P1993] 小 K 的农场

[Portal](https://www.luogu.com.cn/problem/P1993).

可以看出是差分约束，只需要利用之前所讲的随便转化一下就可以了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

struct edge
{
    int to, dist;
    edge(int to = 0, int dist = 0) :
        to(to), dist(dist) {}
};

int n, m;
vector <edge> G[5005];

void addedge(int u, int v, int d) 
{ 
    G[u].push_back(edge(v, d)); 
}

int d[5005], cnt[5005];
bool inq[5005];

bool SPFA(void)
{
    memset(d, 0x3f, sizeof(d));
    d[0] = 0;
    inq[0] = true;
    queue <int> q;
    q.push(0);
    while (!q.empty())
    {
        int u = q.front(); q.pop(); inq[u] = false;
        for (int i = 0; i < G[u].size(); ++i)
        {
            edge &e = G[u][i];
            if (d[e.to] > d[u] + e.dist)
            {
                d[e.to] = d[u] + e.dist;
                cnt[e.to] = cnt[u] + 1;
                if (cnt[e.to] >= n + 1) return true;
                if (!inq[e.to])
                {
                    inq[e.to] = true;
                    q.push(e.to);
                }
            }
        }
    }
    return false;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--)
    {
        int a, b, c, d;
        scanf("%d%d%d", &a, &b, &c);
        if (a == 3) addedge(b, c, 0), addedge(c, b, 0);
        else
        {
            scanf("%d", &d);
            if (a == 1) addedge(b, c, -d);
            else addedge(c, b, d);
        }
    }
    for (int i = 0; i <= n; ++i) addedge(0, i, 0); 
    if (SPFA()) puts("No");
    else puts("Yes");
    return 0;
}
```
{% endfolding %}

### 最短路建模与综合应用

这里的题可能需要一些脑子，但是笔者没有{% emoji tong Sad %}。

#### [SDOI2009] Elaxia 的路线

[Portal](https://www.luogu.com.cn/problem/P2149).

我们对四个点各跑一次最短路，然后依次判断每一条边是否同时是两个最短路中的一条边（注意其中一个要正反搞两次），这些边将构成一个 DAG。然后进行拓扑排序，计算最长的公共时间。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

int n, m;
int x_, y_, x__, y__;

struct edge {
    int from, to, dist;
    edge(int from = 0, int to = 0, int dist = 0) :
        from(from), to(to), dist(dist) {}
};

vector <edge> edges;
vector <int> G[1505];
int d[4][1505];
bool v[1505];

inline void addedge(int u, int v, int w) {
    edges.push_back(edge(u, v, w));
    G[u].push_back(edges.size() - 1);
}

void Dijkstra(int s, int id) {
    memset(d[id], 0x3f, sizeof(d[id]));
    memset(v, 0, sizeof(v));
    d[id][s] = 0;
    #define pii pair<int, int>
    priority_queue <pii, vector<pii>, greater<pii>> q;
    q.push(make_pair(0, s));
    while (!q.empty()) {
        int u = q.top().second; q.pop();
        if (!v[u]) {
            v[u] = true;
            for (int i = 0; i < G[u].size(); ++i) {
                edge &e = edges[G[u][i]];
                if (d[id][e.to] > d[id][u] + e.dist) {
                    d[id][e.to] = d[id][u] + e.dist;
                    q.push(make_pair(d[id][e.to], e.to));
                }
            }
        }
    }
}

int in[1505], len[1505];
vector <edge> G2[1505];
inline void addedge2(int u, int v, int w) {
    G2[u].push_back(edge(u, v, w));
    ++in[v];
}

void Kahn(void) {
    queue <int> q;
    for (int i = 1; i <= n; ++i)
        if (in[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < G2[u].size(); ++i) {
            edge &e = G2[u][i];
            --in[e.to];
            len[e.to] = max(len[e.to], len[e.from] + e.dist);
            if (in[e.to] == 0) q.push(e.to);
        }
    }
}

int main(void) {
    scanf("%d%d%d%d%d%d", &n, &m, &x_, &y_, &x__, &y__);
    while (m--) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        addedge(u, v, w);
        addedge(v, u, w);
    }
    Dijkstra(x_, 0), Dijkstra(y_, 1);
    Dijkstra(x__, 2), Dijkstra(y__, 3);

    int ans = 0;
    for (int i = 1; i <= n; ++i)
        for (int j = 0; j < G[i].size(); ++j) {
            edge &e = edges[G[i][j]];
            // x_ -> i + i -> to + y_ -> to == x_ -> y_
            // x__ -> i + i -> to + y__ -> to == x__ -> y__
            if (d[0][i] + e.dist + d[1][e.to] == d[0][y_] && d[2][i] + e.dist + d[3][e.to] == d[2][y__])
                addedge2(e.from, e.to, e.dist);
        }
    Kahn();
    for (int i = 1; i <= n; ++i) ans = max(ans, len[i]);

    for (int i = 1; i <= n; ++i) G2[i].clear();
    memset(len, 0, sizeof(len)), memset(in, 0, sizeof(in));
    for (int i = 1; i <= n; ++i)
        for (int j = 0; j < G[i].size(); ++j) {
            edge &e = edges[G[i][j]];
            // x_ -> i + i -> to + y_ -> to == x_ -> y_
            // y__ -> i + i -> to + x__ -> to == y__ -> x__
            if (d[0][i] + e.dist + d[1][e.to] == d[0][y_] && d[3][i] + e.dist + d[2][e.to] == d[3][x__])
                addedge2(e.from, e.to, e.dist);
        }
    Kahn();
    for (int i = 1; i <= n; ++i) ans = max(ans, len[i]);

    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}