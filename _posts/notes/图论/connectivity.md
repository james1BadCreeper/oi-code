---
title: 拓扑排序与用 Tarjan 解决图的连通性问题
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 图论
headimg: 'https://s1.ax1x.com/2022/08/04/vmu0Re.jpg'
group: notes
hide: true
status: working
abbrlink: 242bd056
date: 2022-08-04 22:04:57
---

> 在图中，如何判断一张图是否连通？如果删掉某条边，它还连通吗？有向图呢？这些操作有什么特殊性质吗？本文将探讨以 Tarjan 算法为核心的有关图的连通性的问题。

<!--more-->

除了 Tarjan 算法，并查集等内容也能解决一些图连通性问题，请参照笔者相关文章。

## 拓扑排序

~~别问我为什么把这个放到这里来讲，因为接下来很多题都要用到它。~~

严格意义上来说，拓扑排序**不是**一种排序。是对有向无环图（DAG）$G$，将 $G$ 中所有顶点排成一个线性序列，使得图中任意一对顶点 $u$ 和 $v$ ，若它们之间存在一条有向边 $(u,v)$,则 $u$ 在线性序列中出现在 $v$ 之前。

[模板](https://www.luogu.com.cn/problem/UVA10305)。

开一个队列记录所有入度为 $0$ 的点，然后维护即可。这一过程被称之为 Kahn 算法。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

int n, m;
int in[105];
vector <int> G[105];
queue <int> q;

int main(void) {
    while (scanf("%d%d", &n, &m) == 2 && n) {
        memset(in, 0, sizeof(in));
        for (int i = 1; i <= n; ++i) G[i].clear();
        while (m--) {
            int x, y;
            scanf("%d%d", &x, &y);
            G[x].push_back(y);
            ++in[y];
        }
        for (int i = 1; i <= n; ++i)
            if (in[i] == 0) q.push(i);
        while (!q.empty()) {
            int t = q.front();
            q.pop();
            printf("%d ", t);
            for (int i = 0; i < G[t].size(); ++i) {
                --in[G[t][i]];
                if (in[G[t][i]] == 0) q.push(G[t][i]);
            }
        }
        putchar('\n');
    }
    return 0;
}
```

拓扑排序也可以使用 dfs 实现，感兴趣的读者可以自行学习。

有的时候求的拓扑序要求字典序，这时候直接将队列改为优先队列即可。

## 无向图的连通性

给定无向图 $G=(V,E)$，如果 $x\in V$，从图中删去节点 $x$ 和与 $x$ 关联的所有边之后，$G$ 被分裂成两个或两个以上的不相连的子图，那么称 $x$ 称之为 $G$ 的**割点**或**割顶**。如果 $e\in V$，将 $e$ 删去后，$G$ 分裂成两个不相连的子图，则称 $e$ 为 $G$ 的**桥**或**割边**。

我们可以使用 Tarjan 算法（嗯，还是 Robert Tarjan，什么 $O(1)$ LCA、伸展树、LCT 都是他搞出来的）在线性时间内求解无向图的割点和桥。

想要使用 Tarjan 算法，我们需要先了解几个基本概念：

### Tarjan 算法

**时间戳**。我们对图进行深度优先遍历，按照每个节点第一次被访问到的顺序，依次给予 $n$ 个节点 $1\sim n$ 的整数标记，记为时间戳 $dfn[x]$，代表在 DFS 序中出现的位置。

**搜索树**。在无向连通图中任选一个节点出发进行深度优先遍历，每个点只访问一次，所有发生递归的边 $(x,y)$（即 $x$ 到 $y$ 是对 $y$ 的第一次遍历），这样的边有 $n-1$ 条，构成一棵树，称之为”无向连通图的搜索树“。如果这张图不连通，那么它会生成若干棵树，称之为”无向图的搜索森林“。

**追溯值**。除了时间戳外，还有一个概念：追溯值 $low[x]$，也就是能**不经过父亲节点到达的最小时间戳**。设 $\text{subtree}(x)$ 代表搜索树中以 $x$ 的子树，$low[x]$ 定义为以下节点的时间戳的最小值：

1. $\text{subtree}(x)$ 中的节点；
2. 通过 $1$ 条不在搜索树上的边，可以到达 $\text{subtree}(x)$ 的节点。

{% image https://s1.ax1x.com/2022/08/12/vJV8IJ.png::alt=这是一张无向图，粗边标出了搜索树，1 为根节点，每个节点是它的时间戳，括号标出了它的追溯值 %}

如上图，$1$ 节点的追溯值是它自己的时间戳，$(1,5)$ 一条不在搜索树上的边使得 $low[5]=dfn[1]=1$，所以 $2,3,4$ 节点都是 $5$ 的祖先，又因为 $(1,5)$ 这条边，所以 $low[2]=low[3]=low[4]=dfn[1]=1$。剩余节点大致同理。

我们计算追溯值时，应该首先令 $low[x]=dfn[x]$，然后考虑 $x$ 出发的每条边 $(x,y)$：如果在搜索树上 $x$ 是 $y$ 的父亲，那么 $low[x]=\min\{low[x], low[y]\}$；如果 $(x,y)$ 不是搜索树上的边，则 $low[x]=\min\{low[x], dfn[y]\}$。

Tarjan 的时间复杂度为 $O(n+m)$。

至于追溯至究竟是怎么来的，读者可以自行查阅资料。

### 割边判定法则

无向边 $(x,y)$ 是割边，当且仅当搜索树上存在一个 $x$ 的子节点 $y$，满足 $dfn[x]<low[y]$。也就是说从 $\text{subtree}(y)$ 出发，若不经过 $(x,y)$，怎么走都无法到达 $x$ 或比 $x$ 更早访问的节点（因为 $dfn[x]<low[y]$，即想要到 $y$ 必须经过 $(x,y)$，即 $y$ 被困在 $x$ 的子树中了），那么这条边便是桥。

可以发现，桥一定是搜索树中的边，并且一个简单环中的边一定都不是桥。

{% image https://s1.ax1x.com/2022/08/12/vJlYJU.png::alt=虚线标出了桥 %}

在存边的时候使用了一个 edges 数组，这样使用”成对变换“可以轻松的找到反向边。记录 `fa` 的话遇到重边时会出错。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m, num = 0; // num 用于时间戳的标记
int dfn[105], low[105];

struct edge {
    int from, to;
    edge(int from = 0, int to = 0) :
        from(from), to(to) {}
};

vector <edge> edges;
bool bridge[205];
vector <int> G[105];

inline void addedge(int u, int v) {
    edges.push_back(edge(u, v));
    G[u].push_back(edges.size() - 1);
}

void tarjan(int x, int in_edge) { // in_edge 记录递归进入 x 的边的编号
    dfn[x] = low[x] = ++num; // 标记时间戳，并在初始将 low[x] 标记为 dfn[x]
    for (int i = 0; i < G[x].size(); ++i) {
        int y = edges[G[x][i]].to; // 获取边 (x,y)
        if (!dfn[y]) { // 未访问
            tarjan(y, G[x][i]); // 递归遍历
            // 能跑到这里的肯定是搜索树上的
            // 所以 x 是 y 的父亲节点了，也就是 y 属于 subtree(x)
            low[x] = min(low[x], low[y]);
            // 割边判定法则，在搜索树上存在 x 的一个子节点 y 使得 dfn[x] < low[y]
            if (dfn[x] < low[y]) bridge[G[x][i]] = bridge[G[x][i] ^ 1] = true; // 标记的时候标记正边和反边
        }
        else if (G[x][i] != (in_edge ^ 1)) low[x] = min(low[x], dfn[y]);
        // 如果当前这条边是 x->y 的反边 y->x，那么这条命令不会被执行，因为它在搜索树上
        // 但是有重边的话他就不是搜索树上的边
    }
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v), addedge(v, u);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i, -1); // 图不一定连通，每个点都需要 tarjan。最初没有边到 i，用 -1 代替，-1 ^ 1 = -2
    puts("Bridges:");
    for (int i = 0; i < edges.size(); i += 2)
        if (bridge[i]) printf("%d %d\n", edges[i].from, edges[i].to);
    return 0;
}
```

### 割点判定法则

[模板](https://www.luogu.com.cn/problem/P3388)。

如果 $x$ 不是搜索森林中一棵树的根节点，那么 $x$ 是割点当且仅当搜索树上存在 $x$ 的子节点 $y$ 满足 $dfn[x]\le low[y]$。如果这是根节点，这样的 $y$ 必须有两个或以上，$x$ 才是割点。

由于是小于等于，所以父节点和重边即使不考虑也能得到正确结果。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m, root, num = 0, ans = 0;
int dfn[20005], low[20005];

struct edge {
    int from, to;
    edge(int from = 0, int to = 0) :
        from(from), to(to) {}
};

vector <edge> edges;
bool cut[20005];
vector <int> G[20005];

inline void addedge(int u, int v) {
    edges.push_back(edge(u, v));
    G[u].push_back(edges.size() - 1);
}

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    int flag = 0;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = edges[G[x][i]].to;
        if (!dfn[y]) {
            tarjan(y);
            low[x] = min(low[x], low[y]);
            if (dfn[x] <= low[y]) {
                ++flag;
                if (x != root || flag > 1) cut[x] = true; // 如果不是根节点，或者 flag >= 2，就是割点
            }
        }
        else low[x] = min(low[x], dfn[y]); // 可以直接更新
    }
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v), addedge(v, u);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) root = i, tarjan(i); // 记录根节点，然后 tarjan
    for (int i = 1; i <= n; ++i)
        if (cut[i]) ++ans;
    printf("%d\n", ans);
    for (int i = 1; i <= n; ++i)
        if (cut[i]) printf("%d ", i);
    putchar('\n');
    return 0;
}
```

#### [POI2008] BLO-Blockade

[Portal](https://www.luogu.com.cn/problem/P3469).

{% noteblock %}
一张连通的无向图，请你对于每个节点 $i$ 求出，把与节点 $i$ 关联的所有边去掉以后（不去掉节点 $i$ 本身），无向图有多少个有序点对 $(x,y)$，满足 $x$ 和 $y$ 不连通。
{% endnoteblock %}

如果一个点不是割点，那么断掉这个点所连的所有边后图的剩余部分依然连通，只有这个点与图中其它的所有点构成的有序点对满足不连通，共有 $(n-1)\times 2$ 个。

如果这个点是割点，那么断掉后图会分裂成若干个连通块。我们应该求出这些连通块的大小，然后两两相乘再将这些积相加。设再搜索树中，节点 $i$ 的子节点集合中，有 $t$ 个点满足 $dfn[x]\le low[s_k]$，断掉之后，无向图至多分裂为 $t+2$ 个连通块，这些连通块分别是：$t$ 个断掉之后的小子树，$1$ 个当前节点 $x$ 自己构成的连通块，图的剩余部分（$x$ 的父亲及其它）。

由于 Tarjan 算法本质上是一个 dfs。我们可以使用类似求树的重心的方式，设在搜索树中 $size[x]$ 表示 $x$ 的子树大小，那么断掉之后的有序数对数量为：

- 每棵小子树所对应的：

$$
size[s_1]\times (n-size[s_1]) + \dots size[s_t]\times (n-size[s_t]) = \sum\limits_{i=1}^{t}size[s_i]\times (n-size[s_i])
$$

- 当前节点 $x$ 对应的：

$$
1\times(n-1) = n-1
$$

- 剩余部分对应的（前者为剩余部分的大小，后者为小子树和当前节点的大小和）：

$$
\left(n-1-\sum\limits_{i=1}^{t} size[s_i]\right)\times \left(1+\sum\limits_{i=1}^{t} size[s_i]\right)
$$

代码便不难写出：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;
typedef long long i64;

struct edge {
    int from, to;
    edge(int from = 0, int to = 0) :
        from(from), to(to) {}
};

int n, m, num;
int dfn[100005], low[100005];
int Size[100005];
i64 ans[100005];
bool cut[100005];
vector <edge> edges;
vector <int> G[100005];

inline void addedge(int u, int v) {
    edges.push_back(edge(u, v));
    G[u].push_back(edges.size() - 1);
}

void tarjan(int x) {
    dfn[x] = low[x] = ++num; Size[x] = 1;
    int flag = 0;
    i64 sum = 0;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = edges[G[x][i]].to;
        if (!dfn[y]) {
            tarjan(y);
            Size[x] += Size[y];
            low[x] = min(low[x], low[y]);
            if (dfn[x] <= low[y]) {
                ++flag;
                ans[x] += (i64)Size[y] * (n - Size[y]);
                sum += Size[y];
                if (x != 1 || flag > 1) cut[x] = true;
            }
        }
        else low[x] = min(low[x], dfn[y]);
    }
    if (cut[x]) ans[x] += (n - 1) + (n - 1 - sum) * (1 + sum);
    else ans[x] = (n - 1) << 1;
}


int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v); addedge(v, u);
    }
    tarjan(1); // 所有城市都连通，调用一次 tarjan 即可
    for (int i = 1; i <= n; ++i)
        printf("%lld\n", ans[i]);
    return 0;
}
```
{% endfolding %}

