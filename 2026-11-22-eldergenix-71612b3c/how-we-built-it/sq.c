/* sq.c -- lens "extender", written from the definition (independent of bb2.c / bb3.c).
 *
 * Question decided: is there X subset {1..N}, N in X, |X| >= T, such that all x^2+y^2 (x<=y in X) are distinct?
 *
 * usage: sq N T D P part A_1 ... A_{N-1}
 *   A_k must be valid UPPER bounds on a(k) (max Sidon-in-squares subset of {1..k}); A_0 = 0.
 *   D>0: every node reached after exactly D branchings (D include/exclude decisions below N) gets a serial number
 *   in DFS order; only serials with serial % P == part are explored.  Nodes above depth D are explored by every part.
 *   Output: FOUND + witness (re-verified from scratch) or EXHAUSTED + node count.  No time limit, no node cap.
 *
 * State along a path: S (chosen, all > k), tables
 *   used[s]  = 1 iff s = x^2+y^2 for some x<=y in S
 *   dif[d]   = #{ (s,t) in S, s<t : t^2-s^2 = d }
 *   insq[v]  = 1 iff v = s^2 for some s in S
 * Candidate test (w not in S, w < min S):  S+{w} Sidon  <=>  2w^2 and w^2+s^2 (s in S) are all unused.
 * Pair test for candidates u<w (each individually addable): S+{u,w} Sidon  <=>  none of
 *   used[u^2+w^2];  dif[w^2-u^2] > 0  (u^2+t^2 = w^2+s^2, s<t in S);  insq[2w^2-u^2]  (u^2+s^2 = 2w^2);
 *   insq[2u^2-w^2] (w^2+s^2 = 2u^2).
 * (All other coincidences among new sums force equal indices.)  The conflict graph is recomputed from scratch with
 * these tests after every inclusion; an exclusion leaves S and hence the graph unchanged.
 * Bound at a node with candidate set C (all < current k) and |S| = c:
 *   greedy clique partition of C (vertices in decreasing order, first fit).  For any completion X':
 *   |X'| <= #{cliques whose max >= v} + min(A_{v-1}, #{cliques whose min < v})  for every v in 1..k+1.
 *   prune if c + min_v(...) < T.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef unsigned __int128 u128;
#define ONE ((u128)1)
#define MAXN 120

static int N, T, D, P, PART;
static int A[MAXN + 2];
static uint8_t used[2 * MAXN * MAXN + 4];
static int dif[MAXN * MAXN + 4];
static uint8_t insq[2 * MAXN * MAXN + 4];
static int S[MAXN + 2], ns;
static long long nodes, serial;
static time_t t0;

static inline int hib(u128 x) {
    uint64_t h = (uint64_t)(x >> 64);
    if (h) return 127 - __builtin_clzll(h);
    return 63 - __builtin_clzll((uint64_t)x);
}
static inline int cnt128(u128 x) { return __builtin_popcountll((uint64_t)x) + __builtin_popcountll((uint64_t)(x >> 64)); }

static int addable(int w) {
    if (used[2 * w * w]) return 0;
    for (int i = 0; i < ns; i++) if (used[w * w + S[i] * S[i]]) return 0;
    return 1;
}
static void push(int w) {
    for (int i = 0; i < ns; i++) { used[w * w + S[i] * S[i]] = 1; dif[S[i] * S[i] - w * w]++; }
    used[2 * w * w] = 1; insq[w * w] = 1;
    S[ns++] = w;
}
static void pop(void) {
    int w = S[--ns];
    for (int i = 0; i < ns; i++) { used[w * w + S[i] * S[i]] = 0; dif[S[i] * S[i] - w * w]--; }
    used[2 * w * w] = 0; insq[w * w] = 0;
}
static int conflict(int u, int w) { /* u < w, both addable */
    int u2 = u * u, w2 = w * w;
    if (used[u2 + w2]) return 1;
    if (dif[w2 - u2]) return 1;
    if (insq[2 * w2 - u2]) return 1;
    if (2 * u2 > w2 && insq[2 * u2 - w2]) return 1;
    return 0;
}
static int verify(void) {
    static uint8_t tab[2 * MAXN * MAXN + 4];
    memset(tab, 0, sizeof tab);
    for (int i = 0; i < ns; i++) for (int j = i; j < ns; j++) {
        int s = S[i] * S[i] + S[j] * S[j];
        if (tab[s]) return 0;
        tab[s] = 1;
    }
    for (int i = 0; i < ns; i++) if (S[i] < 1 || S[i] > N) return 0;
    for (int i = 0; i < ns; i++) for (int j = i + 1; j < ns; j++) if (S[i] == S[j]) return 0;
    return 1;
}

