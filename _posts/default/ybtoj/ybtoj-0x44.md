---
title: YbtOJ 高效进阶 数据结构 第4章 线段树
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 数据结构
    - 线段树
status: working
abbrlink: f99650a8
date: 2022-08-16 12:16:43
sticky: -100
---

> 写一些线段树来玩一下，其实还挺有意思。

<!--more-->

## A. 求区间和

> 单点修改元素值，求区间连续和。

线段树模板。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, m;
i64 T[400005];

void update(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o] += k;
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    if (x <= mid) update(ls, l, mid, x, k);
    else update(rs, mid + 1, r, x, k);
    T[o] = T[ls] + T[rs];
}

i64 query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    int mid = l + r >> 1;
    i64 res = 0;
    if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--)
    {
        int k, a, b;
        scanf("%d%d%d", &k, &a, &b);
        if (k == 0) update(1, 1, n, a, b);
        else printf("%lld\n", query(1, 1, n, a, b));
    }
    return 0;
}
```

## B. 区间查改

> 区间加区间求和。

线段树模板。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, m, a[1000005];
i64 T[4000005], tag[4000005];

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o] = a[l];
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    T[o] = T[ls] + T[rs];
}

inline void pushdown(int o, int l, int r)
{
    if (!tag[o]) return;
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    tag[ls] += tag[o], tag[rs] += tag[o];
    T[ls] += tag[o] * (mid - l + 1), T[rs] += tag[o] * (r - mid);
    tag[o] = 0;
}

void update(int o, int l, int r, int x, int y, i64 k)
{
    if (x <= l && r <= y)
    {
        tag[o] += k;
        T[o] += (r - l + 1) * k;
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    if (x <= mid) update(ls, l, mid, x, y, k);
    if (mid < y) update(rs, mid + 1, r, x, y, k);
    T[o] = T[ls] + T[rs];
}

i64 query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    pushdown(o, l, r);
    int mid = l + r >> 1;
    i64 res = 0;
    if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    build(1, 1, n);
    while (m--)
    {
        int k, a, b, x;
        scanf("%d%d%d", &k, &a, &b);
        if (k == 1)
        {
            scanf("%d", &x);
            update(1, 1, n, a, b, x);
        }
        else printf("%lld\n", query(1, 1, n, a, b));
    }
    return 0;
}
```

## C. 公园遛狗

[Portal](https://www.luogu.com.cn/problem/P4513).

经典问题。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, m;
int a[500005];

struct Node
{
    int sum, lmax, rmax, dat;
    Node (int sum = 0, int lmax = 0, int rmax = 0, int dat = 0) :
        sum(sum), lmax(lmax), rmax(rmax), dat(dat) {}
}T[2000005];

inline void maintain(int o)
{
    int ls = o << 1, rs = o << 1 | 1;
    T[o].sum = T[ls].sum + T[rs].sum;
    T[o].lmax = max(T[ls].lmax, T[ls].sum + T[rs].lmax);
    T[o].rmax = max(T[rs].rmax, T[rs].sum + T[ls].rmax);
    T[o].dat = max({T[ls].dat, T[rs].dat, T[ls].rmax + T[rs].lmax});
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o].sum = T[o].lmax = T[o].rmax = T[o].dat = a[l];
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    maintain(o);
}

void update(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o].sum = T[o].lmax = T[o].rmax = T[o].dat = k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, k);
    else update(o << 1 | 1, mid + 1, r, x, k);
    maintain(o);
}

