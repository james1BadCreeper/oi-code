---
title: '[Luogu2671][NOIP2015 普及组] 求和'
cover: false
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 找规律
    - 解题
abbrlink: e1d941a1
date: 2022-07-18 11:09:56
plugins:
  - katex
status: done
sticky: -50
sidebar: []
---

> 一道找规律与推导的题目。

<!--more-->

## 题目

[Portal](https://www.luogu.com.cn/problem/P2671).

{% folding blue::展开题目 %}

{% tabs %}

<!-- tab 题目描述 -->

一条狭长的纸带被均匀划分出了$n$个格子，格子编号从$1$到$n$。每个格子上都染了一种颜色$color_i$用$[1,m]$当中的一个整数表示），并且写了一个数字$number_i$。

![](https://cdn.luogu.com.cn/upload/pic/1829.png) 

定义一种特殊的三元组：$(x,y,z)$，其中$x,y,z$都代表纸带上格子的编号，这里的三元组要求满足以下两个条件：

1. $xyz$是整数,$x<y<z,y-x=z-y$

2. $colorx=colorz$

满足上述条件的三元组的分数规定为$(x+z) \times (number_x+number_z)$。整个纸带的分数规定为所有满足条件的三元组的分数的和。这个分数可能会很大，你只要输出整个纸带的分数除以$10,007$所得的余数即可。

<!-- endtab -->

<!-- tab 输入输出格式 -->

{% folding yellow::输入格式 %}

第一行是用一个空格隔开的两个正整数$n$和$m,n$表纸带上格子的个数，$m$表纸带上颜色的种类数。

第二行有$n$用空格隔开的正整数，第$i$数字$number$表纸带上编号为$i$格子上面写的数字。

第三行有$n$用空格隔开的正整数，第$i$数字$color$表纸带上编号为$i$格子染的颜色。

{% endfolding %}

{% folding red::输出格式 %}

一个整数，表示所求的纸带分数除以$10007$所得的余数。

{% endfolding %}

<!-- endtab -->

<!-- tab 输入输出样例 -->

{% codeblock 样例输入#1 %}
6 2
5 5 3 2 2 2
2 2 1 1 2 1
{% endcodeblock %}

{% codeblock 样例输出#1 %}
82
{% endcodeblock %}

{% codeblock 样例输入#2 %}
15 4
5 10 8 2 2 2 9 9 7 7 5 6 4 2 4
2 2 3 3 4 3 3 2 4 4 4 4 1 1 1
{% endcodeblock %}

{% codeblock 样例输出#2 %}
1388
{% endcodeblock %}

<!-- endtab -->

<!-- tab 样例说明 -->

【输入输出样例 1 说明】

纸带如题目描述中的图所示。

所有满足条件的三元组为： $(1, 3, 5), (4, 5, 6)$。

所以纸带的分数为$(1 + 5) \times (5 + 2) + (4 + 6) \times (2 + 2) = 42 + 40 = 82$。


对于第 $1$ 组至第 $2$ 组数据， $1 \le n \le 100, 1 \le m \le 5$；

对于第 $3$ 组至第 $4$ 组数据， $1 \le n \le 3000, 1 \le m \le 100$；

对于第 $5$ 组至第 $6$ 组数据， $1 \le n \le 100000, 1 \le m \le 100000$，且不存在出现次数超过 $20$ 的颜色；

对于全部 $10$ 组数据 ， $1 \le n \le 100000, 1 \le m \le 100000, 1 \le color_i \le m,1\le number_i\le 100000$

<!-- endtab -->

{% endtabs %}

{% endfolding %}

## 解答

$x<y<z,y-x=z-y$ 表示的显然是距离相等，也就是 $y$ 是 $x,z$ 的平均数，而且 $y$ 是个整数，所以 $x,z$ 奇偶性相同，分别进行计算即可。   

又因为颜色与答案无关，所以颜色也可以分别计算。现在的问题就转化为了求：

$$
\sum\limits_{i=1}^{n}\sum\limits_{j=i+1}^{n}(A_i+A_j)(B_i+B_j)
$$

展开后会发现当中有 $n-1$ 个 $A_i B_i$，和所有 $A,B$ 相乘。利用多项式乘法的特性，可得原式等于：

$$
\sum\limits_{i=1}^{n}A_i \times \sum\limits_{i=1}^{n}B_i + \sum\limits_{i=1}^{n}(n-2)A_iB_i
$$

代码就很简单了：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

#define pii pair<int, int>
#define X first
#define Y second

using namespace std;

const int MOD = 10007;

int n, m;
int number[100005], color[100005];
vector <pii> v[100005][2];

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", number + i), number[i] %= MOD;
    for (int i = 1; i <= n; ++i) scanf("%d", color + i);
    for (int i = 1; i <= n; ++i)
        v[color[i]][i & 1].push_back(make_pair(i, number[i]));
    int ans = 0;
    for (int op = 0; op < 2; ++op)
    {
        for (int i = 1; i <= m; ++i)
        {
            int R = v[i][op].size();
            if (R >= 2)
            {
                int res = 0, ret = 0;
                for (int j = 0; j < R; ++j)
                    res = (res + v[i][op][j].X) % MOD, ret = (ret + v[i][op][j].Y) % MOD;
                ans = (ans + res * ret % MOD) % MOD;
                for (int j = 0; j < R; ++j)
                    ans = (ans + (R - 2) % MOD * v[i][op][j].X % MOD * v[i][op][j].Y % MOD) % MOD;
            }
        }
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}