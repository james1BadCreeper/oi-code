---
title: 区间 DP 与高维 DP
plugins:
  - katex
headimg: https://s1.ax1x.com/2022/08/04/vm9MXF.jpg
cover: false
categories:
    - 算法竞赛
    - 学习笔记
    - 动态规划
tag:
    - 动态规划
    - 区间 DP
    - 线性 DP
abbrlink: 1177a5e3
group: notes
hide: true
status: working
date: 2022-07-28 21:27:26
---

> 区间 DP 是线性 DP 的一种特殊形式，它在分阶段地划分问题时，与阶段中元素出现的顺序和由前一阶段的哪些元素合并而来有很大的关系。高维 DP 则是线性 DP 的扩展，大部分是从一条链扩展为了二维结构。

<!--more-->

这两个内容都属于都属于线性 DP 的变形。

## 区间 DP

区间类动态规划是线性动态规划的扩展。

它在分阶段地划分问题时，与阶段中元素出现的顺序和由前一阶段的哪些元素合并而来由很大的关系。其转移方程一般形如 $f_{i,j}=\max\{f_{i,k}+f_{k+1,j}+cost\mid i \le k < j\}$，$cost$ 为将这两组元素合并起来的代价。   
也有另一类区间 DP 的转移方程形如 $f_{i,j}=\max\{f_{i+1,j}+cost_i,f_{i,j-1}+cost_j\}$，这一类的区间是通过左右端点逐个扩展的。

由于大区间的答案依赖于小区间的答案，所以要将区间的长度作为“阶段”来进行 DP。

但是这么说很闹鬼，我们通过题目来认识区间 DP。

### 模板

我们分别来看一看刚才所说的两种转移的具体实例。

#### \[NOI1995\] 石子合并

[Portal](https://www.luogu.com.cn/problem/P1880).

以最大得分为例。

先假设排成一排，设 $f[l][r]$ 表示合并区间 $[l,r]$ 的最大得分，则有 $f[l][r]=\max\limits_{l \leqslant k < r}\{f[l,k]+f[k+1][r]\}+\sum\limits_{i=l}^r a_i$。

编程时有两种思路，一是记忆化搜索，二是递推，但注意递推时要以区间长度递增的顺序来推，因为大区间的答案依赖于小区间的答案。

接下来考虑如何处理环。我们将这条链延长两倍，变成 $n \times 2$ 堆，这样就可以转化成 $n$ 条链，分别为 $[1,n],[2,n+1][3,n+2],\cdots,[n,2n-1]$。**这种复制二倍链是处理环形 DP 极为有效的方式**（因为环形问题中肯定有一条边是用不到的，那么我们将它从任意位置断开，然后就相当于从每个位置断开的情况都求了）。

代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n;
int a[205], sum[205];
int f[205][205];

int main(void)
{
    n = read();
    for (int i = 1; i <= n; ++i) a[i] = a[n + i] = read();
    for (int i = 1; i <= (n << 1); ++i) sum[i] = sum[i-1] + a[i];

    int ans = 1000000000;
    memset(f, 0x3f, sizeof(f));
    for (int i = 1; i <= (n << 1); ++i) f[i][i] = 0;
    for (int len = 2; len <= n; ++len) // 我们只需要处理到 len = n 即可
        for (int i = 1; i <= (n << 1) - len + 1; ++i) {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                f[i][j] = min(f[i][j], f[i][k] + f[k+1][j] + sum[j] - sum[i-1]);
        }
    for (int i = 1; i <= n; ++i) ans = min(ans, f[i][i + n - 1]);
    printf("%d\n", ans);

    ans = 0;
    memset(f, 0, sizeof(f));
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= (n << 1) - len + 1; ++i) {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                f[i][j] = max(f[i][j], f[i][k] + f[k+1][j] + sum[j] - sum[i-1]);
        }
    for (int i = 1; i <= n; ++i) ans = max(ans, f[i][i + n - 1]);
    printf("%d\n", ans);
    return 0;
}
```

状态有 $O(n^2)$ 个，每个状态的决策有 $O(n)$ 个，因此总时间复杂度为 $O(n^3)$。

#### [IOI2000] 回文字串 

[Portal](https://www.luogu.com.cn/problem/P1435).

设 $f[l][r]$ 表示处理区间 $[l,r]$ 的最小代价，那么显然可以花费 $1$ 来处理左端点或右端点，如果左右端点相等的话还可以等价于处理 $[l+1,r-1]$。显然，这是第二种转移。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n;
char s[1005];
int f[1005][1005];

int main(void)
{
	scanf("%s", s + 1);
	n = strlen(s + 1);
	memset(f, 0x3f, sizeof(f));
	for (int i = 1; i <= n; ++i)
		f[i][i] = 0, f[i][i - 1] = 0;
	for (int len = 2; len <= n; ++len)
	{
		for (int i = 1; i <= n - len + 1; ++i)
		{
			int j = i + len - 1;
			f[i][j] = min(f[i][j - 1], f[i + 1][j]) + 1;
			if (s[i] == s[j]) f[i][j] = min(f[i][j], f[i + 1][j - 1]);
		}
	}
	printf("%d\n", f[1][n]);
	return 0;
}
```

