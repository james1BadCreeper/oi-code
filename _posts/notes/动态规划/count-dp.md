---
title: 计数类 DP 与数位统计
cover: false
plugins:
  - katex
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 动态规划
    - 计数 DP
    - 数位 DP
    - 笔记
headimg: https://s1.ax1x.com/2022/08/04/vm9ulT.jpg
group: notes
hide: true
status: working
abbrlink: 3feab2ad
date: 2022-07-29 08:06:47
---

> 严格来说，计数类 DP 不是 DP。动态规划是一类最优化问题，而计数类问题统计的是方案数，但是计数时要求“不重不漏”，需要精确的划分和整体性的计算，因此使用动态规划的子结构与多阶段可以高效的求解计数类 DP，实现上这种方法只能叫做递推。

<!--more-->

其实在之前就已经见识过计数问题了，想一想，还记得吗？对，就是那些统计方案的题目。

## 计数类 DP

其实计数 DP 会与很多组合方法综合，我们会见到的。

### 引子

我们做几道简单题，

#### [HAOI2010] 最长公共子序列

[Portal](https://www.luogu.com.cn/problem/P2516).

求最长公共子序列已经很简单了，至于数量，就是要看它从哪里转移过来。设 $g(i,j)$ 代表 $A_1\dots A_i,B_1\dots B_j$ 的 LCS 个数，$f(i,j)$ 代表 $A_1\dots A_i,B_1\dots B_j$ 的 LCS 长度，那么：

- 如果 $g(i,j)$ 可以从 $g(i-1,j)$ 转移过来，那么需要加上它的方案数；
- 如果 $g(i,j)$ 可以从 $g(i,j-1)$ 转移过来，那么需要加上它的方案数；
- 如果 $g(i,j)$ 可以从 $g(i-1,j-1)$ 转移过来，那么需要加上它的方案数。

但是如果前两条同时生效，那么会出现一个问题：$f(i,j)=f(i-1,j)=f(i,j-1)$，如果此时 $f(i-1,j),f(i,j-1)$ 都可以从 $f(i-1,j-1)$ 转移，那么这里就重复了，需要减去 $g(i-1,j-1)$。这种情况是什么时候？就是当上述的第三种转移没有生效的时候。也就是 $f(i,j) = f(i-1,j-1)$，这样的话 $f(i-1,j-1)=f(i-1,j)=f(i,j-1)$，也就是说 $f(i-1,j),f(i,j-1)$ 都可以从 $f(i-1,j-1)$ 转移过来。

由于空间限制，所以要使用滚动数组。

{% folding cyan::查看代码 %}
```cpp
// 由于这份代码的常数较大，可能需要吸氧才能过
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

#define i64 long long

using namespace std;

const int MOD = 100000000;

char a[5005], b[5005];
int n, m, f[2][5005];
i64 g[2][5005];

int main(void)
{
    scanf("%s%s", a + 1, b + 1);
    n = strlen(a + 1) - 1, m = strlen(b + 1) - 1;
    for (int i = 0; i <= m; ++i)
        g[0][i] = 1;
    g[1][0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
        {
            f[i & 1][j] = max({f[i & 1][j - 1], f[(i - 1) & 1][j], f[(i - 1) & 1][j - 1] + (a[i] == b[j])});
            g[i & 1][j] = 0;
            if (f[i & 1][j] == f[(i - 1) & 1][j]) g[i & 1][j] += g[(i - 1) & 1][j];
            if (f[i & 1][j] == f[i & 1][j - 1]) g[i & 1][j] += g[i & 1][j - 1];
            if (f[i & 1][j] == f[(i - 1) & 1][j - 1] + 1 && a[i] == b[j]) g[i & 1][j] += g[(i - 1) & 1][j - 1];
            if (f[i & 1][j] == f[(i - 1) & 1][j - 1]) g[i & 1][j] -= g[(i - 1) & 1][j - 1];

            g[i & 1][j] %= MOD;
        }
    printf("%d\n%lld\n", f[n & 1][m], g[n & 1][m]);
    return 0;
}
```
{% endfolding %}

#### [Codeforces 149D] Coloring Brackets

[Portal](https://www.luogu.com.cn/problem/CF149D).

设 $f(i,j,p,q)$ 为考虑区间 $[i,j]$，$i$ 的颜色为 $p$，$j$ 的颜色为 $q$ 的方案数。转移也很好写，用 dfs 实现。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stack>

#define i64 long long

using namespace std;

const int MOD = 1000000007;

int n;
char s[705];
int match[705];
int f[705][705][3][3];

void dfs(int l, int r)
{
    #define rep(i) for (int i = 0; i <= 2; ++i)
    if (l + 1 == r)
    {
        f[l][r][0][1] = 1;
        f[l][r][0][2] = 1;
        f[l][r][1][0] = 1;
        f[l][r][2][0] = 1;
        return;
    }
    if (match[l] == r)
    {
        dfs(l + 1, r - 1);
        rep(i) rep(j)
        {
            if (i != 1) f[l][r][1][0] = (f[l][r][1][0] + f[l + 1][r - 1][i][j]) % MOD;
            if (i != 2) f[l][r][2][0] = (f[l][r][2][0] + f[l + 1][r - 1][i][j]) % MOD;
            if (j != 1) f[l][r][0][1] = (f[l][r][0][1] + f[l + 1][r - 1][i][j]) % MOD;
            if (j != 2) f[l][r][0][2] = (f[l][r][0][2] + f[l + 1][r - 1][i][j]) % MOD;
        }
        return;
    }
    dfs(l, match[l]), dfs(match[l] + 1, r);
    rep(i) rep(j) rep(p) rep(q)
    {
        if (j == p && j != 0) continue;
        f[l][r][i][q] = (f[l][r][i][q] + (i64)f[l][match[l]][i][j] * f[match[l] + 1][r][p][q]) % MOD;
    }
}

int main(void)
{
    scanf("%s", s + 1);
    n = strlen(s + 1);
    stack <int> st;
    for (int i = 1; i <= n; ++i)
    {
        if (s[i] == '(') st.push(i);
        else match[st.top()] = i, match[i] = st.top(), st.pop();
    }
    dfs(1, n);
    int ans = 0;
    for (int i = 0; i <= 2; ++i)
        for (int j = 0; j <= 2; ++j)
            ans = (ans + f[1][n][i][j]) % MOD;
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 简单组合

组合数，组合方法这些对于读者来说一定不陌生。它们是与多阶段计数的最佳拍档。

#### [AHOI2009] 中国象棋

[Portal](https://www.luogu.com.cn/problem/P2051).

显然每一行、每一列都最多只能有两个棋子。定义 $f(i,j,k)$ 为考虑前 $i$ 行，有 $j$ 列是一个棋子，$k$ 列是两个棋子，剩下的 $m-j-k$ 列没有棋子，显然计算时要满足 $j+k\le m$。

初始肯定 $f(0,0,0)=1$，接下来一行一行的考虑。

第 $i$ 行肯定可以什么都不放，所以我们用 $f(i-1,j,k)$ 初始化 $f(i,j,k)$。

然后考虑第 $i$ 行放什么。显然要么放 $1$ 个棋子，要么放 $2$ 个棋子。

当放一个棋子的时候，这个棋子可以放在没有棋子的列，也可以放在有 $1$ 个棋子的列。如果放在没有棋子的列，说明当前的 $f(i,j,k)$ 是从 $f(i-1,j-1,k)$ 过来的（多了一个有 $1$ 个棋子的列），放的时候有 $m-(j-1)-k$ 个空列，随便选一个即可。如果放在有一个棋子的列，说明当前的 $f(i,j,k)$ 是从 $f(i-1,j+1,k-1)$ 过来的（有一个棋子的少了一个，两个棋子的多了一个），选择时在 $j+1$ 个有一个棋子的列随便选一个即可。

当放两个棋子的时候，这两个棋子可以都放在没有棋子的列，这样它从 $f(i-1,j-2,k)$ 过来的，放的时候的在 $m-(j-2)-k$ 个空列中组合，共有 $C_{m-(j-2)-k}^2$ 中可能；也可以都放在有一个棋子的列，从 $f(i-1,j+2,k-2)$ 过来，有 $C_{m-(j+2)-k}^2$；还可以一个放在没有棋子的列，一个放在有一个棋子的列，从 $f(i-1,j,k-1)$ 过来（一个放在没有棋子的列使得 $j+1$，一个放在有一个棋子的列使得 $j-1,k+1$，综合起来就是 $j$ 不变，$k+1$），选择时放在没有棋子的列的选择有 $m-j-(k-1)$ 种，放在有一个棋子的列的选择有 $j$ 种。

可以使用滚动数组优化空间。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

const int MOD = 9999973;

int n, m;
i64 f[2][105][105]; // 考虑前 i 行，有 j 列是一个棋子，k 列是两个棋子

int C(int n) {
    return (n * (n - 1)) / 2;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    f[0][0][0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = 0; j <= m; ++j)
            for (int k = 0; j + k <= m; ++k)
            {
                f[i & 1][j][k] = f[(i - 1) & 1][j][k];
                if (/*m - j + 1 - k >= 0 && */j >= 1) f[i & 1][j][k] += f[(i - 1) & 1][j - 1][k] * (m - j + 1 - k); // 放一个棋子，放在没有棋子的列
                if (k >= 1) f[i & 1][j][k] += (f[(i - 1) & 1][j + 1][k - 1]) * (j + 1); // 放一个棋子在有一个棋子的列
                if (/*m - j + 2 - k >= 0 && */j >= 2) f[i & 1][j][k] += f[(i - 1) & 1][j - 2][k] * C(m - j + 2 - k); // 放两个棋子在都没有棋子的列
                if (k >= 2) f[i & 1][j][k] += f[(i - 1) & 1][j + 2][k - 2] * C(j + 2); // 放两个棋子在都有一个棋子的列
                if (/*m - j - k + 1 >= 0 && */k >= 1) f[i & 1][j][k] += f[(i - 1) & 1][j][k - 1] * j * (m - j - k + 1); // 放两个棋子，一个在有一个棋子的列，一个在没有棋子的列
                f[i & 1][j][k] %= MOD;
            }
    i64 ans = 0;
    for (int i = 0; i <= m; ++i)
        for (int j = 0; i + j <= m; ++j)
            ans = (ans + f[n & 1][i][j]) % MOD;
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Codeforces 425E] Sereja and Sets

[Portal](https://codeforces.com/problemset/problem/425/E).

设 $f(r,i)$ 为右端点为 $r$，时间为 $i$ 的方案数。转移采用刷表法。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;
const int MOD = 1000000007;

int n, k;
i64 f[505][505], poww[250005];

int main(void) {
    scanf("%d%d", &n, &k);
    f[0][0] = 1;
    poww[0] = 1;
    for (int i = 1; i <= n * n; ++i) poww[i] = poww[i - 1] * 2 % MOD;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j <= i; ++j)
            for (int k = i + 1; k <= n; ++k)
                f[k][j + 1] = (f[k][j + 1] + f[i][j] * (poww[k - i] - 1) % MOD * poww[(n - k) * (k - i)]) % MOD;
    i64 ans = 0;
    for (int i = 0; i <= n; ++i)
        ans += f[i][k];
    printf("%lld\n", ans % MOD);
    return 0;
}
```
{% endfolding %}

#### [NOIP2021] 数列

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/P7961).

给定整数 $n, m, k$，和一个长度为 $m + 1$ 的正整数数组 $v_0, v_1, \ldots, v_m$。

对于一个长度为 $n$，下标从 $1$ 开始且每个元素均不超过 $m$ 的非负整数序列 $\{a_i\}$，我们定义它的权值为 $v_{a_1} \times v_{a_2} \times \cdots \times v_{a_n}$。

当这样的序列 $\{a_i\}$ 满足整数 $S = 2^{a_1} + 2^{a_2} + \cdots + 2^{a_n}$ 的二进制表示中 $1$ 的个数不超过 $k$ 时，我们认为 $\{a_i\}$ 是一个合法序列。

计算所有合法序列 $\{a_i\}$ 的权值和对 $998244353$ 取模的结果。
{% endnoteblock %}

$S$ 在统计时是会进位的，因此我们设 $f(i,j,k,p)$ 代表考虑 $S$ 从低位开始的前 $i$ 位，考虑了 $j$ 个数（**任意**），有 $k$ 个 $1$（由于是加法，$1$ 只能变多不能变少），向下一位的进位为 $p$（状态中的 $S$ 按照二进制考虑，第 $i$ 位有 $p$ 个 $1$，很显然这样压根就没有考虑进位，进位是在转移的时候进行的）。显然初始时有 $f(0,0,0,0)=1$，接下来考虑如何转移。

显然刷表法会更容易一些。假设再给序列 $a$ 选择 $t$ 个元素 $i$，那么 $S$ 的第 $i$ 位就会加上 $t$ 个 $1$，总共就有了 $t+p$ 个 $1$。同时我们要将第 $i$ 位进行“展平”，也就是向前进位。如果 $t+p$ 是奇数，那么 $1$ 的个数就多了 $1$，否则没变；向前进位的个数为 $\left\lfloor\cfrac{t+p}{2}\right\rfloor$。也就是说，$f(i,j,k,p)$ 转移到了：

$$
f\left(i+1,j+t,k+(t+p)\bmod 2,\left\lfloor\frac{t+p}{2}\right\rfloor\right)
$$

贡献是多少？基数是 $f(i,j,k,p)$，这种转移共有 $\dbinom{n-j}{t}$ 个 $v_i^t$ 中方式（在还没有填的 $a$ 中选出 $t$ 个来填），单次的贡献是 $v_i^t$，所以贡献为 $v_i^t\times \dbinom{n-j}{t}$。

统计答案时判断一下 $1$ 的个数是否合法即可（要加上进位的）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 998244353;

int n, m, K;
int C[35][35], v[105][55];
int f[105][35][35][35];

int count(int x) {
	int res = 0;
	for (;x ; x >>= 1) res += (x & 1);
	return res;
}

int main(void) 
{
	scanf("%d%d%d", &n, &m, &K);
	C[0][0] = 1;
	for (int i = 1; i <= n; ++i) {
		C[i][0] = 1;
		for (int j = 1; j <= i; ++j)
			C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % MOD;
	}
	for (int i = 0; i <= m; ++i) {
		v[i][0] = 1;
		scanf("%d", &v[i][1]);
		for (int j = 2; j <= n; ++j) v[i][j] = 1ll * v[i][j - 1] * v[i][1] % MOD;
	}
	
	f[0][0][0][0] = 1;
    for (int i = 0; i <= m; ++i)
      for (int j = 0; j <= n; ++j)
	    for (int k = 0; k <= K; ++k)
          for (int p = 0; p <= n >> 1; ++p)
		    for (int t = 0; t <= n - j; ++t) {
		  	  int &dp = f[i + 1][j + t][k + ((t + p) & 1)][(t + p) >> 1];
		  	  dp = (dp + 1ll * f[i][j][k][p] * v[i][t] % MOD * C[n - j][t]) % MOD;
			}
	
	int ans = 0;
	for (int k = 0; k <= K; ++k)
		for (int p = 0; p <= n >> 1; ++p)
			if (k + count(p) <= K) ans = (ans + f[m + 1][n][k][p]) % MOD;
	printf("%d\n", ans);
	return 0;
}
```
{% endfolding %}

## 数位统计 DP

有一类题目的计数与数字相关，一般是给定一些限制条件，然后询问满足条件的第 $k$ 小数是多少，或者是有多少个满足条件的数。

### 方法

将数字按每一位拆开，比如最常用的十进制，拆完之后每一位数字都是 $0\sim 9$。问题具有以下特征：

- 计数，统计数的数量；
- 数的值域很大；
- 可以使用”数位“来理解……

比如说 `james1` 数数，数 $100\sim 199$ 和 $200\sim 299$ 几乎一致，$00\sim 99$ 这一部分是完全一样的。这样就可以根据此设计状态来求解问题。

很多时候为了计算方便我们都会先允许前导 $0$ 的存在，最后再减去。

### 例题

我们来看一些简单的题目来进一步认识数位 DP。

#### [ZJOI2010] 数字计数

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/P2602).

给定两个正整数 $a$ 和 $b$，求在 $[a,b]$ 中的所有整数中，每个数码各出现了多少次，$1\le a\le b\le 10^{12}$。
{% endnoteblock %}

设 $f(i)$ 代表满 $i$ 位数的每个数字的出现个数。前 $i-1$ 位的数字贡献是 $10f(i-1)$，第 $i$ 位的数字贡献是 $10^{i-1}$。

现在考虑如何统计答案。将上界从高到低位枚举，不贴着上界时后面可以随便取，贴着时只能取到上界。如果有前导零还需要减去。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

i64 l, r;
i64 f[15], poww[15];
i64 ans1[15], ans2[15];

void calc(i64 n, i64 *ans)
{
    static int a[15];
    int len = 0; i64 tmp = n;
    while (tmp) a[++len] = tmp % 10, tmp /= 10;
    for (int i = len; i >= 1; --i) {
        // 首先考虑后 i - 1 位的贡献
        for (int j = 0; j < 10; ++j) ans[j] += f[i - 1] * a[i]; // 满 i-1 位的数字，有 a[i] 个（不算以 0 打头，因为下一位的时候会考虑）
        // 再考虑第 i 位的贡献
        for (int j = 0; j < a[i]; ++j) ans[j] += poww[i - 1]; // 0 ~ a[i]-1 都会出现 10^(i-1) 次
        n -= poww[i - 1] * a[i]; // 减掉第 i 位后的数字
        ans[a[i]] += n + 1; // a[i] 会在 0~n 出现一次
        // 最后处理前导零
        ans[0] -= poww[i - 1]; // 当第 i 位为 0 时，答案就没有意义，此时剩下的可以随便填，都需要减去
    }
}

int main(void)
{
    cin >> l >> r;
    poww[0] = 1;
    for (int i = 1; i <= 13; ++i) {
        f[i] = f[i - 1] * 10 + poww[i - 1]; 
        poww[i] = poww[i - 1] * 10;
    }
    calc(r, ans1); calc(l - 1, ans2);
    for (int i = 0; i < 10; ++i)
        printf("%lld ", ans1[i] - ans2[i]);
    putchar('\n');
    return 0;
}
```
{% endfolding %}

## Problemset