### 双连通分量与缩点

如果一张图不存在割边，那么这张图被称为**边双连通图**。如果它不存在割点，那么称之为**点双连通图**。

接下来给出的两份代码同时也是无向图连通性的模板。

#### e-DCC 及其缩点

无向连通图的极大边双连通子图被称之为**边双连通分量**，简记为 e-DCC。其中极大子图的意思是不存在一个更大的子图，这个子图包含了原来的子图，也满足这个限制条件。

一个图的边双连通分量之间一定是不相交的。如果两个双连通分量相交了，那么顺去它们中的一条边，两个子图依然是连通的，也就是说这两个双连通分量是一个更大的双连通分量的一部分。

[模板](https://www.luogu.com.cn/problem/P8436)。

e-DCC 的求解非常容易，因为如果图不存在割边，那么我们就把所有的割边都给删掉，图会分裂成若干个连通块，每一个连通块都是一个 e-DCC。

有一个性质：无向连通图是边双连通图，当且仅当任意一条边都包含在至少一个简单环中。

求解 e-DCC 时，经过一个点就要将这个点压入栈。当 $low[x]=dfn[x]$ 时，代表 $x$ 就是连通块深度最大的点。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

struct edge {
    int from, to;
    edge(int from = 0, int to = 0) :
        from(from), to(to) {}
};

int n, m, num = 0, cnt = 0, c[500005];
int dfn[500005], low[500005];
bool bridge[4000005];
int st[500005], tot = 0;
vector <edge> edges;
vector <int> G[500005];
vector <int> ans[500005];

inline void addedge(int u, int v) {
    edges.push_back(edge(u, v));
    G[u].push_back(edges.size() - 1);
}

void tarjan(int x, int in_edge) {
    dfn[x] = low[x] = ++num;
    st[++tot] = x;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = edges[G[x][i]].to;
        if (!dfn[y]) {
            tarjan(y, G[x][i]);
            low[x] = min(low[x], low[y]);
            if (dfn[x] < low[y])
                bridge[G[x][i]] = bridge[G[x][i] ^ 1] = true;
        }
        else if (G[x][i] != (in_edge ^ 1)) low[x] = min(low[x], dfn[y]);
    }
    if (low[x] == dfn[x]) {
        int y; ++cnt;
        do {
            y = st[tot--];
            ans[cnt].push_back(y);
        } while (x != y);
    }
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v), addedge(v, u);
    }   
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i, -1);
    printf("%d\n", cnt);
    for (int i = 1; i <= cnt; ++i) {
        printf("%d ", ans[i].size());
        for (auto x : ans[i])
            printf("%d ", x);
        putchar('\n');
    }
    return 0;
}
```

将每一个 e-DCC 都看作一个节点，把割边看作连接 e-DCC 的边，这样会产生一棵树（不连通的无向图就是森林）。这种把 e-DCC 缩为一个节点的方式就叫做缩点，在解决连通性问题的时候非常有用。

```cpp
for (int i = 0; i < edges.size(); ++i)
    if (c[edges[i].from] != c[edges[i].to]) addedge(c[edges[i].from], c[edges[i].to]); // 不在一个 e-DCC 里面，将 e-DCC 连边