状态有 $O(n^2)$ 个，每个状态的决策只有有 $O(1)$ 个，因此总时间复杂度为 $O(n^2)$。

### 简单应用

这里是一些简单的区间 DP 题目。

#### [CQOI2007] 涂色

[Portal](https://www.luogu.com.cn/problem/P4170).

设 $f(i,j)$ 为完成 $[i,j]$ 涂色需要的最少步数。首先有 $f(i,i)=1$。

当 $s[i] = s[j]$ 时，$f(i,j)=\min\{f(i,j-1),f(i+1,j)\}$（只需要在涂其中某一个时，先涂这种颜色，将涂色的区间改为 $[i,j]$，再考虑 $[i+1,j-1]$ 即可）。

在任何情况下，$[i,j]$ 都能由 $[i,k]$ 和 $[k+1,j]$ 拼出来。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n;
char s[55];
int f[55][55];

int main(void)
{
    scanf("%s", s + 1);
    n = strlen(s + 1);
    memset(f, 0x3f, sizeof(f));
    for (int i = 1; i <= n; ++i)
        f[i][i] = 1;
    for (int len = 1; len < n; ++len)
        for (int i = 1; i <= n - len; ++i) {
            int j = i + len;
            if (s[i] == s[j]) f[i][j] = min(f[i][j - 1], f[i + 1][j]);
            for (int k = i; k < j; ++k)
                f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j]);
        }
    printf("%d\n", f[1][n]);
    return 0;
}
```
{% endfolding %}

#### [NOIP2007 提高组] 矩阵取数游戏

[Portal](https://www.luogu.com.cn/problem/P1005).

每一行的操作都是独立的，因此可以一行一行的思考。设 $f[i][j]$ 为一行中取到只剩下 $[i,j]$ 的最大值，转移只有两种方式：左取和右取。时间复杂度为 $\mathcal{O}(n{m}^2)$。注意需要高精（当然可以用 `__int128`）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

#define LL __int128

using namespace std;

inline LL read(void)
{
    LL x = 0;
    int c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x << 3) + (x << 1) + (c ^ 48), c = getchar();
    return x;
}

inline void print(LL x)
{
    if (x > 9) print(x / 10);
    putchar(x % 10 + '0');
}

int n, m;
LL a[85];
LL f[85][85];

int main(void)
{
    n = read(), m = read();
    LL ans = 0;
    while (n--) {
        for (int i = 1; i <= m; ++i) a[i] = read();
        memset(f, 0, sizeof(f));
        f[1][m] = 0;
        LL k = 1;
        for (int len = m - 1; len >= 0; --len) {
            k <<= 1;
            for (int i = 1; i <= m - len + 1; ++i) {
                int j = i + len - 1;
                f[i][j] = max(f[i - 1][j] + a[i - 1] * k, f[i][j + 1] + a[j + 1] * k);
            }
        }
        LL res = -1;
        for (int i = 1; i <= m; ++i)
            res = max(res, f[i][i] + a[i] * k);
        ans += res;
    }
    print(ans); putchar('\n');
    return 0;
}
```
{% endfolding %}

#### [Codeforces 245H] Queries for Number of Palindromes

