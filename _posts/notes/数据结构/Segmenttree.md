---
title: 线段树基础
headimg: https://s1.ax1x.com/2022/08/08/vMLOds.png
cover: false
categories:
    - 算法竞赛
    - 学习笔记
    - 数据结构
plugins:
    - katex
tag:
    - 数据结构
    - 线段树
abbrlink: 89bb6fd1
date: 2022-08-06 20:08:43
group: notes
hide: true
status: done
---

>线段树（Segment Tree）是一种二叉搜索树，1977 年由 Jon Louis Bentley 发明，可以较为灵活且效率较高地解决各类序列维护问题。本文将引导你学习简单的线段树。

<!--more-->

{% folding blue::更新日志 %}

{% timeline %}

{% timenode 2022/09/17 %}

以新的标准改写了 Problemset 的结构，并增加了几道题目。

{% endtimenode %}

{% timenode 2022/09/06 %}

完成了预定的所有内容。

{% endtimenode %}

{% endtimeline %}

{% endfolding %}

## 简介线段树

“线段树”只是 Segment Tree 的一种称法，因为线段树可以理解为是由很多线段组成的，其它叫法包括“区间树（interval tree）”、范围树等等。但这些称法一般用于特殊领域（如计算几何），本文均用线段树来代表 Segment Tree。

线段树是一种基于分治思想的二叉树结构，有如下特征：   

- 线段树的每一个节点都代表一个区间。   
- 线段树具有唯一的根节点，代表统计范围，一般为 $[1,n]$。   
- 线段树的每个叶子节点长度都为 $1$，形如 $[x,x]$。   
- 一般我们定义，若 $mid=\lfloor(l+r)\div2\rfloor$，那么节点 $[l,r]$ 的左子节点是 $[l,mid]$，右子节点是 $[mid+1,r]$。

