---
title: 简单数据结构
cover: false
plugins:
    - katex
abbrlink: 9ab0fd1d
date: 2022-08-25 09:58:35
categories:
    - 算法竞赛
    - 学习笔记
    - 数据结构
tag:
    - ST 表
    - 树状数组
    - 笔记
headimg:
group: notes
hide: ture
status: working
---

> 包括并查集、ST 表、树状数组和二叉堆，这些都是 NOIP 范围以内的简单常用数据结构。

<!--more-->

## ST 表

在区间最值问题（RMQ）中，Sparse Table 表，又称稀疏表，也就是 ST 表，可以高效的解决 RMQ 问题。ST 表基于倍增的原理，设 $f(i,j)$ 代表区间 $[i,i+2^j - 1]$ 的最大值，也就是从 $i$ 开始 $2^j$ 长度的序列的最大值。

我们按照序列的长度进行递推，可以有两个小区间的结果合并，也就是 $f(i,j)=\max\{f(i,j-1),f(i+2^{j-1},j-1)\}$。

查询的时候，答案就是 $\max\{f(l,k),f(r-2^k+1,k)\}$，其中 $k$ 代表 $\log_2{r-l+1}$，即使区间有重合也没有关系。

[模板](https://www.luogu.com.cn/problem/P3865)。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
int a[100005], lg[100005];
int f[100005][18];

void ST_prework(void)
{
    lg[0] = 0, lg[1] = 0;
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 1; i <= n; ++i) f[i][0] = a[i];
    for (int j = 1; (1 << j) <= n; ++j)
        for (int i = 1; i <= n - (1 << j) + 1; ++i)
            f[i][j] = max(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
}

int query(int l, int r)
{
    int k = lg[r - l + 1];
    return max(f[l][k], f[r - (1 << k) + 1][k]);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    ST_prework();
    while (m--)
    {
        int l, r;
        scanf("%d%d", &l, &r);
        printf("%d\n", query(l, r));
    }
    return 0;
}
```

## 树状数组

树状数组可以高效地维护前缀和。利用 `lowbit` 运算可以很方便地实现：

```cpp
#define lowbit(x) (x & (-(x)))

i64 C[1000005];
void update(int x, int k)
{
	while (x <= n)
	{
		C[x] += k;
		x += lowbit(x);
	}
}

i64 query(int x)
{
	i64 res = 0;
	while (x)
	{
		res += C[x];
		x -= lowbit(x);
	}
	return res;
}
```

### 区间修改，单点查询

### 区间修改，区间查询

### 权值树状数组

构建原序列的权值数列，然后利用树状数组统计。下面的代码可以快速解决逆序对问题：

{% folding cyan::查看代码 %}
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

## Problemset

有一些非常经典的序列问题，它们都比较简单。

### ST 表

ST 表可以维护一些静态信息，前提是信息可以被简单的合并。

#### [JSOI2008] 最大数

[Portal](https://www.luogu.com.cn/problem/P1198).

看起来这种需要动态维护的东西只能用线段树计算，但是改变的只有最后一个数，我们只需要处理这最后一个数的 ST 表即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int M, D, n;
int a[200005], lg[200005];
int f[200005][20];
int x, last = 0; char c;

void update(void)
{
    f[n][0] = a[n];
    for (int j = 1; (1 << j) <= n; ++j)
        f[n - (1 << j) + 1][j] = max(f[n - (1 << j) + 1][j - 1], f[n - (1 << (j - 1)) + 1][j - 1]);
}

int query(int l, int r)
{
    int k = lg[r - l + 1];
    return max(f[l][k], f[r - (1 << k) + 1][k]);
}

int main(void)
{
    memset(f, 0xbf, sizeof(f));
    cin >> M >> D;
    for (int i = 2; i <= 200002; ++i) lg[i] = lg[i >> 1] + 1;
    while (M--)
    {
        cin >> c >> x;
        if (c == 'A')
        {
            a[++n] = (x + last) % D;
            update();
        }
        else cout << (last = query(n - x + 1, n)) << '\n';
    }
    return 0;
}
```
{% endfolding %}

#### [SCOI2007] 降雨量

[Portal](https://www.luogu.com.cn/problem/P2471).

对于每个询问进行分类讨论，然后使用 ST 表加速查询。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <unordered_map>

using namespace std;

int n, m;
int lg[50005];
int y[50005], r[50005];
int f[50005][17];
unordered_map <int, int> pockets;

void ST_prework(void)
{
    lg[0] = 0, lg[1] = 0;
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
    for (int j = 1; (1 << j) <= n; ++j)
        for (int i = 1; i <= n - (1 << j) + 1; ++i)
            f[i][j] = max(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
}

int query(int l, int r)
{
    if (l > r) return -2e9; // 可能是空区间
    int k = lg[r - l + 1];
    return max(f[l][k], f[r - (1 << k) + 1][k]);
}

int calc(int x) // 最后一个年份小于等于 x 的下标
{
    int L = 0, R = n + 1;
    while (L + 1 != R)
    {
        int mid = L + R >> 1;
        if (y[mid] <= x) L = mid;
        else R = mid;
    }
    return L;
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d%d", y + i, r + i);
        f[i][0] = r[i];
        pockets[y[i]] = i;
    }
    ST_prework();
    scanf("%d", &m);
    while (m--)
    {
        int L, R;
        scanf("%d%d", &L, &R); // 自 l 以来 r 是最多的，r 不超过 l，在 [l + 1, r - 1] 之间的年份的降雨量都小于 r, l 年
        if (pockets.find(L) == pockets.end() && pockets.find(R) == pockets.end()) puts("maybe");
        else if (pockets.find(L) == pockets.end()) // L 是不存在的，但是 R 存在
        {
            if (query(calc(L) + 1, pockets[R] - 1) >= r[pockets[R]]) puts("false"); // 大于等于第 R 年的降雨量
            else puts("maybe");
        }
        else if (pockets.find(R) == pockets.end()) // R 是不存在的，但是 L 存在
        {
            if (query(pockets[L] + 1, calc(R)) >= r[pockets[L]]) puts("false"); // 大于等于第 L 年的降雨量
            else puts("maybe");
        }
        else // 都存在
        {
            if (r[pockets[R]] > r[pockets[L]]) puts("false"); // r 超过了 l
            else if (query(pockets[L] + 1, pockets[R] - 1) >= r[pockets[R]]) puts("false"); // 有不小于第 r 年的
            else if (R - L == pockets[R] - pockets[L]) puts("true"); // 这之间的这些年份都给数据了
            else puts("maybe");
        }
    }
    return 0;
}
```
{% endfolding %}

### 树状数组

树状数组简单好写，可以解决不少问题，相比于线段树等更快更好写。

#### [JLOI2011] 不等式组

[Portal](https://www.luogu.com.cn/problem/P5482).

使用权值树状数组维护即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>
#define lowbit(x) ((x) & (-(x)))

using namespace std;
const int EX = 1000005, EN = 2000020, MAXK = 1000000;

int n, cnt = 0, tot = 0;
int type[100005], k[100005];
bool del[100005];

struct Fenwick_Tree
{
    int C[2000050];

    void update(int x, int k)
    {
        x += EX;
        while (x <= EN)
        {
            C[x] += k;
            x += lowbit(x);
        }
    }

    int query(int x)
    {
        x += EX;
        int res = 0;
        while (x)
        {
            res += C[x];
            x -= lowbit(x);
        }
        return res;
    }
}C, D;

int main(void)
{
    scanf("%d", &n);
    char s[5];
    while (n--)
    {
        scanf("%s", s);
        if (s[0] == 'A')
        {
            int a, b, c;
            scanf("%d%d%d", &a, &b, &c);
            if (a == 0)
            {
                if (b > c)
                {
                    ++cnt;
                    type[++tot] = 3;
                }
                else type[++tot] = 0;
            }
            else if (a > 0)
            {
                k[++tot] = floor((1.0 * c - b) / a);
                if (k[tot] > MAXK) type[tot] = 0;
                else if (k[tot] < -MAXK) type[tot] = 3, ++cnt;
                else type[tot] = 1, C.update(k[tot], 1);
            }
            else
            {
                k[++tot] = ceil((1.0 * c - b) / a);
                if (k[tot] < -MAXK) type[tot] = 0;
                else if (k[tot] > MAXK) type[tot] = 3, ++cnt;
                else type[tot] = 2, D.update(k[tot], 1);
            }
        }
        else if (s[0] == 'D')
        {
            int i;
            scanf("%d", &i);
            if (!del[i])
            {
                del[i] = true;
                switch (type[i])
                {
                    case 1: C.update(k[i], -1); break;
                    case 2: D.update(k[i], -1); break;
                    case 3: --cnt; break;
                    default: break;
                }
            }
        }
        else
        {
            int x;
            scanf("%d", &x);
            printf("%d\n", C.query(x - 1) + (D.query(MAXK) - D.query(x)) + cnt);
        }
    }
    return 0;
}
```
{% endfolding %}