[Portal](https://codeforces.com/problemset/problem/245/H).

尽管用专门的字符串算法可以很高效地解决这个问题，但是还可以用区间 DP 来解决。根据经验，我们设 $f(l,r)$ 表示这个范围内的回文子串个数。那么根据容斥原理显然有：

$$
f(l,r) = f(l,r-1) + f(l+1,r) - f(l+1,r-1) + \text{is\_pal}(s_{l\cdots r})
$$

现在的问题就是如何处理 `is_pal`。虽然字符串 Hash 可以在线性复杂度内解决这个问题，但没必要，因为我们的 DP 是平方复杂度。实际上这个问题也可以以平方复杂度用区间 DP 来求解。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

char s[5005];
int n, f[5005][5005];
bool is_pal[5005][5005];

int main(void)
{
    scanf("%s", s + 1);
    n = strlen(s + 1);
    for (int len = 1; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && (i + 1 >= j || is_pal[i + 1][j - 1])) is_pal[i][j] = true;
        }
    for (int len = 1; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            f[i][j] = f[i + 1][j] + f[i][j - 1] - f[i + 1][j - 1] + is_pal[i][j];
        }
    int q, l, r;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%d", &l, &r);
        printf("%d\n", f[l][r]);
    }
    return 0;
}
```
{% endfolding %}

#### \[IOI1998\] Polygon

[Portal](https://www.luogu.com.cn/problem/P4342).    
特别地，请将数据范围看作 $n\leqslant 150$。

我们可以枚举删哪一条边，本题就转化为链上问题。设 $f[l,r]$ 表示区间 $[l,r]$ 的最大值行吗？不行！由于负负得正的存在，这样的状态设计不满足最优子结构性质，那怎么办？在记录一个记录最小值的就行了！

设 $f[l,r,0],f[l,r,1]$ 分别代表区间的最大最小值。当运算为求和是，正常更新即可。乘法需要注意，最大值的来源只可能是最大乘最大或最小乘最小，而最小值的来源则是所有的都可能。实际上，这时候如果搞不清楚，还不如都写上，反正不会得出错误的解就行（没人会卡这里的常数吧{% emoji 滑稽 %}）。代码如下：

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

const int INF = 1000000000;

int n;
char c[105];
int a[105];
int f[105][105][2];

int main(void)
{ 
    cin >> n;
    for (int i = 1; i <= n; ++i)
        cin >> c[i] >> a[i];
    for (int i = n + 1; i <= (n << 1); ++i)
        c[i] = c[i - n], a[i] = a[i - n];
    for (int l = 1; l <= (n << 1); ++l)
        for (int r = 1; r <= (n << 1); ++r)
            if (l != r) f[l][r][0] = -INF, f[l][r][1] = INF;
            else f[l][r][0] = f[l][r][1] = a[l];
    
    for (int len = 2; len <= (n << 1); ++len)
        for (int l = 1; l <= (n << 1) - len + 1; ++l) {
            int r = l + len - 1;
            for (int k = l; k < r; ++k) {
                if (c[k + 1] == 't') {
                    f[l][r][0] = max(f[l][r][0], f[l][k][0] + f[k + 1][r][0]);
                    f[l][r][1] = min(f[l][r][1], f[l][k][1] + f[k + 1][r][1]);
                } else {
                    f[l][r][0] = max({f[l][r][0], f[l][k][0] * f[k + 1][r][0], f[l][k][1] * f[k + 1][r][1]});
                    f[l][r][1] = min({f[l][r][1], f[l][k][0] * f[k + 1][r][1], f[l][k][1] * f[k + 1][r][0], f[l][k][0] * f[k + 1][r][0], f[l][k][1] * f[k + 1][r][1]});
                }
            }
        }

    int ans = -INF;
    for (int i = 1; i <= n; ++i) ans = max(ans, f[i][i + n - 1][0]);
    printf("%d\n", ans);
    for (int i = 1; i <= n; ++i)
        if (f[i][i + n - 1][0] == ans)
            printf("%d ", i);
    putchar('\n');
    return 0;
}
```
{% endfolding %}

#### \[SCOI2003\] 字符串折叠

