---
title: 组合计数基础
plugins:
  - katex
headimg: 'https://z3.ax1x.com/2021/11/07/I1PjMt.jpg'
cover: false
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 组合数学
    - 数学
group: notes
hide: true
status: working
abbrlink: 6513c499
date: 2022-08-29 18:41:00
---

> 组合计数是组合数学的基础，研究某组离散对象满足一定条件的安排的存在性、构造及计数等问题。看似名字人畜无害，实则“算死人，不偿命”。本文将引导你学习简单的组合计数，为接下来学习毒瘤的计数问题作准备。

<!--more-->

## 基本概念

组合计数中有一些基本概念，在这里简略地进行介绍。

### 加法原理

比如 james1 要吃东西，他可以吃傻瓜果和笨蛋果两种果子{% emoji 滑稽 %}，市面上在售的傻瓜果有 $3$ 种，笨蛋果有 $4$ 种，如果 james1 只吃一颗果子就能吃饱，那么他就有 $3+4=7$ 种选择去吃饱。   
形式化地，完成一个 Project 可以有 $n$ 类**办法**，$a_i$ 代表第 $i$ 类方法的数目。那么完成这个 Project 共有 $S=a_1+a_2+\cdots+a_n=\sum\limits_{i=1}^{n}a_i$ 种不同的方法。

### 乘法原理

比如 james1 要变傻（雾），要吃一种傻瓜果和一种笨蛋果，市面上在售的傻瓜果有 $3$ 种，笨蛋果有 $4$ 种，那么 james1 变傻的方式就有 $3 \times 4 = 12$。   
形式化地，完成一个 Project 可以有 $n$ 个**步骤**，$a_i$ 代表完成第 $i$ 步的方法数目。那么完成这个 Project 共有 $S=a_1 \times a_2\times\cdots\times a_n=\prod\limits_{i=1}^{n}a_i$ 种不同的方法。

### 抽屉原理（鸽笼原理）

抽屉原理分为两种情况：

#### 简单形式

james1 要将 $n+1$ 个笨蛋果放到 $n$ 个鸽笼中，那么可以得出至少有一个鸽笼中有两个（或以上）笨蛋果。

证明：

{% noteblock %}
采用反证法。   
假如每个分组有至多 $1$ 个物体，那么最多有 $n$ 个物体，而却有 $n+1$ 个物体，矛盾。
{% endnoteblock %}

证毕。

#### 推广

james1 要将 $n$ 个傻瓜果放到 $k$ 个鸽笼中，那么可以得出至少有一个鸽笼中有大于或等于 $\lceil \cfrac{n}{k} \rceil$ 个笨蛋果。

证明同样采用反证法，留给读者撕烤{% emoji 滑稽 %}。

### 排列数

从 $n$ 个不同元素中，任取 $m$（$m\leqslant n$）个元素**按照一定的顺序排成一列**，方案个数记作 $A_{n}^{m}$（推荐）或 $P_{n}^{m}$。

显然，第一个数有 $n$ 种取法。   
第二个数有 $n-1$ 种。   
$\dots$   
第 $m$ 个数有 $n-m+1$ 种取法。

综上所述，有：

$$
A_{n}^{m}=n(n-1)(n-2)\cdots(n-m+1)=\cfrac{n!}{(n-m)!}
$$

### 组合数

从 $n$ 个不同元素中，任取 $m$（$m\leqslant n$）个元素按照任意的顺序**组成一个集合**，方案个数记作 $C_{n}^{m}$。

这 $m$ 个人是没有顺序的，所以 $A_{m}^{m}$ 个方案是同一个方案，所以总方案数为：

$$
C_{n}^{m}=\cfrac{A_{n}^{m}}{A_{m}^{m}}=\cfrac{n!}{(n-m)!} \div m! = \cfrac{n!}{(n-m)!m!}\\
C_{n}^{m}=\cfrac{A_{n}^{m}}{A_{m}^{m}}=\cfrac{n(n-1)(n-2)\cdots(n-m+1)}{m!}=\cfrac{\prod_{i=n-m+1}^n i}{m!}
$$

