# Run it yourself (fresh shell, from this folder)

    sh verify-it-yourself/run_checks.sh            # seconds; audits witnesses and saved logs, calibrates on n = 32
    FULL=1 sh verify-it-yourself/run_checks.sh     # also reruns the decisive exclusion with bb2, about 1 CPU hour

Expected (default): a line ending `fourth program 280 of 280 parts: OK`, a line `calibration n=32 ... OK`, then `PASS`.
With FULL=1 also `FULL: N=76 T=35 bb2 parts exhausted with the logged node count: 8 of 8`.

Notation: D(N,T) = "is there a valid X in {1..N} with N in X and at least T elements?". Programs take
`prog N T D P part A_1 ... A_{N-1}` (A_k = upper bounds a(k) for the prefix; D=8 P=8 splits the tree into 8 parts).

    cc -O3 -o bb2 how-we-built-it/bb2.c ; cc -O3 -o bb3 how-we-built-it/bb3.c ; cc -O3 -o sq how-we-built-it/sq.c
    B="$(awk '$1 ~ /^[0-9]+$/ {printf "%s ", $2}' object/oeis_b390813_prefix.txt)"
    for p in 0 1 2 3 4 5 6 7; do ./bb3 69 33 8 8 $p $B; done                        # all EXHAUSTED -> a(69) = 32
    for p in 0 1 2 3 4 5 6 7; do ./bb3 71 34 8 8 $p $B 32 33; done                  # -> a(71) <= 33
    for p in 0 1 2 3 4 5 6 7; do ./bb3 72 34 8 8 $p $B 32 33 33; done               # -> a(72) <= 33
    for p in 0 1 2 3 4 5 6 7; do ./bb3 74 35 8 8 $p $B 32 33 33 33 34; done         # -> a(74) <= 34
    for p in 0 1 2 3 4 5 6 7; do ./bb3 76 35 8 8 $p $B 32 33 33 33 34 34 35; done   # -> no 35-set contains 76

and the same with ./bb2 and ./sq. Every line must print EXHAUSTED and no FOUND. The lower bounds are the witnesses
(a(70) >= 33 and a(73) >= 34 from the FOUND lines in object/logs/chain/, a(75) >= 35 and the 34-set containing 76
in object/a_p2_n76.json). Then a(76) = max(a(75), size of the largest valid set containing 76) = max(35, 34) = 35.
Saved logs of all three programs: object/logs/. A fourth program written blind in another session:
how-we-built-it/dsearch.c (usage in its header); its 280-part log of D(76,35) is
object/logs/search4_dsearch_D76_35_all_parts.log.

How to rebuild the object: the witnesses are printed by the searches when a set exists (run D(75,35) and D(76,34));
the statement is the 35-set inside {1..75} together with the exhaustion of D(76,35).
