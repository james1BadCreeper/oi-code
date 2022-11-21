---
title: 块状数据结构
plugins:
  - katex
headimg: 'https://z3.ax1x.com/2021/11/07/I1PbPH.jpg'
categories:
    - 算法竞赛
    - 学习笔记
    - 数据结构
tag:
    - 分块
    - 数据结构
hide: true
status: working
group: notes
abbrlink: f2886e9e
date: 2022-08-22 14:42:52
cover: false
---

> 分块（又称根号算法，因为单次操作复杂度是根号级别），是一种重要的思想，可以用来解决较为简单的题目，也可以用来解决各种可怕的问题。尽管如此，分块的思想不难理解。

<!--more-->

大家在数据结构中应该都听说过分块，因为嵌套数据结构、分块数据结构和可持久化数据结构背称为数据结构三大~~毒瘤~~重要思想。但实际上，除了在数据结构中的应用，分块在其它方面也有许多应用。

## 分块基础

分块的基本思想是，通过对原数据的适当划分，并在划分后的每一个块上预处理部分信息，从而较一般的暴力算法取得更优的时间复杂度。

这么说很空虚，我们通过几道入门题来一一看一下分块的简单应用。

### 数列分块入门

这是 Loj 上的一个系列题。但是前两个都比较简单，所以这里不写了，大家可以学习完之后写一下练习练习，我们先做一下最为经典的 3。

