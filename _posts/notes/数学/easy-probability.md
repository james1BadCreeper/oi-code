---
title: 简单概率论
cover: false
plugins:
    - katex
abbrlink: ccbe608c
date: 2022-09-01 19:05:51
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 概率
    - 期望
    - 数学
headimg: https://pic.imgdb.cn/item/6312b70216f2c2beb17ef8d5.jpg
group: notes
hide: true
status: working
---

> 概率论研究的是随机事件，本文将简单介绍概率论。

<!--more-->

## 事件

这是一些基本概念。

### 随机事件与样本空间

**随机试验**指的是在相同的条件下，对某个随机现象进行的大量重复观测。随机试验可以在相同的条件下重复进行，出现的所有可能结果不止一个，但都已知（比如抛一枚无限薄但有一定质量硬币一定是正面朝上或者反面朝上），每次实验总是出现可能结果之一，但实验前无法预知得到哪一种结果[^1]。

我们把每一种可能，比如抛硬币出现正面，称之为**基本事件**，或者**样本点**，用 $A,B,C\dots$ 表示，全体样本点构成的集合称为**样本空间**，用 $\Omega$ 来表示（有的时候也是用 $S$）。

样本空间可以是有限集，也可以是无限集。比如抛硬币时 $S=\{正，反\}$，是有限集；选取一个 $[0,1]$ 中的实数时，$S=\{x\in\mathbb{R}\mid 0\le x\le 1\}$，就是一个无限集。

**随机事件** $A$ 是 $\Omega$ 的一个子集，当 $A$ 中的某个基本事件发生的时候，我们称 $A$ 事件发生。

- 若 $A=\Omega$，则称 $A$ 是必然事件，比如 `james1` 考试得零分；
- 若 $A=\varnothing$，则称 $A$ 是不可能事件，比如 `james1` 的智商为正数。

实际上上述例子不太准确{% emoji 滑稽 %}，看看就好。

### 事件的运算

由于随机事件是集合，所以它们的运算跟集合运算大致同理。

- 若 $A\subseteq B$，则 $A$ 发生时 $B$ 一定发生。
- 若 $A=B$，则它们包含的样本点是相同的；
- 几个不同的随机事件也会同时发生，发生的情况就是它们的交集，即 $A\cap B$，简记为 $AB$。
- 若 $A\cap B=\varnothing$，那么 $A,B$ 不可能同时发生，也就是说 $A,B$ 是**互斥事件**，或称“互不相容事件“。$n$ 个随机事件互斥的充要条件是任意两个随机事件互斥，此时有 $P(A+B)=P(A)+P(B)$，被称为**互斥事件的概率加法公式**。
- $A\cup B$ 表示 $A,B$ 至少有一个发生，简记为 $A+B$。
- $A$ 发生，$B$ 不发生记为 $A-B$。
- 由 $A$ 不发生所构成的事件，成为 $A$ 的**对立事件**，也就是说 $A$ 的对立事件 $\overline{A}=\complement_{\Omega}A$。
- $A\overline{A}=\varnothing,A+\overline{A}=S,\overline{\overline{A}}=A$。
- **德·摩根定律**，$\overline{A+B}=\overline{A}~\overline{B},\overline{AB}=\overline{A}+\overline{B}$。分别指如果 $A$ 或 $B$ 不发生，那么 $A,B$ 同时不发生；如果 $A$ 且 $B$ 不发生，那么只需要 $A$ 不发生或者 $B$ 不发生即可，具体可以通过 Venn 图来理解。

## 概率模型

概率要来啦！

### 古典概型

当每一个样本点只有有限个基本结果，每个基本结果出现的可能性是一样的，那么：

$$
P(A)=\cfrac{\text{card}(A)}{\text{card}(S)}
$$

> **生日悖论**。一年有 $365$ 天，每个人的生日完全随机，有 $30$ 位同学，问”事件 $A$（存在两个同学生日相同）“的概率。

我们可以求 $1-\overline{A}$，就可以得出答案。那么利用乘法原理：

