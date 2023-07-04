#include <bits/stdc++.h>
using namespace std; 
typedef long long i64; 
 
int n; 
char a[100005], b[100005]; 
set<int> p[26]; 
int C[100005]; 
void add(int x, int k) { for (; x <= n; x += x & -x) C[x] += k; }
int calc(int x) { 
	int r = x; 
	for (int i = x; i; i -= i & -i) r += C[i]; 
	return r; 
}
 
int main(void) {
	int T; scanf("%d", &T); 
	while (T--) {
		
		scanf("%d%s%s", &n, a + 1, b + 1); i64 ans = 1e18, res = 0; 
		
		
		for (int i = 0; i < 26; ++i) p[i].clear(); 
		fill(C + 1, C + n + 1, 0); 
		
		for (int i = 1; i <= n; ++i) a[i] -= 'a', b[i] -= 'a'; 
		for (int i = 1; i <= n; ++i) p[a[i]].insert(i); 
		for (int i = 1; i <= n; ++i) {
			int t = 1e9;
			for (int c = 0; c < b[i]; ++c) if (p[c].size()) t = min(t, calc(*p[c].begin())); 
			if (t != 1e9) ans = min(ans, res + (t - i)); 
		
			if (p[b[i]].size()) {
				t = calc(*p[b[i]].begin()); res += t - i; 
				p[b[i]].erase(p[b[i]].begin()); 
				add(i, 1); add(t + 1, -1); 
			} else break; 
		}
		printf("%lld\n", ans == 1e18 ? -1 : ans); 
	}
	return 0;
}