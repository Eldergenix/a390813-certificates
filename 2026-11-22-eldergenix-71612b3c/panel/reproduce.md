# PANEL_OUT — REPRODUCE seat, claim erdos-773-sidon-squares-next-term (p2, n = 76)

context: NEW cold session of claude-opus-5-5 in the REPRODUCE seat (claude, fresh session, claude-opus-5-5); the other vendor's model (GPT-6.2 Astra via codex exec) is unavailable (usage limit, probed again at 21:15 UTC on 2026-11-22).

## Definition used (quoted from CLAIM.md)
"every subset X of {1..76} that contains 76 and has all sums x^2 + y^2 (x <= y in X) distinct";
"D(N,T) = 'is there a valid X in {1..N} with N in X and |X| >= T?'". a(n) = A390813(n) = max size of such an X in {1..n}, with no requirement that it contains n.
Equivalent form I derived and used: x^2+y^2 = z^2+w^2 with {x<=y} != {z<=w} occurs only as a 3-set {z<x<w} with 2x^2 = z^2+w^2, or as a 4-set of distinct values a^2+b^2 = c^2+d^2. So X is valid iff it contains none of these "supports". At N=76 my generator gives 901 supports (24 triples, 877 quads). The packaged bb2 and dsearch logs give the same count.

All my files are in `mine/`. I wrote them before reading any packaged code.

## (1) Witnesses — own checker `mine/wcheck.py` (checks all pairs x <= y, including x = y)
- `python3 -I mine/wcheck.py 76 35 2 5 6 ... 74 75` -> `size 35 max 75 no N` / `VERDICT: PASS` (35 elements inside {1..75}, so a(75) >= 35 and a(76) >= 35)
- `python3 -I mine/wcheck.py 76 34 1 2 4 ... 75 76` -> `size 34 max 76 contains N` / `VERDICT: PASS` (g(76) >= 34)
- Negative control: `wcheck.py 10 1 1 5 7` -> `collision (1, 7) (5, 5) 50`, `VERDICT: FAIL`.
- The packaged checker/sidon_check.py also prints PASS on both witnesses.

## (2) Own exact search — `mine/mysearch.c` (clang -O3, no node cap)
Design: S starts as {N}. Candidates are branched in decreasing order (include, then exclude). Candidates are removed when they would complete a support. Pair conflicts come from supports that have exactly 2 unchosen elements left.
Bound: |S| + min over thresholds of (greedy clique cover of the candidates above the threshold + A[top candidate at or below the threshold]), and also the plain cover total. Splitting: subtrees at 3 included elements are numbered and assigned round-robin.
Every run was guarded with `perl -e 'alarm shift; exec @ARGV' SECONDS`. No alarm fired: every log ends with `# exit=0`. At most 3 of my search processes ran at once, checked with ps.

Calibration:
- `./mysearch 32 19 0 1 0 <a(1..31)>` -> `EXHAUSTED ... nodes=9627` (NO)
- `./mysearch 32 18 0 1 0 ...` -> `FOUND size=18: 32 31 30 29 28 27 26 25 24 21 20 19 16 13 10 9 6 3`; my checker gives PASS.
- `mine/selfcal.sh 68` (output: `mine/selfcal.out`) recomputes a(1..68) from scratch. Each step uses only this program's own earlier values as prefix bounds: a(n) = a(n-1)+1 if D(n, a(n-1)+1) is YES, else a(n-1).
  - Result: all 68 values equal status/oeis_b390813.txt (`diff` -> "ALL 68 MATCH B-FILE"). That includes a(60..68) = 29 30 30 30 31 31 31 32 32.
  - All 31 FOUND sets pass my checker and contain n.

Chain lemmas. I re-certified these myself, with the prefix = b-file a(1..68) followed by the upper bounds derived from my own earlier steps (logs in `mine/logschain/`):

| run | result | nodes | bb3/sq saved total |
|---|---|---|---|
| D(69,33) | EXHAUSTED, exit=0 | 231,529,645 | 231,532,144 |
| D(71,34) (A69=32, A70=33) | EXHAUSTED, exit=0 | 171,547,103 | 171,548,650 |
| D(72,34) (+A71=33) | EXHAUSTED, exit=0 | 732,761,947 | 732,764,446 |
| D(74,35) (+A72=33, A73=34) | EXHAUSTED, exit=0 | 323,240,777 | 323,242,324 |

Consequences: a(69) <= 32, a(70) <= 33, a(71) <= 33, a(72) <= 33, a(73) <= 34, a(74) <= 34, a(75) <= 35. These are exactly the prefix values used for N = 76. Only upper bounds are needed for pruning, so the chain witnesses at 70 and 73 are not needed.

