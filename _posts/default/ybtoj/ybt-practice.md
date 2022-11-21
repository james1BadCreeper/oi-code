---
title: YbtOJ 高效进阶 2022 模拟赛补题
cover: false
plugins:
    - katex
categories:
    - 算法竞赛
    - 比赛记录
sticky: 1
password: 1#Eqhg24Kngin!aq.-IStogvq2
abstract: 这是学校 一本通付费 OJ（长乐一中集训）的模拟赛，密码请通过校内方式联系笔者获得。这里的题目难度严格小于 NOIP 计划，对于我这种比较渣的选手还是比较友好的。
theme: xray
abbrlink: 4e0b197a
date: 2022-09-19 21:57:02
---

> 垒球。

<!--more-->

## 暑假模拟赛

## Round 1

## Round 2

## Round 3

## Round 4

我是菜狗。

### A 组

### B 组

#### C. 最短直径

[Portal](https://noip.ybtoj.com.cn/contest/724/problem/3).

![](https://pic.imgdb.cn/item/632df23316f2c2beb11c8b4f.jpg)

简单的构造题，在纸上画一画就出来了。

```cpp
#include <iostream>
#include <cstdio>

using namespace std;

int n, k;

int main(void)
{
    freopen("diameter.in", "r", stdin);
    freopen("diameter.out", "w", stdout);
    scanf("%d%d", &n, &k);
    for (int i = 2; i <= k + 1; ++i)
        printf("1 %d\n", i);
    for (int i = k + 2; i <= n; ++i)
        printf("%d %d\n", i, i - k);
    return 0;
}
```

## 国庆集训 Round 1

## 国庆集训 Round 2

### B 组

#### A. 基因突变 

[Portal](http://noip.ybtoj.com.cn/problem/22255).

![](https://pic1.imgdb.cn/item/63383d6e16f2c2beb1f9ae54.jpg)

结论题，不难想。

{% folding::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n;
char s[1000005];

void solve(void)
{
    scanf("%s", s + 1);
    n = strlen(s + 1);
    bool ek = false;
    for (int i = 1; i <= n; ++i)
    {
        if (s[i] != 'l' && s[i] != 't')
        {
            puts("0 0");
            return;
        }
        if (s[i] == 'l') ek = true;
    }
    if (!ek)
    {
        puts("0 0");
        return;
    }
    bool flag1 = true, flag2 = true;
    if (s[1] == 't') flag1 = false;
    for (int i = 2; i <= n; ++i)
        if (s[i] == 'l' && s[i - 1] == 'l') flag1 = false, flag2 = false;
    printf("%d %d\n", flag1 ? 1 : 0, flag2 ? 1 : 0);
}

int main(void)
{
    freopen("gene.in", "r", stdin);
    freopen("gene.out", "w", stdout);
    int T;
    cin >> T;
    while (T--) solve();   
    return 0;
}
```
{% endfolding %}

## 国庆集训 Round 3

## 国庆集训 Round 4

## 国庆集训 Round 5

### B 组

#### B. 魔法森林

[Portal](http://noip.ybtoj.com.cn/problem/22268).

![](https://pic1.imgdb.cn/item/633b08cd16f2c2beb1a76fa9.jpg)

分类讨论题。

{% folding cyan::查看代码 %}
```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;
typedef long long i64;

int n, type[255];
int d[255][255];

int fa[255];
int find(int x)
{
	if (fa[x] == x) return x;
	return fa[x] = find(fa[x]);
}

namespace RealSubtask
{
	int tot = 0;
	struct edge
	{
		int u, v, w;
		edge(int u = 0, int v = 0, int w = 0) :
			u(u), v(v), w(w) {}
		bool operator < (const edge &a) const
		{
			return w < a.w;
		}
	}E[1000005];
	void MAIN(void)
	{
		int ag = 0;
		for (int i = 1; i <= n; ++i) 
		{
			fa[i] = i;
			if (type[i] == 2) ++ag;
		}

        i64 ans = 1e18;
        for (int i = 1; i <= n; ++i) // i 作为中转点
		{
			i64 res = 0;
			for (int j = 1; j <= n; ++j) res += d[i][j];
			ans = min(ans, res);
		}

        if (ag == 2)
        {
            int p = 0, q = 0;
            for (int i = 1; i <= n; ++i)
                if (type[i] == 2)
                {
                    if (!p) p = i;
                    else q = i;
                }
            i64 res = d[p][q];
            for (int i = 1; i <= n; ++i)
                if (type[i] == 3) res += min(d[p][i], d[q][i]);
            ans = min(ans, res);
        }

        if (ag == 0)
        {
            for (int i = 1; i < n; ++i)
		    	for (int j = i + 1; j <= n; ++j)
		    		E[++tot] = edge(i, j, d[i][j]);
		    sort(E + 1, E + tot + 1);
            ans = 0;
		    for (int i = 1; i <= tot && ag < n; ++i)
		    {
		    	int x = find(E[i].u), y = find(E[i].v);
		    	if (x == y) continue;
		    	fa[x] = y;
		    	++ag;
		    	ans += E[i].w;
		    }
        }
        
        printf("%d %lld\n", n - 1, ans);
	}
}

int main(void)
{
	freopen("forest.in", "r", stdin);
	freopen("forest.out", "w", stdout);
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) scanf("%d", type + i);
	i64 ans = 0; int res = 0;
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n; ++j)
		{
			scanf("%d", &d[i][j]);
			if (i < j && (type[i] == 1 || type[j] == 1)) 
				ans += d[i][j], ++res;
		}
	if (res != 0) printf("%d %lld\n", res, ans);
	else RealSubtask::MAIN();
	return 0;
}
```
{% endfolding %}