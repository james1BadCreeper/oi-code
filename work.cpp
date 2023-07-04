#include <bits/stdc++.h>
using namespace std; 

int n, m, tot; 
int t[100005], a[100005], l[100005], r[100005]; 

int main(void) {
    scanf("%d%d", &n, &m); 
    for (int i = 1; i <= n; ++i) {
        scanf("%d", t + i); 
        if (i != 1 && (t[i] >= 0) == (t[i - 1] >= 0)) a[tot] += t[i]; 
        else a[++tot] = t[i];     
    }
    int cnt = 0, ans; 
    for (int i = 1; i <= tot; ++i) {
        if (a[i] >= 0) ans += a[i], ++cnt; 
        l[i] = i - 1, r[i] = i + 1; 
    }
    while (cnt > m) {
        --cnt; 
    }
    
    return !printf("%d\n", ans); 
}