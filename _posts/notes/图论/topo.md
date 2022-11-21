---
title: 省选初级图论
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 学习笔记
    - 图论
tag:
    - 图论
headimg: https://s1.ax1x.com/2022/08/04/vm9ef0.jpg
group: notes
hide: true
status: working
abbrlink: fb40efc5
date: 2022-12-04 14:09:14
---

> 高级的图论知识会包含更多的内容。我们将二分图与网络流排除，这篇文章的内容会覆盖绝大多数常用的图论知识点与技巧。

<!--more-->

## 拆边拆点与点边转化

有些时候的问题比较复杂，我们可以选择将边或者是点进行拆分，来更加方便的处理。比如分层图最短路中，我们使用的就是"点拆点"，把一个点拆成多个点进行维护。当然，这也意味着分层图中的"特殊道路"不会很多，否则点会被拆成很多，复杂度无法承受。

有的时候点或者边不方便做，两者甚至可以相互转化。比如在处理树上边权时，我们就采取了把边权下放到点权的方式。

这种思想在网络流中还有更多应用，所以在这里只简单介绍几个例子。

### 点拆点 | [SCOI 2009] 迷路

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/P4159).

一个有向图有 $n$ 个节点，从节点 $1$ 出发，必须恰好在 $t$ 时刻到达节点 $n$。求共有多少种不同的路径吗？答案对 $2009$ 取模。注意：不能在某个节点逗留，且通过某有向边的时间严格为给定的时间。保证 $2 \leq n \leq 10$，$1 \leq t \leq 10^9$。
{% endnoteblock %}

我们先考虑边权只有 01 怎么做。

设 $f(t,i,j)$ 表示 $i$ 到 $j$ 的长度为 $t$ 的路径数，显然 $t=1$ 时这个就是初始矩阵，转移也就是 $f(t,i,j)=\sum_{k=1}^{n}f(t-1,i,k)+f(1,k,j)$。当然这个转移也可以扩展到任意情况，不过与我们接下来讨论的内容没有关系。

这种时候可以发现 $f(t)=f(t-1)\times f(1)$，那么可以使用矩阵快速幂计算。然而当不全是 01 的时候无法使用矩阵快速幂！怎么办？发现 $w<10$，我们可以将一个点拆成九个点，像这样：

![](https://pic.imgdb.cn/item/63772d7a16f2c2beb180746a.png)

如果我们要连一个点到这个被拆好的点，那么根据距离连接即可。比如有一个点与它距离为 $3$，那么就需要连接到 $2$ 号点。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;
typedef long long i64;
const int MOD = 2009;

int m;

struct Matrix {
    int a[100][100];
    Matrix() { memset(a, 0, sizeof(a)); }
    Matrix operator * (const Matrix &a) const {
        Matrix res;
        for (int i = 1; i <= m; ++i)
            for (int k = 1; k <= m; ++k) {
                i64 r = this->a[i][k];
                for (int j = 1; j <= m; ++j)
                    res.a[i][j] = (res.a[i][j] + r * a.a[k][j]) % MOD;
            }
        return res;
    }
};

Matrix poww(Matrix a, int b) {
    Matrix res; for (int i = 1; i <= m; ++i) res.a[i][i] = 1;
    while (b) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

int n, t;

int main(void)
{
    scanf("%d%d", &n, &t);
    m = 9 * n;
    Matrix f;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= 8; ++j)
            f.a[i + j * n][i + (j - 1) * n] = 1;
        for (int j = 1; j <= n; ++j) {
            int x;
            scanf("%1d", &x);
            if (x) f.a[i][j + (x - 1) * n] = 1;
        }
    }
    f = poww(f, t);
    printf("%d\n", f.a[1][n]);
    return 0;
}
```
{% endfolding %}

### 边转点 | [SDOI2009] HH去散步

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/P2151).

给定一张不带权的无向图，不会立刻沿着刚刚走来的路走回，问有多少种从 $A$ 走到 $B$，距离为 $T$ 的走法？
{% endnoteblock %}

感觉根上一题很相似，但是要求不能来回走！怎么办？考虑将一条无向边转换成两个点，然后这两个点之间不连边，剩下的能到达的边与这些点连边。

初始时连一条 $0\rightarrow a$ 的边，建立初始矩阵的时候枚举两条边 $i,j$，如果满足 $v[i]=u[j]$，而且不是反向边，那么 $i\rightarrow j$ 就可以用 $1$ 的时间去到达。答案也是枚举所有边，第一条边 $0\rightarrow a$ 到当前的边 $k\rightarrow b$ 的答案的和。

{% folding cyan::查看代码 %}
```cpp
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
    Matrix operator * (const Matrix &b) const {
        Ma初始
        for (int i = 0; i <= N; ++i)
            for (int k = 0; k <= N; ++k) {
                int r = a[i][k];
                for (int j = 0; j <= N; ++j)    
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
int u[125], v[125], cnt = 0;

int main(void)
{
    scanf("%d%d%d%d%d", &n, &m, &t, &a, &b); ++a; ++b;
    u[++cnt] = 0; v[cnt] = a;
    while (m--) {
        int x, y; scanf("%d%d", &x, &y); ++x; ++y;
        u[++cnt] = x; v[cnt] = y;
        u[++cnt] = y; v[cnt] = x;
    }
    Matrix f;
    N = cnt;
    for (int i = 1; i <= cnt; ++i)
        for (int j = 1; j <= cnt; ++j)
            if (i != j && i != (j ^ 1) && v[i] == u[j])
                f.a[i][j] = 1;
    f = poww(f, t);
    int ans = 0;
    for (int i = 1; i <= cnt; ++i)
        if (v[i] == b) ans = (ans + f.a[1][i]) % MOD;
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}