---
title: YbtOJ 高效进阶 动态规划 第2章 区间DP
cover: false
plugins:
  - katex
categories:
    - 算法竞赛
    - 做题记录
tag:
  - 背包
  - 区间 DP
status: working
abbrlink: 2b6145f0
date: 2022-08-16 08:13:56
sticky: -100
---

> 随便做一点区间 DP。

<!--more-->

## A. 石子合并

[Portal](https://www.luogu.com.cn/problem/P1880).

模板题。

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
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= (n << 1) - len + 1; ++i)
        {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                f[i][j] = min(f[i][j], f[i][k] + f[k+1][j] + sum[j] - sum[i-1]);
        }
    for (int i = 1; i <= n; ++i) ans = min(ans, f[i][i + n - 1]);
    printf("%d\n", ans);

    ans = 0;
    memset(f, 0, sizeof(f));
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= (n << 1) - len + 1; ++i)
        {
            int j = i + len - 1;
            for (int k = i; k < j; ++k)
                f[i][j] = max(f[i][j], f[i][k] + f[k+1][j] + sum[j] - sum[i-1]);
        }
    for (int i = 1; i <= n; ++i) ans = max(ans, f[i][i + n - 1]);
    printf("%d\n", ans);
    return 0;
}
```

## B. 木板涂色

[Portal](https://www.luogu.com.cn/problem/P4170).

还是板子。

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
    for (int len = 2; len <= n; ++len)
    {
        for (int i = 1; i <= n - len + 1; ++i)
        {
            int j = i + len - 1;
            if (s[i] == s[j]) f[i][j] = min(f[i][j - 1], f[i + 1][j]);
            for (int k = i; k < j; ++k)
                f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j]);
        }
    }
    printf("%d\n", f[1][n]);
    return 0;
}
```

## C. 消除木块

[Portal](https://www.luogu.com.cn/problem/UVA10559).

预处理颜色段，然后定义 $f(l,r,k)$ 代表消除 $[l,r]$，且 $r$ 右边还有 $k$ 个与 $r$ 材质相同的木块。那么要么是消 $r$，要么是在 $[l,r-1)$ 中有与 $r$ 相同的颜色，消完中间将两段颜色合并。

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
    if (l == r) return f[l][r][k] = (len[r] + k) * (len[r] + k);
    f[l][r][k] = dfs(l, r - 1, 0) + (len[r] + k) * (len[r] + k);
    for (int i = l; i < r - 1; ++i)
        if (color[i] == color[r]) f[l][r][k] = max(f[l][r][k], dfs(l, i, len[r] + k) + dfs(i + 1, r - 1, 0));
    return f[l][r][k];
}

int main(void)
{
    int T, kase = 0;
    scanf("%d", &T);
    while (T--)
    {
        memset(f, 0, sizeof(f));
        tot = 0;
        scanf("%d", &n);
        for (int i = 1; i <= n; ++i)
        {
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

## E. 删数问题

![](https://pic.imgdb.cn/item/62fc5a7616f2c2beb15ec2b5.jpg)

板子。

```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

int n, a[105];
int f[105][105];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", a + i);
        f[i][i] = a[i];
    }
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i)
        {
            int j = i + len - 1;
            f[i][j] = abs(a[j] - a[i]) * (j - i + 1);
            for (int k = i; k < j; ++k)
                f[i][j] = max(f[i][j], f[i][k] + f[k + 1][j]);
        }
    printf("%d\n", f[1][n]);
    return 0;
}
```

## F. 恐狼后卫

![](https://pic.imgdb.cn/item/62fc5f8916f2c2beb16169f0.jpg)

我们想要杀掉敌人（生态环保，不能杀狼），一定要集中精力先干掉一个，因为先砍这个一刀再砍那个一刀的结果不会更好，则本题又成了板子。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

#define i64 long long

using namespace std;

int n, atk;
int a[405], b[405], h[405];
i64 f[405][405];

inline i64 deg(int x) { return h[x] / atk + (h[x] % atk == 0 ? 0 : 1); }

int main(void)
{
    memset(f, 0x3f, sizeof(f));
    scanf("%d%d", &n, &atk);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d%d%d", a + i, b + i, h + i);
        f[i][i - 1] = 0;
    }
    f[n + 1][n] = 0;
    for (int i = 1; i <= n; ++i)
        f[i][i] = deg(i) * (a[i] + b[i - 1] + b[i + 1]);
    for (int len = 2; len <= n; ++len)
        for (int i = 1; i <= n - len + 1; ++i)
        {
            int j = i + len - 1;
            for (int k = i; k <= j; ++k)
                f[i][j] = min(f[i][j], f[i][k - 1] + f[k + 1][j] + deg(k) * (a[k] + b[i - 1] + b[j + 1]));
        }
    printf("%lld\n", f[1][n]);
    return 0;
}
```

## G. 矩阵取数

[Portal](https://www.luogu.com.cn/problem/P1005).

板子。

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
    while (n--)
    {
        for (int i = 1; i <= m; ++i) a[i] = read();
        memset(f, 0, sizeof(f));
        f[1][m] = 0;
        LL k = 1;
        for (int len = m - 1; len >= 0; --len)
        {
            k *= 2;
            for (int i = 1; i <= m - len + 1; ++i)
            {
                int j = i + len - 1;
                f[i][j] = max(f[i - 1][j] + a[i - 1] * k, f[i][j + 1] + a[j + 1] * k);
            }
        }
        LL res = -1;
        for (int i = 1; i <= m; ++i)
            res = max(res, f[i][i] + a[i] * k);
        ans += res;
    }
    print(ans);
    putchar('\n');
    return 0;
}
```

## H. 生日欢唱

![](https://pic.imgdb.cn/item/62fd7dba16f2c2beb1024d17.jpg)

~~典中典之读不懂题。~~



## 小结

我是弟中弟，sto [@Cat_Shao](https://www.luogu.com.cn/user/234011) LN 省队爷，还比我小，打不过了，只能 %%%。