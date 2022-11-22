---
title: 背包问题
plugins:
  - katex
cover: false
headimg: https://s1.ax1x.com/2022/08/04/vm9K6U.jpg
categories:
    - 算法竞赛
    - 学习笔记
    - 动态规划
tag:
    - 动态规划
    - 背包
    - 笔记
hide: true
group: notes
status: done
abbrlink: b2626942
date: 2022-07-25 21:27:23
---

> 背包是 DP 中一类重要而特殊的模型。本文将引导你学习各类背包。

<!--more-->

{% folding blue::更新日志 %}

{% timeline %}

{% timenode 2022/09~10 %}

补充了大量习题。

{% endtimenode %}

{% timenode 2022/08/26 %}

增加了一道习题。

{% endtimenode %}

{% timenode 2022/08/19 %}

完成了预定的所有内容。

{% endtimenode %}

{% endtimeline %}

{% endfolding %}

关于背包问题有一个特别著名的讲义：[崔添翼的背包九讲](https://github.com/tianyicui/pack)。想要学好更多复杂的动态规划问题，对背包问题的深入理解必不可少。你可以选择将背包九讲和本文搭配着阅读。

## 01 背包

这类问题的形式是：   

- 你有 $n$ 个物品，要把其中一些物品装进容量为 $V$ 的背包；
- 第 $i$ 个物品自己的体积是 $v_i$；
- 如果把第 $i$ 个物品放进背包，会获得 $w_i$ 的收益；
- 要求在不超过容量的情况下，最大化收益。

01 背包就是这个物品要么选，要么不选。

[这是模板](https://www.luogu.com.cn/problem/P1048)。

### 实现

这个问题初学者较容易设计出错误的状态，如下：

- $dp_n$ 表示前 $n$ 个物品取得的最大价值。然而前面的物品占用了空间，所以会影响我现在决定能不能放第 $n$ 个物品。有后效性。
- $dp_k$ 表示占用 $k$ 的空间取得的价值。然而每个物品只能用一次，决策的时候必须考虑哪些物品用了、哪些没用。有后效性。

那么我们就设计状态：$dp[k,m]$ 表示“只考虑前 $k$ 个物品，**恰好**用 $m$ 的容量能装下的最大价值”。这个状态没有后效性。

转移方程：

$$dp[k][m] = \max\begin{cases}
dp[k-1][m]
\\
dp[k-1][m-v_k]+w_k & ,m \geqslant v_k
\end{cases}$$

初始 $dp[0][0]=0$，**目标（注意由于状态的设计，必须要取最大值，我们定义的是”恰好“，不是”至多“）**：$\max\limits_{0 \le m \le V}{dp[n][m]}$。

之所以状态不定义为“至多用 $m$ 的容量”，是因为这样无法确定还剩多少空间（但实际上这么做是可行的，具体方法请读者自行思考）。

```cpp
memset(dp, 0xcf, sizeof(dp)); // -INF，因为要搞最大值
dp[0][0] = 0;
for (int i = 1; i <= n; ++i)
{
    for (int j = 0; j <= V; ++j)
    {
        dp[i][j] = dp[i-1][j];
        if (j >= v[i]) dp[i][j] = max(dp[i][j], dp[i-1][j-v[i]] + w[i]);
    }
}

int ans = -1;
for (int i = 0; i <= V; ++i)
    ans = max(ans, dp[n][i]);
```

注意到一个问题，$j$ 的循环次序是无关紧要的，这不会影响结果。

实际上，笔者目前没有找到任何能证明不用取最大值是错误的反例，也许这样的反例根本不存在，因为在更新时，由于体积更大，肯定能成功更新。实际上这个代码实现是将那些用不掉的体积填进去了空气，例如我们看这样一组数据：

```
n = 2, V = 4
物品 A: v = 3, w = 10
物品 B: v = 4, w = 1
```

它的 DP 数组长这样：

```
    j= 0 1 2 3  4
i = 1: 0 0 0 10 10
i = 2: 0 0 0 10 10
```

这个 `dp[1][4] = 10` 从哪来的？显然，它是从 `dp[1][1]` 更新来的，我们填进去了体积为 $1$ 的空气。

但考虑到实际中是不在意这点时间（取最大值才线性），且严格符合状态转移的过程，**强烈建议取最大值**。接下来的局部代码为了不影响阅读均不会取最大值，但是完整题目的代码均会取最大值。

---

### 滚动数组

更奇妙的是，还可以把 $dp$ 数组变成一维的。

```cpp
for (int i = 1; i <= n; ++i) 
{
    scanf("%d%d", &v, &w);
    for (int j = V; j >= v; --j)
        dp[j] = max(dp[j], dp[j-v] + w);
}
```

为什么这样是对的？在计算 $dp(i,j)$ 前，$dp[j]$ 里保存的是 $dp(i-1,j)$，而 $dp[j-v]$ 保存的是 $dp(i-1,j-v)$（注意 $j$ 的循环次序），然后 $dp[j]$ 可以顺利更新成 $dp(i,j)$ 的内容，覆盖掉的内容再也不会使用，倒序的循环顺序可以使得计算 $dp(i,j)$ 时 $dp(i,j-v)$ 还未被计算，保存的是 $dp(i-1,j-v)$。所以这样是可以的{% emoji 阴险 %}！

一个 DP 可以通过滚动数组来优化，当且仅当其状态图是分层的，下 $k$ 层的结果，由上 $d$ 层结果唯一确定。   
在 01 背包中，第 $i$ 层的结果由第 $i-1$ 层唯一确定，因此可以使用滚动数组。而且由于顺序的特殊性，所以只用了一维。

但是如果没有这种特殊性怎么办{% emoji tong Sad %}？很简单：

```cpp
int dp[2][MAXV];
for (int i = 1; i <= n; ++i)
{
    for (int j = 0; j <= V; ++j)
    {
        dp[i & 1][j] = dp[(i-1) & 1][j];
        if (j >= v[i]) dp[i & 1][j] = max(dp[i & 1][j], dp[(i-1) & 1][j-v[i]] + w[i]);
    }
}
```

看到了吗？这样就与循环顺序没有关系了，让 DP 在两维中交替进行。

### 解的打印

使用滚动数组后，解的打印几乎变成天方夜谭了。事实上即使不用滚动数组，想打印解还是很困难（试一试会发现无法打印字典序最大的解），此时应该使用 $dp[k,m]$ 表示“只考虑后 $k$ 个物品，**恰好**用 $m$ 的容量能装下的最大价值”。方法相同，打印时使用回溯法打印即可。实际上要求输出解的背包问题很少，但为了防住毒瘤可以考虑学习{% emoji 滑稽 %}。

## 完全背包

这类问题的形式是：   

- 你有 $n$ 种物品，每种有无限多，要把其中一些物品装进容量为 $V$ 的背包；
- 第 $i$ 种物品自己的体积是 $v_i$；
- 如果把第 $i$ 个物品放进背包，会获得 $w_i$ 的收益；
- 要求在不超过容量的情况下，最大化收益。

[这是模板](https://www.luogu.com.cn/problem/P1616)。

根据经验，设 $f[i,j]$ 表示选前 $i$ 种物品，体积为 $j$，物品最大价值，则有：

$$f[i,j] = \max\begin{cases}
f[i-1,j]
\\
f[i,j-v_i]+w_i &, j \geqslant v_i
\end{cases}$$

初始 $f[0][0]=0$，目标：$\max\limits_{0 \leqslant m \leqslant V}{f[n][m]}$。

代码如下：

```cpp
for (int i = 1; i <= n; ++i)
{
    int v = read(), w = read();
    for (int j = 0; j <= V; ++j)
        f[i][j] = f[i-1][j];
    // 这是另一种初始化的写法，代码较长但是效率较高
    for (int j = v; j <= V; ++j)
        f[i][j] = max(f[i][j], f[i][j-v] + w);
}
```

还是计算顺序的原因，在计算 $f(i,j)$ 时，$f[j]$ 保存的是 $f(i-1,j)$，而 $f[j-v]$ 保存的是 $f(i, j-v)$，计算 $f(i,j)$ 时，$f(i,j-v)$ 已经计算完毕。滚动数组代码如下：

```cpp
for (int i = 1; i <= m; ++i)
{
    int v = read(), w = read();
    for (int j = v; j <= V; ++j)
        f[j] = max(f[j], f[j-v] +w);
}
```

## 多重背包

这类问题的形式是：   

- 你有 $n$ 种物品，每种有 $C_i$ 个，要把其中一些物品装进容量为 $V$ 的背包；
- 第 $i$ 种物品自己的体积是 $v_i$；
- 如果把第 $i$ 个物品放进背包，会获得 $w_i$ 的收益；
- 要求在不超过容量的情况下，最大化收益。

如果我们把每种物品都拆成 $M_i$ 个物品，这几乎就跟 01 背包一模一样了。但遗憾的是，这样做很慢，时间复杂度高达 $\mathcal{O}(V\sum M_i)$。

面对这一情况有两种做法：二进制拆分法和单调队列优化法，时间复杂度分别为 $\mathcal{O}(V\sum \log M_i)$ 和 $\mathcal{O}(VN)$。其中单调队列优化法我们会在后续的动态规划教程中进行讨论，也可以阅读背包九讲来学习。

二进制拆分法基于这样一个事实：从 $2^0,2^1,2^2,\cdots,2^{k-1}$ 这 $k$ 个整数中选出若干个相加，可以得到 $0\sim 2^{k}-1$ 之间的所有数。为什么？想一想计算机是怎么用二进制表示数的吧！

进一步地，我们可以求出满足 $\sum\limits_{i=0}^{p}2^i\leqslant C_i$ 的最大整数 $p$，设 $R_i=C_i-\sum\limits_{i=0}^{p}2^i$，那么：

1. 根据 $p$ 的最大性，有 $\sum\limits_{i=0}^{p+1}2^i > C_i$，将 $R_i+\sum\limits_{i=0}^{p}2^i=C_i$ 代入得 $\sum\limits_{i=0}^{p+1}2^i > R_i+\sum\limits_{i=0}^{p}2^i$，可推出 $2^{p+1} > R_i$（两边同时减去 $\sum\limits_{i=0}^{p}2^i$），所以 $2^{p+1}-1 \ge R_i$，因此从 $2^0,2^1,2^2,\cdots,2^p$ 中选出若干个相加可以得到 $0\sim R_i$ 之间的任何整数（实际上是可以得到 $0\sim 2^{p+1}-1$ 之间的任意整数）。   
2. 从 $2^0,2^1,2^2,\cdots,2^p$ 以及 $R_i$ 中选择若干个数相加（注意多了个 $R_i$），可以得到 $R_i\sim R_i + 2^{p+1}-1$ 之间的任何整数（就是把 $1$ 中最后的结论用不等式性质一变形），而由于 $R_i$ 的定义，加上数学结论 $\sum\limits_{i=0}^{p}2^i=2^{p+1}-1$，可得到 $R_i+2^{p+1}-1=R_i=C_i-\sum\limits_{i=0}^{p}2^i+\sum\limits_{i=0}^{p}2^i+1-1=C_i$，所以等量代换后可以证明从 $2^0,2^1,2^2,\cdots,2^p$ 以及 $R_i$ 中选择若干个数相加，可以得到 $R_i\sim C_i$ 之间的任何整数。而又有从 $2^0,2^1,2^2,\cdots,2^p$ 中选出若干个相加可以得到 $0\sim R_i$ 之间的任何整数，因此从 $2^0,2^1,2^2,\cdots,2^p$ 以及 $R_i$ 中选择若干个数相加，可以得到 $0\sim C_i$ 之间的任何整数。

综上所述，我们只需要把数量为 $C_i$ 的第 $i$ 种物品拆成 $p+2$ 个物品，体积为 $2^0\times V_i, 2^1\times V_i,\cdots,2^p\times V_i,R_i\times V_i$，它们可以凑成 $0\sim C_i\times V_i$ 之间所有能被 $V_i$ 整除的数。

[模板](https://www.luogu.com.cn/problem/P1776)，代码如下：

{% folding cyan, 查看代码 %}
```cpp
// 注：我们通常用 v 代表体积，w 代表价值，但是此题中是反的。笔者在编码时遵照的时常规而不是原题。

#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, V;
int f[40005];

struct BadCreeper // (
{
    int v, w;
    BadCreeper(int v = 0, int w = 0) :
        v(v), w(w) {}
};

int main(void)
{
    n = read(), V = read();
    for (int i = 1; i <= n; ++i)
    {
        int w = read(), v = read(), m = read(), c = 1;
        vector <BadCreeper> a;
        while (m - c >= 0)
        {
            m -= c;
            a.push_back(BadCreeper(c * v, c * w));
            c <<= 1;
        }
        if (m != 0) a.push_back(BadCreeper(m * v, m * w)); // R = 0，没必要添加
        for (int j = 0; j < a.size(); ++j) // 枚举物品
            for (int k = V; k >= a[j].v; --k) // 01 转移
                f[k] = max(f[k], f[k - a[j].v] + a[j].w);
    }
    int ans = -1;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

## 混合背包

其实就是将前面三种背包混起来一起乱炖，有的物品只能取 $1$ 次，有的能取 $k$ 次，有的能取无限次。这怎么办？一种方法是将无限次化为最多能取得次数，这样就转化成多重背包了。但实际上没必要，我们只需要判断每种情况，然后套用不同种背包对应的循环即可。

[模板](https://www.luogu.com.cn/problem/P1833)，代码如下：

{% folding cyan, 查看代码 %}

```cpp
// 注：此代码编写时仍按照标准背包的定义

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#define pii pair<int, int>
#define X first
#define Y second

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, V;

inline int calc(string s)
{
    pii res;
    bool flag = 0;
    for (int i = 0; i < s.length(); ++i)
    {
        if (s[i] == ':')
        {
            flag = 1;
            continue;
        }
        if (flag) res.Y = res.Y * 10 + (s[i] - '0');
        else res.X = res.X * 10 + (s[i] - '0');
    }
    return res.X * 60 + res.Y;
}

int f[1005];

int main(void)
{
    string B, E;
    cin >> B >> E;
    V = calc(E) - calc(B);
    n = read();
    for (int i = 1; i <= n; ++i)
    {
        int v = read(), w = read(), p = read();
        if (p == 0)
        {
            for (int j = v; j <= V; ++j)
                f[j] = max(f[j], f[j - v] + w);
        }
        else // 你也可以把 01 背包的情况单独扔出来，不过没必要
        {
            vector <pii> a;
            int s = 1;
            while (p > s)
            {
                p -= s;
                a.push_back(make_pair(v * s, w * s));
                s <<= 1;
            }
            a.push_back(make_pair(v * p, w * p));
            for (int j = 0; j < a.size(); ++j)
                for (int k = V; k >= a[j].X; --k)
                    f[k] = max(f[k], f[k - a[j].X] + a[j].Y);
        }
    }
    int ans = 0;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```

{% endfolding %}

## 二维费用背包

二位费用背包是一类一个物品会消耗两种价值（比如汽水就是一种消耗金钱和寿命的东西{% emoji 滑稽 %}）。

设 $f(i,u,v)$ 表示只考虑前 $i$ 瓶汽水，消耗的金钱恰好为 $u$，消耗的阳寿恰好为 $v$，获得的脂肪是 $w$ {% emoji 滑稽 %}，那么转移方程便是：

$$f(i,u,v) = \max\begin{cases}
f(i-1,u,v)
\\
f(i-1,u-u_i,v-v_i)+w & ,u \geqslant u_i, v \geqslant v_i
\end{cases}$$

同样可以使用滚动数组优化空间（只有第一维是阶段，剩下的是状态，不能滚），注意相应的循环顺序。

[模板](https://www.luogu.com.cn/problem/P1855)，代码如下：

{% folding cyan, 查看代码 %}

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, M, T;
int f[205][205];

int main(void)
{
    n = read(), M = read(), T = read();   
    for (int i = 1; i <= n; ++i)
    {
        int m = read(), t = read();
        for (int j = M; j >= m; --j)
            for (int k = T; k >= t; --k)
                f[j][k] = max(f[j][k], f[j-m][k-t] + 1); // 每个愿望带来的价值都是相等的，都是单位 1
    }
    int ans = 0;
    for (int i = 0; i <= M; ++i)
        for (int j = 0; j <= T; ++j)
            ans = max(ans, f[i][j]);
    cout << ans << endl;
    return 0;
}
```

{% endfolding %}

也很容易转化到二维完全背包、多重背包等问题，这里不做赘述。

## 分组背包

这类问题跟 01 背包的本质区别是：物品可以分为 $k$ 组，每组中的物品相互冲突，即每组中最多只能选一个物品。问最大价值。[模板](https://www.luogu.com.cn/problem/P1757)。

设 $f(i,j)$ 表示在前 $i$ 组中选出总体积为 $j$ 的物品放入背包，物品的最大价值和。那么：

$$f(i,j) = \max\begin{cases}
f(i-1,j)
\\
\max\limits_{1\leqslant k \leqslant C_i} f(i-1,j-v_{(i,k)})+w_{(i,k)} & , j \geqslant v_{(i,k)}
\end{cases}$$

同样可以使用滚动数组滚掉第一维，代码如下：

{% folding cyan, 查看代码 %}

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

#define pii pair<int, int>
#define X first
#define Y second

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, V, f[1005];
vector <pii> a[1005];

int main(void)
{
    V = read(), n = read();
    for (int i = 1; i <= n; ++i)
    {
        int v = read(), w = read(), c = read();
        a[c].push_back(make_pair(v, w));
    }

    for (int i = 0; i < 1000; ++i)
    {
        if (a[i].size() == 0) continue; // 这组没有东西就不算了
        for (int j = V; j >= 0; --j)
            for (int k = 0; k < a[i].size(); ++k) // 决策
                if (j >= a[i][k].X) f[j] = max(f[j], f[j - a[i][k].X] + a[i][k].Y);
    }
    int ans = 0;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```

{% endfolding %}

注意循环顺序，因为 `i,j` 是状态，`k` 是决策，顺序不能混淆。

## 依赖性背包

这类问题长这样：在 01 背包的基础上，某些物品依赖于一个主物品，比如你想购买 Minecraft Java Edition，你就需要注册 Microsoft 账号，否则是不能购买 Minecraft Java Edition 的{% emoji 滑稽 %}。

这里我们讨论基础的依赖性背包，见[模板](https://www.luogu.com.cn/problem/P1064)。

我们对这个题做一个扩展：每个主件可以拥有无限个附件，这怎么办呢？可以将每个主件和它的附件作为分组背包中的一个组，想要对这个组进行背包问题的求解，就必须选择主件。对于包含一个主件和若干个附件的集合有以下可能性：仅选择主件，选择主件后再选择一个附件，选择主件后再选择两个附件……需要将以上可能性的容量和价值转换成一件件物品。因为这几种可能性只能选一种，所以可以将这看成分组背包。虽然这样做很慢，但足以通过原题（毕竟最多才两个附件）。对于本题而言，只需要依次判断每种情况即可。

代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

struct BadCreeper
{
    int v, w;
    BadCreeper(int v = 0, int w = 0) :
        v(v), w(w) {}
};

int n, V, f[32005];
vector <BadCreeper> a[65];

int main(void)
{
    V = read(), n = read();
    for (int i = 0; i < n; ++i)
    {
        int v = read(), p = read(), q = read();
        if (q == 0) a[i].push_back(BadCreeper(v, v * p));
        else a[q - 1].push_back(BadCreeper(v, v * p));
    }
    for (int i = 0; i < n; ++i)
        for (int j = V; j >= 0; --j)
        {
            if (a[i].size() > 0 && j >= a[i][0].v)
                f[j] = max(f[j], f[j - a[i][0].v] + a[i][0].w);
            if (a[i].size() > 1 && j >= a[i][0].v + a[i][1].v)
                f[j] = max(f[j], f[j - a[i][0].v - a[i][1].v] + a[i][0].w + a[i][1].w);
            if (a[i].size() > 2)
            {
                if (j >= a[i][0].v + a[i][2].v)
                    f[j] = max(f[j], f[j - a[i][0].v - a[i][2].v] + a[i][0].w + a[i][2].w);
                if (j >= a[i][0].v + a[i][1].v + a[i][2].v)
                    f[j] = max(f[j], f[j - a[i][0].v - a[i][1].v - a[i][2].v] + a[i][0].w + a[i][1].w + a[i][2].w);
            }
        }
    int ans = 0;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

这类问题的更复杂形态我们会在树形 DP 中讨论。

## Problemset

这里是一些简单各类背包问题的合集。它们本身不难，主要是帮助读者进一步了解背包，为接下来更困难的 DP 学习做准备。强烈读者仔细阅读这一部分，**其中会出现一些前面的专题没有讲述的内容**。

### 简单问题

这里的问题都比较简单，可以当作复习的题目。

#### [NOIP2001 普及组] 装箱问题

[Portal](https://www.luogu.com.cn/problem/P1049).

由于我们希望剩余空间尽可能地小，那么可以将物品的体积视作物品的价值，那么本题就是标准的 01 背包问题。代码如下：

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int v, n, w;
int f[30005];

int main(void)
{
    scanf("%d%d", &v, &n);
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &w);
        for (int j = v; j >= w; j--)
            f[j] = max(f[j], f[j - w] + w);
    }
    printf("%d", v - f[v]); // 根据之前的推论，这么做也可
    return 0;
}
```
{% endfolding %}

#### [CH5201] 数字组合

[Portal](http://noi-test.zzstep.com/contest/0x50%E3%80%8C%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92%E3%80%8D%E4%BE%8B%E9%A2%98/5201%20%E6%95%B0%E5%AD%97%E7%BB%84%E5%90%88).

这个问的是将背包装满的**方案总数**。设计状态：$f[i,j]$ 表示用前 $i$ 个物品，恰好填满 $j$ 的体积的方案总数。首先 $f[i,0]$ 必定为 $1$。转移：$f[i,j]=f[i-1,j]+f[i-1,j-v]$，跟 01 背包很像？的确如此，对于这类改变问法的问题，一般只需将状态转移方程中的 max 改成 sum 即可。事实上，这样做可行的原因在于状态转移方程已经考察了所有可能的背包组成方案。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
int f[10005];

int main(void)
{
    scanf("%d%d", &n, &m);
    f[0] = 1;
    for (int i = 1; i <= n; ++i)
    {
        int w;
        scanf("%d", &w);
        for (int j = m; j >= w; --j) // 数字只能使用一次，对应 01 背包的顺序
            f[j] += f[j - w];
    }
    printf("%d\n", f[m]);
    return 0;
}
```

But WAIT! 这样不会产生之前所说的“不用取最大值也是对的”的填进去空气的情况吗？不会，我们只让 $f[i,0]$ 为 $1$，而 $f[i,1]$ 这种空气是 $0$，没有方案。

#### [CH5202] 自然数拆分 Lunatic 版

[Portal](http://noi-test.zzstep.com/contest/0x50%E3%80%8C%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92%E3%80%8D%E4%BE%8B%E9%A2%98/5202%20%E8%87%AA%E7%84%B6%E6%95%B0%E6%8B%86%E5%88%86Lunatic%E7%89%88).

就是求方案总数的完全背包：容量为 $n$，有 $1\cdots n$ 的物品体积。注意要使用若干个，不能是 $1$ 个（我拆成我自己{% emoji 滑稽 %}），所以最终结果要 `-1`。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

const unsigned int MOD = 2147483648u;

int n;
unsigned int f[4005];

int main(void)
{
    scanf("%d", &n);
    f[0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = i; j <= n; ++j)
            f[j] = (f[j - i] + f[j]) % MOD;
    printf("%u\n", (f[n] - 1 + MOD) % MOD);
    return 0;
}
```
{% endfolding %}

#### [NOI Online #3 入门组] 买表

[Portal](https://www.luogu.com.cn/problem/P6567).

直接使用多重背包的模板即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, m;
bool f[500005];

int main(void)
{
    scanf("%d%d", &n, &m);
    f[0] = true;
    for (int i = 1; i <= n; ++i)
    {
        int k, a, c = 1;
        scanf("%d%d", &k, &a);
        while (a - c >= 0)
        {
            a -= c;
            int v = k * c;
            for (int j = 500000; j >= v; --j)
                f[j] |= f[j - v];
            c <<= 1;
        }
    }
    while (m--)
    {
        int x;
        scanf("%d", &x);
        puts(f[x] ? "Yes" : "No");
    }
    return 0;
}
```
{% endfolding %}

#### [BJOI2019] 排兵布阵

[Portal](https://www.luogu.com.cn/problem/P5322).

假设 `s=1`，那么这就是普通的背包问题了。现在将每个城堡排序一下，枚举要打过多少个玩家，就可以轻松算出贡献了。

{% folding cyan::查看代码 %}
```cpp
// 甚至连滚动数组都不用
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int s, n, m;
int a[105][105];
int f[105][20005]; // 只考虑前 i 座城堡，恰好使用 j 位士兵所获得的最大收益

int main(void)
{
    scanf("%d%d%d", &s, &n, &m);
    for (int i = 1; i <= s; ++i)
        for (int j = 1; j <= n; ++j)
            scanf("%d", &a[j][i]);
    for (int i = 1; i <= n; ++i)
        sort(a[i] + 1, a[i] + s + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) // 若使用滚动数组，需要倒序
        {
            f[i][j] = f[i - 1][j];
            for (int k = 1; k <= s; ++k)
                if (j >= a[i][k] * 2 + 1) f[i][j] = max(f[i][j], f[i - 1][j - a[i][k] * 2 - 1] + k * i); // 贡献为 k * i
        }
    int ans = 0;
    for (int j = 0; j <= m; ++j)
        ans = max(ans, f[n][j]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 较复杂题目

这些题目会比较复杂。

#### [NOIP2018 提高组] 货币系统

[Portal](https://www.luogu.com.cn/problem/P5020).

~~大凯的疑惑。~~

可以猜测 $(m,b)$ 的所有货币面额在 $(n,a)$ 中一定都有，我们只需要看哪些没有。先排序，然后扫描，使用完全背包，可以拼成的就赋值为 $1$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

int n;
int a[105];
bool f[25005];

int main(void)
{
    int T;
    cin >> T;
    while (T--)
    {
        scanf("%d", &n);
        for (int i = 1; i <= n; ++i) scanf("%d", a + i);
        sort(a + 1, a + n + 1);
        int ans = n;
        memset(f, 0, sizeof(f));
        f[0] = true;
        for (int i = 1; i <= n; ++i)
        {
            if (f[a[i]])
            {
                --ans;
                continue;
            }
            for (int j = a[i]; j <= 25000; ++j)
                f[j] |= f[j - a[i]];
        }
        printf("%d\n", ans);
    }
    return 0;
}
```
{% endfolding %}

#### [CSP-J2019] 纪念品

[Portal](https://www.luogu.com.cn/problem/P5662).

由于当日购买的纪念品也可以当日卖出换回金币，那么我们可以强制让第 $i$ 天买的纪念品在第 $i+1$ 天卖掉。但如果它应该在 $i+2$ 天卖掉怎么办？不要紧，在第 $i+1$ 天再买回来，价格是一样的。

于是直接完全背包板子拍上去就行了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int t, n, m;
int p[105][105];
int f[10005];

int main(void)
{
    scanf("%d%d%d", &t, &n, &m);
    for (int i = 1; i <= t; ++i)
        for (int j = 1; j <= n; ++j)
            scanf("%d", p[i] + j);
    for (int i = 1; i < t; ++i)
    {
        memset(f, 0, sizeof(f));
        for (int j = 1; j <= n; ++j)
            for (int k = p[i][j]; k <= m; ++k)
                f[k] = max(f[k], f[k - p[i][j]] + p[i + 1][j] - p[i][j]);
        m += f[m];
    }
    printf("%d\n", m);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1441] 砝码称重

[Portal](https://www.luogu.com.cn/problem/P1441).

使用搜索枚举删掉哪些砝码，然后对剩下的砝码使用背包。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, m, sum = 0, ans = 0;
int a[25];
bool f[2005], del[25];

void dfs(int o, int cnt)
{
    if (cnt == m) 
    {
        memset(f, 0, sizeof(f));
        f[0] = true;
        int ret = 0;
        for (int i = 1; i <= n; ++i)
            if (!del[i])
            {
                for (int j = 2000; j >= 0; --j)
                    if (f[j] && !f[j + a[i]]) f[j + a[i]] = true, ++ret;
            }
        ans = max(ans, ret);
        return;
    }
    if (o > n || cnt > m) return;
    dfs(o + 1, cnt);
    del[o] = true;
    dfs(o + 1, cnt + 1);
    del[o] = false;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) 
    {
        scanf("%d", a + i);
        sum += a[i];
    }
    dfs(1, 0);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1782] 旅行商的背包

[Portal](https://www.luogu.com.cn/problem/P1782).

就是将多重背包和分组背包混在了一起（将奇货拆开），但是本题时间很紧，需要大力卡常。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define V first 
#define W second

using namespace std;
typedef long long i64;

inline int read(void)
{
    int x = 0, c = getchar_unlocked(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar_unlocked();}
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar_unlocked();
    return x * f;
}

int n, m, C, tot = 0;
i64 f[100005];
pair<i64, i64> a[2000005];

int main(void)
{
    n = read(), m = read(), C = read();
    while (n--)
    {
        i64 v = read(), w = read(), d = read(), c = 1;
        // 这个二进制优化非常的强力，先处理物品再统一计算
        while (d)
        {
            if (d >= c) d -= c;
            else c = d, d = 0;
            a[++tot].V = v * c, a[tot].W = w * c;
            c <<= 1;
        }
    }
    // 放到外面统一计算
    for (int i = 1; i <= tot; ++i)
        for (int j = C; j >= a[i].V; --j)
            f[j] = max(f[j], f[j - a[i].V] + a[i].W);
    while (m--)
    {
        i64 a = read(), b = read(), c = read();
        for (int j = C; j >= 0; --j)
            for (int v = 0; v <= j; ++v)
                f[j] = max(f[j], f[j - v] + a * v * v + b * v + c); 
    }
    printf("%lld\n", f[C]);
    return 0;
}
```
{% endfolding %}

{% divider fences %}

#### [Luogu P1284] 三角形牧场

[Portal](https://www.luogu.com.cn/problem/P1284).

注意到所有的木板都需要使用，那么我们只需要确定三角形两条边的长度就可以知道第三边，然后使用海伦公式计算三角形面积。只需要配合一个二维状态的 01 背包就可以计算这样的长度是否可以得到满足。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

double S(double a, double b, double c)
{
	double p = (a + b + c) / 2;
	return sqrt(p * (p - a) * (p - b) * (p - c));
}

bool check(int a, int b, int c)
{
	if (a <= 0 || b <= 0 || c <= 0) return false;
	if (a + b <= c || a + c <= b || b + c <= a) return false;
	return true;
}

int n;
int l[45];
bool f[805][805];

int main(void)
{
	scanf("%d", &n);
	int sum = 0;
	for (int i = 1; i <= n; ++i) 
		scanf("%d", l + i), sum += l[i];
	f[0][0] = true;
	for (int i = 1; i <= n; ++i)
		for (int j = 800; j >= 0; --j)
			for (int k = 800; k >= 0; --k)
			{
				if (j - l[i] >= 0) f[j][k] |= f[j - l[i]][k];
				if (k - l[i] >= 0) f[j][k] |= f[j][k - l[i]];
			}
	double ans = -1.0;
	for (int i = 0; i <= 800; ++i)
		for (int j = 0; j <= 800; ++j)
			if (f[i][j] && check(i, j, sum - i - j)) ans = max(ans, S(i, j, sum - i - j));
	if (ans == -1.0) puts("-1");
	else printf("%d\n", int(ans * 100));
	return 0;
}
```
{% endfolding %}

#### [Luogu P1156] 垃圾陷阱

[Portal](https://www.luogu.com.cn/problem/P1156).

设 $f(j)$ 代表高度为 $j$ 时的最长存活时间，然后就是 01 背包了（采用刷表法）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

struct Rubbish
{
	int t, f, h;
	bool operator < (const Rubbish &a) const
	{
		return t < a.t;
	}
} a[105];

int D, G;
int f[105];

int main(void)
{
	scanf("%d%d", &D, &G);
	for (int i = 1; i <= G; ++i)	
		scanf("%d%d%d", &a[i].t, &a[i].f, &a[i].h);
	sort(a + 1, a + G + 1);
	f[0] = 10;
	for (int i = 1; i <= G; ++i)
		for (int j = D; j >= 0; --j)
			if (f[j] >= a[i].t)
			{
				if (j + a[i].h >= D)
				{
					printf("%d\n", a[i].t);
					return 0;
				}
				f[j + a[i].h] = max(f[j + a[i].h], f[j]);
				f[j] += a[i].f;
			}
	printf("%d\n", f[0]);
	return 0;
}
```
{% endfolding %}

## 小结

背包问题主要以算法模板的形式出现在考试中，难在一道题如何转化成背包问题，但这不是本文讲解的重点。想要学会这一点，只能通过学习各种各样的 DP，做大量的题目，积累经验。请读者务必记住各种背包的原理，而不是背住循环顺序。

学习完毕后，接下来可以阅读：

{% link 区间 DP::/2022/1177a5e3/ %}