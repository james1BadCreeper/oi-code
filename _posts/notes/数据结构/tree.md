---
title: 简单树形问题
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 学习笔记
    - 数据结构
tag:
    - 树
    - LCA
    - 树的直径
    - 树的重心
    - 树链剖分
    - 树上差分
    - 笔记
headimg: https://s1.ax1x.com/2022/08/04/vm9ZYq.jpg
group: notes
hide: true
status: working
abbrlink: 85699f68
date: 2022-08-01 09:37:28
---

> 本文对简单的树形问题进行了讲解。

<!--more-->

有关树的问题在考试时非常常见，所以必须熟练掌握。本文介绍的问题都比较简单，不涉及什么高难的问题。

## 树的性质与遍历

我们知道，一棵树有 $n$ 个点，$n-1$ 条边，且一定是连通的。有几种特殊的树：

- 链：树退化成链式结构。
- “菊花图”：树的深度恰好为 $2$。

[[Luogu P5908] 猫猫和企鹅](https://www.luogu.com.cn/problem/P5908)。

可以简单的使用树的深度优先遍历来解决问题。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, d, ans = 0, dis[100005];
vector <int> G[100005];

void dfs(int o, int f) {
    if (dis[o] <= d && o != 1) ++ans;
    for (int i = 0; i < G[o].size(); ++i) {
        int &y = G[o][i];
        if (y == f) continue;
        dis[y] = dis[o] + 1;
        dfs(y, o);
    }
}

int main(void) {
    scanf("%d%d", &n, &d);
    for (int i = 1; i < n; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    dfs(1, 0);
    cout << ans << endl;
    return 0;
}
```
{% endfolding %}

**边权转点权**。实际上在树形问题中，边权非常不好处理，所以我们可以在深度优先遍历的时候将边权全放给儿子。大概像这样：

```cpp
d[y] = w; // 转移边权为儿子的点权
dfs(y, x);
```

## 树的直径

指的是树上的最长路径，可以通过两次 DFS 求出。第一次 DFS 从任意节点开始遍历，走到最远的地方，然后从这个地方开始第二次 DFS，走到最远的地方。这两个最远的地方连接起来就是树的直径。

[模板](https://www.luogu.com.cn/problem/SP1437)，代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, maxx = 0;
bool v[10005];
int dis[10005];
vector <int> G[10005];

void dfs(int o, int fa)
{
    for (int i = 0; i < G[o].size(); ++i) {
        if (G[o][i] == fa) continue;
        dis[G[o][i]] = dis[o] + 1;
        if (dis[G[o][i]] > dis[maxx]) maxx = G[o][i];
        dfs(G[o][i], o);
    }
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    dfs(1, -1);
    memset(v, 0, sizeof(v));
    dis[maxx] = 0;
    dfs(maxx, -1);
    printf("%d\n", dis[maxx]);
    return 0;
}
```

## 树的重心

对于树上的每一个点，计算其所有子树中最大的子树节点数，使得这个值最小的点就是这棵树的重心。树的重心有以下性质：

> 以树的重心为根时，所有子树的大小都不超过整棵树大小的一半。

使用反证法。设当前的重心为 $u$，与 $u$ 相连的子树 $v$ 的大小超过了整棵树的一半，那么将 $v$ 替换为树的重心，显然这时 $u$ 的子树不超过整棵树大小的一半，而 $v$ 的子树大小减小了 $1$，一定比 $u$ 作为重心更好。

> 树中所有点到某个点的距离和中，到重心的距离和是最小的；如果有两个重心，那么到它们的距离和一样。

因为如果移动了，增加的距离一定大于等于减少的距离。

> 把两棵树通过一条边相连得到一棵新的树，那么新的树的重心在连接原来两棵树的重心的路径上。

如果不在这条路径上，那么只有那个节点的子树的代价会减小，其它的都会增加，得不偿失。

> 在一棵树上添加或删除一个叶子，那么它的重心最多只移动一条边的距离。

增加或减少一个叶子，只能使最大的子树恰好比一半多 $1$，重心只移动 $1$ 即可。

{% divider water %}

现在我们来看如何求出树的重心。我们假定 $1$ 为根节点，然后设 $size[x]$ 代表 $x$ 的子树大小。我们定义 `max_part` 为当前 dfs 到的节点中，最大的子树大小。它的孩子们的子树大小在 dfs 时就可以统计，而剩下的一棵子树就是它父亲对应的子树，这就是 $n-size[x]$。这样只需要调用一次 dfs，时间复杂度为 $O(n)$。

```cpp
int n, pos, ans = 1e7; // pos 为重心，ans 为重心对应的最大子树
int s[105];
vector <int> G[105];

void dfs(int x, int fa)
{
    s[x] = 1;
    int max_part = 0;
    for (int i = 0; i < G[x].size(); ++i) {
        int y = G[x][i];
        if (y == fa) continue; // 想逃回父亲，直接枪毙
        dfs(y, x);
        s[x] += s[y]; // 父节点的子树大小加上子节点的
        max_part = max(max_part, s[y]); // 更新 max_part
    }
    max_part = max(max_part, n - s[x]); // 最后一棵子树是父亲节点对应的子树（这里的子树是指以 x 为根的情况）
    if (max_part < ans) // 答案更优就更新
    {
        ans = max_part;
        pos = x;
    }
}
```

学过树形 DP 的读者应该可以发现这个东西类似于换根 DP，但又不太一样。

## 最近公共祖先（LCA）

LCA 是指点集的 LCA，为了方便，我们记某点集 $A=\{u_i|1\leqslant i\leqslant n\}$ 的最近公共祖先为 $LCA(u_1,u_2,\ldots,u_n)$ 或 $LCA(A)$。含义是离它们最近的一个点，是它们所有点的祖先。

LCA 有以下性质：

1. $LCA(u)=u$；
2. $LCA(u,v)=u$ 的充要条件是 $u$ 是 $v$ 的祖先；
3. 如果 $u$ 不为 $v$ 的祖先并且 $v$ 不为 $u$ 的祖先，那么 $u,v$ 分别处于 $LCA(u,v)$ 的两棵不同子树中；
4. 给定一棵二叉树，前序遍历中，$LCA(S)$ 出现在所有 $S$ 中元素之前，后序遍历中 $LCA(S)$ 则出现在所有 $S$ 中元素之后；
5. 两点集并的最近公共祖先为两点集分别的最近公共祖先的最近公共祖先，即 $LCA(A \cup B) = LCA(LCA(A),LCA(B))$；
6. 两点的最近公共祖先必定处在树上两点间的最短路上，且 $dist(u,v)=h(u)+h(v)-2h(LCA(u,v))$，其中 $h(x)$ 指 $x$ 到树根的距离。

这些性质都比较显然，在此不做证明。

{% divider grass %}

现在我们来讨论 LCA 的求法。

LCA 有多种求法，不同情况要用不同的方法。   
可以在 [模板](https://www.luogu.com.cn/problem/P3379) 进行测试。

### 向上标记法

比如我们现在要求 $LCA(u,v)$，我们可以先让 $u$ 和 $v$ 向上跳到同一深度，然后让它们一起往上调，一定可以找到它们的 LCA。

```cpp
int n, m, root;
vector <int> G[500005];
bool v[500005];

struct node
{
    int p, fa, dep;
}T[500005];

void dfs(int o, int deep)
{
    v[o] = 1;
    T[o].dep = deep;
    for (int i = 0; i < G[o].size(); ++i)
    {
        if (!v[G[o][i]])
        {
            T[G[o][i]].fa = o;
            dfs(G[o][i], deep + 1);
        }
    }
}

int LCA(int x, int y)
{
    if (T[x].dep < T[y].dep) swap(x, y);
    while (T[x].dep > T[y].dep) x = T[x].fa; // 跳到同一深度
    if (x == y) return x; // 此处特判可以略去，但习惯写上
    while (x != y)
    {
        x = T[x].fa;
        y = T[y].fa;
    } // 一起往上跳
    return x;
}

int main(void)
{
    n = read(), m = read(), root = read();
    for (int i = 1; i < n; ++i)
    {
        int x = read(), y = read();
        G[x].push_back(y);
        G[y].push_back(x);
    }
    T[root].fa = -1;
    dfs(root, 1); // 构造树
    
    while (m--)
    {
        int a = read(), b = read();
        cout << LCA(a, b) << endl;
    }
    return 0;
}
```
    
### 树上倍增法

以上做法极慢，最常用的快速求 LCA 的方法是树上倍增法。设 $f[x,k]$ 表示 $x$ 的 $2^k$ 辈祖先。若该节点不存在，则令 $f[x,k]=-1$（不设为 $0$ 的原因是有的题需要设一个编号为 $0$ 的虚拟节点）。那么有 $f[x,k]=f[f[x,k-1]][k-1]$，接下来的思路跟向上标记法大致相同。

在求解 LCA 时，我们先让它们都跳到同一深度。如果此时两个点已经相等，那么这个点就是 LCA（此步不能略去，原因接下来会说明）。然后我们尝试着让它们一起往上跳，如果跳完值还不相等，那一定跳。最后再跳一步即可。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m, root;
int dep[500005];
int lg[500005];
int f[500005][20];
vector <int> G[500005];

void dfs(int o, int fa)
{
    f[o][0] = fa; // 根据定义
    dep[o] = dep[fa] + 1; // 深度为父亲 +1
    for (int i = 1; i <= lg[n]; ++i) // 跳出树的值都会变成 -1
        f[o][i] = f[f[o][i - 1]][i - 1];
    for (int i = 0; i < G[o].size(); ++i)
        if (G[o][i] != fa) dfs(G[o][i], o); // 如果不往父亲回，就以 G[o][i] 为儿子，o 为父亲 dfs
}

int LCA(int x, int y)
{
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = lg[n]; i >= 0; --i) // 从一个最大可能值开始枚举，这样做的正确性基于二进制拆分
        if (dep[f[x][i]] >= dep[y]) x = f[x][i]; // 如果跳这么大深度依然比 y 大，那只能跳
    if (x == y) return x; // 此步不能省去，否则已经是 LCA，最后 return 时还会再跳一次
    for (int i = lg[n]; i >= 0; --i)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i]; // 如果跳这么大都不相等，此时必须要跳
    return f[x][0]; // 最后再跳一步便一定是 LCA
}

int main(void)
{
    scanf("%d%d%d", &n, &m, &root);
    lg[1] = 0;
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 1; i < n; ++i)
    {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    dfs(root, -1); // 让 -1 作为根节点的“父亲“，使得 f 数组中跳出树的都变成 -1
    while (m--)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        printf("%d\n", LCA(x, y));
    }
    return 0;
}
```

除了求解 LCA 问题，很多树上问题都会用到树上倍增法。

### Tarjan 算法

Tarjan 算法可以在 $O(n+m)$ 的时间内解决这个问题。

## 树上前缀和与差分

前缀和和差分是线性结构上的有力工具，但是它们也可以搬到树上来。

### 树上前缀和

设 $S_i$ 代表根节点到节点 $i$ 的权值总和，那么：

- 如果是边权，那么 $d(x,y)=S_x+S_y-2\times S_{LCA(x,y)}$，
- 如果是点权，那么 $d(x,y)=S_x+S_y-S_{LCA(x,y)}-S_{fa[LCA(x,y)]}$（因为 LCA 处只能减一次）。

### 树上点差分

也就是对于点权的树上差分。也就是说，给定若干条路经，求出每个点经过的次数。那么：

```cpp
// s -> t
d[s]++, d[t]++;
d[lca(s, t)]--, d[fa[lca(s, t)]]--;
```

当然不同的数值也可以改。

[模板](https://www.luogu.com.cn/problem/P3128)。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, k, lg[50005], dep[50005];
int f[50005][18], sum[50005];
vector <int> G[50005];

void dfs(int x, int fa)
{
    f[x][0] = fa;
    dep[x] = dep[fa] + 1;   
    for (int i = 1; i <= lg[n]; ++i)
        f[x][i] = f[f[x][i - 1]][i - 1];
    for (int i = 0; i < G[x].size(); ++i)
        if (G[x][i] != fa) dfs(G[x][i], x);
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
        int y = G[x][i];
        if (y == fa) continue;
        get(y, x);
        sum[x] += sum[y];
    }
}

int main(void)
{
    scanf("%d%d", &n, &k);
    for (int i = 1, u, v; i < n; ++i)
    {
        scanf("%d%d", &u, &v);
        G[u].push_back(v), G[v].push_back(u);
    }
    for (int i = 1; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    dfs(1, 0);
    while (k--)
    {
        int s, t, lca;
        scanf("%d%d", &s, &t);
        lca = LCA(s, t);
        sum[s]++, sum[t]++;
        sum[lca]--, sum[f[lca][0]]--;
    }
    get(1, 0);
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = max(ans, sum[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 树上边差分

还是直接将边前缀和搬过来：

```cpp
d[s]++, d[t]++;
d[lca(s, t)] -= 2;
```

## DFS 序列

树在进行 DFS 时，会有入栈出栈的顺序，而且每一个树恰好入栈一次、出栈一次。这样产生的序列就是树的 DFS 序列，又被称之为欧拉序列（欧拉序）。

### 概述

比如这样一棵树：

![](https://pic1.imgdb.cn/item/633aef0b16f2c2beb17446ad.jpg)

它的 DFS 序列就是 `1 4 4 2 6 8 8 6 5 5 2 3 7 7 3 1`。同图论中的 DFS 遍历，

可以发现，DFS 序列有以下性质：

- 若树的大小为 $n$，那么 DFS 序列的长度就等于 $2n$，每个数恰好出现了两次。
- 每棵子树 $x$ 在 DFS 序列中一定是连续的一段，节点 $x$ 一定同时在这个连续段的两端。

DFS 序可以与树上差分结合起来，实现满足差分信息的树上信息高效维护。

### 单点修改

[模板](https://loj.ac/p/144).

> 单点增加，查询子树和。

根据 DFS 序列的性质，我们可以将树上信息转化到链上来维护。怎么转呢？并不需要让每一个树出现两次。可以发现，子树一定是在根后面连着的，那么我们记录 $siz$ 大小就可以了。

接下来就是 Fenwick 树了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#define lowbit(x) (x & -x)

using namespace std;
typedef long long i64;

int n, m, root;
int a[1000005], siz[1000005];
int dfn[1000005], num = 0;
vector <int> G[1000005];

i64 C[1000005];
void add(int x, int k) {
    while (x <= n) {
        C[x] += k;
        x += lowbit(x);
    }
}
i64 sum(int x) {
    i64 res = 0;
    while (x) {
        res += C[x];
        x -= lowbit(x);
    }
    return res;
}

void dfs(int x, int fa) {
    dfn[x] = ++num; siz[x] = 1;
    add(num, a[x]);
    for (auto y : G[x]) {
        if (y == fa) continue;
        dfs(y, x);
        siz[x] += siz[y];
    }
}

int main(void) {
    scanf("%d%d%d", &n, &m, &root);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i);
    for (int i = 1; i < n; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    dfs(root, 0);
    while (m--) {
        int op, a, x;
        scanf("%d%d", &op, &a);
        if (op == 1) {
            scanf("%d", &x);
            add(dfn[a], x);
        } else {
            printf("%lld\n", sum(dfn[a] + siz[a] - 1) - sum(dfn[a] - 1));
        }
    }
    return 0;
}
```
{% endfolding %}

