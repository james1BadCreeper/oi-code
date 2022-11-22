---
title: 基于转移进行优化的 DP
cover: false
plugins:
    - katex
abbrlink: ab715ea1
date: 2022-11-01 22:11:23
categories:
    - 算法竞赛
    - 学习笔记
    - 动态规划
tag:
    - 斜率优化
    - 四边形不等式
    - 单调队列
    - 数据结构优化
    - 笔记
headimg: https://pic1.imgdb.cn/item/6366119716f2c2beb1ab6ef8.jpg
group: notes
hide: true
---

> 从动态规划的转移入手，可以用数据结构直接优化转移过程，或者根据决策单调性使用斜率优化、四边形不等式等。

<!--more-->

## 转移优化综述

{% noteblock quote %}
过早的优化是万恶之源。
{% endnoteblock %}

实际上最关键的事情是有一个好的状态设计，否则第一步就走错后面就完蛋了。

设计状态前，需要明确计算的是什么东西。如果状态设计的不够优秀，要考虑更换。否则，转移会变得相当困难甚至不可做。做了这么多 DP 题，读者应该有一定经验了。

当然，需要保证无后效性、满足最优子结构。

在设计好状态后，先不急着设计一个完美高效的转移。先想想最暴力的转移怎么写，然后在考虑进行优化，比如化简式子、使用数据结构等。

## 数据结构优化 DP

数据结构很多，能优化 DP 的也不少。本质思想就是说，DP 的决策集合是在变化的，我们使用数据结构维护这一个集合以实现高速修改和查询。

### 前缀和优化

当转移方程中有区间和之类的内容，前缀和就派上用场了。

### 线段树优化

线段树很强大，可以进行区间查询，也可以对权值进行统计。这就意味着它能优化的东西很多。

#### [Codeforces 833B] The Bakery

[Portal](https://codeforces.com/problemset/problem/833/B).

> 将一个长度为 $n(1\le \le 35000)$ 的序列分为 $k(1\le k\le 50)$ 段，使得总价值最大。一段区间的价值表示为区间内不同数字的个数。

设 $f(i,j)$ 代表考虑前 $i$ 个数，分为 $j$ 段的总价值。采用刷表法：

$f(i,j)$

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

struct Segment_Tree {
    int T[140005], tag[140005];
    inline void pushdown(int o) {
        if (!tag[o]) return;
        T[o << 1] += tag[o], T[o << 1 | 1] += tag[o];
        tag[o << 1] += tag[o], tag[o << 1 | 1] += tag[o];
        tag[o] = 0;
    }
    void update(int o, int l, int r, int x, int y, int k) {
        if (x <= l && r <= y) return T[o] += k, tag[o] += k, void();
        pushdown(o); int mid = l + r >> 1;
        if (x <= mid) update(o << 1, l, mid, x, y, k);
        if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
        T[o] = max(T[o << 1], T[o << 1 | 1]);
    }
    int query(int o, int l, int r, int x, int y) {
        if (x <= l && r <= y) return T[o];
        pushdown(o); int mid = l + r >> 1, res = 0;
        if (x <= mid) res = max(res, query(o << 1, l, mid, x, y));
        if (mid < y) res = max(res, query(o << 1 | 1, mid + 1, r, x, y));
        return res;
    }
} T[51];

int n, k, a[35005], pre[35005], lst[35005];
int f[35005][55];

int main(void) {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", a + i);
        pre[i] = lst[a[i]], lst[a[i]] = i;
        for (int j = 0; j < k; ++j) {
            T[j].update(1, 0, n, pre[i], i - 1, 1);
            int f = T[j].query(1, 0, n, 0, i - 1);
            T[j + 1].update(1, 0, n, i, i, f);
        }
    }
    printf("%d\n", T[k].query(1, 0, n, n, n));
    return 0;
}
```
{% endfolding %}

## 单调性优化 DP

有些 DP 的转移决策是具有单调性的，这种时候就可以对决策集合进行单调性维护。单调队列可以维护决策取值范围上下界均单调变化。

### 单调队列

还记得“限制长度的最大子段和”吗？那就是一个经典的单调队列优化 DP。

单调队列可以维护决策取值范围上下变化（队首队尾均可以弹出），而且每个决策在候选集合中至多插入或删除一次。

#### [Luogu P1725] 琪露诺

[Portal](https://www.luogu.com.cn/problem/P1725).

几乎就是模板。设 $f(i)$ 为到 $i$ 的最大分数，转移的时候直接从单调队列里获取。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, l, r;
int a[200005], f[200005];
int L = 1, R = 0, q[200005];

int main(void)
{
    scanf("%d%d%d", &n, &l, &r);
    for (int i = 0; i <= n; ++i) scanf("%d", a + i);
    memset(f, 0xbf, sizeof(f));
    f[0] = a[0];
    int ans = -1e9;
    for (int i = l; i <= n; ++i) {
        while (L <= R && f[q[R]] <= f[i - l]) --R;
        while (L <= R && q[L] + r < i) ++L;
        q[++R] = i - l;
        f[i] = f[q[L]] + a[i];
        if (i + r > n) ans = max(ans, f[i]);
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [NOIP2017 普及组] 跳房子

[Portal](https://www.luogu.com.cn/problem/P3957).

考虑二分答案，判定的时候使用单调队列优化 DP 求出最大分数即可。

问题是，如何入队？使用一个指针，在扫描到一个新的位置时开始判断它之前的是否可行，就是一个滑动窗口的过程。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
typedef long long i64;
const i64 INF = 1e18;

int n, d, k;
int x[500005], s[500005];
i64 f[500005]; // 考虑到第 i 个位置的最大得分
int Q[500005];

bool P(int g)
{
    int l = (d > g ? d - g : 1), r = d + g;
    int L = 1, R = 0, j = 0;
    i64 ans = -INF;
    memset(f, 0xbf, sizeof(f));
    memset(Q, 0, sizeof(Q));
    f[0] = 0;
    for (int i = 1; i <= n; ++i) {
        while (j < i && x[i] - x[j] >= l) {
            if (f[j] > -INF) {
                while (L <= R && f[Q[R]] <= f[j]) --R;
                Q[++R] = j;
            }
            ++j;
        }
        while (L <= R && x[i] - x[Q[L]] > r) ++L;
        if (L <= R) f[i] = f[Q[L]] + s[i];
        ans = max(ans, f[i]);
    }
    return ans >= k;
}

int main(void)
{
    scanf("%d%d%d", &n, &d, &k);
    i64 sum = 0;
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d", x + i, s + i);
        if (s[i] > 0) sum += s[i];
    }
    if (sum < k) return puts("-1"), 0;
    int L = 0, R = 1000000001;
    while (L + 1 != R) {
        int mid = L + R >> 1;
        if (P(mid)) R = mid;
        else L = mid;
    }
    printf("%d\n", R);
    return 0;
}
```
{% endfolding %}