Node query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    if (qr <= mid) return query(ls, l, mid, ql, qr);
    if (ql > mid) return query(rs, mid + 1, r, ql, qr);
    Node x = query(ls, l, mid, ql, qr), y = query(rs, mid + 1, r, ql, qr), res;
    res.sum = x.sum + y.sum;
    res.lmax = max(x.lmax, x.sum + y.lmax);
    res.rmax = max(y.rmax, y.sum + x.rmax);
    res.dat = max({x.dat, y.dat, x.rmax + y.lmax});
    return res;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    build(1, 1, n);
    while (m--)
    {
        int k, x, y;
        scanf("%d%d%d", &k, &x, &y);
        if (k == 1) 
        {
            if (x > y) swap(x, y);
            printf("%d\n", query(1, 1, n, x, y).dat);
        }
        else update(1, 1, n, x, y);
    }
    return 0;
}
```

## D. 维护序列

[Portal](https://www.luogu.com.cn/problem/P2023).

板子题。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, m, P;
int a[100005];
i64 add[400005], mul[400005], T[400005];

void build(int o, int l, int r)
{
    mul[o] = 1;
    if (l == r)
    {
        T[o] = a[l] % P;
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    T[o] = (T[ls] + T[rs]) % P;
}

inline void pushdown(int o, int l, int r)
{
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    T[ls] = (T[ls] * mul[o] + (mid - l + 1) * add[o]) % P;
    T[rs] = (T[rs] * mul[o] + (r - mid) * add[o]) % P;
    mul[ls] = mul[ls] * mul[o] % P;
    mul[rs] = mul[rs] * mul[o] % P;
    add[ls] = (add[ls] * mul[o] + add[o]) % P;
    add[rs] = (add[rs] * mul[o] + add[o]) % P;
    mul[o] = 1, add[o] = 0;
}

i64 query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o] % P;
    pushdown(o, l, r);
    int mid = l + r >> 1;
    i64 res = 0;
    if (ql <= mid) res = (res + query(o << 1, l, mid, ql, qr)) % P;
    if (mid < qr) res = (res + query(o << 1 | 1, mid + 1, r, ql, qr)) % P;
    return res;
}

void updateadd(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        add[o] = (add[o] + k) % P;
        T[o] = (T[o] + (i64)k * (r - l + 1)) % P;
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    if (x <= mid) updateadd(ls, l, mid, x, y, k);
    if (mid < y) updateadd(rs, mid + 1, r, x, y, k);
    T[o] = (T[ls] + T[rs]) % P;
}

void updatemul(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        add[o] = add[o] * k % P;
        mul[o] = mul[o] * k % P;
        T[o] = T[o] * k % P;
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    if (x <= mid) updatemul(ls, l, mid, x, y, k);
    if (mid < y) updatemul(rs, mid + 1, r, x, y, k);
    T[o] = (T[ls] + T[rs]) % P;
}

int main(void)
{
    scanf("%d%d", &n, &P);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    build(1, 1, n);
    scanf("%d", &m);
    while (m--)
    {
        int op, x, y, k;
        scanf("%d%d%d", &op, &x, &y);
        if (op == 3) printf("%lld\n", query(1, 1, n, x, y));
        else
        {
            scanf("%d", &k);
            if (op == 1) updatemul(1, 1, n, x, y, k);
            else updateadd(1, 1, n, x, y, k);
        }
    }
    return 0;
}
```

## E. 字符串排序

## F. 取模问题

[Portal](https://codeforces.com/problemset/problem/438/D).

原题。

```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int n, m;
int a[100005];
i64 T[400005], maxx[400005];

inline void maintain(int o)
{
    T[o] = T[o << 1] + T[o << 1 | 1];
    maxx[o] = max(maxx[o << 1], maxx[o << 1 | 1]);
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o] = maxx[o] = a[l];
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o);
}

i64 query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    i64 res = 0;
    int mid = l + r >> 1;
    if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

void update(int o, int l, int r, int x, int y, int k)
{
    if (l == r)
    {
        T[o] %= k, maxx[o] %= k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid && maxx[o << 1] >= k) update(o << 1, l, mid, x, y, k);
    if (mid < y && maxx[o << 1 | 1] >= k) update(o << 1 | 1, mid + 1, r, x, y, k);
    maintain(o);
}

void modify(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o] = k, maxx[o] = k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) modify(o << 1, l, mid, x, k);
    else modify(o << 1 | 1, mid + 1, r, x, k);
    maintain(o);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) 
        scanf("%d", a + i);
    build(1, 1, n);
    while (m--)
    {
        int op, x, y, k;
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1) printf("%lld\n", query(1, 1, n, x, y));
        else if (op == 2)
        {
            scanf("%d", &k);
            update(1, 1, n, x, y, k);
        }
        else modify(1, 1, n, x, y);
    }
    return 0;
}
```

## G. 魔法传输

![](https://pic.imgdb.cn/item/62fd09c316f2c2beb1d6c1eb.jpg)

无聊的数列的弱化版。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
int a[100005];
int tagk[400005], tagd[400005];

inline void pushdown(int o)
{
    tagk[o << 1] += tagk[o], tagk[o << 1 | 1] += tagk[o];
    tagd[o << 1] += tagd[o], tagd[o << 1 | 1] += tagd[o];
    tagk[o] = tagd[o] = 0;
}

void update(int o, int l, int r, int x, int y, int k, int d)
{
    if (x <= l && r <= y)
    {
        tagk[o] += k;
        tagd[o] += d;
        return;
    }
    pushdown(o);
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k, d);
    if (mid + 1 <= y) update(o << 1 | 1, mid + 1, r, x, y, k, d);
}

int query(int o, int l, int r, int p)
{
    if (l == r) return tagk[o] + l * tagd[o];
    int mid = l + r >> 1;
    pushdown(o);
    if (p <= mid) return query(o << 1, l, mid, p);
    return query(o << 1 | 1, mid + 1, r, p);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--)
    {
        char c;
        int x, y;
        scanf("%s %d", &c, &x);
        if (c == 'C')
        {
            scanf("%d", &y);
            update(1, 1, n, x, y, 1 - x, 1);
        }
        else printf("%d\n", query(1, 1, n, x));
    }
    return 0;
}
```

## 小结

只要是数据结构，我就不会：

- sto lxl 毒瘤数据结构；
- sto 同市还比我小的 DS 大佬 [@Cat_Shao](https://www.luogu.com.cn/user/234011)。