### 子树修改

[模板](https://loj.ac/p/145).

> 子树所有节点增加 $x$，子树节点和。

实际上是一样的，我们只需要将树状数组替换为线段树（当然，利用拆分信息树状数组也可以做）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
typedef long long i64;

int n, m, root;
int a[1000005], siz[1000005];
int dfn[1000005], idx[1000005], num = 0;
vector <int> G[1000005];

i64 T[4000005];
int tag[4000005];

void build(int o, int l, int r) {
    if (l == r) return T[o] = a[idx[l]], void();
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    T[o] = T[o << 1] + T[o << 1 | 1];
}
inline void pushdown(int o, int l, int r) {
    if (!tag[o]) return;
    int mid = l + r >> 1;
    T[o << 1] += 1ll * (mid - l + 1) * tag[o], T[o << 1 | 1] += 1ll * (r - mid) * tag[o];
    tag[o << 1] += tag[o], tag[o << 1 | 1] += tag[o];
    tag[o] = 0;
}
void update(int o, int l, int r, int x, int y, int k) {
    if (x <= l && r <= y) return T[o] += 1ll * (r - l + 1) * k, tag[o] += k, void();
    int mid = l + r >> 1; pushdown(o, l, r);
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    T[o] = T[o << 1] + T[o << 1 | 1];
}
i64 query(int o, int l, int r, int x, int y) {
    if (x <= l && r <= y) return T[o];
    int mid = l + r >> 1; i64 res = 0; pushdown(o, l, r);
    if (x <= mid) res += query(o << 1, l, mid, x, y);
    if (mid < y) res += query(o << 1 | 1, mid + 1, r, x, y);
    return res;
}

void dfs(int x, int fa) {
    dfn[x] = ++num; idx[num] = x; siz[x] = 1; 
    for (auto y : G[x]) {
        if (y == fa) continue;
        dfs(y, x);
        siz[x] += siz[y];
    }
}

int main(void) {
    scanf("%d%d%d", &n, &m, &root);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i);
    for (int i = 1; i < n; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    dfs(root, 0);
    build(1, 1, n);
    while (m--) {
        int op, a, x;
        scanf("%d%d", &op, &a);
        if (op == 1) {
            scanf("%d", &x);
            update(1, 1, n, dfn[a], dfn[a] + siz[a] - 1, x);
        } else {
            printf("%lld\n", query(1, 1, n, dfn[a], dfn[a] + siz[a] - 1));
        }
    }
    return 0;
}
```
{% endfolding %}

### 链上修改

这里简单提一下，有兴趣可以写一下[模板](https://loj.ac/p/146)。

我们说过，欧拉序列维护的依旧是前缀和，所以利用树上差分的方式，配合树状数组可以快速修改与查询，会比重链剖分少一个 $\log$。

但是当维护的内容不满足差分的区间可减性，欧拉序列就做不了了。

## 树链与树链剖分

我们学过的很多内容，比如线段树，都只能处理序列，也就是链上的问题。当它跑到了树上，我们还可以使用 DFS 序列来进行处理。但是当维护的信息不满足差分性质后，DFS 序就显得无力了。这是候怎么办？要对树链进行处理了。

### 概念

我们先简单介绍一下相关概念。

**树链**是指树上的一条链，**树链剖分**就是将整棵树剖分成若干条链，使它组合成线性结构，然后可以在线性结构上工作的强大数据结构就可以派上用场了，也简称树剖。

树链剖分有多种形式，比如**重链剖分**，**长链剖分**和**实链剖分**，其中重链剖分最为常用，大部分树剖指的都是它。

由于某些原因，暂时只介绍最常用的重链剖分。长链剖分有时间会回来写。

### 重链剖分

我们先来看一下这个问题：

> 你需要写一种数据结构，支持区间修改和区间查询。

这个我当然会！直接线段树敲上去不久完事了嘛！

那么再来一个：

> 给出一棵树，支持链上修改，链上查询，子树修改，子树查询。

这是什么{% emoji tong gugugu %}？

这个题是有的，[模板](https://www.luogu.com.cn/problem/P3384)。

我们不会这种题，我们只会在链上做，那就需要使用树链剖分。

重链剖分可以将树上的任意一条路径划分成长度不超过 $O(\log n)$ 的连续链，每条链上的点深度互不相同（即自底向上的一条链，链上所有点的 LCA 为链的深度小的那个端点）。

我们首先需要了解几个概念：

- **子树的大小**：子树中节点的个数。
- **重子节点（重节点）**：表示其子节点中子树最大的子结点。如果有多个子树最大的子结点，取其一。如果没有子节点，就无重子节点。
- **轻子节点（轻节点）**：除了重子节点外的所有节点。特别地，树根是轻节点。
- **重边**：连接节点到它的重子节点的边。
- **轻边**：除了重边之外的所有边。
- **重链**：若干条首尾连接的重边，也就是说，一条重链的开头是一个轻节点，剩下的都是重节点。特别地，一个落单的节点也是重链。

这样整棵树就被剖分成若干条重链，可以证明链的规模是 $O(\log n)$ 的。

实现时要通过两次 dfs，大概像这样：

```cpp
int f[100005], son[100005], top[100005]; // son 指重儿子，没有为 -1；top 指重链的顶端节点的标号
int dep[100005], siz[100005], dfn[100005], tot = 0; // dfn 指时间戳
vector <int> G[100005];

void dfs1(int x, int fa)
{
    dep[x] = dep[fa] + 1; f[x] = fa; siz[x] = 1;
    int maxx = -1;
    for (auto y : G[x])
        if (y != fa)
        {
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > maxx) son[x] = y, maxx = siz[y];
        }
}

