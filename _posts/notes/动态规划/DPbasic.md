---
title: 动态规划基础
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 学习笔记
    - 动态规划
tag:
    - 动态规划
    - 线性 DP
hide: true
group: notes
headimg: https://s1.ax1x.com/2022/08/04/vm910J.jpg
abbrlink: cafcfd0d
status: done
date: 2022-07-25 08:58:06
---

> 动态规划（Dynamic Programming，DP）是运筹学的一个分支，是求解决策过程最优化的过程。20 世纪 50 年代初，美国数学家贝尔曼（R.Bellman）等人在研究多阶段决策过程的优化问题时，提出了著名的最优化原理，从而创立了动态规划。动态规划问世以来，在经济管理、生产调度、工程技术和最优控制等方面得到了广泛的应用。例如最短路线、库存管理、资源分配、设备更新、排序、装载等问题，用动态规划方法比用其它方法求解更为方便。所以动态规划不仅在 OI 中应用广泛，在生活实际同样应用广泛。本文将引导你学习简单的动态规划。

<!--more-->

{% folding blue::更新日志 %}

{% timeline %}

{% timenode 2022/08/11 %}

增补了 LIS 的 $O(n\log n)$ 算法及其变形。

{% endtimenode %}

{% timenode 2022/08/08 %}

完成了预定的所有内容。

{% endtimenode %}

{% endtimeline %}

{% endfolding %}

为方便，接下来我们均用 DP 指代动态规划。

## 引子

我们先来看这样一个问题。

`james1` 要写一封**恰好**有 $n$ 个字的入团申请书，但由于 `james1` 非常懒（~~假的~~），所以他会在一开始有 $1$ 个字的文稿里进行以下两种操作：

- 写字：在文稿末尾写 $1$ 个字，就是说如果文稿有 $k$ 个字，那么写字后文稿会有 $k+1$ 个字。
- 复读：将写完这文稿复制，粘贴到文稿末尾，就是说如果文稿有 $k$ 个字，那么复制后文稿会有 $k \times 2$ 个字。

问 `james1` 最少要进行多少次操作，才能完成这封入团申请书？   
~~真实情况下入团申请书可别这么写，否则入不进去别找我。~~

### 贪心的末日

感觉这道题长得很像贪心，然而一般人写的贪心是错的{% emoji 滑稽 %}。   
为什么说是一般人呢？~~因为一般人都不会倒着想题~~。如果正着贪心，因为是恰好 $n$ 个字，比如我要写 $31$，那么我会这样 $1 \rightarrow 2 \rightarrow 4 \rightarrow 8 \rightarrow 16 \rightarrow 17 \rightarrow 18 \rightarrow ... \rightarrow 31$

耗费了大量的操作，这样显然不是最优。

### 决策

既然贪心不行，那么我们则么办呢？

`james1` 显然拥有“选择的权利”。   
每一秒钟，他要么决定“写字”，要么决定“复读”。   
`james1` 写完文稿的过程，可以理解成一系列决策：最开始是 $1$ 个字，通过一次次决策，文稿变得越来越长，最终达到 $n$ 个字。

这里列出了两种凑出 $10$ 个字的方案。第一种步数更少，所以是优秀的方案；后者是不优的方案。

