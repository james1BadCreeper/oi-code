---
title: 数论进阶
plugins:
  - katex
cover: false
headimg: 'https://z3.ax1x.com/2021/11/07/I1P7Ie.jpg'
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 数论
    - 数学
    - 笔记
group: notes
hide: true
abbrlink: 7f8fede0
date: 2022-08-24 18:19:56
---

> 高级的数论知识更为困难，但也更有意思。本文将介绍省选以内的常见数论知识点，并通过题目介绍它们的应用。

<!--more-->

在此之前，请完全消化上一篇文章的内容，并可以与本文的“热身”部分穿插阅读：

{% link 数论初步::/2021/number-theory/ %}

## 热身

这里是一些之前略过的内容，在之前的基础上加以扩展。

### 数论分块

### 实数取模

之前听说过整数取余运算，但是想不到实数都可以取余？我们一一来看。

#### 有理数取余

如果 $x\equiv \cfrac{a}{b}\pmod p$，那么两边同乘 $b$ 得 $bx\equiv a \pmod p$。这不就是之前讲过得同余方程吗？

虽然问题已经解决，但是还要问一句，什么时候无解？显然是 $a$ 不为 $(b,p)$ 的倍数（同余方程的结论），但是我们肯定想要最简分数，此时由于 $b<p$（先取模），加上 $a\perp b$，显然当 $(b,p)\ne 1$ 的时候无解。

有没有发现，这就是求逆元的过程？的确如此，因为逆元干的事就是有理数取余。

[模板](https://www.luogu.com.cn/problem/P2613)，注意分子分母很大，所以读入时就要取模。代码如下：

{% folding cyan::查看代码 %}

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

const int MOD = 19260817;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) {
        x = (x<<3) + (x<<1) + (c^48);
        x %= MOD;
        c = getchar();
    }
    return x;
}

int gcd(int x, int y) {
    if (y == 0) return x;
    return gcd(y, x % y);
}

void exgcd(int a, int b, i64 &x, i64 &y) {
    if (b == 0) {
        x = 1, y = 0;
        return;
    }
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}

int main(void) {
    int a, b, g;
    i64 x, y; // 注意要开 long long
    a = read(), b = read();
    g = gcd(b, MOD);
    if (g == 1) {
        exgcd(b, MOD, x, y);
        x *= a / gcd(b, MOD); // 乘上倍数
        printf("%d\n", (x % MOD + MOD) % MOD);
    }
    else puts("Angry!");
    return 0;
}
```

{% endfolding %}

#### 无理数取余

如果有理数 $a$ 满足 $a^k\equiv b \pmod p$，那么有 $a\equiv \sqrt[k]{b} \pmod p$（根据刚才的经验）。当然，也有很大可能无解。

这个问题可以通过简单枚举来获取答案，代码如下：

```cpp
#include <iostream>
#include <cstdio>

using namespace std;
using i64 = long long;

i64 b, k, p;

inline i64 power(i64 x, i64 y)
{
    i64 res = 1;
    while (y)
    {
        if (y & 1) res = res * x % p;
        x = x * x % p;
        y >>= 1;
    }
    return res;
}

