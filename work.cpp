#include <iostream>
#include <cstdio>

using namespace std;
typedef long long i64;

i64 l, r;
i64 f[20], poww[20];

int main(void)
{
    cin >> l >> r;
    poww[0] = 1;
    for (int i = 1; i <= 13; ++i) {
        f[i] = f[i - 1] * 10 + poww[i - 1]; 
        poww[i] = poww[i - 1] * 10;
    }
    for (int i = 0; i < 10; ++i)
    return 0;
}

// 5499 - 3000 = 2499