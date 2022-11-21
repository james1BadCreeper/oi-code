---
title: YbtOJ 高效进阶 数据结构 第2章 树状数组
cover: false
plugins:
  - katex
abbrlink: 87ad68d7
date: 2022-08-19 15:44:59
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 树状数组
status: working
sticky: -100
---

> 水题使我快乐。

<!--more-->

## A. 单点修改区间查询

板子。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#define lowbit(x) (x & (-x))
#define i64 long long

using namespace std;

int  n, q;
i64 C[1000005];

void add(int x, int k)
{
    while (x <= n)
    {
        C[x] += k;
        x += lowbit(x);
    }
}

i64 sum(int x)
{
    i64 res = 0;
    while (x)
    {
        res += C[x];
        x -= lowbit(x);
    }
    return res;
}

int main(void)
{
    scanf("%d%d", &n, &q);
    for (int i = 1, x; i <= n; ++i)
        scanf("%d", &x), add(i, x);
    while (q--)
    {
        int op, x, y;
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1) add(x, y);
        else printf("%lld\n", sum(y) - sum(x - 1));
    }
    return 0;
}
```
{% endfolding %}

## B. 逆序对

板子。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

#define lowbit(x) (x & (-(x)))

using namespace std;
typedef long long i64;

inline int read(void)
{
	int x = 0, c = getchar_unlocked(), f = 1;
	while (!isdigit(c)) {if (c == '-') f = -1; c = getchar_unlocked();}
	while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar_unlocked();
	return x * f;
}

int n, m, a[500005], b[500005];
int C[500005];

void update(int x, int k)
{
	while (x <= n)
	{
		C[x] += k;
		x += lowbit(x);
	}
}

int query(int x)
{
	i64 res = 0;
	while (x)
	{
		res += C[x];
		x -= lowbit(x);
	}
	return res;
}

int main(void)
{
	n = read();
	for (int i = 1; i <= n; ++i)
		b[i] = a[i] = read();
	sort(b + 1, b + n + 1);
	m = unique(b + 1, b + n + 1) - (b + 1);
	for (int i = 1; i <= n; ++i)
		a[i] = lower_bound(b + 1, b + m + 1, a[i]) - b;
	i64 ans = 0;
	for (int i = n; i >= 1; --i)
	{
		ans += query(a[i] - 1);
		update(a[i], 1);
	}
	printf("%lld\n", ans);
	return 0;
}
```
{% endfolding %}

## D. 区间修改区间查询

板子。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long
#define lowbit(x) (x & (-(x)))

using namespace std;

int n, m;
i64 C[1000005], D[1000005];

inline void add(i64 *a, int x, i64 key)
{
    while (x <= n)
    {
        a[x] += key;
        x += lowbit(x);
    }
}

inline i64 sum(i64 *a, int r)
{
    i64 res = 0;
    while (r)
    {
        res += a[r];
        r -= lowbit(r);
    }
    return res;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    int a, b = 0;
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &a);
        b = a - b;
        add(C, i, b);
        add(D, i, 1ll * (i - 1) * b);
        b = a;
    }
    while (m--)
    {
        int op, l, r, x;
        scanf("%d", &op);
        if (op == 1)
        {
            scanf("%d%d%d", &l, &r, &x);
            add(C, l, x);
            add(C, r + 1, -x);
            add(D, l, 1ll * (l - 1) * x);
            add(D, r + 1, 1ll * (-x) * r);
        }
        else
        {
            scanf("%d%d", &l, &r);
            printf("%lld\n", (r * sum(C, r) - (l - 1) * sum(C, l - 1)) - (sum(D, r) - sum(D, l - 1)));
        }
    }
    return 0;
}
```
{% endfolding %}

## 小结

呜呜呜……