Decisive D(76,35), prefix = b-file a(1..68) + 32 33 33 33 34 34 35 (identical to `programs/pref.sh 75`; checked with diff):
- Split run (`mine/run76.sh`, SD=3, P=8, logs `mine/logs76/my_N76_T35_p{0..7}of8.log`): all 8 parts are EXHAUSTED with exit=0, and each part reports serials=55. Part node counts: 874122381, 867255743, 336395975, 287681023, 279485335, 298319153, 219131323, 592769248. Sum: 3,755,160,181.
- Unsplit run (`mine/logs76/my_N76_T35_unsplit.log`): `EXHAUSTED N=76 T=35 SD=0 P=1 part=0 nodes=3755159579 cpu=1134.0s`, `# exit=0`.
  - The split total is 602 nodes larger. That is consistent with the shared top-of-tree nodes being counted again in each of the other 7 parts, so the two runs are consistent and deterministic.
- So D(76,35) = NO by my own program, and it finished in about 19 CPU-minutes.

Independence note: my program's tree turned out to be essentially the same as bb3/sq's. D(32,19) is identical (9,627 nodes), and the chain and N=76 totals agree to within a few thousand nodes of 3,755,162,596. That is convergent design (greedy descending clique cover + prefix bound), not a different tree.

Different-tree check: I rebuilt the packaged bb2 from source (`clang -O3`, binaries in `mine/pkgbuild/`). Its calibration gives D(32,19) EXHAUSTED and D(32,18) FOUND. I then reran all 8 parts of D(76,35) completely (`mine/runbb2.sh`, logs `mine/bb2rerun/`). Every part is EXHAUSTED with exit=0, and every node count matches the saved logs/p2_76 exactly:

| part | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| nodes | 229144237 | 330192619 | 261632167 | 338257049 | 191243661 | 260298313 | 229521417 | 267535005 |

The total is 2,107,824,468, the same as claimed. The rebuilt sq and bb3 also calibrate correctly on D(32,19) and D(32,18); I did not rerun them at N=76 because my own program covers that tree.

## (3) Soundness audit (target T = a(N), not a(N)+1)
- Mine: every pruning value is an upper bound on |X \ S| for any valid X that extends S inside the current candidates. Clique members pairwise complete a support with S, so X holds at most one per clique. Elements of X that are <= m form a Sidon set in {1..m}, so there are at most A[m] of them. Candidates are removed only when they would complete a support. No bound refers to a(N) or to the relation between T and a(N), so the search is sound for any T. Stopping the cover once the clique count reaches the number still needed is valid, because the later values can only be at least that count.
- bb2: clique and residual-support partition. Parts made from a 3-set or 4-set residual support among singleton vertices allow at most |P|-1 elements. The threshold count min(m(P), |P ∩ [v..k]|) is computed correctly because the members of P that are >= v form a top segment of P. A[v-1] applies only below v. Sound, and independent of T.
- bb3 and sq: the bound is #cliques with top >= v + min(A[v-1], #cliques with bottom < v). Every conflict they record is a genuine coincidence of sums. Sound, and independent of T.
- None of the programs uses a(N) itself or any assumption that T = a(N-1)+1.
- The chain logic is valid: a(n) <= a(n-1)+1, and every set either contains n or lies inside {1..n-1}.
- The final step is valid: a(76) <= max(a(75), max over sets containing 76 of size 36) = 35, using D(76,36) NO ⊆ D(76,35) NO. With the 35-set witness, a(76) = 35. With the 34-set witness, g(76) = 34. So no maximum set contains 76.
- pref.sh carries unverified values beyond index 75 ("35 35 35 36 ..."). For N <= 76 only A_1..A_75 are passed (I checked the command lines in the log headers), so those extra values are never used.

Log completeness (saved package logs):
- logs/p2_76: bb2, bb3 and sq each have parts 0-7, one EXHAUSTED line each, `# exit=0`, and no FOUND, alarm, error or killed. Totals: bb2 2,107,824,468; bb3 3,755,162,596; sq 3,755,162,596. bb3 and sq agree part by part.
- logs/chain: D(69,33), D(71,34), D(72,34) and D(74,35) have 8/8 parts for each of bb2, bb3 and sq, all EXHAUSTED with exit=0. The header prefixes have the right length and end in the right values. The N70/N73 FOUND logs are witnesses only.
- The two `KILLED_not_needed_*_N75_T35` logs (exit=143) are not used by any step of the argument.
- search4_referee/D76_35_all_parts.log: 280 of 280 parts say "EXHAUSTED ... D(76,35)=NO in this part". It has no exit-code lines, so it is supplementary only. I did not rely on it.

## Caveats (not failures of the reproduction)
- The exact searches share one branch-and-bound / clique-cover framework: mine, bb3, sq and dsearch walk the same tree, and bb2 walks a different one. There is no ILP, SAT or checked proof at n = 76. Whether this meets the "two independently written exact searches" wording is for the other seats to judge.
- What I confirm is the computation: both witnesses, the chain, D(76,35) = NO by my own complete run (split and unsplit), and the packaged bb2 complete rerun, which matches the saved logs part for part.

pass: true
