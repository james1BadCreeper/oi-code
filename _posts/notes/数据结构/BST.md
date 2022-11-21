---
title: 二叉搜索树与平衡树
plugins:
  - katex
headimg: 'https://z3.ax1x.com/2021/10/04/4XhyY6.jpg'
cover: false
categories:
    - 算法竞赛
    - 学习笔记
    - 数据结构
tag:
    - 平衡树
    - Treap
    - Splay
    - FHQ-Treap
abbrlink: 9fce88cb
date: 2022-09-07 23:15:22
group: notes
hide: true
---

> 二叉搜索树（Binary Search Tree, BST）是一种二叉树的树形数据结构，能高效地解决许多其它数据结构所不能解决的问题，但由于自身是一个不稳定，容易退化的数据结构，所以需要用特殊手段保证其平衡，本文将引导你学习 BST，以及三种最为常见的平衡树：Treap、Splay 和 fhq-treap。

<!--more-->

{% folding blue::更新日志 %}

{% timeline %}

{% timenode 2022/09/07 %}

开始对旧文章进行全面重构。

{% endtimenode %}

{% endtimeline %}

{% endfolding %}

## 概念

二叉树有一种性质叫做二叉搜索树性质，就是说对于树中的一个节点，它的键值不小于它左子树的键值，不大于它右子树的键值，这就是所谓的“BST 性质”。

BST 的递归定义如下：

1. 空树是 BST；   
2. 若 BST 的左子树不为空，则其左子树上**所有点**的附加权值均小于其根节点的值；     
3. 若 BST 的右子树不为空，则其右子树上**所有点**的附加权值均大于其根节点的值；    
4. BST 的左右子树均为 BST；   
5. BST 集合是满足 1、2、3、4 的最小二叉树集。

所以输出 BST 的中序遍历就是原序列排序的结果。

需要注意的是，一般的，BST 所有节点的键值都不相等。

## 普通 BST

