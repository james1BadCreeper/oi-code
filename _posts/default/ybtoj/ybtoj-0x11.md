---
title: YbtOJ 高效进阶 基础算法 第1章 递推算法
cover: false
plugins:
  - katex
abbrlink: 14d4fdbe
date: 2022-08-15 13:07:48
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 递推
status: done
sticky: -100
---

> ~~我也不知道为什么学校拿这个训练，就先写着吧。~~    
> 感觉题质还是比 NOIp 计划差一点。   
> 所以不配我为他传一张头图。

<!--more-->

由于这是个付费 OJ，所以展示题目，难以复制的使用图片。但是能找到原题的会放别的 OJ 的链接。

递推没什么好说的，其实就是 DP，但不一定是最优化问题。

## A. 错排问题

> 求有多少个 $n$ 个数的排列 $A$，满足任意的 $A_i\ne i$。

很经典的数学问题，可以写出通项公式做到 $O(1)$。

设 $f(i)$ 表示完成前 $i$ 个数的错排的方案数。第 $i$ 个数可以放在第 $1\dots i-1$ 中的任意一个，被挤掉的那个可以放在第 $i$ 个位置，那么方案数就是 $f(i-2)$；不放在那里的话，相当于也是一种禁止，虽然它原来位置的限制没有了，但限制它放在第 $i$ 个位置上，再加上剩下的元素，相当于 $i-1$ 个数的错排，也就是 $f(i-1)$。

```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int n;
long long f[25];

int main(void)
{
    scanf("%d", &n);
    f[1] = 0, f[2] = 1;
    for (int i = 3; i <= n; ++i)
        f[i] = (i - 1) * (f[i - 2] + f[i - 1]);
    printf("%lld\n", f[n]);
    return 0;
}
```

## B. 传球游戏

[Portal](https://www.luogu.com.cn/problem/P1057).

设 $f(x,k)$ 代表传 $k$ 次传到 $x$ 手里的方案数，那么 $f(x,k)=f(x-1,k-1)+f(x+1,k-1)$，注意边界处理，还可以使用滚动数组优化。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
long long f[35][2];

int main(void)
{
    scanf("%d%d", &n, &m);
    f[1][0] = 1;  
    for (int j = 1; j <= m; ++j)
        for (int i = 1; i <= n; ++i)
            f[i][j & 1] = f[i == 1 ? n : (i - 1)][(j - 1) & 1] + f[i == n ? 1 : i + 1][(j - 1) & 1];
    printf("%lld\n", f[1][m & 1]);
    return 0;
}
```

## C. 数的划分

[Portal](https://www.luogu.com.cn/problem/P1025).

设 $f(i,j)$ 代表将 $i$ 分为 $j$ 份的方案数，那么 $j$ 份中要么有 $1$ 份 $1$，要么没有 $1$，就将每一份减 $1$。$f(i,j)=f(i-1,j-1)+f(i-j,j)$。注意初始时 $f[i][i]=1$ 和边界处理。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, k;
long long f[205][10];

int main(void)
{
    scanf("%d%d", &n, &k);    
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= min(i, k); ++j)
        {
            if (i == j) f[i][j] = 1;
            else f[i][j] = f[i - 1][j - 1] + f[i - j][j];
        }
    printf("%lld\n", f[n][k]);
    return 0;
}
```

## D. 栈的问题

[Portal](https://www.luogu.com.cn/problem/P1044).

设 $f(i,j)$ 为 $i$ 个数还没有被输出，$j$ 个数还未入栈。可以输出栈中的一个数，也可以将一个数入栈，所以 $f(i,j)=f(i-1,j)+f(i,j-1)$，初始 $f(i,0)=1$。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
long long f[20][20];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        f[i][0] = 1;
        for (int j = 1; j <= i; ++j)
            f[i][j] = f[i - 1][j] + f[i][j - 1];
    }
    printf("%lld\n", f[n][n]);
    return 0;
}
```

## E. 划分数列

> 长度为 $n$ 的数列 $A$，划分为最少的段数，使得每一段单调不降或单调不升。

设 $f(i)$ 为 $1\dots i$ 的答案，$maxx(i),minn(j)$ 分别为以 $i$ 结尾的最长单调不降，最长单调不升子段的开头的前一位，那么 $f(i)=\min\{f(maxx(i)),f(minn(j))\}+1$。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
int a[100005], f[100005];
int maxx[100005], minn[100005];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) 
    {
        scanf("%d", a + i);
        maxx[i] = minn[i] = i - 1;
        if (a[i] >= a[i - 1]) maxx[i] = maxx[i - 1];
        if (a[i] <= a[i - 1]) minn[i] = minn[i - 1];
        f[i] = min(f[maxx[i]], f[minn[i]]) + 1;
    }
    printf("%d\n", f[n]);
    return 0;
}
```

## F. 求 f 函数

{% noteblock %}

求：

$$
f(x)=\begin{cases}
f(f(x+11)),&x\le 1000,\\
x-10,&x\ge 1001.
\end{cases}
$$

{% endnoteblock %}

递推太复杂，我选择递归。

