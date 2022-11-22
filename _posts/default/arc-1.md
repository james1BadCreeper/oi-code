---
title: ATCoder Regular Contest 乱做记录
cover: false
categories:
    - 算法竞赛
    - 比赛记录
tag:
    - ATCoder
    - 刷
    - 笔记
plugins:
    - katex
headimg: 'https://pic1.imgdb.cn/item/636a0c7d16f2c2beb1e9cddb.jpg'
abbrlink: dae55f85
date: 2022-11-08 16:13:10
---

> 听说做点 AT 题非常好玩，于是就来了。但是笔者太菜了，所以都不会。

<!--more-->

## PART-A

james1 是菜狗。

### AtCoder Regular Contest 104

[Portal](https://atcoder.jp/contests/arc104).

#### A. Plus Minus

[Portal](https://www.luogu.com.cn/problem/AT_arc104_a).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int main(void) 
{
    int a, b;
    cin >> a >> b;
    cout << (a + b) / 2 << ' ' << (a - b) / 2 << endl;
    return 0;
}
```
{% endfolding %}

#### B. DNA Sequence

[Portal](https://www.luogu.com.cn/problem/AT_arc104_b).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, ans = 0;
char a[5005];
int cnt[4];

int main(void) 
{
    scanf("%d%s", &n, a + 1);
    for (int i = 1; i <= n; ++i) {
        memset(cnt, 0, sizeof(cnt));
        for (int j = i; j <= n; ++j) {
            if (a[j] == 'A') ++cnt[0];
            else if (a[j] == 'T') ++cnt[1];
            else if (a[j] == 'G') ++cnt[2];
            else ++cnt[3];
            if (cnt[0] == cnt[1] && cnt[2] == cnt[3]) ++ans;
        }
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### C. Fair Elevator

[Portal](https://www.luogu.com.cn/problem/AT_arc104_c).

先考虑几个基本条件：必须有 $b>a$，每一个位置最多有一个人上下，否则显然不合法。

发现 $n\le 100$，因此可以想的暴力一点。想要给 $-1$ 的位置填数字不太好搞，那么就依次考虑楼层，给楼层安排上这一层上或者是下的人。

设 $f(i)$ 代表考虑前 $i$ 层楼是否可以做到合法，初始时 $f(0)=1$，目标为 $f(2n)$。转移显然是：

$$
f(i)=\vee\{f(j-1)\wedge \text{calc}(j,i)\}(1\le j<i,2\mid i-j+1)
$$

注意区间长度一定要是 $2$ 的倍数，否则是不可能合法的（根本填不进去）。

现在问题就是如何实现判断区间合法的 `calc` 函数。显然这段区间必须是"封闭"的当中的任何位置都不可以跑到区间外面去。再就是必须在前半段上电梯，后半段下电梯，否则必定可以划分成更小的区间。此时上下电梯的位置必须是前半段中的第 $i$ 个和后半段中的第 $i$ 个，这样才能保证坐电梯的层数是相等的。

时间复杂度 $O(n^3)$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
int a[105], b[105];
int cnt[205], p[205];
bool f[205];

bool calc(int L, int R)
{
    for (int i = L; i <= R; ++i) {
        // 在此处下，上来的地方小于 L
        if (p[i] < 0 && a[-p[i]] != -1 && a[-p[i]] < L) return false;
        // 在此处上，下去的地方大于 R
        if (p[i] > 0 && b[p[i]] != -1 && b[p[i]] > R) return false;
    }
    int half = (R - L + 1) >> 1;
    for (int i = L; i <= L + half - 1; ++i) {
        if (p[i] < 0) return false; // 前半段下，不行
        if (p[i + half] > 0) return false; // 后半段上，不行
        if (p[i] && p[i + half] && p[i] + p[i + half]) return false; // 距离不相等，不行
    }
    return true;
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d", a + i, b + i);
        if (a[i] != -1 && b[i] != -1 && a[i] >= b[i]) return puts("No"), 0;
        if (a[i] != -1) ++cnt[a[i]], p[a[i]] = i;
        if (b[i] != -1) ++cnt[b[i]], p[b[i]] = -i;
    }
    n <<= 1;
    for (int i = 1; i <= n; ++i)
        if (cnt[i] > 1) return puts("No"), 0;
    f[0] = 1;
    for (int i = 1; i <= n; ++i)
        for (int j = ((i & 1) ? 2 : 1); j <= i; j += 2)
            if (f[j - 1]) f[i] |= calc(j, i);
    puts(f[n] ? "Yes" : "No");
    return 0;
}
```
{% endfolding %}

### AtCoder Regular Contest 143

[Portal](https://atcoder.jp/contests/arc143).

#### A. Three Integers

[Portal](https://www.luogu.com.cn/problem/AT_arc143_a).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
typedef long long i64;

int main(void) {
    i64 a[3];
    cin >> a[0] >> a[1] >> a[2];
    sort(a, a + 3);
    if (a[0] + a[1] < a[2]) puts("-1");
    else printf("%lld\n", a[2]);
    return 0;
}
```
{% endfolding %}

#### C. Piles of Pebbles

[Portal](https://www.luogu.com.cn/problem/AT_arc143_c).

分类讨论。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, x, y;
int a[200005];

int main(void) {
	scanf("%d%d%d", &n, &x, &y);
	bool flag = 0, g = 0;
	for (int i = 1; i <= n; ++i) {
		scanf("%d", a + i);
		a[i] %= x + y;
		if (a[i] != 0) flag = true;
	}
	if (!flag) return puts("Second"), 0;
	if (x <= y) puts("First");
	else {
		flag = 0;
		for (int i = 1; i <= n; ++i)
			if (a[i] < x) flag = true;
		if (flag) puts("Second");
		else puts("First");
	}
	return 0;
}
```
{% endfolding %}

### AtCoder Regular Contest 145

[Portal](https://atcoder.jp/contests/arc145).

#### A. AB Palindrome

[Portal](https://www.luogu.com.cn/problem/AT_arc145_a).

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
char s[200005];

int main(void)
{
    scanf("%d%s", &n, s);
    if (s[0] == 'A' && s[n - 1] == 'A') puts("Yes");
    else if (s[0] == 'A' && s[n - 1] == 'B') puts("No");
    else if (s[0] == 'B' && s[n - 1] == 'A') 
    {
        if (n > 2) puts("Yes");
        else puts("No");
    }
    else puts("Yes");
    return 0;
}
```
{% endfolding %}

#### B. AB Game

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/AT_arc145_b).

$n$ 轮游戏，第 $i$ 轮有 $i$ 个石子。Alice 先手，每次可以取 $a$ 的倍数的石子，Bob 后手，可以取 $b$ 的倍数的石子。谁取不了石子，谁就输。问 Alice 赢几场。
{% endnoteblock %}

可以肯定，能取多少就取多少。

- 当 $n<a$ 时，显然直接死。
- 当 $a<b$ 时，除了一开始 $n-1$ 轮，剩下都赢。
- 当 $a\ge b$ 时，可以发现有胜利循环节。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int main(void) {
    i64 n, a, b;
    cin >> n >> a >> b;
    if (n < a) puts("0");
    else if (n == a) puts("1");
    else if (a < b) printf("%lld\n", n - a + 1);
    else {
        i64 ans = 1;
        n -= a;
        ans += n / a * b;
        n %= a;
        ans += min(n, b - 1);
        printf("%lld\n", ans);
    }
    return 0;
}
```
{% endfolding %}

#### C. Split and Maximize

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/AT_arc145_c).

对于一个 $1\sim2n$ 的排列 $[p_{1},p_{2},\dots,p_{2n}]$，考虑将 $P$ 拆分为两个子序列 $[a_1,a_2,\dots,a_n]$ 和 $[b_1,b_2,\dots,b_n]$，$P$ 的分数定义为所有拆分方案中的 $\sum a_i b_i$ 的最大值。请求出分数取到最大值的排列的数量，对 $998244353$ 取模。
{% endnoteblock %}

显然只能 $(1,2),(3,4)\dots (2n-1,2n)$ 这样配对，而每一个配对的左右顺序是不影响的，所以答案基数为 $2^n$。

而且要保证是子序列，所以相当于在 $2n$ 中选择 $n$ 个位置进行放置，方案数为 $\binom{2n}{n}$。

所以答案为 $2^n\binom{2n}{n}$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;
const int MOD = 998244353;

int poww(int a, int b) {
    int res = 1;
    for (; b; b >>= 1, a = 1ll * a * a % MOD)
        if (b & 1) res = 1ll * res * a % MOD;
    return res;
}

int main(void) {
    int n; cin >> n;
    i64 ans = poww(2, n);
    for (int i = n + 2; i <= (n << 1); ++i)
        ans = ans * i % MOD;
    cout << ans << endl;
    return 0;
}
```
{% endfolding %}

#### D. Non Arithmetic Progression Set

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/AT_arc145_d).

构造一个有 $n$ 个整数的集合 $S$，满足所有数的和为 $M$，而且每个数的值域均为 $[-10^7,10^7]$ 中，并且对于任意 $x,y,z\in S, x<y<z$，都有 $y-x\neq z-y$。
{% endnoteblock %}

首先，$y-x\neq z-y$ 相当于 $x+z\neq 2y$。

我们先抛开和为 $M$ 的限制，考虑如何搞定最后那个不等的限制条件。   
这里先给出结论：将数写成三进制，当数的所有位都是 $0,1$ 时可以满足。   
为什么呢？考虑 $x+z=2y$，$y$ 乘上 $2$ 之后每一位都是 $0$ 或 $2$，而 $x$ 和 $z$ 必定有一位不一样（否则就相等了），加上之后肯定有一位是 $1$，必定不等于 $2y$。所以不等关系永远满足。

现在假定我们构造出来的集合可以表示为递增序列 $a$，然后进行下面对 $M$ 的限制的讨论。

我们之前构造出来的集合已经有了一个和 $s$，不妨设 $s<M$（反过来的时候大致同理），那么只要给 $a_n$ 加上 $M-s$ 就满足了（本来就最大还加上一个数，不可能使 $x+z=2y$ 重新成立）。但是！本题的值域限制非常苛刻，这样会超出值域的。   

那么考虑均摊一下，集合中的每个数都加上同一个数的话，集合中的不等关系还会满足。我们要保证均摊之后，$a_n$ 的大小依然足够大，才可以使得不等关系成立。也就是说均摊的时候，要给 $a_n$ 留下足够的大小。构造时注意不要超出值域，大概是 $a_n$ 的两倍以上就可以了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
typedef long long i64;

int n, a[10005];
i64 m, s = 0;

int main(void) {
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int x = 0;
        for (int j = 0, z = 1; j < 15; ++j, z *= 3)
            if ((i >> j) & 1) x += z;
        a[i + 1] = x; s += a[i + 1];
    }

    i64 diff = m - s, buf = 0;
    if (diff < 0) {
        if (diff < -2 * a[n]) {
            buf = (2 * a[n] + diff) / n;
            diff -= buf * n;
        }
        a[1] += diff;
    } else if (diff > 0) {
        if (diff > 2 * a[n]) {
            buf = (diff - 2 * a[n]) / n;
            diff -= buf * n;
        }
        a[n] += diff;
    }

    for (int i = 1; i <= n; ++i)
        printf("%d ", a[i] + buf);
    putchar('\n');
    return 0;
}
```
{% endfolding %}

### AtCoder Regular Contest 147

[Portal](https://atcoder.jp/contests/arc147).

#### A. Max Mod Min

[Portal](https://www.luogu.com.cn/problem/AT_arc147_a).

可以预计答案不会很大，直接模拟。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <set>

using namespace std;

int main(void) {
    ios::sync_with_stdio(false);
    int n, x; multiset <int, greater<int>> s;
    cin >> n;
    while (n--) {
        cin >> x;
        s.insert(x);
    }
    int res = 0;
    while (s.size() > 1) {
        auto i = s.end(), j = s.begin();
        --i;
        int t = (*j) % (*i);
        s.erase(j);
        if (t) s.insert(t);
        ++res;
    }
    printf("%d\n", res);
    return 0;
}
```
{% endfolding %}

#### B. Swap to Sort

{% noteblock %}
[Portal](https://www.luogu.com.cn/problem/AT_arc147_b).

现有一个 $1$ 到 $N$ 的排列 $P=(P_1,P_2,\ldots,P_N)$。你可以重复执行以下两种操作来使$P$从小到大排序。

- 操作 $A:$ 选择一个整数 $i$ 满足 $1\ \leq\ i\ \leq\ N-1$，然后交换 $P_i$ 和 $P_{i+1}$。
- 操作 $B:$ 选择一个整数 $i$ 满足 $1\ \leq\ i\ \leq\ N-2$，然后交换 $P_i$ 和 $P_{i+2}$。

请找出一个操作序列满足操作 $A$ 的数量最少。
{% endnoteblock %}

只要奇数都在奇数位置，偶数都在偶数位置，那么只需要 `2` 操作就可以完成。因此我们只需要 `1` 操作修正奇偶性。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
typedef pair<int,int> pii;

int n;
int a[405];
vector <pii> ans;

void f(int ty, int x) {
	ans.emplace_back(make_pair(ty, x));
	swap(a[x], a[x + ty]);
}

int main(void) {
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) scanf("%d", a + i);
	// 奇偶性修正 
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n - 2; ++j)
			if (a[j] % 2 != a[j + 2] % 2 && a[j] % 2 != j % 2) f(2, j);
	for (int i = 1; i < n; ++i)
		if (a[i] % 2 != i % 2) f(1, i);
	// 维护大小 
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n - 2; ++j)
			if (a[j] > a[j + 2]) f(2, j);
	cout << ans.size() << endl;
	for (auto x : ans)
		printf("%c %d\n", x.first == 1 ? 'A' : 'B', x.second);
	return 0;
}
```
{% endfolding %}