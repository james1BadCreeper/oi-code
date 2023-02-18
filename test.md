## 前置知识

在介绍莫比乌斯反演前，我们先来介绍一些前置知识。

### 狄利克雷卷积

狄利克雷卷积是数论函数的基本运算。定义：

$$
h(n)=\sum_{d\mid n}f(d)g\left(\frac{n}{d}\right)
$$

简记为 $h=f*g$。

狄利克雷卷积具有交换律、结合律和分配律。

首先，$\epsilon*f=f$。因此单位函数 $\epsilon$ 为狄利克雷卷积的**单位元**，那么就可以定义数论函数的逆元 $f^{-1}$，满足 $f*f^{-1}=\epsilon$。

一个 $f$ 存在逆元，当且仅当 $f(1)\ne 0$，并且逆元唯一。$f=g$ 的充要条件是 $f*h=g*h(h(1)\ne 1)$。这样一来有 $g(n) = -\cfrac{\sum\limits_{d \mid n, d \neq n} g(d)f\left(\dfrac n d\right)} {f(1)}$。

积性函数的狄利克雷卷积是积性函数，积性函数的逆元也是积性函数。

### 狄利克雷前缀和

任意数论函数 $f$ 卷常数函数 $1$ 等价于做 $f$ 的**狄利克雷前缀和**，即：$g=f*1,g(n)=\sum_{d\mid n}f(d)$。含义是对每个 $n$ 计算给定数论函数在其所因数处的取值和。

将每个数写成无穷序列 $a_n=\{c_1,c_2,\cdots\}$ 表示 $n=\prod p_i^{c_i}$。由于 $x\mid y$ 的充要条件为 $a_x(c_i)\le a_y(c_i)$，因此 $f*1$ 可以看成对下标做其无穷序列的高维前缀和。

```cpp
for (int i = 1; i <= tot; ++i)
    for (int j = 1; j * prime[i] <= n; ++j)
        a[j * prime[i]] += a[j]; 
```

## 莫比乌斯反演

我们定义莫比乌斯函数：

$$
\mu(n)=\begin{cases}1,&n=1,\\0,&\exists d>1,d^{2}\mid n, \\(-1)^{\omega(n)},&\mathrm{otherwise}. \end{cases}
$$

为什么要定义这样一个奇怪的函数呢？实际上它是在对 $\mathbb{N}$ 做容斥。设 $g(n)=\sum_{n\mid d} f(d)$，已知 $g$，要求 $f(1)$。$f(1)$ 等于 $g$ 在 $1$ 的倍数处取的取值和，减去质数处的取值和，但是多减了两个质数乘积的地方，因此还要加回来。这就是容斥原理，容斥系数是 $(-1)^{\omega(n)}$。

莫比乌斯函数不仅是积性函数，还满足：

$$
\sum_{d\mid n}\mu(d)=\begin{cases}
1,&n=1,\\
0,&n\neq 1.
\end{cases}
$$

上述式子描述的其实是 $\mu*1=\epsilon$，这是莫比乌斯函数最重要的性质，其引出了性质：$\sum_{d\mid n}\mu(d)=[n=1]$，我们可以将这个判断式转化为和式，这样更加方便计算。两者的转化称之为莫比乌斯反演。

莫比乌斯反演有以下结论：

- 若 $g=f*1$，则 $f=g*\mu$。
- 若 $g(n)=\sum_{n\mid d}f(d)$，则 $f(n)=\sum_{n\mid d} \mu\left(\dfrac d n\right) g(d)$。这也被称为**莫比乌斯变换**。
- 由于 $\varphi * 1 = \operatorname{id}$，因此 $\operatorname{id}*\mu =\varphi$。

## 常见应用

莫比乌斯反演的一个常见的应用是，$[\gcd(i,j)=1]=\sum_{d\mid \gcd(i,j)}\mu(d)$。虽然看起来这像是废话，但是这一步会将求和式中的艾弗森括号去除，使得和式更方便计算。这一步将 $i,j$ 互质转化为了枚举 $\gcd(i,j)$ 的约数 $d$。如果 $i,j$ 同样需要枚举，那么枚举 $d$ 并计算合法的 $i,j$ 个数（当且仅当 $d\mid i,d\mid j$）即可。具体来说：