```

#### v-DCC 及其缩点

无向连通图的极大点双连通子图被称之为**点双连通分量**，简记为 v-DCC。

在求解的时候，每访问到一个新的节点，都需要将它入栈。若割点的判定法则成立，那么无论如何，都要不断弹出节点，直到 $y$ 被弹出。弹出的所有东西加上 $x$ 就是一个 v-DCC。还有，如果一个点是自己单独一个，那么它也是一个 v-DCC，需要特判。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m, root, num = 0, tot = 0, cnt = 0;
int dfn[500005], low[500005], stack[500005];
vector <int> G[500005], ans[500005];

inline void addedge(int u, int v) { G[u].push_back(v); }

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    stack[++tot] = x;
    if (x == root && G[x].size() == 0) {
        ans[++cnt].push_back(x);
        return;
    }
    int flag = 0;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = G[x][i];
        if (!dfn[y]) {
            tarjan(y);
            low[x] = min(low[x], low[y]);
            if (dfn[x] <= low[y]) {
                ++cnt; ++flag;
                if (x != root || flag > 1) cut[x] = true;
                int z;
                do {
                    z = stack[tot--];
                    ans[cnt].push_back(z);
                } while (z != y);
                ans[cnt].push_back(x);
            }
        }
        else low[x] = min(low[x], dfn[y]);
    }
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        if (u != v) addedge(u, v), addedge(v, u); // 为了方便判断孤立点，所以自环不能加
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) root = i, tarjan(i);
    printf("%d\n", cnt);
    for (int i = 1; i <= cnt; ++i) {
        printf("%d ", ans[i].size());
        for (int j = 0; j < ans[i].size(); ++j)
            printf("%d ", ans[i][j]);
        putchar('\n');
    }
    return 0;
}
```

