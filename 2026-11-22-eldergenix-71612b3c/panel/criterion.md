# PANEL_OUT — CRITERION seat, partial win p2 (erdos-773-sidon-squares-next-term)

context: Claude Agent SDK (Claude Code harness), fresh unattended session, claude-fable-5-2; saw only this directory

## Verdict in one line
The package meets p2 as literally worded, on the reading that "two-method exact pipeline" means the main
criterion's "two independently written exact searches that agree" (the ILP is only its "e.g."); it does NOT meet
a stricter reading that demands two different algorithmic paradigms, and the scope sentence says so.

## What I checked myself

1. Range. n = 76 lies in 69..100. Yes.

2. Witnesses, by the definition with x = y sums included (my own script, plus checker/sidon_check.py, which
   uses combinations_with_replacement, i.e. includes x = y):
   - 35-set inside {1..75}: 35 distinct elements, all 630 sums x^2+y^2 (x <= y) distinct -> a(75) >= 35, a(76) >= 35.
   - 34-set containing 76: 34 distinct elements in 1..76, all 595 sums distinct -> g(76) >= 34.
   - the FOUND sets in logs/chain/ at N = 70 (33 elements, contains 70) and N = 73 (34, contains 73): valid.

3. Chain a(69..76) from the b-file prefix, every link:
   - programs/bfile_a1_68.txt equals the 68 values of status/oeis_b390813.txt (n = 1..68, ends "68 32").
   - a(69) <= 32: D(69,33) NO with A_1..A_68 = b-file. a(70) <= 33 trivially (a(n) <= a(n-1)+1).
     a(71) <= 33: D(71,34) NO. a(72) <= 33: D(72,34) NO. a(73) <= 34 trivially. a(74) <= 34: D(74,35) NO.
     a(75) <= 35 trivially. a(76) <= max(a(75), g(76)) = 35 since D(76,35) NO. Lower bounds: monotonicity from
     a(68) = 32 and the witnesses at 70, 73, 75. So a(69..76) = 32,33,33,33,34,34,35,35; nothing is used
     circularly (each D(N,T) run uses only A_1..A_{N-1}), and no value comes from outside the b-file.
   - Log audit (my own script over all files in logs/): for each of D(69,33), D(71,34), D(72,34), D(74,35),
     D(76,35) and each of bb2, bb3, sq: parts 0..7 all present, each with exactly one EXHAUSTED line carrying the
     right N, T, part, followed by "# exit=0"; no FOUND; the command line recorded in each log has the prefix
     b-file + 32 33 33 33 34 34 35 truncated to N-1 values; the split counter (leaves_at_split / subtrees /
     serials) is identical across the 8 parts of each run, as it must be if the parts partition one enumeration.
     Node totals at D(76,35): bb2 2,107,824,468; bb3 3,755,162,596; sq 3,755,162,596 (equal per part too).
   - search4_referee/D76_35_all_parts.log: 280 blocks, idx 0..279 each exactly once, each "EXHAUSTED part i/280:
     D(76,35)=NO in this part", same UB line (b-file + chain) and same tasks_total_seen = 35511 everywhere,
     sum of my_tasks = 35511, nodes + top_nodes = 1,877,579,790, no FOUND.
   - The two KILLED_not_needed logs (D(75,35), exit 143) are not links of the chain: a(75) = 35 comes from the
     witness and a(74) = 34; I did not count them.

4. Caps. None of bb2.c, bb3.c, sq.c, dsearch.c has a node or time limit. runjob.sh wraps runs in a 6-hour
   alarm, which would give a non-zero exit and no EXHAUSTED line; every counted log has EXHAUSTED and exit=0.

5. Pruning rules, read in the sources (all four sound as far as I can see):
   - prefix (Russian-doll) bound: the part of a completion below v is a Sidon-in-squares subset of {1..v-1},
     so it has at most A_{v-1} elements — valid for any upper bounds A_k >= a(k);
   - clique cover: greedy cliques of genuine pair conflicts (two candidates that complete a collision with the
     chosen set), at most one element per clique, minimised over the threshold v;
   - bb2 only: extra parts from collision supports whose unchosen part is 3 or 4 singleton live vertices (at most
     |part|-1 can be taken) — valid and disjoint by construction;
   - splitting: depth-D nodes numbered in DFS order, part = number mod P; the tree above the split does not
     depend on the part, so the parts cover every node.

6. Small run of my own (all four programs compiled fresh, b-file prefix, N = 4..46, unsplit): every program
   answers NO at T = a(N)+1 and YES at T = a(N) for every N except N = 32, where all four answer NO at T = 19 —
   i.e. each reproduces the b-file and Sievers' n = 32 exception. (bb2, bb3, sq also from N = 2.)

## The "two-method" question, decided
- bb3.c and sq.c are separately written but, as the package itself says, one design: identical node counts in
  every part. dsearch.c (collision-hyperedge counters, loop-style DFS) is also the same tree (half the count:
  it counts include-nodes only); I verified its bound is the same family. These three count as ONE method.
- bb2.c is a different program in substance: feasibility from a precomputed list of minimal collision supports
  with counters (bb3/sq use a table of used sums), an additional hypergraph bound, and a different, smaller
  tree (2.11e9 vs 3.76e9 nodes). It reaches the same NO on all five exclusions.
- So there are two exact searches that differ in data model, bound and tree and that agree: bb2 versus
  bb3/sq/dsearch. That satisfies "two independently written exact searches that agree". It is weaker than the
  criterion's example (branch and bound + ILP): both are branch and bound and both rest on the same two ideas
  (prefix bounds, clique cover) and on the same input prefix. A common-mode failure would therefore have to be
  a mathematical error in those shared ideas, which I checked by reading and found none, or an error in the
  OEIS values a(1..68), which the criterion explicitly allows as input.

## Not confirmed by this seat
- I did not rerun the long searches; the exclusions rest on the saved logs being genuine output of the
  included sources (the REPRODUCE seat's job).
- "Independently written" and "written blind" are authorship statements I cannot verify; I can only say the
  codes are visibly different programs.
- No ILP, SAT or machine-checked proof exists at n = 76 (the package says so).
- Novelty (that n = 76 is not already published) is outside this seat; status/ only shows the b-file ending
  at n = 68 and a third-party page reaching a(71).
- "Second" is exact only relative to the cited OEIS comment for n <= 68 plus this package's chain: for
  n = 69..75 a maximum set containing n^2 exists at 70, 73 and 75 (witnesses), while at 69, 71, 72, 74 the
  maximum equals a(n-1) and D(n, a(n)+1) = NO says nothing about g(n) = a(n); the package does not show
  whether 69, 71, 72 or 74 is itself such an index, so "the next after 32" is NOT established — only "another
  index in 69..100", which is what p2 asks for.

confirmed_scope: Given the OEIS b-file values a(1..68), the saved complete and uncapped logs of two differently built branch-and-bound searches (bb2.c on collision supports with an extra bound and its own tree, versus bb3.c/sq.c/dsearch.c on one shared tree; all four share the prefix-bound and clique-cover framework, and there is no ILP, SAT or checked proof) agree that a(69..76) = 32,33,33,33,34,34,35,35 and that no Sidon-in-squares subset of {1..76} containing 76 has 35 elements, and the two witnesses check by the definition with x = y sums included, so A390813(76) = 35 while every such set containing 76^2 has at most 34 elements; the searches were audited from logs and sources and calibrated on N <= 46, not rerun, and whether 69, 71, 72 or 74 is an earlier such index is not decided here.

pass: true
