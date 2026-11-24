# No maximum Sidon subset of the first 76 squares contains 76^2 (OEIS A390813, Erdős problem 773; a second such index after Sievers' n = 32)

## What
Given the OEIS b-file values a(1..68), the saved complete and uncapped logs of two differently built branch-and-bound searches (bb2.c on collision supports with an extra bound and its own tree, versus bb3.c/sq.c/dsearch.c on one shared tree; all four share the prefix-bound and clique-cover framework, and there is no ILP, SAT or checked proof) agree that a(69..76) = 32,33,33,33,34,34,35,35 and that no Sidon-in-squares subset of {1..76} containing 76 has 35 elements, and the two witnesses check by the definition with x = y sums included, so A390813(76) = 35 while every such set containing 76^2 has at most 34 elements; the searches were audited from logs and sources and calibrated on N <= 46, not rerun, and whether 69, 71, 72 or 74 is an earlier such index is not decided here.

## Check it yourself
`sh verify-it-yourself/run_checks.sh` from this folder (python3 and a C compiler). Expected last line: `PASS`.
It takes a few seconds: it checks both witnesses from the definition, audits every saved log of the chain and of
the decisive exclusion (all parts present, exhausted, exit 0, right prefix, node totals), and calibrates the
rebuilt programs on n = 32. It does NOT rerun the decisive search. For that:
`FULL=1 sh verify-it-yourself/run_checks.sh` reruns "a 35-element set in {1..76} containing 76" with bb2 (8 parts,
4 at a time, 2,107,824,468 nodes, roughly one CPU hour) and compares every part's node count with the saved log.
RUN_IT_YOURSELF.md gives the whole chain with the other programs.

## Compared to
| table | cell | value there | URL | fetched (UTC) |
|---|---|---|---|---|
| OEIS A390813, comment by C. Sievers | indices n where no maximum set contains n^2 | "Up to at least n=68, the case of n=32 is the only one"; nothing for n >= 69 | https://oeis.org/A390813 | 2026-11-22T21:18:51Z |
| OEIS A390813 b-file | a(76) | absent (last line "68 32") | https://oeis.org/A390813/b390813.txt | 2026-11-22T21:18:51Z |
| Erdős Problem a Day #773 | such an index in 69..100 | absent (conditional values through a(71) = 33, nothing on membership of n^2) | https://erdosproblemaday.com/day/773-square-sidon-frontier | 2026-11-22T21:18:52Z |
The statement here: a(76) = 35, and every Sidon-in-squares subset of {1..76} that contains 76 has at most 34
elements (34 is attained), so 76^2 lies in no maximum set.

## Not claimed
- Not "the next such index after 32": for n = 69, 71, 72 and 74 this package does not decide whether a maximum set
  contains n^2 (at 70, 73 and 75 one does). The claim is one index, n = 76, in the range 69..100.
- The value a(76) = 35 is the subject of an earlier package from the same run (2026-11-22-eldergenix-31ef6d4a) and
  a(72) = 33 of another (2026-11-22-eldergenix-bc2e12d9); here a(69..76) are re-certified as lemmas, as the
  partial-win text requires. Nothing about a(77) or later terms.
- No machine-checked (DRAT/LRAT) proof, no ILP and no SAT result at n = 76. The two "methods" are two differently
  built branch-and-bound searches (bb2 against bb3/sq/dsearch) that share the prefix-bound and clique-cover
  framework; a reader who requires two different paradigms should treat this as not meeting that bar.
- The three panel seats were separate fresh sessions of Anthropic models (the planned cross-vendor seat was
  unavailable), and the CRITERION seat was run twice because its first session was cut off by a time cap before
  giving a verdict; see panel/gate.md.

## Credits
Noga Alon and Paul Erdős (the problem); Thomas Bloom (https://www.erdosproblems.com/773); Giorgos Kalogeropoulos,
Michael S. Branicky and Christian Sievers (https://oeis.org/A390813: terms to n = 68, and Sievers' observation
about n = 32, which this claim extends by one index); Patrick White with Claude
(https://erdosproblemaday.com/day/773-square-sidon-frontier, conditional a(69..71) and the prefix / conflict-clique
bounding idea). Searches written by AI model sessions (Claude Opus 5.5, Claude Fable 5.2) run by Eldergenix under
the V3R1T4S kit (https://github.com/elder-plinius/V3R1T4S).
