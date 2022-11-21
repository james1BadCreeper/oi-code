#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
const int MOD = 45989;

int N;
struct Matrix {
    int a[105][105];
    Matrix() { memset(a, 0, sizeof(a)); }
    Matrix operator < (const Matrix &b) const {
        Matrix c;
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k) {
                int r = a[i][k];
                for (int j = 0; j < N; ++j)    
                    c.a[i][j] = (c.a[i][j] + r * b.a[k][j]) % MOD;
            }
        return c;
    }
};

int n, m, t, a, b;

int main(void)
{
    scanf("%d%d%d%d%d", &n, &m, &t, &a, &b);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
    }
    return 0;
}