void dfs2(int x, int topf) // topf 记录这个重链的顶点
{
    dfn[x] = ++tot; top[x] = topf;
    if (son[x] == -1) return;
    dfs2(son[x], topf); // 优先处理重链
    for (auto y : G[x])
        if (y != f[x] && y != son[x]) dfs2(y, y); // 遍历轻儿子
}
```

注意到重链的处理总是优先的，也就是说，重链内的时间戳编号是连续的，那么就决定了我们维护的时候直接对应了一段序列上的区间。

建立一棵线段树，以 $x$ 到 $y$ 的最短路径上加上 $z$ 为例，像这样：

```cpp
while (top[x] != top[y]) // 如果它们不在一条重链上
{
    if (dep[top[x]] < dep[top[y]]) swap(x, y); // 要计算深的节点
    update(1, 1, n, dfn[top[x]], dfn[x], z); // 更新
    x = f[top[x]]; // 跳上来，注意重链的头部已经修改过了，跳到重链头的父亲上
}
if (dep[x] > dep[y]) swap(x, y);
update(1, 1, n, dfn[x], dfn[y], z); // 现在已经在一条重链上，更新
```

发没发现这个处理特别像 LCA 的求解？没错，LCA 确实也可以用树链剖分来求解，但是不如倍增简单，除非恰好这道题目需要用到树链剖分，我们才会使用树链剖分来求解 LCA。

那么对于子树的操作呢？由于是深度优先遍历，所以一棵子树内的时间戳编号也是连续的（依然具有 DFS 序的性质），也可以直接使用线段树维护：

```cpp
update(1, 1, n, dfn[x], dfn[x] + siz[x] - 1, k); // 利用子树的大小直接计算
```

那么完整代码就很简单了：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, m, root, P;
int w[100005], f[100005], son[100005], top[100005];
int dep[100005], siz[100005], dfn[100005], tot = 0, a[100005];
vector <int> G[100005];

void dfs1(int x, int fa)
{
    dep[x] = dep[fa] + 1; f[x] = fa; siz[x] = 1;
    int maxx = -1;
    for (auto y : G[x])
        if (y != fa)
        {
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > maxx) son[x] = y, maxx = siz[y];
        }
}

void dfs2(int x, int topf)
{
    dfn[x] = ++tot; top[x] = topf; a[tot] = w[x];
    if (son[x] == -1) return;
    dfs2(son[x], topf);
    for (auto y : G[x])
        if (y != f[x] && y != son[x]) dfs2(y, y);
}

int T[400005], tag[400005];
inline void maintain(int o) { T[o] = (T[o << 1] + T[o << 1 | 1]) % P; }
inline void pushdown(int o, int l, int r)
{
    if (!tag[o]) return;
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    tag[ls] = (tag[ls] + tag[o]) % P, tag[rs] = (tag[rs] + tag[o]) % P;
    T[ls] = (T[ls] + 1ll * tag[o] * (mid - l + 1)) % P, T[rs] = (T[rs] + 1ll * tag[o] * (r - mid)) % P;
    tag[o] = 0;
}
void build(int o, int l, int r)
{
    if (l == r) {
        T[o] = a[l] % P;
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o);
}
void update(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y) {
        T[o] = (T[o] + 1ll * k * (r - l + 1)) % P;
        tag[o] = (tag[o] + k) % P;
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    maintain(o);
}
int query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    pushdown(o, l, r);
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res = (res + query(o << 1, l, mid, ql, qr)) % P;
    if (mid < qr) res = (res + query(o << 1 | 1, mid + 1, r, ql, qr)) % P;
    return res;
}

int main(void)
{
    memset(son, 0xff, sizeof(son));
    scanf("%d%d%d%d", &n, &m, &root, &P);
    for (int i = 1; i <= n; ++i) scanf("%d", w + i);
    for (int i = 1, u, v; i < n; ++i)
    {
        scanf("%d%d", &u, &v);
        G[u].push_back(v), G[v].push_back(u);
    }
    dfs1(root, 0);
    dfs2(root, root);
    build(1, 1, n);
    while (m--)
    {
        int k, x, y, z;
        scanf("%d", &k);
        if (k == 1)
        {
            scanf("%d%d%d", &x, &y, &z);
            z %= P;
            while (top[x] != top[y])
            {
                if (dep[top[x]] < dep[top[y]]) swap(x, y);
                update(1, 1, n, dfn[top[x]], dfn[x], z);
                x = f[top[x]];
            }
            if (dep[x] > dep[y]) swap(x, y);
            update(1, 1, n, dfn[x], dfn[y], z);
        }
        else if (k == 2)
        {
            scanf("%d%d", &x, &y);
            int ans = 0;
            while (top[x] != top[y])
            {
                if (dep[top[x]] < dep[top[y]]) swap(x, y);
                ans = (ans + query(1, 1, n, dfn[top[x]], dfn[x])) % P;
                x = f[top[x]];
            }
            if (dep[x] > dep[y]) swap(x, y);
            ans = (ans + query(1, 1, n, dfn[x], dfn[y])) % P;
            printf("%d\n", ans);
        }
        else if (k == 3)
        {
            scanf("%d%d", &x, &k);
            update(1, 1, n, dfn[x], dfn[x] + siz[x] - 1, k);
        }
        else
        {
            scanf("%d", &x);
            printf("%d\n", query(1, 1, n, dfn[x], dfn[x] + siz[x] - 1));
        }
    }
    return 0;
}
```
{% endfolding %}

