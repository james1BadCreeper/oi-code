#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

i64 l, r;
i64 f[15], poww[15];
i64 ans1[15], ans2[15];

void calc(i64 n, i64 *ans)
{
    static int a[15];
    int len = 0; i64 tmp = n;
    while (tmp) a[++len] = tmp % 10, n /= 10;
    for (int i = len; i >= 1; --i) {
        for (int j = 0; j < 10; ++j) ans[j] += f[i - 1] * a[i];
        for (int j = 0; j < a[i]; ++j) ans[j] += poww[i - 1];
        n -= poww[i - 1] * a[i]; ans[a[i]] += n + 1;
        ans[0] -= poww[i - 1];
    }
}

int main(void)
{
    cin >> l >> r;
    poww[0] = 1;
    for (int i = 1; i <= 13; ++i) {
        f[i] = f[i - 1] * 10 + poww[i - 1]; 
        poww[i] = poww[i - 1] * 10;
    }
    calc(r, ans1); calc(l - 1, ans2);
    for (int i = 0; i < 10; ++i)
        printf("%lld ", ans1[i] - ans2[i]);
    putchar('\n');
    return 0;
}