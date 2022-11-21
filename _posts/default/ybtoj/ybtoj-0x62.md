---
title: YbtOJ 高效进阶 数学基础 第2章 质数和约数
cover: false
plugins:
  - katex
abbrlink: b59b0a55
date: 2022-08-17 00:52:01
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 数论
status: working
sticky: -100
---

> 如果原创题多一点的话，其实还可以吧……

<!--more-->

## A. 线性筛素数

[Portal](https://www.luogu.com.cn/problem/P3383).

蜜汁相似。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

void print(int x)
{
    if (x > 9) print(x / 10);
    putchar(x % 10 + '0');
}

int n, q, tot;
int prime[100000005];
bool v[100000005];

int main(void)
{
    n = read(), q = read();
    v[0] = v[1] = true;
    for (int i = 2; i <= n; ++i)
    {
        if (!v[i]) prime[++tot] = i;
        for (int j = 1; j <= tot && i * prime[j] <= n; ++j)
        {
            v[i * prime[j]] = true;
            if (i % prime[j] == 0) break;
        }
    }
    while (q--) print(prime[read()]), putchar('\n');
    return 0;
}
```

## B. 质数距离

[Portal](https://www.luogu.com.cn/problem/UVA10140).

蜜汁相似 $\times 2$。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;
using i64 = long long;

bool is_prime[50005];
bool flag[1000005];
vector <int> prime;

int main(void)
{
    for (int i = 2; i <= 50000; ++i)
        is_prime[i] = true;
    int m = sqrt(50000) + 1;
    for (int i = 2; i <= m; ++i)
    {
        if (is_prime[i])
        {
            for (int j = i * i; j <= 50000; j += i)
                is_prime[j] = 0;
        }
    }
    unsigned int l, r;
    while (scanf("%u%u", &l, &r) == 2)
    {
        if (l == 1) l = 2;
        if (l >= r)
        {
            puts("There are no adjacent primes.");
            continue;
        }
        
        memset(flag, 1, sizeof(flag));
        int m = min((unsigned int)(sqrt(r)) + 1, r);
        for (int i = 2; i <= m; ++i)
            if (is_prime[i])
            {
                i64 j = (l % i ? i - l % i + l : l);
                for (; j <= r; j += i)
                    if (j != i) flag[j - l] = false;
            }
        prime.clear();
        for (int i = 0; i <= r - l; ++i)
            if (flag[i]) prime.push_back(i + l);
        if (prime.size() < 2) puts("There are no adjacent primes.");
        else
        {
            #define pii pair<int, int>
            #define X first
            #define Y second
            pii a, b;
            int minn = 0x7fffffff, maxx = 0;
            for (int i = 1; i < prime.size(); ++i)
            {
                if (prime[i] - prime[i - 1] < minn)
                    minn = (a.Y = prime[i]) - (a.X = prime[i - 1]);
                if (prime[i] - prime[i - 1] > maxx)
                    maxx = (b.Y = prime[i]) - (b.X = prime[i - 1]);
            }
            printf("%d,%d are closest, %d,%d are most distant.\n", a.X, a.Y, b.X, b.Y);
        }
    }
    return 0;
}
```
