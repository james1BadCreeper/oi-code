---
title: YbtOJ 高效进阶 图论 第1章 并查集
cover: false
plugins:
    - katex
abbrlink: 48fe9eeb
date: 2022-09-21 19:06:24
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 并查集
status: done
sticky: -100
---

> 并查集是图论？！

<!--more-->

## A. 【模板】并查集

真·模板题。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>

using namespace std;

inline int read(void)
{
    int x = 0, c = getchar();
    while (!isdigit(c)) c = getchar();
    while (isdigit(c)) x = (x<<3) + (x<<1) + (c^48), c = getchar();
    return x;
}

class UnionFind_Set
{
    private:
        int fa[10005], size[10005];
    public:
        inline void init(int n)
        {
            for (int i = 1; i <= n; ++i)
                fa[i] = i, size[i] = 1;
        }
        int find(int x)
        {
            if (fa[x] == x) return x;
            return fa[x] = find(fa[x]);
        }
        inline void uni(int x, int y)
        {
            int xx = find(x), yy = find(y);
            if (xx == yy) return;
            if (size[xx] > size[yy]) swap(xx, yy);
            fa[xx] = yy;
            size[yy] += size[xx];
        }
        inline bool ask(int x, int y)
        {
            if (find(x) == find(y)) return 1;
            else return 0;
        }
}U;

int main(void)
{
    int n = read(), m = read();
    U.init(n);
    while (m--)
    {
        int op = read(), x = read(), y = read();
        if (op == 1) U.uni(x, y);
        else puts(U.ask(x, y) ? "Y" : "N");
    }
    return 0;
}
```
{% endfolding %}