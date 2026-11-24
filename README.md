# Certificates for OEIS A390813, n = 69..76 (Erdős problem #773)

A390813(n) is the size of the largest Sidon subset of the first n positive squares
(all sums a + b with a <= b distinct). The OEIS b-file ends at n = 68. This repository
holds the evidence for

| n | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 |
|---|---|---|---|---|---|---|---|---|
| a(n) | 32 | 33 | 33 | 33 | 34 | 34 | 35 | 35 |

and for one further fact: **no maximum set for n = 76 contains 76^2** (every Sidon-in-squares
subset of {1..76} containing 76 has at most 34 elements). Up to n = 68 the only such index was n = 32
(Christian Sievers, OEIS comment).

## Packages

| folder | claim | quick check |
|---|---|---|
| `2026-11-22-eldergenix-bc2e12d9/` | a(72) = 33 | about 2 CPU minutes |
| `2026-11-22-eldergenix-31ef6d4a/` | a(76) = 35, with the chain a(69..76) | about 10 CPU minutes |
| `2026-11-22-eldergenix-71612b3c/` | no maximum set for n = 76 contains 76^2 | seconds; `FULL=1` reruns the decisive search (about 1 CPU hour) |

In each folder: `sh verify-it-yourself/run_checks.sh` (python3 and a C/C++ compiler). The last line must be `PASS`.
Each package has its own README with exactly what is and is not claimed, the witnesses (`object/`), logs, search
code, and the review panel's reports.

All four checks (including the `FULL=1` rerun) were run again on 2026-11-24 and printed `PASS`.

## What this is not
- Not a machine-checked proof (no DRAT/LRAT). The upper bounds rest on exhaustive branch-and-bound searches,
  each confirmed by at least two separately written programs that agree.
- The searches take the published values a(1..68) as prefix bounds.
- The search programs were written by AI model sessions (Anthropic Claude, OpenAI GPT) and reviewed by
  independent AI sessions of one vendor; the contributor reran the verification scripts.
- Nothing about the asymptotic question (is a(n) = n^(1-o(1))?), which remains open.

## Credits
Noga Alon and Paul Erdős (the problem); Thomas Bloom (erdosproblems.com/773); Giorgos Kalogeropoulos,
Michael S. Branicky and Christian Sievers (A390813, terms to n = 68); Patrick White with Claude
(Erdős Problem a Day, conditional a(69..71) and the prefix / conflict-clique bounding idea).

Contributor: Stefan Creadore (Eldergenix).
