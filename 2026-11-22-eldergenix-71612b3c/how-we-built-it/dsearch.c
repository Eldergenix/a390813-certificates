/* dsearch.c -- referee's own exact search, written from the definition.
   D(N,T): is there X in {1..N}, N in X, |X|>=T, with all x^2+y^2 (x<=y in X) distinct?
   Hypergraph formulation: X is invalid iff it contains all members of some "edge"
   {a,b}u{c,d} with a^2+b^2=c^2+d^2, {a,b}!={c,d} (a=b allowed -> 3-element edge;
   two distinct representations of one number can never share an element).
   DFS choosing elements in strictly decreasing order, starting with N (forced).
   Bounds (both valid for ANY target T, including T = a(N)):
     (B4) see rec(): greedy cover of the allowed candidates by groups of pairwise-conflicting
          candidates, combined with prefix bounds UB[j] >= a(j) (bound on ANY valid subset of {1..j}).
   No node cap and no time cap inside the program.
   usage: dsearch N T parts idx bfile [k=v ...]   (k=v: sets upper bound UB[k]=v)
   The search tree is split at depth SPLIT (default 3: N, 2nd largest, 3rd largest; env DSPLIT): depth-SPLIT nodes are
   numbered in DFS order and part idx handles those with number % parts == idx.
   exit 0 = EXHAUSTED (answer NO for this part), exit 10 = FOUND (YES).
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
typedef unsigned __int128 u128;
typedef uint64_t u64;
#define MAXN 120
#define MAXE 400000
static int SPLIT=3;   /* split depth; env DSPLIT overrides */
static int N,T,PARTS,IDX;
static int UB[MAXN+1];
static int ne, esz[MAXE], etot[MAXE], cnt[MAXE], csum[MAXE], em[MAXE][4];
static int *el[MAXN+1], eln[MAXN+1];
static u64 nodes, topnodes, taskctr, mytasks;
static int chosen[MAXN+2];
static u128 adj[MAXN+1];                      /* adj[u]: candidates w that cannot join X together with u (given current X) */
static struct { int v; u128 old; } ustack[1<<20]; static int usp;
static inline void setadj(int u,int w){ ustack[usp].v=u; ustack[usp].old=adj[u]; usp++; adj[u]|=(u128)1<<w; }
static inline int pc(u128 x){ return __builtin_popcountll((u64)x)+__builtin_popcountll((u64)(x>>64)); }
static inline int hb(u128 x){ u64 h=(u64)(x>>64); return h? 127-__builtin_clzll(h) : 63-__builtin_clzll((u64)x); }
static void addedge(int a,int b,int c,int d){
  int m[4],k=0; m[k++]=a; if(b!=a)m[k++]=b; m[k++]=c; if(d!=c)m[k++]=d;
  for(int i=0;i<k;i++)for(int j=i+1;j<k;j++) if(m[i]==m[j]){fprintf(stderr,"edge overlap bug\n");exit(3);}
  if(k<3||ne>=MAXE){fprintf(stderr,"edge bug\n");exit(3);}
  esz[ne]=k; etot[ne]=0; for(int i=0;i<k;i++){etot[ne]+=m[i]; em[ne][i]=m[i];}
  ne++;
}
static char inX[MAXN+1];
static int add(int j,u128 *forb){            /* put j into X; collect newly forbidden elements and new pair conflicts */
  int *L=el[j], n=eln[j]; u128 f=0;
  for(int i=0;i<n;i++){ int e=L[i]; cnt[e]++; csum[e]+=j;
    if(cnt[e]==esz[e]-1) f|=(u128)1<<(etot[e]-csum[e]);
    else if(cnt[e]==esz[e]-2){               /* exactly two members u,w not in X: u,w cannot both be added */
      int u=-1,w=-1; for(int t=0;t<esz[e];t++){ int m=em[e][t]; if(!inX[m]){ if(u<0)u=m; else w=m; } }
      /* inX[j] is set by caller before add */
      setadj(u,w); setadj(w,u); }
    else if(cnt[e]==esz[e]){fprintf(stderr,"internal: completed edge\n");exit(3);} }
  *forb=f; return 0;
}
static void del(int j,int sp){ int *L=el[j], n=eln[j]; for(int i=0;i<n;i++){ int e=L[i]; cnt[e]--; csum[e]-=j; }
  while(usp>sp){ usp--; adj[ustack[usp].v]=ustack[usp].old; } }
