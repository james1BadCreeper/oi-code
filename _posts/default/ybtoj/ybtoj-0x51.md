---
title: YbtOJ 高效进阶 动态规划 第1章 背包问题
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 背包
    - 动态规划
abbrlink: 642d7f95
date: 2022-08-15 17:41:36
status: done
sticky: -100
---

> YbtOJ 的背包问题，就做做玩一下吧。

<!--more-->

## A. 采药问题

[Portal](https://www.luogu.com.cn/problem/P1048).

01 背包模板。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int V, m;
int f[1005];

int main(void)
{
    scanf("%d%d", &V, &m);
    for (int i = 1; i <= m; ++i)
    {
        int v, w;
        scanf("%d%d", &v, &w);
        for (int j = V; j >= v; --j)
            f[j] = max(f[j], f[j - v] + w);
    }
    int ans = 0;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```

## B. 货币系统

[Portal](https://www.luogu.com.cn/problem/P5020).

大凯的疑惑，完全背包模板。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

int read(void)
{
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int n;
int a[105], f[100005];

inline void solve(void)
{
    n = read();
    for (int i = 1; i <= n; ++i) a[i] = read();
    sort(a + 1, a + n + 1);
    memset(f, 0, sizeof(f));
    f[0] = 1;
    int ans = n;
    for (int i = 1; i <= n; ++i)
        if (f[a[i]])
        {
            --ans;
            continue;
        }
        else
            for (int j = a[i]; j <= a[n]; ++j)
                f[j] |= f[j - a[i]];
    printf("%d\n", ans);
}

int main(void)
{
    int T = read();
    while (T--) solve();
    return 0;
}
```

## C. 宝物筛选

[Portal](https://www.luogu.com.cn/problem/P1776).

多重背包模板，使用二进制优化。

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

int n, V;
int f[40005];

struct BadCreeper
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
        if (m != 0) a.push_back(BadCreeper(m * v, m * w));
        for (int j = 0; j < a.size(); ++j)
            for (int k = V; k >= a[j].v; --k)
                f[k] = max(f[k], f[k - a[j].v] + a[j].w);
    }
    int ans = -1;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%d\n", ans);
    return 0;
}
```

## D. 硬币方案

![](https://pic.imgdb.cn/item/62fa16c816f2c2beb1b8fa86.jpg)

还是多重背包。

```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, m;
int a[105], c[105];
bool f[100005];

int main(void)
{
    while (scanf("%d%d", &n, &m) == 2 && n)
    {
        memset(f, 0, sizeof(f));
        f[0] = 1;
        for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
        for (int i = 1; i <= n; ++i) scanf("%d", &c[i]);
        for (int i = 1; i <= n; ++i)
        {
            vector <int> v;
            int p = 1;
            while (c[i] - p >= 0)
            {
                c[i] -= p;
                v.push_back(a[i] * p);
                p <<= 1;
            }
            if (c[i] != 0) v.push_back(a[i] * c[i]);
            for (int j = 0; j < v.size(); ++j)
                for (int k = m; k >= v[j]; --k)
                    f[k] |= f[k - v[j]];
        }
        int ans = 0;
        for (int i = 1; i <= m; ++i)
            if (f[i]) ++ans;
        printf("%d\n", ans);
    }
    return 0;
}
```

## E. 金明的预算方案

[Portal](https://www.luogu.com.cn/problem/P1064).

简单依赖性背包。

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

## F. 求好感度

![](https://pic.imgdb.cn/item/62fa179916f2c2beb1bb626e.jpg)

~~怎么又是多重背包。~~

```cpp
#include <iostream>
#include <cstdio>
#include <vector>

#define X first 
#define Y second

using namespace std;
typedef pair<int, int> pii;
typedef long long i64;

int n, V;
int v, w, m;
i64 f[2000005];

int main(void)
{
    scanf("%d%d", &n, &V);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d%d%d", &m, &w, &v);
        vector <pii> a;
        int p = 1;
        while (m - p >= 0)
        {
            m -= p;
            a.push_back(make_pair(v * p, w * p));
            p <<= 1;
        }
        if (m != 0) a.push_back(make_pair(v * m, w * m));
        for (int j = 0; j < a.size(); ++j)
            for (int k = V; k >= a[j].X; --k)
                f[k] = max(f[k], f[k - a[j].X] + a[j].Y);
    }
    i64 ans = -1;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%lld\n", ans);
    return 0;
}
```

## G. 购买商品

![](https://pic.imgdb.cn/item/62fa183416f2c2beb1bcb992.jpg)

就是先 01 背包再完全背包。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
typedef long long i64;

int n, V;
int v, w;
i64 f[1000005];

int main(void)
{
    scanf("%d%d", &V, &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d%d", &v, &w);
        for (int j = V; j >= v; --j)
            f[j] = max(f[j], f[j - v] + w);
    }
    i64 ans = 0;
    int useV = 0;
    for (int i = 0; i <= V; ++i)
        if (f[i] > ans)
        {
            ans = f[i];
            useV = i;
        }
    V = V - useV;

    scanf("%d", &n);
    memset(f, 0, sizeof(f));
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d%d", &v, &w);
        for (int j = v; j <= V; ++j)
            f[j] = max(f[j], f[j - v] + w);
    }
    ans = 0;
    for (int i = 0; i <= V; ++i)
        ans = max(ans, f[i]);
    printf("%lld\n", ans);
    return 0;
}
```

