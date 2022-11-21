---
title: 复杂状态的 DP 问题
plugins:
    - katex
cover: false
headimg: 'https://z3.ax1x.com/2021/11/07/I1PR2R.jpg'
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 动态规划
abbrlink: ee93771d
date: 2022-08-31 22:22:55
group: notes
hide: true
---

> 无论是线性 DP、背包、区间 DP 或是树形 DP，它们的区别都在于状态设计上。实际上还有一些奇怪的状态设计，将在本文进行介绍。包括图上 DP、环形结构、后效性处理、基环树 DP 等等。

<!--more-->

[\[UVa 437\] The Tower of Babylon](https://www.luogu.com.cn/problem/UVA437)。

{% noteblock %}

有 $n(n\le 30)$ 种可任意翻转的砖块，每种都有无穷多个。要求选一些摞成柱子，使得一个方块的底的两条边严格小于另一个方块的底的两条边。求最高高度。

{% endnoteblock %}

由于每个砖块的底面长宽分别严格小于它下方砖块的底面长宽，因此不难将这样一种关系作为建图的依据，而本题也就转化为最长路问题。

但问题是每个砖块都有三种摆放方式，怎么破？

我们很容易想到将一块砖拆成三块转来用，因为每种砖都有无穷多个。这是常见的思路。由于是严格小于，所以肯定没有环。

接下来会影响转移的就只用砖块的长和宽了。那么我们可以设 $f(i,j)$ 为“顶面尺寸是 $i$ 和 $j$”吗？不能。因为 $i$ 和 $j$ 这两个值可能很大。我们只需要用 $(id,k)$ 来间接表示这个状态，代表选取第 $id$ 块砖，使用第 $k$ 条边作为高。这样就可以正常求解了。但在代码中我们是直接用 $f(i)$，因为已经将一块砖拆成了三块转。

代码实现上还有一个问题，一个状态能从哪里转过来？必定是它的底面比那块砖小。这就需要先排序。然而这里的排序极为诡异，不是正常的排序，所以快速排序等方法不起作用，需要使用选择排序之类的方法。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
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

struct block
{
    int a, b, c;
    block(int a = 0, int b = 0, int c = 0) :
        a(a), b(b), c(c) {}
    inline bool operator < (const block &x) const {
        return (a < x.a && b < x.b) || (a < x.b && b < x.a);
    }
};

int n;
vector <block> v;
int f[95];

void mysort(void) // 选择排序
{
    for (int i = 0; i < v.size(); ++i)
        for (int j = i + 1; j < v.size(); ++j)
            if (v[j] < v[i]) swap(v[i], v[j]);
}

int main(void)
{
    int kase = 0;
    while (scanf("%d", &n) == 1 && n)
    {
        v.clear();
        memset(f, 0, sizeof(f));
        for (int i = 1; i <= n; ++i)
        {
            int x = read(), y = read(), z = read();
            v.push_back(block(x, y, z));
            v.push_back(block(y, z, x));
            v.push_back(block(z, x, y));
        }
        mysort();
        int ans = 0;
        for (int i = 0; i < v.size(); ++i)
        {
            for (int j = 0; j < i; ++j)
                if ((v[i].a > v[j].a && v[i].b > v[j].b) || (v[i].a > v[j].b && v[i].b > v[j].a))
                    f[i] = max(f[i], f[j]); // 在可以堆的部分中取最大的
            f[i] += v[i].c;
            ans = max(ans, f[i]);
        }
        printf("Case %d: maximum height = %d\n", ++kase, ans);
    }   
    return 0;
}
```
{% endfolding %}

本题还能用记忆化搜索来实现，而且不需要排序。感兴趣的读者可以自行学习 {% emoji 滑稽 %}。

## 环形 DP

这个环形不是说状态图是环形。

## 图上 DP

同样，这个图也不是说状态图是图，而是对于图的问题进行 DP。这类 DP 在基础 DP 中会比较综合，而且会与别的算法综合，但是它们都不难理解。



## 后效性处理

## 基环树 DP

## Problemset

这里的题目并不一定需要用本文介绍的知识来解决，但它们都有一个共同的特点：在非优化性 DP 问题中难度较大。所以请读者做好准备，但是它们对思维能力的提升也是巨大的，会使接下来的学习更为轻松。以下题目按照知识点排序，分别是线性 DP（含背包，区间 DP 和高维 DP），树形 DP，以及用到本文介绍的知识点的一些很难的题目。

### 简单问题

这里的问题很简单，或许需要综合别的算法。

#### [ZJOI2006] 物流运输

[Portal](https://www.luogu.com.cn/problem/P1772).

注意到数据范围小的吓人，所以可以想的暴力一点。设 $f(i)$ 代表前 $i$ 天的最小成本，那么转移的时候直接暴力枚举从什么时间开始走同一条最短路（走不通就是 INF 的代价）。最短路的处理直接多次 Dijkstra。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

#define pii pair<int, int>
#define i64 long long

using namespace std;
const int INF = 0x3f3f3f3f;

int n, m, k, e;
int f[105];
bool ban[25][205];

struct edge {
	int to, dist;
	edge(int to = 0, int dist = 0) :
		to(to), dist(dist) {}
};

vector <edge> G[25];
bool v[25], die[25];
int d[25];
void Dijkstra(void) {
	priority_queue <pii, vector<pii>, greater<pii> > q;
	memset(v, 0, sizeof(v));
	memset(d, 0x3f, sizeof(d));
	d[1] = 0;
	q.push(make_pair(0, 1));
	while (!q.empty()) {
		int u = q.top().second; q.pop();
		if (!v[u]) {
			v[u] = true;
			for (int i = 0; i < G[u].size(); ++i) {
				int v = G[u][i].to, w = G[u][i].dist;
				if (die[v]) continue;
				if (d[v] > d[u] + w) {
					d[v] = d[u] + w;
					q.push(make_pair(d[v], v));
				}
			}
		}
	}
}

int cost[105][105];

int main(void) {
	scanf("%d%d%d%d", &n, &m, &k, &e);
	while (e--) {
		int u, v, d;
		scanf("%d%d%d", &u, &v, &d);
		G[u].push_back(edge(v, d));
		G[v].push_back(edge(u, d));
	}
	
	int dx;
	scanf("%d", &dx);
	for (int i = 1; i <= dx; ++i) {
		int p, a, b;
		scanf("%d%d%d", &p, &a, &b);
		for (int j = a; j <= b; ++j)
			ban[p][j] = true;
	}
	
	// i 到 j 走同一条最短路 
	for (int i = 1; i <= n; ++i)
		for (int j = i; j <= n; ++j) {
			memset(die, 0, sizeof(die));
			for (int k = 1; k <= m; ++k)
				for (int t = i; t <= j; ++t)
					if (ban[k][t]) die[k] = true;
			Dijkstra();
			cost[i][j] = d[m];
		}
		
	memset(f, 0x3f, sizeof(f));
	f[0] = 0;
	for (int i = 1; i <= n; ++i) {
		if (cost[1][i] != INF) f[i] = cost[1][i] * i;
        else f[i] = INF;
		for (int j = 1; j < i; ++j)
			if (cost[j + 1][i] != INF && f[j] != INF) f[i] = min(f[i], f[j] + cost[j + 1][i] * (i - j) + k);
	}
    printf("%d\n", f[n]);
	return 0;
}
```
{% endfolding %}