static int rec(int c,u128 avail){
  if(c==SPLIT){ if((int)(taskctr++ % (u64)PARTS)!=IDX) return 0; mytasks++; }
  if(c<SPLIT) topnodes++; else nodes++;
  if(c>=T) return 1;
  while(avail){
    /* B4. Let v1>v2>...>vp be the allowed candidates, processed in this order and greedily packed
       into groups of pairwise-conflicting candidates (a group can contribute at most ONE element to X,
       because two candidates u,w with adj-conflict complete an edge together with elements already in X).
       After the first i candidates are packed into q_i groups, the rest of X consists of at most q_i
       elements among {v1..vi} plus a valid set inside {1..v_{i+1}} (at most UB[v_{i+1}], UB[0]=0).
       So rest <= min_i (q_i + UB[v_{i+1}]).  Valid for any target T (nothing here assumes T > a(N)). */
    /* Implementation: group-major first-fit (group k = highest unassigned candidate s_k plus, scanning
       downwards, every candidate conflicting with all members so far). This is the same partition as
       candidate-major first-fit, and min_i(q_i+UB[v_{i+1}]) is attained at group seeds, so it is
       enough to test (k-1)+UB[s_k] >= need at each seed and q_total >= need at the end. */
    int need=T-c, ok=1, q=0; u128 rem=avail;
    while(rem){
      int sd=hb(rem);
      if(q+UB[sd]<need){ok=0;break;}
      q++; if(q>=need) break;
      rem&=~((u128)1<<sd);
      u128 Q=rem&adj[sd];
      while(Q){ int w=hb(Q); u128 bw=(u128)1<<w; rem&=~bw; Q&=adj[w]; Q&=~bw; }
    }
    if(ok && q<need) ok=0;
    if(!ok) break;
    int j=hb(avail);
    avail&=~((u128)1<<j);
    int sp=usp; u128 forb; inX[j]=1; add(j,&forb);
    chosen[c]=j;
    int r=rec(c+1,avail&~forb);
    del(j,sp); inX[j]=0;
    if(r) return 1;
  }
  return 0;
}
int main(int argc,char**argv){
  if(argc<6){fprintf(stderr,"usage: dsearch N T parts idx bfile [k=v...]\n");return 2;}
  N=atoi(argv[1]);T=atoi(argv[2]);PARTS=atoi(argv[3]);IDX=atoi(argv[4]);
  if(N<1||N>MAXN||PARTS<1||IDX<0||IDX>=PARTS||T<1) return 2;
  if(getenv("DSPLIT")) SPLIT=atoi(getenv("DSPLIT"));
  if(SPLIT<2||SPLIT>=T) return 2;
  FILE*f=fopen(argv[5],"r"); if(!f){perror("bfile");return 2;}
  char line[256];
  for(int i=0;i<=MAXN;i++) UB[i]=-1;
  UB[0]=0;
  while(fgets(line,sizeof line,f)){ int k,v; if(line[0]=='#')continue; if(sscanf(line,"%d %d",&k,&v)==2 && k>=1&&k<=MAXN) UB[k]=v; }
  fclose(f);
  for(int i=6;i<argc;i++){ int k,v; if(sscanf(argv[i],"%d=%d",&k,&v)==2&&k>=1&&k<=MAXN) UB[k]=v; else {fprintf(stderr,"bad arg %s\n",argv[i]);return 2;} }
  for(int i=1;i<N;i++) if(UB[i]<0){ fprintf(stderr,"no bound for %d\n",i); return 2; }
  UB[N]=N;   /* a(N) is NOT an input: trivial bound for the forced top element */
  /* edges from the definition */
  static int rep[2*MAXN*MAXN+1];
  for(int pass=0;pass<2;pass++){
    ne=0;
    for(int a=1;a<=N;a++)for(int b=a;b<=N;b++){ int s=a*a+b*b;
      for(int c=a;c<=N;c++)for(int d=c;d<=N;d++){
        if(c*c+d*d!=s) continue;
        if(c==a&&d<=b) continue;          /* each unordered pair of representations once */
        addedge(a,b,c,d);
      }}
    if(pass==0){ for(int e=0;e<ne;e++)for(int i=0;i<esz[e];i++)eln[em[e][i]]++;
      for(int v=1;v<=N;v++){ el[v]=malloc(sizeof(int)*(eln[v]+1)); eln[v]=0; }
      for(int e=0;e<ne;e++)for(int i=0;i<esz[e];i++){int v=em[e][i]; el[v][eln[v]++]=e;} }
  }
  (void)rep;
  int n3=0; for(int e=0;e<ne;e++) n3+=(esz[e]==3);
  printf("dsearch v3 N=%d T=%d split_depth=%d parts=%d idx=%d pid=%d edges=%d (3-sets %d, 4-sets %d)\nUB:",N,T,SPLIT,PARTS,IDX,(int)getpid(),ne,n3,ne-n3);
  for(int i=1;i<N;i++) printf(" %d",UB[i]);
  printf("\n"); fflush(stdout);
  u128 avail=0; for(int j=1;j<N;j++) avail|=(u128)1<<j;
  u128 forb; inX[N]=1; add(N,&forb); chosen[0]=N;
  int r=rec(1,avail&~forb);
  if(r){ printf("FOUND size>=%d:",T); for(int i=T-1;i>=0;i--) printf(" %d",chosen[i]); printf("\n"); }
  printf("tasks_total_seen=%llu my_tasks=%llu top_nodes=%llu nodes=%llu\n",(unsigned long long)taskctr,(unsigned long long)mytasks,(unsigned long long)topnodes,(unsigned long long)nodes);
  printf(r? "RESULT D(%d,%d)=YES\n" : "EXHAUSTED part %d/%d: D(%d,%d)=NO in this part\n", r?N:IDX, r?T:PARTS, N, T);
  fflush(stdout);
  return r?10:0;
}