[Portal](https://www.luogu.com.cn/problem/P4302).

首先这肯定是第一种区间 DP，因为折叠可以从中间抠，肯定不是从两头扩展。

设 $f(i,j)$ 为 $[i,j]$ 的最短折叠，按照它的题意，应该有以下三种折叠方式：

- 自己作为一个折叠，显然这时 $f(i,j)=j-i+1$；
- 把若干个重复的东西拼成一个，这怎么搞？不要紧，数据范围那么小，我们暴力点想{% emoji tong ... %}，这些重复的东西长度肯定是一样的，那么我们就枚举长度 $k$，而且显然要求 $k\mid j-i+1$，这一过程我们 `for` 一遍检查是不是约数就可以了，因为检查能不能折叠的操作更费时间，加上括号和数字的代价，有 $f(i,j)=f(i,i+k-1)+2+D\left(\cfrac{j-i+1}{k}\right)$，其中 $D$ 表示求一个数字的位数，在这之前我们要线性扫一遍看能不能折叠，进行一轮这样的转移的时间复杂度为 $\mathcal{O}(n^{1.5})$；
- 枚举中间点进行转移，就是把两坨东西拼在一起，这时候 $f(i,j)=\min \left\{f(i,k) + f(k+1,j)\mid k\in [i,j)\right\}$。

状态有 $\mathcal{O}(n^2)$ 个，总时间复杂度为 $\mathcal{O}(n^{3.5})$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, D[105];
char s[105];
int f[105][105];

inline bool check(int l, int r, int len)
{
    for (int i = l; i <= r; ++i)
        if (s[i] != s[(i - l) % len + l]) return false;
    return true;
}

int main(void)
{
    for (int i = 0; i < 10; ++i) D[i] = 1;
    for (int i = 10; i < 100; ++i) D[i] = 2;
    for (int i = 100; i < 103; ++i) D[i] = 3;
    scanf("%s", s + 1);
    n = strlen(s + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = i; j <= n; ++j)
            f[i][j] = j - i + 1;
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j]);
            for (int k = 1; k <= len; ++k) {
                if (len % k != 0) continue;
                if (check(i, j, k)) f[i][j] = min(f[i][j], f[i][i + k - 1] + 2 + D[len / k]);
            }
        }
    printf("%d\n", f[1][n]);
    return 0;
}
```
{% endfolding %}

## 高维 DP

其实在之前就已经见识过高维 DP 了。像 [[NOIP2002 普及组] 过河卒](https://www.luogu.com.cn/problem/P1002)。要知道的是它们也可以用滚动数组优化。我们直接上题：

### [NOIP2014 提高组] 飞扬的小鸟

[Portal](https://www.luogu.com.cn/problem/P1941).

设 $f(i,j)$ 代表飞到 $i$，高度为 $j$ 的最小点击数。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
constexpr int INF = 0x3f3f3f3f;

int n, m, k;
int x[10005], y[10005], low[10005], high[10005];
bool e[10005];
int f[10005][2005];

int main(void)
{
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d%d", x + i, y + i);
        low[i] = 1, high[i] = m;
    }
    for (int i = 1, a, b, c; i <= k; ++i)
    {
        scanf("%d%d%d", &a, &b, &c);
        e[a] = true;
        low[a] = b + 1;
        high[a] = c - 1;
    }
    memset(f, 0x3f, sizeof(f));
    for (int i = 1; i <= m; ++i) f[0][i] = 0;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = x[i] + 1; j <= x[i] + m; ++j) f[i][j] = min(f[i - 1][j - x[i]] + 1, f[i][j - x[i]] + 1); // 上升
        for (int j = m + 1; j <= x[i] + m; ++j) f[i][m] = min(f[i][m], f[i][j]); // 将飞到天上的拉回来
        for (int j = 1; j <= m - y[i]; ++j) f[i][j] = min(f[i][j], f[i - 1][j + y[i]]); // 下降
        // 禁区
        for (int j = 0; j < low[i]; ++j) f[i][j] = INF;
        for (int j = high[i] + 1; j <= m; ++j) f[i][j] = INF;
    }
    int ans = INF;
    for (int i = 1; i <= n; ++i) ans = min(ans, f[n][i]);
    if (ans < INF) printf("1\n%d\n", ans);
    else
    {
        puts("0");
        for (int i = n; i >= 1; --i)
        {
            bool flag = false;
            for (int j = 1; j <= m; ++j)
                if (f[i][j] < INF) 
                {
                    flag = true;
                    break;
                }
            if (flag)
            {
                ans = 0;
                for (int j = 1; j <= i; ++j)
                    if (e[j]) ++ans;
                printf("%d\n", ans);
                return 0;
            }
        }
    }
    return 0;
}
```
{% endfolding %}

### [CSP-S2019] Emiya 家今天的饭