边双连通分量和点双连通分量统称为双连通分量，即 DCC（Double Connected component）。

点双连通分量可以引出圆方树，是解决点相关路径问题的利器，在本文中不作讨论。

#### [USACO06JAN] Redundant Paths G

[Portal](https://www.luogu.com.cn/problem/P2860).

直接求出 e-DCC 缩点后的树，然后将叶子配对即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

struct edge {
    int from, to;
    edge(int from = 0, int to = 0) :
        from(from), to(to) {}
};

int n, m, num = 0, cnt = 0;
int dfn[5005], low[5005], c[5005], in[5005];
bool bridge[20005];
vector <edge> edges;
vector <int> G[5005];

inline void addedge(int u, int v) {
    edges.push_back(edge(u, v));
    G[u].push_back(edges.size() - 1);
}

void tarjan(int x, int in_edge) {
    dfn[x] = low[x] = ++num;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = edges[G[x][i]].to;
        if (!dfn[y]) {
            tarjan(y, G[x][i]);
            low[x] = min(low[x], low[y]);
            if (dfn[x] < low[y]) bridge[G[x][i]] = bridge[G[x][i] ^ 1] = true;
        }
        else if (G[x][i] != (in_edge ^ 1)) low[x] = min(low[x], dfn[y]);
    }
}

