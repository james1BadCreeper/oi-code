---
title: 简单字符串
plugins:
    - katex
cover: false
categories:
    - 算法竞赛
    - 学习笔记
tag:
    - 字符串
	- 笔记
abbrlink: f51e608c
date: 2022-09-13 11:44:29
group: notes
hide: true
headimg: https://s1.ax1x.com/2022/08/04/vm9379.jpg
---

> 字符串，就是由字符连接而成的序列。常见的字符串问题包括字符串匹配问题、子串相关问题、前缀/后缀相关问题、回文串相关问题、子序列相关问题等。具有很高的工程价值，比如搜索引擎。本文将介绍简单的字符串知识。

<!--more-->

## 简单内容

这里是一些概念。

### 定义

一个**字符集** $\Sigma$ 是一个建立了全序关系的集合，也就是说，$\Sigma$ 中的任意两个不同元素 $\alpha,\beta$ 都可以比较大小，其中的元素称之为字符。

一个**字符串** $S$ 是将 $n$ 个字符顺次排列形成的序列，$n$ 表示字符串的长度，记为 $|S|$。字符的编号从 $0$ 开始（尽可能这样编号，这样是与标准一致的）。

**字典序**是以第 $i$ 个字符作为关键字符进行比较，特别注意，空字符是最小的：`"a" < "aa"`。

子串、子序列、前缀后缀和回文串就不再赘述了。

### char 数组

```cpp
char s[1005]; // 声明字符数组
const char S[] = "abab"; // 常量字符数组

printf("%s", s); scanf("%s", s); // 读入和输出
sprintf(s, "%d ", a[i]); sscanf(s, "%d", &x); // 向字符串输出或从字符串读入
```

### C++ string 类

```cpp
#include <string>
std::string s;
```

`string` 对 char 数组兼容，上述内容都可以直接使用。除此之外，它还有许多特有内容。

`string` 很方便，但也很慢，遇到数据规模大的题应当慎重使用。

建议大家熟练掌握字符数组、`string` 和 `sstream`，都是很有用的。

## 字符串 Hash

## Trie 树

Trie，又称字典树或者前缀树，可以用来保存字符串的集合，是一种树形数据结构。

{% image https://pic.imgdb.cn/item/6371f98c16f2c2beb17889f5.png::alt=一棵简单的 Trie %}

采用类似于树上前缀和的计算方式，可以得出 $2$ 号节点代表字符串 `a`，$6$ 号代表 `aaa`。

实现时，我们一般把根节点记为 $0$ 号节点，因为它不代表任何字符串，每个节点有 `ch[i][sigma_size]`，其中 `sigma_size` 代表 Trie 树的字符集大小（不用 `vector` 之类的是为了保证时间效率，用空间换时间），表示是否有这个儿子。还有一个 `val[i]`，可以用于记录这个字符串的权值，更关键的是记录这个节点是不是字符串。

[模板](https://www.luogu.com.cn/problem/P8306)，代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, q, tot = 0;
int ch[3000005][62], val[3000005]; 

int f(char x) {
	if (x >= 'A' && x <= 'Z') return x - 'A';
	if (x >= 'a' && x <= 'z') return x - 'a' + 26;
	return x - '0' + 52;
}

void insert(char *s) {
	int x = 0, len = strlen(s);
	for (int i = 0; i < len; ++i) {
		int c = f(s[i]);
		if (!ch[x][c]) ch[x][c] = ++tot;
		x = ch[x][c];
		val[x] += 1;
	}
}

int find(char *s) {
	int x = 0, len = strlen(s);
	for (int i = 0; i < len; ++i) {
		int c = f(s[i]);
		if (!ch[x][c]) return 0;
		x = ch[x][c];
	}
	return val[x];
}

char s[3000005];
void solve(void) {
	scanf("%d%d", &n, &q);	
	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j < 62; ++j)
			ch[i][j] = 0;
		val[i] = 0;
	}
	while (n--) {
		scanf("%s", s);
		insert(s);
	}
	while (q--) {
		scanf("%s", s);
		printf("%d\n", find(s));
	}
}

int main(void) {
	int T; scanf("%d", &T);
	while (T--) solve();
	return 0;
}
```

## KMP 模式匹配

### Z 函数

## AC 自动机

## Problemset