int main(void)
{
    scanf("%lld%lld%lld", &b, &k, &p);
    for (i64 i = 1; i <= p; ++i) // 超过 p 没有意义（成循环了）
        if (power(i, k) == b)
            printf("%d\n", i);
    return 0;
}
```

高效求解这个问题非常困难，需要使用 2 次剩余或者 N 次剩余，将在本文的最后部分论述。

### 光速幂

给定 $a,c$，每次询问给出 $b$，光速{% emoji aru 162 %}求出 $a^b \bmod c$，$\mathcal{O}(\sqrt{c})$ 预处理，$\mathcal{O}(1)$ 查询。

还记得扩展欧拉定理吗？就是这个：

$$
a^{b}\equiv
\begin{cases}
a^{b\bmod\varphi(p)}, &a\perp p,\\
a^b, &(a,p)\neq 1,b<\varphi(p),\\
a^{b\bmod\varphi(p)+\varphi(p)},  &(a,b)\neq 1,b\ge\varphi(p).
\end{cases}
\pmod p
$$

我们说过可以用二式三式进行降幂，于是我们就可以将 $b$ 缩小到 $2\times \varphi(c)$ 的范围以内（也就是 $2c$ 以内）。然后我们可以这样推：$a^b=a^{\sqrt{c}\times \lfloor\frac{b}{\sqrt{c}}\rfloor+b\bmod \sqrt{c}}=(a^{\sqrt{c}})^{\lfloor\frac{b}{\sqrt{c}}\rfloor}\times a^{b\bmod \sqrt{c}}$。其中 $\lfloor\frac{b}{\sqrt{c}}\rfloor<2\sqrt{c}$（想一想刚才 $c$ 的范围，$b\bmod \sqrt{c} < \sqrt{c}$。   
于是预处理 $(a^{\sqrt{c}})^i,a^j(1\le i < 2\sqrt{c},1\le j<\sqrt{c})$ 即可。预处理时间复杂度 $\mathcal{O}(\sqrt{c})$，包括处理欧拉函数的时间。

代码如下，读者可以自行编写造数据测试一下。

```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
using i64 = long long;

