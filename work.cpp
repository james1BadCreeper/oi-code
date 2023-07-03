#include <bits/stdc++.h>
using namespace std; 

int n, m; 
int nxt[300005], hd, pre[300005]; 
int mn[300005], mx[300005], rnk[300005]; 

int main(void) {
    scanf("%d%d", &n, &m); hd = 1; 
    for (int i = 1; i < n; ++i) nxt[i] = i + 1, pre[i + 1] = i; 
    for (int i = 1; i <= n; ++i) mn[i] = mx[i] = i; 
    while (m--) {
        int b; scanf("%d", &b); 
        if (b == hd) continue; 
        int a = pre[b], c = nxt[b]; 
        nxt[a] = c; 
        if (c) pre[c] = a; 
        nxt[b] = hd, pre[b] = 0, hd = b; 
        
    }
    return 0; 
}