void dfs(int x) {
    c[x] = cnt;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = edges[G[x][i]].to;
        if (c[y] || bridge[G[x][i]]) continue;
        dfs(y);
    }
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v), addedge(v, u);
    }
    tarjan(1, -1);
    int res = 0;
    for (int i = 1; i <= n; ++i)
        if (!c[i]) {
            ++cnt;
            dfs(i);
        }
    for (int i = 0; i < edges.size(); i += 2) // 统计叶子的个数，入度为 1 的是叶子
        if (c[edges[i].from] != c[edges[i].to]) {
            ++in[c[edges[i].from]];
            ++in[c[edges[i].to]];
        }
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        if (in[i] == 1) ++ans;
    printf("%d\n", (ans + 1) / 2);
    return 0;
}
```
{% endfolding %}

## 有向图的连通性

在有向图中，如果任意两个点都可以互相到达，那么这张图被成为**强连通图**，有向图的极大强连通子图被称之为**强连通分量**（SCC）。显然，一个点最多属于一个 SCC。

### Tarjan 算法

[模板](https://www.luogu.com.cn/problem/P2863).

Tarjan 算法可以求出有向图的强连通分量。当图变成无向图时，该算法也可以正常工作。

这里直接给出代码，原理有时间再写：

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m;
bool ins[10005];
int dfn[10005], low[10005], num = 0;
int st[10005], tot = 0;
int cnt = 0, c[10005], siz[10005];
vector <int> G[10005];

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    ins[st[++tot] = x] = true;
    for (auto y : G[x])
        if (!dfn[y]) {
            tarjan(y);
            low[x] = min(low[x], low[y]);
        } else if (ins[y]) low[x] = min(low[x], dfn[y]);
    if (low[x] == dfn[x]) {
        int y; ++cnt;
        do {
            y = st[tot--]; ins[y] = false;
            c[y] = cnt; ++siz[cnt];
        } while (x != y);
    }
}

int main(void) {  
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].emplace_back(v);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i);
    int ans = 0;
    for (int i = 1; i <= cnt; ++i)
        if (siz[i] > 1) ++ans;
    printf("%d\n", ans);
    return 0;
}
```

### 缩点

同无向图，将一个 SCC 缩成一个点，便是缩点。有向图在缩点之后可以得到 DAG，然后就可以进行拓扑排序之类的操作。

