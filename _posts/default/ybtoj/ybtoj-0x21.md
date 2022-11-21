---
title: YbtOJ 高效进阶 字符串算法 第1章 字符串处理
cover: false
plugins:
    - katex
abbrlink: 48fe9eeb
date: 2022-09-13 19:06:24
categories:
    - 算法竞赛
    - 做题记录
tag:
    - 模拟
status: done
sticky: -100
---

> 新手字符串。

<!--more-->

有的不放题面是因为没有什么可以参考的内容。

## A. 数字反转

模板题。

```cpp
#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>

using namespace std;

string s;
bool flag[15];

int main(void)
{
    cin >> s;
    if (s[0] == '-') reverse(s.begin() + 1, s.end());
    else reverse(s.begin(), s.end());
    for (int i = (s[0] == '-' ? 1 : 0); i < s.length(); ++i)
        if (s[i] == '0') flag[i] = true;
        else break;
    for (int i = 0; i < s.length(); ++i)
        if (!flag[i]) putchar(s[i]);
    putchar('\n');
    return 0;
}
```

## B. 移位包含

模板题。

```cpp
#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>

using namespace std;

string a, b;

bool check(int p)
{
    for (int i = 0; i < a.length(); ++i)
        if (a[i] != b[(p + i) % b.length()]) return false;
    return true;
}

int main(void)
{
    cin >> a >> b;
    if (a.length() > b.length()) swap(a, b);
    for (int i = 0; i < b.length(); ++i)
        if (check(i))
        {
            puts("true");
            return 0;
        }
    puts("false");
    return 0;
}
```

## C. 单词替换

模板题。

```cpp
#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

using namespace std;

string s, x, a, b;
stringstream ss;

int main(void)
{
    getline(cin, s);
    stringstream ss(s);
    cin >> a >> b;
    while (ss >> x)
    {
        if (x == a) x = b;
        cout << x << ' ';
    }
    cout << '\n';
    return 0;
}
```

## D. 字符串环

模板题。

```cpp
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

string a, b;

int check(int i, int j)
{
    int res = 0;
    for (; res < min(a.length(), b.length()); ++i, ++j)
    {
        if (a[i % a.length()] == b[j % b.length()]) ++res;
        else break;
    }
    return res;
}

int main(void)
{
    cin >> a >> b;
    int ans = 0;
    for (int i = 0; i < a.length(); ++i)
        for (int j = 0; j < b.length(); ++j)
            ans = max(ans, check(i, j));
    cout << ans << '\n';
    return 0;
}
```

## E. 生日相同

模板题。

```cpp
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
vector <string> v[13][35];

bool cmp(string &a, string &b)
{
    if (a.length() == b.length()) return a < b;
    return a.length() < b.length();
}

int main(void)
{
    int n;
    cin >> n;
    while (n--)
    {
        string s;
        int a, b;
        cin >> s >> a >> b;
        v[a][b].push_back(s);
    }
    for (int i = 1; i <= 12; ++i)
        for (int j = 1; j <= 31; ++j)
            if (!v[i][j].empty())
            {
                printf("%d %d ", i, j);
                sort(v[i][j].begin(), v[i][j].end(), cmp);
                for (auto s : v[i][j])
                    cout << s << ' ';
                putchar('\n');
            }
    return 0;
}
```

## F. 生日排序

模板题。

```cpp
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct baka
{
    string s;
    int a, b, c;
    bool operator < (const baka &a) const
    {
        if (this->a != a.a) return this->a < a.a;
        if (b != a.b) return b < a.b;
        if (c != a.c) return c < a.c;
        if (s.length() != a.s.length()) s.length() > a.s.length();
        return s > a.s;
    }
};

vector <baka> v;

int main(void)
{
    int n;
    cin >> n;
    while (n--)
    {
        baka t;
        cin >> t.s >> t.a >> t.b >> t.c;
        v.push_back(t);
    }
    sort(v.begin(), v.end());
    for (auto x : v)
        cout << x.s << '\n';
    return 0;
}
```

## G. 字符串还原

模板题。

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>

using namespace std;

int n, k, flag;
string s[3];

void calc(int i, int j) // 判断 j 是否从 i 改来
{
    string res = s[j];
    for (int p = 0; p < res.length(); ++p)
    {
        int c = res[p] - 'a';
        c = (c + k) % 26;
        res[p] = c + 'a';
    }
    if ((!(flag & 1)) && s[i] == res) return flag |= 1, void();
    res = s[j];
    for (int p = 0; p < res.length(); ++p)
    {
        int c = res[p] - 'a';
        c = (c + 26 - k) % 26;
        res[p] = c + 'a';
    }
    if ((!(flag & 2)) && s[i] == res) return flag |= 2, void();
}

int main(void)
{
    cin >> n >> s[0] >> s[1] >> s[2];
    for (int i = 0; i < 3; ++i)
    {
        reverse(s[i].begin(), s[i].end());
        for (k = 0; k <= 6; ++k)
        {
            flag = 0;
            for (int j = 0; j < 3; ++j)
                if (i != j) calc(i, j);
            if (flag == 3) return cout << s[i] << '\n', 0;
        }
        reverse(s[i].begin(), s[i].end());
    }
    return 0;
}
```

## 总结

我被封号了。