#### [USACO11OPEN] Mowing the Lawn G 

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/P2627).

Farmer John 的草坪非常脏乱，因此，Farmer John 只能够让他的奶牛来完成这项工作。Farmer John 有 $N$（$1\le N\le 10^5$）只排成一排的奶牛，编号为 $1\ldots N$。每只奶牛的效率是不同的，奶牛 $i$ 的效率为 $E_i$（$0\le E_i\le 10^9$）。

靠近的奶牛们很熟悉，因此，如果 Farmer John安排超过 $K$ 只连续的奶牛，那么，这些奶牛就会罢工去开派对 :)。因此，现在 Farmer John 需要你的帮助，计算 FJ 可以得到的最大效率，并且该方案中没有连续的超过 $K$ 只奶牛。
{% endnoteblock %}

设 $f(i,0)$ 表示不取第 $i$ 头奶牛的最大效率，$f(i,1)$ 则取，那么：

$$
f(i,0)=\max\{f(i-1,0),f(i-1,1)\}\\
f(i,1)=\max_{j=i-k+1}^{i}\left\{f(j,0)+\sum_{l=j}^{i}e[l]\right\}
$$

第二个式子可以使用前缀和变形并拆开：

$$
f(i,1)=\max_{j=i-k+1}^{i}\left\{f(j,0)-s[j]\right\}+s[i]
$$

`max` 中的内容便可以使用单调队列来维护。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, k;
int a[100005];
int Q[100005], L = 1, R = 0;
i64 s[100005], f[100005][2];

int main(void)
{
	scanf("%d%d", &n, &k);
    Q[++R] = 0;
	for (int i = 1; i <= n; ++i) {
		scanf("%d", a + i);
		s[i] = s[i - 1] + a[i];
		f[i][0] = max(f[i - 1][0], f[i - 1][1]);
        while (L <= R && i - Q[L] > k) ++L;
        f[i][1] = f[Q[L]][0] + s[i] - s[Q[L]];
        while (L <= R && f[i][0] - s[i] > f[Q[R]][0] - s[Q[R]]) --R;
        Q[++R] = i;
	}
	printf("%lld\n", max(f[n][0], f[n][1]));
	return 0;
}
```
{% endfolding %}

#### [Luogu P3800] Power 收集

[Portal](https://www.luogu.com.cn/problem/P3800).

对每一行都进行一次 DP，单调队列值的获取来自上一行。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m, k, t;
int f[4005][4005];
int Q[4005], L, R;

int main(void)
{
    scanf("%d%d%d%d", &n, &m, &k, &t);
    while (k--) {
        int x, y, w;
        scanf("%d%d%d", &x, &y, &w);
        f[x][y] = w;
    }
    for (int i = 2; i <= n; ++i) {
        L = 1, R = 0;
        for (int j = 1; j <= t; ++j) {
            while (L <= R && f[i - 1][j] >= f[i - 1][Q[R]]) --R;
            Q[++R] = j;
        }
        for (int j = 1; j <= m; ++j) {
            while (Q[L] + t < j) ++L;
            if (j + t <= m) {
                while (L <= R && f[i - 1][j + t] >= f[i - 1][Q[R]]) --R;
                Q[++R] = j + t;
            }
            f[i][j] += f[i - 1][Q[L]];
        } 
    }
    int ans = 0;
    for (int i = 1; i <= m; ++i)
        ans = max(ans, f[n][i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### 单调队列优化多重背包

### 斜率优化

### 四边形不等式

## Problemset

