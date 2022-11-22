---
title: 数论初步
headimg: 'https://z3.ax1x.com/2021/11/07/I1P4r6.jpg'
cover: false
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 数论
    - 数学
    - 笔记
plugins:
    - katex
abbrlink: d8a80121
date: 2022-08-16 14:12:23
group: notes
hide: true
status: working
---

> 数论主要研究整数的性质，被数学王子高斯称为“数学上的皇冠”。初等数论是用初等方法研究的数论，它的研究方法本质上说，就是利用整数环的整除性质，主要包括整除理论、同余理论、连分数理论。在 OI 中，几乎跟数学有关的题目都绕不开初等数论，本文将引导你学习初等数论的基础知识。

<!--more-->

{% folding blue::更新日志 %}

{% timeline %}

{% timenode 2022/08/17 %}

调整了例题的结构，将部分题目放到了文章中，同时增补了一些知识点和习题。

{% endtimenode %}

{% timenode 2022/08/16 %}

将旧文章搬了过来。

{% endtimenode %}

{% endtimeline %}

{% endfolding %}

OI 中的数学并没有数竞中的那么难{% emoji 滑稽 %}，因为正确证明不再是首要任务（尽管证明可以帮助你理解），应用才是王道{% emoji 滑稽 %}。

在阅读本文前，请保证您学过小学数学（雾

如果你想以数学竞赛的模式学习数论，请参阅潘承洞的《初等数论》，并提前学习高等数学基本内容：