## H. 魔法开锁

![](https://pic.imgdb.cn/item/62fa3c4e16f2c2beb10bcb2d.jpg)

用迭代的方式快速求出连通块，整个图就会分裂成 $m$ 个绕圈指向的环。设 $f(i,j)$ 表示考虑前 $i$ 个环，已经选择了 $j$ 个点。那么满足题意的方案数为 $f(m,t)$，总方案数是 $C_{n}^{t}$。

转移的时候 $f(i,j)=\sum f(i-1,k)\times C_{Size[i]}^{j-k}$，其中 $Size$ 表示环的大小，是 01 背包，然后有 $j$ 种决策。由于每个环中必须选一个点，所以 $j\ne k$。可以使用滚动数组滚掉第一维。
 
但是为什么 $f$ 要用 `double` 来记啊……造数据的人不知道 double 会有精度误差吗……还是别的问题啊……

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, m, t;
int a[305];
bool v[305];
int Size[305];
double f[305], C[305][305];

void init(void)
{
    C[0][0] = 1;
    for (int i = 1; i <= 300; ++i)
    {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j)
            C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
    }
}

int main(void)
{
    init();
    int T;
    scanf("%d", &T);
    while (T--)
    {
        memset(v, 0, sizeof(v));
        memset(f, 0, sizeof(f));
        memset(Size, 0, sizeof(Size));
        m = 0;
        scanf("%d%d", &n, &t);
        for (int i = 1; i <= n; ++i) scanf("%d", a + i);
        for (int i = 1; i <= n; ++i)
            if (!v[i])
            {
                int x = i, tot = 0;
                while (1)
                {
                    v[x] = true; ++tot;
                    if (a[x] == i) break;
                    x = a[x];
                }
                Size[++m] = tot;
            }
        f[0] = 1;
        for (int i = 1; i <= m; ++i)
            for (int j = n; j >= 0; --j)
            {
                f[j] = 0;
                for (int k = 0; k < j; ++k)
                    f[j] += f[k] * C[Size[i]][j - k];
            }
        printf("%.8lf\n", double(f[t]) / C[n][t]);
    }
    return 0;
}
```

## I. 购买礼物

![](https://pic.imgdb.cn/item/62fa4ab216f2c2beb13bc7be.jpg)

利用所说的“打不过就加入”，我们定义：$f(i,j,k,l)$ 为考虑前 $i$ 个物品，第一张卡使用 $j$ 元，第二张卡使用 $k$ 元，$l$ 是个 `bool`，代表是否使用了礼物卷儿，这样的喜爱值。

那么就可以乱做了，可以使用滚动数组。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

const int INF = 0x7f7f7f7f;

int V1, V2, n, p, v, flag;
int f[505][55][2];

bool checkans(int &ans)
{
    for (int i = 0; i <= V1; ++i)
        for (int j = 0; j <= V2; ++j)
            ans = max({ans, f[i][j][0], f[i][j][1]});
    return ans < 0;
}

int main(void)
{
    scanf("%d%d%d", &V1, &V2, &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d%d%d", &p, &v, &flag);
        for (int j = V1; j >= 0; --j)
            for (int k = V2; k >= 0; --k)
            {
                if (flag) 
                {
                    f[j][k][1] = f[j][k][0] + v; // 这时候只能使用卷儿来初始化
                    f[j][k][0] = -INF; // 直接没有了，必须等待更新
                }
                else f[j][k][1] = max(f[j][k][1], f[j][k][0] + v); // 可以考虑 f[i-1][j][k][1]
                if (j >= p)
                {
                    f[j][k][0] = max(f[j][k][0], f[j - p][k][0] + v);
                    f[j][k][1] = max(f[j][k][1], f[j - p][k][1] + v);
                }
                if (k >= p)
                {
                    f[j][k][0] = max(f[j][k][0], f[j][k - p][0] + v);
                    f[j][k][1] = max(f[j][k][1], f[j][k - p][1] + v);
                }
            }
    }
    int ans = -1;
    if (checkans(ans)) puts("-1");
    else printf("%d\n", ans);
    return 0;
}
```

## J. 课题选择

[Portal](https://www.luogu.com.cn/problem/P1336).

这可以看成一个分组背包，一组的内容是这一个课题选若干个。

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

#define i64 long long

using namespace std;

i64 poww(i64 a, int p)
{
    i64 res = 1;
    while (p)
    {
        if (p & 1) res = res * a;
        a = a * a;
        p >>= 1;
    }
    return res;
}

int n, m;
int a, b;
i64 f[205]; // 滚掉后只剩选多少个课

int main(void)
{
    memset(f, 0x3f, sizeof(f));
    f[0] = 0;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; ++i)
    {
        scanf("%d%d", &a, &b);
        for (int j = n; j >= 0; --j)
            for (int k = 0; k <= j; ++k) // 最多选 j
                f[j] = min(f[j], f[j - k] + a * poww(k, b));
    }
    printf("%lld\n", f[n]); // 选 n 个课
    return 0;
}
```

## 小结

我是大菜狗，[@ix35](https://www.luogu.com.cn/user/113546) 天下第一。