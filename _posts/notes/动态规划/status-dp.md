---
title: 基于状态进行优化的 DP
cover: false
plugins:
    - katex
abbrlink: d52d1ad2
date: 2022-08-18 09:32:02
categories:
    - 算法竞赛
    - 学习笔记
    - 动态规划
tag:
    - 动态规划
    - 状压 DP
    - 插头 DP
    - 倍增 DP
    - 笔记
headimg: https://pic.imgdb.cn/item/62fd978916f2c2beb10d6ef8.jpg
group: notes
hide: true
status: working
---

> 对 DP 的状态设计进行优化，可以引申出状态压缩和倍增优化。

<!--more-->

## 状态压缩

通过将状态利用压缩为整数，这样的操作称之为状态压缩，运用到动态规划上就是状压 DP。在枚举中有一种操作叫做”子集枚举“，就是枚举整数，然后利用位运算取得其在二进制下的每一位，然后就确定了一个元素的存在状态。

### 引入

首先先了解一些常用的二进制计算方法：

|意义（$n$ 位二进制意义下，位数从右起）|运算|解释|
|:-:|:-:|:-:|
|$n$ 的第 $k$ 位|`(n >> k) & 1`|显然|
|$n$ 的第 $k$ 位取反|`n xor (1 << k)`|异或可以取反|
|$n$ 的第 $k$ 位变 $1$|`n \| (1 << k)`|或可以赋值为 $1$|
|$n$ 的第 $k$ 位变 $0$|`n & (~(1 << k))`|创造出一个除了第 $k$ 位都是 $1$ 的东西进行与运算|
|$n$ 的后 $k$ 位|`n & ((1 << k) - 1)`|创造一个后 $k$ 位都是 $1$ 的东西进行与运算|

将由集合构成的状态通过进制转化为整数，称之为状态压缩。我们通过一道简单的题目来大概认识这玩意。