实际上，组合数也通常用 $\dbinom{n}{m}$ 表示，相当于 $C_{n}^{m}$，这也被称之为**二项式系数**。

组合数有以下性质：

1. $C_{n}^{m} = C_{n}^{n-m}$；   
2. $C_{n}^{0}+C_{n}^{1}+C_{n}^{2}+\cdots+C_{n}^{n}=\sum\limits_{i=0}^{n}C_{n}^{i}=2^n$；
3. $\dbinom{n}{k}=\cfrac{n-k+1}{k}\dbinom{n}{k-1}$。

证明：   
{% noteblock %}
1. 从 $n$ 个数中选 $m$ 作为子集，剩下的 $n-m$ 个数也对应一个集合（这一点可以用公式证明，留给读者撕烤）。   
2. 从 $n$ 个不同元素取出若干个元素组成一个集合，当然可以取 $0\sim n$ 个数，即 $\sum\limits_{i=0}^{n}C_{n}^{i}$，每个数又有取和不取两种可能，所以它等于 $2^n$。
3. 由组合数的公式可以推导出，它经常被用来递推求单个组合数。大概长这样：

```cpp
int C(int n, int m) {
	int res = 1;
	for (int i = 1; i <= m; ++i)
		res = 1ll * res * (n - i + 1) / i;
	return res;
}
```
{% endnoteblock %}   

然后是一些不是那么好想，但是在关键时刻非常有用的内容，推荐背诵：

1. $\dbinom{n}{m}=\cfrac{n}{m}\dbinom{n-1}{m-1}$；
2. $\sum\limits_{i=0}^m\dbinom{n}{i}\dbinom{m}{m-i}=\dbinom{m+n}{m}(n\ge m)$，当 $n=m$ 时有 $\sum\limits_{i=0}^{n}{\dbinom{n}{i}}^2=\dbinom{2n}{n}$；
3. $\sum\limits_{i=0}^{n}\dbinom{i}{k}=\dbinom{n+1}{k+1}$；
4. $\dbinom{n}{r}\dbinom{r}{k}=\dbinom{n}{k}\dbinom{n-k}{r-k}$，相当于是 $n$ 只喵喵中选 $r$ 个队长，再在队长中选择 $k$ 个大队长，等价于 $n$ 只喵喵先选 $k$ 个大队长再选 $r-k$ 个队长，因为大队长也是队长。

### 杨辉三角

杨辉三角长这样（所有的空格值都为 $0$）：

|**杨辉三角**|第 $0$ 列|第 $1$ 列|第 $2$ 列|第 $3$ 列|第 $4$ 列|
|:--:|:--:|:--:|:--:|:--:|:--:|
|第 $0$ 行|$1$|||||
|第 $1$ 行|$1$|$1$||||
|第 $2$ 行|$1$|$2$|$1$|||
|第 $3$ 行|$1$|$3$|$3$|$1$||
|第 $4$ 行|$1$|$4$|$6$|$4$|$1$|

可以观察出 $C_{n}^{m}=C_{n-1}^{m}+C_{n-1}^{m-1}$。

证明：

{% noteblock %}
想选出 $m$ 个数，要么选第 $n$ 个数，要么不选，分别对应 $C_{n-1}^{m}$ 和 $C_{n-1}^{m-1}$。
{% endnoteblock %}

所以组合数的杨辉三角递推代码如下：

```cpp
C[0][0] = 1;
for (int i = 1; i <= n; i++) {
    C[i][0] = 1;
    for (int j = 1; j <= i; j++)
        C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
}
```

### 二项式定理

高中学过二项式定理：

$$
(a+b)^n = \sum_{i=1}^{n}\binom{n}{i}a^{n-i}b^i
$$

