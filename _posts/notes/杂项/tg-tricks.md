---
title: 提高简单技巧
headimg: 'https://s1.ax1x.com/2022/08/04/vmpXTA.png'
categories:
    - 算法竞赛
    - 学习笔记
plugins:
    - katex
tag:
    - 双指针
    - 前缀和
    - 差分
    - 单调栈
    - 单调队列
cover: false
abbrlink: 37f7d707
status: working
group: notes
hide: true
date: 2022-07-24 11:21:36
---

> 提高组的常见简单技巧包括前缀和与差分，2-pointers 等，本文简单介绍了它们。

<!--more-->

{% folding blue::更新日志 %}

{% timeline %}

{% timenode 2022/09/13 %}

合并了一些基础算法。

{% endtimenode %}

{% endtimeline %}

{% endfolding %}

## 双指针

双指针里的指针并不是指 C 语言中的指针，而是一个抽象的指针。我一个指针“指”在了序列中的某个地方，一方面表示目前枚举到了这里，另一方面表示正在关注这个数，两个指针也经常有区间左右端点的意义。一般是枚举一个指针，然后另一个指针随着这个指针的移动而移动。   

简单地来说，2-pointer 其实就是用两个指针扫描数组。

大概分为两种，两个指针的行进方向是相同的，或是相向的。

### 普通双指针

因为很常规，所以它没有名字。

#### [Luogu P1102] A-B 数对

