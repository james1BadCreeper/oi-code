---
title: YbtOJ 高效进阶 动态规划 第4章 树形DP
cover: false
plugins:
  - katex
abbrlink: 77d5fca3
date: 2022-08-17 11:49:15
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 动态规划
    - 树形 DP
status: working
sticky: -100
---

> 最近在重学树形 DP，多做一点题目。

<!--more-->

## A. 树上求和

![](https://pic.imgdb.cn/item/62fc6a1a16f2c2beb1678d20.jpg)

就是板子。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int n;
int r[6005], f[6005][2];
bool v[6005];
vector <int> son[6005];

void dp(int x)
{
    f[x][0] = 0, f[x][1] = r[x];
    for (int i = 0; i < son[x].size(); ++i)
    {
        int y = son[x][i];
        dp(y);
        f[x][0] += max(f[y][0], f[y][1]);
        f[x][1] += f[y][0];
    }
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%d", r + i);
    for (int i = 1; i < n; ++i)
    {
        int x, y;
        scanf("%d%d", &x, &y);
        son[y].push_back(x);
        v[x] = true;
    }
    int root;
    for (int i = 1; i <= n; ++i)
        if (!v[i])
        {
            root = i;
            break;
        }
    dp(root);
    printf("%d\n", max(f[root][0], f[root][1]));
    return 0;
}
```

## B. 结点覆盖

![](https://pic.imgdb.cn/item/62fc9e9416f2c2beb18bd698.jpg)

见注释{% emoji tong ... %}。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int n;
int k[1505];
int f[1505][3]; // 0 为父节点，1 为子节点，2 为本身
vector <int> son[1505];
bool v[1505];

void dp(int x)
{
    int r = 1e9;
    f[x][2] = k[x]; // 自己需要付出代价
    for (int i = 0; i < son[x].size(); ++i)
    {
        int y = son[x][i];
        dp(y);
        f[x][0] += min(f[y][1], f[y][2]); // 父亲节点管自己，自己的儿子们不能依靠它们的父亲
        f[x][1] += min(f[y][1], f[y][2]); // 儿子节点管自己，自己的儿子们不能依靠它们的父亲
        r = min(r, f[y][2] - min(f[y][1], f[y][2]));
        // 注意以上
        // 儿子节点管的话，至少有一个要是 f[y][2]
        // 但可能选的全是 f[y][1]，所以我们要补差价
        // r 记录最小的差价，补一份即可
        // 计算差价的时候，使用正常要让子节点依靠自己：f[y][2]。然后减去选择的方案
        // 选择的方案的代价是 f[y][1], f[y][2] 的最小值
        f[x][2] += min({f[y][0], f[y][1], f[y][2]}); // 自己管自己，儿子们靠谁都可以
    }
    f[x][1] += r; // 加上补了的差价
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        int t, m, s;
        scanf("%d", &t);
        scanf("%d%d", &k[t], &m);
        while (m--)
        {
            scanf("%d", &s);
            son[t].push_back(s);
            v[s] = true;
        }
    }
    int root;
    for (int i = 1; i <= n; ++i)
        if (!v[i]) root = i;
    dp(root);
    printf("%d\n", min(f[root][1], f[root][2]));
    return 0;
}
```

## D. 选课方案

[Portal](https://www.luogu.com.cn/problem/P2014).

依赖性背包。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m, s[305];
int f[305][305];
vector <int> son[305];

void dp(int x)
{
    f[x][0] = 0;
    for (int i = 0; i < son[x].size(); ++i)
    {
        int &y = son[x][i];
        dp(y);
        for (int t = m; t >= 0; --t)
            for (int j = t; j >= 0; --j)
                if (t - j >= 0) f[x][t] = max(f[x][t], f[x][t-j] + f[y][j]);
    }
    if (x != 0)
        for (int t = m; t > 0; --t)
            f[x][t] = f[x][t-1] + s[x];
}

int main(void)
{
    n = read(), m = read();
    for (int i = 1; i <= n; ++i)
    {
        int x = read();
        son[x].push_back(i);
        s[i] = read();
    }
    memset(f, 0xff, sizeof(f)); // 求的是最大值
    dp(0); // 从超级节点开始 dp
    printf("%d\n", f[0][m]); // 根据经验，不需要取最大值
    return 0;
}
```

## 小结

我是全国最菜。