```cpp 递归
#include <iostream>
#include <cstdio>

using namespace std;

int f[1000005];

int calc(int x)
{
    if (x >= 1001) return f[x] = x - 10;
    return f[x] = calc(f[x + 11] = calc(x + 11));
}

int main(void)
{
    for (int i = 1; i <= 1000000; ++i)
        calc(i);
    int n, ans = 0;
    while (scanf("%d", &n) == 1 && n)
        ans ^= f[n];
    printf("%d\n", ans);
    return 0;
}
```

但是随便改改就成递推了。

```cpp 递推
#include <iostream>
#include <cstdio>

using namespace std;

int f[1025];

int main(void)
{
    int n, ans = 0;
    for (int i = 1001; i <= 1020; ++i) f[i] = i - 10;
    for (int i = 1000; i >= 1; --i) f[i] = f[f[i + 11]];
    while (scanf("%d", &n) == 1 && n)
    {
        if (n >= 1001) ans ^= n - 10;
        else ans ^= f[n];
    }
    printf("%d\n", ans);
    return 0;
}
```

## G. 无限序列

![](https://s1.ax1x.com/2022/08/15/vazqXT.png)

设每次变化的序列为 $f(i)$，将它视为字符串，可得 $f(i)=f(i-1)+f(i-2)$，设第 $i$ 个序列的长度为 $size(i)$，$1$ 的个数为 $sum(i)$，询问的时候类似与前缀和，随便乱做。

```cpp
#include <iostream>
#include <cstdio>

#define ERROR -1

using namespace std;
typedef long long i64;

i64 len[70], sum[70];

i64 calc(i64 n)
{
    if (n == 0) return 0;
    for (int i = 65; i >= 1; --i)
        if (len[i] <= n) return sum[i] + calc(n - len[i]);
    return ERROR;
}

int main(void)
{
    len[1] = 1, len[2] = 2;
    sum[1] = sum[2] = 1;
    for (int i = 3; i <= 65; ++i)
        len[i] = len[i - 1] + len[i - 2], sum[i] = sum[i - 1] + sum[i - 2];
    int Q;
    scanf("%d", &Q);
    while (Q--)
    {
        i64 a, b;
        scanf("%lld%lld", &a, &b);
        printf("%lld\n", calc(b) - calc(a - 1));    
    }
    return 0;
}
```

## H. 序列个数

![](https://pic.imgdb.cn/item/62f9e19316f2c2beb10a9a1c.jpg)

填 $b_i$ 时放在一个二维 01 矩阵中填，$1$ 表示填，所以每行每列恰好有一个 $1$，相当与从左上角开始的正方形中的 $1$ 的个数为 $a_i$，那么 $op=a_i-a_{i-1}$ 就是 L 字型中 $A_i$ 的个数。递推时一个个 L 考虑。

- $op=0$，答案不变；
- $op=1$，有 $2i-1$ 个位置，但是里面放置了 $a_{i-1}$ 个 $1$，它们使得横竖都废掉了，所以有 $2i-1-2a{i-1}$ 中决策；
- $op=2$，只能横着一个，竖着一个，而且交点不能放，决策数为 $(i-1-a_{i-1})^2$；
- $op\ge 3$，在一个 L 中填不出来这东西。

```cpp
#include <iostream>
#include <cstdio>
#define i64 long long

using namespace std;
const int MOD = 340610;

int n;
int a[10005];

int main(void)
{
    scanf("%d", &n);
    i64 ans = 1;
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", a + i);
        if (a[i] - a[i - 1] == 1) ans = ans * ((i << 1) - 1 - (a[i - 1] << 1)) % MOD;
        else if (a[i] - a[i - 1] == 2)
        {
            if (i - 1 - a[i - 1] > 0) ans = ans * (i - 1 - a[i - 1]) % MOD * (i - 1 - a[i - 1]) % MOD; // 小于 0 证明没有这么多方案数
            else ans = 0;
        }
        else if (a[i] - a[i - 1] >= 3) ans = 0;
    }
    printf("%lld\n", ans);
    return 0;
}
```

## I. 等距跳跃

![](https://pic.imgdb.cn/item/62f9e81316f2c2beb11f9307.jpg)

~~不要问我为什么简单枚举也是递推，我也不知道。~~

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int r, c, n;
int tot;
int X[5005], Y[5005];

int cnt[5005][5005];
int main()
{
    scanf("%d%d%d", &r, &c, &n);
    for (int i = 1; i <= n; i++)
    {
        scanf("%d%d", &X[i], &Y[i]);
        cnt[X[i]][Y[i]]++;
    }
    int ans = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (i != j)
            {
                int xx = X[i], yy = Y[i];
                int dx = X[j] - X[i], dy = Y[j] - Y[i];
                int rx = X[i] - dx, ry = Y[i] - dy;
                if (rx >= 1 && rx <= r && ry >= 1 && ry <= c) continue;
                int sum = 0;
                while (xx >= 1 && xx <= r && yy >= 1 && yy <= c)
                {
                    if (!cnt[xx][yy])
                    {
                        sum = 0;
                        break;
                    }
                    ++sum;
                    xx += dx, yy += dy;
                }
                ans = max(ans, sum);
            }
    printf("%d\n", ans);
    return 0;
}
```

## 总结

我是菜狗。