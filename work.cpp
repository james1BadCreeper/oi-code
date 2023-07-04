#include <bits/stdc++.h>
using namespace std; 
typedef long long i64; 

int n, q; 
int a[6000005], fa[6000005]; 
int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
void merge(int x, int y) { if ((x = find(x)) != (y = find(y))) fa[y] = x; }

struct Node {
    int pre, nxt, x; 
    i64 tag; 
} lk[6000005]; 
int cnt, hd; 

void addlk(int x) { lk[++cnt].x = x; lk[cnt].nxt = hd; lk[hd].pre = cnt; hd = cnt; }
void erase(int x) { lk[lk[x].pre].nxt = lk[x].nxt; lk[lk[x].nxt].pre = lk[x].pre; }

int main(void) {
    scanf("%d%d", &n, &q); 
    for (int i = 1; i <= n; ++i) scanf("%d", a + i); 
    while (q--) {
        int op, x, y; scanf("%d%d%d", &op, &x, &y); 
        if (op == 1) {

        } else {
            int p = query(x - 1), q = query(y); 
            if (p != q) puts("0"); 
            else printf("%d\n", x - p); 
        }
    }
    return 0; 
}