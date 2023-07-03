#include <bits/stdc++.h>
using namespace std; 
typedef long long i64; 

int n, m; 
int a[500005], tag[2000005]; 
i64 T[2000005], s[500005]; 

inline void pushdown(int o) {
    if (!tag[o]) return;
    tag[o << 1] += tag[o]; tag[o << 1 | 1] += tag[o]; 
    T[o << 1] += tag[o]; T[o << 1 | 1] += tag[o]; 
    tag[o] = 0; 
}

void update(int o, int l, int r, int x, int y, int k) {
    if (x <= l && r <= y) return tag[o] += k, T[o] += k, void(); 
    pushdown(o); int mid = l + r >> 1; 
    if (x <= mid) update(o << 1, l, mid, x, y, k); 
    if (mid < y) update(o << 1 | 1, mid + 1, r, x, y, k); 
    T[o] = max(T[o << 1], T[o << 1] | 1); 
}

int ans = -1; 
void query(int o, int l, int r, int x, int y) {
    if (T[o] < 0 || ans != -1) return; 
    if (l == r) {
        if (T[o] == 0) ans = l; 
        return; 
    }
    pushdown(o); int mid = l + r >> 1; 
    if (x <= mid) query(o << 1, l, mid, x, y); 
    if (mid < y) query(o << 1 | 1, mid + 1, r, x, y); 
}

int main(void) {
    scanf("%d%d", &n, &m); 
    for (int i = 1; i <= n; ++i) {
        scanf("%d", a + i), s[i] = s[i - 1] + a[i]; 
        update(1, 1, n, i, i, a[i] - s[i - 1]);  
    }
    while (m--) {
        int p, x; scanf("%d%d", &p, &x); 
        update(1, 1, n, p, p, x - a[p]); 
        if (p < n) update(1, 1, n, p + 1, n, a[p] - x); 
        ans = -1; query(1, 1, n, 1, n); 
        printf("%d\n", ans); a[p] = x; 
    }
    return 0; 
}