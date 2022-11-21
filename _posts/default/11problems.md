---
title: 十一月题目选做
cover: false
plugins:
  - katex
headimg: 'https://pic1.imgdb.cn/item/636611a516f2c2beb1abb72e.jpg'
categories:
  - 算法竞赛
  - 做题记录
tag:
  - 刷
status: working
abbrlink: 75849b25
date: 2022-11-04 21:08:21
---

> 刷一点题！

<!--more-->

## 模拟

模拟。

### 数学模拟

有数的模拟。

#### [UVA11582] Colossal Fibonacci Numbers!

[Portal](https://www.luogu.com.cn/problem/UVA11582).

寻找循环节！

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef unsigned long long u64;

int poww(u64 a, u64 b, int MOD) {
    a %= MOD; int res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res % MOD;
}

int n, f[1000010] = {0, 1, 1};
u64 a, b;
void MAIN(void) {
    scanf("%llu%llu%d", &a, &b, &n);
    if (n == 1 || a == 0) return puts("0"), void();
    for (int i = 3;; ++i) {
        f[i] = (f[i - 1] + f[i - 2]) % n;
        if (f[i] == 1 && f[i - 1] == 1) {
            printf("%d\n", f[poww(a, b, i - 2)]);
            return;
        }
    }
}

int main(void) {
    int T; scanf("%d", &T);
    while (T--) MAIN();
    return 0;
}
```
{% endfolding %}

#### [UVA12716] GCD XOR

[Portal](https://www.luogu.com.cn/problem/UVA12716).

打表找规律发现 $\gcd(a,b)=a\text{ xor } b=c\Longrightarrow c=a-b$，因此枚举 $a,c$，验证 $b$ 即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int N = 3e7;

int f[N + 5];

int main(void) {
    for (int c = 1; c <= N / 2; ++c)
        for (int a = 2 * c; a <= N; a += c) {
            int b = a - c;
            if ((a ^ b) == c) ++f[a];
        }
    for (int i = 2; i <= N; ++i) f[i] += f[i - 1];

    int T; scanf("%d", &T);
    for (int kase = 1; kase <= T; ++kase) {
        int x;
        scanf("%d", &x);
        printf("Case %d: %d\n", kase, f[x]);
    }
    return 0;
}
```
{% endfolding %}

#### [蓝桥杯 2022 国 C] 取模

[Portal](https://www.luogu.com.cn/problem/P8807).

> 给定 $n, m(\le 10^9)$，问是否存在两个不同的数 $x,y$ 使得 $1 \le x < y \le m$ 且 $n \bmod x = n \bmod ⁡y$。

实际上直接枚举即可，可以证明答案不会很大。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <set>

using namespace std;

int main(void) {
    int T; scanf("%d", &T);
    while (T--) {
        int n, m;
        scanf("%d%d", &n, &m);
        bool flag = false;
        set <int> s;
        for (int i = 1; i <= m; ++i)
            if (s.count(n % i)) {
                flag = true;
                break;
            } else s.insert(n % i);
        puts(flag ? "Yes" : "No");
    }
    return 0;
}
```
{% endfolding %}

#### [UVA11489] Interger Game

[Portal](https://www.luogu.com.cn/problem/UVA11489).

和是 $3$ 的倍数！

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main(void)
{
	int T, n;
	static char s[1005];
	scanf("%d", &T);
	for (int kase = 1; kase <= T; ++kase) {
		scanf("%s", s + 1); n = strlen(s + 1);
		if (n == 1) {
			printf("Case %d: S\n", kase);
			continue;
		}
		int cnt = 0, res = 0;
		for (int i = 1; i <= n; ++i) {
			s[i] -= '0';
			res += s[i];
			cnt += (s[i] % 3 == 0);
		}
		bool flag = false;
		for (int i = 1; i <= n; ++i)
			if ((res - s[i]) % 3 == 0) {
				if (s[i] % 3 == 0) cnt -= 1;
				flag = true;
				break;
			}
		if (!flag || cnt % 2) printf("Case %d: T\n", kase);
		else printf("Case %d: S\n", kase);
	}
	return 0;
}
```
{% endfolding %}

### 奇怪的模拟

无法分类。

#### [CSP-S 2022] 策略游戏

[Portal](https://www.luogu.com.cn/problem/P8818).

大型分类讨论现场。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;
const i64 INF = 2e18;

int n, m, q;
int a[100005], b[100005];

namespace Subtask2 {
    const int INF = 2e9;
    struct Node {
        int minn, maxx;
        int maxmi, minp;
        Node() : minn(INF), maxx(-INF), maxmi(-INF), minp(INF) {}
    };
    inline Node hb(const Node &a, const Node &b) {
        Node c;
        c.minn = min(a.minn, b.minn);
        c.maxx = max(a.maxx, b.maxx);
        c.maxmi = max(a.maxmi, b.maxmi);
        c.minp = min(a.minp, b.minp);
        return c;
    }
    struct SegmentTree {
        Node T[400005];
        int *v;
        void build(int o, int l, int r) {
            if (l == r) {
                T[o].minn = T[o].maxx = v[l];
                if (v[l] >= 0) T[o].minp = v[l];
                if (v[l] <= 0) T[o].maxmi = v[l];
                return;
            }
            int mid = l + r >> 1;
            build(o << 1, l, mid);
            build(o << 1 | 1, mid + 1, r);
            T[o] = hb(T[o << 1], T[o << 1 | 1]);
        }
        Node query(int o, int l, int r, int x, int y) {
            if (x <= l && r <= y) return T[o];
            int mid = l + r >> 1; Node res;
            if (x <= mid) res = hb(res, query(o << 1, l, mid, x, y));
            if (mid < y) res = hb(res, query(o << 1 | 1, mid + 1, r, x, y));
            return res;
        }
    }A, B;
    void MAIN(void) {
        A.v = a, B.v = b;
        A.build(1, 1, n); B.build(1, 1, n);
        while (q--) {
            int l1, r1, l2, r2;
            scanf("%d%d%d%d", &l1, &r1, &l2, &r2);
            Node x = A.query(1, 1, n, l1, r1), y = B.query(1, 1, n, l2, r2);
            i64 ans = -2e18;
            ans = max(ans, 1ll * x.maxx * (x.maxx >= 0 ? y.minn : y.maxx));
            ans = max(ans, 1ll * x.minn * (x.minn >= 0 ? y.minn : y.maxx));
            if (x.minp != INF) ans = max(ans, 1ll * x.minp * y.minn);
            if (x.maxmi != -INF) ans = max(ans, 1ll * x.maxmi * y.maxx);
            printf("%lld\n", ans);
        }
    }
}

int main(void) {
    scanf("%d%d%d", &n, &m, &q);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    for (int i = 1; i <= m; ++i) scanf("%d", b + i);
    Subtask2::MAIN();
    return 0;
}
```
{% endfolding %}

## DP

动态规划。

### 简单 DP

大概是 KH $114514^{-1919810}\text{ms}$ 一道的难度。

#### [UVA10003] Cutting Sticks

[Portal](https://www.luogu.com.cn/problem/UVA10003).

区间 DP 模板。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int l, n;
int a[55], f[55][55];

int main(void) {
    while (scanf("%d", &l) == 1 && l) {
        scanf("%d", &n);
        memset(f, 0x3f, sizeof(f));
        for (int i = 1; i <= n; ++i) 
            scanf("%d", a + i);
        a[n + 1] = l;
        for (int len = 2; len <= n + 2; ++len)  
            for (int i = 0; i <= n - len + 2; ++i) {
                int j = i + len - 1;
                if (len == 2) {
                    f[i][j] = 0;
                    continue;
                }
                for (int k = i + 1; k < j; ++k)
                    f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
                f[i][j] += a[j] - a[i];
            }
        printf("The minimum cutting is %d.\n", f[0][n + 1]);
    }
    return 0;
}
```
{% endfolding %}

#### [UVA1626] Brackets sequence

[Portal](https://www.luogu.com.cn/problem/UVA1626).

还是区间 DP 模板。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n;
char s[110];
int f[105][105];

bool check(int i, int j) {
    if (s[i] == '(' && s[j] == ')') return true;
    return s[i] == '[' && s[j] == ']';
}

void print(int i, int j) {
    if (i > j) return;
    if (i == j) {
        if (s[i] == '(' || s[i] == ')') printf("()");
        else printf("[]");
        return;
    }
    int ans = f[i][j];
    if (check(i, j) && ans == f[i + 1][j - 1]) {
        printf("%c", s[i]);
        print(i + 1, j - 1);
        printf("%c", s[j]);
        return;
    }
    for (int k = i; k < j; ++k)
        if (ans == f[i][k] + f[k + 1][j]) {
            print(i, k);
            print(k + 1, j);
            return;
        }
}

int main(void) {
    int T; scanf("%d", &T);
    fgets(s, 105, stdin);
    while (T--) {
        fgets_unlocked(s + 1, 105, stdin);
        fgets_unlocked(s + 1, 105, stdin);
        n = strlen(s + 1) - 1;
        memset(f, 0x3f, sizeof(f));
        for (int i = 1; i <= n; ++i)
            f[i][i] = 1, f[i + 1][i] = 0;
        for (int len = 2; len <= n; ++len)
            for (int i = 1; i <= n - len + 1; ++i) {
                int j = i + len - 1; f[i][j] = len;
                if (check(i, j)) f[i][j] = min(f[i][j], f[i + 1][j - 1]);
                for (int k = i; k < j; ++k)
                    f[i][j] = min(f[i][j], f[i][k] + f[k + 1][j]);
            }
        print(1, n);
        putchar('\n');
        if (T) putchar('\n');
    }
    return 0;
}
```
{% endfolding %}

## 图论

图论。

### 树

树形问题。

#### [APIO2007] 风铃

[Portal](https://www.luogu.com.cn/problem/P3621).

树上深度优先遍历。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int INF = 1e9;

int n, maxdep = -INF, mindep = INF;
int a[100005][2];

void dfs(int x, int dep) {
    if (x == -1) return mindep = min(dep, mindep), maxdep = max(dep, maxdep), void();
    dfs(a[x][0], dep + 1); dfs(a[x][1], dep + 1);   
}

bool flag = false;
int ans = 0;
int solve(int o, int dep) { // 0 全浅，1 全深，2 都有
    if (o == -1) return dep != mindep;
    int x = solve(a[o][0], dep + 1), y = solve(a[o][1], dep + 1);
    ans += ((x == 0 && y) || (x == 2 && y == 1)); // 如果左面全浅，或者右面全身，需要一次
    if (x == 2 && y == 2) exit((puts("-1"), 0)); // 两个都是都有，GG
    if (x == 2 || y == 2 || x + y == 1) return 2;
    if (x + y == 0) return 0;
    return 1;
}

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%d%d", &a[i][0], &a[i][1]);
    dfs(1, 0);
    if (maxdep == mindep) return puts("0"), 0;
    if (maxdep - mindep > 1) return puts("-1"), 0;
    solve(1, 0);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

## 数学

数学题。

### 数论

用到数论知识的简单题。

#### [UVA10791] Minimum Sum LCM

[Portal](https://www.luogu.com.cn/problem/UVA10791).

唯一分解定理。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef unsigned long long u64;

u64 n, kase = 0;
u64 calc(void) {
    if (n == 1) return 2;
    u64 ans = 0, flag = 0;
    for (u64 i = 2; i * i <= n; ++i)
        if (n % i == 0) {
            u64 res = 1;
            while (n % i == 0) res *= i, n /= i;
            ans += res;
            flag++;
        }
    if (n > 1) ans += n, ++flag;
    return ans + (flag == 1);
}

int main(void) {
    while (scanf("%llu", &n) == 1 && n)
        printf("Case %llu: %llu\n", ++kase, calc());
    return 0;
}
```
{% endfolding %}