这个式子非常有用，一定要熟记。另外可以发现，多项式的系数就是杨辉三角。

### 排列组合方法

我们来看三个问题，来引出排列组合的经典计算方法。

#### 隔板法

> $n$ 只兔子参观大连市第二十四中学，其中 $m$ 只兔子关系特别好，它们一定要站在一块。那么有多少种排列方法？

我们把这 $m$ 只兔子看作一只大兔子，那么总共就有 $n-m+1$ 只兔子，排列方案数是 $(n-m+1)!$，然而大兔子里面也有 $m!$ 中方法，那么总方法数就是 $(n-m+1)!m!$。这就是**捆绑法**。

#### 插空法

> $n$ 只兔子参观大连市第二十四中学，其中 $m$ 只兔子有着不共戴天之仇，它们一定要不能站在一块。那么有多少种排列方法？

我们先把 $n-m$ 只兔子给排列好，有 $(n-m)!$ 种方法。这些兔子之间有 $(n-m+1)$ 个空（算最左和最右），再把这些不共戴天的兔子放到这些空里，有 $A_{n-m+1}^{m}$ 个方法。总方案数就是 $(n-m)!\times A_{n-m+1}^{m}$。这就是**插空法**。

#### 插板法

> `james1` 要将 $n$ 个相同的胡萝卜分给 $m$ 只兔子，他秉持雨露均沾的原则，每只兔子至少分到 $1$ 根胡萝卜，有多少种方案？

我们先介绍**隔板法（插板法）**，是指在 $n$ 个元素的 $n-1$ 个空中插入 $k$ 个板，可以把 $n$ 个元素分为 $k+1$ 组。

我们把这 $n$ 个胡萝卜排成 $1$ 行，当中就有 $n-1$ 个空。现在往里面插入 $m-1$ 个板，就可以将胡萝卜分为 $m$ 组，正好可以分给 $m$ 只兔子，而且由于不存在在同一个地方插两个板的情况，所以正好每一只兔子都能至少分到 $1$ 根胡萝卜。那么答案就是 $\dbinom{n-1}{m-1}$。

实际上这个问题相当于求不定方程 $x_1+x_2+\cdots+x_m=n$ 的正整数解的数量。

> 如果他是个大魔王（不可能，绝对不可能{% emoji 滑稽 %}），有的兔子可能 $1$ 根胡萝卜都得不到，那么有多少种方案？

同样的方法，如果允许有兔子分到 $0$ 根胡萝卜，我们只需要再加上 $m$ 根胡萝卜，就相当于刚才的问题了。答案是 $\dbinom{n+m-1}{m-1}=\dbinom{n+m-1}{n}$。

这个问题本质上是要求 $x_1+x_2+\cdots+x_m=n$ 的自然数解的数量。

> 如果 `james1` 偏爱一些兔子，要求第 $i$ 个兔子至少分到 $e_i$ 个胡萝卜，那么有多少种分法呢？

类比上一个问题，我们再加上 $\sum e$ 个胡萝卜，答案就是 $\dbinom{n+\sum e - 1}{n}$。

> 在 $n$ 个数中选 $m$ 个组合，要求任意两个数都不相邻，那么方案数有多少？

$\dbinom{n-m+1}{n}$，因为我们需要插入 $m-1$ 个空。

### 简单问题

真的只是加法原理和乘法原理而已。

#### [Luogu P1866] 编号