[模板](https://www.luogu.com.cn/problem/P3387)。在做的时候可以发现一个 SCC 内的点都可以到达，缩点之后的权值相当于 SCC 内点的权值综合，而且在 Tarjan 的过程中就可以进行 DP：给 SCC 编号时要进行转移，最后也要加上 SCC 内的点权和。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m, ans = 0, a[10005], f[10005];
bool ins[10005];
int dfn[10005], low[10005], num = 0;
int st[10005], tot = 0;
int cnt = 0, c[10005], siz[10005];
vector <int> G[10005];

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    ins[st[++tot] = x] = true;
    for (auto y : G[x])
        if (!dfn[y]) {
            tarjan(y);
            low[x] = min(low[x], low[y]);
        } else if (ins[y]) low[x] = min(low[x], dfn[y]);
    if (low[x] == dfn[x]) {
        int y, sum = 0; ++cnt;
        do {
            y = st[tot--]; ins[y] = false;
            c[y] = cnt; ++siz[cnt];
            sum += a[y];
            for (auto v : G[y])
                f[cnt] = max(f[cnt], f[c[v]]);
        } while (x != y);
        f[cnt] += sum;
        ans = max(ans, f[cnt]);
    }
}

int main(void) {  
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].emplace_back(v);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i);
    printf("%d\n", ans);
    return 0;
}
```

## 一笔画问题

也就是欧拉路问题。

### 无向图情况

给定一张无向图，若存在一条从一个点走到另一个点，不重不漏地经过图上所有的边一次，那么这条路称之为**欧拉路**。特别地，如果从一个点出发回到了一个点，那么这条路称之为**欧拉回路**。存在欧拉回路的图称之为**欧拉图**。不存在欧拉回路但是存在欧拉路的图称为**半欧拉图**。

在小学已经学过，如果图中度数为奇数的点是 $0$ 或 $2$，那么这个图可以一笔画。当为 $0$ 时，这个图是欧拉图，当为 $2$ 时，存在欧拉路。

代填坑（

### 有向图情况

## Problemset

连通性的问题很有意思，我们来看几道题玩一下{% emoji sticker 滑稽狂汗 %}。

### 拓扑排序

基础中的基础。

#### [Luogu P1347] 排序

[Portal](https://www.luogu.com.cn/problem/P1347).

使用拓扑排序。在过程中记录拓扑序（开一个 `ans` 数组记录出队的顺序）以便输出答案。矛盾意味着这不是一个 DAG，也就会导致有的点的入度在拓扑排序结束后依然不为 $0$。如果所有的读入都完成后依然没有唯一的拓扑序（通过记录一个点是第几轮入队的，第 $n$ 轮就是唯一），那么就是有多解。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

#define pii pair<int, int>
#define X first 
#define Y second
#define mp make_pair

using namespace std;

int n, m, now, kase = 0;
int inn[30], in[30], ans[30];
bool s[30];
vector <int> G[30];

int topo(void) {
    for (int i = 0; i < 26; ++i) in[i] = inn[i];
    queue <pii> q;
    int sum = 0;
    for (int i = 0; i < 26; ++i)
        if (s[i] == true && in[i] == 0)
            q.push(mp(i, 1));
    int res = 0, tot = 0;
    while (!q.empty()) {
        int u = q.front().X, val = q.front().Y; q.pop();
        ans[++tot] = u;
        res = max(res, val);

        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i];
            --in[v];
            if (in[v] == 0) q.push(mp(v, val + 1));
        }
    }
    if (res == n) return 1;
    else {
        for (int i = 0; i < 26; ++i)
            if (s[i] && in[i] != 0) return 2;
    }
    return 3;
}

int main(void) {
    scanf("%d%d", &n, &m);
    char gc[5];
    while (m--) {
        ++kase;
        scanf("%s", gc);
        G[gc[0] - 'A'].push_back(gc[2] - 'A');
        ++inn[gc[2] - 'A'];
        s[gc[0] - 'A'] = true, s[gc[2] - 'A'] = true;
        int t = topo();
        if (t == 1) {
            printf("Sorted sequence determined after %d relations: ", kase);
            for (int i = 1; i <= n; ++i)
                printf("%c", ans[i] + 'A');
            printf(".\n");
            return 0;
        }
        else if (t == 2) {
            printf("Inconsistency found after %d relations.\n", kase);
            return 0;
        }
    }
    puts("Sorted sequence cannot be determined.");
    return 0;
}
```
{% endfolding %}