![](https://pic.imgdb.cn/item/62ddebc1f54cd3f937d48caf.jpg)

在我们手上有 $4$ 个字的时候，我们该如何选择？

![](https://pic.imgdb.cn/item/62ddebd1f54cd3f937d4c096.jpg)

然而，我们很难以在“手上有 $4$ 个字”的情况下，选出“接下来是变成 $5$ 个字还是 $8$ 个字”。这需要我们有预判未来的能力！

这显然不是正常人能做到的，所以我们要换一种思维方式。

### 子问题的拆分

我们来考虑另一种思维。现在我们目标是凑出 $n$ 个字，我们尝试先决定“最后一
步是写字还是复读”（某人：终于学会倒着想了{% emoji 滑稽 %}）。

这个问题似乎简单很多。首先，如果 $n$ 是奇数，那么我们没得选，它只能是写字之后的结果；如果 $n$ 是偶数，只要我们知道“凑出 $n-1$ 个字的时间”和“凑出 $\cfrac{n}{2}$ 个字的时间”，就可以选出耗时更短的方案。具体而言：

$$
cost(n)=\begin{cases}
cost(n-1)+1 & ,n \text{ 是奇数} ,\\
\text{min}\{cost(\cfrac{n}{2})+1,~cost(n-1)+1\} & ,n \text{ 是偶数}.
\end{cases}
$$

例如，在 $n=10$ 的时候决策，如果知道“凑出 $5$ 的代价是 $3$”以及“凑出 $9$ 的代价是 $8$”，我们就会选择“从 $5$ 复读得到 $10$”，代价是 $3+1=4$。

### 实现

根据刚才推出的公式，实现变得非常简单：

```cpp
int n, cost[105];
int main(void)
{
    scanf("%d", &n);
    cost[1] = 0;

    for (int i = 1; i <= n; ++i)
    {
        if (n % 2 == 1) cost[i] = cost[i-1]+1;
        else cost = min(cost[i/2]+1, cost[i-1]+1);
        printf("cost[%d] = %d\n", i, cost[i]);
    }
}
```

可以发现就是对着刚才的公式照抄，时空复杂度均为 $\mathcal{O}(n)$。

### 优化

然而这东西不是最优{% emoji 滑稽 %}，之前说了，正着贪心是错的，但倒着贪心是对的{% emoji 滑稽 %}。

但为什么倒着贪心是对的？很简单，因为 $cost(\cfrac{n}{2})+1\leqslant cost(n-1)+1$ 恒成立{% emoji 滑稽 %}。

代码更简单了：

```cpp
int ans = 0;
while (n != 1) 
{
    if (n % 2 == 1) --n;
    else n /= 2;
    ++ans;
}
cout << ans << endl;
```

时间复杂度 $\Theta(\log n)$，空间 $\mathcal{O}(1)$ {% emoji 滑稽 %}。

### 状态和子问题

我们刚刚谈到了“拆分子问题”，就是我们现在需要回答一个大问题（凑 $n$ 个字）。
我们先解决几个小问题（凑 $\cfrac{n}{2}$ 个字、凑 $n−1$ 个字），把这些小问题的结论综合起来，获得大问题的答案。DP 中，一般把我们面对某个问题的情景也叫做“**状态**”。例如我们手上有 $k$ 个字，这是一个状态。我们可以作出“复读”的决策，转移到 $2k$ 这个状态；也可以作出“写字”的决策，转移到 $k+1$ 这个状态。    

引入名词是为了以后能更加简便地描述算法。

### 状态转移方程

$$
cost(n)=\begin{cases}
cost(n-1)+1 & ,n \text{ 是奇数} ,\\
\text{min}\{cost(\cfrac{n}{2})+1,~cost(n-1)+1\} & ,n \text{ 是偶数}.
\end{cases}
$$

刚才这个式子便是**状态转移方程**。

## 硬币问题

{% noteblock link green %}

[Luogu B3635](https://www.luogu.com.cn/problem/B3635).

今有三种面额的钱币，$1$、$5$ 和 $11$。现在要凑出 $n$ 元钱，问最少需要多少张钱才能达到目的？

{% endnoteblock %}

可以证明，贪心是错误的，我们需要用 DP 来解决这道题。

### 状态的设计

显然，$f(i)$ 表示凑出 $i$ 元最少需要多少张钱。 

### 状态的转移

除了刚才的方法，我们还将介绍一种新的方法。

#### 传统方法（填表法）

根据之前的经验，我们可以设计出状态转移方程：

$$
f(i)=\min\{f(i-1)+1,~f(i-5)+1,~f(i-11)+1\}
$$

程序也很好写：

```cpp
int n, f[1005];
memset(f, 0x3f, sizeof(f));
f[0] = 0;

scanf("%d", &n);
for (int i = 1; i <= n; ++i) 
{
    if(i-1 >= 0) f[i] = min(f[i], f[i-1] + 1);
    if(i-5 >= 0) f[i] = min(f[i], f[i-5] + 1);
    if(i-11 >= 0) f[i] = min(f[i], f[i-11] + 1);
    printf("f[%d] = %d\n", i, f[i]);
}
```

像这样，对于每个状态 $i$，计算 $f(i)$ 的方式，我们称之为“**填表法**”，或形象地称之为“我从哪里来”，也就是 pull 型的转移。

#### 刷表法

既然有“我从哪里来”，那么必定有“我到哪里去”，也就是刷表法，push 型的转移，即对于每个状态 $i$，更新 $f(i)$ 所影响的状态，对于此题而言：

```cpp
int n, f[1015]; // 想想这里为什么要把空间开大

// 初始化和读入略

for (int i = 0; i <= n; ++i) // 从 0 开始
{
    // 更新所影响的状态
    f[i+1] = min(f[i+1], f[i] + 1); 
    f[i+5] = min(f[i+5], f[i] + 1);
    f[i+11] = min(f[i+11], f[i] + 1);
    printf("f[%d] = %d\n", i, f[i]);
}
```

通常我们不把刷表法的式子称作状态转移方程，因为它只是一个更新公式。

一般地（嗯，真有不一般地{% emoji 滑稽 %}），填表法和刷表法可以互相转化，只是一种比较容易些，另一种比较难写。需根据实际情况判断用什么方法。

### 解的打印

虽然大多数题目都不要求打印解，但我们还是得会。   
~~有哪位神仙知道怎么做嘛？~~

> 我不会，但我一定是对的。    
> —— 搜索

~~好，那我们来试试吧！~~

比如我们用 `print_ans(n)` 来打印凑 $n$ 块钱时的解。

`print_ans(n)` 从哪里来？当然是从 `print_ans(n-1)`、`print_ans(n-5)` 或 `print_ans(n-11)` 来。

那么是这三个中的那个呢？很简单，我们可以根据 `f` 数组来判断{% emoji 滑稽 %}。

像这样：

```cpp
void print_ans(int x)
{
    if (x >= 1 && f[x] == f[x-1] + 1) //f[x] == f[x-1] + 1 判断我们递推时是不是这么干的
    {
        printf("1 ");
        print_ans(x-1);
    }
    else if (x >= 5 && f[x] == f[x-5] + 1)
    {
        printf("5 ");
        print_ans(x-5);
    }
    else if (x >= 11 && f[x] == f[x-11] + 1)
    {
        printf("11 ");
        print_ans(x-11);
    }
}

```

## 记忆化搜索

**斐波那契数列**。输出斐波那契数列的第 $n$ 项。（$1\leqslant n \leqslant 10^7$）

这里我们用递归来实现：

```cpp
int fib(int n)
{
    if(n <= 2) return 1;
    return fib(n-2) + fib(n-1);
}

int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%d\n", fib(n));
    return 0;
}
```

单说这份程序，谁都写得出来，但是，**它很慢**！慢到什么程度？
因为斐波那契数列是指数级的，所以它的时间复度是 $\Theta (2^n)${% emoji 滑稽 %}，所以在 $n=10^7$ 的时候，算到宇宙毁灭它也算不完{% emoji 滑稽 %}。

可为什么它这么慢？很简单，**重复计算**。比如我要算 $fib(10)$，想知道 $fib(10)$，我需要知道 $fib(8)$ 和 $fib(9)$。而为此需要知道 $fib(6)$、$fib(7)$ 和 $fib(8)$，而 $fib(7)$ 需要计算两次，如此看来，重复计算量是非常大的！

那么我们加个数组记录下来之前算的结果不就行了？像这样：

```cpp
int f[10000005];
bool vis[10000005];

int fib(int n)
{
    if(n <= 2) return 1;
    if (vis[n]) return f[n];
    vis[n] = 1;
    return f[n] = fib(n-2) + fib(n-1);
}

int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%d\n", fib(n));
    return 0;
}
```

为什么使用 $vis$ 数组？只是因为斐波那契数列每一项都是正的，所以有的人会通过 $f[i]=-1$ 的方式来判断 $i$ 没访问过，但有负数就行不通了，所以采用 $vis$ 这种更加通用的方式。

这种搜索时记录以前答案的形式，称之为**记忆化搜索**。

## DP 解题思路

我们通过两道题目感受一下 DP 的解题思路。

### [IOI1994] 数字三角形

[Portal](https://www.luogu.com.cn/problem/P1216).

{% folding cyan::题面 %}
观察下面的数字金字塔。

写一个程序来查找从最高点到底部任意处结束的路径，使路径经过数字的和最大。每一步可以走到左下方的点也可以到达右下方的点。

```
        7 
      3   8 
    8   1   0 
  2   7   4   4 
4   5   2   6   5 
```

在上面的样例中,从 $7 \to 3 \to 8 \to 7 \to 5$ 的路径产生了最大。
{% endfolding %}

#### 设计状态

用一个状态来描述目前需要求解的情景，这个过程称为**设计状态**。

本题可以这样设计状态：$f(x,y)$ 表示“走到 $(x,y)$ 的位置，能获取的最大收益”。注意，走到第 $r$ 行的任意一个都可以。

#### 设计转移

设计完状态之后，我们要写出状态转移方程（或更新公式）。这个步骤叫做“设计转移”。这里采用的是”刷表法”。

由于状态 $(x,y)$ 要么走到下面的 $(x+1, y)$ 走过来，要么从右下角 $(x+1,y+1)$ 走过来。所以可以这样： 

$$f(x+1,y)=\max\{f(x+1,y),f(x,y)+a[x+1][y]\} \\
f(x+1,y+1)=\max\{f(x+1,y+1),f(x,y)+a[x+1][y+1]\}$$

#### 实现

注意答案要取最大值即可。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int r;
int a[1005][1005], f[1005][1005];

int main(void)
{
    scanf("%d", &r);
    for (int i = 1; i <= r; ++i)
        for (int j = 1; j <= i; ++j)
            scanf("%d", &a[i][j]);
    f[1][1] = a[1][1];
    for (int i = 1; i < r; ++i)
        for (int j = 1; j <= i; ++j)
        {
            f[i + 1][j] = max(f[i + 1][j], f[i][j] + a[i + 1][j]);
            f[i + 1][j + 1] = max(f[i + 1][j + 1], f[i][j] + a[i + 1][j + 1]);
        }
    int ans = 0;
    for (int i = 1; i <= r; ++i)
        ans = max(ans, f[r][i]);
    printf("%d\n", ans);
    return 0;
}
```

#### 用 dfs 解决

我们来看：

```cpp
int dfs(int x, int y) // 代表从 [x,y] 出发
{
    if (x == n) return a[x][y];
    return max(dfs(x + 1, y), dfs(x + 1, y + 1)) + a[x][y];
}
```

还要加上记忆化：

```cpp
int g[1005][1005]; // 先执行 memset(g, 0xff, sizeof(g));
int dfs(int x, int y) // 代表从 [x,y] 出发
{
    if (g[x][y] != -1) return g[x][y];
    if (x == n) return g[x][y] = a[x][y];
    return g[x][y] = max(dfs(x + 1, y), dfs(x + 1, y + 1)) + a[x][y];
}
```

其实这样是定义了状态：$f(x,y)$ 代表从 $(x,y)$ 出发。转移方程为：$f(x,y)=max(f(x + 1, y), f(x + 1, y + 1)) + a[x][y]$。考虑到依赖性的问题，所以要倒序 DP。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int r;
int a[1005][1005], f[1005][1005];

int main(void)
{
    scanf("%d", &r);
    for (int i = 1; i <= r; ++i)
        for (int j = 1; j <= i; ++j)
            scanf("%d", &a[i][j]);
    for (int i = 1; i <= r; ++i)
        f[r][i] = a[r][i];
    for (int i = r - 1; i >= 1; --i)
        for (int j = 1; j <= i; ++j)
            f[i][j] = max(f[i + 1][j], f[i + 1][j + 1]) + a[i][j];
    printf("%d\n", f[1][1]);
    return 0;
}
```

### 最大子段和

[Portal](https://www.luogu.com.cn/problem/P1115).

设 $f(i)$ 代表以 $i$ 结尾（必须含 $i$）的最大子段和，那么有 $f(i)=\max\{f(i-1)+a[i],a[i]\}$。目标为 $\max\{f(i)\}$。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, a[200005], f[200005], ans = -10005;

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
    {
        f[i] = 0;
        scanf("%d", &a[i]);
        f[i] = max(f[i - 1] + a[i], a[i]);
        ans = max(f[i], ans);
    }
    printf("%d", ans);
    return 0;
}
```

## DP 二要素

DP 有二要素，**最优子结构**和**无后效性**。

### 最优子结构

大问题的最优解，一定是从小问题的最优解推出来的。比如硬币问题，我们只关心 $f(15)$ 的最优解，从而推出依赖 $f(15)$ 的 $f(16)$、$f(20)$、$f(26)$ 的解（填表法）。

### 无后效性

现在的决策，只与过去的结果有关，而与过去的决策无关。这跟 DP 的本质很像，在每一个阶段根据以前的答案（不是决策）推出当前阶段的答案。为什么必须满足无后效性，我们来看：

[[NOIP2010 提高组] 乌龟棋](https://www.luogu.com.cn/problem/P1541)。

啊，我知道了，我来设计状态，像这样：$f(i)$ 表示走到位置 $i$ 能获得的最大分数。   
But wait！注意这样我们不知道之前用了哪些爬行卡片，所以状态是**有后效性**的！换句话说，之前的决策会影响现在的结果。所以状态定义的不够好，无法转移。

可不就是不知道用了哪些卡片嘛！那我这么干：$f(p,i,j,k,l)$ 代表走到第 $p$ 个位置，用了卡片 $1,2,3,4$ 分别 $i,j,k,l$ 张。   
实际上没必要，因为知道用了哪些卡片就知道走到那个位置了，所以只需定义 $f(i,j,k,l)$。因为本题数据范围很小，所以不会爆空间{% emoji 滑稽 %}。

```cpp
#include <iostream>
#include <cstdio>
#include <cctype>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m;
int a[355], b[5];
int dp[45][45][45][45];

int main(void)
{
    n = read(), m = read();
    for (int i = 1; i <= n; ++i) a[i] = read();
    while (m--) ++b[read()]; //每种卡牌的数量

    dp[0][0][0][0] = a[1];
    //这里使用填表法，当然，刷表法也是可以的
    for (int i = 0; i <= b[1]; ++i)
        for (int j = 0; j <= b[2]; ++j)
            for (int k = 0; k <= b[3]; ++k)
                for (int l = 0; l <= b[4]; ++l)
                {
                    int c = i + j*2 + k*3+ l*4 + 1; //计算从 1 用这些卡牌走到了哪里
                    #define f dp[i][j][k][l] //简化代码的技巧
                    if (i != 0) f = max(f, dp[i-1][j][k][l] + a[c]);
                    if (j != 0) f = max(f, dp[i][j-1][k][l] + a[c]);
                    if (k != 0) f = max(f, dp[i][j][k-1][l] + a[c]);
                    if (l != 0) f = max(f, dp[i][j][k][l-1] + a[c]);
                }
    printf("%d", dp[b[1]][b[2]][b[3]][b[4]]);
    return 0;
}
```

像这样，把引起后效性的因素直接加入到状态里，是处理后效性的手段之一（其它以后会见到），形象地称之为“打不过就加入”。

## 线性 DP

以上可以说都是 DP 的引子，真正的挑战现在才刚刚开始。

线性 DP 是指“具有线性阶段”的 DP，也包含具有多维度阶段的 DP，比如上一道例题“乌龟棋”。接下来我们来看更多的经典问题。

### 一维结构上的动态规划

它们的结构只有一维，但是并不是说状态一定只有一维。

#### [Luogu P1192] 台阶问题

[Portal](https://www.luogu.com.cn/problem/P1192).

设 $f(i)$ 代表跳到第 $i$ 级台阶的方案数，初始 $f(0)=0$，有 $f(i)=\sum_{max(0,i-k)}^{i-1}f(j)$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

const int MOD = 100003;

int n, k;
int f[100005];

int main(void)
{
    scanf("%d%d", &n, &k);
    f[0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = max(i - k, 0); j < i; ++j)
            f[i] = (f[i] + f[j]) % MOD;
    printf("%d\n", f[n]);
    return 0;
}
```
{% endfolding %}

#### 最长上升子序列

最长上升子序列（Longest Increasing Subsequence，LIS）问题：给定一个长度为 $N$ 的序列，求最长上升子序列，即这个长度为 $M(M \le N)$ 的子序列 $B$ 满足 $\forall i \in [1,M), B_i <B_{i+1}$。子序列指在原序列中任意删掉几个数（可以是 $0$ 个数）所得到的序列。注意，这里是严格上升。

你可以在 [POJ2533](https://vjudge.net/problem/POJ-2533) 提交这道题。

怎么求呢？设 $f[i]$ 为 $i$ 结尾的 LIS 长度，那么可以设计出转移：$f[i] = \max\{f[j]+1\}, 0\le j < i,A_j< A_i$。

代码很简单：

```cpp
int ans = 0;
for (int i = 1; i <= n; ++i)
{
    f[i] = 1; // 有时候这一步是必要的（当求的不是最长上升而是最长不升之类的），
    // 如果求的是最长上升，f[i] 必定可以在 j = 0 时取到 1
    // 但是最长不升没有这种特点
    // 为防止出错，建议何时都要把这一步写上去。
    for (int j = 0; j < i; ++j)
        if (a[i] > a[j]) f[i] = max(f[j] + 1, f[i]);
    ans = max(ans, f[i]);
}
```

但是这种做法太慢了，复杂度高达 $O(n^2)$，还有一种基于二分的优化到 $O(n\log n)$ 的做法。   

设 $g(i)$ 表示 $f(j)=i$ 时最小的 $A_j$，也就是长度为 $i$ 的 LIS 的结尾数最小的值。可以证明 $g_i$ 是递增的（子序列都变长了这玩意怎么可能减小），具有单调性。那么对于当前扫描的 $f(i)$，可以在之前已经找到的 $g$ 中，找到第一个大于等于 $A_i$ 的 $g(k)$，则此时 $f(i)=k$。

实际上是因为 LIS 问题要求 $A_i > A_j$，我们用 $A_i$ 去在 $g$ 数组中检索，检索到了一个大于等于的下标，设这个下标对应的最小的 $A_j$ 是 $x$，满足 $x\ge A_j$，由于 $x$ 的最小性，$x$ 的前一个数 $y$ 一定满足 $y<A_j$。也就是说我们实际上要找满足 $g[k']<A_i$ 的最后一个下标 $k'$，就有 $f(i)=k'+1$，而 $k$ 是第一个满足 $g[k]\ge A_i$ 的，而我们的这种求法会使 $k=k'+1$，也就有了 $f(i)=k$。由于在查找的时候 $g[k]\ge A_i$，而我们也要找最小的 $A_j$，就将 $g[k]$ 更新为 $A_i$ 即可。

```cpp
for (int i = 1; i <= n; ++i) g[i]  = INF; // INF 表示不存在
for (int i = 1; i <= n; ++i)
{
    int k = lower_bound(g + 1, g + n + 1, a[i]) - g;
    f[i] = k;
    g[k] = a[i];
}
```

[\[NOIP1999 普及组\] 导弹拦截](https://www.luogu.com.cn/problem/P1020)。

> 某国为了防御敌国的导弹袭击，发展出一种导弹拦截系统。但是这种导弹拦截系统有一个缺陷：虽然它的第一发炮弹能够到达任意的高度，但是以后每一发炮弹都不能高于前一发的高度。某天，雷达捕捉到敌国的导弹来袭。由于该系统还在试用阶段，所以只有一套系统，因此有可能不能拦截所有的导弹。   
> 输入导弹依次飞来的高度（雷达给出的高度数据是 $\le 5\times 10^4$ 的正整数），计算这套系统最多能拦截多少导弹，如果要拦截所有导弹最少要配备多少套这种导弹拦截系统。（个数 $\le 10^5$）。

第一问求的是最长不升子序列，非常简单。
  
第二问怎么做？想一想在导弹不停飞来的过程中，我们会尝试用第一套拦截系统去拦截这个导弹，但万一我们拦截不了，就只能再上一套拦截系统。这样需要上的拦截系统就是原序列的 LIS 长度。这就是 **Dilworth 定理**：将一个序列剖成若干个单调不升子序列的最小个数等于该序列最长上升子序列的个数。

这里分别给出 $O(n^2)$ 和 $O(n\log n)$ 的代码：

```cpp
// O(n^2)
#include <iostream>
#include <cstdio>

using namespace std;

int n;
int a[100005];
int f1[100005], f2[100005];

int main(void)
{
    int x;
    while (cin >> x)
        a[++n] = x;
    int ans1 = 0, ans2 = 0;
    for (int i = 1; i <= n; ++i)
    {
        f1[i] = f2[i] = 1;
        for (int j = 0; j < i; ++j)
        {
            if (a[i] <= a[j]) f1[i] = max(f1[j] + 1, f1[i]);
            if (a[i] > a[j]) f2[i] = max(f2[j] + 1, f2[i]);
        }
        ans1 = max(ans1, f1[i]);
        ans2 = max(ans2, f2[i]);
    }
    cout << ans1 << endl << ans2 << endl;
    return 0;
}
```

```cpp
// O(nlogn)
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace std;

int n;
int a[100005], f[100005], g[100005];

int main(void)
{
    int x, ans = 0;
    while (cin >> x) a[++n] = x;
    memset(g, 0x3f, sizeof(g));
    for (int i = n; i >= 1; --i) // 这里求的是最长不降子序列了，倒过来看就是最长不升子序列
    {
        int k = upper_bound(g + 1, g + n + 1, a[i]) - g;
        f[i] = k;
        g[k] = a[i];
        ans = max(ans, f[i]);
    }
    cout << ans << endl;
    memset(g, 0x3f, sizeof(g));
    ans = 0;
    for (int i = 1; i <= n; ++i)
    {
        int k = lower_bound(g + 1, g + n + 1, a[i]) - g;
        f[i] = k;
        g[k] = a[i];
        ans = max(ans, f[i]);
    }
    cout << ans << endl;
    return 0;
}
```

[[NOIP2004 提高组] 合唱队形](https://www.luogu.com.cn/problem/P1091).

这里的前半部分是最长上升子序列，后半部分是反着的最长上升子序列（因为我们的算法定义的是以 $i$ 结尾的最长上升子序列）。只需要分别计算，然后拼起来即可。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
int a[105], f[105], g[105];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    for (int i = 1; i <= n; ++i)
    {
        f[i] = 1;
        for (int j = 0; j < i; ++j)
            if (a[i] > a[j]) f[i] = max(f[i], f[j] + 1);
    }
    for (int i = n; i >= 1; --i)
    {
        g[i] = 1;
        for (int j = n; j > i; --j)
            if (a[i] > a[j]) g[i] = max(g[i], g[j] + 1);
    }
    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = max(ans, f[i] + g[i] - 1);
    printf("%d\n", n - ans);
    return 0;
}
```
{% endfolding %}

{% noteblock guide %}

上述内容可能稍显抽象，这里给出代码供背诵。

{% folding blue::已为您折叠过长的内容 %}
```cpp
// 最长不升（非严格下降）
memset(g, 0xbf, sizeof(g)); // 要找小于（等于）它的，初始化为负无穷
for (int i = 1; i <= n; ++i) 
{
    int k = upper_bound(g + 1, g + n + 1, a[i], greater<int>()) - g; // 找到第一个 a[i] <= g[k]
    f[i] = k;
    g[k] = a[i];
}

// 最长上升（严格上升）
memset(g, 0x3f, sizeof(g));
for (int i = 1; i <= n; ++i) 
{
    int k = lower_bound(g + 1, g + n + 1, a[i]) - g; // 找到第一个 a[i] >= g[k]
    f[i] = k;
    g[k] = a[i];
}

// 最长不降（非严格上升）
memset(g, 0x3f, sizeof(g));
for (int i = 1; i <= n; ++i) 
{
    int k = upper_bound(g + 1, g + n + 1, a[i]) - g; // 找到第一个 a[i] > g[k]
    f[i] = k;
    g[k] = a[i];
}

// 最长下降（严格下降）
memset(g, 0xbf, sizeof(g));
for (int i = 1; i <= n; ++i) 
{
    int k = lower_bound(g + 1, g + n + 1, a[i], greater<int>()) - g; // 找到第一个 a[i] < g[k]
    f[i] = k;
    g[k] = a[i];
}
```
{% endfolding %}

{% endnoteblock %}

#### 最长公共子序列

给定两个序列 $A$ 和 $B$，求它们长度最大的公共子序列（即既是 $A$ 的子序列也是 $B$ 的子序列）的长度。如 `1, 5, 2, 6, 8, 7` 和 `2, 3, 5, 6, 9, 8, 4` 的最长公共子序列（Longest Common Subsequence，LCS）是 `5, 6, 8` 或 `2, 6, 8`。

怎么做呢？设 $f(i,j)$ 为 $A_1,A_2,\cdots,A_i$ 和 $B_1,B_2,\cdots,B_j$ 的 LCS 长度，那么显然有 $f(i,0)=f(0,i)=0$，且：

$$
f(i,j) =\max\begin{cases}
f(i-1,j),&\\
f(i,j-1),&\\
f(i-1,j-1)+1,&A[i]=B[j].
\end{cases}
$$

但是这样的时间复杂度太高了，我们需要用更快的方法。如下：

[模板](https://www.luogu.com.cn/problem/P1439)。

比如：

```
A: 3 2 1 4 5
B: 1 2 3 4 5
```

我们把 `A` 重新编号成有序的：

```
A: a b c d e
B: c b a d e
```

这样标号之后 LCS 显然不变，因为相对元素没变，但是发现如果这个 LCS 想要成立，那么它必须是 `A` 的子序列，而 `A` 又是单调递增的，也就相当于是在 `B` 中的子序列是单调递增的，也就是要求 `B` 的 LIS。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

int n, ans;
int p[100005], a[100005], f[100005], g[100005];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        int x;
        scanf("%d", &x);
        p[x] = i;
    }
    for (int i = 1; i <= n; ++i)
    {
        int x;
        scanf("%d", &x);
        a[i] = p[x];
    }
    memset(g, 0x3f, sizeof(g));
    for (int i = 1; i <= n; ++i)
    {
        int k = lower_bound(g + 1, g + i + 1, a[i]) - g;
        f[i] = k;
        g[k] = a[i];
        ans = max(ans, f[i]);
    }
    printf("%d\n", ans);
    return 0;
}
```

#### [Luogu P1944] 最长括号匹配

[Portal](https://www.luogu.com.cn/problem/P1944).

设 $f(i)$ 为以 $i$ 结束的最长的括号匹配子串，那么 $f(i)=f(i-1)+2+f(i-f(i-1)-2)$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

string s;
int f[1000005];

int main(void)
{
    cin >> s;
    int ans = 0;
    for (int i = 1; i < s.length(); ++i)
    {
        if ((s[i] == ')' && s[i - f[i-1] - 1] == '(') ||
            (s[i] == ']' && s[i - f[i-1] - 1] == '['))
            {
                f[i] = f[i-1] + 2 + (i - 2 - f[i-1] >= 0 ? f[i - 2 - f[i-1]] : 0);
                ans = max(ans, f[i]);
            }
    }
    if (ans == 0) return 0;
    for (int i = 1; i < s.length(); ++i)
        if (f[i] == ans)
        {
            for (int j = i - ans + 1; j <= i; ++j)
                putchar(s[j]);
            break;
        }
    putchar('\n');
    return 0;
}
```
{% endfolding %}

#### [NOIP2000 提高组] 乘积最大

[Portal](https://www.luogu.com.cn/problem/P1018).

设计状态。设 $f[i][j]$ 为前 $i$ 位（不含 $i$）用 $j$ 个乘号的答案，转移也很简单，$f[i][j]=\max\limits_{x<i}\{f[i][j-1]\times a_{[i,x)}\}$。

P.S. 注意！这题的代码细节巨多，一定要按照工程标准编写这道题：处理字符串用左闭右开区间！

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

string dp[45][8];

int n, k;
string a;

inline string get(int x, int y) // 获取子串
{
    string res = "";
    for (int i = x; i < y; ++i)
        res = res + a[i];
    return res;
}

inline string choose(string x, string y)
{
    if (x.length() != y.length()) return x.length() > y.length() ? x : y;
    for (int i = 0; i < x.length(); ++i)
        if (x[i] != y[i]) return x[i] > y[i] ? x : y;
    return x;
}

int p[5005], q[5005], res[5005];
inline string mul(string x, string y)
{
    int lenx = x.length(), leny = y.length();
    for (int i = lenx - 1; i >= 0; --i) p[lenx - i - 1] = x[i] - '0';
    for (int i = leny - 1; i >= 0; --i) q[leny - i - 1] = y[i] - '0';
    memset(res, 0, sizeof(res));
    for (int i = 0; i < lenx; ++i)
        for (int j = 0; j < leny; ++j)
            res[i+j] += p[i] * q[j];
    int len = lenx + leny;
    for (int i = 0; i < len; ++i)
    {
        res[i+1] += res[i] / 10;
        res[i] %= 10;
    }
    while (res[len] == 0 && len >= 0) --len;
    if (len == -1) return "0";
    string ans = "";
    for (int i = 0; i <= len; ++i)
        ans = char(res[i] + '0') + ans;
    return ans;
}

int main(void)
{
    cin >> n >> k >> a;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= k; ++j)
            dp[i][j] = "0";
    for (int i = 1; i <= n; ++i) // 长度为 1，用 0 个乘号
        dp[i][0] = get(0, i);
    for (int i = 2; i <= n; ++i) // 从长度为 2 开始计算
        for (int j = 1; j <= min(k, i - 1); ++j) // i 个数，最多 i-1 个乘号
            for (int pos = j; pos < i; ++pos)
                dp[i][j] = choose(dp[i][j], mul(dp[pos][j-1], get(pos, i)));
    cout << dp[n][k] << endl;
    return 0;
}
```
{% endfolding %}

#### [Luogu P1868] 饥饿的奶牛

[Portal](https://www.luogu.com.cn/problem/P1868).

> 给定 $n$ 个区间，找出不重复的区间，使得区间总长度尽量的长。

根据数据范围和经验，设 $f(i)$ 代表前 $i$ 的长度序列的最大答案，那么显然有对于区间 $(j,i)$：

$$
f(i) = \max(f(i), \max(f(j-1)+j-i+1))
$$

代码如下：

{% folding cyan, 查看代码 %}
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

int n;
int f[3000005];
vector <int> a[3000005];

int main(void)
{
    n = read();
    for (int i = 1; i <= n; ++i)
    {
        int l = read(), r = read();
        a[r].push_back(l);
    }
    for (int i = 1; i <= 3000000; ++i)
    {
        f[i] = f[i - 1];
        for (auto x : a[i])
            f[i] = max(f[i], f[x - 1] + i - x + 1);
    }
    printf("%d\n", f[3000000]);
    return 0;
}
```
{% endfolding %}

### 多维结构上的动态规划

它们本身就是高维问题。

#### [NOIP2002 普及组] 过河卒

[Portal](https://www.luogu.com.cn/problem/P1002).

设 `ans[i][j]` 代表走到 $(i,j)$ 的方案数，有 $ans(i,j)=ans(i−1,j)+ans(i,j−1)$，具体如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#define ll long long

using namespace std;

const int h[9][2] = {{0, 0}, {1, 2}, {2, 1}, {-1, 2}, {2, -1}, {-1, -2}, {-2, -1}, {1, -2}, {-2, 1}};
bool flag[25][25];
ll ans[25][25];

int main(void)
{
    int n, m, x, y;
    cin >> n >> m >> x >> y;
    for (int i = 0; i < 9; i++)
    {
        int tx = x + h[i][0], ty = y + h[i][1];
        if (tx >= 0 && tx <= n && ty >= 0 && ty <= m)
            flag[tx][ty] = 1;
    }
    if (flag[0][0] == 1)
    {
        puts("0");
        return 0;
    }
    ans[0][0] = 1;
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++)
        {
            if (flag[i][j]) continue;
            if (i != 0) ans[i][j] += ans[i - 1][j];
            if (j != 0) ans[i][j] += ans[i][j - 1];
        }
    cout << ans[n][m] << '\n';
    return 0;
}
```
{% endfolding %}

#### [NOIP2000 提高组] 方格取数

[Portal](https://www.luogu.com.cn/problem/P1004).

走两回相当于两个人同时走（常见套路），那么定义 `dp[i][j][k][l]` 为其中一人向右走 $i$，向下走 $j$；另一人向右走 $k$，向下走 $l$。

**当 $i+j=k+l$ 时**，转移：

$$dp[i][j][k][l]=gold+\max \begin{cases}
dp[i-1][j][k-1][l],\\
dp[i-1][j][k][l-1],\\
dp[i][j-1][k-1][l],\\
dp[i][j-1][k][l-1].\\
\end{cases}$$

其中 $gold$ 指当前能获得的分数。

可以证明这一设计没有后效性，满足最优子结构。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
int a[15][15];
int dp[15][15][15][15];

int main(void)
{
    scanf("%d", &n);
    int x, y, z;
    while (scanf("%d%d%d", &x, &y, &z) == 3 && x)
        a[x][y] = z;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            for (int k = 1; k <= n; ++k)
                for (int l = 1; l <= n; ++l)
                    if (i + j == k + l) // 实际上是 i+j-2 == k+l-2，这是 dp 的前提
                    {
                        #define f dp[i][j][k][l]

                        f = max(f, dp[i-1][j][k-1][l]);
                        f = max(f, dp[i-1][j][k][l-1]);
                        f = max(f, dp[i][j-1][k-1][l]);
                        f = max(f, dp[i][j-1][k][l-1]);
                        f += a[i][j] + a[k][l];
                        if (i == k && j == l) f -= a[i][j]; // 想一想，为什么

                        #undef f
                    }
    printf("%d\n", dp[n][n][n][n]);
    return 0;
}
```
{% endfolding %}

还可以写一下 [[NOIP2008 提高组] 传纸条](https://www.luogu.com.cn/problem/P1006) 加以巩固。

## Problemset

动态规划的实践性很强，需要做大量的题目。这里会给出一些。为了更好的解决一下题目，请先了解背包问题的基础概念部分（仅第 $1$ 节即可）：

{% link 背包 DP::/2022/b2626942/ %}

### 简单题

这里的问题比较简单。

#### [ABC261D] Flipping and Bonus

[Portal](https://atcoder.jp/contests/abc261/tasks/abc261_d).

当前的状态由 $f[i,j]$ 来定义，表示翻到了第 $i$ 个硬币，分数为 $j$ 时所获得的最大钱数，没有后效性。

转移很简单：要么是正面，要么反面，对应即可（详见代码）。满足最优子结构。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar();
    return x;
}

int n, m;
int x[5005], y[5005];
i64 f[5005][5005], ans;

int main(void)
{
    n = read(), m = read();
    for (int i = 1; i <= n; ++i) x[i] = read();
    for (int i = 1; i <= m; ++i)
    {
        int c = read();
        y[c] = read();
    }
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 0; j < i; ++j) // 注意最多拿 i-1 分
            f[i][0] = max(f[i][0], f[i - 1][j]); // f[i][0] 是从 f[i-1][x] 中的任意一个不选反面的
        for (int j = 1; j <= i; ++j)
            f[i][j] = f[i - 1][j - 1] + x[i] + y[j]; // 一定是选正面了
    }
    for (int i = 0; i <= n; ++i)
        ans = max(ans, f[n][i]);
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1280] 尼克的任务

[Portal](https://www.luogu.com.cn/problem/P1280).

设 $f(i)$ 代表从 $i$ 时间开始能获得的最多休息时间，那么：

$$
f(i) = \begin{cases}
f(i+1)+1,\\
\max\{f(i+t_j)\}.
\end{cases}
$$

这个时刻有任务采用第一种转移，否则采用第二种。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n, k, f[10005];
vector <int> v[10005];

int main(void)
{
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= k; ++i)
    {
        int p, t;
        scanf("%d%d", &p, &t);
        v[p].push_back(t);
    }
    for (int i = n; i >= 1; --i)
    {
        if (v[i].size() == 0) f[i] = f[i + 1] + 1;
        else
        {
            for (int j = 0; j < v[i].size(); ++j)
                f[i] = max(f[i], f[i + v[i][j]]);
        }
    }
    printf("%d\n", f[1]);
    return 0;
}
```
{% endfolding %}

#### [UVa 1025] A spy in the Metro

[Portal](https://www.luogu.com.cn/problem/UVA1025).

影响决策的只有当前时间和所处的车站。设 $f(i,j)$ 表示时刻 $i$，位于车站 $j$，最少需要等待的时间。有如下三种决策：

- 等 $1$ 分钟；
- 乘左边来的车（如果存在）；
- 乘右边来的车（如果存在）。

代码如下：

{% folding cyan::查看代码 %}
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

int n, T, m1, m2, kase = 0;
int t[55];
bool has_train[205][55][2];
int f[205][55];

int main(void)
{
    while (scanf("%d", &n) == 1 && n)
    {
        memset(f, 0x3f, sizeof(f));
        memset(has_train, 0, sizeof(has_train));
        f[0][1] = 0;
        
        T = read();
        for (int i = 1; i < n; ++i) t[i] = read();
        m1 = read();
        for (int i = 1; i <= m1; ++i) 
        {
            int x = read();
            for (int j = 1; j <= n; ++j)
            {
                has_train[x][j][0] = true;
                x += t[j];
                if (x > T) break;
            }
        }
        m2 = read();
        for (int i = 1; i <= m2; ++i) 
        {
            int x = read();
            for (int j = n; j >= 1; --j)
            {
                has_train[x][j][1] = true;
                x += t[j-1];
                if (x > T) break;
            }
        }
        
        for (int i = 1; i <= T; ++i)
            for (int j = 1; j <= n; ++j)
            {
                f[i][j] = f[i-1][j] + 1;
                if (j > 1 && has_train[i][j][0] && i >= t[j - 1])
                    f[i][j] = min(f[i][j], f[i - t[j - 1]][j - 1]);
                if (j < n && has_train[i][j][1] && i >= t[j])
                    f[i][j] = min(f[i][j], f[i - t[j]][j + 1]);
            }
        printf("Case Number %d: ", ++kase);
        if (f[T][n] >= 0x3f3f3f3f) puts("impossible");
        else printf("%d\n", f[T][n]);
    }
    return 0;
}
```
{% endfolding %}

#### [UVA10943] How do you add?

[Portal](https://www.luogu.com.cn/problem/UVA10943).

设 $f(i,k)$ 代表将 $i$ 划分为 $k$ 个数。直接暴力转移即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int MOD = 1000000;

int n, k;
int f[105][105];

int main(void)
{
    for (int i = 0; i <= 100; ++i)
    {
        f[i][1] = 1;
        for (int j = 0; j <= 100; ++j)
            for (int k = 0; k <= i; ++k)
                f[i][j] = (f[i][j] + f[i - k][j - 1]) % MOD;
    }
    while (scanf("%d%d", &n, &k) == 2 && n) printf("%d\n", f[n][k]);
    return 0;
}
```
{% endfolding %}

### 较复杂内容

从这里开始的题目会比较有趣。

#### [Luogu P4310] 绝世好题

[Portal](https://www.luogu.com.cn/problem/P4310).

按照求解 LIS 问题的经验，很容易设计出一个 $O(n^2)$ 的暴力 DP。由于 $b_i\&b_{i-1}\ne 0$，也就是说在二进制意义下至少有一位相同。设 $f(i)$ 代表以在二进制中第 $i$ 位为 $1$ 的数结尾的满足题意的子序列长度，那么扫描到一个 $a_i$，就可以计算以 $a_i$ 结尾的最长子序列长度。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, ans = 0;
int a[100005], f[32]; // f[i] 代表以在二进制中第 i 位为 1 的数结尾的满足题意的子序列长度

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", a + i);
        int k = 1; // k 代表以 a[i] 结尾的最长子序列长度
        for (int j = 0; j <= 30; ++j)
            if ((1 << j) & a[i]) k = max(k, f[j] + 1);
            // a[i] 的第 j 位是 1，a[i] 可以与第 j 位为 1 的数结尾的子序列连起来
        for (int j = 0; j <= 30; ++j)
            if ((1 << j) & a[i]) f[j] = max(f[j], k);
            // a[i] 的第 j 位是 1，f[j] 对应的子序列可以以 a[i] 结尾
        ans = max(ans, k);
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1385] 密令

[Portal](https://www.luogu.com.cn/problem/P1385).

可以发现一条神奇的性质：不论怎样，这个字符串的字符和永远不变，而且任意和相等的字符串都可以变出来。

证明：采用构造方法。我们从前往后构造新的字符串，考虑第一个字符，如果比目标大就用 $(−1, +1)$ 变换，比目标小就用 $(+1,−1)$ 变换。然后去搞第二个字符……以此类推，直到搞完前 $n − 1$ 位。至于最后一位，我们断言它此时必定等于目标串的最后一位。这是因为两种变换均不会改变字母和 $sum$。

于是整个问题被简化为：给定 $sum$，有多少种长度为 $n$ 的序列满足：

- 每个元素在 $[1, 26]$ 之间；   
- 序列和为 $sum$。

设 $dp[k][x]$ 表示长度为 $k$ 的序列之和为 $x$ 的方案数，答案显然是 $dp[n][sum]$。转移是显然的：

$$
dp[k][x] = \sum\limits_{1\le i\le\min\{26,x\}}dp[k − 1][x − i]
$$

我们不需要惧怕多组询问，预处理之后就能直接出答案。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <string>

#define MOD 1000000007

using namespace std;

char s[105];
int f[105][3000];

int main(void)
{
    for (int i = 0; i <= 26; ++i) f[1][i] = 1;
    for (int i = 2; i <= 102; ++i)
    {
        for (int j = 1; j <= 2602; ++j)
            for (int k = 1; k <= 26; ++k)
                if (j > k) f[i][j] = (f[i][j] + f[i-1][j-k]) % MOD;
    }
    int T;
    scanf("%d", &T);
    while (T--)
    {
        int sum = 0;
        string s;
        cin >> s;
        for (int i = 0; i < s.length(); ++i) sum += s[i] - 'a' + 1;
        cout << (f[s.length()][sum] - 1) % MOD << '\n';
    }
    return 0;
}
```
{% endfolding %}

#### [Codeforces 1061C] Multiplicity

[Portal](https://codeforces.com/problemset/problem/1061/C).

> 从序列 $\{a_1, a_2,\cdots,a_n\}$ 中选出非空子序列 $\{b_1,b_2,\cdots,b_k\}$，一个子序列合法需要满足 $\forall\ i \in [1,k],i\mid b$。求有多少互不相等的合法子序列，答案对 $10^9+7$ 取模。

由于人类尚不知道素数分布规律之类的内容，所以 OI 中的数学题很多都建立在枚举之上。这道题就是一个典型的例子。

往往线性 DP 的状态设计都很暴力，求什么就什么。我们令 $f[i,j]$ 代表前 $i$ 个数中选择 $j$ 个数的方案数。显然有：

$$
f[i][j] = \begin{cases}
f[i-1][j]+f[i-1][j-1]&,j\mid a[i]\\
f[i-1][j]
\end{cases}
$$

但是这样显然是 TLE + MLE。如何优化？由于 $j\mid a[i]$，我们可以不枚举 $j$ 而是枚举 $a[i]$ 的约数（方法。同时我们还可以把第一维给滚掉。

但是枚举约数时，由于转移必须要有顺序，所以我们需要排个序。滚动时也要注意，由于我们需要 $(i-1,j-1)$ 的内容，所以需要倒序枚举。因为我们需要的是上一阶段前面的解，第二种转移已经不需要了。

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>

#define MOD 1000000007

using namespace std;

inline int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n;
int a[100005];
int dp[1000005];

int main(void) {
    n = read();
    dp[0] = 1;
    for (int i = 1; i <= n; ++i) a[i] = read();

    for (int i = 1; i <= n; ++i) {
        vector <int> v; // 先求约数
        for (int j = 1; j * j <= a[i]; ++j) {
            if (a[i] % j == 0) {
                v.push_back(j);
                if (j * j != a[i]) v.push_back(a[i] / j);
            }
        }
        sort(v.begin(), v.end()); // 排序
        for (int j = min(int(v.size() - 1), i); j >= 0; --j)
            dp[v[j]] = (dp[v[j] - 1] + dp[v[j]]) % MOD;
    }

    int ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = (ans + dp[i]) % MOD;
    cout << ans << endl;
    return 0;
}
```
{% endfolding %}

#### [Luogu P3861] 拆分

[Portal](https://www.luogu.com.cn/problem/P3861).

> 给定一个整数 $n(n\le 10^{12})$，求将 $n$ 分解为互不相同的不小于 $2$ 的数的乘积的方案数。答案模 $998244353$。

预处理出 $n$ 的约数数组 $d$，以及用 $pos[i]$ 代表约数 $i$ 在 $d$ 中的位置。由于约数很大，所以如果 $i>\sqrt{n}$，就用 $n\div i$ 代表其位置。设 $f(i,j)$ 代表将 $d_i$ 拆分成不大于 $d_j$ 的约数个数方案数，转移方程不难写出。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;
typedef long long i64;
const int MOD = 998244353;

int tot, pos1[1000005], pos2[1000005], f[6721][6721];
i64 n, d[7000];

int main(void) {
    int T; cin >> T; 
    while (T--) {
        tot = 0;
        cin >> n;
        int sqrtn = sqrt(n);
        for (i64 i = 1; i * i <= n; ++i)
            if (n % i == 0) {
                d[++tot] = i;
                if (i * i != n) d[++tot] = n / i;
            }
        sort(d + 1, d + tot + 1);
        for (int i = 1; i <= (tot + 1) / 2; ++i)
            pos1[d[i]] = i, pos2[d[i]] = tot - i + 1;
        for (int i = 1; i <= tot; ++i) {
            f[i][1] = (i == 1 ? 1 : 0);
            for (int j = 2; j <= tot; ++j) {
                f[i][j] = f[i][j - 1];
                if (d[i] % d[j] == 0) {
                    i64 tmp = d[i] / d[j];
                    i64 pos = tmp <= sqrtn ? pos1[tmp] : pos2[n / tmp];
                    f[i][j] = (f[i][j] + f[pos][j - 1]) % MOD;
                }
            }
        }
        printf("%d\n", f[tot][tot] - 1);
    }
    return 0;
}
```
{% endfolding %}

#### \[Codeforces 10D\] LCIS

[Portal](https://codeforces.com/problemset/problem/10/D).

> 求最长公共上升子序列的长度，并给出解。

根据之前求解 LIS 与 LCS 的经验，我们可以得出这样一个做法：

{% noteblock blue %}

设 $f(i,j)$ 表示 $A_1\sim A_i$ 与 $B_1\sim B_j$ 的 LCIS 长度。这样行吗？不行！因为我们不知道 LCIS 结尾的数是什么，无法转移，有后效性。那么我们就规定以 $B_j$ 结尾（$B_j$ 也等于 $A_i$）。

- 当 $A_i \ne B_j$ 时，有 $f(i,j)=f(i-1,j)$；
- 当 $A_i = B_j$ 时，$f(i,j)=\max\limits_{0\le k<j,B_k<B_j}\{f(i-1,k)\}+1$。

代码便不难写出（打印解的方法将在下文介绍）。

```cpp
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
        if (a[i] == b[j])
        {
            for (int k = 0; k < j; ++k)
                if (b[k] < b[j]) f[i][j] = max(f[i][j], f[i-1][k] + 1);
        }
        else f[i][j] = f[i-1][j];
```

{% endnoteblock %}

这样的时间复杂度是 $\mathcal{O}(n^3)$，足以通过原题，但是我们还可以做得更好。$B_k<B_j$ 相当于 $B_k<A_i$。随着 $i$ 的增大，$k$ 可以取的值只多不少。一旦来了个更好的决策，我们只需保留它即可。综上，我们只需要动态更新决策。代码如下：

```cpp
for (int i = 1; i <= n; ++i)
{
    int val = 0; // val 代表之前代码中“f[i-1][k]“的最大值，一开始为 0（前提是序列中的数都是正数，否则它要为 —INF）
    if (b[0] < a[i]) val = f[i-1][0];
    for (int j = 1; j <= m; ++j)
    {
        if (a[i] == b[j]) f[i][j] = val + 1;
        else f[i][j] = f[i-1][j];
        if (b[j] < a[i]) val = max(val, f[i-1][j]);
        // 这个 j 相当于 k，相当于原始代码中的 f[i-1][k]
    }
}
```

现在的问题就成了如何打印解，这里给出 DP 打印解的一般方式：记录每个状态从哪里来，然后用递归输出。完整代码如下：

{% folding cyan::查看代码 %}
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

int n, m;
int a[505], b[505];
int f[505][505], g[505][505]; // g[i][j] 记录长度为 i 的序列选取的第 j 个数的编号从哪个编号转移过来（在 b 中的编号）

void print(int s) // 递归输出
{
    if (s == 0) return; // 回到了 0，也就是什么都没有
    print(g[n][s]);
    printf("%d ", b[s]);
}

int main(void)
{
    n = read();
    for (int i = 1; i <= n; ++i) a[i] = read();
    m = read();
    for (int i = 1; i <= m; ++i) b[i] = read();
    for (int i = 1; i <= n; ++i)
    {
        int t = 0; // 记录最后选取的数的编号（与之前不同）
        for (int j = 1; j <= m; ++j)
        {
            if (a[i] == b[j])
            {
                f[i][j] = f[i-1][t] + 1; // 将这个编号对应的 f[i-1][j] 进行转移
                g[i][j] = t; // 从 t 来
            }
            else
            {
                f[i][j] = f[i-1][j];
                g[i][j] = g[i-1][j]; // 从 g[i-1][j] 对应的编号来
            }
            if (b[j] < a[i]) t = j; // 编号可以取 j
        }
    }
    // 注意以下，由于状态定义了结尾，所以要取最大值
    int s = 0;
    for (int i = 1; i <= m; ++i)
        if (f[n][i] > f[n][s]) s = i;
    printf("%d\n", f[n][s]);
    print(s); putchar('\n');
    return 0;
}
```
{% endfolding %}

#### [NOIP2005 提高组] 过河

[Portal](https://www.luogu.com.cn/problem/P1052).

当 $L$ 比较小，直接暴力 DP 即可。当 $L$ 比较大怎么办呢？发现其实 $S,T,M$ 都很小，实际上当两个石子的间隔超过 $S\times T$ 之后，都是可以跳过去的，因此可以根据此压缩状态。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

int L;
int S, T, M;
int A[105], stone[105];
bool flag[4000005];
int f[4000005];

int main(void)
{
    memset(f, 0x7f, sizeof(f));
    scanf("%d%d%d%d", &L, &S, &T, &M);
    for (int i = 1; i <= M; ++i)
        scanf("%d", stone + i);
    sort(stone + 1, stone + M + 1);
    if (S == T) // 特判
    {
        int ans = 0;
        for (int i = 1; i <= M; ++i)
            if (stone[i] % S == 0) ++ans;
        printf("%d\n", ans);
        return 0;
    }
    for (int i = 1; i <= M; ++i)
    {
        int d = stone[i] - stone[i - 1];
        A[i] = A[i - 1] + min(d, S * T); // 在上一个石子的基础上
        flag[A[i]] = true;
    }
    L = A[M] + S * T; // 最远跳这么远
    f[0] = 0;
    for (int i = 1; i <= L; ++i)
        for (int j = S; j <= T; ++j)
            if (i >= j) f[i] = min(f[i], f[i - j] + flag[i]);
    int ans = 1e9;
    for (int i = A[M]; i <= L; ++i)
        ans = min(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Codeforces 41D] Pawn

[Portal](https://www.luogu.com.cn/problem/CF41D).

由于规模很小，所以状态可以设计的暴力一点。不知道倍数无法进行转移，那么就将对 $k+1$ 取模后的结果加入状态。

本题需要打印解，我们只需要在更新的时候看一下这个状态从哪里来的就可以了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, m, k;
char s[105];
int a[105][105];
int f[105][105][15];
char d[105][105][15];

void dfs(int x, int y, int back)
{
	if (x == n) return printf("%d\n", y), void();
	if (d[x][y][back] == 'R') dfs(x + 1, y - 1, ((back - a[x][y]) % k + k) % k);
	else dfs(x + 1, y + 1, ((back - a[x][y]) % k + k) % k);
	putchar(d[x][y][back]);
}

int main(void)
{
	scanf("%d%d%d", &n, &m, &k);
	++k;
	memset(f, 0xff, sizeof(f));
	for (int i = 1; i <= n; ++i)
	{
		scanf("%s", s + 1);
		for (int j = 1; j <= m; ++j)
			a[i][j] = int(s[j] - '0'); 
	}
	for (int i = 1; i <= m; ++i) f[n][i][a[n][i] % k] = a[n][i];
	for (int i = n - 1; i >= 1; --i)
		for (int j = 1; j <= m; ++j)
			for (int x = 0; x < k; ++x)
			{
				int back = ((x - a[i][j]) % k + k) % k;
				int &dp = f[i][j][x], &val = a[i][j];
				#define check(x) (x != -1 && x + val > dp)
				if (j > 1 && check(f[i + 1][j - 1][back])) 
				{
					d[i][j][x] = 'R';
					f[i][j][x] = f[i + 1][j - 1][back] + val;
				}
				if (j < m && check(f[i + 1][j + 1][back])) 
				{
					d[i][j][x] = 'L';
					f[i][j][x] = f[i + 1][j + 1][back] + val;
				}
			}
	int ans = -1, pos;
	for (int i = 1; i <= m; ++i)
		if (f[1][i][0] > ans) ans = f[1][i][0], pos = i;
	printf("%d\n", ans);
	if (ans != -1) dfs(1, pos, 0);
	return 0;
}
```
{% endfolding %}

#### [UVA116] Unidirectional TSP

[Portal](https://www.luogu.com.cn/problem/UVA116).

由于需要打印字典序最小的解，所以设 $f(i,j)$ 代表从 $(i,j)$ 出发的最小代价比较方便（字典序是从前往后比的）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

int m, n;
int a[15][105];
int f[15][105], nxt[15][105];

int main(void) {
    while (scanf("%d%d", &m, &n) == 2) {
        for (int i = 1; i <= m; ++i)
            for (int j = 1; j <= n; ++j)
                scanf("%d", &a[i][j]);
        int ans = 1e9, first;
        for (int j = n; j >= 1; --j)
            for (int i = 1; i <= m; ++i) {
                if (j == n) f[i][j] = a[i][j];
                else {
                    int rows[3] = {i, (i == 1 ? m : i - 1), (i == m ? 1 : i + 1)};
                    sort(rows, rows + 3);
                    f[i][j] = 1e9;
                    for (int k = 0; k < 3; ++k) {
                        int v = f[rows[k]][j + 1];
                        if (v < f[i][j]) {
                            f[i][j] = v;
                            nxt[i][j] = rows[k];
                        }
                    }
                    f[i][j] += a[i][j];
                }
                if (j == 1 && f[i][j] < ans) ans = f[first = i][j];
            }
        printf("%d", first);
        for (int i = nxt[first][1], j = 2; j <= n; i = nxt[i][j], ++j)
            printf(" %d", i);
        printf("\n%d\n", ans);
    }
    return 0;
}
```
{% endfolding %}

#### [UVA1625] Color Length

[Portal](https://www.luogu.com.cn/problem/UVA1625).

状态直接 $f(i,j)$ 表示当前取到的位置，用的最小代价。现在就是要搞清楚代价。代价需要预处理好每一个字符首次出现和最后出现的位置，在扫描的时候维护，把“已经出现但尚未结束”的值 +1。可以使用滚动数组优化空间。

扫描时维护一个 $c$ 数组，代表当前新增的代价。正常来讲新增的代价是不变的，当有新的字母出现时，代价就增加 $1$，一个字母终结时，代价就 $-1$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
 
using namespace std;
const int INF = 1e9;
 
int n, m;
char a[5005], b[5005];
int f[2][5005], c[2][5005];
int sa[26], sb[26], ea[26], eb[26];
 
int main(void) {
    int T; scanf("%d", &T);
    while (T--) {
        scanf("%s%s", a + 1, b + 1);
        n = strlen(a + 1), m = strlen(b + 1);
        memset(sa, 0x3f, sizeof(sa));
        memset(sb, 0x3f, sizeof(sb));
        memset(ea, 0, sizeof(ea));
        memset(eb, 0, sizeof(eb));
        memset(c, 0, sizeof(c));
        memset(f, 0, sizeof(f));
        for (int i = 1; i <= n; ++i) {
            a[i] -= 'A';
            sa[a[i]] = min(sa[a[i]], i);
            ea[a[i]] = i;
        }
        for (int i = 1; i <= m; ++i) {
            b[i] -= 'A';
            sb[b[i]] = min(sb[b[i]], i);
            eb[b[i]] = i;
        }
 
        int t = 0;
        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <= m; ++j) {
                if (!i && !j) continue;
 
                f[i & 1][j] = min(i ? (f[(i - 1) & 1][j] + c[(i - 1) & 1][j]) : INF, (j ? (f[i & 1][j - 1] + c[i & 1][j - 1]) : INF));
 
                if (i) {
                    c[i & 1][j] = c[(i - 1) & 1][j];
                    if (sa[a[i]] == i && sb[a[i]] > j) ++c[i & 1][j]; // 首个 a[i]
                    if (ea[a[i]] == i && eb[a[i]] <= j) --c[i & 1][j]; // a[i] 完全结束
                } else if (j) {
                    c[i][j] = c[i][j - 1];
                    if (sb[b[j]] == j && sa[b[j]] > i) ++c[i & 1][j];
                    if (eb[b[j]] == j && ea[b[j]] <= i) --c[i & 1][j];
                }
            }
        }
        printf("%d\n", f[n & 1][m]);
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P1412] 经营与开发

[Portal](https://www.luogu.com.cn/problem/P1412).

正序很难做，但是倒序可以。设 $f(i)$ 代表从 $i$ 开始的最大收入，转移不难写出。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, type[100005], a[100005];
double k, c, w, f[100005];

int main(void) {
	cin >> n >> k >> c >> w;
	for (int i = 1; i <= n; ++i)
		scanf("%d%d", &type[i], &a[i]);
	for (int i = n; i >= 1; --i) {
		if (type[i] == 1) f[i] = max(f[i + 1], f[i + 1] * (1 - 0.01 * k) + a[i]);
		else f[i] = max(f[i + 1], f[i + 1] * (1 + 0.01 * c) - a[i]);
	}
	printf("%.2lf\n", f[1] * w);
	return 0;
}
```
{% endfolding %}

## 小结

动态规划的做法需要在实践中积累经验。可以看出，线性 DP 的状态设计和转移往往都比较暴力，没有太多技巧可言。但细节很多，综合性强。学完线性 DP 后，接下来可以阅读背包问题的剩余内容：

{% link 背包 DP::/2022/b2626942/ %}