[模板](https://www.luogu.com.cn/problem/P5076)。

{% noteblock %}

您需要写一种数据结构，来维护一些数（ 都是 $10^9$ 以内的数字）的集合，最开始时集合是空的。其中需要提供以下操作，操作次数 $q$ 不超过 $10^4$：

1. 查询 $x$ 数的排名（排名定义为比当前数小的数的个数 $+1$。若有多个相同的数，应输出最小的排名）。   
2. 查询排名为 $x$ 的数。   
3. 求 $x$ 的前驱（前驱定义为小于 $x$，且最大的数）。若未找到则输出 $-2147483647$。   
4. 求 $x$ 的后继（后继定义为大于 $x$，且最小的数）。若未找到则输出 $2147483647$。   
5. 插入一个数 $x$。   

{% endnoteblock %}

BST 的节点会这样定义：

```cpp
struct Node
{
    int l, r; // 左右儿子
    int val; // 键值
    int cnt, size; // 表示这个节点数个个数，和子树的大小
}T[maxn];
```

辅助函数如下：

```cpp
int tot = 0; // 用数组模拟链表
inline int newNode(int val) // 新建节点，并返回编号
{
    T[++tot] = val;
    T[tot].size = T[tot].cnt = 1;
    return tot;
}
inline void maintain(int p) // 维护当前节点的信息
{
    T[p].size = T[T[p].l].size + T[T[p].r].size + T[p].cnt;
}
```

为了使边界处理情况更为方便，我们会插入一个 `INF` 节点和一个 `-INF` 节点，这样建树：

```cpp
int root = 1; // 定义根节点，必须这么做的原因是在平衡树中会出现树根改变的情况
const int INF = 2147483647; // 定义 INF
void build(void)
{
    newNode(-INF), newNode(INF);
    root = 1, T[1].r = 2;
}
```

求排名：

```cpp
int Rank(int p, int val)
{
    if (p == 0) return 1;
    // 只有在此 val 不存在时才会访问到 0 号节点，根据定义，排名应该 +1，所以返回 1。
    // 在不同情况下这里会返回不同的值，比如如果次数不存在输出 -1 这里就应该返回 ERROR_BAD_USAGE，若数不存在就不用 +1，则返回 0。
    if (val == T[p].val) return T[T[p].l].size + 1; // 找到节点，返回
    if (val < T[p].val) return Rank(T[p].l, val); // 小，在左子树中找
    return Rank(T[p].r, val) + T[T[p].l].size + T[p].cnt; // 大，右子树找，注意要加上左子树和此节点
}
```

求排名为 $k$ 的数：

```cpp
int kth(int p, int rnk)
{
    if (p == 0) return INF; // 0 意味着当前树上的节点数小于 rnk，返回 INF
    if (T[T[p].l].size >= rnk) return kth(T[p].l, rnk); // 在左子节点
    if (T[T[p].l].size + T[p].num >= rnk) return T[p].val; // 当前节点
    return kth(T[p].r, rnk - T[T[p].l].size - T[p].cnt); // 右子节点
}
```

怎么查前驱呢？初始 `ans = 1`，检索 `val`，有三种可能的结果：

1. BST 中没有 val。后继一定在已经遍历的节点中，这一点可以用微扰来证明。
2. 有 val 节点，但是这个节点没有右子树。这种情况的答案同 1。
3. 有 val 节点，有右子树。答案是右子树的最低端。

代码（请读者自行实现后继）：

```cpp
inline int GetPre(int val)
{
    int ans = 1, p = root; // T[1].val == -INF
    while (p)
    {
        if (val == T[p].val)
        {
            if (T[p].l) // 存在左子节点
            {
                p = T[p].l;
                while (T[p].r) p = T[p].r; // 使劲往右走
                ans = p;
            }
            break;
        }
        if (T[p].val < val && T[p].val > T[ans].val) ans = p; // 尝试更新 ans
        p = val < T[p].val ? T[p].l : T[p].r; // 遍历
    }
    return T[ans].val;
}
```

当然前驱和后继也可以直接使用 Rank 和 kth 函数实现，大概像这样：

```cpp
kth(root, Rank(root, x) - 1) // 前驱
kth(root, Rank(root, x) + 1) // 后继，但是应该写成 kth(root, Rank(root, x + 1))
```

由于这里是严格大于和严格小于，所以说排名不能简单地写成 `+1`，边界条件非常容易写错，所以还是推荐大家来写前驱后继函数。

插入的代码非常好写：

```cpp
void insert(int &p, int val) // p 是父亲节点
{
    if (p == 0)
    {
        p = newNode(val); // 新建节点
        return;
    }
    if (val == T[p].val) // 有这一键值
    {
        ++T[p].cnt; // 将个数加上 1
        maintain(p);
        return;
    }
    // 接下来看它是在左子树还是在右子树
    if (val < T[p].val) insert(T[p].l, val); // 根据 BST 的定义，在左子树
    else insert(T[p].r, val); // 在右子树
    maintain(p); // 由于进行了插入，当前节点的信息需要重新计算
}
```

注意我们在有这一键值的时候并没有重新计算父亲节点，因为插入是递归进行的，父亲节点的附加信息一定会被重新计算。

## 平衡 BST

但是这样的 BST 是个假的，因为如果插入 `1 2 3 4 5 6 7`，它就会变成一条链。

### 简介

表达同种意思的 BST 有多种，有平衡的，有不平衡的。在数据随机的情况下，它就是平衡的。不平衡怎么办？给他搞成平衡的呗{% emoji 滑稽 %}。

那为什么平衡树有快有慢呢？这是因为，越快的平衡树能使平衡树越满。但有些平衡树实现过于复杂，比如红黑树（Red-Black Tree，简称 RBT），它的插入有 $5$ 种情况，删除有 $6$ 种情况，在考场上根本打不出来（当然工程中你必须打），所以一般情况下我们不使用红黑树。

但要注意的是，**尽量不要在考场手写上写平衡树**。若 STL 能满足要求，就使用 STL。STL 的红黑树（Red-Black Tree，RBT，号称最快平衡树）开了 O2 以后，跑得比大多数手写平衡树都要快（不信你可以学完平衡树后自己实现一个山寨版的 `set` 来和 STL 比比速度）。尽量不要自己再造轮子，造不好就被碾着腿了。

在工程中，常用的平衡树是 AVL、RBT 和 B 树（B 树不是二叉树，而是多叉的）；而在竞赛中，常用的则是 Treap 和 Splay（伸展树）。当然还有诸如 WBLT，替罪羊树等平衡树。本文会介绍旋转 Treap、Splay 和非旋 Treap。

### 平衡的思路

有三种：旋转，分裂与合并，重构。   
但它们的目的都是相同的：平衡我们的二叉搜索树。

对于旋转，有单旋转和双旋转。单旋指一个节点和它的父亲转，双旋还会涉及到它的爷爷。

下面将介绍三种最为常用的平衡树。

## Treap

这时一种最基础的平衡树，但如果要实现名次树，它的速度甚至可以跟 RBT 叫板（当然是在 OI 的数据范围内）。   
接下来我们要用 Treap 实现名次树（因为所有操作都是围绕排名来进行的），[模板](https://www.luogu.com.cn/problem/P6136)。

### 原理

Treap 是最简单的一种**弱平衡** BST（是指不会为了把自己搞成除了最后一层不是满的二叉树而过多的变换自己的形态，AVL 便是严格平衡树），它是一个合成词，有 Treap = Tree + Heap，所以 Treap 又叫做树堆（怎么听上去那么搞笑{% emoji 滑稽 %}）。

Treap 有两种形式，无旋式有旋转式。无旋式能做到快速分裂与合并。

回到刚才所说的树堆中。Treap 除了节点的权值满足二叉搜索树性质以外，它的附加权值还满足堆性质（这里统一为大根堆性质）。这样就可以证明如果每个节点的附加权值全不同，那么 Treap 的形态是唯一的（但不一样也不影响我们干活）。

{% noteblock %}

普通的 BST 在随机情况下就是平衡的。Treap 通过人为制造平衡，随机赋予节点的附加权值。由于堆是一棵完全二叉树，所以 Treap 是期望平衡的，期望复杂度 $\mathcal{O}(\log n)$。

{% endnoteblock %}

理论上应该没有毒瘤来卡你{% emoji 滑稽 %}。

关键是旋转，怎么转？可以看这张图：

{% image https://z3.ax1x.com/2021/11/01/IPN10S.png, alt=zig(y) 代表 y 旋转前处于父亲节点，然后右旋至它的右子节点；zag(x) 是左旋，旋转到它的左子节点。圆形代表节点，三角代表子树。 %}

当 $y$ 右旋时，它会移到它的右子节点的位置，将他它左子节点 $x$ 移到它原来的位置，而由于 $x$ 的右子树 $B$ 不能再属于 $x$ 了，而根据 BST 的定义，$B<y$，所以 $B$ 变成了 $y$ 的左子树。另外，当旋转之后，$x$ 和 $y$ 的附加信息都需要重新计算，而且 $y$ 是 $x$ 的儿子，所以先维护 $y$ 再维护 $x$。

左旋类似，这里留给读者自己撕烤{% emoji 滑稽 %}。

### 实现

接下来我们要干一件大事：推翻我们以前 BST 的写法。

为什么？因为它实在是太容易出错了。如果你感觉很难接受，没关系，在文中我们还会介绍各种写法及其优劣。

首先是节点的定义，像这样：

```cpp
struct Node
{
    int ch[2]; // 左右儿子的编号
    int val, dat; // 键值和随机权值
    int size, cnt; // 子树大小和此节点的值的个数
}T[maxn];
```

可以发现变化是将左右儿子和到了一起。这是为了方便实现旋转。更加激进的写法是:

```cpp
struct Node
{
    Node *ch[2]; // 左右儿子
    int val, dat;
    int size, cnt;
};
```

看见什么了？指针？的确如此。这种方法的好处多多，第一会使你的代码更加流畅，不会出现数组套数组的窘况。二是会节省一些内存。但考虑到竞赛中不要使用指针的基本原则（虽然工程中这种写法是必备的），接下来的代码统一采用数组伪指针的形式。

{% noteblock %}

实际上还有另一种记录节点的方式，就是多记录每个节点的父亲。虽然这种做法在下文要介绍的伸展树中比较常见，但在 Treap 中，它的优点是旋转时可以更自然的对一个节点进行旋转，而不是在函数调用中写先定义好的父亲。如果你想学习这种写法，学完接下来介绍的 Splay 后你就可以给它迁移过来辣！

{% endnoteblock %}

首先是一些基本定义，如下：

```cpp
Node T[100005];
int tot;
inline void maintain(int p) { T[p].size = T[T[p].ch[0]].size + T[T[p].ch[1]].size + T[p].cnt; }
inline int newNode(int val)
{
    T[++tot].val = val;
    T[tot].dat = rand();
    T[tot].size = T[tot].cnt = 1;
    return tot;
}
```

唯一值得注意的是 `dat` 值的设置，随机一个数即可。

然后是旋转。旋转的原理已经了解过了，这里再次放出那张图，然后直接阅读下面的代码（同时进行手动模拟，对着每个节点转，就是改变这个节点的信息，不要对着图中的位置转）：

当 $y$ 右旋时，它会移到它的右子节点的位置，将他它左子节点 $x$ 移到它原来的位置，而由于 $x$ 的右子树 $B$ 不能再属于 $x$ 了，而根据 BST 的定义，$B<y$，所以 $B$ 变成了 $y$ 的左子树。另外，当旋转之后，$x$ 和 $y$ 的附加信息都需要重新计算，而且 $y$ 是 $x$ 的儿子，所以先维护 $y$ 再维护 $x$。

{% image https://pic.imgdb.cn/item/631897ba16f2c2beb136bf42.jpg::alt=zig(y) 代表 y 旋转前处于父亲节点，然后右旋至它的右子节点；zag(x) 是左旋，旋转到它的左子节点。圆形代表节点，三角代表子树。 %}

```cpp
inline void rotate(int &p, int d) // 绕着 p 点旋转，d = 0 左旋，d = 1 右旋，d^1 = 1-d
{
    int q = T[p].ch[d^1]; // 找到 p 的儿子，这个儿子将要旋转到根上
    T[p].ch[d^1] = T[q].ch[d]; // 接下自己的儿子要丢的儿子，因为自己要变成自己的儿子的儿子了
    T[q].ch[d] = p; // 原来儿子的儿子设为原来的爸爸
    p = q; // 原来的儿子正式登上爸爸的宝座，注意 p 是引用
    maintain(T[p].ch[d]), maintain(p); // 从下到上重新计算附加信息
}
```

注意某些节点旋转后，附加节点的信息就必须重新计算，而且要注意计算顺序。但某些文章你会看到类似这样的旋转：

```cpp
inline void zig(int &p) // 右旋
{
    int q = T[p].l;
    T[p].l = T[q].r, T[q].r = p, p = q;
    maintain(T[p].r), maintain(p);
}
inline void zag(int &p) // 左旋
{
    int q = T[p].r;
    T[p].r = T[q].l, T[q].l = p, p = q;
    maintain(T[p].l), maintain(p);
}
```

这也是对的，但是把一个函数就能完成的内容拆到两个函数里实属麻烦，而且到后来你会发现，`zig zag` 就是个屎坑，千万别跳，千万别跳，千万别跳，三体警告{% emoji 滑稽 %}。

然后是插入操作，只需要在不符合堆性质的时候进行旋转维护堆性质即可，代码如下：

```cpp
void insert(int &p, int val)
{
    if (p == 0) // 此节点不存在，直接新建
    {
        p = newNode(val);
        return;
    }
    if (val == T[p].val) ++T[p].cnt; // 存在这一键值，计数 +1
    else
    {
        // 这种方式能极大地简化代码
        int d = (val < T[p].val ? 0 : 1);
        insert(T[p].ch[d], val);
        if (T[T[p].ch[d]].dat > T[p].dat) rotate(p, d^1); // 不符合堆性质，儿子比父亲大，将儿子转到父亲的位置上
    }
    maintain(p);
}
```

常规的 `Rank kth GetPre GetNext` 操作和普通 BST 没有什么区别，留给读者自行实现。   

最后是 `Remove` 操作。为什么普通 BST 没有 `Remove` 操作呢？因为 `Remove` 操作意味着对二叉搜索树性质的复杂维护。但是 Treap 不一样，它支持旋转。我们只需要把要删除的节点转成叶子节点，然后直接删除即可。代码如下：

```cpp
void Remove(int &p, int val)
{
    if (p == 0) return;
    if (T[p].val == val)
    {
        if (T[p].cnt > 1) // 这个节点多于 1
        {
            --T[p].cnt;
            maintain(p);
            return;
        }
        if (T[p].ch[0] || T[p].ch[1])
        {
            int d = (T[p].ch[1] == 0 || T[T[p].ch[0]].dat > T[T[p].ch[1]].dat) ? 1 : 0; // 维护大根堆性质的前提下选择转到哪个方向
            rotate(p, d); // 将要删除的节点 p 转到儿子的位置上
            Remove(T[p].ch[d], val);
            maintain(p); // 此时节点的附加信息要重新计算
        }
        else p = 0; // 叶子节点直接删除
        return;
    }
    Remove(T[p].ch[val < T[p].val ? 0 : 1], val); // 递归删除
    maintain(p);
}
```

那么完整代码就不难写出了：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

const int INF = 2147483647;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

struct Node
{
    int ch[2];
    int val, dat;
    int size, cnt;
};

class Treap
{
    private:
        Node T[1100005];
        int tot;
        inline void maintain(int p) { T[p].size = T[T[p].ch[0]].size + T[T[p].ch[1]].size + T[p].cnt; }
        inline int newNode(int val)
        {
            T[++tot].val = val;
            T[tot].dat = rand();
            T[tot].size = T[tot].cnt = 1;
            return tot;
        }
        inline void rotate(int &p, int d)
        {
            int q = T[p].ch[d^1];
            T[p].ch[d^1] = T[q].ch[d], T[q].ch[d] = p, p = q;
            maintain(T[p].ch[d]), maintain(p);
        }
    public:
        int root;
        inline void build(void)
        {
            srand(time(0)); // 不写这个，小心毒瘤 Hack 你
            newNode(-INF), newNode(INF);
            T[1].ch[1] = 2, root = 1;
            maintain(root);
        }
        void insert(int &p, int val)
        {
            if (p == 0)
            {
                p = newNode(val);
                return;
            }
            if (val == T[p].val) ++T[p].cnt;
            else
            {
                int d = (val < T[p].val ? 0 : 1);
                insert(T[p].ch[d], val);
                if (T[T[p].ch[d]].dat > T[p].dat) rotate(p, d^1);
            }
            maintain(p);
        }
        int Rank(int p, int val)
        {
            if (p == 0) return 1;
            if (val == T[p].val) return T[T[p].ch[0]].size + 1;
            if (val < T[p].val) return Rank(T[p].ch[0], val);
            return Rank(T[p].ch[1], val) + T[T[p].ch[0]].size + T[p].cnt;
        }
        int kth(int p, int rnk)
        {
            if (p == 0) return INF;
            if (rnk <= T[T[p].ch[0]].size) return kth(T[p].ch[0], rnk);
            if (rnk <= T[T[p].ch[0]].size + T[p].cnt) return T[p].val;
            return kth(T[p].ch[1], rnk - T[T[p].ch[0]].size - T[p].cnt);
        }
        inline int GetPre(int val)
        {
            int ans = 1, p = root;
            while (p)
            {
                if (val == T[p].val)
                {
                    if (T[p].ch[0])
                    {
                        p = T[p].ch[0];
                        while (T[p].ch[1]) p = T[p].ch[1];
                        ans = p;
                    }
                    break;
                }
                if (T[p].val < val && T[p].val > T[ans].val) ans = p;
                p = T[p].ch[val < T[p].val ? 0 : 1];
            }
            return T[ans].val;
        }
        inline int GetNext(int val)
        {
            int ans = 2, p = root;
            while (p)
            {
                if (val == T[p].val)
                {
                    if (T[p].ch[1])
                    {
                        p = T[p].ch[1];
                        while (T[p].ch[0]) p = T[p].ch[0];
                        ans = p;
                    }
                    break;
                }
                if (T[p].val > val && T[p].val < T[ans].val) ans = p;
                p = T[p].ch[val < T[p].val ? 0 : 1];
            }
            return T[ans].val;
        }
        inline void Remove(int &p, int val)
        {
            if (p == 0) return;
            if (T[p].val == val)
            {
                if (T[p].cnt > 1)
                {
                    --T[p].cnt;
                    maintain(p);
                    return;
                }
                if (T[p].ch[0] || T[p].ch[1])
                {
                    int d = (T[p].ch[1] == 0 || T[T[p].ch[0]].dat > T[T[p].ch[1]].dat) ? 1 : 0;
                    rotate(p, d); Remove(T[p].ch[d], val);
                    maintain(p);
                }
                else p = 0;
                return;
            }
            Remove(T[p].ch[val < T[p].val ? 0 : 1], val);
            maintain(p);
        }
}T;

int main(void)
{
    int n = read(), m = read();
    T.build();
    while (n--) T.insert(T.root, read());
    int ans = 0, last = 0;
    while (m--)
    {
        int opt = read(), x = read();
        x ^= last;
        if (opt == 1) T.insert(T.root, x);
        else if (opt == 2) T.Remove(T.root, x);
        else if (opt == 3) last = T.Rank(T.root, x) - 1;
        else if (opt == 4) last = T.kth(T.root, x + 1);
        else if (opt == 5) last = T.GetPre(x);
        else last = T.GetNext(x);
        if (opt > 2) ans ^= last;
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

## FHQ-Treap

正常的 Treap 并不支持分裂与合并，但是由范浩强提出的无旋 Treap 可以快速地分裂与合并，实现 Splay 的大部分功能，而且效率比 Splay 高很多。

这种平衡树甚至被称为“最好写的平衡树”，情况远远没有 Splay 那么复杂。

### 实现

所有 Treap 的基本模板都适用，但是要注意，我们不再记录一个 $val$ 出现的次数 $cnt$，因为基于分裂与合并实现的平衡树没办法简单的实现找的一个节点的位置。但是不用担心，即使不记录 $cnt$，树中的节点键值可以重复，它依然可以正常工作。

```cpp
struct Node
{
	int ch[2];
	int size;
	int val, dat;
} T[1100005];
int tot = 0;
inline void maintain(int p) { T[p].size = T[T[p].ch[0]].size + T[T[p].ch[1]].size + 1; }
inline int newNode(int val)
{
	T[++tot].val = val;
	T[tot].dat = rand(), T[tot].size = 1;
	return tot;
}
int main(void) { srand(time(0)); }
```

### 分裂与合并

由于 FHQ 并不支持旋转，所以一切维护平衡的手段都依赖于分裂与合并。

什么是平衡树的分裂与合并呢？简单地说，之前的平衡树之能有一个根，但是现在可以有多个。由于 BST 的递归性质，所以可以很方便地合并两个 BST。

#### 分裂

**按 val 分裂**。按照键值 `val` 将 Treap 分裂成两棵子树，其中一棵树 $x$ 的值全部小于等于 $val$，剩下的是另外一棵 $y$ 全部大于 $val$ 的。

函数定义为 `split(p, key, x, y)`，代表遍历到 $p$，根据 $key$ 作为键值分裂成两棵子树 $x,y$。具体怎么做呢？

如果 $val[p]\le key$，那么应该被放到 $x$ 上，否则被放到 $y$ 上。而放在子树中的具体哪一个位置？很显然需要递归进行。

```cpp
void split(int p, int key, int &x, int &y)
{
	if (!p) return x = y = 0, void(); // 空了，分裂之后都是 0
	if (T[p].val <= key) // p 应该被分裂到 x 上
	{
		x = p; // 让 p 作为 x 的根，现在左子树全归 x 了
		split(T[p].ch[1], key, T[p].ch[1], y); // 开始分裂右子树，右子树中有 <= key 的应该给 x 的右子树
	}
	else 
	{
		y = p;
    	split(T[p].ch[0], key, x, T[p].ch[0]);
    }
    maintain(p); // 当前节点的信息需要重新计算
}
```

---

**按 size 分裂**。按照子树的大小，前 $siz$ 给 $x$，剩余的给 $y$，也很容易实现。

```cpp
void split(int p, int siz, int &x, int &y)
{
	if (!p) return x = y = 0, void();
	if (T[T[p].ch[0]].size + 1 <= siz) // p 应该被分裂到 x 上
	{
		x = p; // 让 p 作为 x 的根，现在左子树全归 x 了
		split(T[p].ch[1], siz - T[T[p].ch[0]].size - 1, T[p].ch[1], y); // 开始分裂右子树，右子树中应有 siz - p->l->size - 1 归 x
	}
	else 
	{
		y = p;
    	split(T[p].ch[0], siz, x, T[p].ch[0]);
    }
    maintain(p);
}
```

#### 合并

合并的时候显然要求 $x$ 中的每一个节点都小于 $y$ 中的每一个节点，然后根据 Treap 的堆性质来判断是将 $x$ 合并到 $y$ 还是将 $y$ 合并到 $x$。

```cpp
int merge(int x, int y) // 要求 x 中的每一个节点都小于 y 中的每一个节点
{
    if (x == 0 || y == 0) return x + y; // 有一棵是空的，那么返回另一棵
    if (T[x].dat > T[y].dat) // 需要维护大根堆性质，将 y 合并到 x
    {
        T[x].ch[1] = merge(T[x].ch[1], y);
        maintain(x);
        return x;
    }
    else
    {
        T[y].ch[0] = merge(x, T[y].ch[0]);
        maintain(y);
        return y;
    }
}
```

### 实现名次树

分裂与合并是 FHQ-Treap 的核心操作，剩下的所有操作都基于分裂与合并。

#### 插入

将 $key$ 分裂出来，然后合并三次即可。

```cpp
void insert(int key)
{
    int x, y;
    split(root, key - 1, x, y); // 到时候 val 的左子节点是 x，右子节点是 y
    root = merge(merge(x, newNode(key)), y);
}
```

#### 删除

分裂两次将 $key$ 分裂出来，然后进行删除。

```cpp
void Remove(int key)
{
    // 将 root 按照 key 分裂成 x, z
    // 将 x 按照 key - 1 分裂成 x, y
    // 这时 x < key, y = key, z > key
    // y 中的只需要删一个
    // 如果删去所有的，那么合并时直接 root = merge(x, z)
    int x, y, z;
    split(root, key, x, z);
    split(x, key - 1, x, y);
    if (y) y = merge(T[y].ch[0], T[y].ch[1]); 
    root = merge(merge(x, y), z);
}
```

#### 求排名

将 $key$ 分裂出来，然后就是 $x$ 树的大小 +1 了。

```cpp
int Rank(int key)
{
    int x, y, ans;
    split(root, key - 1, x, y);
    ans = T[x].size + 1; // 排名等于 < key 的个数 +1
    root = merge(x, y); // 再把它合并回去，还原现场
    return ans;
}
```

#### 求 k 小

有两种方式，但是比较推荐类似于之前的方式：

另一种是利用基于大小的分裂，但是这样代码会变多。

```cpp
int kth(int rnk) // 递归查找
{
    int p = root;
    while (p)
    {
        if (T[T[p].ch[0]].size + 1 == rnk) break;
        else if (T[T[p].ch[0]].size + 1 > rnk) p = T[p].ch[0];
        else
        {
            rnk -= T[T[p].ch[0]].size + 1;
            p = T[p].ch[1];
        }
    }
    return T[p].val;
}
```

#### 前驱后继

对于前驱，将小于的分裂出来，然后再这棵树上尽可能往右走。后继大致同理。

```cpp
int GetPre(int key)
{
    int x, y, p, ans;
    split(root, key - 1, x, y); // 找到小于等于的
    p = x;
    while (T[p].ch[1]) p = T[p].ch[1]; // 尽可能往右走
    ans = T[p].val; // 在还原现场之前先记录答案
    root = merge(x, y);
    return ans;
}

int GetNext(int key)
{
    int x, y, p, ans;
    split(root, key, x, y);
    p = y;
    while (T[p].ch[0]) p = T[p].ch[0];
    ans = T[p].val;
    root = merge(x, y);
    return ans;
}
```

#### 性能

FHQ-Treap 通过平衡树模板的代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Node
{
    int ch[2], size;
	int val, dat;
} T[1100005];
int tot = 0, root;
inline void maintain(int p) { T[p].size = T[T[p].ch[0]].size + T[T[p].ch[1]].size + 1; }
inline int newNode(int val)
{
	T[++tot].val = val;
	T[tot].dat = rand(), T[tot].size = 1;
	return tot;
}

void split(int p, int key, int &x, int &y)
{
	if (!p) return x = y = 0, void(); // 空了，分裂之后都是 0
	if (T[p].val <= key) // p 应该被分裂到 x 上
	{
		x = p; // 让 p 作为 x 的根，现在左子树全归 x 了
		split(T[p].ch[1], key, T[p].ch[1], y); // 开始分裂右子树，右子树中有 <= key 的应该给 x 的右子树
	}
	else 
	{
		y = p;
    	split(T[p].ch[0], key, x, T[p].ch[0]);
    }
    maintain(p); // 当前节点地信息需要重新计算
}

int merge(int x, int y) // 要求 x 中的每一个节点都小于 y 中的每一个节点
{
    if (x == 0 || y == 0) return x + y; // 有一棵是空的，那么返回另一棵
    if (T[x].dat > T[y].dat) // 需要维护大根堆性质，将 y 合并到 x
    {
        T[x].ch[1] = merge(T[x].ch[1], y);
        maintain(x);
        return x;
    }
    else
    {
        T[y].ch[0] = merge(x, T[y].ch[0]);
        maintain(y);
        return y;
    }
}

void insert(int key)
{
    int x, y;
    split(root, key - 1, x, y); // 到时候 val 的左子节点是 x，右子节点是 y
    root = merge(merge(x, newNode(key)), y);
}

void Remove(int key)
{
    // 将 root 按照 key 分裂成 x, z
    // 将 x 按照 key - 1 分裂成 x, y
    // 这时 x < key, y = key, z > key
    // y 中的只需要删一个
    // 如果删去所有的，那么合并时直接 root = merge(x, z)
    int x, y, z;
    split(root, key, x, z);
    split(x, key - 1, x, y);
    if (y) y = merge(T[y].ch[0], T[y].ch[1]); 
    root = merge(merge(x, y), z);
}

int Rank(int key)
{
    int x, y, ans;
    split(root, key - 1, x, y);
    ans = T[x].size + 1; // 排名等于 < key 的个数 +1
    root = merge(x, y); // 再把它合并回去，还原现场
    return ans;
}

int kth(int rnk) // 递归查找
{
    int p = root;
    while (p)
    {
        if (T[T[p].ch[0]].size + 1 == rnk) break;
        else if (T[T[p].ch[0]].size + 1 > rnk) p = T[p].ch[0];
        else
        {
            rnk -= T[T[p].ch[0]].size + 1;
            p = T[p].ch[1];
        }
    }
    return T[p].val;
}

int GetPre(int key)
{
    int x, y, p, ans;
    split(root, key - 1, x, y); // 找到小于等于的
    p = x;
    while (T[p].ch[1]) p = T[p].ch[1]; // 尽可能往右走
    ans = T[p].val; // 在还原现场之前先记录答案
    root = merge(x, y);
    return ans;
}

int GetNext(int key)
{
    int x, y, p, ans;
    split(root, key, x, y);
    p = y;
    while (T[p].ch[0]) p = T[p].ch[0];
    ans = T[p].val;
    root = merge(x, y);
    return ans;
}

int read(void)
{
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int main(void)
{
    srand(time(0));
    int n = read(), m = read();
    while (n--) insert(read());
    int ans = 0, last = 0;
    while (m--)
    {
        int opt = read(), x = read();
        x ^= last;
        if (opt == 1) insert(x);
        else if (opt == 2) Remove(x);
        else if (opt == 3) last = Rank(x);
        else if (opt == 4) last = kth(x);
        else if (opt == 5) last = GetPre(x);
        else last = GetNext(x);
        if (opt > 2) ans ^= last;
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

测试性能之后发现，Treap 用时 9.78s，FHQ-Treap 用时 12.60s，FHQ 还是会慢一些，不过速度足够了。

正如我们所说，Treap 是 OI 范围内能用到的最快的平衡树，FHQ 结合了 Treap 的优点并且支持 Splay 的分裂与合并，何乐而不为呢？

### 分裂与合并的序列

FHQ 可以用来实现 Splay 的快速分裂合并功能，[模板](https://www.luogu.com.cn/problem/P3391)。

我们先看一下如何实现分裂与合并的序列：我们只需要把区间的下标依次插入 Treap 中，**也就是我们不再利用二叉搜索树的性质，不再是根据权值而建立平衡树，只是利用了它们能够分裂与合并的特性**。

区间翻转的时候，我们按照大小分裂为 $[1,l-1],[l,r],[r+1,n]$，然后给中间的树打上一个 $rev$ 标记，代表是否将左右儿子翻转（由于中序遍历的性质，将左右儿子反转后的序列便是原序列），然后操作的时候要进行 `pushdown`。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Node
{
    int ch[2], size;
	int val, dat;
	bool rev;
} T[100005];
int tot = 0, root;
inline void maintain(int p) { T[p].size = T[T[p].ch[0]].size + T[T[p].ch[1]].size + 1; }
inline int newNode(int val)
{
	T[++tot].val = val;
	T[tot].dat = rand(), T[tot].size = 1;
	return tot;
}

inline void pushdown(int p)
{
	if (!T[p].rev) return;
	swap(T[p].ch[0], T[p].ch[1]);
	T[T[p].ch[0]].rev ^= 1, T[T[p].ch[1]].rev ^= 1;
	T[p].rev = 0;
}

void split(int p, int siz, int &x, int &y)
{
	if (!p) return x = y = 0, void(); // 空了，分裂之后都是 0
	pushdown(p);
	if (T[T[p].ch[0]].size + 1 <= siz) // p 应该被分裂到 x 上
	{
		x = p; // 让 p 作为 x 的根，现在左子树全归 x 了
		split(T[p].ch[1], siz - T[T[p].ch[0]].size - 1, T[p].ch[1], y); // 开始分裂右子树，右子树中有 <= key 的应该给 x 的右子树
	}
	else 
	{
		y = p;
    	split(T[p].ch[0], siz, x, T[p].ch[0]);
    }
    maintain(p); // 当前节点地信息需要重新计算
}

int merge(int x, int y) // 要求 x 中的每一个节点都小于 y 中的每一个节点
{
    if (x == 0 || y == 0) return x + y; // 有一棵是空的，那么返回另一棵
    if (T[x].dat > T[y].dat) // 需要维护大根堆性质，将 y 合并到 x
    {
		pushdown(x);
        T[x].ch[1] = merge(T[x].ch[1], y);
        maintain(x);
        return x;
    }
    else
    {
		pushdown(y);
        T[y].ch[0] = merge(x, T[y].ch[0]);
        maintain(y);
        return y;
    }
}

void Reverse(int l, int r)
{
	int x, y, z;
	split(root, l - 1, x, y);
	split(y, r - l + 1, y, z);
	T[y].rev ^= 1;
	root = merge(merge(x, y), z);
}

void print(int x)
{
	if (!x) return;
	pushdown(x);
	print(T[x].ch[0]);
	printf("%d ", T[x].val);
	print(T[x].ch[1]);
}

int main(void)
{
    srand(time(0));
    int n, m;
	scanf("%d%d", &n, &m);
	for (int i = 1, x; i <= n; ++i)
		root = merge(root, newNode(i));
	while (m--)
	{
		int l, r;
		scanf("%d%d", &l, &r);
		Reverse(l, r);
	}
	print(root);
    return 0;
}
```
{% endfolding %}

## Splay

Splay，就是大名鼎鼎的“伸展树（因为伸展是它最经典的操作）”，也叫“自适应查找树”。1985 年由 Daniel Sleator 和 Robert Endre Tarjan（对，就是这个著名的 Tarjan）发明。

Splay 的平衡方式是通过旋转来伸展（有时候叫做“提根”），即把一个叶子节点通过旋转提到根节点。

值得一提的是，Splay 具有“自适应性”，就是它会根据你的操作调整自身结构，使得接下来的查询变得越来越快（像不像并查集）。但即使如此，这货还是很慢{% emoji 滑稽 %}。

伸展树有什么用呢？虽然它可以用来实现名次树，但在竞赛中要实现名次树的话，还是乖乖用 Treap 吧。Splay 的伸展操作最大的用处是进行快速地分裂与合并（嗯，就是 fhq-treap 干的事，但实践中还是伸展树用的更多）。

Splay 在旋转时会涉及到节点的爷爷，所以它是双旋平衡树。

### 伸展操作

Splay 的节点怎么定义呢？一般来说有两种方式。第一种是记录节点的父亲的，因为（哪来那么多因为，这不是上文说的定义吗）。第二种是不记录父亲的。但是第一种相对来讲逻辑更为清晰（尽管旋转操作中的编码较为复杂），第二种在某些情况下会用到。因为笔者是傻瓜，不会第二种，所以这里只介绍第一种情况。

但是这之前，我们还需要搞明白 Splay 最关键的操作：伸展操作（`splay` 操作）如何进行。

`splay` 操作要分三种情况考虑。

1. $x$ 的父节点是根节点，这时候进行一次单旋转即可，就完成了 `splay` 操作。
2. $x$，它的父节点和它的爷爷“三点共线”，这时进行两次方向相同的旋转操作即可。而且先转 $x$ 的父节点再转 $x$；
3. 三点不共线。这是需要将 $x$ 进行不同方向的两次旋转。

通过以上方式我们就能完成 `splay` 操作啦！

{% noteblock guide green %}

这里可以自行画图感受伸展操作的过程，使得更容易理解接下来的内容。

{% endnoteblock %}

Splay 实际上有两种写法，第一种方法的节点像这样定义：

```cpp
struct Node
{
    int ch[2], fa;
    int val;
    // otherthings, such as size and cnt
};
```

然后我们要实现一些基本的模板，如下：

```cpp
class Splay
{
    private:
        Node T[100005];
        int tot;
        /*
        inline void maintain(int p)
        { 
            T[p].size = T[T[p].ch[0]].size + T[T[p].ch[1]].size + T[p].cnt;
        }
        */
};
```

怎么实现旋转呢？由于我们记录了父亲节点，所以可以换一种方式定义旋转：定义 `rotate(x)` 为将 $x$ 的**父亲节点上旋**到 $x$ 的爷爷。我们可以写一个函数来判断它是父亲节点的左儿子还是右儿子。

```cpp
inline int get(int x)
{
    return x == T[T[x].fa].ch[1];
}
```

旋转操作要注意：因为我们记录了父亲节点，意味着在旋转时需要对每个节点的父亲进行维护。但同时我们也不需要考虑旋转方向了。

```cpp
inline void rotate(int x)
{
    // y: father, z: grandfather
    int y = T[x].fa, z = T[y].fa, d = get(x); // d 是左右儿子，不是方向
    T[y].ch[d] = T[x].ch[d^1];
    if (T[x].ch[d^1]) T[T[x].ch[d^1]].fa = y;
    T[x].ch[d^1] = y, T[y].fa = x; // 原来的儿子把原来的爸爸当成儿子（上旋）
    // 接下来把本来是爸爸的 y 的爸爸给 x
    T[x].fa = z;
    if (z) T[z].ch[get(y)] = x; // 本来 z 的儿子是 y，现在却变成了 x。如果 z 都等于 0 了还改就没什么意思了，是不？（况且我们还要根据是否等于 0 来判断父亲存不存在）
    maintain(y), maintain(x); // 从下到上重新计算
}
```

感觉很绕？还是建议自行模拟一下。

有了旋转操作，就不难实现伸展操作了。伸展的原理之前已经讲过，这里直接给出代码：

```cpp
inline void splay(int x) // 把 x 提根
{
    // 定义 f 为 x 的爸爸
    // 我们需要保证 f 存在（它是 0 就结束了）才能继续循环
    // 常规来讲，我们需要上旋 x
    for (int f = T[x].fa; f = T[x].fa; rotate(x))
        if (T[f].fa) // 如果 T[f].fa 存在，就意味着是情况 2 或 3，需要进行第一次旋转
        {
            rotate(get(x) == get(f) ? f : x); 
            // 当 get(x) == get(f) 时，意为着三点共线，转 f
            // 否则三点不共线，转两次 x（还有一次在循环更新处）
            // 这里的循环顺序很巧妙，保证处理完情况 2 和 3 的特别旋转放送后，立马会进行一次 rotate(x)（for 循环的更新）
        }
    root = x; // 根变成了 x
}
```

Splay 的第二种写法不记录节点的父亲，这时就变成了递归版的 Splay。~~由于笔者很弱不会~~，所以想学习这种写法请参考《算法竞赛入门经典·训练指南》，或者网上的其它资料。实际上本文介绍的这种写法逻辑更为清晰，这里做无耻推荐（

某些情况下我们会使用指针实现 Splay。实际上笔者更推荐指针，但你的代码必须能跟现有的模块集成。现在大多数选手的 Splay 都是用数组伪指针的形式写的，如果不采用这种形式可能会在今后的学习中造成困扰，所以这里推荐大家使用伪指针。

### 用 Splay 实现名次树

名次树最终要的就是要保证平衡。但是 Splay 怎么保证平衡？好像很难搞。想想我们 Treap 是怎么搞的吧！用随机来创造平衡。那我们就随机伸展来保证平衡{% emoji 滑稽 %}。

~~此处应该有 BGM。~~

不要笑，真是这么搞。听上去比 Treap 更不靠谱？~~我也是这么认为的~~。这里用 Splay 实现名次树仅仅是作为一个练习，考场上用 Treap 就好。

所以结论是：Splay 实现的名次树照样是弱平衡的随机平衡树（而且比 Treap 还慢），不过 Treap 在竞赛中已经足够用了，不建议作死去学 RBT。

注意到一个问题，由于 Splay 是记录父亲的，所以这时候如果我们还使用递归代码就会很麻烦（需要传父亲或者到处都是 `get` 函数），尤其是插入操作，进行伸展操作前要对性质进行维护，而递归的维护依赖于递归性质，所以这时不如使用迭代。

由于实现删除操作需要运用伸展树的分裂与合并，故这回我们写最初的[弱化版](https://www.luogu.com.cn/problem/P5076)。

唯一不同的只有插入操作，剩下的代码都可以照搬。插入要这样进行：动态记录当前节点和当前节点的父亲，如果找到了与要插入的值相同的节点，那么 `cnt++`，同时伸展这个点；否则递归地往下找，找到了空节点就创建新节点，然后伸展这个点，代码如下：

```cpp
inline void insert(int val)
{
    int cur = root, fa = 0;
    while (1)
    {
        if (T[cur].val == val) // 找到了值相等的节点
        {
            ++T[cur].cnt;
            maintain(cur);
            maintain(fa);
            // 以上为进行维护
            splay(cur); // 伸展
            return;
        }
        fa = cur;
        cur = T[cur].ch[val > T[cur].val]; // 往下走
        if (cur == 0)
        {
            newNode(val); // 新建节点
            // 重新计算父子关系和子树大小
            T[tot].fa = fa;
            T[fa].ch[val > T[fa].val] = tot;
            maintain(fa);
            splay(tot); // 伸展
            return;
        }
    }
}
```

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

const int INF = 0x7fffffff;

struct Node
{
    int ch[2], fa;
    int val;
    int size, cnt;
};

class Splay
{
    public:
        int root;
    private:
        Node T[100005];
        int tot;
        inline void newNode(int val)
        {
            T[++tot].val = val;
            T[tot].size = T[tot].cnt = 1;
        }
        inline void maintain(int p)
        {
            T[p].size = T[T[p].ch[0]].size + T[T[p].ch[1]].size + T[p].cnt;
        }
        inline int get(int x)
        {
            return x == T[T[x].fa].ch[1];
        }
        inline void rotate(int x)
        {
            int y = T[x].fa, z = T[y].fa, d = get(x);
            T[y].ch[d] = T[x].ch[d^1];
            if (T[x].ch[d^1]) T[T[x].ch[d^1]].fa = y;
            T[x].ch[d^1] = y, T[y].fa = x;
            T[x].fa = z;
            if (z) T[z].ch[y == T[z].ch[1]] = x;
            maintain(y);
            maintain(x);
        }
        inline void splay(int x)
        {
            for (int f = T[x].fa; f = T[x].fa; rotate(x))
                if (T[f].fa) rotate(get(x) == get(f) ? f : x);
            root = x;
        }
    public:
        inline void build(void)
        {
            newNode(-INF), newNode(INF);
            T[1].ch[1] = 2, T[2].fa = 1;
            maintain(root = 1);
        }
        inline void insert(int val)
        {
            int cur = root, fa = 0;
            while (1)
            {
                if (T[cur].val == val)
                {
                    ++T[cur].cnt;
                    maintain(cur);
                    maintain(fa);
                    splay(cur);
                    return;
                }
                fa = cur;
                cur = T[cur].ch[val > T[cur].val];
                if (cur == 0)
                {
                    newNode(val);
                    T[tot].fa = fa;
                    T[fa].ch[val > T[fa].val] = tot;
                    maintain(fa);
                    splay(tot);
                    return;
                }
            }
        }
        int Rank(int p, int val)
        {
            if (p == 0) return 1;
            if (val == T[p].val) return T[T[p].ch[0]].size + 1;
            if (val < T[p].val) return Rank(T[p].ch[0], val);
            return Rank(T[p].ch[1], val) + T[T[p].ch[0]].size + T[p].cnt;
        }
        int kth(int p, int rnk)
        {
            if (p == 0) return INF;
            if (rnk <= T[T[p].ch[0]].size) return kth(T[p].ch[0], rnk);
            if (rnk <= T[T[p].ch[0]].size + T[p].cnt) return T[p].val;
            return kth(T[p].ch[1], rnk - T[T[p].ch[0]].size - T[p].cnt);
        }
        inline int GetPre(int val)
        {
            int ans = 1, p = root;
            while (p)
            {
                if (val == T[p].val)
                {
                    if (T[p].ch[0])
                    {
                        p = T[p].ch[0];
                        while (T[p].ch[1]) p = T[p].ch[1];
                        ans = p;
                    }
                    break;
                }
                if (T[p].val < val && T[p].val > T[ans].val) ans = p;
                p = T[p].ch[val < T[p].val ? 0 : 1];
            }
            return T[ans].val;
        }
        inline int GetNext(int val)
        {
            int ans = 2, p = root;
            while (p)
            {
                if (val == T[p].val)
                {
                    if (T[p].ch[1])
                    {
                        p = T[p].ch[1];
                        while (T[p].ch[0]) p = T[p].ch[0];
                        ans = p;
                    }
                    break;
                }
                if (T[p].val > val && T[p].val < T[ans].val) ans = p;
                p = T[p].ch[val < T[p].val ? 0 : 1];
            }
            return T[ans].val;
        }
}T;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int main(void)
{
    int q = read();
    T.build();
    while (q--)
    {
        int op = read(), x = read();
        if (op == 1) printf("%d\n", T.Rank(T.root, x) - 1);
        else if (op == 2) printf("%d\n", T.kth(T.root, x + 1));
        else if (op == 3) printf("%d\n", T.GetPre(x));
        else if (op == 4) printf("%d\n", T.GetNext(x));
        else T.insert(x);
    }
    return 0;
}
```

{% endfolding %}

### 分裂与合并

由于 Splay 支持伸展操作，因此它可以很方便的进行分裂与合并，进而实现可以分裂与合并的序列。[模板](https://www.luogu.com.cn/problem/P3391)。

还有非常多的应用，请见 Problemset。

## Problemset

直接的平衡树应用很少，大多情况下的直接应用都是利用 Splay 的分裂与合并来进行复杂信息维护，但是更复杂的信息就需要使用树套树了，也就是平衡树的最大应用：作为数据结构进行嵌套。这里我们还是看一些经典题目。内容不多，推荐都完成。

### 直接平衡树

### 可分裂与合并的序列

这是平衡树单独出现时最经典的应用了。这里笔者有的代码使用 FHQ，有的用的 Splay，只是为了练习不同的平衡树。