算法理论实践复杂度为 $O(n\log^2 n)$，但实际上远远达不到上界，重链剖分的常数很小。

但即使如此，树链剖分也是比 DFS 序慢的。如果能使用 DFS 序，更推荐用它。

### 树链剖分求 LCA

之前维护链的过程很像求 LCA 往上跳的过程，所以重链剖分也可以用来求 LCA。

```cpp
int LCA(int x, int y)
{
    while (top[x] != top[y]) // 不在一条重链上
    {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        x = f[top[x]];
    }
    return dep[x] < dep[y] ? x : y;
}
```

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, m, root;
vector <int> G[500005];
int f[500005], son[500005], top[500005];
int dep[500005], siz[500005];

void dfs1(int x, int fa)
{
    dep[x] = dep[fa] + 1; f[x] = fa; siz[x] = 1;
    int maxx = -1;
    for (auto y : G[x])
        if (y != fa)
        {
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > maxx) son[x] = y, maxx = siz[y];
        }
}

void dfs2(int x, int topf)
{
    top[x] = topf;
    if (son[x] == -1) return;
    dfs2(son[x], topf);
    for (auto y : G[x])
        if (y != f[x] && y != son[x]) dfs2(y, y);
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
    scanf("%d%d%d", &n, &m, &root);
    for (int i = 1, u, v; i < n; ++i)
    {
        scanf("%d%d", &u, &v);
        G[u].push_back(v), G[v].push_back(u);
    }
    dfs1(root, 0);
    dfs2(root, root);
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

一般来讲树链剖分会比倍增算法快一点，常数也很小（想想那个二维数组吧）。

## 树上启发式合并

## Problemset

我们来看一些有趣的题目，前面的一些题目都很简单，后面的部分题目相当复杂。

### 简单树形问题

不涉及什么算法，只需要树的有关知识，以及求解树的直径和树的重心的方法等即可。

#### [JLOI2012] 树

[Portal](https://www.luogu.com.cn/problem/P3252).

直接 dfs 遍历树即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, c, ans = 0, a[100005];
vector <int> son[100005];

void dfs(int o, int sum)
{
    if (sum == c) return ++ans, void();
    if (sum > c) return;
    for (auto x : son[o])
        dfs(x, sum + a[x]);
}

int main(void)
{
    scanf("%d%d", &n, &c);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    for (int i = 1, u, v; i < n; ++i)
    {
        scanf("%d%d", &u, &v);
        son[u].push_back(v);
    }
    for (int i = 1; i <= n; ++i)
        dfs(i, a[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [YsOI2020] 植树

[Portal](https://www.luogu.com.cn/problem/P6591).

类似于求树的重心，不过要统计的是子树的大小。注意 $1$ 号节点肯定是可以的。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, s[1000005];
bool flag[1000005];
vector <int> G[1000005];

void dfs(int x, int fa)
{
    int num = 0;
    s[x] = 1;
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i];
        if (y == fa) continue;
        dfs(y, x);
        s[x] += s[y];
        if (!num) num = s[y];
        if (num != s[y]) flag[x] = true;
    }
    if (x != 1 && num && num != n - s[x]) flag[x] = true;
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; ++i)
    {
        scanf("%d%d", &u, &v);
        G[u].push_back(v), G[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; ++i)
        if (!flag[i]) printf("%d ", i);
    putchar('\n');
    return 0;
}
```
{% endfolding %}

#### [NOIP2014 提高组] 联合权值

[Portal](https://www.luogu.com.cn/problem/P1351).

由于距离为 $2$，所以枚举每一个点，与它相邻的点两两互为联合点，然后使用 $n$ 项式的平方计算即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

#define MOD 10007

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n;
int W[200005];
vector <int> G[200005];

int main(void)
{
    n = read();
    for (int i = 1; i < n; ++i)
    {
        int u = read(), v = read();
        G[u].push_back(v);
        G[v].push_back(u);
    }
    for (int i = 1; i <= n; ++i) W[i] = read();
    int anssum = 0, ansmax = 0;
    for (int i = 1; i <= n; ++i)
    {
        int res = 0, ret = 0;
        int maxx1 = 0, maxx2 = 0;
        for (int j = 0; j < G[i].size(); ++j)
        {
            #define pocket W[G[i][j]]
            if (pocket > maxx1) maxx2 = maxx1, maxx1 = pocket;
            else if (pocket > maxx2) maxx2 = pocket;
            res = (res + pocket) % MOD;
            ret = (ret + pocket * pocket) % MOD;
        }
        anssum = (anssum + res * res % MOD - ret + MOD) % MOD;
        ansmax = max(ansmax, maxx1 * maxx2);
    }
    printf("%d %d\n", ansmax, anssum);
    return 0;
}
```
{% endfolding %}

#### [NOI2011] 道路修建

[Portal](https://www.luogu.com.cn/problem/P2052).

我们采用类似求树的重心的方法。统计一条边的费用时，一部分大小是子树的大小，另一部分是 $n$ 减去这个子树的大小。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

#define i64 long long

using namespace std;

struct edge
{
    int v, d;
    edge(int v = 0, int d = 0) :
        v(v), d(d) {}
};

int n;
i64 ans;
int s[1000005];
vector <edge> G[1000005];

void dfs(int x, int fa)
{
    s[x] = 1;
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i].v, w = G[x][i].d;
        if (y == fa) continue;
        dfs(y, x);
        ans += (i64)w * abs(s[y] - (n - s[y]));
        s[x] += s[y];
    }
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i < n; ++i)
    {
        int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        G[u].push_back(edge(v, d));
        G[v].push_back(edge(u, d));
    }
    dfs(1, 0);
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1395] 会议

[Portal](https://www.luogu.com.cn/problem/P1395).

树中所有点到某个点的距离和中，到重心的距离和是最小的。我们只需要先求出树的重心，然后用 dfs 计算距离即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, ans = 1e7, pos, s[50005];
vector <int> G[50005];

void dfs(int x, int fa)
{
    s[x] = 1;
    int max_part = 0;
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i];
        if (y == fa) continue;
        dfs(y, x);
        s[x] += s[y];
        max_part = max(max_part, s[y]);
    }
    max_part = max(max_part, n - s[x]);
    if (max_part < ans || (max_part == ans && x < pos))
    {
        pos = x;
        ans = max_part;
    }
}

int sum = 0, dep[50005];
void dfs2(int x, int fa)
{
    dep[x] = dep[fa] + 1;
    sum += dep[x];
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i];
        if (y == fa) continue;
        dfs2(y, x);
    }
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i < n; ++i)
    {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    dfs(1, 0);
    dep[0] = -1;
    dfs2(pos, 0);
    printf("%d %d\n", pos, sum);
    return 0;
}
```
{% endfolding %}

### LCA 的综合应用

包括 LCA、树上差分等内容。有的时候也会与别的算法综合（比如二分），但不涉及高难的算法。

#### [Luogu P3938] 斐波那契

[Portal](https://www.luogu.com.cn/problem/P3938).

树的规模很大，但是深度很小，我们考虑不建树使用向上标记法。由于 `dep` 肯定是编号越大的节点越大，因此现在的问题就是有如何求解一个节点的爸爸。

我们预处理出斐波那契数列 $F$。一只兔子的编号是 $F_x+F_{x-1}+1$，它儿子的编号就是 $F_x+F_x+F_{x-1}+1$，所以我们只需要不断减去比它小的一在数列中的编号即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
typedef long long i64;
const i64 MAXL = 1e12;

i64 f[105] = {0, 1};

int main(void)
{
    int i;
    for (i = 2;; ++i)
    {
        f[i] = f[i - 1] + f[i - 2];
        if (f[i] > MAXL) break;
    }

    int n;
    scanf("%d", &n);
    while (n--)
    {
        i64 a, b;
        scanf("%lld%lld", &a, &b);
        while (a != b)
        {
            if (a > b) swap(a, b);
            b -= (*(lower_bound(f, f + i + 1, b) - 1));
        }
        printf("%lld\n", a);
    }
    return 0;
}
```
{% endfolding %}