static int bound(u128 C, const u128 *adj) {
    u128 mem[MAXN + 2]; int top[MAXN + 2], bot[MAXN + 2], q = 0;
    if (!C) return 0;
    int k = hib(C);
    u128 x = C;
    while (x) {
        int v = hib(x); x &= ~(ONE << v);
        int j;
        for (j = 0; j < q; j++) if ((mem[j] & adj[v]) == mem[j]) break; /* v adjacent to every member */
        if (j == q) { mem[q] = 0; top[q] = v; q++; }
        mem[j] |= ONE << v; bot[j] = v;
    }
    int ntop[MAXN + 2], nbot[MAXN + 2];
    memset(ntop, 0, sizeof(int) * (k + 2)); memset(nbot, 0, sizeof(int) * (k + 2));
    for (int j = 0; j < q; j++) { ntop[top[j]]++; nbot[bot[j]]++; }
    int best = q < A[k] ? q : A[k]; /* v = k+1: all of X' lies in [1..k] */
    int hi = 0, botge = 0;
    for (int v = k; v >= 1; v--) {
        hi += ntop[v]; botge += nbot[v];
        int lo = q - botge; if (A[v - 1] < lo) lo = A[v - 1];
        if (hi + lo < best) best = hi + lo;
    }
    return best;
}

static void build_adj(u128 C, u128 *adj) {
    int L[MAXN + 2], m = 0;
    u128 x = C;
    while (x) { int v = hib(x); x &= ~(ONE << v); L[m++] = v; }
    for (int i = 0; i < m; i++) adj[L[i]] = 0;
    for (int i = 0; i < m; i++) for (int j = i + 1; j < m; j++) { /* L[i] > L[j] */
        if (conflict(L[j], L[i])) { adj[L[i]] |= ONE << L[j]; adj[L[j]] |= ONE << L[i]; }
    }
}

static void found(void) {
    if (!verify() || S[0] != N) { fprintf(stderr, "INTERNAL ERROR witness\n"); exit(2); }
    printf("FOUND size=%d :", ns);
    for (int i = ns - 1; i >= 0; i--) printf(" %d", S[i]);
    printf("\nnodes=%lld time=%lds\n", nodes, (long)(time(NULL) - t0));
    fflush(stdout); exit(0);
}

/* C: candidates (all < every element of S), adj valid for C, depth = branchings so far */
static void dfs(u128 C, u128 *adj, int depth) {
    nodes++;
    if ((nodes & ((1LL << 25) - 1)) == 0)
        fprintf(stderr, "progress nodes=%lld serial=%lld ns=%d t=%lds\n", nodes, serial, ns, (long)(time(NULL) - t0));
    if (ns >= T) found();
    if (!C) return;
    if (D > 0 && depth == D) { if ((serial++ % P) != PART) return; }
    if (ns + bound(C, adj) < T) return;
    int k = hib(C);
    u128 rest = C & ~(ONE << k);
    /* include k */
    push(k);
    {
        u128 C2 = 0, x = rest & ~adj[k];
        while (x) { int w = hib(x); x &= ~(ONE << w); if (addable(w)) C2 |= ONE << w; }
        /* sanity: the pair test must agree with the direct addability test */
        if (C2 != (rest & ~adj[k])) { fprintf(stderr, "INTERNAL ERROR pair test incomplete\n"); exit(2); }
        u128 adj2[MAXN + 2];
        build_adj(C2, adj2);
        dfs(C2, adj2, depth + 1);
    }
    pop();
    /* exclude k */
    dfs(rest, adj, depth + 1);
}

int main(int argc, char **argv) {
    if (argc < 6) { fprintf(stderr, "usage: sq N T D P part A_1..A_{N-1}\n"); return 1; }
    N = atoi(argv[1]); T = atoi(argv[2]); D = atoi(argv[3]); P = atoi(argv[4]); PART = atoi(argv[5]);
    if (N < 1 || N > MAXN || P < 1 || PART < 0 || PART >= P) { fprintf(stderr, "bad args\n"); return 1; }
    if (argc != 5 + N) { fprintf(stderr, "need %d prefix values\n", N - 1); return 1; }
    A[0] = 0; for (int k = 1; k < N; k++) A[k] = atoi(argv[5 + k]);
    t0 = time(NULL);
    ns = 0; push(N);
    u128 C = 0;
    for (int w = 1; w < N; w++) if (addable(w)) C |= ONE << w;
    static u128 adj[MAXN + 2];
    build_adj(C, adj);
    printf("N=%d T=%d candidates_after_N=%d\n", N, T, cnt128(C));
    dfs(C, adj, 0);
    printf("EXHAUSTED N=%d T=%d D=%d P=%d part=%d nodes=%lld serials=%lld time=%lds\n", N, T, D, P, PART, nodes, serial,
           (long)(time(NULL) - t0));
    return 0;
}
