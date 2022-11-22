---
title: Codeforces Div.2 VP 记录
cover: false
categories:
    - 算法竞赛
    - 比赛记录
tag:
    - Div.2
    - Codeforces
    - 比赛
    - 笔记
plugins:
    - katex
headimg: 'https://pic.imgdb.cn/item/6371c85216f2c2beb12c24ef.jpg'
abbrlink: d71fa5a7
date: 2022-11-14 16:13:10
---

> 我不会做普及题了。

<!--more-->

只有有意思的题目才会写题解（~~当然对笔者这种普及组选手来说，可能所有题都有意思）~~。

## 11 月 比赛记录

新征途。

### Codeforces Round #830 (Div.2)

[Portal](https://codeforces.com/contest/1732).

#### A. Bestie

[Portal](https://codeforces.com/contest/1732/problem/A).

$(n,n-1)=1$，所以最多只需要改 $n,n-1$ 两次，讨论一下即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int gcd(int x, int y) {
    if (y == 0) return x;
    return gcd(y, x % y);
}

int main(void) {
    int T;
    cin >> T;
    while (T--) {
        int n, a[25], g = 0;
        cin >> n;
        for (int i = 1; i <= n; ++i) cin >> a[i], g = gcd(g, a[i]);
        if (g == 1) puts("0");
        else if (gcd(g, n) == 1) puts("1");
        else if (gcd(g, n - 1) == 1) puts("2");
        else puts("3");
    }
    return 0;
}
```
{% endfolding %}

#### B. Ugu

[Portal](https://www.luogu.com.cn/problem/CF1732B).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

int main(void)
{
    int T, n;
    cin >> T;
    while (T--) {
        string s;
        cin >> n >> s;
        bool rev = 0;
        int ans = 0;
        for (int i = 1; i < n; ++i) {
            int a = s[i] - '0', b = s[i - 1] - '0';
            if (a == rev && b != rev)
                ++ans, rev = !rev;
        }
        cout << ans << endl;
    }
    return 0;
}
```
{% endfolding %}

#### C. Sheikh (Hard Version)

{% noteblock %}
[Portal](https://codeforces.com/contest/1732/problem/C2).

一个数列的价值是数列的总和减去所有元素的异或和。每个询问形如 $L_i,R_i$，含义是希望找到一个区间 $[l,r]$，满足 $L_i\le l\le r\le R_i$，使得这个区间的价值最大。输出所有价值最大的区间中最短的一个（多解则任意）。$n,q\le 2\times 10^5$
{% endnoteblock %}

由于异或是不进位的，所以当区间长度变长的时候，价值必然不会变小。

如果一个数是零，那么显然它对答案是没有贡献的，所以可以删掉左右两端的零，中间的也可以删掉。由于最多有 $32$ 位会导致异或与加法等价，所以去掉 $0$ 之后可以暴力计算。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
typedef long long i64;

int n, q;
int a[100005];
int pre[100005], nxt[100005];
i64 sum[100005], xxor[100005];

i64 calc(int l, int r) {
    return (sum[r] - sum[l - 1]) - (xxor[r] ^ xxor[l - 1]);
}

int main(void) 
{
    int T;
    cin >> T;
    while (T--) {
        cin >> n >> q;
        for (int i = 1; i <= n; ++i)
            cin >> a[i], sum[i] = sum[i - 1] + a[i], xxor[i] = (xxor[i - 1] ^ a[i]);
        for (int i = 1; i <= n; ++i)
            if (a[i]) pre[i] = i;
            else pre[i] = pre[i - 1];
        nxt[n + 1] = n + 1;
        for (int i = n; i >= 1; --i)
            if (a[i]) nxt[i] = i;
            else nxt[i] = nxt[i + 1];
        while (q--) {
            int l, r;
            cin >> l >> r;
            l = min(r, nxt[l]);
            r = max(l, pre[r]);
            if (l == r) {
                cout << l << ' ' << l << '\n';
                continue;
            }
            int u = l, v = r, ans = r - l + 1;
            i64 res = calc(l, r);
            for (int i = l; i <= r && calc(i, r) == res; i = nxt[i + 1])
                for (int j = r; j >= i && calc(i, j) == res; j = pre[j - 1])
                    if (j - i + 1 < ans) ans = j - i + 1, u = i, v = j;
            cout << u << ' ' << v << '\n';
        }
    }   
    return 0;
}
```
{% endfolding %}

#### D. Balance (Hard version)

{% noteblock %}
[Portal](https://codeforces.com/contest/1732/problem/D2).

最初你有一个集合，该集合仅包括一个元素 $0$，而且它会一直存在。你需要处理 $q(\le 2\times 10^5)$ 个下述类型的操作：

- `+ x` 向集合中添加一个整数 $x$ 。数据保证集合中原来没有这个整数。
- `- x` 从集合中移除整数 $x$ 。数据保证集合包含这个就要删除的整数。
- `? k` 找出当前是 $k$ 的倍数且不被包含在集合中的最小非负整数 $x$。
{% endnoteblock %}

考虑乱搞，用四个 `map` 分别记录：`s`，数是否在集合内；`ans`，用于类似离线直接跳到之前枚举的最大位置的 $ans$（即对于查询过的 $k$ 所查询到的 $ans$）；`del`，有哪些是被查询过的，但是后来被无情删除的；`vis`，有哪些数可以被作为 $k$ 的答案（查询的时候顺带统计）。

修改的时候要利用 `vis` 来更新 `del`。查询的时候一开始直接拿 $0$ 搞，之后再查就从 $ans[x]$ 开始暴力跳，并且在 $vis$ 中插入；最后再与被删除的当中（因为删除了，又可以取了）取个最小值。

时间复杂度的严格证明很困难，不打算研究了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <set> 
#include <map>

using namespace std;
typedef long long i64;

map <i64, i64> s, ans;
map <i64, set<i64>> del, vis;

int main(void) 
{
    int q; cin >> q;
    s[0] = 1;
    while (q--) {
        char opt; i64 x;
        cin >> opt >> x;
        if (opt == '+') {
            s[x] = true;
            for (auto i : vis[x])
                del[i].erase(x);
        } else if (opt == '-') {
            s[x] = false;
            for (auto i : vis[x])
                del[i].insert(x);
        } else {
            while (s[ans[x]]) {
                vis[ans[x]].insert(x);
                ans[x] += x;
            }
            i64 res = ans[x];
            if (!del[x].empty()) res = min(res, *del[x].begin());
            cout << res << '\n';
        }
    }
    return 0;
}
```
{% endfolding %}

#### E. Location

{% noteblock %}
[Portal](https://codeforces.com/contest/1732/problem/E).

初始给定序列 $a,b$，需要维护以下操作：

1. 给定 $l,r,x$，对于 $\forall i\in[l,r]$，令 $a_i=x$。
2. 给定 $l,r$，求 $\min_{i\in[l,r]}\frac{\operatorname{lcm}(a_i,b_i)}{\gcd(a_i,b_i)}$。

数据范围 $1\leq n,q,a_i\leq 5\times 10^4$。
{% endnoteblock %}

说点题外话，这道题对我来说很有实力，也让我更加期待 NOIP 结束后学习数据结构的生活了{% emoji 滑稽 %}。

看到这么诡异的查询当然分块。

这玩意相当于是求 $f(i)=\cfrac{a_i\times b_i}{\gcd(a_i,b_i)^2}$，显然一开始这玩意的值可以预处理出来。

现在考虑如何修改，零散块当然暴力维护，但是整块怎么办？注意这里修改操作的特殊性，是**区间赋值**，而且数的值域也很小，因此可以预处理 $v_{i,j}$ 代表 $i$ 块修改为 $j$ 之后的答案。

但是这样预处理的复杂度就太高了，需要考虑优化。方法是开一个桶 $t$，$t[x]$ 记录含有因子 $x$ 的最小 $b$，然后枚举每 $a$ 的因数，如果 $t[x]$ 存在就 $a$ 就可能是最大公约数，尝试更新。

基本上不卡常，只要块长不离谱就很容易过的。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <vector>
#include <cstring>
#pragma GCC optimize(3, "Ofast")

using namespace std;
typedef unsigned int uint;
const uint BLOCK_SIZE = 160;
const uint INF = 0x9f9f9f9f;

uint read(void) {
    uint x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar();
    return x;
}

void print(uint x) {
    if (x > 9) print(x / 10);
    putchar(x % 10 ^ 48);
}

uint n, q;
uint a[50005], b[50005];
uint L[334], R[334], minn[334];
uint setv[334], v[334][50005];
uint pos[50005];
vector<uint> d[50005];

uint gcd(uint x, uint y) {
    if (y == 0) return x;
    return gcd(y, x % y);
}

inline uint F(uint x) {
    uint g = gcd(a[x], b[x]);
    return a[x] * b[x] / (g * g); 
}

inline void pushdown(uint o) {
    if (!setv[o]) return;
    for (uint i = L[o]; i <= R[o]; ++i) a[i] = setv[o];
    setv[o] = 0;
}

inline void maintain(uint l, uint r, uint x) {
    uint p = pos[l]; pushdown(p);
    for (uint i = l; i <= r; ++i) a[i] = x;
    minn[p] = INF;
    for (uint i = L[p]; i <= R[p]; ++i) minn[p] = min(minn[p], F(i));
}

void update(uint l, uint r, uint x) 
{
    uint p = pos[l], q = pos[r];
    if (p == q) {
        maintain(l, r, x);
        return;
    }
    for (uint i = p + 1; i < q; ++i) setv[i] = x, minn[i] = v[i][x];
    maintain(l, R[p], x); maintain(L[q], r, x);
}

uint Query(uint l, uint r) {
    uint p = pos[l]; pushdown(p);
    uint ans = INF;
    for (uint i = l; i <= r; ++i) ans = min(ans, F(i));
    return ans;
}

uint query(uint l, uint r)
{
    uint p = pos[l], q = pos[r];
    if (p == q) return Query(l, r);
    uint ans = INF;
    for (uint i = p + 1; i < q; ++i) ans = min(ans, minn[i]);
    ans = min(ans, Query(l, R[p])); ans = min(ans, Query(L[q], r));
    return ans;
}

int main(void)
{
    n = read(), q = read();
    for (uint i = 1; i <= n; ++i) a[i] = read();
    for (uint i = 1; i <= n; ++i) b[i] = read();
    uint t = n / BLOCK_SIZE;
    for (uint i = 1; i <= t; ++i)
        L[i] = R[i - 1] + 1, R[i] = i * BLOCK_SIZE;
    if (R[t] < n) { ++t; L[t] = R[t - 1] + 1; R[t] = n; }
    for (uint i = 1; i <= t; ++i) {
        minn[i] = INF;
        for (uint j = L[i]; j <= R[i]; ++j)
            pos[j] = i, minn[i] = min(minn[i], F(j));
    }

    for (uint i = 1; i <= 50000; ++i) // 预处理约数
        for (uint j = 1; j * j <= i; ++j)
            if (i % j == 0) {
                d[i].emplace_back(j);
                if (j * j != i) d[i].emplace_back(i / j);
            }

    static uint tmp[50005]; // tmp[x] 记录含有因子 x 的最小 b
    memset(v, 0x9f, sizeof(v));
    for (uint i = 1; i <= t; ++i) { // 预处理修改后的答案
        memset(tmp, 0x9f, sizeof(tmp));
        for (uint j = L[i]; j <= R[i]; ++j)
            for (uint a : d[b[j]]) tmp[a] = min(tmp[a], b[j]);
        for (uint j = 1; j <= 50000; ++j)
            for (uint a : d[j]) // 枚举 a 是 j 的因子
                if (tmp[a] != INF) v[i][j] = min(v[i][j], j * tmp[a] / (a * a)); // a 可能是最大公约数，尝试更新
    }

    while (q--) {
        uint op = read(), l = read(), r = read(), x;
        if (op == 1) {
            x = read();
            update(l, r, x);
        } else {
            print(query(l, r));
            putchar('\n');
        }
    }
    return 0;
}
```
{% endfolding %}

### Codeforces Round #822 (Div.2)

[Portal](https://codeforces.com/contest/1734).

#### A. Select Three Sticks

[Portal](https://codeforces.com/contest/1734/problem/A).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
using i64 = long long;

int n;
int a[305];

inline void solve(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    sort(a + 1, a + n + 1);
    int ans = 2e9;
    for (int i = 2; i < n; ++i)
        ans = min(ans, a[i + 1] - a[i - 1]);
    cout << ans << endl;
}

int main(void)
{
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

#### B. Bright, Nice, Brilliant	GNU

[Portal](https://codeforces.com/contest/1734/problem/B).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
using i64 = long long;

inline void solve(void)
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= i; ++j)
            if (j == 1 || j == i) cout << "1 ";
            else cout << "0 ";
        putchar('\n');
    }
}

int main(void)
{
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

#### C. Removing Smallest Multiples

[Portal](https://codeforces.com/contest/1734/problem/C).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
using i64 = long long;

char s[1000005];

inline void solve(void)
{
    int n;
    i64 ans = 0;
    scanf("%d%s", &n, s + 1);
    for (int i = 1; i <= n; ++i)
    {
        for (int j = i; j <= n; j += i)
            if (s[j] != '1')
            {
                if (s[j] == '0') ans += i;
                s[j] = '2';
            }
            else break;
    }
    printf("%lld\n", ans);
}

int main(void)
{
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

#### D. Slime Escape

[Portal](https://codeforces.com/contest/1734/problem/D).

贪心的向左右走，只要是正的，就可以走，同时记录左右最大的和用以计算。

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

int n, k;
int a[200005];
i64 s[200005];

void solve(void)
{
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    if (a[k] < 0) return puts("NO"), void();
    s[k] = 0;
    for (int i = k - 1; i >= 1; --i) s[i] = s[i + 1] + a[i];
    for (int i = k + 1; i <= n; ++i) s[i] = s[i - 1] + a[i];
    int l = k - 1, r = k + 1;
    i64 sl = a[k], sr = a[k];
    while (l > 0 && r <= n) {
        int ll = l, rr = r;
        while (s[l] + sr >= 0 && l > 0) sl = max(sl, a[k] + s[l]), l -= 1;
        while (s[r] + sl >= 0 && r <= n) sr = max(sr, a[k] + s[r]), r += 1;
        if (ll == l && rr == r) return puts("NO"), void();
    }
    puts("YES");
}

int main(void)
{
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

### Codeforces Round #833 (Div.2)

[Portal](https://codeforces.com/contest/1748).

#### A. The Ultimate Square

[Portal](https://codeforces.com/contest/1748/problem/A).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

void solve(void) {
    int n;
    cin >> n;
    cout << (n + 1) / 2 << endl;   
}

int main(void) {
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

#### B. Diverse Substrings

[Portal](https://codeforces.com/contest/1748/problem/B).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

int a[15];

void solve(void) {
    int n, ans = 0;
    static char s[100005];
    cin >> n >> s;
    for (int i = 0; i < n; ++i) {
        memset(a, 0, sizeof(a));
        int maxx = 0, flag = 0;
        for (int j = i; j < n; ++j) {
            int x = s[j] - '0';
            if (a[x] == 0) ++flag;
            a[x] += 1;
            maxx = max(a[x], maxx);
            if (maxx <= flag) ++ans;
            if (a[s[j] - '0'] > 10) break;
        }
    }
    cout << ans << endl;
}

int main(void) {
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

#### C. Zero-Sum Prefixes

{% noteblock %}
[Portal](https://codeforces.com/contest/1748/problem/C).

给定一个长为 $n$ 的数列 $a$，你可以将其中的每个 $0$ 分别改成任意整数。求出你最多能让多少个 $k$ 满足 $a_1+\dots+a_k=0$。
{% endnoteblock %}

考虑 $0$ 干了什么，可以变动后面一段的前缀和，这样用一个 STL map 记录一下哪一个前缀和最多，然后就把这个 $0$ 变成这个的相反数。

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

int n;
int a[200005];
i64 s[200005];

void solve(void) {
    cin >> n;
    for (int i = 1; i <= n; ++i)
        cin >> a[i];
    int ans = 0, maxt = 0;
    bool fs = true;
    map<i64, int> se;
    for (int i = 1; i <= n; ++i) {
        if (a[i] == 0) {
            if (fs) fs = false;
            else {
                ans += maxt;
                maxt = 0; se.clear();
                s[i] = 0;
            }
        }
        s[i] = s[i - 1] + a[i];
        if (fs) ans += (s[i] == 0);
        else maxt = max(maxt, se[s[i]] += 1);
    }
    if (!fs) ans += maxt;
    cout << ans << '\n';
}

int main(void) {
    ios::sync_with_stdio(false);
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

### Codeforces Round #802 (Div.2)

[Portal](https://codeforces.com/contest/1700).

#### A. Optimal Path

[Portal](https://codeforces.com/contest/1700/problem/A).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

void solve(void) {
    i64 n, m;
    cin >> m >> n;
    cout << n * (n - 1) / 2 + (n + n * m) * m / 2 << "\n";
}

int main(void) {
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

#### B. Palindromic Numbers

[Portal](https://codeforces.com/contest/1700/problem/B).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

void solve(void) {
    int n;
    static char a[100005], b[100005], c[100005];
    cin >> n >> b;
    if (b[0] == '9') {
        for (int i = 0; i <= n; ++i) a[i] = 1;
        for (int i = 0; i < n; ++i) b[i] -= '0';
        for (int i = n - 1; i >= 0; --i)
            if (a[i] < b[i]) {
                a[i] += 10;
                a[i - 1] -= 1;
                c[i] = a[i] - b[i]; 
            } else c[i] = a[i] - b[i];
        for (int i = 0; i < n; ++i)
            putchar(c[i] + '0');
        return;
    }
    for (int i = 0; i < n; ++i)
        putchar('9' - b[i] + '0');
}

int main(void) {
    int T; cin >> T;
    while (T--) {
        solve();
        puts("");
    }
    return 0;
}
```
{% endfolding %}

#### C. Helping the Nature

[Portal](https://codeforces.com/contest/1700/problem/C).

{% noteblock %}
给定一个长度为 $n$ 的序列 $A$，支持以下三种操作：

- 将 $A_1,\dots,A_i$ 都减去一；
- 将 $A_i,\dots,A_n$ 都减去一；
- 全局加上一。
{% endnoteblock %}

将序列差分，那么前两种操作对应：

- $B_{i+1}$ 加上一；
- $B_{i}$ 减去一。

那么除了第一个数，剩下的所有数我们都可以将其变成 $0$，这样序列中的所有数都相等，把第一个数作为“标准”进行维护即可。

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

int n;
int a[200005], b[200005];

void solve(void) {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i]; 
        b[i] = a[i] - a[i - 1];
    }
    i64 ans = 0, h = a[1];
    for (int i = 2; i <= n; ++i) {
        ans += abs(b[i]);
        if (b[i] < 0) h += b[i];
    }
    cout << ans + abs(h) << "\n";
}

int main(void) {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

### Codeforces Round #813 (Div. 2)

[Portal](https://codeforces.com/contests/1712).

#### A. Wonderful Permutation

[Portal](https://codeforces.com/contest/1712/problem/A).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

int n, k;
int p[105];
bool a[105];

void solve(void)
{
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; ++i) scanf("%d", p + i);
	memset(a, 0, sizeof(a));
	for (int i = 1; i <= k; ++i)
		a[p[i]] = true;
	int ans = 0;
	for (int i = 1; i <= k; ++i)
		if (!a[i]) ++ans;
	cout << ans << endl;
}

int main(void)
{
	int T; scanf("%d", &T);
	while (T--) solve();
	return 0;
}
```
{% endfolding %}

#### B. Woeful Permutation

[Portal](https://codeforces.com/contest/1712/problem/B).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

int n;
int a[100005];

void solve(void)
{
	cin >> n;
	for (int i = 1; i <= n; ++i) a[i] = i;
	for (int i = n; i >= 1; i -= 2)
		if (i > 1) swap(a[i], a[i - 1]);
	for (int i = 1; i <= n; ++i)
		printf("%d ", a[i]);
	putchar('\n');
}

int main(void)
{
	int T; scanf("%d", &T);
	while (T--) solve();
	return 0;
}
```
{% endfolding %}

#### C. Sort Zero

[Portal](https://codeforces.com/contest/1712/problem/C).

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

using namespace std;
typedef long long i64;

int n;
int a[100005];
bool b[100005];

void solve(void)
{
	scanf("%d", &n);
	memset(b, 0, sizeof(b));
	set<int> s;
	int maxx = 0, ans = 0;
	for (int i = 1; i <= n; ++i) {
		scanf("%d", a + i);
		if (b[a[i]]) a[i] = 0;
		if (a[i] < maxx) {
			ans += s.size();
			for (auto x : s) b[x] = true;
			s.clear();
			maxx = 0;
		}
		if (b[a[i]]) a[i] = 0;
		if (a[i] != 0) s.insert(a[i]);
		maxx = max(maxx, a[i]);
	}
	cout << ans << '\n';
}

int main(void)
{
	int T; scanf("%d", &T);
	while (T--) solve();
	return 0;
}
```
{% endfolding %}

#### D. Empty Graph

{% noteblock %}
[Portal](https://codeforces.com/contest/1712/problem/D).

给定一个长为 $n$ 的序列 $a$。

定义一 $n$ 个点的无向完全图，点 $l$ 和点 $r$ 之间的距离为 $\min\limits_{i\in[l,r]}\{a_i\}$。

你可以进行 $k$ 次操作，每次操作可以选定 $\forall i \in [1,n]$ 并将 $a_i$ 赋值为一个 $[1,10^9]$ 的整数。请最大化这个图的直径。

设 $d(u,v)$ 表示 $u$ 到 $v$ 的最短路径长度，图的直径定义为 $\max\limits_{1\leq u < v \leq n} d(u,v)$。
{% endnoteblock %}

二分答案。直径显然只能是 $a_i,a_{i+1}$，距离要么是直接走，要么是全局最小，根据此 check 即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
const int INF = 1e9;

int n, k;
int a[100005], b[100005];

bool P(int x) // 最小值最大，现在最小值为 x 
{
	int res = 0;
	for (int i = 1; i <= n; ++i) b[i] = a[i];
	for (int i = 1; i <= n; ++i)
		if (2 * b[i] < x) ++res, b[i] = INF;
	int t = 2;
	for (int i = 1; i < n; ++i)
		t = min(t, (b[i] < x) + (b[i + 1] < x));
	return res + t <= k;
}

void solve(void) 
{
	cin >> n >> k;
	for (int i = 1; i <= n; ++i)
		cin >> a[i];
	int L = 0, R = INF + 1;
	while (L + 1 != R) {
		int mid = L + R >> 1;
		if (P(mid)) L = mid;
		else R = mid;
	}
	cout << L << '\n';
}

int main(void)
{
	ios::sync_with_stdio(false);
	int T; cin >> T;
	while (T--) solve();		
	return 0;
}
```
{% endfolding %}