[Portal](https://loj.ac/p/6280).

~~啊，我知道你会树状数组和线段树，我们今天要用分块以龟速解决这道题。~~

我们把序列 $A$ 分成若干个长度块，如下图：

{% image https://s4.ax1x.com/2022/01/22/7fawMq.png::alt=分块后的序列 %}

具体怎么分呢？把若干个数分成一块。我们记录每个点的数值，再记录每个块的和（记作 $sum[i]$），还要给每个块打上一个标记 $add[i]$（类似于线段树的延迟标记，而且标记是永久的，否则你往哪传{% emoji 滑稽 %}）。

在进行操作的时候，我们把连续的块称之为**整块**，两边的不完整的块称为**零散块**。

对于修改操作，分两种情况进行：

- 当 $x$ 和 $y$ 处于同一段时，直接暴力加，然后令这一段 $sum[i]+=k\times(y-x+1)$；
- 否则，设 $x$ 处于第 $p$ 段，$r$ 处于第 $q$ 段。对于 $i\in [p+1,q-1]$，令 $add[i]+=k$。对于开头、结尾不足一整段的两部分，按照前一种情况暴力更新。

对于查询操作，也分两种情况：

- 当 $x$ 和 $y$ 处于同一段时，$\sum\limits_{i=x}^{y}+add[i]\times (y-x+1)$ 就是答案。
- 否则，设 $x$ 处于第 $p$ 段，$r$ 处于第 $q$ 段。记 $ans=0$。对于 $i\in [p+1,q-1]$，令 $ans+=sum[i]+add[i]\times len[i]$，$len$ 表示这一段的长度。对于开头、结尾不足一整段的两部分，按照前一种情况暴力查询。

当块的长度较长时，操作的暴力会比较慢（全堆在一个块里）；块的长度较短时，维护部分会比较慢（块变多了，改的 $add$ 标记数量变多了）。

根据经验，当块的大小在序列长度的算术平方根时会比较快，证明可以采用均值不等式，设块的大小为 $t$：

$$
t+\frac{n}{t}\ge 2\sqrt{t\times\frac{n}{t}} = 2\sqrt{n}
$$

当且仅当 $t=\frac{n}{t}\iff t^2=n$ 时等号成立。

然而在某些毒瘤题中，还可能需要通过实验确定最好的块值（我们接下来会看到的{% emoji 滑稽 %}）。

{% divider flowers %}

接下来就到了愉快的写代码时间，基本结构是这样：

```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
using i64 = long long;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m, t;
i64 a[100005], sum[100005], add[100005]; // 原序列，块和，延迟标记
int pos[100005], L[100005], R[100005]; // 元素所在额块的编号，块的左右端点

inline void update(int l, int r, i64 k)
{
    // do update
}

inline i64 query(int l, int r)
{
    // do query
}

int main(void)
{
    n = read(), m = read();
    t = sqrt(n); // 分块
    for (int i = 1; i <= n; ++i) a[i] = read();
    for (int i = 1; i <= t; ++i) // 先处理好这 t 个块的左右端点
    {
        L[i] = (i - 1) * t + 1;
        R[i] = i * t;
    }
    if (R[t] < n) ++t, L[t] = R[t - 1] + 1, R[t] = n; // 最后剩一小段不够凑成完整的块
    //========== 预处理 ========== start >
    for (int i = 1; i <= t; ++i)
        for (int j = L[i]; j <= R[i]; ++j)
        {
            pos[j] = i;
            sum[i] += a[j];
        }
    //========== 预处理 ========== end >
    while (m--)
    {
        int op = read(), x = read(), y = read();
        if (op == 1) update(x, y, read());
        else printf("%lld\n", query(x, y));
    }
    return 0;
}
```

实际上 `sum, L, R` 数组无需开这么大，但已经没有优化的必要了。

两个操作的代码如下：

```cpp
void update(int l, int r, i64 k)
{
    int p = pos[l], q = pos[r];
    if (p == q) // 同个块内，暴力
    {
        for (int i = l; i <= r; ++i) a[i] += k;
        sum[p] += (r - l + 1) * k;
        return;
    }
    for (int i = p + 1; i < q; ++i) add[i] += k; // 大块打标记
    for (int i = l; i <= R[p]; ++i) a[i] += k; // 左
    sum[p] += (R[p] - l + 1) * k; // 和
    for (int i = L[q]; i <= r; ++i) a[i] += k; // 右
    sum[q] += (r - L[q] + 1) * k; // 和
}

i64 query(int l, int r)
{
    // 同 update
    int p = pos[l], q = pos[r];
    i64 ans = 0;
    if (p == q)
    {
        for (int i = l; i <= r; ++i) ans += a[i];
        ans += add[p] * (r - l + 1);
        return ans;
    }
    for (int i = p + 1; i < q; ++i) ans += sum[i] + add[i] * (R[i] - L[i] + 1);
    for (int i = l; i <= R[p]; ++i) ans += a[i];
    ans += add[p] * (R[p] - l + 1);
    for (int i = L[q]; i <= r; ++i) ans += a[i];
    ans += add[q] * (r - L[q] + 1);
    return ans;
}
```

时间复杂度为 $\mathcal{O}(m\sqrt{n})$。

{% divider books %}

上述做法还可以优化，不过时间复杂度并没有变。   
优化的方式是前缀和，大体思路是明确的。然而在有修改的情况下，前缀和不方便维护，只能维护单个块内的前缀和，这样零散块的查询就可以使用前缀和了。

### 复杂度分析

分块的复杂度要从查询和修改，整块和零散块来看。比如刚才的题，若将序列分为 $t$ 块：

- 查询：
    - 整块 $O(t)$，
    - 零散块 $O(\frac{n}{t})$，
- 修改
    - 整块 $O(t)$，
    - 零散块 $O(\frac{n}{t})$。

## 块状数组

刚才的分块是针对数组的，这类分块我们统称为块状数组。接下来我们将见识更多的题目来详细认识块状数组。

### 基础训练

这些题都很基础。

#### [Luogu P2801] 教主的魔法

[Portal](https://www.luogu.com.cn/problem/P2801).

{% noteblock %}
区间加，查询区间大于等于某个数的数的个数。
{% endnoteblock %}

树状数组维护的是前缀和，线段树维护的信息满足大区间的解可以由小区间合并得到（区间可加性）。但是很可惜，这么奇怪的查询内容虽然满足区间可加性，但是每一个不同的 `c` 值需要有不同的线段树，所以结论就是用普通线段树做就是送猪头{% emoji 滑稽 %}。

但是树套树是可以维护这种复杂信息的，然而树套树做不到区间修改，所以使用分块来解决。

分块之后，我们看一看要怎么查询。我们将每一个块作为一个子问题分别求解。先将这个块排好序，那么查询时就能快很多（直接二分）。什么时候排序？显然是修改的时候，而且只有暴力修改的部分（因为只有它的顺序会变化）。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, q, t;
int L[1000005], R[1000005], pos[1000005];
int a[1000005], T[1000005], add[1000005];

inline void calc(int p)
{
    for (int i = L[p]; i <= R[p]; ++i) T[i] = a[i];
    sort(T + L[p], T + R[p] + 1);
}

inline void update(int l, int r, int k)
{
    int p = pos[l], q = pos[r];
    if (p == q)
    {
        for (int i = l; i <= r; ++i) a[i] += k;
        calc(p);
        return;    
    }
    for (int i = l; i <= R[p]; ++i) a[i] += k;
    for (int i = L[q]; i <= r; ++i) a[i] += k;
    for (int i = p + 1; i < q; ++i) add[i] += k;
    calc(p), calc(q);
}

inline int answer(int l, int r, int c)
{
    int ans = 0, p = pos[l], q = pos[r];
    if (p == q)
    {
        for (int i = l; i <= r; ++i)
            if (a[i] + add[p] >= c) ++ans;
        return ans;
    }
    for (int i = l; i <= R[p]; ++i)
        if (a[i] + add[p] >= c) ++ans;
    for (int i = L[q]; i <= r; ++i)
        if (a[i] + add[q] >= c) ++ans;
    for (int i = p + 1; i < q; ++i)
        ans += R[i] - (lower_bound(T + L[i], T + R[i] + 1, c - add[i]) - T) + 1;
    return ans;
}

int main(void)
{
    n = read(), q = read();
    t = sqrt(n);
    for (int i = 1; i <= n; ++i) a[i] = read();
    for (int i = 1; i <= t; ++i)
    {
        L[i] = (i - 1) * t + 1;
        R[i] = i * t;
    }
    if (R[t] < n)
    {
        R[++t] = n;
        L[t] = R[t - 1] + 1;
    }
    for (int i = 1; i <= t; ++i)
    {
        for (int j = L[i]; j <= R[i]; ++j)
            pos[j] = i;
        calc(i);
    }
    while (q--)
    {
        char op[3];
        scanf("%s", op);
        if (op[0] == 'M')
        {
            int l = read(), r = read(), k = read();
            update(l, r, k);
        }
        else
        {
            int l = read(), r = read(), c = read();
            printf("%d\n", answer(l, r, c));
        }
    }
    return 0;
}
```
{% endfolding %}

我们设把原序列分成 $x$ 个块，那么查询的时候：

- 整块 $O\left(x\log \cfrac{n}{x}\right)$，
- 零散块 $O\left(\cfrac{n}{x}\right)$。

修改复杂度：

- 整块 $O(1)$，
- 零散块 $O\left(\cfrac{n}{x}\right)$。

当块的大小为 $\sqrt{n\log n}$ 的时候，时间复杂度可以达到利用这种做法的最优的 $O(m\sqrt{n\log n}$。

同样，你也可以通过实验来确定更好的块长。

#### [Loj 6279] 数列分块入门 3

[Portal](https://loj.ac/p/6279).

和上一题差不多，读者可以自行练习一下。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;

int n;
int a[100005], T[100005], add[1005];
int pos[100005], L[1005], R[1005];

void maintain(int p)
{
	for (int i = L[p]; i <= R[p]; ++i) T[i] = a[i];
	sort(T + L[p], T + R[p] + 1);
}

void update(int l, int r, int k)
{
	int p = pos[l], q = pos[r];
	if (p == q)
	{
		for (int i = l; i <= r; ++i) a[i] += k;
		maintain(p);
		return;
	}
	for (int i = l; i <= R[p]; ++i) a[i] += k;
	maintain(p);
	for (int i = L[q]; i <= r; ++i) a[i] += k;
	maintain(q);
	for (int i = p + 1; i < q; ++i) add[i] += k;
}

inline int search(int l, int r, int pos, int k)
{
	int L = l - 1, R = r + 1;
	while (L + 1 != R)
	{
		int mid = L + R >> 1;
		if (T[mid] + add[pos] < k) L = mid;
		else R = mid;
	}
	return T[L] + add[pos];
}

int query(int l, int r, int k)
{
	int ans = -1, p = pos[l], q = pos[r];
	if (p == q)
	{
		for (int i = l; i <= r; ++i)
			if (a[i] + add[p] < k && a[i] + add[p] > ans) ans = a[i] + add[p];
		return ans;
	}
	for (int i = l; i <= R[p]; ++i)
		if (a[i] + add[p] < k && a[i] + add[p] > ans) ans = a[i] + add[p];
	for (int i = L[q]; i <= r; ++i)
		if (a[i] + add[q] < k && a[i] + add[q] > ans) ans = a[i] + add[q];
	for (int i = p + 1; i < q; ++i)
		if (T[L[i]] + add[i] < k) ans = max(ans, search(L[i], R[i], i, k));
	return ans;
}

int main(void)
{
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) scanf("%d", a + i);
	int t = sqrt(n);
	for (int i = 1; i <= t; ++i)
	{
		L[i] = (i - 1) * t + 1;
		R[i] = i * t;
	}
	if (R[t] < n) ++t, L[t] = R[t - 1] + 1, R[t] = n;
	for (int i = 1; i <= t; ++i)
	{
		for (int j = L[i]; j <= R[i]; ++j)
			pos[j] = i;
		maintain(i);
	}
	for (int i = 1; i <= n; ++i)
	{
		int opt, l, r, c;
		scanf("%d%d%d%d", &opt, &l, &r, &c);
		if (opt == 0) update(l, r, c);
		else printf("%d\n", query(l, r, c));
	}
	return 0;
}
```
{% endfolding %}

#### [Loj 6281] 数列分块入门 5

[Portal](https://loj.ac/p/6281).

区间开方这一操作非常经典，它进行不了多少次，所以我们只需要暴力维护就可以了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>

using namespace std;

int n, t;
int a[50005], pos[50005];
int L[305], R[305], maxx[305], sum[305];

void update(int l, int r)
{
	int p = pos[l], q = pos[r];
	for (int i = p; i <= q; ++i)
		if (maxx[i] > 1)
		{
			int s = max(l, L[i]), t = min(r, R[i]);
			for (int j = s; j <= t; ++j)
				if (a[j] > 1)
				{
					sum[i] -= a[j];
					a[j] = sqrt(a[j]);
					sum[i] += a[j]; maxx[i] = max(maxx[i], a[j]);
				}
		}
}

int query(int l, int r)
{
	int p = pos[l], q = pos[r], ans = 0;
	if (p == q)
	{
		for (int i = l; i <= r; ++i)
			ans += a[i];
		return ans;
	}
	for (int i = p + 1; i < q; ++i) ans += sum[i];
	for (int i = l; i <= R[p]; ++i) ans += a[i];
	for (int i = L[q]; i <= r; ++i) ans += a[i];
	return ans;
}

int main(void)
{
	memset(maxx, 0x3f, sizeof(maxx));
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) scanf("%d", a + i);
	t = sqrt(n);
	for (int i = 1; i <= t; ++i)
	{
		L[i] = (i - 1) * t + 1;
		R[i] = i * t;
	}
	if (R[t] < n) ++t, L[t] = R[t - 1] + 1, R[t] = n;
	for (int i = 1; i <= t; ++i)
		for (int j = L[i]; j <= R[i]; ++j)
		{
			pos[j] = i;
			sum[i] += a[j]; maxx[i] = max(maxx[i], a[j]);
		}
	for (int i = 1; i <= n; ++i)
	{
		int opt, l, r, c;
		scanf("%d%d%d%d", &opt, &l, &r, &c);
		if (opt == 0) update(l, r);
		else printf("%d\n", query(l, r));
	}
	return 0;
}
```
{% endfolding %}

#### [UVA12003] Array Transformer

[Portal](https://www.luogu.com.cn/problem/UVA12003).

还是使用二分的思路。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;
typedef long long i64;

int n, m, u;
int L[1005], R[1005], pos[300005];
int a[300005], T[300005];

void maintain(int p)
{ 
	for (int i = L[p]; i <= R[p]; ++i) T[i] = a[i];
	sort(T + L[p], T + R[p] + 1); 
}

int query(int l, int r, int k)
{
	int ans = 0, p = pos[l], q = pos[r];
	if (p == q)
	{
		for (int i = l; i <= r; ++i)
			if (a[i] < k) ++ans;
		return ans;
	}
	for (int i = p + 1; i < q; ++i) 
		ans += lower_bound(T + L[i], T + R[i] + 1, k) - (T + L[i]);
	for (int i = l; i <= R[p]; ++i)
		if (a[i] < k) ++ans;
	for (int i = L[q]; i <= r; ++i)
		if (a[i] < k) ++ans;
	return ans;
}

int main(void)
{
	scanf("%d%d%d", &n, &m, &u);
	for (int i = 1; i <= n; ++i) scanf("%d", a + i);
	int t = sqrt(n);
	for (int i = 1; i <= t; ++i)
		L[i] = R[i - 1] + 1, R[i] = i * t;
	if (R[t] < n) ++t, L[t] = R[t - 1] + 1, R[t] = n;
	for (int i = 1; i <= t; ++i)
	{
		for (int j = L[i]; j <= R[i]; ++j)
			pos[j] = i;
		maintain(i);
	}
	while (m--)
	{
		int l, r, v, p;
		scanf("%d%d%d%d", &l, &r, &v, &p);
		a[p] = 1ll * u * query(l, r, v) / (r - l + 1);
		maintain(pos[p]);
	}
	for (int i = 1; i <= n; ++i) printf("%d\n", a[i]);
	return 0;
}
```
{% endfolding %}

### 区间众数问题

这是分块解决的一个经典模型。

#### [Violet] 蒲公英

[Portal](https://www.luogu.com.cn/problem/P4168).

> 静态多次询问区间众数，强制在线。

区间众数不具有区间可加性，所以不可以使用线段树维护。

我们预处理出数组 $c[x][y][num]$，代表 $num$ 在块 $[x][y]$ 中的出现次数，同时预处理出答案。在查询的时候，整块的答案可以直接得到，零散块只需要单次扫描统计即可。

设块的大小为 $T$，那么时间复杂度为 $O(nT^2+mn/T)$，那么当 $T=\sqrt[3]{n}$ 时会比较快，时间复杂度为 $O(n^{\frac{3}{5}})$。这种做法比较慢，而且空间较大。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

int n, m, tot, ans[2];
int a[40005], b[40005];
int c[37][37][40005], f[37][37][2];
int L[205], R[205], pos[40005];

void work(int x, int y, int num) {
    ++c[x][y][num];
    if (c[x][y][num] > ans[0] || (c[x][y][num] == ans[0] && num < ans[1]))
        ans[1] = num, ans[0] = c[x][y][num];
}

int query(int l, int r) {
    int p = pos[l], q = pos[r];
    int x = 0, y = 0;
    if (p + 1 <= q - 1) x = p + 1, y = q - 1;
    memcpy(ans, f[x][y], sizeof(ans));
    if (p == q) {
        for (int i = l; i <= r; ++i) work(x, y, a[i]);
        for (int i = l; i <= r; ++i) --c[x][y][a[i]];
    } else {
        for (int i = l; i <= R[p]; ++i) work(x, y, a[i]);
        for (int i = L[q]; i <= r; ++i) work(x, y, a[i]);
        for (int i = l; i <= R[p]; ++i) --c[x][y][a[i]];
        for (int i = L[q]; i <= r; ++i) --c[x][y][a[i]];
    }
    return b[ans[1]];
}

int main(void) {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    memcpy(b, a, sizeof(b));
    sort(b + 1, b + n + 1);
    tot = unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; ++i)
        a[i] = lower_bound(b + 1, b + tot + 1, a[i]) - b;
    
    int t = pow(double(n), (double)1 / 3);
    int len = t ? n / t : n;

    for (int i = 1; i <= t; ++i) {
        L[i] = R[i - 1] + 1; R[i] = i * len;
    }
    if (R[t] < n) { 
        L[t + 1] = R[t] + 1;
        R[++t] = n; 
    }

    for (int i = 1; i <= t; ++i)
        for (int j = L[i]; j <= R[i]; ++j)
            pos[j] = i;

    memset(c, 0, sizeof(c));
    memset(f, 0, sizeof(f));    
    for (int i = 1; i <= t; ++i)
        for (int j = i; j <= t; ++j) {
            for (int k = L[i]; k <= R[j]; ++k)
                ++c[i][j][a[k]];
            for (int k = 1; k <= tot; ++k)
                if (c[i][j][k] > f[i][j][0]) {
                    f[i][j][0] = c[i][j][k];
                    f[i][j][1] = k;
                }
        }

    int last = 0;
    while (m--) {
        int l, r;
        scanf("%d%d", &l, &r);
        l = (l + last - 1) % n + 1; r = (r + last - 1) % n + 1;
        if (l > r) swap(l, r);
        printf("%d\n", last = query(l, r));
    }
    return 0;
}
```
{% endfolding %}

#### [Ynoi2019 模拟赛] Yuno loves sqrt technology III

[Portal](https://www.luogu.com.cn/problem/P5048).

这次询问的是区间众数的出现次数，但本质上是一样的。我们记录 $f(i,j)$ 代表第 $[i,j]$ 块的区间众数出现次数。我们对每一个数都开一个 STL vector，记录其出现的位置，并记 $ax[i]$ 为第 $i$ 个数出现在其 vector 中的位置。

整块的答案是可以直接统计的，而对于零散块，则获取当前的数在 vector 中的位置，考虑 $ans$ 是否可以变大，进行暴力更新即可。由于 $ans$ 最多变大 $2\sqrt{n}$，因此询问的时间复杂度为 $O(\sqrt{n})$。

总时间复杂度为 $O(n\sqrt{n})$。

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

int read(void) {
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}
void print(int x) {
    if (x > 9) print(x / 10);
    putchar_unlocked(x % 10 + '0');
}

int n, m, tot;
int a[500005], b[500005], pos[500005];
int L[720], R[720], f[720][720];
int cnt[500005], ax[500005];
std::vector <int> v[500005];

int query(int l, int r) {
    int p = pos[l], q = pos[r], ans = f[p + 1][q - 1];
    if (p == q) {
        for (int i = l; i <= r; ++i) cnt[a[i]] = 0;
        for (int i = l; i <= r; ++i) ans = std::max(ans, ++cnt[a[i]]);
        return ans;
    }
    for (int i = l; i <= R[p]; ++i) {
        int p = ax[i];
        while (p + ans < v[a[i]].size() && v[a[i]][p + ans] <= r) ++ans;
    }
    for (int i = L[q]; i <= r; ++i) {
        int p = ax[i];
        while (p - ans >= 0 && v[a[i]][p - ans] >= l) ++ans;
    }
    return ans;
}

int main(void) {
    n = read(), m = read();
    for (int i = 1; i <= n; ++i) b[i] = a[i] = read();
    std::sort(b + 1, b + n + 1);
    tot = std::unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; ++i) {
        a[i] = std::lower_bound(b + 1, b + tot + 1, a[i]) - b;
        v[a[i]].emplace_back(i); ax[i] = v[a[i]].size() - 1;
    }

    int t = sqrt(n);
    for (int i = 1; i <= t; ++i) L[i] = R[i - 1] + 1, R[i] = i * t;
    if (R[t] < n) { ++t; L[t] = R[t - 1] + 1; R[t] = n;}
    for (int i = 1; i <= t; ++i) for (int j = L[i]; j <= R[i]; ++j) pos[j] = i;
    for (int i = 1; i <= t; ++i) {
        memset(cnt, 0, sizeof(cnt));
        for (int j = i; j <= t; ++j) {
            f[i][j] = f[i][j - 1];
            for (int k = L[j]; k <= R[j]; ++k)
                f[i][j] = std::max(f[i][j], ++cnt[a[k]]);
        }
    }

    int last = 0;
    while (m--) {
        int l = (read() ^ last), r = (read() ^ last);
        print(last = query(l, r));
        putchar_unlocked('\n');
    }
    return 0;
}
```
{% endfolding %}

#### [Loj 6285] 数列分块入门 9

[Portal](https://loj.ac/p/6285).

套用上一题的做法，另外记录区间众数的值即可。

{% folding cyan::查看代码 %}
```cpp
#include <bits/stdc++.h>

int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar();
    return x;
}

int n, m, tot;
int a[500005], b[500005], pos[500005];
int L[720], R[720], f[720][720][2];
int cnt[500005], ax[500005];
std::vector <int> v[500005];

int query(int l, int r) {
    int p = pos[l], q = pos[r], ans[2];
    memcpy(ans, f[p + 1][q - 1], sizeof(ans));
    if (p == q) {
        for (int i = l; i <= r; ++i) cnt[a[i]] = 0;
        for (int i = l; i <= r; ++i) {
            ++cnt[a[i]];
            if (cnt[a[i]] > ans[0] || (cnt[a[i]] == ans[0] && a[i] < ans[1])) {
                ans[1] = a[i];
                ans[0] = cnt[a[i]];
            }
        }
        return b[ans[1]];
    }
    for (int i = l; i <= R[p]; ++i) {
        int p = ax[i];
        if (p + ans[0] - 1 < v[a[i]].size() && v[a[i]][p + ans[0] - 1] <= r) ans[1] = std::min(ans[1], a[i]);
        while (p + ans[0] < v[a[i]].size() && v[a[i]][p + ans[0]] <= r) ++ans[0], ans[1] = a[i];
    }
    for (int i = L[q]; i <= r; ++i) {
        int p = ax[i];
        if (p - ans[0] + 1 >= 0 && v[a[i]][p - ans[0] + 1] >= l) ans[1] = std::min(ans[1], a[i]);
        while (p - ans[0] >= 0 && v[a[i]][p - ans[0]] >= l) ++ans[0], ans[1] = a[i];
    }
    return b[ans[1]];
}

int main(void) {
    #ifdef LOCAL
        freopen("a2.in", "r", stdin);
    #endif
    n = read();
    for (int i = 1; i <= n; ++i) b[i] = a[i] = read();
    #ifdef LOCAL
        int cnt1 = 0, cnt2 = 0;
        for (int i = 1067; i <= 1132; ++i)
            if (a[i] == 1622647857) ++cnt1;
            else if (a[i] == 1622647862) ++cnt2;        
    printf("%d %d\n", cnt1, cnt2);
    #endif
    std::sort(b + 1, b + n + 1);
    tot = std::unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; ++i) {
        a[i] = std::lower_bound(b + 1, b + tot + 1, a[i]) - b;
        v[a[i]].emplace_back(i); ax[i] = v[a[i]].size() - 1;
    }

    int t = sqrt(n);
    for (int i = 1; i <= t; ++i) L[i] = R[i - 1] + 1, R[i] = i * t;
    if (R[t] < n) { ++t; L[t] = R[t - 1] + 1; R[t] = n;}
    for (int i = 1; i <= t; ++i) for (int j = L[i]; j <= R[i]; ++j) pos[j] = i;
    for (int i = 1; i <= t; ++i) {
        memset(cnt, 0, sizeof(cnt));
        for (int j = i; j <= t; ++j) {
            f[i][j][0] = f[i][j - 1][0], f[i][j][1] = f[i][j - 1][1];
            for (int k = L[j]; k <= R[j]; ++k) {
                ++cnt[a[k]];
                if (cnt[a[k]] > f[i][j][0] || (cnt[a[k]] == f[i][j][0] && a[k] < f[i][j][1])) 
                    f[i][j][0] = cnt[a[k]], f[i][j][1] = a[k];
            }
        }
    }
    #ifdef LOCAL
        printf("%d %d\n", pos[1067], pos[1132]);
        return 0;
    #endif
    int m = n;
    while (m--) {
        int l = read(), r = read();
        printf("%d\n", query(l, r));
    }
    return 0;
}
```
{% endfolding %}

### [Ynoi2017] 由乃打扑克

[Portal](https://www.luogu.com.cn/problem/P5356).

如果直接套用上一题的做法，那么复杂度是 $O(m\sqrt{n\log n}\log n)$ 的（因为块内也需要二分了），这样会被卡掉。

将块的大小设置为 $\sqrt{n}\log n$，每次修改的复杂度就为 $O(\sqrt{n}\log n)$。查询的时候有 $\cfrac{\sqrt{n}}{\log{n}}$ 个整块，$\sqrt{n}\log n$ 个零散的数，还是太慢了，把两个零散的块合并为一个假的块再二分即可。时间复杂度为 $O(m\sqrt{n}\log n)$。

## 块状链表



## 莫队

有时题目的查询非常诡异，常规的在线做法（读入一个询问回答一个）不起作用，需要离线做法（读入所有询问后统一处理）。如果还要对询问进行分块，那么莫队就出现了。

一般来讲，分块分为静态分块和动态分块，

## 树分块

## Problemset