[Portal](https://www.luogu.com.cn/problem/P1102).
 
$A-B=C$ 可以化为 $A-C=B$，也就是说可以找到序列中的一个数减掉 $C$ 等于序列中的另一个数。显然对于一个 $A$, $B$ 是唯一的。

当然可以排序后使用二分来找这个 $B$ 的数量，不过今天我们介绍另一种做法。我们还是先排序，这些 $B$ 一定是连续的。从左到右扫描序列，记当前扫描到 $a_i$，当继续向右扫，$a_i$ 会增加，所对应的 $B$ 的下标不会减小。利用这个性质扫描一次即可，时间复杂度为 $O(n)$（需要使用基数排序）。

维护两个下标 $l,r$，使得使得任意时刻 $l$ 是第一个满足 $A-C=B$ 的位置，$r$ 是第一个不满足 $A-C=B$ 的位置，那么个数就是 $r-l$。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
using i64 = long long;

int n, c;
int a[200005];

int main(void)
{
    scanf("%d%d", &n, &c);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    sort(a + 1, a + n + 1);
    int l = 1, r = 1;
    i64 ans = 0;
    for (int i = 1; i <= n; ++i)
    {
        while (a[l] < a[i] - c && l <= n) ++l; // 找到第一个 B 的下标
        while (a[r] <= a[i] - c && r <= n) ++r; // 找到第一个不是 B 的下标
        ans += r - l; // l <= r 恒成立，直接加即可
    }
    printf("%lld\n", ans);
    return 0;
}
```

#### [Luogu P1115] 最大子段和

[Portal](https://www.luogu.com.cn/problem/P1115).

先计算前缀和。一个子段和等于 $S[r]-S[l-1]$，当 $S[r]$ 一定时，找到最小的 $S[l-1]$ 就可以了。可以简单地记录 $S[l-1]$，代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int read(void)
{
    int x = 0, c = getchar(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar();
    return x * f;
}

int sum[200005];

int main(void)
{
    int n = read(), ans = -2000000000, minn = 0;
    for (int i = 1; i <= n; ++i) sum[i] = read() + sum[i-1];
    for (int R = 1; R <= n; ++R)
    {
        ans = max(ans, sum[R] - minn);
        minn = min(minn, sum[R]);
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [USACO16OPEN] Diamond Collector S

[Portal](https://www.luogu.com.cn/problem/P3143).

根据直觉肯定要先排序。由于总共有两个架子，然后容易发现，如果两个钻石放到同一个盒子，那么这两个钻石中间的也可以放到同一个盒子。问题实际上是要找出两个不相交的区间，使得最大值和最小值之差都小于 $K$，且区间长度之和最大。

这时就有两种思路，一个是前后各做两边双指针，然后合并。

{% folding cyan::扫两遍 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, k;
int a[50005];
int f[50005], g[50005];

int main(void)
{
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    sort(a + 1, a + n + 1);

    // 此时 f[i] 代表从 i 开始（必须含 i）向右可以选的最长区间
    for (int L = 1, R = 1; L <= n; ++L)
    {
        while (R < n && a[R + 1] <= k + a[L])
            ++R; // R 尽量往左走
        f[L] = R - L + 1; // 从 L 开始的最大区间长度
    }
    // 接下来让 f[i] 代表在 [i,n] 中选一段区间的最大长度（可以不含 i）
    for (int i = n - 1; i >= 1; --i)
        f[i] = max(f[i], f[i + 1]);

    // 此时 g[i] 代表从 i 开始（含 i）向左可以选的最长区间
    for (int L = n, R = n; R >= 1; --R)
    {
        while (L > 1 && a[R] - a[L - 1] <= k)
            --L; // L 尽量往左走
        g[R] = R - L + 1; // 从 R 开始的最大区间长度
    }
    // 接下来让 g[i] 代表在 [1,i] 中选一段区间的最大长度（可以不含 i）
    for (int i = 2; i <= n; ++i)
        g[i] = max(g[i], g[i - 1]);

    int ans = 0;
    for (int i = 1; i < n; ++i)
    // 合并答案，枚举分界点 i，分别求 [1,i], [i+1,n] 两端区间中选一段最长区间的长度，然后相加
        ans = max(ans, f[i + 1] + g[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

另一种是只扫一遍，同时统计：

{% folding cyan::扫一遍 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n, k, ans = 0, res = 0; // ans 记录最大答案，res 记录 [0,i-1] 中的最长区间
int a[50005], c[50005]; // c[i] 记录从 i 开始向左走，最长的区间，就是满足条件的 [x,i] 的最大长度

int main(void)
{
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i);
    sort(a + 1, a + n + 1);
    c[1] = 1;
    for (int i = 1, R = 1; i <= n; ++i) // [i,R] 放在第二个架子，能放多少放多少
    // 在 [0,i - 1] 中挑一段区间放在第一个架子
    // 在 i 右边选一段区间放没有意义，因为迟早能扫到
    // 也就是说，这个算法相当于枚举 i，找到最大的 R，并计算在 [0,i - 1] 中挑一段区间的最大长度
    {
        while (R < n && a[R + 1] <= a[i] + k)
        {
            ++R; // R 越大越好

            /*
            c[R] 表示从 R 开始向左走的最大长度
            R 能跑到这里，是在当前这个 i 之前前所未有的
            也就是当前这个 i 是第一个能使 R 跑到这里的
            那么这个 i 一定是使得区间长度最长的最小 i
            */
            c[R] = R - i + 1;
        }
        res = max(res, c[i - 1]); // 更新 res，res 就是 max(c[0],c[1]...c[i-1])
        ans = max(ans, R - i + 1 + res); // R - i + 1 为 [i,R] 的长度
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 尺取法

~~我也不知道这是个什么东西，但是给的题单是这样的，笔记我就这么写了。~~   
~~感觉就是滑动窗口，但可能还不太一样。~~{% emoji aru 84 %}

实际上指的就是两个移动方向相同的双指针，然后统计它们之间的信息。

#### [Luogu P1638] 逛画展

[Portal](https://www.luogu.com.cn/problem/P1638).

使用滑动窗口（同向双指针），并开一个桶记录看的各个画家的画的数量，在这个数量到 $m$ 钱让 `R++`，到了之后让 `L++` 直到不满足条件。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m, cnt = 0;
int ans = 1000005, ansL = 1, ansR = 1000000;
int a[1000005], b[2005];

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    int L = 1, R = 1;
    while (R <= n)
    {
        if (b[a[R]] == 0) ++cnt;
        ++b[a[R]];
        while (L < R && cnt == m && b[a[L]] > 1) --b[a[L]], ++L;
        if (cnt == m && R - L < ansR - ansL)
        {
            ansL = L;
            ansR = R;
        }
        ++R;
    }
    printf("%d %d\n", ansL, ansR);
    return 0;
}
```
{% endfolding %}

#### [UVa 11572] Unique Snowflakes

[Portal](https://www.luogu.com.cn/problem/UVA11572).

直接使用滑动窗口，然后利用一个 `set` 判重即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <unordered_set>

using namespace std;

int n;
int a[1000005];
unordered_set <int> s;

void solve(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    s.clear();
    int L = 1, R = 1, ans = 0;
    while (R <= n)
    {
        while (R <= n && s.find(a[R]) == s.end()) s.insert(a[R++]);
        ans = max(ans, R - L);
        s.erase(a[L++]);
    }
    printf("%d\n", ans);
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

## 排序

~~是普及组内容，但是笔者不会。~~

排序算法（英语：Sorting algorithm）是一种将一组特定的数据按某种顺序进行排列的算法。排序算法多种多样，性质也大多不同。

**稳定性**。稳定性是指相等的元素经过排序之后相对顺序是否发生了改变。拥有稳定性这一特性的算法会让原本有相等键值的纪录维持相对次序，即如果一个排序算法是稳定的，当有两个相等键值的纪录 $A$ 和 $B$，且在原本的列表中 $A$ 出现在 $B$ 之前，在排序过的列表中 $A$ 也将会是在 $B$ 之前。

关于稳定性的意义：

- 如果只是简单的进行数字的排序，那么稳定性将毫无意义。
- 如果排序的内容仅仅是一个复杂对象的某一个属性（比如排序的是结构体，其中一个数字作为排序依据），那么稳定性依旧将毫无意义。
- 如果要排序的内容是一个复杂对象的多个属性（比如排序的是结构体，其中几个数字参与运算作为排序依据），**但是其原本的初始顺序毫无意义**，那么稳定性依旧将毫无意义。
- 除非要排序的内容是一个复杂对象的多个属性，**且其原本的初始顺序存在意义**，那么我们需要在进行排序的基础上保持原有顺序的意义，才需要使用到稳定性的算法。例如要排序的内容是一组原本按照价格高低排序的对象，如今需要按照销量高低排序，使用稳定性算法，可以使得相同销量的对象依旧保持着价格高低的排序展现，只有销量不同的才会重新排序。

也就是说，除非要排序的内容具有多个属性，而且在相等时要保证原有的顺序，才需要使用稳定性排序。

**排序的方法**。排序要么是基于比较，要么是基于分类。下文会进行介绍。

**复杂度**。包括时间复杂度和空间复杂度。有最坏复杂度，平均复杂度和最优复杂度。一般来讲算法竞赛中只看最坏。

### 冒泡排序

工作原理是每次检查相邻两个元素，如果前面的元素与后面的元素满足给定的排序条件，就将相邻两个元素交换。当没有相邻的元素需要交换时，排序就完成了。

扫了一次后，最后一项必定是最大的。扫 $n-1$ 次就行了。

时间复杂度 $O(n^2)$，是一种稳定的排序算法。

```cpp
for (int i = 1; i < n; ++i)
    for (int j = 1; j <= n - i; ++j)
        if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
```

这个东西可以优化，如果扫完了之后发现没有交换的内容，那么说明已经排好序了。代码如下：

```cpp
for (int i = 1; i < n; ++i)
{
    bool flag = false;
    for (int j = 1; j <= n - i; ++j)
        if (a[j] > a[j + 1]) 
        {
            swap(a[j], a[j + 1]);
            flag = true;
        }
    if (!flag) break;
}
```

**冒泡排序与逆序对**。我们知道逆序对是指 $a[i]>a[j],i<j$，它也是冒泡排序的最小交换次数。需要知道的是，冒泡排序交换元素的次数就等于原序列的逆序对数。

可以这样理解：交换条件是 `a[j] > a[j + 1]`，这就是指一个逆序对，交换了之后如果不产生新的逆序对，那么逆序对数必然会减少一；由于交换后 $j$ 前面的依然在 $j$ 前面，$j+1$ 后面的依然在后面，所以逆序对数没有改变。综上可得冒泡排序交换元素的最小次数就等于原序列的逆序对数。

### 选择排序

原理是每次寻找第 $i$ 小的元素，与第 $i$ 个位置上的东西交换，时间复杂度 $O(n^2)$。

是一种不稳定的排序算法，如 *3* 3 1，将斜体 *3* 和 1 比较，变成了 1 3 *3*，斜体 *3* 到了直体 3 后面。

```cpp
for (int i = 1; i < n; ++i)
{
    int ith = i; // 就是寻找后面最小的元素，那个一定是第 i 小
    for (int j = i + 1; j <= n; ++j)
        if (a[j] < a[ith]) ith = j;
    swap(a[i], a[ith]);
}
```

### 插入排序

插入排序的工作原理是将待排列元素划分为“已排序”和“未排序”两部分，每次从“未排序的”元素中选择一个插入到“已排序的”元素中的正确位置。

一个与插入排序相同的操作是打扑克牌时，从牌桌上抓一张牌，按牌面大小插到手牌后，再抓下一张牌。

是一种稳定的排序算法。平均时间复杂度为 $O(n^2)$。

```cpp
for (int i = 2; i <= n; ++i) {
    int now = a[i], j = i - 1;
    for (; j >= 1; --j) {
        if (a[j] > now) a[j + 1]  = a[j];
        else break;
    }
    a[j + 1] = now;
}
```

### 快速排序

基于分治，但是均可以使用 STL 替代：

```cpp
sort(a + 1, a + n + 1); // 排序
nth_element(a + 1, a + k, a + n + 1); // 查找 k 小元素
```

### 归并排序

没什么用处，求逆序对的话使用权值树状数组更为直观。

### 离散化

就是将无穷大集合中的若干元素映射为有限集合来方便统计。当有些数据因为本身很大或者类型不支持，自身无法作为数组的下标来方便地处理，而影响最终结果的只有元素之间的相对大小关系时，我们可以将原来的数据按照从大到小编号来处理问题，即离散化。

~~但是这个为什么东西叫离散化？我也不知道。~~

最常见的是有 $n$ 个 `int` 范围内的整数，可能有重复，假定去重后有 $m$ 个整数。我们要把每个 $a[i]$ 用 $1\sim m$ 之间的整数代替，并且保证顺序大小不变。当然可以使用 `map`，但是我们来探讨一个常数更小的算法。

我们可以把 $a$ 排序并去重，得到有序数组 $b[1]\sim b[m]$，若查询 $i$ 代表的数值，返回 $b[i]$ 即可。若要查询整数 $a[j]$ 被那个 $1\sim m$ 之间的整数代替，只需要在 $b$ 中二分查找它的位置。

```cpp
void discrete(void) { // 离散化
    sort(b + 1, b + n + 1); // 先排序
    m = unique(b + 1, b + n + 1) - (b + 1); // 再去重
}

int P(int x) { // 查询谁替代了 x
    return lower_bound(b + 1, b + n + 1, x) - b;
}
```

[\[Codeforces 670C\] Cinema](https://codeforces.com/problemset/problem/670/C).

这些语言的数比较大，可以把它们都放到一个数组里，离散化后就容易统计了。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

inline int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m, size;
int a[200005], b[200005], c[200005];
int d[800005];
int cnt[800005];

inline int P(int x) {
    return lower_bound(d+1, d+size+1, x) - d;
}

int main(void) {
    n = read();
    int tot = 0;
    for (int i = 1; i <= n; ++i) a[i] = read(), d[++tot] = a[i];
    m = read();
    for (int i = 1; i <= m; ++i) b[i] = read(), d[++tot] = b[i];
    for (int i = 1; i <= m; ++i) c[i] = read(), d[++tot] = c[i];
    sort(d+1, d+tot+1);
    size = unique(d+1, d+tot+1) - (d+1);
    for (int i = 1; i <= n; ++i)
        ++cnt[P(a[i])];
    int ans = 0, ret = -1, res = -1;
    for (int i = 1; i <= m; ++i) {
        int x = cnt[P(b[i])], y = cnt[P(c[i])];
        if (x > ret || (x == ret && y > res))
            ans = i, ret = x, res = y;
    }
    cout << ans << endl;
    return 0;
}
```
{% endfolding %}

## 单调栈与单调队列

如果栈或队列中的元素具有单调性，那么它们就分别被成为单调栈和单调队列，可以及时帮我们排除不可能的决策。

### 单调栈

[模板](https://www.luogu.com.cn/problem/P5788)。

由于求的是之后，所以我们可以从后往前考虑。

第一个大于？想到了什么？如果这个数是后被考虑到的（即下标小），而且它还比前面考虑到的数大，那么这些数就不可能称为答案了。用俗语来说就是：

{% noteblock radiation red %}

如果一个人比你小，还比你强，那么你就永远也打不过他了。

但是钢铁洪流不会认输。

{% endnoteblock %}

我们可以建立一个栈，记录编号，如果发现满足这种条件的东西，那么就弹栈，最后 `push` 即可。代码如下：

```cpp
int n;
int a[3000005];
int ans[3000005];
stack <int> s;

int main(void)
{
    n = read();
    for (int i = 1; i <= n; ++i) a[i] = read();
    for (int i = n; i >= 1; --i) // 倒序扫
    {
        while (!s.empty() && a[s.top()] <= a[i]) s.pop(); // 弹栈
        ans[i] = s.empty() ? 0 : s.top(); // 记录答案
        s.push(i); // 将这个数压进栈里
    }
    // 最后输出答案即可
    return 0;
}
```

注意，`stack` 很慢，如果可以建议手写。

### 单调队列

[模板](https://www.luogu.com.cn/problem/P1886)。

这回可不一样了，它还要求区间的长度。如果我们开一个双端队列，那么不就可以及时弹掉不符合要求的数了吗？

所以说，单调队列相比单调栈增加了弹出超过时间的数的功能。

```cpp
deque <int> q;
for (int i = 1; i <= n; ++i)
{
    while (!q.empty() && a[q.back()] >= a[i]) q.pop_back();
    while (!q.empty() && q.front() <= i - k) q.pop_front();
    q.push_back(i);
    // print(a[q.front()])
}

for (int i = 1; i <= n; ++i)
{
    while (!q.empty() && a[q.back()] <= a[i]) q.pop_back();
    while (!q.empty() && q.front() <= i - k) q.pop_front();
    q.push_back(i);
    // print(a[q.front()])
}
```

当然，也可以使用数组模拟 `deque`。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, k;
int q[1000005];
int a[1000005];

int main(void)
{
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    
    int L = 1, R = 0;
    for (int i = 1; i <= n; ++i)
    {
        while (L <= R && a[q[R]] >= a[i]) --R;
        while (L <= R && q[L] <= i - k) ++L;
        q[++R] = i;
        if (i >= k) printf("%d ", a[q[L]]);
    }
    putchar('\n');

    L = 1, R = 0;
    for (int i = 1; i <= n; ++i)
    {
        while (L <= R && a[q[R]] <= a[i]) --R;
        while (L <= R && q[L] <= i - k) ++L;
        q[++R] = i;
        if (i >= k) printf("%d ", a[q[L]]);
    }
    putchar('\n');
    return 0;
}
```

## 前缀和与差分

这是很简单但很重要的优化技巧。

### 前缀和

前缀和是一种重要的预处理，能大大降低查询的时间复杂度。可以简单理解为“数列的前 $n$ 项的和”。

#### 实现

[模板](https://www.luogu.com.cn/problem/B3612)。

设 $S_i=\sum_{j=1}^{i}A_i$，则 $\sum_{i=l}^{r} = S_r-S_{l-1}$。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

inline int read(void) {
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

i64 sum[100005];

int main(void) {
    int n = read();
    for (int i = 1; i <= n; ++i) sum[i] = read() + sum[i-1];
    int m = read();
    while (m--) {
        int l = read(), r = read();
        printf("%lld\n", sum[r] - sum[l - 1]);
    }
    return 0;
}
```

#### [Luogu P1115] 最大子段和

[Portal](https://www.luogu.com.cn/problem/P1115).

最大子段和问题有很多种解决方式，比如动态规划。这里我们用前缀和再实现一个线性算法（虽然应该算滑动窗口）。

一个子段和等于 $S[r]-S[l-1]$，当 $S[r]$ 一定时，找到最小的 $S[l-1]$ 就可以了。可以简单地记录 $S[l-1]$，代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int read(void) {
    int x = 0, c = getchar(), f = 1;
    while (!isdigit(c)) {if (c == '-') f = -1; c = getchar();}
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar();
    return x * f;
}

int sum[200005];

int main(void) {
    int n = read(), ans = -2000000000, minn = 0;
    for (int i = 1; i <= n; ++i) sum[i] = read() + sum[i-1];
    for (int R = 1; R <= n; ++R) {
        ans = max(ans, sum[R] - minn);
        minn = min(minn, sum[R]);
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 差分

差分是前缀和的逆运算。对于一个数组 $A$，定义它的差分数组 $B$，满足 $B_1=A_1, B_i=A_i-A_{i-1}(i\ge 2)$。对这个差分数组求前缀和得到的就是原数组，对前缀和求差分也能得到原数组。

#### 实现

[\[Luogu P2367\] 语文成绩](https://www.luogu.com.cn/problem/P2367)。

求出原序列 $A$ 的差分序列 $B$。求出将 $b_x$ 增加 $1$，那么 $A_x \cdots A_n$ 都会增加 $1$。利用这个规律就可以解决这个问题了。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

int n, p;
i64 a[5000005], b[5000005];

int main(void) {
    scanf("%d%d", &n, &p);
    for (int i = 1; i <= n; ++i) scanf("%lld", a + i);
    for (int i = 1; i <= n; ++i) b[i] = a[i] - a[i - 1];
    while (p--) {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        b[x] += z, b[y + 1] -= z;
    }
    i64 ans = 1000000000000;
    for (int i = 1; i <= n; ++i) // 此时的 a 用于计算前缀和
        a[i] = a[i - 1] + b[i], ans = min(ans, a[i]);
    printf("%lld\n", ans);
    return 0;
}
```

#### [Luogu P3406] 海底高铁

[Portal](https://www.luogu.com.cn/problem/P3406).

我们只需要统计出坐每辆车的次数就可以进行计算了。维护一个差分数组，对于将 $[l,r-1]+1$ 的操作，只需要让 $S_l+1, S_{r-1}+1$ 即可，最后算一遍前缀和就行了。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

int m, n, a, b, c, p[100005];
i64 sum[100005];

int main(void) {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; ++i)
        scanf("%d", p + i);
    for (int i = 2; i <= m; ++i)
        ++sum[min(p[i - 1], p[i])], --sum[max(p[i - 1], p[i])];
    for (int i = 2; i <= n; ++i)
        sum[i] += sum[i - 1];
    i64 ans = 0;
    for (int i = 1; i < n; ++i) {
        scanf("%d%d%d", &a, &b, &c);
        ans += min(a * sum[i], b * sum[i] + c);
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

### 高维问题

前缀和和差分不仅能够在一维线性空间上工作，还可以在高维空间上工作。主要应用是二位前缀和和二维差分（~~三维可以自己类比，需要一定的空间想象能力，这里不做介绍 {% psw 实际上你也用不着 %}~~）{% emoji 滑稽 %}。

#### 二维前缀和

[\[Luogu P1719\] 最大加权矩形](https://www.luogu.com.cn/problem/P1719)。要求给出一个 $O(n^3)$ 的算法。

我们可以枚举矩形的四个端点坐标，然后使用二维前缀和在常数时间内解决问题。利用容斥原理很容易得到结果，具体过程请读者自行模拟：记 

$$s[i,j]=\sum\limits_{p=1}^i\sum\limits_{q=1}^j A_{i,j}$$

则

$$
\sum\limits_{x_{1}=1}^{x_{2}}\sum\limits_{y_{1}=1}^{y_{2}} A_{i,j}=
s[x_{2},y_{2}]-s[x_{1}-1,y_{2}]-s[x_{2},y_{1}-1]+s[x_{1}-1,y_{1}-1]
$$

那么 $O(n^4)$ 的代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, ans = -1000000000;
int sum[125][125];

int solve(int x1, int y1, int x2, int y2) {
    return sum[x2][y2] - sum[x1 - 1][y2] - sum[x2][y1 - 1] + sum[x1 - 1][y1 - 1];
}

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            int x;
            scanf("%d", &x);
            sum[i][j] = sum[i - 1][j] + sum[i][j - 1] + x - sum[i - 1][j - 1];
        }
    for (int x1 = 1; x1 <= n; x1++)
        for (int y1 = 1; y1 <= n; y1++)
            for (int x2 = x1; x2 <= n; x2++)
                for (int y2 = y1; y2 <= n; y2++)
                    ans = max(ans, solve(x1, y1, x2, y2));
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

考虑优化。还记得《最大子段和》吗？可以枚举上下边界，中间的这一大排就是个最大子段和问题！可以使用滑动窗口或 DP 实现最大子段和。这里用一个很聪明的方式：如果当前累计和是非负数就保留，负数就扔掉。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, ans = -1000000000;
int sum[125][125];

int solve(int x1, int y1, int x2, int y2)
{
    return sum[x2][y2] - sum[x1 - 1][y2] - sum[x2][y1 - 1] + sum[x1 - 1][y1 - 1];
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            int x;
            scanf("%d", &x);
            sum[i][j] = sum[i - 1][j] + sum[i][j - 1] + x - sum[i - 1][j - 1];
        }
    for (int x1 = 1; x1 <= n; x1++)
        for (int x2 = x1; x2 <= n; x2++)
        {
            int s = 0;
            for (int i = 1; i <= n; i++)
            {
                s += solve(x1, i, x2, i);
                ans = max(ans, s);
                if (s < 0) s = 0;
            }
        }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P2004] 领地选择

[Portal](https://www.luogu.com.cn/problem/P2004).

使用二位前缀和，简单枚举即可。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m, c;
int sum[1005][1005];
int ans = -1000000000, ansx, ansy;

int main(void)
{
    scanf("%d%d%d", &n, &m, &c);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
        {
            int x;
            scanf("%d", &x);
            sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + x;
        }
    for (int i = 1; i <= n - c + 1; i++)
        for (int j = 1; j <= m - c + 1; j++)
        {
            if (sum[i + c - 1][j + c - 1] + sum[i - 1][j - 1] - sum[i + c - 1][j - 1] - sum[i - 1][j + c - 1] >= ans)
            {
                ans = sum[i + c - 1][j + c - 1] + sum[i - 1][j - 1] - sum[i + c - 1][j - 1] - sum[i - 1][j + c - 1];
                ansx = i;
                ansy = j;
            }
        }
    printf("%d %d\n", ansx, ansy);
    return 0;
}
```
{% endfolding %}

#### [HNOI2003] 激光炸弹

[Portal](https://www.luogu.com.cn/problem/P2280).

直接用二位前缀和处理，然后枚举激光炸弹投放的位置即可。空间较为紧张，最好直接在前缀和数组上进行读入。虽然目标是一个质点，但我们可以把它看成一个块块，前缀和就可以做了{% emoji 滑稽 %}。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int read(void)
{
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int n, m;
int s[5005][5005];

int main(void)
{
    n = read(), m = read();
    while (n--)
    {
        int x = read(), y = read(), w = read();
        s[x + 1][y + 1] += w;
    }
    for (int i = 1; i <= 5001; ++i)
        for (int j = 1; j <= 5001; ++j)
            s[i][j] += s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1];
    int ans = 0;
    for (int i = m; i <= 5001; ++i)
        for (int j = m; j <= 5001; ++j)
            ans = max(ans, s[i][j] - s[i - m][j] - s[i][j - m] + s[i - m][j - m]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### 二维差分

[\[Luogu P3397\] 地毯](https://www.luogu.com.cn/problem/P3397)。

将差分扩展到二维即可。请读者自行用容斥原理进行模拟。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
int a[1005][1005];
int ans[1005][1005];

int main(void)
{
    scanf("%d%d", &n, &m);
    while (m--)
    {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        a[x2 + 1][y2 + 1]++;
        a[x1][y1]++;
        a[x1][y2 + 1]--;
        a[x2 + 1][y1]--;
    }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            ans[i][j] = ans[i - 1][j] + ans[i][j - 1] - ans[i - 1][j - 1] + a[i][j];
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            printf("%d ", ans[i][j]);
        putchar('\n');
    }
    return 0;
}
```
{% endfolding %}

### 高维问题的 DP 解法

~~这是什么？~~

你能够轻松想象三维前缀和甚至四维前缀和的做法码？~~反正笔者不能。~~

用 DP 计算可以使过程变得简单许多。这种 DP 是 sosDP（Sum over Subsets DP，子集和 DP，是状压 DP 的一种）。

鉴于本文的内容水平，这里不给出具体原理，请读者自行模拟。

```cpp 二维前缀和
// 读入时直接读到前缀和数组里
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
        scanf("%d", &s[i][j]);

// 进行两次运算，注意 +=，一次是 i - 1，一次是 j - 1
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
        s[i][j] += s[i - 1][j];
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
        s[i][j] += s[i][j - 1];
```

```cpp 三维前缀和
// 和二维基本相同
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= k; ++j)
        for (int k = 1; k <= n; ++k)
            scanf("%d", &s[i][j][k]);
for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
        for (int k = 1; k <= n; k++) 
            s[i][j][k] += s[i - 1][j][k];
for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
        for (int k = 1; k <= n; k++)
            s[i][j][k] += s[i][j - 1][k];
for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
        for (int k = 1; k <= n; k++)
            s[i][j][k] += s[i][j][k - 1];
```

但是查询好像还是要用容斥。这里给出公式供背诵：

三维维护：`s[i][j][k] = s[i-1][j][k] + s[i][j-1][k] + s[i][j][k-1] - s[i-1][j-1][k] - s[i-1][j][k-1] - s[i][j-1][k-1] + s[i-1][j-1][k-1] + a[i][j][k]`
三维求和：`s[x2][y2][z2] - s[x1-1][y2][z2] - s[x2][y1-1][z2] - s[x2][y2][z1-1] + s[x1-1][y1-1][z2] + s[x1-1][y2][z1-1] + s[x2][y1-1][z1-1] - s[x1-1][y1-1][z1-1]`

差分大概也是同样的原理：

三维差分： `b[x1][y1][z1] += c , b[x2+1][y1][z1] -=c , b[x1][y2+1][z1] -= c , b[x1][y1][z2+1] -= c , b[x2+1][y2+1][z1] += c , b[x2+1][y1][z2+1] += c , b[x1][y2+1][z2+1] += c , b[x2+1][y2+1][z2+1] -= c`

然后呢{% emoji aru-l 2030 %}？$n$ 维前缀和的问题较为困难，需要使用 sosDP，请读者阅读笔者的《基于状态进行优化的 DP》一文。

## 贪心的应用

真的不是简单贪心了。

### 贪心方法

必须满足当前一步是最优解，那么全局就是最优解。

**排序贪心**。这种做法是离线的，通过排序找出最优的。   
**反悔贪心**。这种做法是在线的，无论当前的选择是什么都接受，如果之后发现这个选项不优了，那么就废弃掉。

### 贪心证明

1. **微扰法**。对局部的任何改变都不会使得结果变好。
2. **范围缩放**。证明局部的最优策略必定是全局的最优策略，或者是局部最优决策已经包含了所有可能的情况。
3. **反证法**。
4. **数学归纳法**。$n=1$ 时成立；$n=m$ 成立时，$n=m+1$ 也成立。那么对于任意的 $n$ 就都成立。

### 简单题

更多的题会在 Problemset 中出现。

#### [USACO07NOV] Sunscreen G

[Portal](https://www.luogu.com.cn/problem/P2887).

按照 `minSPF` 递减进行排序，每次找能用的最大的防晒霜。微扰可以证明。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#define pint pair<int, int>

using namespace std;

int c, l;
pint a[2505], b[2505];

int main(void) {
    scanf("%d%d", &c, &l);
    for (int i = 1; i <= c; ++i)
        scanf("%d%d", &a[i].first, &a[i].second);
    for (int i = 1; i <= l; ++i)
        scanf("%d%d", &b[i].first, &b[i].second);
    sort(a + 1, a + c + 1, greater<pint>());
    sort(b + 1, b + l + 1, greater<pint>());
    int ans = 0;
    for (int i = 1; i <= c; ++i) {
        for (int j = 1; j <= l; ++j)
            if (b[j].second && b[j].first >= a[i].first && b[j].first <= a[i].second) {
                b[j].second -= 1;
                ans += 1;
                break;
            }
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [USACO06FEB] Stall Reservations S

[Portal](https://www.luogu.com.cn/problem/P2859).

将每一个事件拆分成开吃和结束吃，然后找能吃的放进去。可以证明再开一个的话结果不会更好。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

struct event {
    int id, type, time;
    event(int id = 0, int type = 0, int time = 0) :
        id(id), type(type), time(time) {}
    bool operator < (const event &a) const {
        return time < a.time;
    }
} a[100005];

struct fence {
    int id, time;
    fence(int id = 0, int time = 0) :
        id(id), time(time) {}
    bool operator < (const fence &a) const {
        return time > a.time;
    }
};

int n, cnt;
int use[100005];
priority_queue <fence> q;

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        a[(i << 1) - 1] = event(i, 1, x);
        a[i << 1] = event(i, 2, y);
    }
    sort(a + 1, a + n * 2 + 1);
    cnt = 1; q.push({1, 0});
    for (int i = 1; i <= n * 2; ++i) {
        if (a[i].type == 1) {
            if (q.empty() || q.top().time > a[i].time) {
                cnt += 1;
                q.push({cnt, 0});
            }
            int u = q.top().id; q.pop();
            use[a[i].id] = u;
        } else q.push({use[a[i].id], a[i].time + 1});
    }
    printf("%d\n", cnt);
    for (int i = 1; i <= n; ++i)
        printf("%d\n", use[i]);
    return 0;
}
```
{% endfolding %}

#### [UVA1193] Radar Installation

[Portal](https://www.luogu.com.cn/problem/UVA1193).

每一个建筑能被哪个区间的监视器监视是可以预处理的，然后将范围按照左端点排序，以此考虑是否能监视，能监视就监视。

因为如果不监视而要新建的话，未来也是可以新建的，现在就监视并不会丢失解。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#define pdb pair<double, double>
#define L first 
#define R second

using namespace std;

int n, d;
pdb a[1005];

int main(void) {
    int kase = 0;
    while (scanf("%d%d", &n, &d) == 2 && n) {
        bool flag = true;
        for (int i = 1; i <= n; ++i) {
            int x, y;
            scanf("%d%d", &x, &y);
            double t = d * d - y * y;
            if (t < 0) flag = false;
            t = sqrt(t);
            a[i].L = x - t, a[i].R = x + t;
        }
        if (!flag) {
            printf("Case %d: -1\n", ++kase);
            continue;
        }
        sort(a + 1, a + n + 1);
        int ans = 1;
        double pos = a[1].R;
        for (int i = 2; i <= n; ++i) {
            if (pos >= a[i].L) pos = min(pos, a[i].R);
            else pos = a[i].R, ++ans;
        }
        
        printf("Case %d: %d\n", ++kase, ans);
    }
    return 0;
}
```
{% endfolding %}

#### [NOIP2012 提高组] 国王游戏

[Portal](https://www.luogu.com.cn/problem/P1080).

{% noteblock %}
恰逢 H 国国庆，国王邀请 $n(n\le 1000)$ 位大臣来玩一个有奖游戏。首先，他让每个大臣在左、右手上面分别写下一个整数，国王自己也在左、右手上各写一个整数。然后，让这 $n$ 位大臣排成一排，国王站在队伍的最前面。排好队后，所有的大臣都会获得国王奖赏的若干金币，每位大臣获得的金币数分别是：排在该大臣前面的所有人的左手上的数的乘积除以他自己右手上的数，然后向下取整得到的结果。

国王不希望某一个大臣获得特别多的奖赏，所以他想请你帮他重新安排一下队伍的顺序，使得获得奖赏最多的大臣，所获奖赏尽可能的少。注意，国王的位置始终在队伍的最前面。
{% endnoteblock %}

大胆假设，小心求证：按照左右手上的数的乘积从小到大排序。

我们采用微扰法来证明，交换大臣 $i$ 和 $i+1$，原来的奖励是：

$$
\frac{1}{B[i]}\prod_{j=0}^{i-1} A[j]\\
\frac{A[i]}{B[i+1]}\prod_{j=0}^{i-1} A[j]
$$

交换后的奖励是：

$$
\frac{1}{B[i+1]}\prod_{j=0}^{i-1}A[j]\\
\frac{A[i+1]}{B[i]}\prod_{j=0}^{i-1}A[j]
$$

其它奖励并不变，我们只需要比较上面两个，即比较：

$$
\max\left\{\frac{1}{B[i]},\frac{A[i]}{B[i+1]}\right\}\\
\max\left\{\frac{1}{B[i+1]},\frac{A[i+1]}{B[i]}\right\}
$$

通分得到：

$$
\max\{B[i+1],A[i]\times B[i]\}\\
\max\{B[i],A[i+1]\times B[i+1]\}
$$

分类讨论比较大小后发现前者不会比后者差。

本题需要高精，代码偷懒了。

{% folding cyan::查看代码 %}
```py
n = int(input())
a, b = map(int, input().split())
ans = []
for i in range(n):
    ans.append([int(ii) for ii in input().split()])
ans.sort(key=lambda x: (x[0] * x[1]))
s = 0
for i in range(n):
    s = max(s, a // ans[i][1])
    a *= ans[i][0]
print(s)
```
{% endfolding %}

## 倍增

## Problemset

我们来看一些题目。

### 双指针

这里的双指针可能会稍微难一些{% emoji tong Disagree %}。

#### [Luogu P5745] 数列求和

[Portal](https://www.luogu.com.cn/problem/P5745).

虽然可以用二分做到 $O(n \log n)$，然而并不推荐这样做，因为还有写起来更简单跑起来也更快的 $O(n)$ 做法。   
我们先定义两个指针 $l=r=1$，如果$\sum\limits_{i=l}^{r}i < m$，那么令 `r++`，否则令 `l++`，可看出数列中的每个元素都被扫了常数次，时间复杂度$O(n)$。   
相比 $O(n^2)$ 的枚举，少枚举了不少不可能成为答案的解。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

#define LL long long

using namespace std;

inline int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n, m;
int a[4000005];
LL sum[4000005];

int main(void) {
    n = read(), m = read();
    for (int i = 1; i <= n; ++i) 
        a[i] = read(), sum[i] = sum[i-1] + a[i]; //计算前缀和

    int l = 1, r = 1, L, R;
    LL ans = -1;
    while (r <= n) {
        while (sum[r] - sum[l-1] <= m) {
            if (sum[r] - sum[l-1] > ans) {
                ans = sum[r] - sum[l-1];
                L = l, R = r;
            }
            ++r;
        }
        ++l;
    }

    printf("%d %d %lld", L, R, ans);
    return 0;
}
```
{% endfolding %}

#### [ARC 098B] Xor Sum 2

[Portal](https://www.luogu.com.cn/problem/AT4142).

什么时候加和和异或和相等？只有当异或的时候不存在两个二进制位都是 $1$ 的时候。也就是说，如果右端点固定，缩小左端点，那么这样两个二进制位都是 $1$ 的情况是只能变少或不变，不能变多的。那么就可以使用尺取法了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, s1, s2;
int a[200005];

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i);
    i64 ans = 0;
    for (int L = 1, R = 0; L <= n; ++L) {
        while (R < n && s1 + a[R + 1] == (s2 ^ a[R + 1])) ++R, s1 += a[R], s2 ^= a[R];
        ans += R - L + 1;
        s1 -= a[L], s2 ^= a[L];
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [CEOI2017] Sure Bet

[Portal](https://www.luogu.com.cn/problem/P4653).

如果都取一个灯泡，我们肯定是取权值更大的更好，所以先排序。A、B 的和肯定是平均好，所以用双指针同时扫描 AB，记录当前的和，其中有一个大了就移动另一个指针。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n;
double a[100005], b[100005], suma[100005], sumb[100005];

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%lf%lf", a + i, b + i);
    sort(a + 1, a + n + 1, greater<double>());
    sort(b + 1, b + n + 1, greater<double>());
    for (int i = 1; i <= n; ++i) {
        suma[i] = suma[i - 1] + a[i];
        sumb[i] = sumb[i - 1] + b[i];   
    }
    double ans = 0.0;
    for (int i = 1, j = 1; i <= n && j <= n;) {
        ans = max(ans, min(suma[i] - i - j, sumb[j] - i - j));
        if (suma[i] <= sumb[j]) ++i;
        else ++j;
    }
    printf("%.4lf\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Codeforces 939E] Maximize!

[Portal](https://codeforces.com/problemset/problem/939/E).

最大的肯定要选，再选若干个最小的。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
typedef long long i64;

int n, L = -1, maxn = 0;
i64 sum = 0;
vector <int> a;

int main(void) {
    scanf("%d", &n);
    while (n--) {
        int op, x;
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d", &x);
            a.push_back(x);
            sum += x - maxn;
            maxn = x;
            while (L + 1 < a.size() && 1ll * a[L + 1] * (L + 2) < sum)
                sum += a[++L];
        } else {
            if (L == -1) puts("0");
            else printf("%.6lf\n", maxn - (double)sum / double(L + 2));
        }
    }
    return 0;
}
```
{% endfolding %}

#### [NOIP2011 提高组] 选择客栈

[Portal](https://www.luogu.com.cn/problem/P1311).

枚举右指针，当一个客栈可以被用来喝咖啡时，就移动左指针更新。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, k, p, t = 1;
int color[200005];
int cnt[200005]; // 当前 cnt[i] 表示颜色为 i 的客栈的数目
i64 ans = 0;

int main(void) {
    scanf("%d%d%d", &n, &k, &p);
    for (int i = 1, money; i <= n; ++i) {
        scanf("%d%d", color + i, &money);
        if (money <= p) { // 到 i 为止的客栈都可以满足喝咖啡的条件了
            for (int j = t; j <= i; ++j) ++cnt[color[j]];
            t = i + 1;
            ans += cnt[color[i]] - 1; // 不能选择同一个客栈
        }
        else ans += cnt[color[i]];
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

### 前缀和与差分

前缀和与差分很重要，出现概率很高。

#### [USACO05JAN] Moo Volume S

[Portal](https://www.luogu.com.cn/problem/P6067).

绝对值很烦人，所以考虑排序。只计算当前 $i$ 及以前的，最后将结果乘以二。通过找规律发现 $ans_i=a_i\times (i-1)-s_{i-1}$，代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int n;
int a[100005];
long long sum[100005];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    sort(a + 1, a + n + 1);
    for (int i = 1; i <= n; ++i) sum[i] = sum[i - 1] + a[i];
    long long ans = 0;
    for (int i = 1; i <= n; ++i) ans += (long long)a[i] * (i - 1) - sum[i - 1];
    printf("%lld\n", ans * 2);
    return 0;
}
```
{% endfolding %}

#### [USACO16JAN] Subsequences Summing to Sevens S

[Portal](https://www.luogu.com.cn/problem/P3131).

区间和，前缀和！也就是说我们要使得 $7\mid (s[r]-s[l-1])$，要求最大的 $r-l$。   

根据取模的性质，可以转化为 $s[r]\equiv s[l-1] \pmod 7$，那么开两个数组 `L, R`，扫到 `s[i]`，就将其计入数组中。具体过程见代码：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int main(void)
{
    int n, x, sum = 0, ans = 0;
    int l[7] = {0, -1, -1, -1, -1, -1, -1}, r[7] = {0, -1, -1, -1, -1, -1, -1};
    // l[i] 存 mod 7 为 i 的最小 l - 1 , r[i] 存 mod 7 为 i 的最大 r
    // 开始时 s[0] = 0, s[0] mod 7 = 0，所以 l[0] = r[0] = 0
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", &x);
        sum = (sum + x) % 7; // 利用随时取模性质
        if (l[sum] == -1) l[sum] = i; // 第一次扫到的肯定最小
        r[sum] = i; // 最后一次扫到的肯定最大
    }
    for (int i = 0; i < 7; ++i)
        ans = max(ans, r[i] - l[i]);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [NOIP2012 提高组] 借教室

[Portal](https://www.luogu.com.cn/problem/P1083).

借教室的这一过程很容易用差分来进行。我们只需要知道需要通知哪一个申请人修改订单。可以使用二分答案的方式来判断是哪一个人。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int read(void) {
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int n, m, r[1000005];
int c[1000005], sum[1000005], d[1000005], s[1000005], t[1000005];

bool solve(int x) {
    for (int i = 1; i <= n; ++i) c[i] = r[i] - r[i - 1];
    for (int i = 1; i <= x; ++i) {
        c[s[i]] -= d[i];
        c[t[i] + 1] += d[i];
    }
    for (int i = 1; i <= n; ++i) sum[i] = sum[i - 1] + c[i];
    for (int i = 1; i <= n; ++i)
        if (sum[i] < 0) return 0;
    return 1;
}

int main(void) {
    n = read(), m = read();
    for (int i = 1; i <= n; ++i) r[i] = read();
    for (int i = 1; i <= m; ++i) d[i] = read(), s[i] = read(), t[i] = read(); 
    if (solve(m)) puts("0");
    else {
        puts("-1");
        int L = 0, R = m + 1;
        while (L + 1 != R) {
            int mid = L + R >> 1;
            if (solve(mid)) L = mid;
            else R = mid;
        }
        printf("%d\n", R);
    }
    return 0;
}
```
{% endfolding %}

#### [NOIP2011 提高组] 聪明的质监员

[Portal](https://www.luogu.com.cn/problem/P1314).

注意到这个条件：$w_j \ge W$。$W$ 越小，得到的 $y$ 就越大，具有单调性，因此可以二分答案，我们只要找到 $y,s$ 大小的分界点即可。现在的问题就是如何在线性时间内计算出 $y$。

前缀和就可以轻松完成。查询的内容是典型的多次查询区间和（每个 $y$ 都是两个区间和相乘）。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
using i64 = long long;

int read(void) {
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int n, m;
i64 s, ans = 1000000000005;
int w[200005], v[200005];
int l[200005], r[200005];
i64 sum[200005], sumv[200005];

bool P(int W) {
    for (int i = 1; i <= n; ++i) {
        sum[i] = sum[i-1] + (w[i] >= W);
        sumv[i] = sumv[i-1] + (w[i] >= W ? v[i] : 0);
    }
    i64 y = 0;
    for (int i = 1; i <= m; ++i)
        y += (sum[r[i]] - sum[l[i] - 1]) * (sumv[r[i]] - sumv[l[i] - 1]);
    i64 ret = llabs(y - s);
    if (ret < ans) ans = ret;
    if (y > s) return 1;
    else return 0;
}

int main(void) {
    n = read(), m = read();
    cin >> s;
    int minw = 1000005, maxw = 0;
    for (int i = 1; i <= n; ++i) {
        w[i] = read(), v[i] = read();
        minw = min(minw, w[i]);
        maxw = max(maxw, w[i]);
    }
    for (int i = 1; i <= m; ++i)
        l[i] = read(), r[i] = read();
    int L = minw - 1, R = maxw + 1;
    while (L + 1 != R) {
        int mid = L + R >> 1;
        if (P(mid)) L = mid;
        else R = mid;
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [USACO07MAR] Face The Right Way G

[Portal](https://www.luogu.com.cn/problem/P2882).

注意到数据范围只有 $5000$，容易想到要用一个 $O(n^2)$ 算法，我们可以枚举 $K$，问题就变成了如何在线性时间内计算这个 $K$ 能否完成任务。

从前到后进行扫描。如果遇到了一个 `B`，那么肯定要进行以它为起点的翻转，否则扫描到后面它就再也无力翻身了。这样可以进行模拟，时间复杂度为 $O(n^2)$，需要进行优化。

考虑差分。差分还可以这么做：**建立另一个数组来记录变化**，而不是对着原序列进行差分。由于只有 `0, 1` 两种数，我们可以用异或来简化运算。设 $b[i]$ 代表 $i$ 以后的元素都要异或上 $b[i]$，给 $[i, i + k - 1]$ 进行翻转，只需要 `b[i] ^= 1, b[i + k] ^= 1` 即可，另外再用一个 `c` 记录当前的状态，来判断当前扫描到的是 `1` 还是 `0`。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n;
int a[5005], b[5005];

int main(void) {
    scanf("%d", &n);
    char s[5];
    for (int i = 1; i <= n; ++i) {
        scanf("%s", s);
        a[i] = (s[0] == 'F');
    }
    int mink = 0, minres = 1000000;
    for (int k = 1; k <= n; ++k) {
        memset(b, 0, sizeof(b));
        bool c = 0, flag = true;
        int res = 0;
        for (int i = 1; i <= n; ++i) {
            c ^= b[i];
            if ((a[i] ^ c) == 0) {
                if (i + k - 1 > n) {
                    flag = false;
                    break;
                }
                b[i] ^= 1, b[i + k] ^= 1;
                c ^= 1, ++res;    
            }
        }
        if (flag && res < minres) minres = res, mink = k;
    }
    printf("%d %d\n", mink, minres);
    return 0;
}
```
{% endfolding %}

#### [Poetize6] IncDec Sequence

[Portal](https://www.luogu.com.cn/problem/P4552).

区间加减这种操作已经很熟悉了，肯定是把原序列做成差分序列进行计算，问题就变成了：给定一个序列 $B$，可以每次选择一对数，一个 `+1`，一个 `-1`，使得 $B_2\cdots B_n$ 都为 $0$。注意 $B_1$ 对结果没有影响，我们计算时也可以选择 $B_{n+1}$。

我们尽可能地选择 $B_2\cdots B_n$ 进行操作，这样可以尽快地接近目标。还有剩余怎么办？那就跟 $B_1$ 或 $B_{n+1}$ 配对就可以了。设 $B_2\cdots B_n$ 中正数和为 $p$，负数和为 $q$，则前者可以进行 $\min(p,q)$ 次，后者进行 $\left|p-q\right|$ 次，总共是 $\max(p,q)$ 次。根据是否选择 $B_1$ 进行配对，得到的序列共有 $\left|p-q\right|+1$ 种。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;
using i64 = long long;

int n;
i64 a[100005], b[100005];

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%lld", a + i);
    for (int i = 1; i <= n; ++i) b[i] = a[i] - a[i - 1];
    int p = 0, q = 0;
    for (int i = 2; i <= n; ++i)
        if (b[i] >= 0) p += b[i];
        else q -= b[i];
    printf("%lld\n%lld\n", max(p, q), abs(p - q) + 1);
    return 0;
}
```
{% endfolding %}

#### [Codeforces 1700C] Helping the Nature

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

#### [USACO07JAN] Tallest Cow S

[Portal](https://www.luogu.com.cn/problem/P2879).

要求所有的奶牛都尽可能的高。因此给了 $a,b$，就意味着它们之间的奶牛都比他们低 $1$，用差分维护即可。注意可能有重复的条件，所以要用一个 `set` 判重。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <set>
#include <algorithm>

#define pii pair<int, int>
#define X first
#define Y second

using namespace std;

int n, p, h, r, d[10005], s[10005];
set <pii> us;

int main(void)
{
    scanf("%d%d%d%d", &n, &p, &h, &r);
    while (r--)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        if (a > b) swap(a, b);
        if (us.find(make_pair(a, b)) != us.end()) continue;
        us.insert(make_pair(a, b));
        --d[a + 1], ++d[b];
    }
    for (int i = 1; i <= n; ++i)
    {
        s[i] = s[i - 1] + d[i];
        printf("%d\n", s[i] + h);
    }
    return 0;
}
```
{% endfolding %}

#### [Codeforcs 1703F] Yet Another Problem About Pairs Satisfying an Inequality

[Portal](https://codeforces.com/contest/1703/problem/F).

对不等式进行拆解，得到 $a_i<i$，$a_j<j$ 和 $i < a_j$ 三个不等式。前两个很容易判断，第三个考虑暴力算法，枚举 $i,j$ 可以轻松得到答案。

对此进行优化。$i < a_j$ 变形为 $a_j\ge i+1$（因为都是整数）。

除了前缀和，还有后缀和，就是表示它后面而不是前面的和。$a_j\ge i+1$ 具有明显的这种性质，要求的就是满足这个条件的数的个数，维护即可。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
using i64 = long long;

int n, t;
int a[200005];
i64 sum[200005];

int main(void) {
    scanf("%d", &t);
    while (t--) {
        memset(sum, 0, sizeof(sum));
        scanf("%d", &n);
        for (int i = 1; i <= n; ++i) {
            scanf("%d", a + i);
            if (a[i] < i) ++sum[a[i]]; // 满足基本条件，将这个 sum +1
        }
        for (int i = n; i >= 1; --i) // 计算后缀和
            sum[i] += sum[i + 1];
        i64 ans = 0;
        for (int i = 1; i <= n; ++i)
            if (a[i] < i) ans += sum[i + 1]; // a[j] >= i+1
        printf("%lld\n", ans);
    }
    return 0;
}
```
{% endfolding %}

#### [USACO11MAR] Brownie Slicing G

[Portal](https://www.luogu.com.cn/problem/P3017).

切的刀越多，每头牛分的就越少，满足单调性，考虑二分答案。

二分 Bessie 能得到的碎屑数目。通过暴力计算使得其它奶牛分的数目都大于等于她，若最后横着切的刀数大于 $a$（说明这些蛋糕够分给更多的奶牛，Bessie 分少了），就可以。其中需要用二维前缀和来优化。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int read(void) {
    int x = 0, c = getchar_unlocked();
    while (!isdigit(c)) c = getchar_unlocked();
    while (isdigit(c)) x = x * 10 + c - '0', c = getchar_unlocked();
    return x;
}

int r, c, a, b;
int s[505][505];

int calc(int x1, int y1, int x2, int y2) {
    return s[x2][y2] - s[x1 - 1][y2] - s[x2][y1 - 1] + s[x1 - 1][y1 - 1];
}

bool P(int x) {
    int cuta = 0, row = 1;
    for (int i = 1; i <= r; ++i) {
        int col = 1, cutb = 0;
        for (int j = 1; j <= c; ++j) {
            if (calc(row, col, i, j) >= x)  
                col = j + 1, ++cutb;
        }
        if (cutb >= b) ++cuta, row = i + 1;
    }
    return cuta >= a;
}

int main(void) {
    r = read(), c = read(), a = read(), b = read();
    for (int i = 1; i <= r; ++i)
        for (int j = 1; j <= c; ++j)
            s[i][j] = s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1] + read();
    int L = -1 , R = s[r][c] + 1;
    while (L + 1 != R) {
        int mid = (L + R) / 2, ss;
        if (P(mid)) L = mid;
        else R = mid;
    }
    printf("%d\n", L);
    return 0;
}
```
{% endfolding %}

### 单调栈与单调队列

但是单调栈和单调队列还有更多的应用，但是不会很难。

单调队列还可以优化 DP，请阅读笔者的相应文章。

#### [USACO06NOV] Bad Hair Day S

[Portal](https://www.luogu.com.cn/problem/P2866).

可以看出这是一个单调栈，每个数的答案就是栈的大小。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

int n, tot;
int a[80005], s[80005];

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    i64 ans = 0;
    for (int i = 1; i <= n; ++i)
    {
        while (tot && s[tot] <= a[i]) --tot;
        ans += tot;
        s[++tot] = a[i];
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### [POI2008] PLA-Postering

[Portal](https://www.luogu.com.cn/problem/P3467).

注意到宽度与答案没有关系，而高度相同的可以使用一张海报。我们使用单调栈来进行维护，如果进来了一个元素不比栈顶大，那么是有机会合并的。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n;
int w[250005];
int s[250005], tot = 0;

int main(void) {
    scanf("%d", &n);
    for (int i = 1, d; i <= n; ++i)
        scanf("%d%d", &d, w + i);
    int ans = n;
    for (int i = 1; i <= n; ++i) {
        while (tot && w[i] <= s[tot]) {
            if (w[i] == s[tot]) --ans;
            --tot;
        }
        s[++tot] = w[i];
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### 单调队列大模板

这里有许多单调队列的模板题。

{% tabs %}

<!-- tab [Luogu P2032] 扫描 -->

[Portal](https://www.luogu.com.cn/problem/P2032).

单调队列模板。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, k;
int a[2000005], q[2000005], L = 1, R = 0;

int main(void)
{
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) scanf("%d", a + i);
    for (int i = 1; i <= n; ++i)
    {
        while (L <= R && i - q[L] >= k) ++L;
        while (L <= R && a[i] >= a[q[R]]) --R;
        q[++R] = i;
        if (i >= k) printf("%d\n", a[q[L]]);
    }
    return 0;
}
```
{% endfolding %}

<!-- endtab -->

<!-- tab [Luogu P2251] 质量检测 -->

[Portal](https://www.luogu.com.cn/problem/P2251).

还是模板。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m, L = 1, R = 0;
int a[100005], q[100005] = {0x3f3f3f3f};

int main(void) {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", a + i);
        if (i <= m) q[i] = min(q[i - 1], a[i]);
    }
    for (int i = 1; i <= n; ++i) {
        while (L <= R && i - q[L] >= m) ++L;
        while (L <= R && a[i] <= a[q[R]]) --R;
        q[++R] = i;
        if (i >= m) printf("%d\n", a[q[L]]);
    }
    return 0;
}
```
{% endfolding %}

<!-- endtab -->

<!-- tab [Luogu P1440] 求 m 区间内的最小值 -->

[Portal](https://www.luogu.com.cn/problem/P1440).

还是模板。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m, L = 1, R = 0;
int a[2000005], ans[2000005], q[2000005] = {0x3f3f3f3f};

int main(void)
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) 
        scanf("%d", a + i);
    for (int i = 1; i <= n; ++i)
    {
        while (L <= R && i - q[L] >= m) ++L;
        while (L <= R && a[i] <= a[q[R]]) --R;
        q[++R] = i;
        ans[i] = a[q[L]];
    }
    for (int i = 1; i <= n; ++i)
        printf("%d\n", ans[i - 1]);
    return 0;
}
```
{% endfolding %}

<!-- endtab -->

{% endtabs %}

#### [Luogu P1714] 切蛋糕

[Portal](https://www.luogu.com.cn/problem/P1714).

这是经典的要求长度的最大子段和问题，我们枚举右端点，利用滑动窗口维护目前遇到过的最小的 $S[l-1]$ 即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
int a[500005], sum[500005];
int L = 1, R = 0, Q[500005];

int main(void) {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) 
        scanf("%d", a + i), sum[i] = sum[i - 1] + a[i];
    int ans = -1e9;
    Q[++R] = 0;
    for (int i = 1; i <= n; ++i) {
        while (L <= R && Q[L] + m < i) ++L;
        ans = max(ans, sum[i] - sum[Q[L]]);
        while (L <= R && sum[Q[R]] >= sum[i]) --R;
        Q[++R] = i;
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

#### [Luogu P1419] 寻找段落

[Portal](https://www.luogu.com.cn/problem/P1419).

显然可以二分。

如何 `check(x)`？条件是 $s[l\dots r]\ge x\times (r-l+1)$，那么就将序列整个减去 $x$，用单调队列判断是否存在一个和大于等于零的子段。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, a[100005];
double S[100005];
int s, t;
int Q[100005], L = 1, R = 0;

bool calc(double x) {
	L = 1, R = 0;
    for (int i = 1; i <= n; ++i)
        S[i] = S[i - 1] + a[i] - x;
    for (int i = s; i <= n; ++i) {
        while (L <= R && S[Q[R]] >= S[i - s]) --R;
        while (L <= R && Q[L] < i - t) ++L;
        Q[++R] = i - s;
        if (L <= R && S[i] - S[Q[L]] >= 0) return true;
    }
    return false;
}

int main(void) {
    scanf("%d%d%d", &n, &s, &t);
    for (int i = 1; i <= n; ++i)
        scanf("%d", a + i);
    double L = -10000, R = 10000;
    for (int op = 0; op < 200; ++op) {
        double mid = (L + R) / 2;
        if (calc(mid)) L = mid;
        else R = mid;
    }
    printf("%.3lf\n", R);
    return 0;
}
```
{% endfolding %}

### 贪心

这里的贪心会变得更难想。

[^1]: 但是钢铁洪流永不认输。