[Portal](https://www.luogu.com.cn/problem/P5664).

如果没有第三种限制，那么直接使用乘法原理碾过去即可。那么我们只需要求解出违反限制三的方案数，然后减去即可。

也就是说要求由食材的出现次数大于 $\left\lfloor \cfrac{k}{2}\right\rfloor$，不难发现满足这一条件的食材最多只有 $1$ 个，否则 $\left(\left\lfloor \cfrac{k}{2}\right\rfloor+1\right)\times 2 > k$。于是我们可以枚举这一个食材，设该食材为 $x$，每一个食材都进行 DP：

设 $f(i,k,c)$ 代表考虑前 $i$ 个烹饪方法，总共选出 $k$ 道菜，其中 $c$ 个菜使用食材 $x$，那么：

$$
f(i,k,c)=\max\begin{cases}
f(i-1,k,c),\\
f(i-1,k-1,c-1)\times a_{i,j},&j=x,\\
f(i-1,k-1,c)\times a_{i,j},&j\ne x.
\end{cases}
$$

但是这样的状态数爆炸了，有 $O(n^3)$ 个，转移有 $O(m)$ 个，还要进行 $O(m)$ 次 DP，时间复杂度 $O(n^3 m^2)$，无法通过。

$c>\left\lfloor \cfrac{k}{2}\right\rfloor$ 意味着什么？注意它等价于 $c>\cfrac{k}{2}$，也就是 $c-(k-c)>0$，也就是使用 $x$ 的菜肴数量减去未使用 $x$ 的菜肴数量。设 $f(i,\Delta c)$ 代表考虑前 $i$ 个烹饪方法，$x$ 用的次数剪掉 $x$ 未被用的次数为 $\Delta c$，那么：

1. 若 $j=x$，则 $f(i,\Delta c + 1)\leftarrow f(i,\Delta c+1)+f(i-1,\Delta c)\times a_{i,j}$；
2. 若 $j\ne x$，则 $f(i,\Delta c - 1)\leftarrow f(i,\Delta c-1)+f(i-1,\Delta c)\times a_{i,j}$；
3. 不选， $f(i,\Delta c)\leftarrow f(i,\Delta c) + f(i-1,\Delta c)$。

最后的答案就是所有 $\Delta$ 为正的 $f(n,\Delta c)$ 和，这样就满足 $c-(k-c)>0$。

还可以进一步进行优化：对于第二种转移，我们不需要枚举 $j$，而是直接使用 $a_i$ 的前缀和减去 $a_{i,x}$ 即可。

实现时由于数组的下标不能是负的，所以我们只需要给下标加上 $100$ 即可。最终时间复杂度为 $O(m\times n^2)$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
const int MOD = 998244353;

int n, m;
int S[105];
int a[105][2005];
int f[105][205];

int main(void)
{
    scanf("%d%d", &n, &m);
    int ans = 1;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
            scanf("%d", &a[i][j]), S[i] = (S[i] + a[i][j]) % MOD;
        ans = 1ll * ans * (S[i] + 1) % MOD;
    }
    ans = (ans - 1 + MOD) % MOD;
    for (int x = 1; x <= m; ++x)
    {
        memset(f, 0, sizeof(f));
        f[0][0 + 100] = 1;
        for (int i = 1; i <= n; ++i)
        {
            int re = (S[i] + MOD - a[i][x]) % MOD; // 利用前缀和进行计算
            for (int j = 1; j <= 201; ++j)
            {
                f[i][j + 1] = (f[i][j + 1] + 1ll * a[i][x] * f[i - 1][j]) % MOD;
                f[i][j - 1] = (f[i][j - 1] + 1ll * re * f[i - 1][j]) % MOD;
                f[i][j] = (f[i][j] + f[i - 1][j]) % MOD;
            }
        }
        int no = 0;
        for (int i = 1; i <= n; ++i)
            ans = (ans - f[n][i + 100] + MOD) % MOD;
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

## Problemset

~~嗯，题真多。~~

### 区间 DP

这里是区间 DP 的内容。

#### [Codeforces 607B] Zuma

[Portal](https://codeforces.com/problemset/problem/607/B).

设 $f(i,j)$ 代表消掉区间 $[i,j]$ 的答案，转移对于读到这里的读者来说不是困难。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n;
int a[505];
int f[505][505];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    memset(f, 0x3f, sizeof(f));
    for (int i = 1; i <= n; ++i) f[i][i] = 1, f[i][i - 1] = 1;
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i)
        {
            int j = i + len - 1;
            if (a[i] == a[j]) f[i][j] = f[i + 1][j - 1];
            for (int k = i; k < j; ++k)
                f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j]);
        }
    printf("%d\n", f[1][n]);
    return 0;
}
```
{% endfolding %}

#### [NOIP2003 提高组] 加分二叉树

[Portal](https://www.luogu.com.cn/problem/P1040).

设 $f[i][j]$ 代表区间 $[i,j]$ 的最大答案，那么 $f[i][j]=\max\{f[i,k-1]\times f[k+1][j]+f[k][k]\}$。初始条件要注意，由于空树的贡献是 $1$，所以要令 $f(i,i-1)=1$。本题需要打印解，用 $root[i][j]$ 记录区间 $[i,j]$ 所选取的根节点，然后递归遍历二叉树即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int n;
int a[35], root[35][35];
i64 f[35][35];

void print_ans(int l, int r)
{
    if (l > r) return;
    printf("%d ", root[l][r]);
    if (l == r) return;
    print_ans(l, root[l][r] - 1);
    print_ans(root[l][r] + 1, r);
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) 
    {
        scanf("%d", &a[i]);
        f[i][i] = a[i];
        f[i][i - 1] = 1;
        root[i][i] = i;
    }
    f[n + 1][n] = 1;
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i)
        {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                if (f[i][j] < f[i][k - 1] * f[k + 1][j] + a[k])
                {
                    f[i][j] = f[i][k - 1] * f[k + 1][j] + a[k];
                    root[i][j] = k;
                }
        }
    printf("%lld\n", f[1][n]);
    print_ans(1, n);
    putchar('\n');
    return 0;
}
```
{% endfolding %}