{% image https://z3.ax1x.com/2021/11/12/IDLXJP.jpg::width=400px %}

所以还是以 OI 的模式学习数论吧(￣ ‘i ￣;)。

注意，OI 的模式学习数论，是指证明过程不会那么详细，会显得“感性理解”，甚至有些直接跳过证明，但足以让你在考场上回忆起这些性质并用它们解题（考场上想的做法往往可以用对拍或其他手段来验证，感性证明“好像是对的”就足够了，重要的还是大量做题）。严谨甚至多种的证明过程请参考《初等数论》《具体数学》等书籍——这未必不是好事，虽然会花费时间，但收获也是巨大的。

希望在阅读时您能拿起纸笔和键盘，适当的演算和完成代码实现。

## 基础知识

接下来若没有特殊说明，我们的所有数都满足 $x \in \mathbb{Z}$。

### 模运算

大家都应该知道取模 $\text{mod}$。

#### 随时取模性质

就是说 $\sum (x_i \bmod p) = (\sum x_i)\bmod p$，$\prod (x_i \bmod p) = (\prod x_i)\bmod p$。用人话说，运算的最终结果要取模，那么运算过程中的加法、减法和乘法都可以随时取模（但是除法不行！需要用下文介绍的逆元来解决）。

这东西可以证明，下文会出现（

#### C++ 的取模

在 C++ 中，用 `%` 来表示取模。

这里问个问题，$-5\bmod 3$ 是多少？   

有的人会说是 $-2$，实际上是错误的{% emoji 滑稽 %}。

我们不知道 $-5\bmod 3$ 是多少，但我们知道它等于 $1\bmod 3$，所以它的答案是 $1$。

从 C99 和 C++11 起，规定商向零取整（舍弃小数部分），取模的符号即与被除数相同。从此以下运算结果保证为真：

```txt
5 % 3 == 2;
5 % -3 == 2;
-5 % 3 == -2;
-5 % -3 == -2;
```

这也就意味着，如果存在负数，那么必须这样实现取模：`(a % b + b) % b`。

### 整除与约数

若 $a=bk$，则 $b$ 整除 $a$，记作 $b\mid a$。不整除记作 $b\nmid a$。   
也称 $b$ 是 $a$ 的约数（因数），$a$ 是 $b$ 的倍数。

整除有一些性质：

- $a\mid b \iff -a\mid b \iff a\mid -b \iff |a| \mid |b|$；
- 如果 $a\mid b, a\mid c$，则可得 $a\mid b+c$；
- 如果 $a\mid b, b\mid c$，则 $a\mid c$，也就是整除的传递性；
- 若 $m\ne 0$，则 $a\mid b \iff ma\mid mb$；

证明如下：

{% noteblock %}
由 $b=aq\iff b=(-a)(-q) \iff -b = a(-q) \iff |b| = |a|\mid |q|$ 得。

$a\mid b, a\mid c$ 可得 $ak_1=b,ak_2=c$，所以 $a(k_1+k_2)=b+c$，所以 $a\mid b+c$。

$b=aq_1,c=bq_2$，得 $c=a(q_1q_2)\Longrightarrow a\mid c$。

$a\mid b \iff b=aq \iff mb = (ma)q \iff ma\mid mb$。
{% endnoteblock %}

还有：

- $a\mid b$ 且 $b\mid a \Longrightarrow b=\pm a$，这是因为这两个式子代表 $b=ap,a=bq\Rightarrow ab=abpq\Rightarrow pq=1\Rightarrow p=\pm 1$；
- 设 $b\neq 0$，则 $a\mid b \Longrightarrow |a|\le |b|$；
- 若 $a\neq 0,b=qa+c$，那么 $a\mid b \iff a\mid c$。

核心的证明思路就是将 $a\mid b$ 写作 $b=pa$。

#### 求 N 的正约数集合

若 $a$ 是 $n$ 的约数，则 $\cfrac{n}{a}$ 也是 $n$ 的约数，且 $a$ 和 $\cfrac{n}{a}$ 之中必有一个 $\leqslant \sqrt{n}$。
枚举 $1\sim \sqrt{n}$ 的所有数，判断是不是 $n$ 的约数即可，代码如下：

```cpp
int factor[1605];
inline int divisor(int n) // 返回约数个数，存在 factor 中
{
    int m = 0, t = min(n, sqrt(n) + 1);
    for (int i = 1; i <= t; ++i)
        if (n % i == 0)
        {
            factor[++m] = i;
            if (i != n / i) factor[++m] = n / i;
        }
    //sort(factor + 1, factor + m + 1);
    //此步有可能是必要的，比如在某些 DP 中，转移要有顺序。
    return m;
}
```

以上方法称为试除法，通过试除法可以证明，一个整数 $n$ 的约数个数（记作 $d(n)$）上界为 $2\sqrt{n}$（实际上远远不到，经过试验能得到下表）。   

|范围|$d(n)$ 最大的 $n$|$\max\{d(n)\}$|
|:-:|:-:|:-:|
|$\leqslant 10^4$|数据丢失|$64$|
|$\leqslant 10^5$|$83160$|$128$|
|$\leqslant 10^6$|$720720$|$240$|
|$\leqslant 10^7$|$8648640$|$448$|
|$\leqslant 10^8$|$73513440$|$768$|
|$\le 10^9$|{% emoji 滑稽 %}|$1344$|
|$\le 10^{12}$|{% emoji 滑稽 %}|$6720$|
|$\le 10^{15}$|{% emoji 滑稽 %}|$26880$|
|64 位整数| ~~有这种数据吗~~ |$103680$|

“Pollard Rho” 是一个比“试除法”更高效的算法。难度大大超出了本文的范围，感兴趣的读者可以自行了解。

#### 1~n 每个数的正约数集合

求 $1\sim n$ 每个数的正约数集合可以采用倍数法。对于每个数 $d$，$d,2d,3d,\cdots,\lfloor \cfrac{n}{d}\rfloor\times d$ 的约数就有 $d$。代码如下：

```cpp
vector <int> factor[1000005];
for (int i = 1; i <= n; ++i)
    for (int j = i; j <= n; j += i)
        factor[j].push_back(i);
```

时间复杂度为 $O(\sum\limits_{i=1}^{n}\cfrac{n}{i})=O(n\log n)$，这一等式称之为**调和级数**。

### GCD 与 LCM

若 $c$ 同时是 $a$ 和 $b$ 的约数，则称 $c$ 是 $a$ 和 $b$ 的公约数。   
$a$ 和 $b$ 的最大公约数（Greatest Common Divisor，GCD）记为 $\gcd(a,b)$ ，或者在没有歧义的条件下记作 $(a,b)$。

若 $c$ 同时是 $a$ 和 $b$ 的倍数，则称 $c$ 是 $a$ 和 $b$ 的公倍数。   
$a$ 和 $b$ 的最小公倍数（Least Common Multiple，LCM）记为 $\text{lcm}(a,b)$ ，或者在没有歧义的条件下记作 $[a,b]$。

GCD 和 LCM 有一些显然的性质，如下：

- $(a,a)=(0,a)=a,$
- $\forall a\mid b, (a,b)=a,$
- $(a,b)=(a,a+b)=(a,ka+b) \Rightarrow (a,b)=(a,b\bmod a),$
- $(ka,kb)=k\times(a,b),$
- $(a,b,c)=((a,b),c),$
- $[a,b,c]=[[a,b],c],$
- $[a,b]=a\times b \div (a,b),$
- 如果 $d$ 是 $a,b$ 的公约数，那么 $d\mid (a,b).$

值得注意的是，若 $(a,b)=1$，则称 $a$ 和 $b$ 互质（互素），记作 $a\perp b$[^1]。

还有一些比较难证的家伙，请关注一下：

- 如果 $(n,x)=1$，那么 $(n,n-x)=1$，
- 若 $a\perp m$，则 $(m,ab)=(m,b)$，
- 若 $a\perp m,m\mid ab$，则 $m\mid b$，
- $[a,b](a,b)=ab$。

证明：

{% noteblock %}

采用反证法。如果存在 $(n,i)=1$ 且 $(n,n-i)=k(k\ne 1)$，那么 $(n-i)\bmod k = 0,n\bmod k = 0$，用后者减掉前者得 $i\bmod k = 0$，即 $k\mid i$，而 $k$ 又是是 $n$ 的因子，所以 $k$ 是 $n,i$ 的公约数，所以 $(n,i)\ne 1$，矛盾。

二三很难证，这里不证了（

第四个是这样的：$[a,b](a,b)=ab\div (a,b) \times (a,b)=ab$。

{% endnoteblock %}

完整的最大公约数理论及其不同的证明体系请参考《初等数论》。

### 求 GCD 与 LCM

求 GCD 有两种方法：辗转相除和更相减损。

#### 辗转相除法

刚才我们知道，$(a,b)=(a,b\bmod a)$，但注意**代码实现不能这么写**！    
比如给个 $(2,1)$，它就会一直是 $(2,1)$。

所以要这么写：

```cpp
int gcd(int x, int y)
{
    if (y == 0) return x;
    return gcd(y, x % y);
}
```

看见了吗？反过来就行了。  

那么它的复杂度是多少？

- 若 $a<b$，这时候 $(a,b)=(b,a)$；
- 若 $a\geq b$，这时候 $(a,b)=(b,a \bmod b)$，而对 $a$ 取模会让 $a$ 至少折半。这意味着这一过程最多进行 $\mathcal{O}(\log n)$ 次。

而不可能连续两次发生第一种情况。所以复杂度是 $\mathcal{O}(\log(a+b))$，但实际上远远达不到这个上界。当 $a=F_x,b=F_{x-1}$ 时（$F$ 指斐波那契数列），会跑满 $\mathcal{O}(\log(a+b))$，因为模运算会被卡成减法，也就成了更相减损法。

#### 更相减损法

还是根据性质，有：

```cpp
int gcd(int x, int y)
{
    if (y == 0) return x;
    return gcd(y, x - y);
}
```

它的复杂度是 $\mathcal{O}(\max(a,b))$，主要用于高精度运算中，因为模运算（除法）不好实现。

但比如这样：求 $\gcd(1,10^{10000})$ 用更相减损会很慢。除非数据有特殊性质，否则还是老老实实实现高精模运算。

#### 求解 LCM

根据定义，要这么写：

```cpp
inline int lcm(int x, int y)
{
    return x / gcd(x, y) * y;
}
```

注意顺序，因为 $x\times y$ 可能会溢出！注意这样的细节，毕竟算法竞赛不是数学竞赛。

### 质数（素数）

如果一个数 $x(x\in\mathbb{N})$ 的约数仅有 $1$ 和它本身，那么就称 $x$ 是质数（素数），特别地，$0$ 和 $1$ 不是质数，如果一个自然数不是质数，他就是合数。

**质数的判定**。试除法是最常用的判定质数方法，代码如下：

```cpp
inline bool is_prime(int x)
{
    if (x < 2) return false;
    for (int i = 2, t = min(x, sqrt(x) + 2); i < t; ++i)
        if (x % i == 0) return false;
    return true; 
}
```

用反证法可以证明这样做的正确性（即证明合数一定存在一个不超过 $\sqrt{x}$ 的约数）。

“Miller-Rabin”是一个进阶的素数随机判定方法，虽然是随机算法，但没有任何已知数字通过了高级概率性测试被判定为伪素数。感兴趣的读者可以自行了解。

为方便，接下来我们用 $p$ 代表质数。

### 计算技巧

这是一些编程中的计算技巧。

#### 快速幂

快速幂是一种分治法的应用，但这货在数论中有大量应用。这里直接给出模板。

```cpp
inline i64 mpow(i64 a, i64 p, i64 m)
{
    a %= p; // 如果 a 在 int 范围内，此步不必要
    i64 res = 1;
    while (p)
    {
        if (p & 1) res = res * a % m;
        a = a * a % m;
        p >>= 1;
    }
    return res % m; // 小心 m = 1
}
```

#### 快速乘

如果我们要求 64 位整数乘 64 位整数，显然 `long long` 是要炸掉的。那么用什么？`__int128`？你觉得毒瘤 CCF 不会使用 128 位整数乘法吗{% emoji 滑稽 %}（不用怕，不会的，使用 `__int128` 就行了，否则接下来的这种方法也不起作用了）。

一种解决方式是把快速幂中的乘法改成加法。但这样实在是太慢了，这里给出一种利用语言特性进行运算的方法，原理请读者自行学习，代码如下：

```cpp
using i64 = long long;
inline i64 mul(i64 a, i64 b, i64 p)
{
    a %= p, b %= p;
    i64 c = (long double)a * b / p;
    i64 ans = a * b - c * p;
    if (ans < 0) ans += p;
    else if (ans >= p) ans -= p;
    return ans;
}
```

考场还是能用 `__int128` 就用。

### 唯一分解定理

对于一个数 $n$，它可以唯一分解成 $n=\prod p_i^{k_i}$。

这玩意可以证明，我也不证了（这货还叫作算术基本定理，我怎么会证

这玩意的过程也叫做分解质因数。代码如下：

```cpp
int p[105], c[105], m;
void divide(int n)
{
    m = 0;
    int t = min(n, sqrt(n) + 1);
    for (int i = 2; i <= t; ++i)
        if (n % i == 0)
        {
            p[++m] = i, c[m] = 0;
            while (n % i == 0) n /= i, ++c[m];
        }
    if (n > 1) p[++m] = n, c[m] = 1; // 剩下的这个 n 可能是质数
}
```

{% noteblock guide green %}
在 `Linux` 终端下，可以使用 `factor` 命令快速分解质因数。命令如下：

```sh
factor 998244353
```

无误的话，会输出：

```sh
998244353: 998244353
```
{% endnoteblock %}

这货既然敢叫算术基本定理，那么必有过人之处。这里不加证明的给出算术基本定理的常用推论（证明参考《初等数论》）。

给定 $a=\prod_{i=1}^{s} p_i^{\alpha_i}$，

1. $d$ 是 $a$ 的约数的充要条件是 $d=\prod_{i=1}^s p_i^{e_i},e_i\le\alpha_i$，
2. $b=\prod_{i=1}^{s} p_i^{\beta_i}$，那么 $(a,b)=\prod_{i=1}^{s} p_i^{\min(\alpha_i,\beta_i)},[a,b]=\prod_{i=1}^{s} p_i^{\max(\alpha_i,\beta_i)}$，如果我们承认它成立，可以发现 $[a,b](a,b)=ab$ 的原因是 $\min(\alpha,\beta)+\max(\alpha,\beta) =\alpha+\beta$，直呼厉害{% emoji 真棒 %}，
3. 用除数函数 $\tau(a)$（读作 tau，也可记作 $d(a)$）表示 $a$ 的正约数的个数，则 $\tau(a)=\prod_{i=1}^s(\alpha_i+1)$，这也是**约数个数定理**。其实这是约数条件的推论，对于每个质因子上的幂次，我们取 $0\sim \alpha_i$ 的任意整数，共 $\alpha_i+1$ 个，由乘法原理直接得出，
4. 用除数和函数 $\sigma(a)$（读作 sigma）表示 $a$ 所有正约数的和，则 

$$
\sigma(a)=\prod_{i=1}^s\left(\sum_{j=0}^{\alpha_i}(p_i)^j\right)=\prod_{i=1}^s\sigma(p_i^{\alpha_i})
$$

### 数论函数

定义域为整数的函数称为数论函数。特别地，对于数论函数 $f$，如果 $\forall a\perp b, f(ab)=f(a)f(b)$，则称 $f$ 为**积性函数**；如果 $\forall a,b,f(ab)=f(a)f(b)$，则称 $f$ 为完全积性函数。

对于**所有**积性函数 $f$，有 $n=\prod p_i^{c_i}$，则 $f(n)=\prod f(p_i^{c_i})$。证明很简单，由于第二个式子中的所有 $p_i^{c_i}$ 都必定互质，多次利用积性函数的定义即可证明。

### 简单数论题

~~什么，为什么题目来的这么快。~~   
~~但是不用担心，这里都是一些基础题目，用介绍的知识和一小点小学数学就可以解决。~~{% emoji sticker 滑稽狂汗 %}

好，那就来吧[^3]！

#### 线性 GCD

> 给定 $n(n \le 10^7)$ 个整数，对于每个数 $A_i$，求出删除它以后剩下的数的最大公约数。

还记得 $(a,b,c)=((a,b),c)$ 吗？这个式子告诉我们，GCD 运算存在某种“结合律”，我们可以轻易地把两坨数的 GCD 拼起来。

我们有：

$$
Ans_i=(a_1,\cdots,a_{i-1},a_{i+1},\cdots,a_n)=((a_1,\cdots,a_{i-1}),(a_{i+1},\cdots,a_n))
$$

处理出前缀 GCD 和后缀 GCD，便可以快速求解了。

#### [NOIP2001 普及组] 最大公约数和最小公倍数问题

[Portal](https://www.luogu.com.cn/problem/P1029).

有 $p\mid y_0$，我们可以枚举 $y_0$ 的所有约数，然后利用 $(p,q)[p,q]=pq$ 判断是否满足条件即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int gcd(int x, int y) { return y == 0 ? x : gcd(y, x % y); }
int lcm(int x, int y) { return x / gcd(x, y) * y; }

int main(void) {
    int x, y, ans = 0;
    cin >> x >> y;
    for (int p = 1; p * p <= y; ++p) 
        if (y % p == 0) {
            if (gcd(p, y / p * x) == x) ++ans;
            if (y / p != p && gcd(y / p, p * x) == x) ++ans;
        }
    cout << ans << endl;
    return 0;
}
```
{% endfolding %}

#### [Luogu P1572] 计算分数

[Portal](https://www.luogu.com.cn/problem/P1572).

`gcd` 可以用于模拟分数运算。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <string>

#define i64 long long

using namespace std;

i64 gcd(i64 x, i64 y) { return y == 0 ? x : gcd(y, x % y); }
i64 lcm(i64 x, i64 y) { return x / gcd(x, y) * y; }

struct frac {
    i64 p, q;
    frac(i64 p = 0, i64 q = 0) :
        p(p), q(q) {}
    frac operator+ (const frac &a) const {
        frac res;
        res.q = lcm(q, a.q);
        res.p = res.q / q * p + res.q / a.q * a.p;
        i64 g = gcd(res.p, res.q);
        res.p /= g, res.q /= g;
        return res;
    }
};

int main(void) {
    i64 a, b;
    scanf("%lld/%lld", &a, &b);
    frac ans(a, b);
    while (~scanf("%lld/%lld", &a, &b))
        ans = ans + frac(a, b);
    if (ans.q < 0) ans.p = -ans.p, ans.q = -ans.q;
    if (ans.q == 1) printf("%lld\n", ans.p);
    else printf("%lld/%lld\n", ans.p, ans.q);
    return 0;
}
```
{% endfolding %}

#### [AHOI2005] 约数研究

[Portal](https://www.luogu.com.cn/problem/P1403).

换一种思路：枚举约数，然后加起来即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int main(void) {
    int n, sum = 0;
    cin >> n;
    for (int i = 1; i <= n; i++)
        sum += n / i;
    cout << sum << endl;
    return 0;
}
```
{% endfolding %}

#### [Luogu P2651] 添加括号III

[Portal](https://www.luogu.com.cn/problem/P2651).

不难发现除了 $a_2$，其它数都可以放在分子的位置上。然后直接约分，每次用最大公约数除，最后 $a_2=1$ 就是可以。

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
    scanf("%d", &T);
    while (T--) {
        int n, a, b;
        scanf("%d%d%d", &n, &a, &b);
        n -= 2;
        b /= gcd(a, b);
        while (n--) {
            scanf("%d", &a);
            b /= gcd(a, b);
        }
        if (b == 1) puts("Yes");
        else puts("No");
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P2660] zzc 种田

[Portal](https://www.luogu.com.cn/problem/P2660).

类似于一个 gcd 的迭代过程。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int main(void) {
    i64 a, b, ans = 0;
    cin >> a >> b;
    while (a && b) {
        if (a > b) swap(a, b); // 令 a < b
        ans += a * (b / a) * 4; // 边长为 a 的正方形，种 b / a 个
        b %= a; // 之后 b %= a
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

#### \[NOIP2009 提高组\] Hankson 的趣味题

[Portal](https://www.luogu.com.cn/problem/P1072).

由于 $[x,b_0]=b_1$，证明 $x$ 是 $b_1$ 的约数，用试除法枚举，判断是否满足条件即可。代码如下：

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int gcd(int x, int y) { return y == 0 ? x : gcd(y, x % y); }
inline int lcm(int x, int y) { return x / gcd(x, y) * y; }

int main(void)
{
    int T = read();
    while (T--)
    {
        int a0 = read(), a1 = read(), b0 = read(), b1 = read(), ans = 0;
        for (int i = 1; i * i <= b1; ++i)
            if (b1 % i == 0)
            {
                if (gcd(i, a0) == a1 && lcm(i, b0) == b1) ++ans;
                if (i != b1 / i)
                {
                    int j = b1 / i;
                    if (gcd(j, a0) == a1 && lcm(j, b0) == b1) ++ans;
                }
            }
        printf("%d\n", ans);
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P1414] 又是毕业季II

[Portal](https://www.luogu.com.cn/problem/P1414).

我们可以计算每个数的约数，然后放到一个数组里面统计。回答的时候从大到小看当前这个约数的数量够不够大。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

inline int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int n;
int c[1000005];

int main(void) {
    n = read();
    for (int i = 1; i <= n; ++i) {
        int x = read();
        int t = sqrt(x + 0.5);
        for (int j = 1; j <= t; ++j)
            if (x % j == 0) {
                ++c[j];
                if (j * j != x) ++c[x / j];
            }
    }
    for (int i = 1, j = 1000000; i <= n; ++i) {
        while (c[j] < i) --j;
        printf("%d\n", j);
    }
    return 0;
}
```
{% endfolding %}

#### [NOIP2013 提高组] 转圈游戏

[Portal](https://www.luogu.com.cn/problem/P1965).

根据题意使用快速幂计算即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

int poww(int a, int p, int m)
{
    int res = 1;
    while (p)
    {
        if (p & 1) res = (i64)res * a % m;
        a = (i64)a * a % m;
        p >>= 1;
    }
    return res % m;
}

int main(void)
{
    int n, m, k, x;
    scanf("%d%d%d%d", &n, &m, &k, &x);
    printf("%d\n", (x + (i64)m * poww(10, k, n)) % n);
    return 0;
}
```
{% endfolding %}

#### [NOIP2009 普及组] 细胞分裂

[Portal](https://www.luogu.com.cn/problem/P1069).

将 $m_1$ 分解质因数，然后每个次数乘上 $m_2$，便相当于 $m_1^{m_2}$。对于每一个 $s$，想要让 $m_1^{m_2} \mid s^x$，需要 $m$ 的每个质因子次数都小于 $s^x$ 的。那么依次检查每个质因子，要求 $ex\ge c$ 即可（$e$ 代表将 $s$ 分解质因数后的幂次，$c$ 是 $m$ 处理过后的幂次）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, m;
int m1, m2;
int p[10005], c[10005];

void init(void)
{
    for (int i = 2; i * i <= m1; ++i)
        if (m1 % i == 0)
        {
            p[++m] = i, c[m] = 0;
            while (m1 % i == 0)
            {
                ++c[m];
                m1 /= i;
            }
        }
    if (m1 > 1) p[++m] = m1, c[m] = 1;
    for (int i = 1; i <= m; ++i)
        c[i] *= m2;
}

int main(void)
{
    scanf("%d%d%d", &n, &m1, &m2);
    init();
    int ans = 2e9, s;
    while (n--)
    {
        scanf("%d", &s);
        int x = 0;
        for (int i = 1; i <= m; ++i)
        {
            if (s % p[i] != 0)
            {
                x = 2e9;
                break;
            }
            else
            {
                int res = 0;
                while (s % p[i] == 0)
                {
                    ++res;
                    s /= p[i];
                }
                x = max(x, c[i] / res + (c[i] % res == 0 ? 0 : 1));
            }
        }
        ans = min(ans, x);
    }
    if (ans != 2e9) printf("%d\n", ans);
    else puts("-1");
    return 0;
}
```
{% endfolding %}

#### [UVA11889] Benefit

[Portal](https://www.luogu.com.cn/problem/UVA11889).

当 $a\nmid c$ 的时候无解。否则要求满足下列条件的最小 $x$：

$$
\begin{aligned}
&\frac{x}{(a,x)}=\frac{c}{a}=d\\
\Longrightarrow & x=dk(k=(a,x))\\
\Longrightarrow & k=(a,dk)\\
\Longrightarrow & \frac{k}{g}=\left(\frac{a}{g},k\times\frac{d}{g}\right)\\
\Longrightarrow & \frac{k}{g}=\left(\frac{a}{g},k\right) \left(\frac{a}{g}\perp\frac{d}{g}\right) \\
\Longrightarrow & \frac{k}{(\frac{a}{g},k)}=g
\end{aligned}
$$

然后就可以递归求解了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int gcd(int x, int y)
{
    if (y == 0) return x;
    return gcd(y, x % y);
}

int calc(int a, int d) // b / gcd(a, b) = d
{
    int g = gcd(a, d);
    if (g == 1) return d;
    return d * calc(a / g, g);
}

int main(void)
{
    int T;
    scanf("%d", &T);
    while (T--)
    {
        int a, c;
        scanf("%d%d", &a, &c);
        if (c % a != 0) puts("NO SOLUTION");
        else printf("%d\n", calc(a, c / a));
    }   
    return 0;
}
```
{% endfolding %}

{% divider water %}

接下来我们会介绍一些较为有趣的数论算法。

## 素数筛法

素数筛能快速地求出区间内的质数。

[模板](https://www.luogu.com.cn/problem/P3383)。

### 普通筛

对于所有的数，标记所有它的倍数（二倍及以上）为合数。时间复杂度为调和级数 $\mathcal{O}(n\log n)$。代码如下：

```cpp
int prime[MAXN];
bool vis[MAXN];
for (int i = 2; i <= n; ++i)
{
    if (vis[i] == 0) prime[++tot] = i;
    for (int j = i * 2; j <= n; j += i)
        vis[j] = 1;
}
```

这种方式很慢，不要在考场上使用。

### Eratosthenes 筛法

Eratosthenes 筛法，简称埃氏筛，是算法竞赛中最常用的筛法。

考虑这样一件事情：如果 $x$ 是合数，那么 $x$ 的倍数也一定是合数，它们肯定会被某个相同的质因子筛掉，那我们就没有必要筛合数了。      
如果我们从小到大考虑每个数，然后同时把当前这个数的所有（比自己大的）倍数记为合数，那么运行结束的时候没有被标记的数就是素数了。

代码如下：

```cpp
int prime[MAXN];
bool vis[MAXN];
for (int i = 2; i <= n; ++i)
    if (vis[i] == 0)
    {
        prime[++tot] = i;
        for (int j = i * 2; j <= n; j += i)
            vis[j] = 1;
    }
```

埃氏筛的复杂度是 $\mathcal{O}(\sum_{p\leqslant n} \cfrac{n}{p}=n \log\log n)$，效率较高，但仍需较好的 IO 优化才可通过刚才的模板。

这个代码还可以加一些优化：内层循环不必从 $i\times 2$ 开始，它在筛 $2$ 的时候就已经被筛掉了；外层循环也不必到 $n$ 结束，需要筛的已经在 $\sqrt{n}$ 前筛掉了（但这道题中这个两个都不能用，因为分别会爆 `int` 和无法统计素数）。但实际上，优化后的复杂度并没有改变。

埃氏筛的速度已经足够快，如果不是时间特别吃紧，考场上使用埃氏筛即可。

刚才是“质数的整数倍是合数“，还有一种思路：整数的质数倍是合数。代码如下：

```cpp
for (int i = 2; i <= n; ++i) {
    if (!vis[i]) prime[++tot] = i;
    for (int j = 1; j <= tot && i * prime[j] <= n; ++j) 
        vis[i * prime[j]] = true;
}
```

### 欧拉筛

欧拉筛（也称作线性筛）的时间复杂度为 $\mathcal{O}(n)$，是常用筛法中最快的筛法。

埃氏筛的问题在于它仍然会重复标记合数（从 $x^2$ 开始），我们需要设计一个算法，使得合数只会被它最小的质因子筛一次，这就是欧拉筛（由于欧拉筛的这个特性，它也常被用做求合数最小质因子的方法）。

我们设一个 $v$ 数组代表每个数的最小质因子，设质数集合为 $p$，那么可以这样维护 $v$：

1. 一次考虑 $2\sim n$ 间的每一个数 $i$。
2. 如果 $v[i]$ 没有值，那么说明它是素数，把它加进 $p$ 中。
3. 接下来对于每个质数 $p[j]$，令 $v[i\times p[j]]=p[j]$。由于 $p[j] \le v[i]$，所以 $p[j]$ 一定是 $i\times p[j]$ 的最小质因子。

由于所有的合数只会被标记一次，时间复杂度为 $\mathcal{O}(n)$，严谨的复杂度和正确性证明较为复杂，感兴趣的读者可以自行了解。代码如下：

```cpp
int n, tot = 0;
int v[MAXN], prime[MAXN];
inline void primes(void)
{
    for (int i = 2; i <= n; ++i)
    {
        if (v[i] == 0) // 是质数
        {
            v[i] = i;
            prime[++tot] = i;
        }
        for (int j = 1; j <= tot && i * prime[j] <= n; ++j) // 给 i 乘上一个质数，乘积小于 n 才循环
        {
            if (prime[j] > v[i]) break; // i 的最小质因子比 prime[j] 更小，break
            v[i * prime[j]] = prime[j]; // 标记
        }
    }
}
```

实际上如果不用记录最小值因子的话，也可以这么做：

```cpp
for (int i = 2; i <= n; ++i) {
    if (!vis[i]) prime[++tot] = i;
    for (int j = 1; j <= tot && i * prime[j] <= n; ++j) {
        vis[i * prime[j]] = true;
        if (i % prime[j] == 0) break;
    }
}
```

只多了一个 `break`，就让埃氏筛变成了欧拉筛，每个数只被筛了一次。

### 性能比较

虽然时间复杂度上来看欧拉筛更为优秀，然而在实际上，欧拉筛不会快多少，除非数据范围到了 $\ge 10^7$，使用埃氏筛即可。在笔者的电脑上测试如下（性能弱于 i7-8700K，编译时开启 O2，C++14 标准，GCC 版本为 9.4.0，系统为 WSL2 Ubuntu）：

|数据范围|埃氏筛|欧拉筛|
|:-:|:-:|:-:|
|$n=10^6$|5.992ms|5.962ms|
|$n=2\times 10^6$|11.870ms|11.851ms|
|$n=5\times 10^6$|39.872ms|29.814ms|
|$n=10^7$|126.325ms|58.926ms|
|$n=2\times 10^7$|363.797ms|127.527ms|
|$n=5\times 10^7$|1037.448ms|329.183ms|
|$n=10^8$|2247.814ms|661.493ms|

### [Luogu P1835] 素数密度

[Portal](https://www.luogu.com.cn/problem/P1835).

> 给定区间 $[L,R](1\le L\le R < 2^{31},R-L\le 10^6)$，求区间中的素数个数。

$L,R$ 这么大，但是 $R-L$ 的值却这么小，而且任何一个合数 $n$ 都一定包含一个不超过 $\sqrt{n}$ 的质因子，那么我们只需要用筛法求出 $2\sim \sqrt{R}$ 以内的所有质数，然后筛掉这些数的倍数即可。代码如下：

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

#define LL long long

using namespace std;

LL L, R;
bool is_prime[50000];
bool flag[1000005];

inline void solve(int n)
{
    for (int i = 2; i <= n; ++i)
        is_prime[i] = 1;
    int g = sqrt(n) + 1;
    for (int i = 2; i <= g; ++i)
    {
        if (is_prime[i])
        {
            for (int j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }
}

int main(void)
{
    cin >> L >> R;
    if (L == 1)
        L = 2;
    if (L > R)
    {
        puts("0");
        return 0;
    }
    int m = sqrt(R) + 1;
    solve(m);

    for (int i = 0; i <= int(R - L); ++i)
        flag[i] = 1;
    for (int i = 2; i <= m; ++i)
        if (is_prime[i])
        {
            LL j = L;
            while (j % i != 0)
                ++j;
            for (; j <= R; j += i)
                if (i != j)
                    flag[j - L] = 0;
        }
    int ans = 0;
    for (int i = 0; i <= R - L; ++i)
        if (flag[i])
            ++ans;
    cout << ans << endl;
    return 0;
}
```
{% endfolding %}

### 筛法与积性函数

筛法的作用远不止筛出素数，它们还可以在筛素数的同时求出积性函数。在欧拉函数一节中，我们将看到如何用筛法求解欧拉函数；在《数论进阶》中，我们还会更深入地探讨这个问题。

## 扩展欧几里得算法

扩展欧几里得算法用于求解不定方程 $ax+by=\gcd(a,b)$ 的整数解，是数论基础算法之一。

### 裴蜀定理

**裴蜀定理**。$\forall a,b, ~\exists x,y,~\text{i.e.}~ ax+by=\gcd(a,b)$

关于裴蜀定理的证明，网上资料较多，这里不做赘述。

[模板](https://www.luogu.com.cn/problem/P4549)。

根据裴蜀定理，$ax+by=\gcd(a,b)$，也就是说 $ax+by>0$ 时，最小为 $\gcd(a,b)$，也就是说答案是 $\gcd(A)$，而且是一个正数。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int gcd(int x, int y)
{
    if (y == 0) return x;
    return gcd(y, x % y);
}

int main(void)
{
    int x, ans = 0;
    scanf("%d", &x);
    while (scanf("%d", &x) == 1) ans = gcd(ans, x < 0 ? -x : x);
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}

### 扩展欧几里得

由于 $ax+by=\gcd(a,b)=\gcd(b,a\bmod b)$ 且 $ax+by=(\lfloor\cfrac{a}{b}\rfloor\times b+a\bmod b)x+by=b(y+\lfloor\cfrac{a}{b}\rfloor x)+(a\bmod b)x$，所以求不定方程 $ax+by=\gcd(a,b)$，相当于求 $b(y+\lfloor\cfrac{a}{b}\rfloor x)+(a\bmod b)x=\gcd(b, a\bmod b)$。

当然这里把代码实现考虑了进去（还记得为什么求 gcd 要把 $x$ 和 $y$ 写反吗），所以新式子中的 $(y+\lfloor\cfrac{a}{b}\rfloor x)$ 相当于 $y'$，$x$ 相当于 $x'$。

进而有 $y = y' - \lfloor\cfrac{a}{b}\rfloor\times x'$，$x$ 是 $x'$。

所以：

```cpp
void exgcd(int a, int b, int &x, int &y) {
    if (b == 0) return x = 1, y = 0, void(); // 显然
    exgcd(b, a%b, y, x); // 根据刚才推出的结论
    // 此时已经求出了 x', y'
    y -= a / b * x;
}
```

那么如何求出方程的所有解呢？

设用 `exgcd` 求出的解为 $x_0,y_0$，那么显然有 $x=x_0+k\times\cfrac{b}{\gcd(a,b)},y=y_0-k\times\cfrac{a}{\gcd(a,b)}$。

最小正数 $x$ 显然为 $x_0 \bmod \cfrac{b}{\gcd(a,b)}$。

[模板](https://www.luogu.com.cn/problem/P5656)。

根据裴蜀定理，$ax+by$ 的答案只能是 $\gcd(a,b)$ 的倍数，也就是需要满足 $(a,b)\mid c$。直接使用 `exgcd` 算法，求出来的解，然后再乘上 $c\div (a,b)$，就能够得到一组原方程的解。

具体实现见代码：

```cpp
// 笔记原因，只摘录了重要代码
i64 exgcd(i64 a, i64 b, i64 &x, i64 &y) {
    if (b == 0) return x = 1, y = 0, a;
    i64 d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

int main(void) {
    i64 a = read(), b = read(), c = read(), x, y;
    i64 g = exgcd(a, b, x, y);
    if (c % g != 0) puts("-1");
    else {
        x *= c / g, y *= c / g;
        i64 p = b / g, q = a / g, k;
        if (x < 0) k = ceil((1.0 - x) / p), x += p * k, y -= q * k;
        else if (x >= 0) k = (x - 1) / p, x -= p * k, y += q * k;
        if (y > 0) {
            printf("%lld %lld %lld ", (y - 1) / q + 1, x, (y - 1) % q + 1);
            printf("%lld %lld\n", x + (y - 1) / q * p, y)
        }
        else printf("%lld %lld\n", x, y + q * (i64)ceil((1.0 - y) / q));
    }
}
```

## 欧拉函数

欧拉函数，即 $\varphi(n)$，代表 $1\sim n$ 中与 $n$ 互质的数的个数。

比如在 $1$~$12$ 中，$1,5,7,11$ 这四个数与 $12$ 互质，所以 $\varphi(12)=4$。

### 求解

怎么求欧拉函数呢？设 $n=\prod p_i^{k_i}$，则有：

$$\varphi(n)=n\prod(1-\cfrac{1}{p_i})=n\prod \cfrac{p_i-1}{p_i}$$

证明：

{% noteblock %}
设 $ans = n$。

设 $p$ 是 $n$ 的质因子，则要在 $ans$ 中减掉 $\cfrac{n}{p}$。   
$q$ 也是，则要在 $ans$ 中减掉 $\cfrac{n}{q}$。   
但是 $pq$ 的倍数被减了两遍，所以我们要找回它的冤魂，再加上 $\cfrac{n}{pq}$。

所以：

$$ans = n - \cfrac{n}{p} - \cfrac{n}{p} + \cfrac{n}{pq} = n\times(1-\cfrac{1}{p}-\cfrac{1}{q}-\cfrac{1}{pq})=n(1-\cfrac{1}{p})(1-\cfrac{1}{q})$$

实际上上述思想称之为容斥原理，根据容斥原理的公式（请自行了解），可以得到要证明的公式。
{% endnoteblock %}

根据结论有如下代码：

```cpp
inline int phi(int n) {
    int ans = n;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) {
            ans = ans / i * (i-1); // 因为是约数，所以能除
            while (n % i == 0) n /= i;
        }
    if (n > 1) ans = ans / n * (n-1);
    return ans;
}
```

时间复杂度为 $\mathcal{O}(\sqrt{n})$，当然你可以用 Pollard Rho 来加速，不过没必要。

### 性质

1. $\forall n > 1, 1\sim n$ 中与 $n$ 互质的数的和为 $n\times \varphi(n) \div 2$。且 $2\mid\varphi(n)(n>2)$。

证明：

{% noteblock %}

由于 $(n,x)=(n,n-x)$，所以与 $n$ 不互质的数 $x,n-x$ 成对出现，每一对的和为 $n \div 2$。进而易证原命题。由于成对出现，所以 $2\mid\varphi(n)(n> 2)，\varphi(1)=\varphi(2)=1$。

{% endnoteblock %}

2. 欧拉函数是积性函数。根据欧拉函数的计算式可以直接得到。那么，欧拉函数也满足积性函数的性质。进而可以推出，$\forall 2\nmid n, \varphi(2n)=\varphi(n)$，因为奇数 $n$ 显然与 $2$ 互质，且 $\varphi(2)=1$，所以 $\varphi(2n)=\varphi(2)\times \varphi(n)=1\times\varphi(n)=\varphi(n)$。

接下来的 $3,4$ 中，$p$ 均属于质数集。

3. $\varphi(p)=p-1$；
4. $\varphi(p^k)=p^{k}-p^{k-1}$；

证明：

{% noteblock %}

根据定义，性质 $3$ 显然。   
带入计算式中后运用乘法分配律，可以证明性质 $4$。

{% endnoteblock %}

5. 设 $m=m_1\times m_2$，若 $m_1$ 和 $m_2$ 有相同的素因数，那么 $\varphi(m)=m_2 \varphi(m_1)$；
6. $n = \sum_{d\mid n} \varphi(d)$.

这是两条很重要的性质。这里略去证明，请参考《初等数论》。

### 筛法求欧拉函数

我们先看如何使用埃氏筛求解欧拉函数。根据欧拉函数的计算式，我们先可以初始化 $\varphi(i)=i$，然后从 $2$ 开始扫描（根据计算式，而 $1$ 不是质数，不能扫）。如果扫到一个 $\varphi(i)=i$，那么说明这是一个质数，就扫描它的所有倍数，乘上 $(i-1) \div i$。代码如下：

```cpp
int n, phi[100005];
inline void euler(void)
{
    for (int i = 1; i <= n; ++i) phi[i] = i;
    for (int i = 2; i <= n; ++i)
        if (phi[i] == i)
        {
            for (int j = i; j <= n; j += i)
                phi[j] = phi[j] / i * (i - 1);
        }
}
```

我们还可以用欧拉筛来求解欧拉函数。在线性筛中，如果 $n$ 能被筛掉，那么它一定是被比它小的一个数 $n'$ 乘上一个质数 $p$ 筛掉。

当 $p \mid n'$ 时，有：

$$
\begin{aligned}
\varphi(n) &=  n\times \prod_{i=1} \cfrac{p_i-1}{p_i}\\ & 
= p \times n' \times \prod_{i=1} \cfrac{p_i-1}{p_i}\\ &
= p \times \varphi(n')
\end{aligned}
$$

当 $p \nmid n'$ 时，可得 $p\perp n'$（因为 $p$ 是质数），有：

$$
\begin{aligned}
\varphi(n) &= \varphi(p) \times \varphi(n') \\
&= (p-1) \times \varphi(n')
\end{aligned}
$$

所以代码如下：

```cpp
int v[maxn], prime[maxn], phi[maxn], tot = 0;
inline void euler(void) {
    phi[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (v[i] == 0) {
            v[i] = i, prime[++tot] = i;
            phi[i] = i - 1;
        }
        for (int j = 1; j <= tot && i * prime[j] <= n; ++j) {
            if (prime[j] > v[i]) break;
            v[i * prime[j]] = prime[j];
            phi[i * prime[j]] = phi[i] * (i % prime[j] == 0 ? prime[j] : prime[j] - 1);
        }
    }
}
```

当然不标记 `v` 也可以做：

```cpp
phi[1] = 1;
for (int i = 2; i <= n; ++i) {
    if (!phi[i]) prime[++tot] = i, phi[i] = i - 1;
    for (int j = 1; j <= tot && i * prime[j] <= n; ++j)
        if (i % prime[j] == 0) {
            phi[i * prime[j]] = phi[i] * prime[j];
            break;
        }
        else phi[i * prime[j]] = phi[i] * (prime[j] - 1);
}
```

## 同余

如果两数 $a,b$ 除以 $c$ 的余数相等，则称 $a,b$ 模 $c$ 同余，又称 $b$ 是 $a$ 对模 $c$ 的剩余，记作 $a\equiv b\pmod c$。

同余是基本数论中最难的部分。但不要害怕，如果欧拉函数那块学得非常明白，这部分的学习将异常轻松（实际上在《数论进阶》开始前，都算简单的{% emoji 滑稽 %}）。

### 基本概念

同余有一些基础的概念。

#### 性质

同余跟等式不一样，但是它也有跟等式相似的性质，也由于等式不相似的性质。包括（分栏是为了减小长度，但是可以看出有三类性质）：

{% tabs %}

<!-- tab 等价关系-->

**自反性**，即 $a\equiv a \pmod m$，      
**对称性**，即 $a\equiv b \iff b \equiv a \pmod m$，   
**传递性**，即 $a\equiv b \pmod m,~ b\equiv c \pmod m \Longrightarrow a\equiv c \pmod m$。

证明：

{% noteblock %}

自反性：$m\mid 0 \Longrightarrow m\mid a-a \Longrightarrow a\equiv a \pmod m$，   
对称性：$m\mid a-b \iff m\mid b-a$，    
传递性：$m\mid a-b,m\mid b-c \Longrightarrow m\mid (a-b)+(b-c) \Longrightarrow m\mid a-c \Longrightarrow a\equiv c \pmod m$。

{% endnoteblock %}

还有一条特殊的，$a\equiv b\pmod m$ 的充要条件是 $m\mid a-b \Longleftrightarrow -m \mid a-b$，所以说最初的同余式等价于 $a\equiv b \pmod {(-m)}$。
<!-- endtab -->

<!-- tab 与等式相似-->

同余式可以相加，即如果有 $a\equiv b \pmod m, c\equiv d \pmod m$，则 $a+c\equiv b+d \pmod m$。进而可以证明，如果 $a\equiv b \pmod m$，则 $a+c\equiv b+c \pmod m$。

同余式也可以相乘，即如果有 $a\equiv b \pmod m, c\equiv d \pmod m$，则 $ac\equiv bd \pmod m$。进而可以证明，如果 $a\equiv b \pmod m$，则 $ac\equiv bc \pmod m$。

同余式还有同幂性：$x\equiv y \pmod m \Longrightarrow x^n \equiv y^n \pmod m$。

证明：

{% noteblock %}
由 $m\mid a-b,m\mid c-d \Longrightarrow m\mid (a-b)+(c-d) \Longrightarrow m\mid (a+c)-(b+d)$，就可以证明同余式可以相加。加上同余式的自反性，就可以证明后面一个。

由 $a=b+k_1m,c=d+k_2m$，得 $ac=bd+(bk_2+dk_1+k_1k_2m)m$，进而推出 $ac\equiv bd \pmod m$。自反性带入后可证明后一个。
{% endnoteblock %}

同余式相加的这一性质告诉我们，同余式可以在加减意义上移项。

在这之后我们可以简单证明以下随时取模性质。

对于加减法，我们要证明 $a\bmod m + b \equiv a+b \pmod m$。    
用显然成立的 $a\bmod m \equiv a \pmod m$ 加上 $b\equiv b\pmod m$ 后得证。

对于乘法，要证明 $a\bmod m \times b \equiv ab \pmod m$。   
用显然成立的 $a\bmod m \equiv a\pmod m$ 乘上 $b\equiv b\pmod m$ 后得证。

<!-- endtab -->

<!-- tab 与等式不同-->

若 $a\equiv b \pmod m,d\ge 1, d\mid m$，则 $a\equiv b \pmod d$。这一点由 $d\mid m,m\mid a-b$ 利用整除的传递性得到。

$a\equiv b \pmod m \iff da \equiv db \pmod{|d| m}$，这由 $|d| m \mid da-db \iff m\mid a-b$ 推出。

$ac\equiv bc \pmod m \iff a\equiv b \pmod{m\div (c,m)}$，进而有当 $(c,m)=1$ 时，$ac\equiv bc \pmod m \iff a\equiv b \pmod{m}$。证明是将最初的同余式写作 $m\mid c(a-b)\iff \cfrac{m}{(c,m)}\mid\cfrac{c}{(c,m)}(a-b)$，由于 $\cfrac{m}{(c,m)}\perp\cfrac{c}{(c,m)}$（把公约数除光了当然互质了），加上最大公约数的推论（可以回去看看），这等价于 $\cfrac{m}{(c,m)}\mid(a-b)$，也就是要证的结论。
<!--endtab -->

{% endtabs %}

#### 同余方程

注意同余的这个式子 $a\equiv b\pmod c$，如果把它看做一个方程，那么这个式子叫做同余方程。

同余方程的特例——**横线性方程**是我们要讨论的内容。它长这样：$ax\equiv b\pmod m$。   
怎么解呢？不难得出，这个方程有解的充要条件是 $ax-b$ 是 $m$ 的倍数。设这个倍数为 $-y$ 倍，那么这个方程等价于 $ax-b=-ym$，即 $ax+my=b$。这是什么？扩展欧几里得算法！

根据裴蜀定理，$b$ 必须是 $\gcd(a,m)$ 的倍数，那么就调用 `exgcd(a,m,x,y)` 即可。

我们设求出来的东西满足 $ax_0+my_0=\gcd(a,m)$，那么 $x=x_0\times b\div \gcd(a,m)$ 就是原线性同余方程的一组解。方程的所有解则是所有模 $m\div (a,m)$ 与 $x$ 同余的整数（注意这一点！）。这一点跟当时解二元一次不定方程的原理一样，$a\left(x+k\times \cfrac{m}{(a,m)}\right)+m\left(y+k\times \cfrac{a}{(a,m)}\right)=b$

由于我们的代码实现的 $\gcd$ 只能对正数有意义，所以要将 $a$ 变为正数，而 $my$ 这一项怎么样无所谓，因为 $y$ 的符号是随便的。$b$ 也要变为相反数。

{% divider fence %}

[[Luogu 1516] 青蛙的约会](https://www.luogu.com.cn/problem/P1516)。

{% noteblock %}

两只青蛙在网上相识了，它们聊得很开心，于是觉得很有必要见一面。它们很高兴地发现它们住在同一条纬度线上，于是它们约定各自朝西跳，直到碰面为止。可是它们出发之前忘记了一件很重要的事情，既没有问清楚对方的特征，也没有约定见面的具体位置。不过青蛙们都是很乐观的，它们觉得只要一直朝着某个方向跳下去，总能碰到对方的。但是除非这两只青蛙在同一时间跳到同一点上，不然是永远都不可能碰面的。为了帮助这两只乐观的青蛙，你被要求写一个程序来判断这两只青蛙是否能够碰面，会在什么时候碰面。

我们把这两只青蛙分别叫做青蛙 A 和青蛙 B，并且规定纬度线上东经 $0$ 度处为原点，由东往西为正方向，单位长度 $1$ 米，这样我们就得到了一条首尾相接的数轴。设青蛙 A 的出发点坐标是 $x$，青蛙 B 的出发点坐标是 $y$。青蛙 A 一次能跳 $m$ 米，青蛙 B 一次能跳 $n$ 米，两只青蛙跳一次所花费的时间相同。纬度线总长 $L$ 米。现在要你求出它们跳了几次以后才会碰面。

{% endnoteblock %}

题目显然是让我们求同余方程 $x+km\equiv y+kn \pmod L$ 的解，移项（由于随时取模性质，同余式是可以移项的）可得 $k(m-n)\equiv y-x \pmod L$。那么令 $a=m-n,k=x,b=y-x$，就转化为了 $ax\equiv b\pmod L$。

也就相当于 $ax-b=nL\iff ax-nL=b$。

代码如下：

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

inline i64 read(void) {
    i64 x = 0; int c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

i64 exgcd(i64 a, i64 b, i64 &x, i64 &y) {
    if (b == 0) return x = 1, y = 0, a;
    int d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

int main(void) {
    i64 x = read(), y = read(), m = read(), n = read(), L = read();
    i64 a = m - n, b = y - x, c, d;
    if (a < 0) a = -a, b = -b;
    i64 g = exgcd(a, L, c, d);
    if (b % g == 0) {
        c = c * (b / g);
        i64 M = L / g;
        printf("%lld\n", (c % M + M) % M);
    }
    else puts("Impossible");
    return 0;
}
```

#### 同余类与剩余系

把全体整数分为若干个两两不相交的集合，使得在同一个集合中的任意两个数对模 $m$ 一定同余，而属于不同集合中的两个数模 $m$ 一定不同余。每一个这样的集合被称为模 $m$ 的**同余类**或模 $m$ 的**剩余类**。这个集合记作模 $m$ 的同余类 $\overline{a}$。显然，这个集合是 $\{a+km\}(k\in \mathbb{Z})$。

模 $m$ 的同余类一共有 $m$ 个，分别为 $\overline{0},\overline{1},\overline{2},\cdots,\overline{m-1}$。从每个同余类中取出一个数，它们所构成的集合称为模 $m$ 的**完全剩余系**。

如果一组数 $z_1,z_2,\cdots,z_t$ 满足 $(z_j,m)=1,1\leqslant j \leqslant t$，以及对于任意的 $a,(a,m)=1$，有且仅有一个 $z_j$ 满足 $a\equiv z_j \pmod m$，那么称这组数为模 $m$ 的**既约**（或**互素**）**剩余系**。也叫作**简化剩余系**。也可以给它换一个定义：$1\sim m$ 中与 $m$ 互质的数代表的同余类共有 $\varphi(m)$ 个，它们构成 $m$ 的**简化剩余系**。

有关于同余类和剩余系还有很多概念、性质与证明，请参考《初等数论》。同余类与剩余系是初等数论的基础。我们可以用它来证明欧拉函数的有关内容，还有需许许多多有趣的东西。

### Fermat-Euler 定理

$\forall a\perp n, a^{\varphi(n)}\equiv 1 \pmod n$
特别地，当 $n$ 为素数，有 $\forall a\perp n, a^n\equiv a \pmod n$。

通常把第一个同余式称为**欧拉定理**，第二个称为**费马小定理**。费马小定理还有一种写法：$a^{p-1}\equiv 1 \pmod p$。

费马小定理是欧拉定理的一个特殊情况。我们先假设欧拉定理成立，然后我们来证明费马小定理。

{% noteblock %}

我们先求证费马小定理的第二种写法。

因为 $p$ 是素数，所以 $\varphi(p)=p-1$。   
当 $a\perp p$ 时，费马小定理就是欧拉定理，显然成立。

我们把第二种写法转成熟悉的等式 $a^{p-1} \bmod p \equiv 1 \pmod p$（注，数学中没有这么写的，只是为了方便）。   
两边同乘 $a$，可以得到 $a^{p-1} \bmod p \times a \equiv a \pmod p$，彻底拆掉同余，有 $a^{p-1} \bmod p \times a\bmod p = a \bmod p$，又由于随时取模，可得 $a^{p-1} \times a \bmod p = a \bmod p$，即 $a^p\equiv a \pmod p$。

{% endnoteblock %}

现在我们只需要证明欧拉定理就可以证明以上所有定理。证明需要对同余类和剩余系进行分析（也有其它证法），这里就不证了{% emoji aru 84 %}。

{% divider music %}

但是你敢信吗，这玩意还能继续往下推{% emoji aru 132 %}。

这便是**扩展欧拉定理**，也就是 $\forall a,n,a^{2\varphi(n)}\equiv a^{\varphi(n)}\pmod n$，那么：

$$
a^{b}\equiv
\begin{cases}
a^{b\bmod\varphi(n)}, &a\perp n,\\
a^b, &(a,n)\neq 1,b\le\varphi(n),\\
a^{b\bmod\varphi(n)+\varphi(n)},  &(a,n)\neq 1,b>\varphi(n).
\end{cases}
\pmod n
$$

要注意，实际上即使在 $a,b$ 不互质的情况下，二式和三式也依然成立，但是这两个式子相互独立，也就是当 $b\le \varphi(n)$ 的时候，三式可能是错误的。

{% noteblock %}
许多题目要求我们把答案对一个数 $p$（不一定是质数，非质数就是加强难度），这时可以用扩展欧拉定理二式三式进行降幂，如果满足三式的条件，那么就把底数对 $p$ 取模，指数对 $\varphi(p)$ 取模后加上 $\varphi(p)$ 即可。
{% endnoteblock %}

[模板](https://www.luogu.com.cn/problem/P5091)。注意边读入边取模，代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

int a, m, b, phi;

inline void calcPhi(int n) {
    int ans = n, t = sqrt(n + 0.5);
    for (int i = 2; i <= t; ++i)
        if (n % i == 0) {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    if (n > 1) ans = ans / n * (n - 1);
    phi = ans;
}

inline int read(void) {
    int x = 0, c = getchar();
    bool flag = false;
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) {
        x = (x<<3) + (x<<1) + (c^48);
        if (x > phi) { // 大于才取模
            x %= phi;
            flag = true; // 需要加
        }
        c = getchar();
    }
    return x + (flag ? phi : 0);
}

inline int power(int x, int p) {
    int res = 1;
    while (p) {
        if (p & 1) res = (long long)res * x % m;
        x = (long long)x * x % m;
        p >>= 1;
    }
    return res;
}

int main(void) {
    scanf("%d%d", &a, &m);
    calcPhi(m);
    b = read();
    printf("%d\n", power(a, b));
    return 0;
}
```
{% endfolding %}

### 逆元

逆元，万物的起源，像出现了新的道具，游戏被开启了新的副本，数论从此向前迈进了一步，以前做不了的事情——除法取模，现在已经近在眼前（啊，不是[^2]

#### 引子

**模意义下的除法**。尽管我们会在《数论进阶》中介绍有理数取模（虽然不是什么鬼分数都能做{% emoji tong ... %}），我们先常规想一想。    
比如 $5\div 3 \times 12$，出现了除法，除法不满足随时取模性质，于是我们的程序就会死亡。但是后面乘上了 $12$，也就是这个式子可以当作 $5\times 4$，它又变成合法的了。   
然而我们不是上帝，做不到预判未来，只能给当下记个帐，于是，逆元诞生了。

---

也就是说，执行“除以 $x$”的时候，我们试着乘上另一个数（记账），等到日后乘上 $x$ 的时候，我们就能把数变回来，得到正确结果。也就是我们能找到一个满足 $k\equiv \cfrac{1}{x}\pmod p$，这个 $k$ 就是我们所求！

然后你会发现这就是有理数取余{% emoji 滑稽 %}。   
不过我们先不管这些{% emoji 滑稽 %}，我们按照正常的思维继续搞。

比如我们要找的 $\cfrac{1}{3}$ 的逆元，在 $\bmod 11$ 意义下的话，我们可以将 $\div 3$ 变成 $\times 4$，于是 $5\times 4 \times 12\equiv 9 \pmod{11}$，事实上，$5\div 3 \times 12=20, 20\bmod 11 = 9$。

我们记一个数的逆元为 $\text{inv}(x)$，显然，它满足：

$$
x\times \text{inv}(x) \equiv 1 \pmod p
$$

这个 $\text{inv}(x)$ 也可记作 $x^{-1}$（即 $\cfrac{1}{x}$）。

#### 求法

还记得费马小定理吗？当 $x\perp p$ 时，我们有 $x\times \text{inv}(x) \equiv 1 \pmod p$，和费马小定理 $x^{p-1}\equiv 1 \pmod p$，那么可以得到：

$$
x\times \text{inv}(x) \equiv x^{p-1} \pmod p
$$

两边同时除以 $x$（因为 $x\perp p$），得到：

$$
\text{inv}(x) \equiv x^{p-2} \pmod p
$$

这便是用费马小定理求逆元的方法，直接调用快速幂 `power(x, p-2, p)` 即可。

{% divider grass %}

然而你看那个东西不是同余方程吗？我们直接调用 `exgcd` 就能求出逆元啦（可以发现有解仅当 $x\perp p$）！扩展欧几里得求逆元往往比费马小定理快，而且在模数不是质数的情况下，如果有逆元也能求出来。

[模板](https://www.luogu.com.cn/problem/P1082)。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

inline int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int a, b;

void exgcd(int a, int b, int &x, int &y) {
    if (b == 0) return x = 1, y = 0, void();
    exgcd(b, a%b, y, x);
    y -= a / b * x;
}

int main(void) {
    a = read(), b = read();
    int x, y;
    exgcd(a, b, x, y); // x 是 a 在模 b 意义下的逆元
    printf("%d\n", (x % b + b) % b); 
    return 0;
}
```

由此也可看出，一个数往往有不只一个逆元，因为同余方程的解不止一个，不过我们只用那个最小的正的即可。

#### 线性求逆元

这个问题有两种形式，我们分开来看：

{% tabs %}

<!-- tab 递推逆元 -->

[模板](https://www.luogu.com.cn/problem/P3811)。

{% noteblock %}
求出 $1,2,\cdots,n$ 中每个数关于 $m$ 的逆元，且 $m$ 为质数。
{% endnoteblock %}

求 $i^{-1}$，这么干：

$$
k=\left\lfloor \cfrac{m}{i}\right\rfloor, j=m\bmod i, \\
\begin{aligned}
\therefore&~m=ki+j \\
\Longrightarrow& ki+j \equiv 0 \pmod m\\
\Longrightarrow& kj^{-1}+i^{-1}\equiv 0 \pmod m （两边同乘~i^{-1}\times j^{-1}）\\
\Longrightarrow& i^{-1}\equiv -kj^{-1} \pmod m\\
\Longrightarrow& i^{-1}\equiv -kj^{-1} \pmod m\\
\Longrightarrow& i^{-1}\equiv -{\left\lfloor \cfrac{m}{i}\right\rfloor}\left(m\bmod i\right)^{-1} \pmod m
\end{aligned}
$$

这便是逆元的递推公式了，然而这样得出的逆元是个负数{% emoji tong ... %}，所以我们要给 $-\left\lfloor \cfrac{m}{i}\right\rfloor$ 加上 $m$，模板题代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, p;
int inv[3000005];

int main(void)
{
    scanf("%d%d", &n, &p);
    inv[1] = 1; // inv[1] = 1
    for (int i = 2; i <= n; ++i)
        inv[i] = (long long)(p - p / i) * inv[p % i] % p;
    for (int i = 1; i <= n; ++i)
        printf("%d\n", inv[i]);
    return 0;
}
```
{% endfolding %}
<!-- endtab -->

<!-- tab 任意逆元 -->
[模板](https://www.luogu.com.cn/problem/P5431)。

{% noteblock %}
给定 $n(1\le a_i < p)$ 个正整数，求它们模 $p$ 意义下的逆元。
{% endnoteblock %}

计算前缀积 $s$（模意义），然后计算 $s[n]$ 的逆元 $sinv[n]$。由于 $sinv[n]$ 是 $n$ 个数积的逆元，也就是它等于 $\cfrac{1}{a_1\cdots a_n}$，再乘上前缀和后缀就可以得到当前的逆元了。

时间复杂度 $\mathcal{O}(n+\log p)$，代码如下：

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

int n, p, k;
int a[5000005], sp[5000005], sf[5000005], inv;

int poww(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = 1ll * res * a % p;
        a = 1ll * a * a % p;
        b >>= 1;
    }
    return res;
}

int main(void) {
    n = read(), p = read(), k = read();
    sp[0] = 1, sf[n + 1] = 1;
    for (int i = 1; i <= n; ++i) a[i] = read();
    for (int i = 1; i <= n; ++i) sp[i] = 1ll * sp[i - 1] * a[i] % p;
    for (int i = n; i >= 1; --i) sf[i] = 1ll * sf[i + 1] * a[i] % p;
    inv = poww(sp[n], p - 2);
    int tmp = 1, ans = 0;
    for (int i = 1; i <= n; ++i)
        ans = (ans + 1ll * inv * sp[i - 1] % p * sf[i + 1] % p * (tmp = 1ll * tmp * k % p)) % p;
    printf("%d\n", ans);
    return 0;
}
```
{% endfolding %}
<!-- endtab -->

{% endtabs %}

### 中国剩余定理（CRT）

这起源于一个叫做韩信点兵的故事。相传韩信点他数量巨大的兵，然后实在是数不过来，于是就让每三个组成一队看余数，每五个组成一队看余数，每七个组成一队看余数，然后就算出来总人数了。

[模板](https://www.luogu.com.cn/problem/P1495)。

也就是说，要解决横线性同余方程组，保证模数两两互质：

$$
\begin{cases}
x\equiv a_1 \pmod {m_1}\\
x\equiv a_2 \pmod {m_2}\\
\cdots\\
x\equiv a_3 \pmod {m_3}\\
\end{cases}
$$

设 $M=\prod_{i=1}^{n}m_i,M_i=m\div m_i$，$t_i$ 是线性同余方程 $M_it_i\equiv 1 \pmod{m_i}$ 的一个解，也就是说 $t_i$ 是 $M_i$ 模 $m_i$ 的逆元（显然 $M_i \perp m_i$ 当且仅当 $m_i$ 两两互质），那么 $x=\sum_{i=1}^{n}a_iM_it_i + kM$，最小非负整数解需要求 $\sum_{i=1}^{n}a_iM_it_i\bmod M$。

为什么这是正确的呢？由于 $M_it_i\equiv 1 \pmod{m_i}$，所以 $a_iM_it_i \equiv a_i \pmod{m_i}$。   
而且 $M_j=\prod_{i\ne j}m_i$，也就是说对于 $\forall i\ne j, m_i\mid M_j$，所以 $a_jt_jM_j\equiv 0\pmod{m_i}$。

综上所述，可以推断出 $x\equiv \sum_{j=1}^{n} a_{j} M_{j} t_{j} + kM \equiv a_i+\sum_{j=1,j\ne i}^{n} 0 +0\equiv a_{i} \pmod{m_i}$。

我们一个一个的来解释。$x\equiv \sum_{j=1}^{n} a_{j} M_{j} t_{j} + kM\pmod{m_i}$ 是显然的，因为这连个玩意相等；$\sum_{j=1}^{n} a_{j} M_{j} t_{j} + kM \equiv a_i+\sum_{j=1,j\ne i}^{n} 0 +0\pmod{m_i}$，这是因为除了 $j=i$ 以外，$a_jt_jM_j\equiv 0\pmod{m_i}$，而当 $j=i$ 时，$a_iM_it_i \equiv a_i \pmod{m_i}$，而且显然 $kM\equiv 0\pmod{m_i}$，进而这个 $a_i+\sum_{j=1,j\ne i}^{n} 0 +0=a_i$。

```cpp
#include <iostream>
#include <cstdio>
#define i64 long long

using namespace std;

void exgcd(i64 a, i64 b, i64 &x, i64 &y) {
    if (b == 0) return x = 1, y = 0, void();
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}

int n;
i64 mm = 1;
i64 m[15], a[15], M[15], t[15];

i64 CRT(void) {
    i64 ans = 0, x;
    for (int i = 1; i <= n; ++i) {
        M[i] = mm / m[i];
        exgcd(M[i], m[i], t[i], x); // t[i] 为 M[i] 模 m[i] 的逆元
        ans = (ans + a[i] * M[i] * t[i]) % mm;
    }
    return (ans % mm + mm) % mm;
}

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d", m + i, a + i);
        mm *= m[i];
    }
    printf("%lld\n", CRT());
    return 0;
}
```

### exCRT

之前的模数是两两互质的，但如果是任意的怎么办？[模板](https://www.luogu.com.cn/problem/P4777)。

由于 CRT 算法中 $M_i$ 这一奇怪的存在，导致模数任意的时候逆元不存在，也就是说 CRT 的一切都已经不再使用，我们需要设计一种新的算法。

思路大概是这样的：我们需要找到一种方式，能将两个同余方程进行合并。形式化地：

$$
\begin{cases}
a\equiv r_1 \pmod{m_1}\\
a\equiv r_2 \pmod{m_2}
\end{cases}
$$

我们先假定一定可以合并，然后看看什么时候合并之后的解是 $\varnothing$。

这玩意儿等价于 $a=k_1m_1+r_1=k_2+m_2+r_2 \Longrightarrow k_1m_1-k_2m_2=r_2-r_1$。这个熟悉！二元一次不定方程！直接使用裴蜀定理和 `exgcd` 计算即可。

于是：

- 如果 $\gcd(m_1,m_2)\mid (r_2-r_1)$，那么可以合并；
- 否则，exCRT 是无解的。

现在有引理：合并之后的模数是原来两个模数的 `lcm`。这里不做证明。所以要注意，**需要保证所有数的最小公倍数是可以存的下的，否则不可以使用 exCRT**！

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

i64 exgcd(i64 a, i64 b, i64 &x, i64 &y) {
    if (b == 0) return x = 1, y = 0, a;
    i64 g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

int n;
i64 A[1000005], B[1000005];

i64 exCRT(void) {
    i64 M = B[1], ans = A[1]; // M 为当前合并的余数，ans 为当前答案
    for (int i = 2; i <= n; ++i) {
        i64 a = M, b = B[i], x, y, c = (A[i] - ans + b) % b;
        i64 g = exgcd(a, b, x, y);
        if (c % g != 0) return -1;

        x = (__int128)x * (c / g) % (B[i] / g);
        ans = ans + M * x;

        M = B[i] / g * M;
        ans = (ans % M + M) % M;
    }
    return ans;
}

int main(void) {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%lld%lld", B + i, A + i);
    printf("%lld\n", exCRT());
    return 0;
}
```

## Problemset

至此，基础数论的知识已经全部介绍完毕。理论上这些内容已能解决 OI 中大部分 NOIp 级别以内的数论题目（一些较难的省选级别的知识点的请参考《数论进阶》）。接下来我们看一些基础数论的题目：

### 简单数学

这些数学问题都是基础知识的简单应用。包括各种基础知识和素数筛法。

#### [UVA10780] Again Prime? No Time.

[Portal](https://www.luogu.com.cn/problem/UVA10780).

使用唯一分解定理来进行判断即可，需要预处理出素数。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int prime[10005], tot = 0;
bool v[10005];
void GetPrime(void)
{
    for (int i = 2; i <= 10000; ++i)
        if (!v[i])
        {
            prime[++tot] = i;
            for (int j = i * 2; j <= 10000; j += i)
                v[j] = true;
        }
}

int A[10005], B[10005];

int main(void)
{
    GetPrime();
    int T, n, m;
    scanf("%d", &T);
    for (int kase = 1; kase <= T; ++kase)
    {
        scanf("%d%d", &m, &n);
        printf("Case %d:\n", kase);
        memset(A, 0, sizeof(A));
        memset(B, 0, sizeof(B));
        for (int i = 1; i <= tot; ++i)
        {
            for (int j = prime[i]; j <= n; j *= prime[i])
                A[i] += n / j;
        }
        for (int i = 1; i <= tot; ++i)
            while (m % prime[i] == 0)
            {
                m /= prime[i];
                ++B[i];
            }
        bool flag = true;
        int k = 2e9;
        for (int i = 1; i <= tot; ++i)
        {
            if (A[i] < B[i])
            {
                flag = false;
                break;
            }
            else if (B[i] != 0) k = min(k, A[i] / B[i]);
        }
        if (!flag) puts("Impossible to divide");
        else printf("%d\n", k);
    }
    return 0;
}
```
{% endfolding %}

#### [UVA10892] LCM Cardinality

[Portal](https://www.luogu.com.cn/problem/UVA10892).

唯一分解定理！将 $n$ 进行质因数分解，然后利用唯一分解定理中最小公倍数的性质，让其中一个数等于分解后的指数，另一个就可以随便取了。根据乘法原理答案就是 $\prod c_i\times 2 + 1$，但是这样会算重复，需要除以二在加上一（有一组没有重复）。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
typedef long long i64;

int main(void)
{
    int n, kase = 0;
    while (scanf("%d", &n) == 1 && n)
    {
        static int C[2005];
        memset(C, 0, sizeof(C));
        int tot = 0, nn = n;
        for (int i = 2; i * i <= n; ++i)
            if (n % i == 0)
            {
                ++tot;
                while (n % i == 0) n /= i, C[tot]++;
            }
        if (n > 1) C[++tot] = 1;
        i64 ans = 1;
        for (int i = 1; i <= tot; ++i)
            ans *= C[i] * 2 + 1;
        printf("%d %lld\n", nn, ans / 2 + 1);
    }
    return 0;
}
```
{% endfolding %}

#### [UVA11752] The Super Powers

[Portal](https://www.luogu.com.cn/problem/UVA11752).

可以发现一定是某个数的合数次方幂，那么就可以直接枚举了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;
typedef unsigned long long u64;
const u64 MAXN = 18446744073709551615ull;

int vis[70000];
vector <u64> v;

int main(void)
{
	for (int i = 2; i * i <= 69000; ++i)
		if (!vis[i])
		{
			for (int j = i * 2; j <= 69000; j += i)
				vis[j] = true;
		}
	
	v.push_back(1);
	
	for (int i = 2; i < 65536; ++i)
	{
		u64 tmp = 1ull * i * i * i * i;
		for (int j = 4;; ++j)
		{
			if (vis[j]) v.push_back(tmp);
			if (tmp <= MAXN / i) tmp = tmp * i;
			else break;
		}
	}
	
	sort(v.begin(), v.end());
	int m = unique(v.begin(), v.end()) - v.begin();
	for (int i = 0; i < m; ++i)
		printf("%llu\n", v[i]);
	return 0;
}
```
{% endfolding %}

#### [HAOI2007] 反素数

[Portal](https://www.luogu.com.cn/problem/P1463).

我们可以知道，$1\sim n$ 中约数最多的数中最小的一个 $m$，便是 $1\sim n$ 中最大的反素数。为什么呢？根据 $m$ 的定义，显然有 $\forall x<m, g(x)<g(m); \forall x>m,g(x)\le g(m)$，前者说明 $m$ 是反素数，而后者说明大于大于 $m$ 的数都不是反素数。   
计算一下可以知道，$1\sim n$ 的任何数的不同质因子不会超过 $10$ 个，而且它们的质数不超过 $30$。   
反素数分解质因数后，质因子的指数一定是单调递减的。这一点可以用反证法证明，只需要换一个质因子，就可以得到一个更小但是约数个数相等的数。

这时搜索树的规模已经降到足够小，直接 DFS 即可。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

const int PRIME[] = {0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
const i64 INF = 0x7fffffff;

i64 n, ans;
int c[15];
int ret;

void dfs(int o, i64 res, int cnt)
{
    if (o > 10)
    {
        if (cnt > ret || (cnt == ret && res < ans))
        {
            ans = res;
            ret = cnt;
        }
        return;
    }
    i64 res0 = res;
    for (int i = 0; i <= c[o - 1]; ++i, res0 *= PRIME[o])
    {
        if (res0 > n) return;
        c[o] = i;
        dfs(o + 1, res0, cnt * (i + 1)); // 按照乘法原理计算约数个数
    }
}

int main(void)
{
    scanf("%lld", &n);
    c[0] = INF;
    dfs(1, 1, 1);
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

### 小专题锦集

包括筛法，exgcd 等。

#### [NOI2002] 荒岛野人

[Portal](https://www.luogu.com.cn/problem/P2421).

题目的这个条件是什么意思？形象化地，就是对于任意两个野人，使得同余方程：

$$
C_i+xP_i\equiv C_j+xP_j \pmod M
$$

没有解，或者不存在一个 $x\le L_i,x\le L_j$ 的解。这种问题已经很熟悉了，就是要转化为 `exgcd` 可以解决的形式，然后求出最小的 $x$，来看是否有解。

移项得 $(P_i-P_j)x\equiv C_j-C_i \pmod M$，等价于 $(P_i-P_j)x-(C_j-C_i)=nM$，也就是 $(P_i-P_j)x-nM=C_j-C_i$，使用 `exgcd` 计算即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

#define ERROR_WRONG_DATA -1

using namespace std;

int exgcd(int a, int b, int &x, int &y)
{
    if (b == 0)
    {
        x = 1, y = 0;
        return a;
    }
    int g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

int n;
struct Pockets
{
    int c, p, l;
    bool operator < (const Pockets &a) const { return c < a.c; }
    void summer_init(void) { scanf("%d%d%d", &c, &p, &l); }
} A[20];

bool check(int m)
{
    for (int i = 1; i < n; ++i)
        for (int j = i + 1; j <= n; ++j)
        {
            int a = A[i].p - A[j].p, b = m, c = A[j].c - A[i].c, x, y;
            if (a < 0) a = -a, c = -c;
            int d = exgcd(a, b, x, y);
            if (c % d == 0)
            {
                int M = m / d;
                x = x * (c / d);
                x = (x % M + M) % M;
                if (x <= A[i].l && x <= A[j].l) return false;
            }
        }
    return true;
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        A[i].summer_init();
    sort(A + 1, A + n + 1);
    for (int m = A[n].c; m <= 1000000; ++m)
        if (check(m))
        {
            printf("%d\n", m);
            return 0;
        }
    return ERROR_WRONG_DATA;
}
```
{% endfolding %}

#### [Codefroces 449C] Jzzhu and Apples

[Portal](https://codeforces.com/problemset/problem/449/C).

从大到小检查所有质数，有能配对就配，奇数个就扔掉 $2x$ 给 $2$ 用。这样最后至多只有 $1$ 个没有配对。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>

using namespace std;

int n;
bool v[100005];
int p[50005], tot = 0;

int main(void) 
{
	scanf("%d", &n);
	for (int i = 2; i <= (n >> 1); ++i) {
		if (!v[i]) p[++tot] = i;
		for (int j = 1; j <= tot && i * p[j] <= n; ++j) {
			v[i * p[j]] = true;
			if (i % p[j] == 0) break;
		}
	}
	memset(v, 0, sizeof(v));
	vector<pair<int, int> > ans;
	for (int i = tot; i >= 1; --i) {
		vector<int> a;
		for (int j = p[i]; j <= n; j += p[i])
			if (!v[j]) a.push_back(j); 
		if (a.size() & 1) {
			swap(a[1], a[a.size() - 1]);
			a.pop_back();
		}
		for (int j = 0; j < a.size(); j += 2) {
			v[a[j]] = v[a[j + 1]] = true;
			ans.push_back({a[j], a[j + 1]}); 
		}
	}
	printf("%d\n", ans.size());
	for (auto i : ans)
		printf("%d %d\n", i.first, i.second);
	return 0;
}
```
{% endfolding %}

### 欧拉函数

欧拉函数的应用很多。

#### [SDOI2008] 仪仗队

[Portal](https://www.luogu.com.cn/problem/P2158)。

除了 $(1,2),(2,1),(2,2)$ 三个人外，一个倒霉鬼 $(x,y)$ 能被看到，当且仅当 $1\le x,y\le n, x\neq y, \gcd(x,y)=1$。   
由于我们不需要考虑 $x=y$ 的情况，所以可以以直线 $x=y$ 把队伍分成两半来做，其中一半的答案的二倍再加上抛掉的那三个就是最终的答案。   
对于每个 $2\le y\le n$，我们只需要求出有多少个 $x$ 满足 $1\le x < y \wedge (x,y)=1$，相当于 $1\le x \le y$，那么这就是 $\varphi(y)$。

所以这道题的答案就是 $3+2\times \sum\limits_{i=2}^{n-1} \varphi(i)$，用筛法计算即可。

{% folding cyan, 查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, ans = 3;
int phi[40005];

int main(void) {
    scanf("%d", &n);
    if (n == 1) {
        puts("0");
        return 0;
    }
    for (int i = 1; i < n; ++i) phi[i] = i;
    for (int i = 2; i < n; ++i)
        if (phi[i] == i) {
            for (int j = i; j < n; j += i)
                phi[j] = phi[j] / i * (i - 1);
        }
    for (int i = 2; i < n; ++i)
        ans += (phi[i] << 1);
    printf("%d\n", ans);
    return 0; 
}
```
{% endfolding %}

#### [SDOI2008] 沙拉公主的困惑

[Portal](https://www.luogu.com.cn/problem/P2155).

> 多次询问 $[1,n!]$ 中与 $m!$ 互质的数的个数。

如果说范围是 $M!$ 呢？那么答案显然就是 $\varphi(M!)$。

由于 $(a,b)=a(a, ka+b)$，也就是说若 $(M!,a)=1$，那么 $(M!,a+k\times M!)=1$，而且 $M\le N\Longrightarrow M!\mid N!$，所以 $\varphi(m!)$ 这玩意的出现次数是每过 $M!$ 就会循环的，而且完整循环！循环次数是 $N!\div M!$ 次。

答案就是 $N! \div M! \times \varphi(M!)$，下面考虑如何求解这个玩意。

$$
\begin{aligned}
ans &=N! \div M! \times \varphi(M!)\\
&=N! \div M! \times M! \prod_{p_i\mid M!} \cfrac{p_i-1}{p_i}\\
&=N! \prod_{p_i\mid M!} \cfrac{p_i-1}{p_i}
\end{aligned}
$$

$N!$ 可以预处理，后面这个东西也可以用线性筛预处理，但是要分子分母分开算，否则约分会出错。分母直接拿逆元来算就可以了。

当 `N / R > M / R` 的时候答案为 $0$，因为显然 $N$ 中的 $R$ 根本无法约干净，取模后答案就成 $0$ 了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#define i64 long long

using namespace std;
const int MAXN = 10000000;

int T, R;
int N, M;
bool v[MAXN + 5];
int prime[MAXN], tot;
int inv[MAXN + 5], f[MAXN + 5], g[MAXN + 5], h[MAXN + 5];

int main(void)
{
    scanf("%d%d", &T, &R);
    inv[1] = 1;
    for (int i = 2; i <= MAXN; ++i)
        inv[i] = (i64)(R - R / i) * inv[R % i] % R;
    f[0] = 1;
    for (int i = 1; i <= MAXN; ++i)
        if (i % R != 0) f[i] = (i64)f[i - 1] * i % R;
        else f[i] = f[i - 1];
    g[0] = g[1] = h[0] = h[1] = 1;
    for (int i = 2; i <= MAXN; ++i)
    {
        if (!v[i]) 
        {
            g[i] = (i64)g[i - 1] * (i - 1) % R;
            h[i] = (i64)h[i - 1] * (i % R != 0 ? inv[i] : 1) % R;
            prime[++tot] = i;
        }
        else g[i] = g[i - 1], h[i] = h[i - 1];
        for (int j = 1; j <= tot && i * prime[j] <= MAXN; ++j)
        {
            v[i * prime[j]] = true;
            if (i % prime[j] == 0) break;
        }
    }
    while (T--)
    {
        scanf("%d%d", &N, &M);
        if (N / R > M / R) puts("0");
        else printf("%lld\n", (i64)f[N] * g[M] % R * h[M] % R);
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P2568] GCD

[Portal](https://www.luogu.com.cn/problem/P2568).

> 给定正整数 $n$，求 $1\le x,y\le n$ 且 $\gcd(x,y)$ 为素数的数对 $(x,y)$ 有多少对。

我们枚举 $(x,y)=p$，现在要统计这个东西。

设 $x=px',y=py'$，则 $x\perp y,x,y\le \frac{n}{p}$，不妨设 $x'\le y'$，当 $y'=k$ 时，$x'$ 的个数就是 $\varphi(k)$。因此个数就是 $2\times \sum_{i=1}^{n/p}\varphi(i)-1$。那么线性筛处理欧拉函数前缀和即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;
const int N = 1e7;
typedef long long i64;

int n, tot = 0, p[N + 5];
int phi[N + 5];
i64 s[N + 5];

int main(void) {
    int n;
    scanf("%d", &n);
    phi[1] = s[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!phi[i]) phi[i] = i - 1, p[++tot] = i;
        for (int j = 1; j <= tot && i * p[j] <= n; ++j) {
            if (i % p[j] == 0) {
                phi[i * p[j]] = phi[i] * p[j];
                break;
            } else phi[i * p[j]] = phi[i] * (p[j] - 1);
        }
        s[i] = s[i - 1] + phi[i];
    }
    i64 ans = 0;
    for (int i = 1; i <= tot; ++i)
        ans += 2 * s[n / p[i]] - 1;
    printf("%lld\n", ans);
    return 0;
}
```
{% endfolding %}

### 扩展欧拉定理

扩展欧拉定理可以用来降幂。

#### [Luogu P4139] 上帝与集合的正确用法

[Portal](https://www.luogu.com.cn/problem/P4139).

由于保证了这玩意是个定值，而且幂次数是无限大的，所以我们可以使用扩展欧拉定理的三式进行降幂，用递归函数来计算即可。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#define i64 long long

using namespace std;
const int MAXN = 10000000;

int phi[MAXN + 5];

void REFLECTION_BLUE(void)
{
    for (int i = 1; i <= MAXN; ++i) phi[i] = i;
    for (int i = 2; i <= MAXN; ++i)
        if (phi[i] == i)
        {
            for (int j = i; j <= MAXN; j += i)
                phi[j] = phi[j] / i * (i - 1);
        }   
}

int poww(int a, int b, int p)
{
    int res = 1;
    while (b)
    {
        if (b & 1) res = (i64)res * a % p;
        a = (i64)a * a % p;
        b >>= 1;
    }
    return res % p;
}

int pockets(int p) // 求 2^2^...^2 mod p
{
    if (p == 1) return 1;
    return poww(2, pockets(phi[p]) + phi[p], p); // 2^2^..^2 % phi[p] + phi[p] 次方
}

int main(void)
{
    REFLECTION_BLUE();
    int T;
    scanf("%d", &T);
    while (T--)
    {
        int p;
        scanf("%d", &p);
        printf("%d\n", pockets(p) % p);
    }
    return 0;
}
```
{% endfolding %}

#### [Ynoi2016] 炸脖龙 I

[Portal](https://www.luogu.com.cn/problem/P3934).

这道题同样是使用扩展欧拉定理进行降幂，注意快速幂计算和取余运算的时候**每一步**都需要按照扩展欧拉定理的方式来进行，也就是需要写一个新的取模函数。对于区间修改，使用一个支持“区间修改，单点查询“的树状数组即可解决。由于数较大的地方可能会爆炸，请使用 `__int128`，这基本上是一个模板，推荐仔细阅读代码。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#define i64 long long

using namespace std;
constexpr int MAXP = 20000000;
constexpr int MAXN = 500000;

int phi[MAXP + 5];
int v[MAXP + 5], prime[MAXP + 5], tot;

inline void REFLECTION_BLUE(void)
{
    phi[1] = 1;
    for (i64 i = 2; i <= MAXP; ++i)
    {
        if (v[i] == 0)
        {
            v[i] = i, prime[++tot] = i;
            phi[i] = i - 1;
        }
        for (int j = 1; j <= tot && i * prime[j] <= MAXP; ++j)
        {
            if (prime[j] > v[i]) break;
            v[i * prime[j]] = prime[j];
            phi[i * prime[j]] = phi[i] * (i % prime[j] == 0 ? prime[j] : prime[j] - 1);
        }
    }
}

int n, m;
i64 A[MAXN + 5], C[MAXN + 5];

inline void update(int x, int k)
{
    while (x <= n)
    {
        C[x] += k;
        x += (x & -x);
    }
}

inline i64 query(int x)
{
    i64 res = 0;
    while (x)
    {
        res += C[x];
        x -= (x & -x);
    }
    return res;
}

inline i64 BLUE(__int128 a, int p)
{
    if (a <= p) return a;
    return a % p + p;
}

inline i64 poww(__int128 a, i64 b, int p)
{
    __int128 res = 1;
    while (b)
    {
        if (b & 1) res = BLUE(res * a, p);
        a = BLUE(a * a, p);
        b >>= 1;
    }
    return res;
}

i64 pocket(int l, int r, int p) // 区间 [l,r] 在 mod p 意义下的答案
{
    if (p == 1) return 1; // 模数为 1 时，要返回 1，这样能保证乘方运算的正确，否则 0 次幂都是 1 了
    i64 t = query(l);
    if (l == r) return BLUE(t, p);
    return poww(t, pocket(l + 1, r, phi[p]), p);
}

int main(void)
{
    REFLECTION_BLUE();
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%lld", A + i);
        update(i, A[i] - A[i - 1]);
    }
    while (m--)
    {
        int op, l, r, x;
        scanf("%d%d%d%d", &op, &l, &r, &x);
        if (op == 1)
        {
            update(l, x);
            update(r + 1, -x);
        }
        else printf("%lld\n", pocket(l, r, x) % x);
    }
    return 0;
}
```
{% endfolding %}

然后可以去做一下不带修改的双倍经验：[[Codeforces 906D] Power Tower](http://codeforces.com/problemset/problem/906/D).

#### [SPOJ] POWTOW - Power Tower City

[Portal](https://www.luogu.com.cn/problem/SP10050).

依然是使用扩展欧拉定理进行降幂，但是要注意 $0$ 的讨论。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

#define i64 long long

using namespace std;
const i64 MOD = 1e9;

bool flag = false;

int phi(int n)
{
    int ans = n, R = min(n, int(sqrt(n)) + 1);
    for (int i = 2; i <= R; ++i)
        if (n % i == 0)
        {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}

inline i64 BLUE(i64 a, i64 p)
{
    if (a < p) return a;
    return a % p + p;
}

i64 poww(i64 a, i64 b, i64 p)
{
    i64 res = 1;
    while (b)
    {
        if (b & 1) res = BLUE(res * a, p);
        a = BLUE(a * a, p);
        b >>= 1;
    }
    return res;
}

i64 pocket(i64 a, i64 b, i64 p) // a^a^...^a^a（b 个） % p
{
    if (p == 1) return 1;
    if (b == 1) return BLUE(a, p);
    return poww(a, pocket(a, b - 1, phi(p)), p);
}

int main(void)
{
    int T;
    scanf("%d", &T);
    while (T--)
    {
        i64 a, b;
        scanf("%lld%lld", &a, &b);
        if (a == 0) printf("%d\n", b % 2 == 0);
        else if (b == 0) puts("1");
        else
        {
            i64 ans = pocket(a, b, MOD);
            if (ans >= MOD) printf("...%09lld\n", ans % MOD);
            else printf("%lld\n", ans);
        }
    }
    return 0;
}
```
{% endfolding %}

### 中国剩余定理

包括 CRT 和 exCRT。

#### [TJOI2009] 猜数字

[Portal](https://www.luogu.com.cn/problem/P3868).

直接使用 CRT，但是小心爆 `long long`。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

#define i64 long long

using namespace std;

void exgcd(i64 a, i64 b, i64 &x, i64 &y)
{
    if (b == 0)
    {
        x = 1, y = 0;
        return;
    }
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}

int n;
i64 a[15], b[15], M[15], t[15];

i64 CRT(void)
{
    i64 mm = 1, ans = 0, x;
    for (int i = 1; i <= n; ++i) mm *= b[i];
    for (int i = 1; i <= n; ++i)
    {
        M[i] = mm / b[i];
        exgcd(M[i], b[i], t[i], x);
        ans = ans + ((__int128)a[i] * M[i] % mm * t[i]) % mm;
    }
    return (ans % mm + mm) % mm;
}

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%lld", a + i);
    for (int i = 1; i <= n; ++i) scanf("%lld", b + i);
    printf("%lld\n", CRT());
    return 0;
}
```
{% endfolding %}

{% divider books %}

通过以上题目可以发现：OI 中的数学题只需要了解基本的概念并灵活运用它们。说起来简单，但想要灵活运用就离不开一个严谨的数学思维体系，多刷题是个很有效的策略。

[^1]: 实际上这个垂直符号仅在 OI 中使用，笔者在各类数学教材中并没有见到这个符号，但是在《具体数学》这种计算机数学教材中就有见到。
[^2]: 如果你想要较真，实际上 $\gcd$ 才是万恶之源，说是“数论只会 $\gcd$“，但这也恰恰证明了 $\gcd$ 无与伦比的重要性。
[^3]: 没有题号的题目可能是经典问题，或者笔者太弱了找不到出处。