inline int read(void) {
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

int a, b, c, T, t, phi;
int fac[1000005], facp[2000005];

void calcPhi(int n) {
    int ans = n, t = sqrt(n + 0.5);
    for (int i = 2; i <= t; ++i)
        if (n % i == 0)
        {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    if (n > 1) ans = ans / n * (n - 1);
    phi = ans;
}

void init(void) {
    a %= c; // 一般来讲，此行是必要的
    t = sqrt(c + 0.5);
    fac[0] = facp[0] = 1;
    for (int i = 1; i <= t; ++i) fac[i] = (i64)fac[i - 1] * a % c; // 预处理 a^i
    for (int i = 1; i <= t * 2; ++i) facp[i] = (i64)facp[i - 1] * fac[t] % c; // 预处理 (a^sqrt(c))^i
    calcPhi(c); // 计算 c 的欧拉函数以降幂
}

inline int calc(int b) {
    return (i64)facp[b / t] * fac[b % t] % c;
}

int main(void)
{
    a = read(), c = read(), T = read();
    init();
    while (T--)
    {
        b = read();
        if (b >= phi) b = b % phi + phi; // 利用扩展欧拉定理三式降幂
        printf("%d\n", calc(b));
    }
    return 0;
}
```

光速幂的这个 `t` 不一定非是算数平方根，取一个较快的值就行（往往是 $2$ 的次方以用位运算加速取模）。

然后我们来看这道鬼题：[\[Luogu 5110\] 块速递推](https://www.luogu.com.cn/problem/P5110)。

多次询问（而且真多）一个递推式的第 $n$ 项，而且询问的内容在 64 位无符号整数内，可以肯定不是矩阵快速幂加速递推，需要把这个东西写成通项公式再做。

写通项公式的方式有很多，这里使用搞斐波那契数列的特征方程来做：

设 $a_n=233a_{n-1}+666a_{n−2}$ 的特征方程为：

$$
x^{n+2}=233x^{n+1}+666x^n\\
x^2=233x+666\\
x^2-233x-666=0\\
x_1=\frac{233+\sqrt{56953}}{2},x_2=\frac{233-\sqrt{56953}}{2}
$$

然后开始算方程组：

$$
a_n=px_1^{n}+qx_2^n,\\
\because a_0=0,a_1=1\\
\therefore \begin{cases}
p+q=0,\\
px_1+qx_2=1,
\end{cases}\\
\therefore \begin{cases}
p+q=0,\\
p\times\cfrac{233+\sqrt{56953}}{2}+q\times\cfrac{233-\sqrt{56953}}{2}=1,
\end{cases}\\
\therefore \begin{cases}
p=\cfrac{1}{\sqrt{56953}},\\
q=-\cfrac{1}{\sqrt{56953}},
\end{cases}\\
\therefore a_n=\cfrac{1}{\sqrt{56953}}\left(\frac{233+\sqrt{56953}}{2}\right)^{n}-\cfrac{1}{\sqrt{56953}}\left(\frac{233-\sqrt{56953}}{2}\right)^{n},\\
\therefore a_n=\cfrac{1}{\sqrt{56953}}\left(\left(\frac{233+\sqrt{56953}}{2}\right)^{n}-\left(\frac{233-\sqrt{56953}}{2}\right)^{n}\right)
$$

这个式子是化简不动了，但是注意这题是对 $10^9+7$ 取模，然后就可以利用之前的无理数取模，发现 $188305837$ 跟 $\sqrt{56953}$ 是同余的。然后就可以做成（注意利用在数论初步中讲的负数取模，可得 $-94152802 \bmod 10^9+7=905847205$）：

$$
a_n=\cfrac{1}{188305837}\left(\left(\frac{233+188305837}{2}\right)^{n}-\left(\frac{233-188305837}{2}\right)^{n}\right)\\
a_n=\cfrac{1}{188305837}\left(94153035^{n}-(-94152802)^{n}\right)\\
a_n=\cfrac{1}{188305837}\left(94153035^{n}-905847205^{n}\right)
$$

利用有理数取模，知 $\cfrac{1}{188305837}\equiv 233230706 \pmod{10^9+7}$，所以最终式子化为：

$$
a_n=233230706(94153035^{n}-905847205^{n})
$$

然后就可以正常使用光速幂计算了。注意 $10^9+7$ 是质数（$\varphi(10^9+7)=10^9+6$），而且底数小于这个质数（即互质），所以降幂的时候用一式降幂即可。代码如下：

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
using u64 = unsigned long long;
using i64 = long long;

const int MOD = 1000000007;

namespace Mker
{
    unsigned long long SA, SB, SC;
    void init() { scanf("%llu%llu%llu", &SA, &SB, &SC); }
    unsigned long long rand()
    {
        SA ^= SA << 32, SA ^= SA >> 13, SA ^= SA << 1;
        unsigned long long t = SA;
        SA = SB, SB = SC, SC ^= t ^ SA;
        return SC;
    }
}

int t;
int f1[33000], f2[66000], f3[33000], f4[66000];

inline int pow1(int n) { return (i64)f2[n / t] * f1[n % t] % MOD; }
inline int pow2(int n) { return (i64)f4[n / t] * f3[n % t] % MOD; }

int main(void)
{
    int T, ans = 0;
    scanf("%d", &T);
    Mker::init();
    f1[0] = f2[0] = f3[0] = f4[0] = 1;
    t = sqrt(MOD + 0.5);
    for (int i = 1; i <= t; ++i) f1[i] = (i64)f1[i - 1] * 94153035 % MOD;
    for (int i = 1; i <= (t << 1); ++i) f2[i] = (i64)f2[i - 1] * f1[t] % MOD;
    for (int i = 1; i <= t; ++i) f3[i] = (i64)f3[i - 1] * 905847205 % MOD;
    for (int i = 1; i <= (t << 1); ++i) f4[i] = (i64)f4[i - 1] * f3[t] % MOD; 
    while (T--)
    {
        u64 n = Mker::rand() % (MOD - 1); // phi(MOD) = MOD - 1
        ans ^= (u64)233230706 * (pow1(n) - pow2(n) + MOD) % MOD;
    }
    printf("%d\n", ans);
    return 0;
}
```

{% endfolding %}

{% divider water %}

当然，你还可以利用这个原理整个矩阵光速幂，这里就不说了{% emoji tong ... %}。

### Wilson 定理

这就是威尔逊定理，不用怀疑{% emoji 滑稽 %}。

### exCRT

我们知道 CRT 在求解横线性同余方程组的时候要求模数两两互质，为什么呢？

### 数论分块

### 随机判定方法

## 筛法与积性函数

## 高次同余方程

## Problemset

这里的题可能有一些难，也会与别的算法综合，但是都应该完成。