$P\left(\overline{A}\right)=\cfrac{A_{365}^{30}}{\prod_{i=1}^{30}365}=\cfrac{365\times \cdots \times336}{365\times\cdots\times 365}\approx 0.29368$，也就是说 $P(A)\approx 70\%$！

### 古典概型的应用

我们来看一些简单的题目。

#### [Luogu P2719] 搞笑世界杯

[Portal](https://www.luogu.com.cn/problem/P2719).

$card(S)$ 很好求，就是 $C_{2n}^{n}$。但是计算最末尾两个人拿到相同球票的概率较难（因为不一定需要抛硬币了），考虑计算不同的。也就是说，我们要求出“到最后两人时，两类门票都没有被卖空“的概率。满足条件的事件共有 $C_{2n-2}^{n-1}$ 种，每种事件发生的概率是 $\cfrac{1}{2^{2n-2}}$（因为一定需要抛硬币），也就是 $C_{2n-2}^{n-1}\times\cfrac{1}{2^{2n-2}}$。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
double ans = 1.0;

int main(void)
{
    scanf("%d", &n);
    n >>= 1;
    for (int i = 1; i < n; ++i)
        ans = ans * (i + n - 1) / (i << 2);
    printf("%.4lf\n", 1 - ans);   
    return 0;
}
```

#### [UVa 1636] Headshot

[Portal](https://www.luogu.com.cn/problem/UVA1636).

直接抠一枪没有子弹的概率是 `00` 的个数除以 `0` 的个数，转一下没有子弹的概率是 `0` 的个数初一总数。

{% folding cyan::查看代码 %}
```cpp
#include <cstdio>
#include <cstring>

int main(void) {
    static char s[120];
    while (scanf("%s", s) == 1) {
        int a = 0, b = 0, n = strlen(s);
        for (int i = 0; i < n; ++i)
            if (s[i] == '0') {
                ++b;
                if (s[(i + 1) % n] == '0') ++a;
            }
        // a / b ? b / n
        if (a * n == b * b) puts("EQUAL");
        else if (a * n > b * b) puts("SHOOT");
        else puts("ROTATE");
    }
    return 0;
}
```
{% endfolding %}

#### [UVa 10491] Cows and Cars

[Portal](https://www.luogu.com.cn/problem/UVA10491).

直接分刚开始抓到牛和没抓到牛简单讨论即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int main(void)
{
    int a, b, c;
    while (scanf("%d%d%d", &a, &b, &c) == 3)
        printf("%.5lf\n", double(a * b + b * b - b) / (double(a + b) * (a + b - c - 1)));
        // a / (a + b) * b / (a + b - c - 1)
        // b / (a + b) * (b - 1) / (a + b - c - 1)
        // a * b + b * b - b
    return 0;
}
```
{% endfolding %}

## 期望

我们讨论的均是离散型随机变量。

### 概念

抛 $100$ 次硬币，期望有 $50$ 枚朝上。数学期望指的就是每次实验中期望的结果。形式化地，若随机变量 $X$ 的取值有 $x_1\cdots x_n$，那么 $E(X)=\sum p_i x_i$，前提是这个式子**绝对收敛**。

### 期望的性质

期望是一个线性函数，也就是说 $E(aX+bY)=a\times E(X)+b\times (Y)$。这个东西很有用。比如 `james1` 要抛骰子，抛两个六面骰子，三个八面骰子{% emoji tong ... %}，那么六面骰子的期望抛出值为 $E(X)=3.5$，八面骰子的期望抛出值为 $E(Y)=4.5$，那么 $E(2X+3Y)=2E(X)+3E(Y)=20.5$。

### 数学期望 DP

数学期望的线性性质非常重要，是我们对它进行 DP 的前提。

#### [Luogu P4316] 绿豆蛙的归宿

[Portal](https://www.luogu.com.cn/problem/P4316).

{% noteblock %}
给出张 $n$ 个点 $m$ 条边的有向无环图，起点为 $1$，终点为 $n$，每条边都有一个长度，并且从起点出发能够到达所有的点，所有的点也都能够到达终点。

绿豆蛙从起点出发，走向终点。 到达每一个顶点时，如果该节点有 $k$ 条出边，绿豆蛙可以选择任意一条边离开该点，并且走向每条边的概率为 $\frac{1}{k}$ 。现在绿豆蛙想知道，从起点走到终点的所经过的路径总长度期望是多少？
{% endnoteblock %}

设 $f(x)$ 表示从 $x$ 走到终点的路径期望长度。等等，为什么不是从 $1$ 到 $x$ 呢？想想转移的时候要乘上概率，如果用正的状态定义，我们不知到有多大的概率走到 $x$，这样会使得计算变的相当困难。采用倒序的话，就不存在这种问题了。事实上，很多期望 DP 都是**倒序进行**的。

那么：

$$
f(x)=\frac{1}{k}\sum_{i=1}^{k}(F[y]+z)
$$

这样我们建反图，在上面进行 DP。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

struct edge {
    int v, d;
    edge(int v = 0, int d = 0) :
        v(v), d(d) {}
};

int n, m;
int in[100005], deg[100005];
double dis[100005];
vector <edge> G[100005];
queue <int> q;

int main(void) {
    scanf("%d%d", &n, &m);
    while (m--) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        G[v].push_back(edge(u, w));
        ++deg[u], ++in[u]; // 进入 u 的度数
    }
    q.push(n);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i].v, w = G[u][i].d;
            dis[v] += (dis[u] + w) / deg[v];
            --in[v];
            if (in[v] == 0) q.push(v);
        }
    }
    printf("%.2lf\n", dis[1]);
    return 0;
}
```
{% endfolding %}

#### [Cnoi2020] 线性生物

[Portal](https://www.luogu.com.cn/problem/P6835).

{% noteblock %}
线形生物要从 $1$ 号台阶走到 $n+1$ 号台阶。$1,2,3,\ldots,n$ 号台阶都有一条连向下一台阶的有向边 $i\rightarrow i+1$。还有 $m$ 条**返祖边** $u_i \rightarrow v_i (u_i \ge v_i)$，它们构成了一个**返祖图**。

线形生物每步会 **等概率地** 选取当前台阶的一条出边并走向对应的台阶。当走到 $n+1$ 号台阶时，线形生物就会停止行走。求线性生物期望行走的步数值。
{% endnoteblock %}

记 $E[i]$ 为从 $i\rightarrow i+1$ 的期望步数值，那么 $E_{x\rightarrow y}=\sum_{i=x}^{y-1}E[i]$。

## Problemset

纯概率的题目不是很多，往往会与其它算法综合。但是也有一些简单题目：

### 简单题目

这里是一些概率的简单应用。

#### [国家集训队] 单位选错

[Portal](https://www.luogu.com.cn/problem/P1297).

答案是 $\sum \frac{1}{\max\{a_{i-1},a_{i}\}}$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, A, B, C;
int a[10000005];

int main(void) {
    scanf("%d%d%d%d%d", &n, &A, &B, &C, a);
    for (int i = 1; i < n; i++)
    	a[i] = ((long long) a[i - 1] * A + B) % 100000001;
    for (int i = 0; i < n; i++)
    	a[i] = a[i] % C + 1;
    double ans = 0;
    for (int i = 1; i <= n; ++i)
        ans += 1.0 / max(a[i - 1], a[i % n]);
    printf("%.3lf\n", ans);
    return 0;
}
```
{% endfolding %}

#### [UVa 11181] Probability | Given

[Portal](https://www.luogu.com.cn/problem/UVA11181).

#### [UVa 1637] Double Patience

[Portal](https://www.luogu.com.cn/problem/UVA1637).

[^1]: 实际情况复杂得多，硬要说的话我只要知道空气流动速度、抛硬币的力的大小方向、接触面积等内容，一定能计算出抛硬币的结果。甚至计算机的随机数计算也是有一个公式的（所以生成的是伪随机数）。真正的随机数还有一些伪随机数不具有的性质，比如与信息熵相关的性质。