{% image https://z3.ax1x.com/2021/04/03/cnxtl4.png, alt=一棵线段树 %}

对于上图这棵维护区间 $[1,4]$ 的线段树而言，可以发现，一个节点的左子节点是它的编号乘 $2$，右子节点是乘 $2$ 加 $1$。我们可以利用这点方便地来存储线段树。

但凑巧的是，这颗线段树是满二叉树，其它情况类似于这种：

{% image https://z3.ax1x.com/2021/04/03/cnz4KJ.md.jpg, alt=另一棵线段树 %}

其实去掉最后一层这树仍是满二叉树，这种情况依然可以使用上述方法存树。

## 线段树的存储

线段树的正常写法是堆式线段树。

其实就是只用一个数组 $T$ 存储线段树，用堆的编号来表示线段树的左儿子和右儿子（`lc = o << 1, rc = o << 1 | 1`），不过进行操作的时候要多传两个数据 $l$ 和 $r$。

> 注意，线段树的节点必须开四倍空间！否则如果遇到非满二叉树的线段树，二倍空间就会爆炸！

在网上你能看到这样一种堆式线段树：

```cpp
struct Node
{
    int l, r;
    int val;
}T[4*MAXN];
```

注意，它是记录了当前节点 $o$ 的**区间** $[l,r]$，在传参时可以省掉两个参数（听不懂？那就不管，往下看就行）。   
有时候要维护的信息特别复杂，我们会将数组 $T$ 的类型改为结构体，但还是不会使用记录区间的方式。

一般我们使用堆式线段树中的数组方式，而不记录左右儿子（不记慢不了多少）。接下来若不是特殊情况，我们均使用这种方式。

## 线段树的建树

我们再来看这棵线段树：

{% image https://z3.ax1x.com/2021/04/03/cnxtl4.png, alt=嘻嘻，还是我 %}

最后一层若当作原序列的值，即 $[i,i]$ 保存 $A_i$ 的值。由于线段树是二叉树结构，可以很方便地从下往上传递信息。以区间和为例，令节点 $[l,r]$ 表示 $\sum\limits_{i=l}^{r}A_i$，显然 $[l,r]=[l,mid]+[mid+1,r]$（这里的区间代表区间所对应的值）。

比如原序列是 `1 2 3 4`，那么对于节点 $1$~$7$，可推算出它们的值分别为 `10 3 7 1 2 3 4`。

那么建树的代码大概就像这样：

```cpp
#define L(x) ((x)<<1)
#define R(x) ((x)<<1|1)

// 当然，你也可以不用宏定义。

int T[4*N];

inline void maintain(int o)
{
    T[o] = T[L(o)] + T[R(o)];
    // 从下往上传递信息。事实上你也可以写在需要调用 maintain 函数的函数里，不过有时传递的信息较为复杂，还是建议写一个 maintain 函数。网上有的教程把它写作 pushup，至于为什么，接下来你了解到 pushdown 就知道了。
}

void build(int o, int l, int r) //o 代表当前维护结点的标号，l 和 r 代表所对应的区间
{
    if (l == r) //如果这是叶子节点
    {
        T[o] = a[l]; //赋值
        return;
    }

    int mid = l + r >> 1; // 计算中值
    build(L(o), l, mid); // 为左半段建树
    build(R(o), mid+1, r); // 为右半段建树

    maintain(o); // 计算当前结点的值
}
```

在 `main` 用 `build(1, 1, n) ` 来调用 `build`。
由于每个节点只访问了一次，所以建树的时间复杂度为 $\mathcal{O}(n)$。

## 点修改与区间查询

[模板](https://www.luogu.com.cn/problem/P3374)。

### 点修改

还是以这棵线段树为例：   

{% image https://z3.ax1x.com/2021/04/03/cnxtl4.png, alt=我又来啦 %}

根据刚才的数据，初始化后它应该长这样：   

{% image https://z3.ax1x.com/2021/04/04/cuIpnO.png, alt=初始化后的线段树 %}

我们先来进行点修改，比如要给原序列的第 $2$ 个元素加上 $1$，那么这棵线段树会怎么变化呢？   
可以发现，节点 $4$、$2$、$1$ 都会加上 $1$。线段树就变成了这个样子：  

{% image https://z3.ax1x.com/2021/04/04/cuIzPs.png, alt=点修改后的线段树 %}

那代码怎么实现呢？一般来讲，根节点 $1$ 总是线段树执行的入口，从根节点出发，递归找到需要修改的叶子节点，这里代码如下：

```cpp
void update(int o, int l, int r, int x, int k) //给原序列第 x 个元素加上 k。
{
    if (l == r) //这是叶子节点
    {
        T[o] += k; //直接加
        return;
    }

    int mid = l + r >> 1;
    if (x <= mid) update(L(o), l, mid, x, k); // 叶子节点在 [l,mid] 处。
    else update(R(o), mid+1, r, x, k); // 叶子节点在 [mid+1,r] 处。

    maintain(o); //重新计算这个节点的值。
}
```

由于线段树的层数在 $\log$ 级别，所以点修改的时间复杂度为 $\mathcal{O}(\log n)$。

### 区间查询

查询区间 $[l,r]$ 的和，从根节点开始，递归执行下列过程：

1. 若当前区间 $[l,r]$ 完全覆盖了需要求解的范围，那么直接返回答案。
2. 若当前区间与左子节点有重叠，访问左子节点 $[l,mid]$。
3. 若当前区间与右子节点有重叠，访问右子节点 $[mid+1,r]$（注意不是访问左子节点后就不用访问右子节点了）。

那怎么解释这个东西呢？还是看那棵线段树 ~~（它的出镜率为什么这么高）~~：   

{% image https://z3.ax1x.com/2021/04/04/cuIzPs.png, alt=嗯，又是我 %}

比如现在我们要查 $[1,3]$。   
$[1,3]$ 与 $[1,2]$ 和 $[3,4]$ 都有重叠，所以我们要分别访问。   
$[1,2]$ 完全覆盖，直接返回。   
$[3,4]$ 左子节点有覆盖，右子节点没有，访问左子节点。   
$[3,3]$ 直接返回。   
所以答案是 $4+3=7$。

那么代码就长这样：

```cpp
int query(int o, int l, int r, int ql, int qr) //[ql,qr] 是要查的区间
{
    if (ql <= l && r <= qr) return T[o]; //完全包含
    int mid = l + r >> 1, res = 0;
    // 接下来，只要你在（哪怕只有一个元素），我就查
    if (ql <= mid) res += query(L(o), l, mid, ql, qr); //左子节点
    if (mid + 1 <= qr) res += query(R(o), mid+1, r, ql, qr); //右子节点
    return res;
}
```
同 `update`，`query` 的时间复杂度也是 $\mathcal{O}(\log n)$。

以下代码就可以通过刚才的模板了。 

{% folding cyan, 查看代码 %}

```cpp
#include <iostream>
#include <cstdio>

#define L(x) (x<<1)
#define R(x) (L(x)|1)

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x * f;
}

int n, m;
int T[2000005];
int a[500005];

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o] = a[l];
        return;
    }
    int mid = l + r >> 1;
    build(L(o), l, mid);
    build(R(o), mid+1, r);
    T[o] = T[L(o)] + T[R(o)];
}

void update(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o] += k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(L(o), l, mid, x, k);
    else update(R(o), mid+1, r, x, k);
    T[o] = T[L(o)] + T[R(o)];
}

int query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res += query(L(o), l, mid, ql, qr);
    if (mid + 1 <= qr) res += query(R(o), mid+1, r, ql, qr);
    return res;
}

int main(void)
{
    n = read(), m = read();
    for (int i = 1; i <= n; ++i)
        a[i] = read();
    build(1, 1, n);

    while (m--)
    {
        int op = read(), x = read(), y = read();
        if (op == 1) update(1, 1, n, x, y);
        else printf("%d\n", query(1, 1, n, x, y));
    }
    return 0;
}
```

{% endfolding %}

### Problemset

在讨论区间修改之前，我们先看几道线段树的题目。

#### [Luogu P4513] 小白逛公园

[Portal](https://www.luogu.com.cn/problem/P4513).

最大子段和可以使用 $\mathcal{O}(n \log n)$ 的分治法进行求解，因为这个子段要么在序列的左半段，要么在右半段，要么跨越中点。加上多组询问，这就是线段树嘛！

最大和的子段在中点两端好说，现在就来看一下跨越中点的情况。  
线段树的每个节点维护三个值：最大子段和、最大前缀和、最大后缀和所对应的区间（此区间是线段树的节点所对应的区间）。那么最大子段和跨越中点时，就是前半区间的最大后缀和，加上后半区间的最大前缀和。

维护四个信息：区间和 $sum$，仅靠左端的最大连续和 $lmax$，靠右段的 $rmax$，以及区间最大子段和 $dat$。

在 `query` 的时候，我们需要看它是否完全在左区间还是完全在右区间，都不是就是跨区间，需要根据左右节点的查询结果计算当前答案。

{% folding cyan open::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, m;
int a[500005];

struct Node
{
    int sum, lmax, rmax, dat;
    Node (int sum = 0, int lmax = 0, int rmax = 0, int dat = 0) :
        sum(sum), lmax(lmax), rmax(rmax), dat(dat) {}
}T[2000005];

inline void maintain(int o)
{
    int ls = o << 1, rs = o << 1 | 1;
    T[o].sum = T[ls].sum + T[rs].sum;
    T[o].lmax = max(T[ls].lmax, T[ls].sum + T[rs].lmax);
    T[o].rmax = max(T[rs].rmax, T[rs].sum + T[ls].rmax);
    T[o].dat = max({T[ls].dat, T[rs].dat, T[ls].rmax + T[rs].lmax});
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o].sum = T[o].lmax = T[o].rmax = T[o].dat = a[l];
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    maintain(o);
}

void update(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o].sum = T[o].lmax = T[o].rmax = T[o].dat = k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, k);
    else update(o << 1 | 1, mid + 1, r, x, k);
    maintain(o);
}

Node query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    if (qr <= mid) return query(ls, l, mid, ql, qr);
    if (ql > mid) return query(rs, mid + 1, r, ql, qr);
    Node x = query(ls, l, mid, ql, qr), y = query(rs, mid + 1, r, ql, qr), res;
    res.sum = x.sum + y.sum;
    res.lmax = max(x.lmax, x.sum + y.lmax);
    res.rmax = max(y.rmax, y.sum + x.rmax);
    res.dat = max({x.dat, y.dat, x.rmax + y.lmax});
    return res;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    build(1, 1, n);
    while (m--)
    {
        int k, x, y;
        scanf("%d%d%d", &k, &x, &y);
        if (k == 1) 
        {
            if (x > y) swap(x, y);
            printf("%d\n", query(1, 1, n, x, y).dat);
        }
        else update(1, 1, n, x, y);
    }
    return 0;
}
```
{% endfolding %}

#### [UVa 1400] "Ray, Pass me the dishes!"

[Portal](https://www.luogu.com.cn/problem/UVA1400).

> 给定一个序列和多组询问 $(l,r)$，查询区间 $[l,r]$ 的最大子段和，**并给出答案对应的字典序最小的子区间**。

这回要求输出答案的区间了（），但是也没有什么好怕的。我们只需要记录一个 `max_sub` 来记录区间。

首先是建树，像这样：

```cpp
void build(int o, int l, int r)
{
    if (l == r)
    {
        maxsub[o] = make_pair(l, r);
        maxpre[o] = l;
        maxsuf[o] = r;
        return;
    }
    // 以上显然

    int mid = l + r >> 1, lc = o << 1, rc = o << 1 | 1;
    build(lc, l, mid);
    build(rc, mid+1, r);

    // maintain
}
```

如何维护这一节点呢？根据刚才所说，对应三种情况：

```cpp
maxsub[o] = better(maxsub[lc], maxsub[rc]); // 左右区间
maxsub[o] = better(maxsub[o], make_pair(maxsuf[lc], maxpre[rc])); // 跨越中点
```

其中 `better` 函数用于比较哪个子区间更好。

`maxsuf` 和 `maxpre` 怎么维护呢？以 `maxpre` 为例子，像这样：

```cpp
LL v1 = sum(l, maxpre[lc]);
LL v2 = sum(l, maxpre[rc]);
if (v1 == v2) maxpre[o] = min(maxpre[lc], maxpre[rc]); // 右端点肯定是越小越好的
else maxpre[o] = v1 > v2 ? maxpre[lc] : maxpre[rc];
```

其中 `sum` 指原序列的区间和，容易用前缀和求解。

接下来是查询，大概像这样：

```cpp
Interval query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return maxsub[o]; // 在区间范围内
    int mid = l + r >> 1, lc = o << 1, rc = o << 1 | 1;
    if (qr <= mid) return query(lc, l, mid, ql, qr); // 完全在左半端
    if (ql > mid) return query(rc, mid+1, r, ql, qr); // 完全在右半段
    Interval ans = better(query(lc, l, mid, ql, qr), query(rc, mid+1, r, ql, qr)); // 不跨越中点
    return better(ans, make_pair(calc_suf(lc, l, mid, ql).L, calc_pre(rc, mid+1, r, qr).R)); // 跨越中点
}
```

注意求解前缀和后缀的函数，这里的写法完全符合刚才的定义，这里给出 `calc_pre` 的实现，`calc_suf` 的实现大致相同。

```cpp
Interval calc_pre(int o, int l, int r, int qr)
{
    if (maxpre[o] <= qr) return make_pair(l, maxpre[o]);  // 完全在查询范围内
    int mid = l + r >> 1, lc = o << 1, rc = o << 1 | 1;
    if (qr <= mid) return calc_pre(lc, l, mid, qr); // 在左半端
    // 注意它要么完全在左半段要么跨越中点，不可能全在右半段
    Interval ans = make_pair(l, calc_pre(rc, mid+1, r, qr).R); // 跨越中点
    return better(ans, make_pair(l, maxpre[lc])); // 与完全在左半段比较
}
```

下面是完整代码：

{% folding cyan, 查看代码 %}

```cpp
#include <iostream>
#include <cstdio>

#define Interval pair<int, int>
#define L first
#define R second
#define LL long long

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x * f;
}

int n, m;
int a[500005];
Interval maxsub[2000005];
LL prefix_sum[500005];
int maxpre[2000005], maxsuf[2000005];

inline LL sum(int l, int r) {return prefix_sum[r] - prefix_sum[l-1];}
inline LL sum(Interval x) {return sum(x.L, x.R);}
inline Interval better(Interval a, Interval b)
{
    if (sum(a) != sum(b)) return sum(a) > sum(b) ? a : b;
    return a < b ? a : b;
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        maxsub[o] = make_pair(l, r);
        maxpre[o] = l;
        maxsuf[o] = r;
        return;
    }

    int mid = l + r >> 1, lc = o << 1, rc = o << 1 | 1;
    build(lc, l, mid);
    build(rc, mid+1, r);

    LL v1 = sum(l, maxpre[lc]);
    LL v2 = sum(l, maxpre[rc]);
    if (v1 == v2) maxpre[o] = min(maxpre[lc], maxpre[rc]);
    else maxpre[o] = v1 > v2 ? maxpre[lc] : maxpre[rc];

    v1 = sum(maxsuf[lc], r);
    v2 = sum(maxsuf[rc], r);
    if (v1 == v2) maxsuf[o] = min(maxsuf[lc], maxsuf[rc]);
    else maxsuf[o] = v1 > v2 ? maxsuf[lc] : maxsuf[rc];

    maxsub[o] = better(maxsub[lc], maxsub[rc]);
    maxsub[o] = better(maxsub[o], make_pair(maxsuf[lc], maxpre[rc]));
}

Interval calc_pre(int o, int l, int r, int qr)
{
    if (maxpre[o] <= qr) return make_pair(l, maxpre[o]);
    int mid = l + r >> 1, lc = o << 1, rc = o << 1 | 1;
    if (qr <= mid) return calc_pre(lc, l, mid, qr);
    Interval ans = make_pair(l, calc_pre(rc, mid+1, r, qr).R);
    return better(ans, make_pair(l, maxpre[lc]));
}

Interval calc_suf(int o, int l, int r, int ql)
{
    if (maxsuf[o] >= ql) return make_pair(maxsuf[o], r);
    int mid = l + r >> 1, lc = o << 1, rc = o << 1 | 1;
    if (ql > mid) return calc_suf(rc, mid+1, r, ql);
    Interval ans = make_pair(calc_suf(lc, l, mid, ql).L, r);
    return better(ans, make_pair(maxsuf[rc], r));
}

Interval query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return maxsub[o];
    int mid = l + r >> 1, lc = o << 1, rc = o << 1 | 1;
    if (qr <= mid) return query(lc, l, mid, ql, qr);
    if (ql > mid) return query(rc, mid+1, r, ql, qr);
    Interval ans = better(query(lc, l, mid, ql, qr), query(rc, mid+1, r, ql, qr));
    return better(ans, make_pair(calc_suf(lc, l, mid, ql).L, calc_pre(rc, mid+1, r, qr).R));
}

int main(void)
{
    int kase = 0;
    while (scanf("%d%d", &n, &m) == 2)
    {
        printf("Case %d:\n", ++kase);
        for (int i = 1; i <= n; ++i) 
        {
            a[i] = read();
            prefix_sum[i] = prefix_sum[i-1] + a[i];
        }
        build(1, 1, n);
        while (m--)
        {
            int l = read(), r = read();
            Interval ans = query(1, 1, n, l, r);
            printf("%d %d\n", ans.L, ans.R);
        }
    }
    return 0;
}
```

{% endfolding %}

根据以上可以发现，线段树可以维护的是容易按照区间进行划分和合并，这一点又称满足**区间可加性**。关于这一点，接下来还会详细叙述。

#### 区间 GCD

[Portal](http://noi-test.zzstep.com/contest/0x40%E3%80%8C%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E8%BF%9B%E9%98%B6%E3%80%8D%E4%BE%8B%E9%A2%98/4302%20Interval%20GCD)。

{% noteblock link green %}

给定长度为 $n(n\leqslant 5\times 10^5)$ 的序列 $A$，$m(m\leqslant 10^5)$ 次操作：

1. `C l r d`，表示区间加 $d$。   
2. `Q l r`，表示求 $\gcd(A_l,A_{l+1},\cdots,A_{r-1},A_r)$。

{% endnoteblock %}

这个问题看上去很棘手，怎么办呢？强烈建议读者停下来自行思考——想一想 $\gcd$ 的性质，利用在树状数组学过的内容将原问题转换为可以用点修改实现的。还有一点可以发现：$\gcd$ 满足区间可加性，可以通过小区间的 $\gcd$ 求出大区间的 $\gcd$。

请读者先自行撕烤，然后再看解答。

{% folding purple, 查看解答 %}

根据 $\gcd(x,y)=\gcd(x,y-x)$，而且还有 $\gcd(x,y,z)=\gcd(x,y-x,z-y)$，这是什么？差分序列！那么我们可以用支持单点修改的线段树来解决这个问题，这样的话，`Q l r` 相当于求 `gcd(a[l], query(1, 1, n, l + 1, r)`。$A$ 数组的值可以用一个支持“区间修改，单点查询”的树状数组实现。线段树修改时，需要进行两次单点修改。

你可能会问一个问题，负数怎么办？实际上 $\gcd$ 的性质对负数同样成立，但是你的输出总不能是负的吧{% emoji 滑稽 %}，所以我们在输出时 `abs` 一下就好。注意由于有负数，所以 `gcd` 的代码实现要更改（因为取模运算有坑）。

{% endfolding %}

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

inline i64 read(void)
{
    i64 x = 0;
    int c = getchar(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x * f;
}

i64 gcd(i64 a, i64 b)
{
    if (b == 0) return a;
    return gcd(b, (a % b + b) % b);
}

int n, m;
i64 a[500010], b[500010];

class FenwickTree
{
    private:
        i64 C[500010];
        #define lowbit(x) (x & -x)
    public:
        inline void add(int x, i64 k)
        {
            while (x <= n)
            {
                C[x] += k;
                x += lowbit(x);
            }
        }
        inline i64 sum(int x)
        {
            i64 res = 0;
            while (x)
            {
                res += C[x];
                x -= lowbit(x);
            }
            return res;
        }
}F;

class SegmentTree
{
    private:
        i64 T[2000050];
        inline void maintain(int o)
        {
            T[o] = gcd(T[o << 1], T[o << 1 | 1]);
        }
    public:
        void build(int o, int l, int r)
        {
            if (l == r)
            {
                T[o] = b[l];
                return;
            }
            int mid = l + r >> 1;
            build(o << 1, l, mid);
            build(o << 1 | 1, mid + 1, r);
            maintain(o);
        }
        void update(int o, int l, int r, int x, i64 k)
        {
            if (l == r)
            {
                T[o] += k;
                return;
            }
            int mid = l + r >> 1;
            if (x <= mid) update(o << 1, l, mid, x, k);
            else update(o << 1 | 1, mid + 1, r, x, k);
            maintain(o);
        }
        i64 query(int o, int l, int r, int ql, int qr)
        {
            if (ql <= l && r <= qr) return T[o];
            int mid = l + r >> 1;
            i64 res = 0;
            if (ql <= mid) res = gcd(res, query(o << 1, l, mid, ql, qr)); 
            if (mid < qr) res = gcd(res, query(o << 1 | 1, mid + 1, r, ql, qr));
            return res;
        }
}S;

int main(void)
{
    n = read(), m = read();
    for (int i = 1; i <= n; ++i)
    {
        a[i] = read();
        b[i] = a[i] - a[i-1];
    }
    S.build(1, 1, n);
    // 树状数组不建树，到时候直接加上 a[l] 即可
    while (m--)
    {
        char c;
        cin >> c;
        int l = read(), r = read();
        if (c == 'C')
        {
            i64 k = read();
            F.add(l, k);
            S.update(1, 1, n, l, k);
            if (r < n)
            {
                F.add(r + 1, -k);
                S.update(1, 1, n, r + 1, -k);
            }
        }
        else printf("%lld\n", abs(gcd(a[l] + F.sum(l), l < r ? S.query(1, 1, n, l + 1, r) : 0)));
    }
    return 0;
}
```
{% endfolding %}

## 区间修改与延迟标记

>根据刚才的学习，可以发现线段树是个很厉害的数据结构，但它的威力可不止如此，来看，还有更厉害的：

### 延迟标记的介绍
[【Luogu 3372】【模板】线段树 1](https://www.luogu.com.cn/problem/P3372)

{% noteblock link green %}
进行下面两种操作： 

- 将区间 $[l,r]$ 的每一个数加上 $x$；  
- 求区间 $[l,r]$ 的和。   
{% endnoteblock %}

这回可不一样了，点修改只会影响树中的 $\log n$ 个节点，而区间修改最坏情况下会影响区间中的所有节点，这可怎么办？我们这里要引入一个叫做“延迟标记”的东西（或者叫它懒标记，即 lazy tag）。

试想，如果我们在一次修改操作中发现节点 $o$ 代表的区间 $[o_l,o_r]$ 中要修改的区间 $[l,r]$ 被完全覆盖，那么更新点 $o$ 的子树就是徒劳的。可以给节点 $o$ 做一个标记，省掉接下来的操作。就是打完标记后我们可以立即返回，此标记代表“该节点曾经被修改过，但其子节点尚未更新”。   

如果在后续的指令中，需要从节点 $o$ 向下递归，那么我们就下传 $o$ 的标记，并清空 $o$ 的标记。

接下来我们看一下这道题该怎么写。首先建树和维护当前节点的过程没有变化，而对于修改操作需要这样写：

```cpp
void update(int o, int l, int r, int x, int y, int k) //区间 [x,y] 加上 k
{
    if (x <= l && r <= y) //在区间范围内
    {
        T[o] += (LL)k * (r-l+1);
        tag[o] += k;
        return;
    }
    pushdown(o, l, r);

    int mid = l + r >> 1;
    if (x <= mid) update(L(o), l, mid, x, y, k);
    if (mid+1 <= y) update(R(o), mid+1, r, x, y, k);
    maintain(o);
}
```

这里有几点需要注意，首先是 $T$ 数组的修改，别忘了这是区间修改，要加上的数需要乘上区间的长度。      

然后是 `pushdown` 函数（有的版本写成 `spread`），需要这样：

```cpp
inline void pushdown(int o, int l, int r)
{
    if (tag[o]) //标记不是 0 才有必要下传，但非要传也非不可，慢不了多少
    {
        tag[L(o)] += tag[o];
        tag[R(o)] += tag[o];
        // 下传标记

        int mid = l + r >> 1;

        // 注意区间的长度
        T[L(o)] += (LL)tag[o] * (mid-l+1);
        T[R(o)] += (LL)tag[o] * (r-mid);

        tag[o] = 0; // 清除父亲节点的标记（因为下传了）
    }
}
```

需要分别修改左右儿子标记的值和数值。

最后是递归的过程，由于是区间修改，所以左右都需要判断（`mid + 1 <= y` 有的版本会写成 `mid < y` ）。

注意查询的时候也需要下传标记（否则你怎么查，子节点没法计算了）。

注意到了吧？`pushdown` 的反义词是 `pushup`，所以有人把 `maintain` 写成 `pushup`。

想一想，为什么以上操作都可以保证最后的结果时间复杂度是正确的呢（建议手玩）？

查询操作道理基本相同，相信大家可以自己写出来。

那么对于这道题而言：

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define LL long long
#define L(x) (x<<1)
#define R(x) (L(x)|1)

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar(), f = 1;
    while (!isdigit(c)){if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x * f;
}

int n, m;
LL a[100005];
LL T[400005], tag[400005];

inline void maintain(int o)
{
    T[o] = T[L(o)] + T[R(o)];
}

inline void pushdown(int o, int l, int r)
{
    if (tag[o])
    {
        tag[L(o)] += tag[o];
        tag[R(o)] += tag[o];

        int mid = l + r >> 1;
        T[L(o)] += (LL)tag[o] * (mid-l+1);
        T[R(o)] += (LL)tag[o] * (r-mid);

        tag[o] = 0;
    }
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o] = a[l];
        return;
    }

    int mid = l + r >> 1;
    build(L(o), l, mid);
    build(R(o), mid+1, r);

    maintain(o);
}

void update(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        T[o] += (LL)k * (r-l+1);
        tag[o] += k;
        return;
    }
    pushdown(o, l, r);

    int mid = l + r >> 1;
    if (x <= mid) update(L(o), l, mid, x, y, k);
    if (mid+1 <= y) update(R(o), mid+1, r, x, y, k);
    maintain(o);
}

LL query(int o, int l, int r, int ql, int qr)
{
    LL res = 0;
    if (ql <= l && r <= qr) return T[o];
    
    int mid = l + r >> 1;
    pushdown(o, l, r);
    
    if (ql <= mid) res += query(L(o), l, mid, ql, qr);
    if (mid+1 <= qr) res += query(R(o), mid+1, r, ql, qr);
    
    return res;
}

int main(void)
{
    n = read(), m = read();
    for (int i = 1; i <= n; ++i)
        a[i] = read();
    build(1, 1, n);

    while (m--)
    {
        int op = read();
        if (op == 1)
        {
            int x = read(), y = read(), k = read();
            update(1, 1, n, x, y, k);
        }
        else
        {
            int x = read(), y = read();
            printf("%lld\n", query(1, 1, n, x, y));
        }
    }
    return 0;
}
```
{% endfolding %}

是不是有点意思了？还有更复杂的。

### 多组延迟标记

你以为延迟标记只能由有一组？只要你愿意，都可以整出一百组（~~不过好像也没有一百组~~）！

#### [UVa 11992] Fast Matrix Operations

[Portal](https://www.luogu.com.cn/problem/UVA11992)。

> 有一个 $r$ 行 $c$ 列的全零矩阵，矩阵不超过 $20$ 行，支持子矩阵加，子矩阵赋值和查询子矩阵和、最小值和最大值。

由于矩阵最多有 $20$ 行，所以可以每行造一棵线段树，那么本体转化为一维问题。

现在由于有两种操作，那么就有两个标记，但两个标记总得有个顺序吧！否则不乱套了！   
由于先加后赋值是没有任何意义的，所以我们规定先赋值后加。

值得一提的是，对于这种要维护信息较多的线段树，建议使用结构体，否则代码会显得很乱。

不过这里笔者有点懒，未把自己的代码改成全用结构体，仅在查询时使用了结构体，请大家谅解。

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

#define L(o) ((o) << 1)
#define R(o) (L(o) | 1)

using namespace std;

struct Answer
{
    int sum, min, max;
    Answer(int s = 0, int i = 1000000002, int a = -1)
    {
        sum = s;
        min = i;
        max = a;
    }
};

inline Answer up(Answer a, Answer b)
{
    return Answer(a.sum + b.sum, min(a.min, b.min), max(a.max, b.max));
}

struct SegmentTree
{
    int sumv[1000005], minv[1000005], maxv[1000005];
    int addv[1000005], setv[1000005];

    inline void init(void)
    {
        // 没有初值，可以这样建树。
        memset(sumv, 0, sizeof(sumv));
        memset(minv, 0, sizeof(minv));
        memset(maxv, 0, sizeof(maxv));
        memset(setv, -1, sizeof(setv));
        memset(addv, 0, sizeof(addv));
    }

    inline void maintain(int o)
    {
        sumv[o] = sumv[L(o)] + sumv[R(o)];
        minv[o] = min(minv[L(o)], minv[R(o)]);
        maxv[o] = max(maxv[L(o)], maxv[R(o)]);
    }

    inline void pushdown(int o, int l, int r)
    {
        int mid = l + r >> 1;
        // 先搞 set，再搞 add
        if (setv[o] >= 0)
        {
            setv[L(o)] = setv[R(o)] = setv[o];
            addv[L(o)] = addv[R(o)] = 0; // 有 set 标记需清空 add 标记。

            sumv[L(o)] = (mid - l + 1) * setv[o];
            sumv[R(o)] = (r - mid) * setv[o];
            minv[L(o)] = minv[R(o)] = maxv[L(o)] = maxv[R(o)] = setv[o];

            setv[o] = -1;
        }
        if (addv[o] > 0)
        {
            addv[L(o)] += addv[o];
            addv[R(o)] += addv[o];
            
            sumv[L(o)] += (mid - l + 1) * addv[o];
            sumv[R(o)] += (r - mid) * addv[o];
            minv[L(o)] += addv[o];
            minv[R(o)] += addv[o];
            maxv[L(o)] += addv[o];
            maxv[R(o)] += addv[o];

            addv[o] = 0;
        }
    }

    inline void update_add(int o, int l, int r, int x, int y, int k)
    {
        if (x <= l && r <= y)
        {
            addv[o] += k;
            sumv[o] += (r - l + 1) * k;
            minv[o] += k;
            maxv[o] += k;
            return;
        }
        pushdown(o, l, r);
        int mid = l + r >> 1;
        if (x <= mid) update_add(L(o), l, mid, x, y, k);
        if (mid + 1 <= y) update_add(R(o), mid+1, r, x, y, k);
        maintain(o);
    }

    inline void update_set(int o, int l, int r, int x, int y, int k)
    {
        if (x <= l && r <= y)
        {
            addv[o] = 0;
            setv[o] = k;
            sumv[o] = (r - l + 1) * k;
            minv[o] = maxv[o] = k;
            return;
        }
        pushdown(o, l, r);
        int mid = l + r >> 1;
        if (x <= mid) update_set(L(o), l, mid, x, y, k);
        if (mid + 1 <= y) update_set(R(o), mid+1, r, x, y, k);
        maintain(o);
    }

    // 强烈不建议在这里使用全局变量计算答案，这是禁忌，会让代码很乱。
    inline Answer query(int o, int l, int r, int ql, int qr)
    {
        if (ql <= l && r <= qr) return Answer(sumv[o], minv[o], maxv[o]);
        pushdown(o, l, r);
        int mid = l + r >> 1;
        Answer res;
        if (ql <= mid) res = up(res, query(L(o), l, mid, ql, qr));
        if (qr >= mid + 1) res = up(res, query(R(o), mid+1, r, ql, qr));
        return res;
    }
}T[21];

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int r, c, m;

int main(void)
{
    while (scanf("%d%d%d", &r, &c, &m) == 3)
    {
        for (int i = 1; i <= r; ++i) T[i].init();
        while (m--)
        {
            int op = read();
            if (op == 1)
            {
                int x1 = read(), y1 = read(), x2 = read(), y2 = read(), v = read();
                for (int i = x1; i <= x2; ++i)
                    T[i].update_add(1, 1, c, y1, y2, v);
            }
            else if (op == 2)
            {
                int x1 = read(), y1 = read(), x2 = read(), y2 = read(), v = read();
                for (int i = x1; i <= x2; ++i)
                    T[i].update_set(1, 1, c, y1, y2, v);
            }
            else
            {
                int x1 = read(), y1 = read(), x2 = read(), y2 = read();
                int sumr = 0, minr = 1000000002, maxr = -1;
                for (int i = x1; i <= x2; ++i)
                {
                    Answer ret = T[i].query(1, 1, c, y1, y2);
                    sumr += ret.sum;
                    minr = min(minr, ret.min);
                    maxr = max(maxr, ret.max);
                }
                printf("%d %d %d\n", sumr, minr, maxr);
            }
        }
    }   
    return 0;
}
```
{% endfolding %}

#### \[AHOI2009\] 维护序列

[Portal](https://www.luogu.com.cn/problem/P2023).

> 区间加，区间乘，区间求和。

根据刚才的经验，要么是现加后乘，要么是先乘后加，但是都可以吗？注意，先加后乘是无法表示的，因为当乘的标记袭来后，原来的加的标记就必须变成一个分数，这不就完蛋了吗？

实现较为简单，这里不做赘述。

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define LL long long
#define L(o) ((o) << 1)
#define R(o) (L(o) | 1)

using namespace std;

int n, p;
int a[100005];
int T[400005];
int addv[400005], mulv[400005];

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

inline void maintain(int o)
{
    T[o] = (T[L(o)] + T[R(o)]) % p;
}

void build(int o, int l, int r)
{
    mulv[o] = 1;
    if (l == r)
    {
        T[o] = a[l] % p;
        return;
    }
    int mid = l + r >> 1;
    build(L(o), l, mid);
    build(R(o), mid+1, r);
    maintain(o);
}

inline void pushdown(int o, int l, int r)
{
    int mid = l + r >> 1;

    T[L(o)] = int(((LL)T[L(o)] * mulv[o] % p + (LL)addv[o] * (mid - l + 1) % p) % p);
    T[R(o)] = int(((LL)T[R(o)] * mulv[o] % p + (LL)addv[o] * (r - mid) % p) % p);

    mulv[L(o)] = int((LL)mulv[L(o)] * mulv[o] % p);
    mulv[R(o)] = int((LL)mulv[R(o)] * mulv[o] % p);

    addv[L(o)] = int(((LL)addv[L(o)] * mulv[o] + addv[o]) % p);
    addv[R(o)] = int(((LL)addv[R(o)] * mulv[o] + addv[o]) % p);

    mulv[o] = 1;
    addv[o] = 0;
}

void update_mul(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        addv[o] = int(addv[o] * (LL)k % p);
        mulv[o] = int(mulv[o] * (LL)k % p);
        T[o] = int((LL)T[o] * k % p);
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1;
    if (x <= mid) update_mul(L(o), l, mid, x, y, k);
    if (mid + 1 <= y) update_mul(R(o), mid+1, r, x, y, k);
    maintain(o);
}

void update_add(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        addv[o] = (addv[o] + k) % p;
        T[o] = int((T[o] + (LL)k * (r - l + 1)) % p);
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1;
    if (x <= mid) update_add(L(o), l, mid, x, y, k);
    if (mid + 1 <= y) update_add(R(o), mid+1, r, x, y, k);
    maintain(o);
}

int query(int o, int l, int r, int ql, int qr)
{
    if(ql <= l && r <= qr) return T[o];
    pushdown(o, l, r);
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res = (res + query(L(o), l, mid, ql, qr)) % p;
    if (mid + 1 <= qr) res = (res + query(R(o), mid+1, r, ql, qr)) % p;
    return res;
}

int main(void)
{
    n = read(), p = read();
    for (int i = 1; i <= n; ++i) a[i] = read();
    int m = read();
    build(1, 1, n);
    while (m--)
    {
        int op = read(), x = read(), y = read();
        if (op == 1)
        {
            int k = read();
            update_mul(1, 1, n, x, y, k);
        }
        else if (op == 2)
        {
            int k = read();
            update_add(1, 1, n, x, y, k);
        }
        else printf("%d\n", query(1, 1, n, x, y));
    }
    return 0;
}
```
{% endfolding %}

## 线段树的本质

> 给定序列，支持区间模意义下每个数乘方，区间求和。

{% noteblock danger::BadCreeper 说： %}

线段树好牛逼啊，我要用线段树做这道题。

{% endnoteblock %}

但是线段树做不了这道题{% emoji sticker 滑稽喝水 %}，为什么呢？

### 区间可加性

记得之前提到的“区间可加性”吗？刚才的区间乘方操作满足这一性质吗？线段树的工作原理是将两个小区间的值合并成大区间的值。比如在最初的区间加区间查询问题中，我们可以通过 $\sum_{i=l}^{mid}A_i+\sum_{i=mid+1}^{r}A_i$ 来得到 $\sum_{i=l}^{r}A_i$，可以合并。但是区间乘方不行。

知道了这个原理之后，我们就可以解决更多的题目了。

### \[TJOI2009\] 开关

[Portal](https://www.luogu.com.cn/problem/P3870).

> 初始的全 $0$ 序列，区间异或，区间查询和。

不难发现，这个问题满足区间可加性。我们只需要修改最初区间修改的线段树的模板即可。每次异或，所有的 $0$ 会变成 $1$，所有的 $1$ 会变成 $0$，那么 `T[o] = r - l + 1 - T[o]`，此时标记要取反。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define L(o) (o << 1)
#define R(o) (o << 1 | 1)

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m;
int T[400005];
bool tag[400005];

inline void pushdown(int o, int l, int r)
{
    if (tag[o])
    {
        tag[L(o)] ^= 1;
        tag[R(o)] ^= 1;

        int mid = l + r >> 1;
        T[L(o)] = mid - l + 1 - T[L(o)];
        T[R(o)] = r - mid - T[R(o)];

        tag[o] = 0;
    }
}

void update(int o, int l, int r, int x, int y)
{
    if (x <= l && r <= y)
    {
        T[o] = r - l + 1 - T[o];
        tag[o] ^= 1;
        return;
    }

    pushdown(o, l, r);
    int mid = l + r >> 1;

    if (x <= mid) update(L(o), l, mid, x, y);
    if (mid + 1 <= y) update(R(o), mid + 1, r, x, y);
    T[o] = T[L(o)] + T[R(o)];
}

int query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];

    int mid = l + r >> 1, res = 0;
    pushdown(o, l, r);

    if (ql <= mid) res += query(L(o), l, mid, ql, qr);
    if (mid + 1 <= qr) res += query(R(o), mid + 1, r, ql, qr);

    return res;
}

int main(void)
{
    n = read(), m = read();
    while (m--)
    {
        int c = read(), a = read(), b = read();
        if (c == 0) update(1, 1, n, a, b);
        else printf("%d\n", query(1, 1, n, a, b));
    }
    return 0;
}
```
{% endfolding %}

### 延迟标记与其它

标记是什么？它是一个“欠条”，相当于告诉线段树我在这欠了东西，继续向下递归需要 `pushdown`。**但是**，如果打了一个标记，当前节点的信息无法简单维护，那么标记便不再起作用。

为什么要 `pushdown`？因为不下传标记查询没有办法进行？

实际上标记可以永久化，就是在查询的时候累计一下标记。但是只限于特殊的标记，比如区间加是可以做的，以最初的区间加区间查询和为例：

```cpp
void update(int o, int l, int r, int x, int y, int k)
{
    T[o] += (i64)k * (min(r, y) - max(l, x) + 1); // 节点直接加
    if (x <= l && r <= y)
    {
        tag[o] += k; // 标记说明它一下的都要执行这个操作
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(L(o), l, mid, x, y, k);
    if (mid+1 <= y) update(R(o), mid+1, r, x, y, k);
    // 注意这里不能 maintain，因为维护时 min,max 的操作，导致 maintain 后结果是错的
}

i64 query(int o, int l, int r, int ql, int qr, i64 add) // add 表示当前累加的标记和
{
    if (ql <= l && r <= qr) return T[o] + add * (r - l + 1); // 累计的乘上长度
    i64 res = 0;
    int mid = l + r >> 1;
    if (ql <= mid) res += query(L(o), l, mid, ql, qr, add + tag[o]); // 加上当前的标记值
    if (mid+1 <= qr) res += query(R(o), mid+1, r, ql, qr, add + tag[o]); 
    return res;
}
```

但是区间赋值不行，因为查询的过程中无法累加，操作的先后顺序会改变结果，不知道哪个是先做的，无法维护。   
所以这种做法局限性非常大，但是在嵌套和可持久化的时候，标记可能会难以下传。这时候就必须用标记永久化了。

## 扫描线

[模板](https://www.luogu.com.cn/problem/P5490)。

> 求 $n$ 个矩形的面积并。

我们把这些矩形放在平面直角坐标系中，然后手握一根竖直的线线（当然你也可以握着一条横的上下扫{% emoji sticker 滑稽喝水 %}，我们把它称之为扫描线，从左到右扫过整个坐标系，在这条扫描线上覆盖的图形长度只有在每个矩形的左右边界处会变化。

{% image https://s2.ax1x.com/2019/08/08/eTuDjP.gif::alt=图片来自 <a target="_blank" rel="noopener" href="https://www.luogu.com.cn/blog/happy404/solution-p5490"> https://www.luogu.com.cn/blog/happy404/solution-p5490</a>，可以看到在扫并起来的矩形时覆盖的长度变化 %}

也就是说，整个图形可以被分为 $n\times 2$ 段（每到一个矩形的两边长度就需要改变），每一段覆盖的长度 $L$ 是固定的（扫描这一段时的长度不变），这样就可以使用幼儿园数学{% emoji aru 132 %}求解这一段的矩形面积：$L\times 该段的宽度$。

实现上，我们取出 $n$ 个矩形的左右边界，如果一个矩形的两个对角顶点坐标分别为 $(x_1,y_1),(x_2,y_2),x_1<x_2,y_1<y_2$，那么左边界为 $(x_1,y_1,y_2,1)$，右边界为 $(x_2,y_1,y_2,-1)$，四个数分别代表 $x$ 坐标，$y$ 坐标上界，$y$ 坐标下界，对以后的面积是加 $1$ 还是减 $1$（类似差分）。现在将这 $2n$ 个四元组按照 $x$ 从小到大的顺序排序。这就相当于给了我们 $2n$ 个区间加的操作，但是 $y$ 的范围很大，所以需要离散化后才能完成，而区间加就是典型的线段树操作了。

实现的时候有一点细节：

- 我们不需要 `pushdown`，因为我们只查总区间，只需要返回 `T[1]` 即可，其余节点就让它~~自生自灭~~就行了。
- 在修改的时候 $y_2$ 要减去 $1$，因为给的是点，我们想要覆盖，令 $y$ 覆盖的是 $[y,y+1]$，那么作为上面的 $y_2$ 就需要减去 $1$。
- 这里的标记很特殊，我们用一个 `flag` 来记录，当它是正数的时候说明被覆盖了，这时候就直接赋值为对应的区间所对应的值，如果没有被覆盖就是左右儿子的和。而且它在排序后进行扫描，只可能是一个自然数。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
typedef long long i64;

struct line
{
    int x, yl, yh, flag;
    line(int x = 0, int yl = 0, int yh = 0, int flag = 0) :
        x(x), yl(yl), yh(yh), flag(flag) {}
    bool operator < (const line &a) const { return x < a.x; }
}e[2000005];

int n, m;
int b[2000005], raw[2000005], tag[8000005];
i64 T[8000005];

inline void maintain(int o, int l, int r)
{
    if (tag[o]) T[o] = raw[r + 1] - raw[l];
    else T[o] = T[o << 1] + T[o << 1 | 1];
}

void update(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        tag[o] += k;
        maintain(o, l, r);
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    maintain(o, l, r);
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1, x_, y_, x__, y__; i <= n; ++i)
    {
        scanf("%d%d%d%d", &x_, &y_, &x__, &y__);
        e[(i << 1) - 1] = line(x_, y_, y__, 1);
        e[i << 1] = line(x__, y_, y__, -1);
        b[(i << 1) - 1] = y_, b[i << 1] = y__;
    }
    sort(b + 1, b + (n << 1) + 1);
    m = unique(b + 1, b + (n << 1) + 1) - (b + 1);
    for (int i = 1; i <= (n << 1); ++i)
    {
        int pos1 = lower_bound(b + 1, b + m + 1, e[i].yl) - b;
        int pos2 = lower_bound(b + 1, b + m + 1, e[i].yh) - b;
        raw[pos1] = e[i].yl, raw[pos2] = e[i].yh;
        e[i].yl = pos1, e[i].yh = pos2;
    }
    sort(e + 1, e + (n << 1) + 1);
    i64 ans = 0;
    for (int i = 1; i < (n << 1); ++i)
    {
        update(1, 1, m, e[i].yl, e[i].yh - 1, e[i].flag);
        ans += T[1] * (e[i + 1].x - e[i].x);
    }
    printf("%lld\n", ans);
    return 0;
}
```

当然，扫描线也可以从下往上扫，像这样：

{% image https://oi-wiki.org/geometry/images/scanning.svg::alt=来自 <a target="_blank" rel="noopener" href="https://oi-wiki.org/geometry/scanning/"> OI-wiki</a> %}

## 权值线段树

对于序列 $A$ 构造一个序列 $B$，其中 $B_i$ 表示 $A$ 中数值 $i$ 出现的次数，也就是 $a_j=i$ 的 $j$ 的个数，这样的 $B$ 称之为 $A$ 的**权值数列**，对 $B$ 造一棵线段树就是**权值线段树**。

主要应用于一些计数问题，和可持久化搭配有奇效。为了使以后主席树（可持久化权值线段树，应用很多）的学习更加顺利，我们这里通过一道题来谈一下代码实现：

[逆序对](https://www.luogu.com.cn/problem/P1908)。

~~啊，不要问我问什么是这道题，因为它太经典了。~~

我们知道这道题可以用归并排序或者树状数组解决。今天我们再来用权值线段树解决它。

这种东西一般都需要先离散化。考虑枚举 $j$，对于每个 $j$ 只需要找到在它之前有多少个大于它的 $a_i$ 即可，对 $A$ 前 $j-1$ 位建立权值线段树，每次只需要查询线段树上 $[a_j+1,n]$ 的和即可，然后修改对于权值线段树来说就是点修改。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n;
int a[500005], T[2000005];

void init(void)
{
    static int tmp[500005];
    for (int i = 1; i <= n; ++i)
        tmp[i] = a[i];
    sort(tmp + 1, tmp + n + 1);
    int m = unique(tmp + 1, tmp + n + 1) - (tmp + 1);
    for (int i = 1; i <= n; ++i)
        a[i] = lower_bound(tmp + 1, tmp + m + 1, a[i]) - tmp;
}

int query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

void update(int o, int l, int r, int x)
{
    if (l == r) 
    {
        T[o]++;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x);
    else update(o << 1 | 1, mid + 1, r, x);
    T[o] = T[o << 1] + T[o << 1 | 1];
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    init();
    long long ans = 0;
    for (int i = 1; i <= n; ++i)
    {
        ans += query(1, 1, n, a[i] + 1, n);
        update(1, 1, n, a[i]);
    }
    printf("%lld\n", ans);
    return 0;
}
```

注意，虽然上述代码是正确的，但是对这道题来说显然不是最优的，因为查询不是简单的区间查询，而是一端固定的区间。但是用更通用的方式来写显然不易出错。

## 动态开点与线段树合并

通过记录左右儿子的编号，而不是使用完全二叉树的编号法则，这种方式称之为动态开点。代码大概长这样：

```cpp
struct Node
{
    int lc, rc; // 左右节点编号
    int dat; // 当前维护的值
}T[SIZE * 2]; // 终于只需要二倍空间啦！
int root, tot; // 根节点编号，节点个数

int newNode(void)
{
    ++tot;
    T[tot].lc = T[tot].rc = T[tot].dat = 0;
    return tot;
}

void update(int o, int l, int r, int x, int k)
{
    if (l == r) 
    {
        T[o].dat += k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid)
    {
        if (!T[o].lc) T[o].lc = build();
        update(T[o].lc, l, mid, x, k);
    }
    else
    {
        if (!T[o].rc) T[o].rc = build();
        update(T[o].rc, mid + 1, r, x, k);
    }
    maintain(o);
}

int main(void)
{
    tot = 0;
    root = build(); // 建树
}
```

假设现在有两棵维护相同值域的基于动态开点实现的权值线段树，现在我们想要将它们维护的值相加。这就需要通过**线段树合并**来实现，从两个根节点开始同步遍历两棵线段树，也就是说，两个指针 $o_1,o_2$，在实现中采用 $p,q$，所代表的子区间是一致的。

如果两个其中之一为空，那么返回那个非空的。如果都不是空的，那么需要递归合并两棵子树，然后删去节点 $q$，以 $p$ 作为合并的节点（维护最大值）。

```cpp
int merge(int p, int q, int l, int r)
{
    if (!p) return q; if (!q) return p;
    if (l == r)
    {
        T[p].dat += T[q].dat;
        return p;
    }
    int mid = l + r >> 1;
    T[p].lc = merge(T[p].lc, T[q].lc, l, mid);
    T[p].rc = merge(T[p].rc, T[q].rc, mid + 1, r);
    T[p].dat = max(T[T[p].lc].dat, T[T[p].rc].dat);
    return p;
}
```

时间复杂度与线段树的规模一致。

## Problemset

GSS 系列是 SPOJ 上一个很经典的系列，可以先做一下 [GSS1](https://www.luogu.com.cn/problem/SP1043) 和 [GSS3](https://www.luogu.com.cn/problem/SP1716)。

感觉内容很多？的确如此，线段树是真·最常用的数据结构，它可以解决很多的问题，这里给出一些：

### 标记处理类

是指如何拆分或变形要处理的内容，使得更容易维护。

#### \[Luogu P1438\] 无聊的数列

[Portal](https://www.luogu.com.cn/problem/P1438).

> 区间加等差数列，单点查询。

等差数列看作一个整体当成标记的话非常难维护，因为首项一直在改变。对于一次操作，可以拆成对区间的 `k-d*l`（为当前的 $l$）和 `+d*i`（为当前下标）。这个 `+d*?` 的操作只需要开一个标记，然后再查询的时候乘上当前的 $l$ 就可以了。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define L(o) (o << 1)
#define R(o) (o << 1 | 1)

using namespace std;
using i64 = long long;

int n, m;
int a[100005];
i64 tagk[400005], tagd[400005];

inline void pushdown(int o, int l, int r)
{
    int mid = l + r >> 1;
    tagk[L(o)] += tagk[o], tagk[R(o)] += tagk[o];
    tagd[L(o)] += tagd[o], tagd[R(o)] += tagd[o];
    tagk[o] = tagd[o] = 0;
}

void update(int o, int l, int r, int x, int y, int k, int d)
{
    if (x <= l && r <= y)
    {
        tagk[o] += k;
        tagd[o] += d;
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1;
    if (x <= mid) update(L(o), l, mid, x, y, k, d);
    if (mid + 1 <= y) update(R(o), mid + 1, r, x, y, k, d);
}

i64 query(int o, int l, int r, int p)
{
    if (l == r) return tagk[o] + l * tagd[o];
    int mid = l + r >> 1;
    pushdown(o, l, r);
    if (p <= mid) return query(L(o), l, mid, p);
    return query(R(o), mid + 1, r, p);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
    while (m--)
    {
        int opt; scanf("%d", &opt);
        if (opt == 1)
        {
            int l, r; i64 k, d;
            scanf("%d%d%lld%lld", &l, &r, &k, &d);
            update(1, 1, n, l, r, k - d * l, d);
        }
        else 
        {
            int p; scanf("%d", &p);
            printf("%lld\n", query(1, 1, n, p) + a[p]);
        }
    }
    return 0;
}
```
{% endfolding %}

为什么是单点查询？因为查询的时候每个下标是变化的，`+d*?` 的 `?` 一直在变化，只能做单点。

#### [Luogu P1471] 方差

[Portal](https://www.luogu.com.cn/problem/P1471).

将方差公式随便导一下，然后维护两棵线段树，分别维护区间和和区间平方和。当前节点的区间平方和维护同样需要拆开，但都非常好做。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
double a[100005], T1[400005], T2[400005];
double tag[400005];

inline void maintain(int o)
{
    T1[o] = T1[o << 1] + T1[o << 1 | 1];
    T2[o] = T2[o << 1] + T2[o << 1 | 1];
}

inline void pushdown(int o, int l, int r)
{
    if (!tag[o]) return;
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    tag[ls] += tag[o], tag[rs] += tag[o];
    T2[ls] += tag[o] * tag[o] * (mid - l + 1) + 2 * T1[ls] * tag[o], T2[rs] += tag[o] * tag[o] * (r - mid) + 2 * T1[rs] * tag[o];
    T1[ls] += tag[o] * (mid - l + 1), T1[rs] += tag[o] * (r - mid);
    tag[o] = 0;
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        T1[o] = a[l];
        T2[o] = a[l] * a[l];
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o);
}

void update(int o, int l, int r, int x, int y, double k)
{
    if (x <= l && r <= y)
    {
        T2[o] += k * k * (r - l + 1) + 2 * T1[o] * k;
        T1[o] += (r - l + 1) * k;
        tag[o] += k;
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    maintain(o);
}

double query(double *T, int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    pushdown(o, l, r);
    int mid = l + r >> 1;
    double res = 0;
    if (ql <= mid) res += query(T, o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(T, o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i)
        scanf("%lf", a + i);
    build(1, 1, n);
    while (m--)
    {
        int op, x, y;
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1)
        {
            double k;
            scanf("%lf", &k);
            update(1, 1, n, x, y, k);
        }
        else if (op == 2) printf("%.4lf\n", query(T1, 1, 1, n, x, y) / (y - x + 1));
        else
        {
            double p = query(T1, 1, 1, n, x, y), q = query(T2, 1, 1, n, x, y);
            p /= (y - x + 1);
            printf("%.4lf\n", q / (y - x + 1) - p * p);
        }
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P6327] 区间加区间 sin 和

[Portal](https://www.luogu.com.cn/problem/P6327).

高中课本介绍了三角函数的和差角公式：

$$
\sin(\alpha+\beta)=\sin \alpha \cos \beta + \cos\alpha\sin\beta\\
\cos(\alpha+\beta)=\cos\alpha\cos\beta - \sin\alpha\sin\beta
$$

维护两个量 `Sin` 和 `Cos`，记录一个标记 `tag`，`update` 和 `pushdown` 的时候用公式维护加上标记的值的三角函数值即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

#define i64 long long

using namespace std;

int read(void)
{
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int n, m;
int a[200005];
i64 tag[800005];
double Sin[800005], Cos[800005];

inline void maintain(int o)
{
    Sin[o] = Sin[o << 1] + Sin[o << 1 | 1];
    Cos[o] = Cos[o << 1] + Cos[o << 1 | 1];
}

inline void maintain(int o, double sinx, double cosx)
{
    double sina = Sin[o], cosa = Cos[o];
    Sin[o] = sina * cosx + cosa * sinx;
    Cos[o] = cosa * cosx - sina * sinx;
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        Sin[o] = sin(a[l]);
        Cos[o] = cos(a[l]);
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o);
}

inline void pushdown(int o)
{
    if (!tag[o]) return;
    double sinx = sin(tag[o]), cosx = cos(tag[o]);
    maintain(o << 1, sinx, cosx);
    maintain(o << 1 | 1, sinx, cosx);
    tag[o << 1] += tag[o];
    tag[o << 1 | 1] += tag[o];
    tag[o] = 0;
}

int k;
double sink, cosk;
void update(int o, int l, int r, int x, int y)
{
    if (x <= l && r <= y)
    {
        maintain(o, sink, cosk);
        tag[o] += k;
        return;
    }
    pushdown(o);
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y);
    maintain(o);
}

double query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return Sin[o];
    pushdown(o);
    double res = 0;
    int mid = l + r >> 1;
    if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

int main(void)
{
    n = read();
    for (int i = 1; i <= n; ++i) a[i] = read();
    build(1, 1, n);
    m = read();
    while (m--)
    {
        int op = read();
        if (op == 1)
        {
            int l = read(), r = read();
            k = read();
            sink = sin(k), cosk = cos(k);
            update(1, 1, n, l, r);
        }
        else 
        {
            int l = read(), r = read();
            printf("%.1lf\n", query(1, 1, n, l, r));
        }
    }
    return 0;
}
```
{% endfolding %}

### 区间和并类

最经典的问题就是小白逛公园，单点修改区间最大子段和，这种分治思路可以解决很多问题。

#### [COCI2010-2011#6] STEP

[Portal](https://www.luogu.com.cn/problem/P6492).

简单题。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, q;
bool a[200005];

struct Node
{
    int dat;
    int lmax, rmax;
}T[800005];

inline void maintain(int o, int l, int r)
{
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    T[o].lmax = T[ls].lmax, T[o].rmax = T[rs].rmax, T[o].dat = max(T[ls].dat, T[rs].dat);
    if (a[mid] != a[mid + 1])
    {
        T[o].dat = max(T[o].dat, T[ls].rmax + T[rs].lmax);
        if (T[ls].lmax == mid - l + 1) T[o].lmax = T[ls].dat + T[rs].lmax;
        if (T[rs].rmax == r - mid) T[o].rmax = T[rs].dat + T[ls].rmax;
    }
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o].dat = T[o].lmax = T[o].rmax = 1;
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o, l, r);
}

void update(int o, int l, int r, int x)
{
    if (l == r) 
    {
        a[x] ^= 1;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x);
    else update(o << 1 | 1, mid + 1, r, x);
    maintain(o, l, r);
}

int main(void)
{
    scanf("%d%d", &n, &q);
    build(1, 1, n);
    while (q--)
    {
        int x;
        scanf("%d", &x);
        update(1, 1, n, x);
        printf("%d\n", T[1].dat);
    }
    return 0;
}
```
{% endfolding %}

#### 「Wdsr-2.7」文文的摄影布置

[Portal](https://www.luogu.com.cn/problem/P7706).

观察条件 $A_i+A_k-\min\{B_j\},i<j<k$，我们在线段树的节点中维护 $A$ 的最大值和 $B$ 的最小值，以及区间答案 $ans$。

现在难就难在满足线段树的“区间可加性”，也就是如何从左右儿子合并出当前节点的答案。$\min\{B_j\}$ 可以当成一个值，就是区间 $B$ 最小值。肯定可以三个数全从左子节点或右子节点过来，也可以两个数从一个节点过来，一个数从另一个节点过来。这样的话，我们记 $lmax$ 代表 $A_i-B_j$ 的最大值，$rmax$ 代表 $A_k-B_j$ 的最大值。这两个可以简单维护，要么从左右节点单独过来，要么两个下标在不同的区间，而且由于 $i<j<k$，所以顺序一定。这样 $ans$ 就要么是左子节点的 $lmax$ 和右子节点的 $amax$ 合并过来，要么是从右子节点的 $rmax$ 和左子节点的 $lmax$ 合并过来。

那么 $lmax$ 和 $rmax$ 呢？大致同理，要么都在叶子节点，要么跨区间，跨区间的时候就是通过维护的 $A,B$ 值来计算即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
const int INF = 1e9;

int n, m;
int a[500005], b[500005];

struct Node
{
    int amax, bmin;
    int lmax, rmax, ans;
}T[2000005];

inline Node merge(Node a, Node b)
{
    Node ans;
    ans.amax = max(a.amax, b.amax);
    ans.bmin = min(a.bmin, b.bmin);
    ans.lmax = max({a.lmax, b.lmax, a.amax - b.bmin});
    ans.rmax = max({a.rmax, b.rmax, b.amax - a.bmin});
    ans.ans = max({a.ans, b.ans, a.amax + b.rmax, a.lmax + b.amax});
    return ans;
}

void build(int o, int l, int r)
{
    T[o].lmax = T[o].rmax = T[o].ans = -INF; // 初始什么都没有，是负无穷
    if (l == r)
    {
        T[o].amax = a[l], T[o].bmin = b[l];
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    T[o] = merge(T[ls], T[rs]);
}

void update(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o].amax = a[l], T[o].bmin = b[l];
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    if (x <= mid) update(ls, l, mid, x, k);
    else update(rs, mid + 1, r, x, k);
    T[o] = merge(T[ls], T[rs]);
}

Node query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    if (qr <= mid) return query(ls, l, mid, ql, qr);
    if (mid < ql) return query(rs, mid + 1, r, ql, qr);
    return merge(query(ls, l, mid, ql, qr), query(rs, mid + 1, r, ql, qr));
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    for (int i = 1; i <= n; ++i) scanf("%d", b + i);
    build(1, 1, n);
    while (m--)
    {
        int op, x, y;
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1)
        {
            a[x] = y;
            update(1, 1, n, x, y);
        }
        else if (op == 2) 
        {
            b[x] = y;
            update(1, 1, n, x, y);
        }
        else printf("%d\n", query(1, 1, n, x, y).ans);
    }
    return 0;
}
```
{% endfolding %}

#### [USACO08FEB] Hotel G

[Portal](https://www.luogu.com.cn/problem/P2894).

这道题还是区间的合并问题，只不过成了区间修改。还是定义 `lmax, rmax` 为左右起的连续空房的区间长度，实现对于读到这里的读者来说应该不是困难。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, m;

struct Node
{
    int dat; // 区间最大长度的最大值
    int lmax, rmax; // 从左右起的最大空房数
    int tag; // 0 退房，1 住房
}T[200005];

inline void maintain(int o, int l, int r)
{
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    if (T[ls].dat == mid - l + 1) T[o].lmax = T[ls].dat + T[rs].lmax;
    else T[o].lmax = T[ls].lmax;
    if (T[rs].dat == r - mid) T[o].rmax = T[rs].dat + T[ls].rmax;
    else T[o].rmax = T[rs].rmax;
    T[o].dat = max({T[ls].dat, T[rs].dat, T[ls].rmax + T[rs].lmax});
}

inline void pushdown(int o, int l, int r)
{
    if (T[o].tag == -1) return;
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    T[ls].tag = T[rs].tag = T[o].tag;
    if (T[o].tag == 1)
    {
        T[ls].dat = T[ls].lmax = T[ls].rmax = 0;
        T[rs].dat = T[rs].lmax = T[rs].rmax = 0;
    }
    else
    {
        T[ls].dat = T[ls].lmax = T[ls].rmax = mid - l + 1;
        T[rs].dat = T[rs].lmax = T[rs].rmax = r - mid;
    }
    T[o].tag = -1;
}

void build(int o, int l, int r)
{
    T[o].dat = T[o].lmax = T[o].rmax = r - l + 1;
    T[o].tag = -1;
    if (l == r) return;
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
}

void update(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        if (k == 1) T[o].dat = T[o].lmax = T[o].rmax = 0;
        else T[o].dat = T[o].lmax = T[o].rmax = r - l + 1;
        T[o].tag = k;
        return;
    }
    pushdown(o, l, r);
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    maintain(o, l, r);
}

int query(int o, int l, int r, int x)
{
    int mid = l + r >> 1, ls = o << 1, rs = o << 1 | 1;
    pushdown(o, l, r);
    if (T[ls].dat >= x) return query(ls, l, mid, x);
    if (T[ls].rmax + T[rs].lmax >= x) return mid - T[ls].rmax + 1;
    return query(rs, mid + 1, r, x);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    build(1, 1, n);
    while (m--)
    {
        int op, x, y;
        scanf("%d%d", &op, &x);
        if (op == 1)
        {
            if (T[1].dat >= x)
            {
                int t = query(1, 1, n, x);
                printf("%d\n", t);
                update(1, 1, n, t, t + x - 1, 1);
            }
            else puts("0");
        }
        else
        {
            scanf("%d", &y);
            update(1, 1, n, x, x + y - 1, 0);
        }
    }
    return 0;
}
```
{% endfolding %}

#### [SCOI2010] 序列操作

[Portal](https://www.luogu.com.cn/problem/P2572).

很显然还是区间合并，但是 01 我们都要维护。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#define rep(i) for (int i = 0; i < 2; ++i)
#define ls (o << 1)
#define rs (ls | 1) 

using namespace std;

struct Node {
    int sum[2], lmax[2], rmax[2], dat[2];
    int setv, rev;
    Node() : setv(-1), rev(0) {}
} T[400005];

int n, m;
int a[100005];

inline Node hb(const Node &a, const Node &b) {
    Node c;
    rep(i) {
        c.sum[i] = a.sum[i] + b.sum[i];
        c.lmax[i] = (a.sum[1 - i] ? a.lmax[i] : a.sum[i] + b.lmax[i]);
        c.rmax[i] = (b.sum[1 - i] ? b.rmax[i] : b.sum[i] + a.rmax[i]);
        c.dat[i] = max({a.dat[i], b.dat[i], a.rmax[i] + b.lmax[i]});
    }
    return c;
}

void pushup(int o) {
    rep(i) {
        T[o].sum[i] = T[ls].sum[i] + T[rs].sum[i];
        T[o].lmax[i] = (T[ls].sum[1 - i] ? T[ls].lmax[i] : T[ls].sum[i] + T[rs].lmax[i]);
        T[o].rmax[i] = (T[rs].sum[1 - i] ? T[rs].rmax[i] : T[rs].sum[i] + T[ls].rmax[i]);
        T[o].dat[i] = max({T[ls].dat[i], T[rs].dat[i], T[ls].rmax[i] + T[rs].lmax[i]});
    }
}

void pushdown(int o, int l, int r) { // 先执行 set，再执行 rev
    int mid = l + r >> 1;
    if (T[o].setv != -1) {
        T[ls].setv = T[rs].setv = T[o].setv;
        T[ls].rev = T[rs].rev = 0;
        rep(i) {
            T[ls].sum[i] = T[ls].lmax[i] = T[ls].rmax[i] = T[ls].dat[i] = (mid - l + 1) * (T[o].setv == i);
            T[rs].sum[i] = T[rs].lmax[i] = T[rs].rmax[i] = T[rs].dat[i] = (r - mid) * (T[o].setv == i);
        }
        T[o].setv = -1;
    }
    if (T[o].rev) {
        T[ls].rev ^= 1; T[rs].rev ^= 1;
        swap(T[ls].sum[0], T[ls].sum[1]); swap(T[rs].sum[0], T[rs].sum[1]);
        swap(T[ls].lmax[0], T[ls].lmax[1]); swap(T[rs].lmax[0], T[rs].lmax[1]);
        swap(T[ls].rmax[0], T[ls].rmax[1]); swap(T[rs].rmax[0], T[rs].rmax[1]);
        swap(T[ls].dat[0], T[ls].dat[1]); swap(T[rs].dat[0], T[rs].dat[1]);
        T[o].rev = 0;
    }
}

void build(int o, int l, int r) {
    if (l == r) {
        rep(i) T[o].sum[i] = T[o].lmax[i] = T[o].rmax[i] = T[o].dat[i] = (a[l] == i);
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    pushup(o);
}

void update(int o, int l, int r, int x, int y, int k) {
    if (x <= l && r <= y) {
        if (k <= 1) {
            T[o].setv = k; T[o].rev = 0;
            rep(i) T[o].sum[i] = T[o].lmax[i] = T[o].rmax[i] = T[o].dat[i] = (r - l + 1) * (i == k);
        } else {
            T[o].rev ^= 1;
            swap(T[o].sum[0], T[o].sum[1]);
            swap(T[o].lmax[0], T[o].lmax[1]);
            swap(T[o].rmax[0], T[o].rmax[1]);
            swap(T[o].dat[0], T[o].dat[1]);   
        }
        return;
    }
    pushdown(o, l, r); int mid = l + r >> 1;
    if (x <= mid) update(ls, l, mid, x, y, k);
    if (mid < y) update(rs, mid + 1, r, x, y, k);
    pushup(o);
}

Node query(int o, int l, int r, int x, int y) {
    if (x <= l && r <= y) return T[o];
    pushdown(o, l, r); int mid = l + r >> 1;
    if (y <= mid) return query(ls, l, mid, x, y);
    if (mid < x) return query(rs, mid + 1, r, x, y);
    return hb(query(ls, l, mid, x, y), query(rs, mid + 1, r, x, y));
}

int main(void) {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    build(1, 1, n);
    while (m--) {
        int op, l, r;
        scanf("%d%d%d", &op, &l, &r); ++l; ++r;
        if (op < 3) update(1, 1, n, l, r, op);
        else if (op == 3) printf("%d\n", query(1, 1, n, l, r).sum[1]);
        else printf("%d\n", query(1, 1, n, l, r).dat[1]);
    }
    return 0;
}
```
{% endfolding %}

### 奇怪的技巧

这里是线段树的一些经典应用。

#### [Luogu P4145] 上帝造题的七分钟 2 / 花神游历各国

[Portal](https://www.luogu.com.cn/problem/P4145).

懒标记？

如果您能提出质疑，那么笔者为您点赞{% emoji tong doge %}。如果不能，你可能要重新去看《线段树的本质》一节（~~笔者要被扣工资了~~）{% emoji tong ... %}。

要注意到的是，如果使用延迟标记，那么当前的区间和是无法维护的。因为它不像区间加区间 sin 和这种东西可以进行拆解，每个数开平方后区间的和无法简单维护。

但是区间开方这种东西，很容易就开到 $1$ 了。然而对着 $1$ 开方是没有用的。所以如果区间的最大值是 $1$，那么区间开方这种操作就没必要进行了。

我们直接使用线段树，但是不需要延迟标记，维护到叶子节点为止。如果一个区间已经比 $1$ 小，那么就不用维护了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

#define i64 long long

using namespace std;

int n, m;
i64 a[100005];
i64 sum[400005], maxx[400005];

inline void maintain(int o)
{
    sum[o] = sum[o << 1] + sum[o << 1 | 1];
    maxx[o] = max(maxx[o << 1], maxx[o << 1 | 1]);
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        sum[o] = maxx[o] = a[l];
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o);
}

void update(int o, int l, int r, int x, int y)
{
    if (l == r) // 叶子节点重新维护
    {
        sum[o] = sqrt(sum[o]);
        maxx[o] = sqrt(maxx[o]);
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid && maxx[o << 1] > 1) update(o << 1, l, mid, x, y); // 最大值大于 1 才修改
    if (mid < y && maxx[o << 1 | 1] > 1) update(o << 1 | 1, mid + 1, r, x, y);
    maintain(o);
}

i64 query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return sum[o];
    int mid = l + r >> 1;
    i64 res = 0;
    if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%lld", a + i);
    scanf("%d", &m);
    build(1, 1, n);
    while (m--)
    {
        int k, l, r;
        scanf("%d%d%d", &k, &l, &r);
        if (l > r) swap(l, r);
        if (k == 0) update(1, 1, n, l, r);
        else printf("%lld\n", query(1, 1, n, l, r));
    }
    return 0;
}
```
{% endfolding %}

然后可以做一下双倍经验题：[GSS4](https://www.luogu.com.cn/problem/SP2713)。

#### [Codeforces 438D] The Child and Sequence

[Portal](https://codeforces.com/problemset/problem/438/D).

注意到这跟上一题很像，因为取模运算的次数也是 $\log$ 级别，而单点修改也掀不起什么风浪，这样暴力维护也可以接受。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int n, m;
int a[100005];
i64 T[400005], maxx[400005];

inline void maintain(int o)
{
    T[o] = T[o << 1] + T[o << 1 | 1];
    maxx[o] = max(maxx[o << 1], maxx[o << 1 | 1]);
}

void build(int o, int l, int r)
{
    if (l == r)
    {
        T[o] = maxx[o] = a[l];
        return;
    }
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
    maintain(o);
}

i64 query(int o, int l, int r, int ql, int qr)
{
    if (ql <= l && r <= qr) return T[o];
    i64 res = 0;
    int mid = l + r >> 1;
    if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
    if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

void update(int o, int l, int r, int x, int y, int k)
{
    if (l == r)
    {
        T[o] %= k, maxx[o] %= k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid && maxx[o << 1] >= k) update(o << 1, l, mid, x, y, k);
    if (mid < y && maxx[o << 1 | 1] >= k) update(o << 1 | 1, mid + 1, r, x, y, k);
    maintain(o);
}

void modify(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o] = k, maxx[o] = k;
        return;
    }
    int mid = l + r >> 1;
    if (x <= mid) modify(o << 1, l, mid, x, k);
    else modify(o << 1 | 1, mid + 1, r, x, k);
    maintain(o);
}

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) 
        scanf("%d", a + i);
    build(1, 1, n);
    while (m--)
    {
        int op, x, y, k;
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1) printf("%lld\n", query(1, 1, n, x, y));
        else if (op == 2)
        {
            scanf("%d", &k);
            update(1, 1, n, x, y, k);
        }
        else modify(1, 1, n, x, y);
    }
    return 0;
}
```
{% endfolding %}

#### [TJOI2018] 数学计算

[Portal](https://www.luogu.com.cn/problem/P4588).

这不是模拟吗（

但是不行，我们知道除法是没有随时取模性质的，就算算逆元，也没有保证互质，逆元不一定有。

注意到最多除一次，以时间建立一棵线段树，根节点维护的是当前的 $x$ 值。对于一个乘操作，我们就将当前询问的编号乘上 $x$，对于除法，我们就将这个编号改为 $1$。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int Q, M;
int T[400005];

void build(int o, int l, int r)
{
    T[o] = 1;
    if (l == r) return;
    int mid = l + r >> 1;
    build(o << 1, l, mid);
    build(o << 1 | 1, mid + 1, r);
}

void update(int o, int l, int r, int x, int k)
{
    if (l == r)
    {
        T[o] = (k == 0) ? 1 : k;
        return;
    }
    int mid = l + r >> 1, ls = o << 1, rs = ls | 1;
    if (x <= mid) update(ls, l, mid, x, k);
    else update(rs, mid + 1, r, x, k);
    T[o] = (i64)T[ls] * T[rs] % M;
}

int main(void)
{
    int TT;
    scanf("%d", &TT);
    while (TT--)
    {
        scanf("%d%d", &Q, &M);
        build(1, 1, Q);
        for (int i = 1; i <= Q; ++i)
        {
            int op, x;
            scanf("%d%d", &op, &x);
            if (op == 1) update(1, 1, Q, i, x);
            else update(1, 1, Q, x, 0);
            printf("%d\n", T[1] % M);
        }
    }
    return 0;
}
```
{% endfolding %}

{% span bold red::这种基于时间的操作非常常见，请读者一定要熟记。 %}

### 其它线段树

包括扫描线、权值线段树和动态开点线段树等内容。

#### [Luogu P1637] 三元上升子序列

[Portal](https://www.luogu.com.cn/problem/P1637).

直接使用权值线段树维护即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, m;
int a[30005], b[30005];
int pre[30005], suf[30005];

struct SugmentTree
{
    int T[120005];
    void update(int o, int l, int r, int x, int k)
    {
        if (l == r)
        {
            T[o] += k;
            return;
        }
        int mid = l + r >> 1;
        if (x <= mid) update(o << 1, l, mid, x, k);
        else update(o << 1 | 1, mid + 1, r, x, k);
        T[o] = T[o << 1] + T[o << 1 | 1];
    }
    int query(int o, int l, int r, int ql, int qr)
    {
        if (ql <= l && r <= qr) return T[o];
        int mid = l + r >> 1, res = 0;
        if (ql <= mid) res += query(o << 1, l, mid, ql, qr);
        if (mid < qr) res += query(o << 1 | 1, mid + 1, r, ql, qr);
        return res;
    }
}A, B;

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i), b[i] = a[i];
    sort(b + 1, b + n + 1);
    m = unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; ++i)
        a[i] = lower_bound(b + 1, b + m + 1, a[i]) - b;
    for (int i = 1; i <= n; ++i)
    {
        if (a[i] > 1) pre[i] = A.query(1, 1, n, 1, a[i] - 1);
        A.update(1, 1, n, a[i], 1);
    }
    for (int i = n; i >= 1; --i)
    {
        if (a[i] < n) suf[i] = B.query(1, 1, n, a[i] + 1, n);
        B.update(1, 1, n, a[i], 1);
    }
    long long ans = 0;
    for (int i = 2; i < n; ++i) ans += 1ll * pre[i] * suf[i];
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1502] 窗口的星星

[Portal](https://www.luogu.com.cn/problem/P1502).

我们将每个星星看成一个左下角为 $(x,y)$，右上角为 $(x+w-1,y+h-1)$ 的矩形，窗户看成一个点，那么使用扫描线维护平面中的最大值即可。而且 $x$ 坐标的值相对与结果没有关系，所以也不需要记录 $val,raw$。

由于区间最大值不能标记永久化，所以还是要正常下传标记。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;
typedef long long i64;

struct Line
{
    int x, yl, yh, c;
    Line(int x = 0, int yl = 0, int yh = 0, int c = 0) :
        x(x), yl(yl), yh(yh), c(c) {}
    bool operator < (const Line &a) const
    {
        // 先扫加的来保证结果正确
        if (x != a.x) return x < a.x;
        else return c > a.c;
    }
}e[20005];

int n, W, H, m;
int b[20005];
i64 T[80005], tag[80005];

inline void pushdown(int o)
{
    if (!tag[o]) return;
    tag[o << 1] += tag[o];
    tag[o << 1 | 1] += tag[o];
    T[o << 1] += tag[o];
    T[o << 1 | 1] += tag[o];
    tag[o] = 0;
}

void update(int o, int l, int r, int x, int y, int k)
{
    if (x <= l && r <= y)
    {
        T[o] += k;
        tag[o] += k;
        return;
    }
    pushdown(o);
    int mid = l + r >> 1;
    if (x <= mid) update(o << 1, l, mid, x, y, k);
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k);
    T[o] = max(T[o << 1], T[o << 1 | 1]);
}

void solve(void)
{
    scanf("%d%d%d", &n, &W, &H);
    for (int i = 1, x, y, l; i <= n; ++i)
    {
        scanf("%d%d%d", &x, &y, &l);
        e[(i << 1) - 1] = Line(x, y, y + H - 1, l);
        e[i << 1] = Line(x + W - 1, y, y + H - 1, -l);
        b[(i << 1) - 1] = y, b[i << 1] = y + H - 1;
    }
    sort(b + 1, b + (n << 1) + 1);
    m = unique(b + 1, b + (n << 1) + 1) - (b + 1);
    for (int i = 1; i <= (n << 1); ++i)
    {
        int pos1 = lower_bound(b + 1, b + m + 1, e[i].yl) - b;
        int pos2 = lower_bound(b + 1, b + m + 1, e[i].yh) - b;
        e[i].yl = pos1, e[i].yh = pos2;
    }
    sort(e + 1, e + (n << 1) + 1);
    i64 ans = 0;
    memset(T, 0, sizeof(T));
    memset(tag, 0, sizeof(tag));
    for (int i = 1; i <= (n << 1); ++i)
    {
        update(1, 1, n << 1, e[i].yl, e[i].yh, e[i].c);
        ans = max(ans, T[1]);
    }
    printf("%lld\n", ans);
}

int main(void)
{
    int T;
    scanf("%d", &T);
    while (T--) solve();
    return 0;
}
```
{% endfolding %}

## 小结

一定要熟练掌握线段树。在这之后推荐学习更多数据结构，以及嵌套数据结构和可持久化。