$$
\begin{aligned}
\sum\limits_{i = 1} ^ n \sum\limits_{j = 1} ^ m [\gcd(i, j) = 1] & = \sum\limits_{i = 1} ^ n \sum\limits_{j = 1} ^ m \sum\limits_{d\mid \gcd(i, j)} \mu(d) \\
& = \sum\limits_{d = 1} ^ {\min(n, m)} \mu(d) \sum\limits_{i = 1} ^ n \sum\limits_{j = 1} ^ m [d\mid i\land d\mid j] \\
& = \sum\limits_{d = 1} ^ {\min(n, m)} \mu(d) \left\lfloor \dfrac n d \right\rfloor \left\lfloor \dfrac m d \right\rfloor \\
\end{aligned}
$$

就可以直接使用数论分块在 $O(\sqrt{n}+\sqrt{m})$ 的时间内计算。

类似的过程欧拉函数也可以应用。欧拉函数也有性质 $\sum_{d\mid n}\varphi(d)=n$，因此有欧拉反演：

$$
\begin{aligned}
\sum_{i=1}^n\sum_{j=1}^m\gcd(i,j)&=\sum_{i=1}^n\sum_{j=1}^m\sum_{d\mid \gcd(i,j)} \varphi(d)\\
&=\sum_{d=1}^{\min(n,m)}\varphi(d)\sum\limits_{i = 1} ^ n \sum\limits_{j = 1} ^ m [d\mid i\land d\mid j] \\
&=\sum\limits_{d = 1} ^ {\min(n, m)} \varphi(d) \left\lfloor \frac n d \right\rfloor \left\lfloor \frac m d \right\rfloor
\end{aligned}
$$

我们再来看一个例题：求 $\sum_{i=1}^n\sum_{j=1}^m\operatorname{lcm}(i,j)$。

可以做出如下推导：

$$
\begin{aligned}
\sum_{i=1}^n\sum_{j=1}^m\operatorname{lcm}(i,j)&=\sum_{i=1}^n\sum_{j=1}^m\frac{ij}{\gcd(i,j)}\\
&=\sum_{d=1}^n \frac 1 d \sum_{i=1}^n\sum_{j=1}^m ij[\gcd(i,j)=d]\\
&=\sum_{d=1}^n d \sum_{i=1}^{\lfloor \frac n d\rfloor}\sum_{j=1}^{\lfloor \frac m d\rfloor} ij[\gcd(i,j)=1]\\
\end{aligned}
$$

设 $F(n,m)=\sum_{i=1}^n\sum_{j=1}^m ij[\gcd(i,j)=1]$，则原式为 $\sum_{d=1}^n d\times F(\lfloor \frac n d\rfloor, \lfloor \frac m d \rfloor)$。如果 $F$ 函数可以快速计算，那么原式就可以通过数论分块快速计算。$F$ 函数的计算可以通过莫比乌斯反演完成：

$$
\begin{aligned}
F(n,m)&=\sum_{i=1}^n\sum_{j=1}^m ij\sum_{d\mid\gcd(i,j)}\mu(d)\\
&=\sum_{d=1}^n \mu(d)\sum_{i=1}^n\sum_{j=1}^m ij[d\mid i\land d\mid j]\\
&=\sum_{d=1}^n \mu(d)d^2\sum_{i=1}^{\lfloor \frac n d\rfloor}\sum_{j=1}^{\lfloor \frac m d\rfloor}ij
\end{aligned}
$$

后面的这一个和式 $\sum_{i=1}^{\lfloor \frac n d\rfloor}\sum_{j=1}^{\lfloor \frac m d\rfloor}ij$ 可以直接计算：$\sum_{i=1}^n\sum_{j=1}^m ij=\frac{n(n+1)}{2}\frac{m(m+1)}{2}$，因此 $F$ 函数也可以通过数论分块计算。

至此，我们就以 $O(n+m)$ 的时间复杂度解决了问题。

--- 

## 研究感想

本文简单介绍了莫比乌斯反演及其应用，然而与积性函数相关的内容还有很多，比如以杜教筛为基的一类压 $\log$ 筛法。通过本次研究性学习我感受到了数论的美妙，还有更多的知识等待我去学习。