#### [NOIP2006 提高组] 能量项链

[Portal](https://www.luogu.com.cn/problem/P1063).

环上问题肯定要复制成二倍链，而且为了方便计算我们最好现予处理出能量珠的头尾标记。转移时 $f[i,j]=\max\{f[i,k]+f[k+1,j]+head[i]\times head[k+1]\times tail[j]\}$。相乘的三个数分别是左区间对应的头标记，右区间对应的头标记和右区间对应的尾标记。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
int a[205], head[205], tail[205];
int f[205][205];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", a + i);
        head[i] = head[i + n] = a[i];
        tail[i - 1] = tail[i + n - 1] = a[i];
    }
    tail[n << 1] = a[1];
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= (n << 1) - len + 1; ++i)
        {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                f[i][j] = max(f[i][j], f[i][k] + f[k + 1][j] + head[i] * head[k + 1] * tail[j]);
        }
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = max(ans, f[i][i + n - 1]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1220] 关路灯

[Portal](https://www.luogu.com.cn/problem/P1220).

设 $f(i,j)$ 表示关掉 $[i,j]$ 的最小功率？不行，因为我们还需要知道张大爷（~~不是笔者~~）的位置，到底是在 $i$ 还是在 $j$。所以 $f(i,j,0)$ 表示张大爷在 $i$，$f(i,j,1)$ 表示张大爷在 $j$。转移对读到这的读者来说不是困难。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

using namespace std;

int n, c;
int p[155], w[155];
int sum[155];
int f[155][155][2];

int calc(int x, int y) { // 除开区间 [x, y] 的和
    return sum[x - 1] + sum[n] - sum[y];
}

int main(void)
{
    memset(f, 0x3f, sizeof(f));
    scanf("%d%d", &n, &c);
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d", p + i, w + i);
        sum[i] = sum[i - 1] + w[i];
    }
    f[c][c][0] = f[c][c][1] = 0;
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            f[i][j][0] = min(f[i + 1][j][0] + (p[i + 1] - p[i]) * calc(i + 1, j), f[i + 1][j][1] + (p[j] - p[i]) * calc(i + 1, j));
            f[i][j][1] = min(f[i][j - 1][0] + (p[j] - p[i]) * calc(i, j - 1), f[i][j - 1][1] + (p[j] - p[j - 1]) * calc(i, j - 1));
        }
    printf("%d\n", min(f[1][n][0], f[1][n][1]));
    return 0;
}
```
{% endfolding %}

#### [USACO07OPEN] Cheapest Palindrome G

[Portal](https://www.luogu.com.cn/problem/P2890).

设 $f(i,j)$ 代表完成 $[i,j]$ 的最小代价，然后分类转移。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, m;
char s[2005];
int f[2005][2005];
int a[30], b[30];

int main(void)
{
    cin >> n >> m;
    scanf("%s", s + 1);
    for (int i = 1; i <= n; ++i) {
        char x; int y, z;
        cin >> x >> y >> z;
        a[x - 'a'] = y, b[x - 'a'] = z;
    }

    memset(f, 0x3f, sizeof(f));
    for (int i = 1; i <= m; ++i) 
        f[i][i] = 0, f[i][i - 1] = 0;
    f[m + 1][m] = 0;
    for (int len = 2; len <= m; ++len)
        for (int i = 1; i <= m - len + 1; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j]) f[i][j] = min(f[i][j], f[i + 1][j - 1]);
            f[i][j] = min(f[i][j], f[i][j - 1] + min(b[s[j] - 'a'], a[s[j] - 'a']));
            f[i][j] = min(f[i][j], f[i + 1][j] + min(b[s[i] - 'a'], a[s[i] - 'a']));
        }
    printf("%d\n", f[1][m]);
    return 0;
}
```
{% endfolding %}

#### \[HNOI2010\] 合唱队

