---
title: 可持久化数据结构
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 学习笔记
    - 数据结构
tag:
    - 可持久化
    - 笔记
group: notes
hide: true
status: working
abbrlink: 32a993e5
date: 2022-08-21 14:37:12
headimg: https://s1.ax1x.com/2022/08/08/vMLIRf.jpg
---

> 可持久化数据结构可以保留每一个历史版本。本文将介绍几种简单的可持久化数据结构以及它们的应用。

<!--more-->

## 概念

所有版本都可以访问，但是只有最新版本可以修改，称之为部分可持久化。
所有版本都既可以访问又可以修改，称之为完全可持久化。
若支持将两个历史版本合并，则又称为汇合可持久化（Confluently Persistent）。

无论如何，基本思想是修改时要保留原来的内容。

## 可持久化线段树

毫无疑问，这是最重要的可持久化数据结构，因为它可以用来实现可持久化数组，进而实现更多内容。

### 概述

当我们进行修改的时候，最暴力的想法就是：我再建一棵线段树不就得了嘛！

但是你觉得可能吗{% emoji sticker 滑稽 %}？空间炸裂了……

对于点修改来说，一次修改至多会影响 $O(\log n)$ 个节点，所以我们只需要复制这 $O(\log n)$ 个节点即可。但是复制？这样的话就需要记录左右儿子的编号，实现的时候需要采用动态开点，这样可以将新建的节点的儿子编号拉到原线段树上。

```cpp
struct Node
{
    int lc, rc; // 左右节点编号
    int dat; // 当前维护的值
}T[SIZE * 2]; // 终于只需要二倍空间啦！
int root[MAXQ], tot; // 每次修改的根节点编号，节点个数

int build(int l, int r)
{
    int o = ++tot;
    if (l == r)
    {
        T[o].dat = a[l];
        return o;
    }
    int mid = l + r >> 1;
    T[o].lc = build(l, mid);
    T[o].rc = build(mid + 1, r);
    T[o].dat = T[T[o].lc].dat + T[T[o].rc].dat;
    return o;
}

int main(void)
{
    root[0] = build(1, n); // 建立初始的可持久化线段树
}
```

### 点修改

在修改的时候，变化的内容的节点编号要新建，而没有变化的直接复制原来的版本信息即可。