#### [JLOI2014] 松鼠的新家

[Portal](https://www.luogu.com.cn/problem/P3258).

直接使用树上点差分即可，但是要注意处理重复的情况。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, a[300005], dep[300005];
int f[300005][35], sum[300005];
vector <int> G[300005];

void dfs(int x, int fa)
{
    f[x][0] = fa;
    dep[x] = dep[fa] + 1;   
    for (int i = 1; i <= 30; ++i)
        f[x][i] = f[f[x][i - 1]][i - 1];
    for (auto y : G[x])
        if (y != fa) dfs(y, x);
}

int LCA(int x, int y)
{
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 30; i >= 0; --i)
        if (dep[f[x][i]] >= dep[y]) x = f[x][i];
    if (x == y) return x;
    for (int i = 30; i >= 0; --i)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}

void get(int x, int fa)
{
    for (auto y : G[x])
        if (y != fa)
        {
            get(y, x);
            sum[x] += sum[y];
        }
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)   
        scanf("%d", a + i);
    for (int i = 1, u, v; i < n; ++i)
    {
        scanf("%d%d", &u, &v);
        G[u].push_back(v); G[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i < n; ++i)
    {
        int lca = LCA(a[i], a[i + 1]);
        sum[a[i]]++; sum[a[i + 1]]++;
        sum[lca]--; sum[f[lca][0]]--;
    }
    get(1, 0);
    for (int i = 2; i <= n; ++i) --sum[a[i]];
    for (int i = 1; i <= n; ++i) printf("%d\n", sum[i]);
    return 0;
}
```
{% endfolding %}

#### [BJOI2018] 求和

[Portal](https://www.luogu.com.cn/problem/P4427).

注意到 $k$ 的范围很小，因此可以将不同的 $k$ 分开来做，那么这道题就成了树上前缀和的模板题。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
typedef long long i64;
const int MOD = 998244353;

int n, m, lg[300005];
int f[300005][25], dep[300005];
i64 w[55][300005];
vector <int> G[300005];

void dfs(int x, int fa)
{
	if (fa == 0) dep[x] = 0;
	else dep[x] = dep[fa] + 1;
	f[x][0] = fa; w[1][x] = dep[x];
	for (int i = 2; i <= 50; ++i) w[i][x] = w[i - 1][x] * dep[x] % MOD;
	for (int i = 1; i <= lg[n]; ++i)
		f[x][i] = f[f[x][i - 1]][i - 1];
	for (auto y : G[x])
		if (y != fa) dfs(y, x);
}

void df5(int x, int fa)
{
    for (auto y : G[x])
        if (y != fa)
        {
            for (int i = 1; i <= 50; ++i) w[i][y] = (w[i][y] + w[i][x]) % MOD;
            df5(y, x);
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

int main(void)
{
	scanf("%d", &n);
	for (int i = 1, u, v; i < n; ++i)
	{
		scanf("%d%d", &u, &v);
		G[u].push_back(v), G[v].push_back(u);
	}
	for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
	dfs(1, 0);
    df5(1, 0);
	scanf("%d", &m);
	while (m--)
	{
		int x, y, k;
		scanf("%d%d%d", &x, &y, &k);
        int lca = LCA(x, y);
        printf("%lld\n", ((w[k][x] + w[k][y] - w[k][lca] - w[k][f[lca][0]]) % MOD + MOD) % MOD);
	}
	return 0;
}
```
{% endfolding %}

#### [NOIP2015 提高组] 运输计划

