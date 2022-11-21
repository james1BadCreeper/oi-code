---
title: 洛谷 NOIP 计划 2022 模拟赛补题
cover: false
plugins:
  - katex
abbrlink: 180170fd
date: 2022-09-19 21:57:02
categories:
    - 算法竞赛
    - 比赛记录
headimg: https://pic.imgdb.cn/item/6328764316f2c2beb1de1402.jpg
sticky: 1000
password: 187051#yugu22nnoip#116858f3
abstract: NOIP 计划 2022 共计 16 场模拟赛的补题，以及去年笔者报名的夏令营和秋令营共 7 场与 NOIP 计划 2021 同步的模拟赛补题。密码不会公开（或者通过 NOIP 计划群私聊笔者索要）。
theme: xray
---

> NOIP 计划 2022 共计 $16$ 场模拟赛的补题，以及去年笔者报名的夏令营和秋令营共 $7$ 场与 NOIP 计划 2021 同步的模拟赛补题。其中有价值的部分分会特别打一下。

<!--more-->

## NOIP 2022 模拟赛 1

第一场，By 宣毅鸣。

### A. T1

[Portal](https://www.luogu.com.cn/problem/T252153).

> 给出一个包含小写字母，大写字母和数字的字符串，把小写字母变成大写字母，把大写字母变成小写字母，把数字变成9-他本身（比如0->9,7->2）。

签到题。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

int main(void)
{
    string s;
    cin >> s;
    for (int i = 0; i < s.length(); ++i)
        if ('a' <= s[i] && s[i] <= 'z') putchar(s[i] - 'a' + 'A');
        else if ('A' <= s[i] && s[i] <= 'Z') putchar(s[i] - 'A' + 'a');
        else putchar(9 - (s[i] - '0') + '0');
    putchar('\n');
    return 0;
}
```
{% endfolding %}

### B. T2

[Portal](https://www.luogu.com.cn/problem/T252163).

{% noteblock %}
给出一个长度$n$个数组$a_i$。求所有长度为$m$的子序列，去掉最大和最小两个数后的乘积的乘积对$10^9+7$取模后的结果。保证所有$a_i$互不相同。

对于$30\%$的数据，$n \leq 10$。  
对于$50\%$的数据，$n \leq 100$。  
对于$100\%$的数据，$n \leq 5000, 3 \leq m \leq n, 1 \leq a_i \leq 10000$,$a_i$互不相同。
{% endnoteblock %}

#### Subtask 1

直接暴力 `dfs` 即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
using i64 = long long;

const int MOD = 1000000007;

int n, m;
int a[5005];

int poww(int a, int b)
{
    int res = 1;
    while (b)
    {
        if (b & 1) res = (i64)res * a % MOD;
        a = (i64)a * a % MOD;
        b >>= 1;
    }
    return res;
}

int s[5005], cnt[5005];

void dfs(int o, int x)
{
    if (x == m)
    {
        for (int i = 2; i < m; ++i)
            ++cnt[s[i]];
        return;
    }
    if (o > n) return;
    dfs(o + 1, x);
    s[x + 1] = o;
    dfs(o + 1, x + 1);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    sort(a + 1, a + n + 1);
    dfs(1, 0);
    i64 ans = 1;
    for (int i = 1; i <= n; ++i)
        ans = ans * poww(a[i], cnt[i]) % MOD;
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### Subtask 2 & 3

去掉最大和最小的两个数？显然序列的顺序是不会影响答案的，那么将原序列排序后再选取子序列，最小的元素是最左边的，最大的元素是最右边的。

答案是乘积的乘积，所以我们考虑计算一个数被乘入了几次答案。正常来讲它会被选入 $\dbinom{n-1}{m-1}$ 次，然后它是最大的数的情况会有 $\dbinom{i-1}{m-1}$ 次，最小的数的情况有 $\dbinom{n-i}{m-1}$ 次。

由于组合数很大，所以需要进行取模，由于 $MOD=10^9+7$ 与任何数互质，所以可以使用费马小定理统一模掉 $MOD-1$ 来进行计算。 

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
const int MOD = 1000000007;

int n, m;
int a[5005];
int C[5005][5005];

int qpow(int a, int b)
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
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i);
    sort(a + 1, a + n + 1);

    C[0][0] = 1;
    for (int i = 1; i <= n; ++i)
    {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j)
            C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % (MOD - 1);
    }

    int ans = 1;
    for (int i = 1; i <= n; ++i)
        ans = 1ll * ans * qpow(a[i], ((C[n - 1][m - 1] - C[i - 1][m - 1] - C[n - i][m - 1]) % (MOD - 1) + (MOD - 1)) % (MOD - 1)) % MOD;
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### C. T3

[Portal](https://www.luogu.com.cn/problem/T252171).

{% noteblock %}

给出一颗树，边有边权，每次求一条路径上小于等于$z$的边权的异或和。

对于$30\%$的数据，$n,m \leq 1000$。

对于$50\%$的数据，$n,m \leq 10^5$。

对于另外$30\%$的数据，询问中的$z = 10^9$。

对于$100\%$的数据，$n,m \leq 5\times 10^5,0 \leq z \leq 10^9,1 \leq x,y \leq n$。

{% endnoteblock %}

#### 部分分

前三段部分分分别可以使用暴力跳和树上前缀和来解决。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n, m;
struct edge
{
    int v, d;
    edge(int v = 0, int d = 0) :
        v(v), d(d) {}
};
vector <edge> G[500005];
int f[500005], dep[500005], son[500005];
int top[500005], siz[500005], S[500005], d[500005];

void dfs1(int x, int fa)
{
    f[x] = fa; dep[x] = dep[fa] + 1; siz[x] = -1;
    int maxx = -1;
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i].v, w = G[x][i].d;
        if (y != fa)
        {
            S[y] = (S[x] ^ w), d[y] = w;
            dfs1(y, x);
            siz[x] += siz[y];
            if (siz[y] > maxx) son[x] = y, maxx = siz[y];
        }
    }
}

void dfs2(int x, int topf)
{
    top[x] = topf;
    if (son[x] == -1) return;
    dfs2(son[x], topf);
    for (int i = 0; i < G[x].size(); ++i)
    {
        int y = G[x][i].v;
        if (y != f[x]) dfs2(y, y);
    }
}

int LCA(int x, int y)
{
    while (top[x] != top[y])
    {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        x = f[top[x]];
    }
    return dep[x] < dep[y] ? x : y;
}

int calc(int x, int y, int z)
{
    if (dep[x] < dep[y]) swap(x, y);
    int ans = 0;
    while (dep[x] > dep[y])
    {
        if (d[x] <= z) ans ^= d[x];
        x = f[x];
    }
    while (x != y)
    {
        if (d[x] <= z) ans ^= d[x];
        if (d[y] <= z) ans ^= d[y];
        x = f[x], y = f[y];
    }
    return ans;
}

int main(void)
{
    memset(son, 0xff, sizeof(son));
    scanf("%d", &n);
    for (int i = 1, u, v, d; i < n; ++i)
    {
        scanf("%d%d%d", &u, &v, &d);
        G[u].push_back(edge(v, d));
        G[v].push_back(edge(u, d));
    }
    dfs1(1, 0);
    dfs2(1, 1);
    scanf("%d", &m);
    while (m--)
    {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        if (z == 1000000000) printf("%d\n", S[x] ^ S[y] ^ S[LCA(x, y)] ^ S[LCA(x, y)]);
        else printf("%d\n", calc(x, y, z));
    }
    return 0;
}
```
{% endfolding %}

#### 正解

### D. T4

## NOIP 2022 模拟赛 2

## NOIP