{% image https://pic1.imgdb.cn/item/6339022816f2c2beb1a0594f.jpg::alt=修改了位置 1 的值，只有节点 1248 的值会发生变化，需要新建 %}

```cpp
int main(void)
{
    root[i] = update(root[i - 1], 1, n, x, k); // 在第 i - 1 个版本的基础上创建第 i 个版本
}

int update(int pre, int l, int r, int x, int k) // 在 pre 号节点的基础上，x 加上 k
{
    int o = ++tot; // 先新建这个节点
    T[o] = T[pre]; // 先把原来的东西复制过来
    if (l == r)
    {
        T[o].dat += k; // 叶子节点直接修改
        return o; // 返回当前的节点
    }
    int mid = l + r >> 1;
    if (x <= mid) T[o].lc = update(T[pre].lc, l, mid, x, k); // 修改的内容在左子区间，将左儿子的编号修改为新建节点的编号
    else T[o].rc = update(T[pre].rc, mid + 1, r, x, k); // 修改的内容在右子区间，修改右儿子的编号
    T[o].dat = T[T[o].lc].dat + T[T[o].rc].dat; // 维护当前节点的值
    return o; // 返回当前节点
}
```

### 可持久化数组

[模板](https://www.luogu.com.cn/problem/P3919)。

可持久化线段树可以用来实现可持久化数组，而且非常简单，因为只需要做到单点修改和单点查询{% emoji 滑稽 %}。

所以 `maintain` 之类的操作根本不需要，查询的时候几乎跟普通线段树一模一样。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

struct Node
{
    int lc, rc;
    int dat;
}T[44000005];
int tot, root[1000005];

int n, m;
int a[1000005];

int build(int l, int r)
{
    int o = ++tot;
    if (l == r)
    {
        T[o].dat = a[l];
        return o;
    }
    int mid = l + r >> 1;
    T[o].lc = build(l, mid);
    T[o].rc = build(mid + 1, r);
    return o;
}

int update(int pre, int l, int r, int x, int k)
{
    int o = ++tot;
    T[o] = T[pre];
    if (l == r)
    {
        T[o].dat = k;
        return o;
    }
    int mid = l + r >> 1;
    if (x <= mid) T[o].lc = update(T[pre].lc, l, mid, x, k);
    else T[o].rc = update(T[pre].rc, mid + 1, r, x, k);
    return o;
}

int query(int o, int l, int r, int x)
{
    if (l == r) return T[o].dat;
    int mid = l + r >> 1;
    if (x <= mid) return query(T[o].lc, l, mid, x);
    return query(T[o].rc, mid + 1, r, x);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    root[0] = build(1, n);
    for (int i = 1; i <= m; ++i)
    {
        int pre, op, x, k;
        scanf("%d%d%d", &pre, &op, &x);
        if (op == 1)
        {
            scanf("%d", &k);
            root[i] = update(root[pre], 1, n, x, k);
        }
        else
        {
            root[i] = root[pre];
            printf("%d\n", query(root[i], 1, n, x));
        }
    }
    return 0;
}
```

### 主席树

也就是可持久化权值线段树。

[模板](https://www.luogu.com.cn/problem/P3834)。

我们可以依次读入这些数后离散化，然后扫描每个数，建立主席树，第 $k$ 个版本维护 $[1,k]$ 各个数值。由于权值树是可以直接相加或相减的，所以整个过程类似于差分。

下面说的具体一点。在离散化后，假设有 $m$ 个数，那么值域就变为了 $[1,m]$。$dat$ 代表所对应的值域 $[L,R]$ 插入了多少个数。插入一个数后，就要在它离散化后的位置上加上 $1$。查询的时候，有 $T[r_i].dat - T[l_i-1].dat$ 的值就是 $[l_i,r_i]$ 中有多少个数落在值域 $[L,R]$ 上。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
const int MAXN = 200005;

struct Node
{
    int lc, rc;
    int dat;
}T[MAXN * 20];
int tot, root[200005];

int n, Q, m;
int a[200005];
int tmp[200005];

void init(void)
{
    for (int i = 1; i <= n; ++i)
        tmp[i] = a[i];
    sort(tmp + 1, tmp + n + 1);
    m = unique(tmp + 1, tmp + n + 1) - (tmp + 1);
    for (int i = 1; i <= n; ++i)
        a[i] = lower_bound(tmp + 1, tmp + m + 1, a[i]) - tmp;
}

int build(int l, int r) // 一开始只是新建节点，但是没有数值
{
    int o = ++tot;
    if (l == r) return o;
    int mid = l + r >> 1;
    T[o].lc = build(l, mid);
    T[o].rc = build(mid + 1, r);
    return o;
}

int update(int pre, int l, int r, int x, int k)
{
    int o = ++tot;
    T[o] = T[pre];
    if (l == r)
    {
        T[o].dat += k;
        return o;
    }
    int mid = l + r >> 1;
    if (x <= mid) T[o].lc = update(T[pre].lc, l, mid, x, k);
    else T[o].rc = update(T[pre].rc, mid + 1, r, x, k);
    T[o].dat = T[T[o].lc].dat + T[T[o].rc].dat;
    return o;
}

int query(int p, int q, int l, int r, int k) // 查询的时候同步进行查询
{
    if (l == r) return l; // 值域只有一个，直接返回
    int mid = l + r >> 1, res = T[T[q].lc].dat - T[T[p].lc].dat; // 落在 [L, mid] 的值域的数的个数
    if (k <= res) return query(T[p].lc, T[q].lc, l, mid, k); // 要查的比 res 小，在左子节点查
    return query(T[p].rc, T[q].rc, mid + 1, r, k - res); // 比 res 大，减去 res 在右子节点查
}

int main(void)
{
    scanf("%d%d", &n, &Q);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i);
    init();
    root[0] = build(1, m);
    for (int i = 1; i <= n; ++i)
        root[i] = update(root[i - 1], 1, m, a[i], 1);
    while (Q--)
    {
        int l, r, k;
        scanf("%d%d%d", &l, &r, &k);
        printf("%d\n", tmp[query(root[l - 1], root[r], 1, m, k)]);
    }
    return 0;
}
```

至于空间到底开多大，要看修改次数，然后按照空间复杂度计算即可，如果内存限制允许可以稍微开大一点。本题中 $\lceil\log_2 2\times 10^5\rceil=18$，再加上一个初始版本的两倍空间，开到 $20$ 倍空间就足够了。

### 标记永久化

### 可持久化并查集

## Problemset

高级数据结构的显著特点是：基本原理不难理解，但是灵活应用较难。这里将通过大量题目来展示可持久化数据结构的用法。

### 简单问题

这里的问题都比较的板，可以练习一下。

#### [POI2014] KUR-Couriers

[Portal](https://www.luogu.com.cn/problem/P3567).

建立主席树，查询的时候如果在 $[l,mid]$ 的值域的数的个数大于要求的数量，那么就到左子树查询，右子树同理，否则就是无解。因为右子树的值域区间大小只能比左子树的值域区间大小小或者相等，所以当落在左子树值域的数的个数满足限制条件时，右子树就不可能满足了，就不用查询了。

**注意**！由于这题的最优做法并不是主席树，所以主席树会被卡空间。初始版本是不能被建立的。要使用如代码中所示的方法。

{% folding cyan::查看代码 %}
```cpp
#include <cstdio>
const int MAXN = 500000;

int read(void)
{
    int x = 0, c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar();
    return x;
}

struct Node
{
    int lc, rc, dat;
}T[21 * MAXN + 480000];
int tot = 0, root[MAXN + 1];

int build(int l, int r)
{
    int o = tot++;
    if (l == r) return o;
    int mid = l + r >> 1;
    T[o].lc = build(l, mid);
    T[o].rc = build(mid + 1, r);
    return o;
}

void update(int &now, int l, int r, int x) // 注意 now 是引用，会被更新
{
    T[++tot] = T[now]; now = tot;
    T[tot].dat++;
    if (l == r) return;
    int mid = l + r >> 1;
    if (x <= mid) update(T[now].lc, l, mid, x);
    else update(T[now].rc, mid + 1, r, x);
}

int k;
int query(int p, int q, int l, int r)
{
    if (l == r) return l;
    int mid = l + r >> 1;
    if (2 * (T[T[q].lc].dat - T[T[p].lc].dat) > k) return query(T[p].lc, T[q].lc, l, mid);
    if (2 * (T[T[q].rc].dat - T[T[p].rc].dat) > k) return query(T[p].rc, T[q].rc, mid + 1, r);
    return 0;
}

int main(void)
{
    int n, m, l, r, x;
    n = read(), m = read();
    for (int i = 1; i <= n; ++i)
    {
        root[i] = root[i - 1];
        update(root[i], 1, n, read()); // 直接修改
    }
    while (m--)
    {
        l = read(), r = read();
        k = r - l + 1;
        printf("%d\n", query(root[l - 1], root[r], 1, n));
    }
    return 0;
}
```
{% endfolding %}

### 较复杂问题