[Portal](https://www.luogu.com.cn/problem/P3205).

设 $f(i,j)$ 表示形成理想队列中区间 $[i,j]$ 的方案数？但是这样我们不知道上一个人是谁，那么就设 $f(i,j,0)$ 代表上一个人从左边加，$f(i,j,1)$ 代表上一个人从右边加，初始条件仅设其中一个为 $1$ 即可（否则会算重）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define MOD 19650827

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x << 3) + (x << 1) + (c ^ 48), c = getchar();
    return x;
}

int n;
int a[1005];
int f[1005][1005][2];

int main(void)
{
    n = read();
    for (int i = 1; i <= n; ++i) {
        a[i] = read();
        f[i][i][0] = 1;
    }
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            if (a[i] < a[i + 1]) f[i][j][0] += f[i + 1][j][0];
            if (a[i] < a[j]) f[i][j][0] += f[i + 1][j][1];
            f[i][j][0] %= MOD;

            if (a[j] > a[i]) f[i][j][1] += f[i][j - 1][0];
            if (a[j] > a[j - 1]) f[i][j][1] += f[i][j - 1][1];
            f[i][j][1] %= MOD;
        }
    cout << (f[1][n][0] + f[1][n][1]) % MOD << endl;
    return 0;
}
```
{% endfolding %}

#### [UVA1336] Fixing the Great Wall

[Portal](https://www.luogu.com.cn/problem/UVA1336).

和“关路灯”很相似。这种不知到当前的时间，而在转移时将“未来增加的费用”计算好，然后“时间归零”的技巧很有用。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

int n, x;
double v;
double f[1005][1005][2]; // k = 0 在 i, k = 1 在 j 
int s[1005];

struct mouse {
	int x, c, d;
	mouse(int x = 0, int c = 0, int d = 0) :
		x(x), c(c), d(d) {}
	bool operator < (const mouse &a) const {
		return x < a.x;
	}
} a[1005];

int g(int x, int y) {
	return s[n] - (s[y] - s[x - 1]);
}

int main(void) 
{
	// 计算未发生，但是肯定会发生的代价，然后“时钟归零” 
	while (scanf("%d%lf%d", &n, &v, &x) == 3 && n) {
		for (int i = 1; i <= n; ++i)
			scanf("%d%d%d", &a[i].x, &a[i].c, &a[i].d);
		a[++n] = mouse(x);
		sort(a + 1, a + n + 1);
		int t, sum = 0;
		for (int i = 1; i <= n; ++i) {
			if (a[i].x == x) t = i;
			s[i] = s[i - 1] + a[i].d;
			sum += a[i].c;
		}
		for (int i = 1; i <= n; ++i)
			for (int j = i; j <= n; ++j)
				f[i][j][0] = f[i][j][1] = 1e9;
		f[t][t][0] = f[t][t][1] = 0;
		for (int len = 2; len <= n; ++len)
			for (int i = 1; i <= n - len + 1; ++i) {
				int j = i + len - 1;
				f[i][j][0] = min(f[i + 1][j][0] + (a[i + 1].x - a[i].x) / v * g(i + 1, j), 
								 f[i + 1][j][1] + (a[j].x - a[i].x) / v * g(i + 1, j));
				f[i][j][1] = min(f[i][j - 1][0] + (a[j].x - a[i].x) / v * g(i, j - 1), 
								 f[i][j - 1][1] + (a[j].x - a[j - 1].x) / v * g(i, j - 1));
			}
		printf("%d\n", int(sum + min(f[1][n][0], f[1][n][1])));
	}
	return 0;	
}
```
{% endfolding %}

#### [USACO19DEC] Greedy Pie Eaters P