[Portal](https://www.luogu.com.cn/problem/P2680).

像这种要求最大值最小的问题显然想到二分答案，然后使用树上边差分进行 `check` 即可，就找一个虫洞在所有不满足条件的运输计划上，取最大的一个来判断是否可行。

{% folding cyan::查看代码 %}
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

#### [NOIP2013 提高组] 货车运输

[Portal](https://www.luogu.com.cn/problem/P1967).

直觉告诉我们，走的路应该在最大生成树上。那我们先求出生成树，然后预处理 LCA，要记 `w[x][i]` 代表 $x$ 向上蹦 $2^i$ 次所遇到的最小边权，然后直接做就行了。

{% folding cyan::查看代码 %}
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

#### [NOIP2016 提高组] 天天爱跑步

[Portal](https://www.luogu.com.cn/problem/P1600).

{% noteblock %}
一棵包含 $n$ 个点的树，有 $m$ 个玩家，第 $i$ 个玩家的起点为 $s_i$，终点为 $t_i$。所有玩家在第 $0$ 秒同时从自己的起点出发，以每秒跑一条边的速度向着终点跑去。

每个结点上都放置了一个观察员。在结点 $j$ 的观察员会选择在第 $w_j$ 秒观察玩家，一个玩家能被这个观察员观察到当且仅当该玩家恰好在第 $w_j$ 秒也正好到达了结点 $j$ 。问每个观察员会观察到多少人？
{% endnoteblock %}

一个玩家能够被观察员 $x$ 观察到，当且仅当：

- 这个观察员在 $[s,LCA(s,t)]$ 上，那么需要满足 $dep[s]-dep[x]=w[x]$，相当于 $dep[s]=dep[x]+w[x]$。
- 这个观察员在 $(LCA(s,t),t]$ 上，那么需要满足 $dep[s]+dep[x]-2\times dep[LCA(s,t)]=w[x]$，相当于 $dep[s]-2\times dep[LCA(s,t)]=w[x]-dep[x]$。

这个模型就很清晰了，因为右面的信息只和观察员的位置有关。因此对于每一个玩家，我们都使用树上差分的思路。将玩家拆分成两个（一个从 $s$ 到 $lca$，一个从 $t$ 到 $lca$），然后把差分维护操作，给每一个节点都加上一种“物品”。放置相应节点编号的到 STL vector 中。然后进行 DFS。

建立数组 $c1,c2$ 来维护树上前缀和（因为有两种，分开维护比较方便）。使用 DFS 来统计树上前缀和，刚才已经把差分的操作放置到了 STL vector 中，我们只需要将这些操作执行，统计前后 $c$ 的变化就可以得到当前节点的答案了。

注意编号可能是负的，需要平移。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
const int N = 300005;

int read(void) {
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int n, m, f[N][20];
int dep[N], lg[N];
int w[N], ans[N], c1[N * 2], c2[N * 2];
vector <int> G[N];
vector <int> a1[N], b1[N], a2[N], b2[N];

void dfs(int x, int fa) {
    f[x][0] = fa; dep[x] = dep[fa] + 1;
    for (int i = 1; i <= lg[n]; ++i)
        f[x][i] = f[f[x][i - 1]][i - 1];
    for (auto y : G[x])
        if (y != fa) dfs(y, x);
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

void dfs2(int x, int fa) {
    int val1 = c1[w[x] + dep[x]], val2 = c2[w[x] - dep[x] + n];
    for (auto y : G[x]) {
        if (y == fa) continue;
        dfs2(y, x);
    }
    for (auto i : a1[x]) c1[i]++;
    for (auto i : b1[x]) c1[i]--;
    for (auto i : a2[x]) c2[i + n]++;
    for (auto i : b2[x]) c2[i + n]--;
    ans[x] = c1[w[x] + dep[x]] - val1 + c2[w[x] - dep[x] + n] - val2;
}

int main(void) {
    n = read(), m = read();
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 1, u, v; i < n; ++i) {
        u = read(), v = read();
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; ++i) w[i] = read();
    while (m--) {
        int s = read(), t = read();
        int lca = LCA(s, t);
        a1[s].emplace_back(dep[s]);
        b1[f[lca][0]].emplace_back(dep[s]);
        a2[t].emplace_back(dep[s] - 2 * dep[lca]);
        b2[lca].emplace_back(dep[s] - 2 * dep[lca]);
    }
    dfs2(1, 0);
    for (int i = 1; i <= n; ++i) printf("%d ", ans[i]);
    putchar('\n');
    return 0;
}
```
{% endfolding %}

### 重链剖分

重链剖分的内容很多，因为线性结构上可以工作的强大数据结构都可以放到树上工作了。但是限制于本文难度的定位，这里的题目都不会很难。

**但是一定要注意**！重链剖分不能学傻了，有些简单问题是直接可以用树上前缀和与差分、或者欧拉序来解决的。而且不要看见复杂树上问题就开始剖树了，还有树上倍增法可以使用！

这里的题目有的是 DFS 序列可以做的。

#### 重链剖分模板合集

这里是一些板子题，可以练习一下。

{% tabs %}

<!-- tab [ZJOI2008] 树的统计 -->

[Portal](https://www.luogu.com.cn/problem/P2590).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, q;
int a[30005], w[30005];
int f[30005], son[30005], dep[30005];
int siz[30005], dfn[30005], tot = 0, top[30005];
vector <int> G[30005];

void dfs1(int x, int fa)
{
    dep[x] = dep[fa] + 1; f[x] = fa, siz[x] = 1;
    int maxx = -1;
    for (int y : G[x])
        if (y != fa) {
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > maxx) son[x] = y, maxx = siz[y];
        }
}

void dfs2(int x, int topf)
{
    dfn[x] = ++tot; a[tot] = w[x]; top[x] = topf;
    if (son[x] == -1) return;
    dfs2(son[x], topf);
    for (int y : G[x])
        if (y != f[x] && y != son[x]) dfs2(y, y);
}

int sum[120005], maxx[120005];
inline void maintain(int o)
{
    sum[o] = sum[o << 1] + sum[o << 1 | 1];
    maxx[o] = max(maxx[o << 1], maxx[o << 1 | 1]);
}
void build(int o, int l, int r)
{
    if (l == r) {
        sum[o] = maxx[o] = a[l];
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o);
}
void update(int o, int l, int r, int x, int k)
{
    if (l == r) {
        sum[o] = maxx[o] = k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, k);
    else update(o << 1 | 1, mid + 1, r, x, k);
    maintain(o);
}
int queryMAX(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return maxx[o];
    int mid = l + r >> 1, res = -40000;
    if (ql <= mid) res = max(res, queryMAX(o << 1, l, mid, ql, qr));
    if (mid < qr) res = max(res, queryMAX(o << 1 | 1, mid + 1, r, ql, qr));
    return res;
}
int querySUM(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return sum[o];
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res += querySUM(o << 1, l, mid, ql, qr);
    if (mid < qr) res += querySUM(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

int main(void)
{
    memset(son, 0xff, sizeof(son));
    scanf("%d", &n);
    for (int i = 1, u, v; i < n; ++i) {
        scanf("%d%d", &u, &v);
        G[u].push_back(v), G[v].push_back(u);
    }
    for (int i = 1; i <= n; ++i) scanf("%d", w + i);
    dfs1(1, 0);
    dfs2(1, 1);
    build(1, 1, n);
    scanf("%d", &q);
    char s[10]; int x, y;
    while (q--) {
        scanf("%s%d%d", s, &x, &y);
        if (s[0] == 'C') update(1, 1, n, dfn[x], y);
        else if (s[1] == 'M') {
            int ans = -40000;
            while (top[x] != top[y]) {
                if (dep[top[x]] < dep[top[y]]) swap(x, y);
                ans = max(ans, queryMAX(1, 1, n, dfn[top[x]], dfn[x]));
                x = f[top[x]];
            }
            if (dep[x] > dep[y]) swap(x, y);
            ans = max(ans, queryMAX(1, 1, n, dfn[x], dfn[y]));
            printf("%d\n", ans);
        } else {
            int ans = 0;
            while (top[x] != top[y]) {
                if (dep[top[x]] < dep[top[y]]) swap(x, y);
                ans += querySUM(1, 1, n, dfn[top[x]], dfn[x]);
                x = f[top[x]];
            }
            if (dep[x] > dep[y]) swap(x, y);
            ans += querySUM(1, 1, n, dfn[x], dfn[y]);
            printf("%d\n", ans);
        }
    }
    return 0;
}
```
{% endfolding %}

<!-- endtab -->

<!-- tab [Codeforces 343D] Water Tree -->

[Portal](https://codeforces.com/problemset/problem/343/D).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, q, num = 0;
vector <int> G[500005];
int dfn[500005], top[500005], siz[500005];
int son[500005], f[500005], dep[500005];

void dfs1(int x, int fa) {
	f[x] = fa; dep[x] = dep[fa] + 1; siz[x] = 1;
	int max_part = -1;
	for (int i = 0; i < G[x].size(); ++i) {
		int y = G[x][i];
		if (y != fa) {
			dfs1(y, x);
			siz[x] += siz[y];
			if (siz[y] > max_part) max_part = siz[y], son[x] = y;
		}
	}
}

void dfs2(int x, int topf) {
	dfn[x] = ++num; top[x] = topf;
	if (son[x] == -1) return;
	dfs2(son[x], topf);
	for (int i = 0; i < G[x].size(); ++i) {
		int y = G[x][i];
		if (y != f[x] && y != son[x]) dfs2(y, y);
	}
}

int tag[2000005];

inline void pushdown(int o) {
	if (tag[o] == -1) return;
	tag[o << 1] = tag[o], tag[o << 1 | 1] = tag[o];
	tag[o] = -1;
}

void update(int o, int l, int r, int x, int y, int k) {
	if (x <= l && r <= y) {
		tag[o] = k;
		return;
	}
	pushdown(o);
	int mid = l + r >> 1;
	if (x <= mid) update(o << 1, l, mid, x, y, k);
	if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
}

int query(int o, int l, int r, int x) {
	if (l == r) return (tag[o] == -1 ? 0 : tag[o]);
	int mid = l + r >> 1;
	pushdown(o);
	if (x <= mid) return query(o << 1, l, mid, x);
	return query(o << 1 | 1, mid + 1, r, x);
}

int main(void) {
	memset(son, 0xff, sizeof(son));
	memset(tag, 0xff, sizeof(tag));
	scanf("%d", &n);
	for (int i = 1, u, v; i < n; ++i) {
		scanf("%d%d", &u, &v);
		G[u].push_back(v), G[v].push_back(u);
	}
	dfs1(1, 0);
	dfs2(1, 1);
	scanf("%d", &q);
	while (q--) {
		int opt, x;
		scanf("%d%d", &opt, &x);
		if (opt == 1) update(1, 1, n, dfn[x], dfn[x] + siz[x] - 1, 1);
		else if (opt == 2) {
			while (top[x] != 1) {
				update(1, 1, n, dfn[top[x]], dfn[x], 0);
				x = f[top[x]];
			}
			update(1, 1, n, 1, dfn[x], 0);
		}
		else printf("%d\n", query(1, 1, n, dfn[x]));
	}
	return 0;
}
```
{% endfolding %}

<!-- endtab -->

<!-- tab [NOI2015] 软件包管理器 -->

[Portal](https://www.luogu.com.cn/problem/P2146).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;
typedef long long i64;

int n, q, num = 0;
int f[100005], dep[100005], siz[100005];
int dfn[100005], son[100005], top[100005];
vector <int> G[100005];

int T[400005], tag[400005];
inline void pushdown(int o, int l, int r) {
	if (tag[o] == -1) return;
	int mid = l + r >> 1;
	T[o << 1] = (mid - l + 1) * tag[o], T[o << 1 | 1] = (r - mid) * tag[o];
	tag[o << 1] = tag[o], tag[o << 1 | 1] = tag[o];
	tag[o] = -1;
}
void update(int o, int l, int r, int x, int y, int k) {
	if (x <= l && r <= y) return T[o] = k * (r - l + 1), tag[o] = k, void();
	int mid = l + r >> 1; pushdown(o, l, r);
	if (x <= mid) update(o << 1, l, mid, x, y, k);
	if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
	T[o] = T[o << 1] + T[o << 1 | 1];
}
int query(int o, int l, int r, int x, int y) {
	if (x <= l && r <= y) return T[o];
	int mid = l + r >> 1, res = 0; pushdown(o, l, r);
	if (x <= mid) res += query(o << 1, l, mid, x, y);
	if (mid < y) res += query(o << 1 | 1, mid + 1, r, x, y);
	return res;
}

void dfs1(int x, int fa) {
	siz[x] = 1; f[x] = fa;
	if (fa == -1) dep[x] = 1;
	else dep[x] = dep[fa] + 1;
	int max_part = -1;
	for (int y : G[x]) {
		dfs1(y, x);
		siz[x] += siz[y];
		if (siz[y] > max_part) {
			max_part = siz[y];
			son[x] = y;
		}
	}
}

void dfs2(int x, int topf) {
	dfn[x] = ++num; top[x] = topf;
	if (son[x] == -1) return;
	dfs2(son[x], topf);
	for (int y : G[x])
		if (y != son[x]) dfs2(y, y);
}

int main(void) {
	memset(son, -1, sizeof(son));
	memset(tag, -1, sizeof(tag));
	scanf("%d", &n);
	for (int i = 1; i < n; ++i) {
		scanf("%d", f + i);
		G[f[i]].push_back(i);
	}
	f[0] = -1;
	dfs1(0, -1);
	dfs2(0, 0);
	scanf("%d", &q);
	char s[15]; int x;
	while (q--) {
		scanf("%s%d", s, &x);
		if (s[0] == 'i') {
			int ans = 0;
			while (x != -1) {
				int before = query(1, 1, n, dfn[top[x]], dfn[x]);
				update(1, 1, n, dfn[top[x]], dfn[x], 1);
				int after = query(1, 1, n, dfn[top[x]], dfn[x]);
				x = f[top[x]];
				ans += after - before;
			}
			printf("%d\n", ans);
		} else {
			int before = query(1, 1, n, dfn[x], dfn[x] + siz[x] - 1);
			update(1, 1, n, dfn[x], dfn[x] + siz[x] - 1, 0);
			int after = query(1, 1, n, dfn[x], dfn[x] + siz[x] - 1);
			printf("%d\n", before - after);
		}
	}
	return 0;
}
```
{% endfolding %}

<!-- endtab -->

{% endtabs %}

#### [SDOI2011] 染色

[Portal](https://www.luogu.com.cn/problem/P2486).

{% noteblock %}
给定一棵 $n$ 个节点的无根树，共有 $m$  个操作，操作分为两种：

1. 将节点 $a$ 到节点 $b$ 的路径上的所有点（包括 $a$ 和 $b$）都染成颜色 $c$。
2. 询问节点 $a$ 到节点 $b$ 的路径上的颜色段数量。

颜色段的定义是极长的连续相同颜色被认为是一段。例如 `112221` 由三段组成：`11`、`222`、`1`。
{% endnoteblock %}

先树剖。链上的颜色段维护很简单，但是到了树上呢？修改是照常的，关键是查询。查询的时候考虑如何合并，以 LCA 作为分界线，分别处理左右的链，然后再合并。处理的时候同常规的树剖，关键要记录是左边的链还是右边的链（我们会 `swap(x, y)`，这样左右链就会变化），可以用一个 `flag` 来完成记录，合并的时候注意合的是什么即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, m, a[100005], num = 0;
int f[100005], dep[100005], siz[100005];
int dfn[100005], top[100005], son[100005];
int idx[100005];
vector <int> G[100005];

struct Node {
    int lc, rc, cnt;
    int tag;
    Node(int lc = 0, int rc = 0, int cnt = 0, int tag = 0) :
        lc(lc), rc(rc), cnt(cnt), tag(tag) {}
    friend Node operator + (const Node &a, const Node &b) {
        return Node(a.lc, b.rc, a.cnt + b.cnt - (a.rc == b.lc));
    }
} T[400005];
void build(int o, int l, int r) {
    if (l == r) return T[o] = Node(a[idx[l]], a[idx[l]], 1), void(); 
    int mid = l + r >> 1;
    build(o << 1, l, mid); build(o << 1 | 1, mid + 1, r);
    T[o] = T[o << 1] + T[o << 1 | 1];
}
inline void pushdown(int o) {
    if (!T[o].tag) return;
    int x = T[o].tag;
    T[o << 1] = T[o << 1 | 1] = Node(x, x, 1, x);
    T[o].tag = 0;
}
void update(int o, int l, int r, int x, int y, int k) {
    if (x <= l && r <= y) return T[o] = Node(k, k, 1, k), void();
    pushdown(o); int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    T[o] = T[o << 1] + T[o << 1 | 1];
}
Node query(int o, int l, int r, int x, int y) {
    if (x <= l && r <= y) return T[o];
    pushdown(o); int mid = l + r >> 1;
    if (y <= mid) return query(o << 1, l, mid, x, y);
    if (mid < x) return query(o << 1 | 1, mid + 1, r, x, y);
    return query(o << 1, l, mid, x, y) + query(o << 1 | 1, mid + 1, r, x, y);
}

void dfs1(int x, int fa) {
    dep[x] = dep[fa] + 1; siz[x] = 1; f[x] = fa;
    int maxx = -1;
    for (auto y : G[x])
        if (y != fa) {
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > maxx)
                maxx = siz[y], son[x] = y;
        }
}

void dfs2(int x, int topf) {
    dfn[x] = ++num; top[x] = topf; idx[dfn[x]] = x;
    if (son[x] == -1) return;
    dfs2(son[x], topf);
    for (auto y : G[x])
        if (y != f[x] && y != son[x]) dfs2(y, y);
}

int main(void) {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    for (int i = 1; i < n; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    memset(son, -1, sizeof(son));
    dfs1(1, 0);
    dfs2(1, 1);
    build(1, 1, n);
    while (m--) {
        char s[5];
        int x, y, k;
        scanf("%s%d%d", s, &x, &y);
        if (s[0] == 'C') {
            scanf("%d", &k);
            while (top[x] != top[y]) {
                if (dep[top[x]] < dep[top[y]]) swap(x, y);
                update(1, 1, n, dfn[top[x]], dfn[x], k);
                x = f[top[x]];
            }
            if (dep[x] < dep[y]) swap(x, y);
            update(1, 1, n, dfn[y], dfn[x], k);
        } else {
            Node ans1, ans2, tmp; bool flag = 0;
            while (top[x] != top[y]) {
                if (dep[top[x]] < dep[top[y]]) swap(x, y), flag = !flag;
                tmp = query(1, 1, n, dfn[top[x]], dfn[x]);
                if (flag) ans2 = Node(tmp.lc, ans2.rc, ans2.cnt + tmp.cnt - (ans2.lc == tmp.rc));
                else ans1 = Node(ans1.lc, tmp.lc, ans1.cnt + tmp.cnt - (ans1.rc == tmp.rc));
                x = f[top[x]];
            }
            if (dep[x] < dep[y]) swap(x, y), flag = !flag;
            tmp = query(1, 1, n, dfn[y], dfn[x]);
            if (flag) ans2 = Node(tmp.lc, ans2.rc, ans2.cnt + tmp.cnt - (ans2.lc == tmp.rc));
                else ans1 = Node(ans1.lc, tmp.lc, ans1.cnt + tmp.cnt - (ans1.rc == tmp.rc));
            printf("%d\n", ans1.cnt + ans2.cnt - (ans1.rc == ans2.lc));
        }
    }
    return 0;
}
```
{% endfolding %}

#### [NOI2021] 轻重边

[Portal](https://www.luogu.com.cn/problem/P7735).

{% noteblock %}
有一棵 $n$ 个结点的树，树上的每一条边可能是轻边或者重边。接下来你需要对树进行 $m$ 次操作，在所有操作开始前，树上所有边都是轻边。操作有以下两种：

1. 给定两个点 $a$ 和 $b$，首先对于 $a$ 到 $b$ 路径上的所有点 $x$（包含 $a$ 和 $b$），你要将与 $x$ 相连的所有边变为轻边。然后再将 $a$ 到 $b$ 路径上包含的所有边变为重边。
2. 给定两个点 $a$ 和 $b$，你需要计算当前 $a$ 到 $b$ 的路径上一共包含多少条重边。
{% endnoteblock %}

肯定是树剖（LCT 也可，但是笔者不会 LCT{% emoji qq 大哭 %}），但是如何简单维护呢？

考虑使用颜色来维护。初始时每一个点的颜色都不同；每一次修改的时候，我们都将 $(a,b)$ 间染上一个**新的**颜色，然后重边的判定法则就变成了：连接的两个端点的颜色相同（想一想看是不是这样）。

那么实现一个可以统计颜色相同的相邻对的线段树即可，与《染色》一题相似。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

int n, m;
vector <int> G[100005];
int dep[100005], siz[100005], f[100005];
int dfn[100005], top[100005], son[100005], num = 0;

void dfs1(int x, int fa) {
    dep[x] = dep[fa] + 1; siz[x] = 1; f[x] = fa;
    int maxx = -1;
    for (auto y : G[x]) 
        if (y != fa) {
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > maxx) {
                maxx = siz[y];
                son[x] = y;
            }
        }
}

void dfs2(int x, int topf) {
    dfn[x] = ++num; top[x] = topf;
    if (son[x] == -1) return;
    dfs2(son[x], topf);
    for (auto y : G[x])
        if (y != f[x] && y != son[x]) dfs2(y, y);
}

// Segment Tree
struct Node {
    int lc, rc, tag;
    int cnt;
    Node(int lc = 0, int rc = 0, int cnt = 0, int tag = 0) :
        lc(lc), rc(rc), tag(tag), cnt(cnt) {}
} T[400005];
inline Node hb(const Node &a, const Node &b) {
    Node c(a.lc, b.rc, a.cnt + b.cnt + (a.rc == b.lc));
    return c;
}
inline void pushdown(int o, int l, int r) {
    if (!T[o].tag) return;
    int mid = l + r >> 1;
    T[o << 1] = Node(T[o].tag, T[o].tag, mid - l, T[o].tag);
    T[o << 1 | 1] = Node(T[o].tag, T[o].tag, r - mid - 1, T[o].tag);
    T[o].tag = 0;   
}
void update(int o, int l, int r, int x, int y, int k) {
    if (x <= l && r <= y) {
        T[o] = Node(k, k, r - l, k);
        return;
    }
    pushdown(o, l, r); int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    T[o] = hb(T[o << 1], T[o << 1 | 1]);
}
Node query(int o, int l, int r, int x, int y) {
    if (x <= l && r <= y) return T[o];
    pushdown(o, l, r); int mid = l + r >> 1;
    if (y <= mid) return query(o << 1, l, mid, x, y);
    if (mid < x) return query(o << 1 | 1, mid + 1, r, x, y);
    return hb(query(o << 1, l, mid, x, y), query(o << 1 | 1, mid + 1, r, x, y));
}
int query(int x, int y) {
    bool flag = 0;
    Node ans1, ans2, tmp;
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]]) swap(x, y), flag = !flag;
        tmp = query(1, 1, n, dfn[top[x]], dfn[x]);
        if (flag) ans2 = Node(tmp.lc, ans2.rc, ans2.cnt + tmp.cnt + (ans2.lc == tmp.rc));
        else ans1 = Node(ans1.lc, tmp.lc, ans1.cnt + tmp.cnt + (ans1.rc == tmp.rc));
        x = f[top[x]];
    }
    if (dep[x] < dep[y]) swap(x, y), flag = !flag;
    tmp = query(1, 1, n, dfn[y], dfn[x]);
    if (flag) ans2 = Node(tmp.lc, ans2.rc, ans2.cnt + tmp.cnt + (ans2.lc == tmp.rc));
    else ans1 = Node(ans1.lc, tmp.lc, ans1.cnt + tmp.cnt + (ans1.rc == tmp.rc));
    return ans1.cnt + ans2.cnt + (ans1.rc == ans2.lc);
}

int main(void) {
    int TT; scanf("%d", &TT);
    while (TT--) {
        scanf("%d%d", &n, &m);
        num = 0;
        memset(son, -1, sizeof(son));
        memset(T, 0, sizeof(T));
        for (int i = 1; i <= n; ++i) G[i].clear();
        for (int i = 1; i < n; ++i) {
            int u, v;
            scanf("%d%d", &u, &v);
            G[u].emplace_back(v);
            G[v].emplace_back(u);
        }
        for (int i = 1; i <= n; ++i) update(1, 1, n, i, i, -i);
        dfs1(1, 0);
        dfs2(1, 1);
        for (int i = 1; i <= m; ++i) {
            int op, x, y;
            scanf("%d%d%d", &op, &x, &y);
            if (op == 1) {
                while (top[x] != top[y]) {
                    if (dep[top[x]] < dep[top[y]]) swap(x, y);
                    update(1, 1, n, dfn[top[x]], dfn[x], i);
                    x = f[top[x]];
                }
                if (dep[x] < dep[y]) swap(x, y);
                update(1, 1, n, dfn[y], dfn[x], i);
            } else printf("%d\n", query(x, y));
        }
    }
    return 0;
}
```
{% endfolding %}

### 其它树上问题

## 小结

树上的问题非常重要，接下来可以阅读：
