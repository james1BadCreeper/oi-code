---
title: 基础线性代数
cover: false
plugins:
    - katex
abbrlink: 22ceb96a
date: 2022-08-27 09:50:36
categories:
    - OI
    - 学习笔记
tag:
    - 线性代数
    - 矩阵快速幂
    - 高斯消元
    - 数学
    - 笔记
headimg:
group: notes
hide: true
status: working
---

> 线性代数是代数学的一个分支，主要处理线性关系问题。OI 中用到的相关知识并不多，主要是矩阵乘法与高斯消元等，本文将简单介绍这些内容。

<!--more-->

## 矩阵乘法

一个 $n\times m$ 的矩阵可以看作是一个 $n\times m$ 的二维数组。矩阵加法和减法就是把相对应的数加减，而矩阵乘法则很有意思，拥有几何意义，这里不做展开。

### 矩阵乘法

设 $A$ 是 $n\times m$ 的矩阵，$B$ 是 $m\times p$ 的矩阵，那么 $C=A\times B$ 是 $n\times p$ 的矩阵，并且：

$$
C_{i,j}=\sum_{k=1}A_{i,k}B_{k,j}
$$

[模板](https://www.luogu.com.cn/problem/B3615)，代码如下：

```cpp
for (int i = 0; i < n; ++i)
    for (int k = 0; k < n; ++k) {
        int r = a[i][k];
        for (int j = 0; j < n; ++j)
            c[i][j] += r * b[k][j];
    }
```

注意循环顺序！这样的顺序对答案不会造成影响，但是缓存访问是连续的，因此跑得较快。

### 矩阵快速幂

矩阵乘法不满足交换律，但是满足结合律（也满足分配律），因此快速幂的计算方式可以直接移用。注意矩阵乘法有一个“单位矩阵”的概念，类似于数的乘法中“1”。

[模板](https://www.luogu.com.cn/problem/P3390)，代码入下：

```cpp
struct Matrix {
    int a[105][105];
    Matrix() { memset(a, 0, sizeof(a)); };
    void reset(void) { // 构建单位矩阵
        for (int i = 1; i <= n; ++i)
            a[i][i] = 1;
    }
};

Matrix operator * (const Matrix &x, const Matrix &y) {
    Matrix z;
    for (int i = 1; i <= n; ++i)
        for (int k = 1; k <= n; ++k) {
            int r = x.a[i][k];
            for (int j = 1; j <= n; ++j)
                z.a[i][j] = (z.a[i][j] + 1ll * r * y.a[k][j]) % MOD;
        }
    return z;
}

Matrix poww(Matrix a, i64 b) {
    Matrix res; res.reset();
    while (b) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}
```

## 矩阵与递推

矩阵乘法的一大作用就是加速递推。

### 概述

递推都有一个递推式子，我们可以把这个式子中的变量写进矩阵，然后再乘上一个矩阵进行变换。由于递推式必变，因此每次乘上的矩阵都是相同的，可以使用矩阵快速幂来进行计算。

### 例题

我们来看几道简单的题目。

#### [Luogu P1962] 斐波那契数列

[Portal](https://www.luogu.com.cn/problem/P1962).

要将 $\begin{bmatrix}F_{n-1}\\F_{n-2}\end{bmatrix}$ 变换为 $\begin{bmatrix}F_{n}\\F_{n-1}\end{bmatrix}$，利用待定系数法可以求得需要乘上 $\mathbf{A}=\begin{bmatrix}1 & 1\\1 & 0\end{bmatrix}$，即 $\begin{bmatrix}F_{n}\\F_{n-1}\end{bmatrix}=\begin{bmatrix}1 & 1\\1 & 0\end{bmatrix}\begin{bmatrix}F_{n-1}\\F_{n-2}\end{bmatrix}$。要多次变换的话，由于矩阵乘法有结合律，利用矩阵快速幂就可以搞定，最后在乘上初始 $F$ 矩阵。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
typedef long long i64;
const int MOD = 1000000007;

struct Matrix {
    int a[3][3];
    Matrix() { memset(a, 0, sizeof(a)); };
    void reset(void) {
        for (int i = 1; i <= 2; ++i)
            a[i][i] = 1;
    }
    Matrix friend operator * (const Matrix &a, const Matrix &b) {
        Matrix c;
        for (int i = 1; i <= 2; ++i)
            for (int k = 1; k <= 2; ++k) {
                int r = a.a[i][k];
                for (int j = 1; j <= 2; ++j)
                    c.a[i][j] = (c.a[i][j] + 1ll * r * b .a[k][j]) % MOD;
            }
        return c;
    }
};

Matrix poww(Matrix T, i64 b) {
    Matrix res; res.reset();
    while (b) {
        if (b & 1) res = res * T;
        T = T * T;
        b >>= 1;
    }
    return res;
}

int main(void) {
    i64 n; cin >> n;
    if (n <= 2) return puts("1"), 0;
    Matrix a, f;
    a.a[1][1] = a.a[1][2] = a.a[2][1] = 1;
    f.a[1][1] = 1; // f 记录 F[1], F[0]
    a = poww(a, n - 1); // 从 n = 2 变换，所以 -1
    f = a * f;
    cout << f.a[1][1] << '\n';
    return 0;
}
```
{% endfolding %}

#### [Luogu P1939] 【模板】矩阵加速（数列）

[Portal](https://www.luogu.com.cn/problem/P1939).

方法是相同的。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
const int MOD = 1000000007;
const int N = 3;

struct Matrix {
    int a[4][4];
    Matrix(void) { memset(a, 0, sizeof(a)); }
    void build(void) {
        for (int i = 1; i <= N; ++i)
            a[i][i] = 1;
    }
    Matrix friend operator * (const Matrix &a, const Matrix &b) {
        Matrix c;
        for (int i = 1; i <= N; ++i)
            for (int k = 1; k <= N; ++k) {
                int r = a.a[i][k];
                for (int j = 1; j <= N; ++j)
                    c.a[i][j] = (c.a[i][j] + 1ll * r * b.a[k][j]) % MOD;
            }
        return c;
    }
};

Matrix poww(Matrix a, int b) {
    Matrix res; res.build();
    while (b) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

int main(void) {
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        if (n <= 3) {
            puts("1");
            continue;
        }
        Matrix a, f;
        a.a[1][1] = a.a[1][3] = a.a[2][1] = a.a[3][2] = 1;
        f.a[1][1] = f.a[2][1] = f.a[3][1] = 1;
        a = poww(a, n - 3);
        f = a * f;
        cout << f.a[1][1] << '\n';
    }
    return 0;
}
```
{% endfolding %}

#### [Luogu P1349] 广义斐波那契数列

[Portal](https://www.luogu.com.cn/problem/P1349).

只是我们的变换矩阵不一样了。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
const int N = 2;

int m;
struct Matrix {
    int a[3][3];
    Matrix(void) { memset(a, 0, sizeof(a)); }
    void build(void) {
        for (int i = 1; i <= N; ++i)
            a[i][i] = 1;
    }
    Matrix friend operator * (const Matrix &a, const Matrix &b) {
        Matrix c;
        for (int i = 1; i <= N; ++i)
            for (int k = 1; k <= N; ++k) {
                int r = a.a[i][k];
                for (int j = 1; j <= N; ++j)
                    c.a[i][j] = (c.a[i][j] + 1ll * r * b.a[k][j]) % m;
            }
        return c;
    }
};

Matrix poww(Matrix a, int b) {
    Matrix res; res.build();
    while (b) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

int main(void) {
    int p, q, a1, a2, n;
    cin >> p >> q >> a1 >> a2 >> n >> m;
    if (n <= 2) return (cout << (n == 1 ? a1 : a2) << '\n', 0);
    Matrix a, f;
    a.a[1][1] = p, a.a[1][2] = q, a.a[2][1] = 1;
    f.a[1][1] = a2, f.a[2][1] = a1;
    a = poww(a, n - 2);
    f = a * f;
    cout << f.a[1][1] << '\n';
    return 0;
}
```
{% endfolding %}

## 高斯消元

## 线性空间

## Problemset

主要是应用。

### 矩阵快速幂

如之前所说，它应用真的不少。

#### [NOI2012] 随机数生成器

[Portal](https://www.luogu.com.cn/problem/P2044).

这样：

$$
\begin{bmatrix}F_n\\c\end{bmatrix}=\begin{bmatrix}a&1\\0&1\end{bmatrix}\begin{bmatrix}F_{n-1}\\c\end{bmatrix}
$$

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
typedef long long i64;
typedef __int128_t LL;

i64 m, a, c, x0, n, g;

struct Matrix {
    i64 a[2][2];
    Matrix() { memset(a, 0, sizeof(a)); }
    void build(void) { a[0][0] = a[1][1] = 1; }
    Matrix friend operator * (const Matrix &a, const Matrix &b) {
        Matrix c;
        for (int i = 0; i < 2; ++i)
            for (int k = 0; k < 2; ++k) {
                i64 r = a.a[i][k];
                for (int j = 0; j < 2; ++j)
                    c.a[i][j] = (c.a[i][j] + (LL)r * b.a[k][j]) % m;
            }
        return c;
    }
};

Matrix poww(Matrix a, i64 b) {
    Matrix res; res.build();
    while (b) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

int main(void) {
    cin >> m >> a >> c >> x0 >> n >> g;
    Matrix A, F;
    A.a[0][0] = a; A.a[0][1] = A.a[1][1] = 1;
    F.a[0][0] = x0, F.a[1][0] = c;
    A = poww(A, n);
    F = A * F;
    cout << F.a[0][0] % g << endl;
    return 0;
}
```
{% endfolding %}