[Portal](https://www.luogu.com.cn/problem/P5851).

考虑计算 $f(i,j)$ 为吃掉 $[i,j]$ 的派所能得到的最大体重。那么有 $f(i,j)=\max\{f(i,k-1)+f(k+1,j)+p(i,j,k)\}$，其中 $p(i,j,k)$ 代表区间 $[i,j]$ 中 $k$ 未被吃掉，现在吃了获得的最大体重，可以通过区间 DP 预处理得到。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, m;
int w[90005], l[50005], r[50005];
int f[305][305], g[305][305][305];

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; ++i) {
        int w, l, r;
        scanf("%d%d%d", &w, &l, &r);
        for (int j = l; j <= r; ++j)
            g[l][r][j] = w;
    }
    for (int len = 1; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            for (int k = i; k <= j; ++k)
                g[i][j][k] = max({g[i][j][k], g[i + 1][j][k], g[i][j - 1][k]});
        }
    for (int len = 1; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                f[i][j] = max(f[i][j], f[i][k] + f[k + 1][j]);
            for (int k = i; k <= j; ++k)
                f[i][j] = max(f[i][j], f[i][k - 1] + f[k + 1][j] + g[i][j][k]);
        }
    printf("%d\n", f[1][n]);
    return 0;
}
```
{% endfolding %}

#### [CSP-S 2021] 括号序列

[Portal](https://www.luogu.com.cn/problem/P7914).

大型分类讨论，注意不要算重，有许多种方法可以解决这个问题：细分状态或者对特别处理算重的情况。对于读到这里的读者来说应该能完成这道题。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;
const int MOD = 1000000007;

int n, k;
char s[505];
i64 f[505][505][6];
// f[0]: **
// f[1]: (..)
// f[2]: (..)**
// f[3]: (..)..(..) 包含 1
// f[4]: **(..) 
// f[5]: **(..)** 包含 0 

int main(void)
{
	scanf("%d%d%s", &n, &k, s + 1);
	for (int i = 1; i <= n; ++i) f[i][i-1][0] = 1;
	for (int len = 1; len <= n; ++len)
		for (int i = 1; i <= n - len + 1; ++i) {
			int j = i + len - 1;
			if (len <= k) f[i][j][0] = f[i][j-1][0] && (s[j] == '*' || s[j] == '?');
			if (len >= 2) {
				if ((s[i] == '(' || s[i] == '?') && (s[j] == ')' || s[j] == '?'))
					f[i][j][1] = (f[i+1][j-1][0] + f[i+1][j-1][2] + f[i+1][j-1][3] + f[i+1][j-1][4]) % MOD;
				for (int k = i; k < j; ++k) {
					f[i][j][2] = (f[i][j][2] + f[i][k][3] * f[k+1][j][0]) % MOD;
					f[i][j][3] = (f[i][j][3] + (f[i][k][2]+f[i][k][3]) * f[k+1][j][1]) % MOD;
					f[i][j][4] = (f[i][j][4] + (f[i][k][4]+f[i][k][5]) * f[k+1][j][1]) % MOD;
					f[i][j][5] = (f[i][j][5] + f[i][k][4] * f[k+1][j][0]) % MOD;
				}
			}
			f[i][j][5] = (f[i][j][5] + f[i][j][0]) % MOD;
			f[i][j][3] = (f[i][j][3] + f[i][j][1]) % MOD;
		}
	printf("%lld\n", f[1][n][3]);
	return 0;
}
```
{% endfolding %}

### 高维 DP

### 线性 DP 的综合应用

这里的题难度会稍大。

#### [UVA10559] Blocks

[Portal](https://www.luogu.com.cn/problem/UVA10559).

预处理颜色段，然后定义 $f(l,r,k)$ 代表消除 $[l,r]$，且 $r$ 右边还有 $k$ 个与 $r$ 材质相同的木块。那么要么是消 $r$，要么是在 $[l,r-1)$ 中有与 $r$ 相同的颜色，消完中间段之后将两段颜色合并再进行消除。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, a[205];
int tot, len[205], color[205];
int f[205][205][205];

int dfs(int l, int r, int k)
{
    if (f[l][r][k]) return f[l][r][k];
    if (l == r) return f[l][r][k] = (len[r] + k) * (len[r] + k); // 只有一段，只能消除
    f[l][r][k] = dfs(l, r - 1, 0) + (len[r] + k) * (len[r] + k); // 先消除 [l, r - 1]
    for (int i = l; i < r - 1; ++i) // 在 [l, r - 2] 中寻找与 r 相同的颜色
        // 先消除 [i + 1, r - 1]
        // 然后 [l, i] 就会与 [r, r + len[r] + k] 合并，可以直接消除
        if (color[i] == color[r]) f[l][r][k] = max(f[l][r][k], dfs(l, i, len[r] + k) + dfs(i + 1, r - 1, 0));
    return f[l][r][k];
}

int main(void)
{
    int T, kase = 0;
    scanf("%d", &T);
    while (T--) {
        memset(f, 0, sizeof(f));
        tot = 0;
        scanf("%d", &n);
        for (int i = 1; i <= n; ++i) {
            scanf("%d", a + i);
            if (a[i] == a[i - 1]) ++len[tot];
            else ++tot, len[tot] = 1, color[tot] = a[i];
        }
        n = tot;
        printf("Case %d: %d\n", ++kase, dfs(1, n, 0));
    }
    return 0;
}
```
{% endfolding %}

## 小结

区间 DP 和高维 DP 是线性结构上比较难的一类 DP，但只要多写题，也不是什么难事。