[Portal](https://www.luogu.com.cn/problem/P1866).

为了尽可能防止负数需要先排序，然后使用乘法原理计算。但如果负数真的出现了，就无解。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

#define i64 long long

using namespace std;
const i64 MOD = 1000000007;

int n;
int a[55];
i64 ans = 1;

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", a + i);
    sort(a + 1, a + n + 1);
    for (int i = 1, k; i <= n; i++) {
        if ((k = a[i] - (i - 1)) > 0) ans = ans * k % MOD;
        else return puts("0"), 0;
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [NOIP2016 提高组] 组合数问题

[Portal](https://www.luogu.com.cn/problem/P2822).

我们直接利用杨辉三角预处理组合数，然后使用二维前缀和计算答案即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int C[2005][2005];
int ans[2005][2005];

int main(void) {
    int T, k;
    scanf("%d%d", &T, &k);

    for (int i = 0; i <= 2002; ++i) {
        C[i][0] = 1, C[i][i] = 1;
        for (int j = 1; j <= i; ++j)
            C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % k;
    }
    for (int i = 0; i <= 2002; ++i) {
        for (int j = 1; j <= i; ++j)
            ans[i][j] = ans[i - 1][j] + ans[i][j - 1] - ans[i - 1][j - 1] + (C[i][j] == 0);
        ans[i][i + 1] = ans[i][i];
    }

    while (T--) {
        int n, m;
        scanf("%d%d", &n, &m);
        printf("%d\n", ans[n][min(n, m)]);
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P1287] 盒子与球

[Portal](https://www.luogu.com.cn/problem/P1287).

设 $f(i,j)$ 代表考虑前 $i$ 个球，有 $j$ 个盒子的方案数。显然可以是由前 $i-1$ 个球，$j$ 个盒子放在这 $j$ 个盒子中的任意一个，也可以只考虑 $j-1$ 个盒子，只能放在第 $j$ 个盒子，但是这第 $j$ 个盒子的位置可以任意摆放。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, r;
int f[15][15];

int main(void)
{
    scanf("%d%d", &n, &r);
    f[0][0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= min(i, r); ++j)
            f[i][j] = j * (f[i - 1][j] + f[i - 1][j - 1]);
    printf("%d\n", f[n][r]);
    return 0;
}
```
{% endfolding %}

那么盒子相同呢？这样可以乘 $j$ 的就只有 $f(i-1,j)$ 了，可以做一下 [[Luogu P1655] 小朋友的球](https://www.luogu.com.cn/problem/P1655)。

#### [HNOI2008] 越狱

[Portal](https://www.luogu.com.cn/problem/P3197).

考虑计算不会越狱，显然是 $m\times (m-1)^{n-1}$，然后直接计算。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <algorithm>

#define i64 long long
#define mod 100003

using namespace std;

i64 poww(i64 a, i64 b)
{
    if (b == 0) return 1;
    if (b == 1) return a % mod;
    i64 x = poww(a, b / 2);
    if (b % 2 == 0) return x % mod * x % mod;
    return x % mod * x % mod * a % mod;
}

int main()
{
    i64 m, n;
    cin >> m >> n;
    cout << poww(m, n) - m * poww(m - 1, n - 1);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1246] 编码

[Portal](https://www.luogu.com.cn/problem/P1246).

利用组合数，计算每一位即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int C[30][30];

void init(void)
{
    C[0][0] = 1;
    for (int i = 1; i <= 26; ++i)
    {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j)
            C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
    }
}

int n;
string s;

int main(void)
{
    init();
    cin >> s; n = s.length();
    for (int i = 1; i < n; ++i)
        if (s[i] <= s[i - 1])
        {
            puts("0");
            return 0;
        }
    int ans = 0;
    for (int i = 1; i < n; ++i) ans += C[26][i];
    for (int i = 0; i < n; ++i)
        for (char j = (i == 0 ? 'a' : s[i - 1] + 1); j < s[i]; ++j)
            ans += C['z' - j][n - i - 1];
    cout << ans + 1 << '\n';
    return 0;
}
```
{% endfolding %}

#### [NOIP2011 提高组] 计算系数

[Portal](https://www.luogu.com.cn/problem/P1313).

直接根据二项式定理，配合逆元等数学工具就可以方便计算。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;
const int MOD = 10007;

i64 poww(int a, int b)
{
    a %= MOD;
    i64 res = 1;
    while (b)
    {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

i64 C(int n, int m)
{
    static i64 c[1005];
    c[0] = 1;
    for (int i = 1; i <= m; ++i)
        c[i] = c[i - 1] * (n - i + 1) % MOD * poww(i, MOD - 2) % MOD;
    return c[m];
}

int main(void)
{
    int a, b, k, n, m;
    scanf("%d%d%d%d%d", &a, &b, &k, &n, &m);
    printf("%lld\n", C(k, n) * poww(a, n) % MOD * poww(b, m) % MOD);
    return 0;
}
```
{% endfolding %}

#### [Luogu P2638] 安全系统

[Portal](https://www.luogu.com.cn/problem/P2638).

不难看出放 01 可以分开计算，最后使用乘法原理组合在一起。而任何一个都可以使用插板法的结论进行计算。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef unsigned long long u64;

int n, a, b;
u64 C[105][105];

int main(void)
{
    C[0][0] = 1;
    for (int i = 1; i <= 100; ++i)
    {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j)
            C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
    }
    scanf("%d%d%d", &n, &a, &b);
    u64 ans = 0;
    for (int i = 0; i <= a; ++i)
        for (int j = 0; j <= b; ++j)
            ans += C[n + i - 1][i] * C[n + j - 1][j];
    printf("%llu\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P8557] 炼金术

[Portal](https://www.luogu.com.cn/problem/P8557).

一种金属有 $2^k-1$ 种可能被熔炼出来，然后每一种金属的可能都要乘起来（乘法原理）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 998244353;

inline int poww(int a, int b)
{
    int res = 1;
    while (b) {
        if (b & 1) res = 1ll * res * a % MOD;
        a = 1ll * a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main(void)
{
    int n, k;
    cin >> n >> k;
    cout << poww(poww(2, k) - 1, n) << '\n';
    return 0;
}
```
{% endfolding %}

#### [UVA11609] Teams

[Portal](https://www.luogu.com.cn/problem/UVA11609).

队长有 $n$ 中可能，每一个人当队长剩下的人都有 $2^{n-1}$ 种可能。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 1000000007;

int poww(int a, int b)
{
	int res = 1;
	while (b)
	{
		if (b & 1) res = 1ll * res * a % MOD;
		a = 1ll * a * a % MOD;
		b >>= 1;
	}
	return res;
}

int main(void)
{
	int T, kase = 0, n;
	scanf("%d", &T);
	while (T--)
	{
		scanf("%d", &n);
		printf("Case #%d: %d\n", ++kase, 1ll * n * poww(2, n - 1) % MOD);
	}
	return 0;
}
```
{% endfolding %}

## 多重集相关问题

我们知道一个多重集 $S$ 可以不满足互异性。设 $S=\{n_1\times a_1\cdots n_k\times a_k\}$ 代表由 $n_i$ 个 $a_i$ 组成的多重集。

### 多重集的排列数 | 多重组合数

这两个是同一个概念，是要求多重集 $S$ 的全排列个数，我们要除掉重复的个数，那么 $S$ 的全排列个数等于：

$$
\binom{n}{n_1,\dots,n_k}=\frac{n!}{\prod_{i=1}^k n_i!}
$$

{% noteblock %}
[\[UVA11076\] Add Again](https://www.luogu.com.cn/problem/UVA11076).

给出 $n$ 个数字，求出他们排列后能形成的所有整数的和，整数不重复。
{% endnoteblock %}

数的个数显然就是多重组合数，然后有 $n$ 个位置，总共的序列个数有多重组合数 $s$ 个。设所有数的和为 $sum$，那么平均每一位的值就是 $sum\div n$，然后再乘上基数，全是 $1$ 即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
typedef unsigned long long u64;

int main(void)
{
    int n; u64 f[15] = {1};
    const u64 one[] = {0, 1, 11, 111, 1111, 11111, 111111, 1111111, 11111111, 111111111, 1111111111, 11111111111, 111111111111};
    for (int i = 1; i <= 12; ++i)
        f[i] = f[i - 1] * i;
    while (scanf("%d", &n) == 1 && n)
    {
        static int cnt[10];
        u64 sum = 0;
        memset(cnt, 0, sizeof(cnt));
        for (int i = 1, x; i <= n; ++i)
            scanf("%d", &x), ++cnt[x], sum += x;
        u64 s = f[n];
        for (int i = 0; i < 10; ++i) s /= f[cnt[i]];
        printf("%llu\n", sum * s * one[n] / n);
    }
    return 0;
}
```
{% endfolding %}

## 组合工具

计数问题有一些常见的工具可以辅助计算。

### 容斥原理

容斥原理大家一定不陌生，我们现在要给出 $n$ 个集合的情况。

$$
\left|\bigcup_{i=1}^{n}S_i\right|=\sum_{m=1}^n(-1)^{m-1}\sum_{a_i<a_{i+1} }\left|\bigcap_{i=1}^mS_{a_i}\right|
$$

很多时候容斥原理还是暴力算法的拍档，它们可以帮助我们骗到更多的分数。

### Lucas 定理

Lucas 定理是说，对于**质数** $p$，有：

$$
\binom{n}{m}\bmod p = \binom{\left\lfloor n/p \right\rfloor}{\left\lfloor m/p\right\rfloor}\cdot\binom{n\bmod p}{m\bmod p}\bmod p
$$

[模板](https://www.luogu.com.cn/problem/P3807)，代码如下：

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m, p;
int fac[100005];

void exgcd(int a, int b, int &x, int &y) {
    if (b == 0) return x = 1, y = 0, void();
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}

int inv(int t) {
    int x, y;
    exgcd(t, p, x, y);
    return (x % p + p) % p;
}

int C(int n, int m) {
    if (m > n) return 0;
    if (m == 0 || m == n) return 1;
    return 1ll * fac[n] * inv(fac[m]) % p * inv(fac[n - m]) % p;
}

int Lucas(int n, int m) {
    if (m == 0 || m == n) return 1;
    return 1ll * Lucas(n / p, m / p) * C(n % p, m % p) % p;
}

int main(void) {
    fac[0] = 1;
    int T; cin >> T;
    while (T--) {
        cin >> n >> m >> p;
        for (int i = 1; i <= p; ++i) fac[i] = 1ll * fac[i - 1] * i % p;
        cout << Lucas(n + m, m) << '\n';
    }
    return 0;
}
```

因此可以发现，Lucas 定理面对的模数不会很大，否则是无法计算的。

### N 项式定理

二项式定理也能扩展为 $n$ 项式定理：

$$
(x_1+\cdots+x_t)^n=\sum_{n_i\ge 0,n_1+\cdots+n_t}\binom{n}{n_1,\cdots,n_t}x_{1}^{n_1}\cdots x_{t}^{n_t}
$$

其中 

$$
\binom{n}{n_1,\cdots,n_t}=\cfrac{n!}{n_1!\cdots n_t!}
$$

## 经典问题与方法

这里是一些经典问题的方法与解答。

### 错排问题

### 圆排列

## 经典计数

### 斐波那契数

### 卡特兰数

### 斯特林数

### 递推的相关问题

## Problemset

实际上高难度的计数问题会与多项式、生成函数等内容综合。所以这里的题目往往综合了其它算法，比如递推（计数 DP），概率等。

### 计数问题

借助动态规划算法的多阶段决策，可以高效的解决计数问题。

#### [HAOI2008] 硬币购物

[Portal](https://www.luogu.com.cn/problem/P1450).

{% noteblock %}
共有 $4$ 种硬币。面值分别为 $c_1,c_2,c_3,c_4$。

某人去商店买东西，去了 $n$ 次，对于每次购买，他带了 $d_i$ 枚 $i$ 种硬币，想购买 $s$ 的价值的东西。请问每次有多少种付款方法。
{% endnoteblock %}

如果每一种硬币都有无限多，那么这就成了个完全背包，直接做就可以，记为 $f(s)$。

接下来考虑性质。利用补集思想，我们可以用 $f(s)$ 减去至少有一个超过。也就是 $f(s-(d_1+1)\times c_1)$，以此类推。使用容斥原理计算即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;
const int M = 100000;

int c1, c2, c3, c4, q;
i64 f[M + 5];
inline i64 g(i64 x) { return x < 0 ? 0 : f[x]; }

int main(void) {
    cin >> c1 >> c2 >> c3 >> c4 >> q;
    f[0] = 1;
    for (int i = c1; i <= M; ++i) f[i] += f[i - c1];
    for (int i = c2; i <= M; ++i) f[i] += f[i - c2];
    for (int i = c3; i <= M; ++i) f[i] += f[i - c3];
    for (int i = c4; i <= M; ++i) f[i] += f[i - c4];
    while (q--) {
        int d1, d2, d3, d4, s;
        cin >> d1 >> d2 >> d3 >> d4 >> s;
        d1 = (d1 + 1) * c1, d2 = (d2 + 1) * c2, d3 = (d3 + 1) * c3, d4 = (d4 + 1) * c4;
        cout << g(s) - g(s - d1) - g(s - d2) - g(s - d3) - g(s - d4) +
        g(s - d1 - d2) + g(s - d1 - d3) + g(s - d1 - d4) + g(s - d2 - d3)
        + g(s - d2 - d4) + g(s - d3 - d4) - g(s - d1 - d2 - d3) - g(s - d1 - d3 - d4)
        - g(s - d1 - d2 - d4) - g(s - d2 - d3 - d4) + g(s - d1 - d2 - d3 - d4) << '\n'; 
    }
    return 0;
}
```
{% endfolding %}

### 数学推导

推式子（柿子）是一件有趣的事情！

#### [Codeforces 1545B] AquaMoon and Chess

只有 $0$ 和 $11$ 是有用的，答案是 $\dbinom{a+b}{a}$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 998244353;

int n;
char a[100005];

int poww(int a, int b) {
	int res = 1;
	for (; b; b >>= 1, a = 1ll * a * a % MOD)
		if (b & 1) res = 1ll * res * a % MOD;
	return res;
}

int C(int n, int m) {
	int res = 1;
	for (int i = 1; i <= m; ++i)
		res = 1ll * res * (n - i + 1) % MOD * poww(i, MOD - 2) % MOD;
	return res;
}

void solve(void)
{
	cin >> n >> a;
	int x = 0, y = 0;
	for (int i = 0; i < n; ++i)
		if (i < n - 1 && a[i] == '1' && a[i + 1] == '1') ++x, ++i;
		else if (a[i] == '0') ++y;
	cout << C(x + y, y) << '\n';
}

int main(void) 
{
	ios::sync_with_stdio(false);
	int T; cin >> T;
	while (T--) solve();
	return 0;
}
```
{% endfolding %}

#### [SCOI2010] 生成字符串

[Portal](https://www.luogu.com.cn/problem/P1641).

{% noteblock %}
把 $n$ 个 $1$ 和 $m$ 个 $0$ 组成字符串，在任意的前 $k$ 个字符中，$1$ 的个数不能少于 $0$ 的个数。满足要求的字符串共有多少个？
{% endnoteblock %}

考虑其几何意义：选 1 代表向右上走，选 0 代表向右下走，要走到 $(n+m,n-m)$，而且需要满足任意时刻的纵坐标是非负的。

怎么办呢？利用补集思想，总共有 $\dbinom{n+m}{m}$ 种走法，能够走到 $y=-1$，相当于从 $(0,-2)$ 开始走，$n'=n+1,m'=m-1$，走到 $(n+m,n-m)$，方案数为 $\dbinom{n+1+m-1}{m-1}$，因此答案为 $\dbinom{n+m}{m}-\dbinom{n+m}{m-1}$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 20100403;

int fac[2000005];
int poww(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = 1ll * res * a % MOD;
        a = 1ll * a * a % MOD;
        b >>= 1;
    }
    return res;
}
int inv(int a) {
    return poww(a, MOD - 2);
}
int C(int n, int m) {
    return 1ll * fac[n] * inv(fac[m]) % MOD * inv(fac[n - m]) % MOD;
}

int main(void) {
    fac[0] = 1;
    for (int i = 1; i <= 2000000; ++i) fac[i] = 1ll * fac[i - 1] * i % MOD;
    int n, m;
    scanf("%d%d", &n, &m);
    printf("%d\n", (C(n + m, m) - C(n + m, m - 1) + MOD) % MOD);
    return 0;
}
```
{% endfolding %}

### 数学综合

这里的问题会与数论结合在一起，综合性会比较强。

#### [SDOI2010] 古代猪文

[Portal](https://www.luogu.com.cn/problem/P2480).

翻译成人话，就是要求：

$$
g^{\sum_{k\mid n} \binom{n}{k}} \bmod 999911659
$$

使用费马小定理降幂可得：

$$
g^{\sum_{k\mid n} \binom{n}{k} \bmod 999911658} \bmod 999911659
$$

$999911658=2\times 3\times 4679 \times 35617$，这样小的质数模数已经可以使用 Lucas 定理，求四遍之后使用 CRT 合并出解即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 999911659;

int fac[40005] = {1};
int poww(int a, int b, int p) {
    int res = 1;
    while (b) {
        if (b & 1) res = 1ll * res * a % p;
        a = 1ll * a * a % p;
        b >>= 1;
    }
    return res % p;
}

void exgcd(int a, int b, int &x, int &y) {
    if (b == 0) return x = 1, y = 0, void();
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}

int inv(int t, int p) {
    int x, y; exgcd(t, p, x, y);
    return (x % p + p) % p;
}

int C(int n, int m, int p) {
    if (m > n) return 0;
    if (m == 0 || m == n) return 1;
    return 1ll * fac[n] * inv(fac[m], p) % p * inv(fac[n - m], p) % p;
}

int Lucas(int n, int m, int p) {
    if (m == 0 || m == n) return 1;
    return 1ll * Lucas(n / p, m / p, p) * C(n % p, m % p, p) % p;
}

int a[10], m[10] = {0, 2, 3, 4679, 35617}, M[10];
int CRT(void) {
    int ans = 0, x;
    for (int i = 1; i <= 4; ++i) {
        M[i] = (MOD - 1) / m[i];
        ans = (ans + 1ll * a[i] * M[i] % (MOD - 1) * inv(M[i], m[i])) % (MOD - 1);
    }
    return (ans % (MOD - 1) + (MOD - 1)) % (MOD - 1);
}

int n, g;
int calc(void) {
    for (int op = 1; op <= 4; ++op) {
        for (int i = 1; i <= 40000; ++i) fac[i] = 1ll * fac[i - 1] * i % m[op];
        for (int i = 1; i * i <= n; ++i)
            if (n % i == 0) {
                a[op] = (a[op] + Lucas(n, i, m[op])) % m[op];
                if (i * i != n) a[op] = (a[op] + Lucas(n, n / i, m[op])) % m[op];
            }
    }
    return CRT();
}

int main(void) {
    cin >> n >> g;
    int t = calc();
    if (g == MOD && t == 0) puts("0"); // 注意这里，因为费马小定理取模的原因，所以 t == 0 时其实 t == k(MOD - 1)，所以答案是 0 不是 1，扩展欧拉定理降幂的时候也有类似的问题
    else cout << poww(g, t, MOD) << '\n';
    return 0;
}
```
{% endfolding %}