[最短 Hamilton 路径](http://noi-test.zzstep.com/contest/0x00%E3%80%8C%E5%9F%BA%E6%9C%AC%E7%AE%97%E6%B3%95%E3%80%8D%E4%BE%8B%E9%A2%98/0103%20%E6%9C%80%E7%9F%ADHamilton%E8%B7%AF%E5%BE%84)。

对于已经读到这的读者来说，很容易设计出状态：$F({a},i)$ 代表已经经过了 ${a}$ 中的所有节点，目前处于点 $i$ 的长度。这样行吗？要将 ${a}$ 用 `vector` 记录下来，这样 `F` 就成了一个 `map`，无法接受。

考虑状态压缩，我们搞一个长度为 $20$ 的 $01$ 序列，$1$ 代表经过了，$0$ 代表没有经过。$01$ 序列是什么？二进制串！二进制串的枚举是什么？枚举整数！

这样我们就可以定义 $f(i,j),i<2^n$，来表示跟刚才同样的内容。初始时显然 $f(1,0)=0$，最终目标是 $f(2^n-1,n-1)$（$n$ 个 $1$，终点为 $n-1$），接下来考虑如何进行转移。

有 $f(i,j)=\min\{f(i\text{ xor } (1 << j),k)+d(k,j)\},(i>>j)\And 1=1$，也就是说当 $j$ 已经访问过，$f(i,j)$ 才可以从 $k\rightarrow j$ 过来，而且状态集合中没有 $j$。$k$ 的取值仅限于在 $i\text{ xor } (1 << j)$ 中有的。

代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
constexpr int MAXN = 1 << 20;

int n;
int a[25][25];
int f[MAXN + 5][25];

int main(void)
{
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            scanf("%d", &a[i][j]);
    memset(f, 0x3f, sizeof(f));
    f[1][0] = 0;
    for (int i = 1; i < 1 << n; ++i)
        for (int j = 0; j < n; ++j)
            if ((i >> j) & 1) // i 的第 j 位是 1，要取反使得来的状态的第 j 位是 0
                for (int k = 0; k < n; ++k)
                    if ((i ^ (1 << j)) >> k & 1) // i xor (1 << j) 的第 k 位是 1
                        f[i][j] = min(f[i][j], f[i ^ (1 << j)][k] + a[k][j]);
    printf("%d\n", f[(1 << n) - 1][n - 1]);
    return 0;
}
```

注意到了吗？状态压缩后的状态内容是反的，在人看起来是反的。最后一位代表了第一位的状态。

### 状态压缩实例

我们来看更多问题来熟练应用状态压缩算法。

#### [Luogu P1171] 售货员的难题

[Portal](https://www.luogu.com.cn/problem/P1171).

这就是经典的货郎担问题（TSP）。设 $d(i,S)$ 代表当前在城市 $i$，经过的城市状压后为 $S$ 的最小距离即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
const int INF = 0x3f3f3f3f;

int n;
int d[25][25];
int f[25][(1 << 20) + 5]; 

int main(void)
{
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			scanf("%d", &d[i][j]);
	memset(f, 0x3f, sizeof(f));
	f[0][1] = 0;
    // 这里采用了刷表法
	for (int i = 0; i < (1 << n); ++i)
		for (int j = 0; j < n; ++j)
		{
			if (f[j][i] == INF) continue;
			for (int k = 0; k < n; ++k)
				if (((i >> k) & 1) == 0)
					f[k][i | (1 << k)] = min(f[k][i | (1 << k)], f[j][i] + d[j][k]);
		}
	int ans = INF;
	for (int i = 0; i < n; ++i)
		ans = min(ans, f[i][(1 << n) - 1] + d[i][0]);
	printf("%d\n", ans);
	return 0;
}
```
{% endfolding %}

#### [SCOI2005] 互不侵犯

[Portal](https://www.luogu.com.cn/problem/P1896).

设 $f(i,j,k)$ 表示考虑前 $i$ 行，放了 $j$ 个国王，当前行的状态压缩后为 $k$。现在枚举上一行的状态 $l$，问题就是判断 $l$ 合不合法。

不难发现，整个过程中我们只需要判断当前行和上一行的合法情况。对于 $k$，如果它存在相邻的两位都是 $1$，那就不行。我们可以将它左移或右移一位，这样在跟原来的值做与运算，结果不是 $0$ 的话就说明存在相邻的两位数是 $1$。跟上一行判断的时候也可以用类似的方法。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int n, K, nn;
i64 f[10][85][1030];

int getlen(int x)
{
    int res = 0;
    while (x)
    {
        x -= (x & (-x));
        ++res;
    }
    return res;
}

bool check(int x, int y)
{
    if (y & (y >> 1)) return false; // 上一行左右有人
    if (x & y) return false; // 当前人的头上有人
    if ((x << 1) & y) return false; // 当前人的左上有人
    if ((x >> 1) & y) return false; // 当前人的右上有人
    return true;
}

int main(void)
{
    scanf("%d%d", &n, &K);
    nn = 1 << n;
    f[0][0][0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = 0; j <= K; ++j)
            for (int k = 0; k < nn; ++k)
            {
                int len = getlen(k);
                if (len > j) continue;
                if (k & (k >> 1)) continue; // 左右有人，不行
                for (int l = 0; l < nn; ++l) // 上一行的状态
                    if (check(k, l)) f[i][j][k] += f[i - 1][j - len][l];
            }
    i64 ans = 0;
    for (int i = 0; i < nn; ++i)
        ans += f[n][K][i];
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P2622] 关灯问题 II

[Portal](https://www.luogu.com.cn/problem/P2622).

设 $f(i)$ 表示当前每盏灯的状态压缩后最少按多少次按钮，转移的时候使用刷表法，从全开开始枚举状态，依次动每个开关，看它能够转移到哪里。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
constexpr int MAXN = (1 << 10) + 5;

int n, nn, m;
int a[105][15], f[MAXN];

int main(void)
{
    scanf("%d%d", &n, &m);
    nn = 1 << n;
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            scanf("%d", &a[i][j]);
    memset(f, 0x3f, sizeof(f));
    f[nn - 1] = 0;
    for (int i = nn - 1; i >= 0; --i)
        for (int j = 1; j <= m; ++j)
        {
            int now = i;
            for (int k = 0; k < n; ++k)
                if (a[j][k + 1] == 1) now &= (~(1 << k));
                else if (a[j][k + 1] == -1) now |= (1 << k);
            f[now] = min(f[now], f[i] + 1);
        }
    if (f[0] == 0x3f3f3f3f) puts("-1");
    else printf("%d\n", f[0]);
    return 0;
}
```
{% endfolding %}

#### [USACO06NOV] Corn Fields G

[Portal](https://www.luogu.com.cn/problem/P1879).

我们将每一行的状态状压，然后用类似于“互不侵犯”的方式来判断这一行的状态是否合法（是否自己连着或者跟上一行连着）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 100000000;

int n, m;
int a[15][15];
int f[15][4100]; 

bool check(int row, int x)
{
	if (x & (x << 1)) return false;
	for (int i = 0; i < m; ++i)
		if (((x >> i) & 1) == 1 && !a[row][i]) return false;
	return true;
}

int main(void)
{
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i)
		for (int j = 0; j < m; ++j)
			scanf("%d", &a[i][j]);
	f[0][0] = 1;
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 0; j < (1 << m); ++j)
			if (check(i, j))
				for (int k = 0; k < (1 << m); ++k)
					if ((k & j) == 0) f[i][j] = (f[i][j] + f[i - 1][k]) % MOD;
	}
	int ans = 0;
	for (int i = 0; i < (1 << m); ++i)
		ans = (ans + f[n][i]) % MOD;
	printf("%d\n", ans);
	return 0;
}
```
{% endfolding %}

{% divider water %}

接下来的题目不再是蛮干了，而是有一些特殊的技巧。

#### 状压 DP 计数 | [Codeforces 11D] A Simple Task

[Portal](https://codeforces.com/problemset/problem/11/D).

设 $f(i,j)$ 代表终点为 $j$，走过的点状压后为 $j$ 的方案数。那么转移很容易写，如果走了回去那么就累计答案。

注意这样会把每一个环算重两遍，而且还会出写走 $i\rightarrow j,j\rightarrow$ 的错误情况，需要去掉。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, m;
i64 f[600000][20];
bool a[20][20];

int main(void)
{
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= m; ++i)
	{
		int u, v;
		scanf("%d%d", &u, &v); --u, --v;
		a[u][v] = a[v][u] = true;
	}
	i64 ans = 0;
	for (int i = 0; i < n; ++i)
		f[1 << i][i] = 1;
	for (int i = 1; i < (1 << n); ++i)
		for (int j = 0; j < n; ++j)
		{
			if (!f[i][j]) continue;
			for (int k = 0; k < n; ++k)
			{
				if (!a[j][k]) continue;
				if ((i & (-i)) > (1 << k)) continue;
				if ((1 << k) & i)
				{
					if ((1 << k) == (i & (-i)))
						ans += f[i][j];
				}
				else f[i | (1 << k)][k] += f[i][j];
			}
		}
	printf("%lld\n", (ans - m) / 2);
	return 0;
}
```
{% endfolding %}

#### 三进制状态压缩 | [NOI 2001] 炮兵阵地

[Portal](https://www.luogu.com.cn/problem/P2704).

我们可以这样定义，$2$ 表示放置了炮兵，$2$ 下面必须是 $1$，$1$ 下边必须是 $0$，同一行两个 $2$ 间隔不小于 $3$。

那么：

```txt
    0
    0
0 0 2 0 0
    1
    0
```

也就是说一个状态不再由 01 表示，而是由 012 表示，也就是三进制。

把每一行的状态压缩为一个 $M$ 位的三进制数，设 $f(i,j)$ 代表第 $i$ 行的状态压缩后为 $j$ 时，前 $i$ 行最多能放置多少合法的炮兵。

但即使这样，状态也很复杂，使用填表法的话状态转移方程会很复杂，所以我们采用刷表法，利用 dfs 进行更新。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
constexpr int P[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147};

int n, m;
char s[105][15];
int f[105][59050];
int v[15];

// 标记 row 可以填什么，last 为 row - 1 行的状态
// 0 表示只能填 0，1 表示只能填 1，2 表示只能填 0 或 2
// 结果记录在 v 数组中
inline void mark(int row, int last)
{
    for (int i = 0; i < m; ++i, last /= 3)
    {
        if (last % 3 == 2) v[i] = 1;
        else if (last % 3 == 1) v[i] = 0;
        else if (s[row][i] == 'H') v[i] = 0;
        else v[i] = 2;
    }
}

// 第 row 行的状态为 last，填第 row + 1 行，扫描到第 col 列，cnt 为填的炮兵数，now 为第 row + 1 行状态
void dfs(int row, int col, int last, int now, int cnt)
{
    if (col == m) // 0 ~ m-1 都填完了
    {
        f[row + 1][now] = max(f[row + 1][now], f[row][last] + cnt);
        return;
    }
    if (v[col] == 2 || v[col] == 0) dfs(row, col + 1, last, now, cnt); // 选择填 0，now 没有变化
    if (v[col] == 1) dfs(row, col + 1, last, now + P[col], cnt); // 填了 1，now 的第 col - 1 位加上 P[col - 1]
    if (v[col] == 2)
    {
        int v1 = v[col + 1], v2 = v[col + 2];
        // 右起两个不能放
        if (v[col + 1] == 2) v[col + 1] = 0;
        if (v[col + 2] == 2) v[col + 2] = 0;
        dfs(row, col + 1, last, now + 2 * P[col], cnt + 1); // 填了 2，now 的第 col - 1 位加上 2 * P[col - 1]
        v[col + 1] = v1, v[col + 2] = v2; // 还原
    }
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%s", s[i]);
    memset(f, 0xff, sizeof f);
    f[0][0] = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < P[m]; ++j)
        {
            if (f[i][j] == -1) continue;
            mark(i + 1, j); // 标记 dfs 中 row + 1 的可填状态
            dfs(i, 0, j, 0, 0);
        }
    int ans = 0;
    for (int i = 0; i < P[m]; ++i)
        ans = max(ans, f[n][i]);
    printf("%d\n", ans);
    return 0;
}
```

这道题也可以使用二进制状态压缩来做，不过相当麻烦。还可以使用轮廓线动态规划。轮廓线 DP（又称插头 DP）是状压 DP 的一种特殊形式，我们之前讨论的所有状压 DP 都是以“格子”作为状态的，但有的时候图形很复杂，需要以图形的轮廓作为状态。这种 DP 将在笔者后续的文章中论述{% emoji sticker 滑稽狂汗 %}。

### 子集和 DP（sosDP）

sosDP，指的是 Sum over Substes DP，用来解决子集类的求和问题（也能解决高维空间的求和问题，但是时空往往不允许）。

#### 高维前缀和

sosDP 的最重要应用便是实现高维前缀和。

> 给定一个含 $2^n$ 个整数的集合 $A$，我们需要求出每一个子集和，求出前缀和。即子集和为 $s_i$，那么前缀和 $f_i=\sum_{j\subset i}s_j$。

这个问题可以直接使用 sosDP 来求解，时间复杂度为 $O(n\times 2^n)$。

```cpp
for (int j = 0; j < n; ++j)  
    for (int i = 0; i < (1 << n); ++i)
        if ((i >> j) & 1) f[i] += f[i ^ (1 << j)];
```

这是什么？还记得 DP 对前缀和的递推吗？这里是一样的道理。注意到 $j$ 在外层，如果第 $j$ 位（代表维度）是 $1$，那么就将前缀和转移过来。

#### [ARC100E] Or Plus Max

[Portal](https://www.luogu.com.cn/problem/AT_arc100_c).

> 给你一个长度为 $2^n$ 的序列 $a$，每个$1\le K\le 2^n-1$，找出最大的 $a_i+a_j$（$i \mathbin{\mathrm{or}} j \le K$，$0 \le i < j < 2^n$）并输出。  
$\mathbin{\mathrm{or}}$ 表示按位或运算。

记得吗？前缀和可以做区间和，还可以做前缀最大值。而现在是两个最大值相加，那么我们用高维前缀和处理最大值和次大值即可。

注意到这里不是子集，而是最高位，因此还需要对高维前缀和数组取最大值。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int INF = 1e9;

int n, nn;
struct Node {
    int max1, max2;
    Node(int max1 = -INF, int max2 = -INF) : max1(max1), max2(max2) {}
    Node operator + (const Node &x) {
        Node y;
        if (max1 > x.max1) {
            y.max1 = max1;
            y.max2 = max(max2, x.max1);
        } else {
            y.max1 = x.max1;
            y.max2 = max(max1, x.max2);
        }
        return y;
    }
} a[300000];

int main(void) {
    scanf("%d", &n); nn = 1 << n;
    for (int i = 0; i < nn; ++i) scanf("%d", &a[i].max1);
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < nn; ++i)
            if ((i >> j) & 1) a[i] = a[i] + a[i ^ (1 << j)];
    int ans = 0;
    for (int i = 1; i < nn; ++i) {
        ans = max(ans, a[i].max1 + a[i].max2);
        printf("%d\n", ans);
    }
    return 0;
}

```
{% endfolding %}

### 基于轮廓线的状态压缩

## 倍增优化 DP

还记得我们在图上 DP 中提到的 [跑路](https://www.luogu.com.cn/problem/P1613) 吗？这种状态设计就是典型的倍增优化 DP。相似的例子还有 ST 表和树上倍增法求 LCA，它们都是倍增优化 DP，会有一个阶段的长度是”成倍增长的“，这就要比原本的“线性增长”快了很多。

### 例题

我们通过几道简单的题目来看一看如何设计倍增型的状态。

## Problemset

题放心，少不了的。

### 状态压缩 DP

#### [Codeforces 8C] Looking for Order

[Portal](https://codeforces.com/problemset/problem/8/C).

一次最多只能拿两个物品，那么转移的时候使用两个 `for` 循环。记 $f(i)$ 代表拿取得物品状态压缩后的最小代价，那么可以直接转移。但是这样的复杂度过高，实际上很多物品的转移都是互不干扰的，只要第一个物品确定了，那么剩下的就可以直接 `break` 掉了。

本题需要打印解，我们只需要在更新的时候看一下这个状态从哪里来的就可以了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
const int MAXN = (1 << 24);
const int INF = 0x3f3f3f3f;

int n, m;
int x[30], y[30];
int f[MAXN + 5], pre[MAXN + 5];

inline int dis(int p, int q)
{
	int a = x[p] - x[q], b = y[p] - y[q];
	return a * a + b * b;
}

int main(void) 
{
	scanf("%d%d%d", x, y, &n);
	for (int i = 1; i <= n; ++i) scanf("%d%d", x + i, y + i);
	memset(f, 0x3f, sizeof(f));
	f[0] = 0;
	for (int i = 0; i < (1 << n); ++i)
	{
		if (f[i] == INF) continue;
		for (int j = 0; j < n; ++j)
		{
			if (i & (1 << j)) continue;
			for (int k = 0; k < n; ++k)
			{
				if (i & (1 << k)) continue;
				int dp = (i | (1 << j) | (1 << k));
				int cost = f[i] + dis(0, j + 1) + dis(j + 1, k + 1) + dis(k + 1, 0);
				if (f[dp] > cost)
				{
					f[dp] = cost;
					pre[dp] = i;
				}
			}
			break; // 接下来的第 j 个物品会有其它的 i 来负责
		}
	}
	printf("%d\n", f[(1 << n) - 1]);
	int now = (1 << n) - 1;
	while (now)
	{
		printf("0 ");
		int update = (now ^ pre[now]);
		for (int i = 0; i < n; ++i)
			if (update & (1 << i)) printf("%d ", i + 1);
		now = pre[now];
	}
	puts("0");
	return 0;
}
```
{% endfolding %}

#### [Luogu P3694] 邦邦的大合唱站队

[Portal](https://www.luogu.com.cn/problem/P3694).

类似于上一题，设 $f(i)$ 表示当前 $M$ 个队伍完成站队的情况，完成记为 $1$，未完成记为 $0$，状态压缩后的结果。转移时采用填表法，仅当 $i$ 的第 $j$ 位是 $1$ 时转移：

$$
f(i)=\min\{f(i \text{ xor } (1<<j)+cnt[j]-sum(x,y))\}
$$

其中 $cnt[j]$ 代表 $j$ 这个乐队的人数，当中有 $sum(x,y)$ 个是不需要重新安排的。$sum$ 的具体求法见代码。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
constexpr int MM = (1 << 20) + 5;

int n, m, mm;
int a[100005], cnt[100005]; 
int sum[100005][20];
int f[MM];

int main(void)
{
    scanf("%d%d", &n, &m); mm = 1 << m;
    for (int i = 1; i <= n; ++i) 
    {
        scanf("%d", a + i);
        --a[i]; // 将它修改为从 0 开始编号的
        ++cnt[a[i]];
        for (int j = 0; j < m; ++j) sum[i][j] = sum[i - 1][j]; // 将前缀和移过来
        ++sum[i][a[i]]; // 修改当前 i 的前缀和
    }
    memset(f, 0x3f, sizeof(f));
    f[0] = 0;
    for (int i = 1; i < mm; ++i)
    {
        int len = 0; // 记录需要安排的人数
        for (int j = 0; j < m; ++j)
            if (i & (1 << j)) len += cnt[j];
        for (int j = 0; j < m; ++j)
            if (i & (1 << j)) f[i] = min(f[i], f[i ^ (1 << j)] + cnt[j] - (sum[len][j] - sum[len - cnt[j]][j]));
            // sum[len][j] 代表在长度为 n 中序列的前 len 位，也就是安排了的人数位，这样 j 的人数
            // 减去 sum[len-cnt[j]][j]，给这些人留出 cnt[j] 个位置。这样 j 的人数
    }
    printf("%d\n", f[mm - 1]);
    return 0;
}
```
{% endfolding %}

#### [UVA10817] Headmaster's Headache

[Portal](https://www.luogu.com.cn/problem/UVA10817).

设 $f(i,s_0,s_1,s_2)$ 代表扫描到第 $i$ 个人，没有人教的课程、一个人教和两个人教的课程状压后分别是 $s_0,s_1,s_2$，实现采用记忆化搜索。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;
const int INF = 1e9;

int S, m, n;
int c[125], s[125], f[125][300][300];

int dp(int i, int s0, int s1, int s2) {
	if (i == m + n) return s2 == (1 << S) - 1 ? 0 : INF;
	if (f[i][s1][s2] != -1) return f[i][s1][s2];
	int &ans = f[i][s1][s2];
	ans = INF;
	if (i >= m) ans = dp(i + 1, s0, s1, s2);
	int m0 = s[i] & s0, m1 = s[i] & s1;
	s0 ^= m0; s1 = (s1 ^ m1) | m0; s2 |= m1;
	return ans = min(ans, dp(i + 1, s0, s1, s2) + c[i]);
}

int main(void) {
	while (cin >> S >> m >> n && S) {
		memset(s, 0, sizeof(s));
		string st;
		getline(cin, st);
		for (int i = 0; i < m + n; ++i) {
			getline(cin, st);
			stringstream ss(st);
			ss >> c[i];
			int x;
			while (ss >> x) s[i] |= (1 << (x - 1));
		}
		memset(f, -1, sizeof(f));
		cout << dp(0, (1 << S) - 1, 0, 0) << '\n';
	}
	return 0;
}
```
{% endfolding %}

#### [HNOI2007] 梦幻岛宝珠

[Portal](https://www.luogu.com.cn/problem/P3188).

{% noteblock wrong %}
这不就是个 01 背包吗？看我秒杀紫题！
{% endnoteblock %}

一看数据范围：$\le 2^{30}$。

蚌埠住了……

但是发现了奇怪的东西：花费一定能被表示成 $a\times 2^b,a\le 10,b\le 30$。

### 插头 DP

部分题目相当复杂，但是训练价值也更大。

### 倍增优化 DP