#### [Luogu P1113] 杂务

[Portal](https://www.luogu.com.cn/problem/P1113).

记 $f(i)$ 代表完成任务 $i$ 需要的最短时间。进行拓扑排序，统计一个节点的时候顺带更新它关联的 $f$ 值。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

int n;
int len[10005], in[10005], f[10005];
vector <int> G[10005];

void Kahn(void) {
    queue <int> q;
    for (int i = 1; i <= n; ++i)
        if (in[i] == 0) {
            q.push(i);
            f[i] = len[i];
        }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < G[u].size(); ++i) {
            --in[G[u][i]];
            if (in[G[u][i]] == 0) q.push(G[u][i]);
            f[G[u][i]] = max(f[G[u][i]], f[u] + len[G[u][i]]);
        }
    }
}

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        int x, y;
        scanf("%d%d", &x, len + i);
        while (scanf("%d", &y) == 1 && y) {
            G[y].push_back(x);
            ++in[x];
        }
    }
    Kahn();
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = max(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [HNOI2015] 菜肴制作

[Portal](https://www.luogu.com.cn/problem/P3243).

很容易想到拓扑排序，但是这个顺序？我们想要让小的编号尽量靠前，但是直接用小根堆就错了：因为这道题要保证的不是字典序，而是小的尽量靠前，即使一个大的出现在了它前面。这样的话发现一个越大的数，它越在后面越有利，因为这样小的就跑到前面去了。

那么，建反图，进行拓扑排序，使用 Kahn 算法配上一个大根堆，这样可以保证最终大的尽可能地晚出，把小的顶到前面。

**本题非常经典，强烈建议读者记住这个结论。**

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int n, m, in[100005], ans[100005];
vector <int> G[100005];

void Kahn(void) {
    int tot = 0;
    priority_queue <int> q;
    for (int i = 1; i <= n; ++i)
        if (in[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.top(); q.pop(); ans[++tot] = u;
        for (int i = 0; i < G[u].size(); ++i) {
            --in[G[u][i]];
            if (in[G[u][i]] == 0) q.push(G[u][i]);
        }
    }
    if (tot != n) puts("Impossible!");
    else {
        for (int i = n; i >= 1; --i)
            printf("%d ", ans[i]);
        putchar('\n');
    }
}

int main(void) {
    int T;
    scanf("%d", &T);
    while (T--) {
        memset(in, 0, sizeof(in));
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; ++i) G[i].clear();
        while (m--) {
            int u, v;
            scanf("%d%d", &u, &v);
            G[v].push_back(u);
            ++in[u];
        }
        Kahn();
    }
    return 0;
}
```
{% endfolding %}

### 连通性问题

这里的题都比较基础。

#### [Luogu P2002] 消息扩散

[Portal](https://www.luogu.com.cn/problem/P2002).

先求出 SCC，如果两个 SCC 相连，那么只需要发布一个消息就可以了。

{% folding cyan::查看代码 %}
```cpp
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
    if (low[x] == dfn[x]) {
        int y; ++cnt;
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
```
{% endfolding %}

#### [USACO03FALL] 受欢迎的牛 G

[Portal](https://www.luogu.com.cn/problem/P2341).

只有当出度为 $0$ 的 SCC 仅有一个时，才会有明星出现，数量是这个 SCC 的大小。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m;
int st[10005], tot = 0;
bool ins[10005];
int out[10005];
int c[10005], siz[10005], cnt = 0;
int dfn[10005], low[10005], num = 0;
vector <int> G[10005];

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    ins[st[++tot] = x] = true;
    for (int y : G[x])
        if (!dfn[y]) {
            tarjan(y);
            low[x] = min(low[x], low[y]);
        } else if (ins[y]) low[x] = min(low[x], dfn[y]);
    if (low[x] == dfn[x]) {
        int y; ++cnt;
        do {
            y = st[tot--]; ins[y] = false;
            siz[cnt] += 1; c[y] = cnt;
        } while (x != y);
    }
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].emplace_back(v);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i);
    for (int i = 1; i <= n; ++i)
        for (int j : G[i])
            if (c[i] != c[j]) ++out[c[i]];
    int ans = 0, flag = 0;
    for (int i = 1; i <= cnt; ++i)
        if (out[i] == 0) ans = siz[i], ++flag;
    if (flag > 1) puts("0");
    else printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 连通性的应用

这里的题都需要一些综合分析能力。

#### [Codeforces 51F] Caterpillar

[Portal](https://codeforces.com/problemset/problem/51/F).

{% noteblock %}
一个毛毛虫定义为一个无向联通无环图上存在一条路径 $p$ 使得任意一个节点距离 $p$ 的距离至多为 $1$。
毛毛虫可以包含自环（一条从一个顶点连向自己的边），但是不可以包含重边。

这个图片是一个毛毛虫的例子：

![](https://cdn.luogu.com.cn/upload/vjudge_pic/CF51F/c75bb54dfb4fc3a4d0709384b3d4f7a809015076.png)

现在你有一张无向图 $G$（不一定联通） 。你被允许做一些合并操作。

每次操作将两个顶点合并成一个顶点。每次选择任意两个顶点 $a,b (a\neq b)$，这些顶点以及它们的边（至少连接着 $a,b$ 中一个点的边）将被删除，而后顶点 $w$ 会被加入，以及对于每条边 $(x,a),(x,b)$ 都会有新边 $(x,w)$ 加入。如果有一条边 $(a,b)$ 它会被转换成自环 $(w,w)$。得到的图（操作结束后）可能会有重边。我们注意到这个操作减少了 $1$ 个顶点，却没有改变边的数量。
合并操作可以简单的描述为将图中两个顶点合并为图中的一个顶点并继承原来所有的边。你可以连续地使用合并操作，从而将给定的图转变成一个毛毛虫。求出这张图转变成一个毛毛虫的最少操作次数。
{% endnoteblock %}

毛毛虫上不能长出来环，所以把每一个 e-DCC 缩点，图会变成一个森林，我们需要处理每一棵树，然后把这些树合并，需要树的个数减去一的代价。

由于环必须要合并，如果要直接统计操作次数还需要统计环的大小，不妨换一个思路，默认所有点都需要合并，然后减去不需要合并的。

现在考虑最后一个问题，一棵树怎么处理？直觉告诉我们：这条路径 $p$ 应该是长度为 $d$ 直径，这样才能让要动的点更少。直径可以让我们少合并 $d$ 个点，叶子上的点也可以不用合并（画个图看看），但是直径两端还有两个叶子，所以要减去。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

struct edge {
    int u, v;
    edge(int u = 0, int v = 0) :
        u(u), v(v) {}
};
int n, m, cnt = 0, ans = 0, c[50005];
int num = 0, dfn[50005], low[50005];
int st[50005], tot = 0;
bool vis[50005];
vector <int> G[50005], F[50005];
vector <edge> edges;
inline void addedge(int u, int v) {
    edges.push_back(edge(u, v));
    G[u].push_back(edges.size() - 1);
}

void tarjan(int x, int in_edge) {
    dfn[x] = low[x] = ++num;
    st[++tot] = x;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = edges[G[x][i]].v;
        if (!dfn[y]) {
            tarjan(y, G[x][i]);
            low[x] = min(low[x], low[y]);
        } else if (G[x][i] != (in_edge ^ 1)) low[x] = min(low[x], dfn[y]);
    }
    if (low[x] == dfn[x]) {
        int y; ++cnt;
        do {
            y = st[tot--];
            c[y] = cnt;
        } while (x != y);
    }
}

vector <int> li;
int d[50005];
void dfs(int x, int fa) {
    d[x] = d[fa] + 1;
    for (int y : F[x])
        if (y != fa) dfs(y, x);
}
void find(int x) {
    li.emplace_back(x); vis[x] = true;
    for (int i : F[x]) if (!vis[i]) find(i);
}
void kill(int x) {
    if (F[x].empty()) return ans += 1, void();
    li.clear(); find(x);
    dfs(x, 0);
    int u = x, leaf = 0;
    for (int y : li) if (d[y] > d[u]) u = y;
    d[u] = 0; dfs(u, 0);
    for (int y : li) if (d[y] > d[u]) u = y;
    for (int y : li) leaf += (F[y].size() == 1);
    ans += d[u] + leaf - 2;
}

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v);
        addedge(v, u);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i, -1);
    for (int i = 0; i < edges.size(); ++i)
        if (c[edges[i].u] != c[edges[i].v]) F[c[edges[i].u]].push_back(c[edges[i].v]);
    int ret = -1;
    for (int i = 1; i <= n; ++i)
        if (!vis[i]) ++ret, kill(i);
    printf("%d\n", n - ans + ret);
    return 0;
}
```
{% endfolding %}