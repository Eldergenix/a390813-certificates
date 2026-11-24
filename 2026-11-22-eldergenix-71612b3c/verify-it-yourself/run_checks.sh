#!/bin/sh
# Run from the package folder: sh verify-it-yourself/run_checks.sh   (last line: PASS or FAIL)
# Default (under a minute): 1. witnesses from the definition and a full audit of the saved logs (check_p2.py);
#   2. rebuilds bb2 and bb3 and calibrates them on Sievers' n = 32: "a 19-set containing 32" is exhausted without
#      a set, "an 18-set containing 32" is found.
# FULL=1 sh verify-it-yourself/run_checks.sh additionally reruns the decisive exclusion D(76,35) with bb2
#   (8 parts, 4 at a time, about 2.1e9 nodes, roughly 1 CPU hour) and compares every part's node count with the log.
fail() { echo "$1"; echo FAIL; exit 1; }
python3 -I -B verify-it-yourself/check_p2.py || fail "object or log check failed"
D=$(mktemp -d) || fail "mktemp"
cc -O3 -o "$D/bb2" verify-it-yourself/bb2.c 2> "$D/build2.log" || fail "compile bb2 failed"
cc -O3 -o "$D/bb3" verify-it-yourself/bb3.c 2> "$D/build3.log" || fail "compile bb3 failed"
B=$(awk '$1 ~ /^[0-9]+$/ {printf "%s ", $2}' object/oeis_b390813_prefix.txt)
B31=$(echo $B | tr ' ' '\n' | head -n 31 | tr '\n' ' ')
for g in bb2 bb3; do
  "$D/$g" 32 19 0 1 0 $B31 > "$D/$g.19" 2>&1 || fail "$g D(32,19) run"
  grep -q "^EXHAUSTED" "$D/$g.19" && ! grep -q "FOUND" "$D/$g.19" || fail "$g: D(32,19) should be exhausted"
  "$D/$g" 32 18 0 1 0 $B31 > "$D/$g.18" 2>&1
  grep -q "FOUND size=18" "$D/$g.18" || fail "$g: D(32,18) should find a set"
done
echo "calibration n=32 (no 19-set contains 32, an 18-set does) with bb2 and bb3: OK"
if [ "$FULL" = 1 ]; then
  C=$(python3 -I -c "import json;c=json.load(open('object/a_p2_n76.json'))['chain'];print(' '.join(str(c[str(k)]) for k in range(69,76)))") || fail "chain"
  for q in "0 1 2 3" "4 5 6 7"; do
    for p in $q; do ( "$D/bb2" 76 35 8 8 $p $B $C > "$D/p$p.log" 2>&1; echo "exit=$?" >> "$D/p$p.log" ) & done
    wait
  done
  n=0
  for p in 0 1 2 3 4 5 6 7; do
    a=$(grep "^EXHAUSTED" "$D/p$p.log" | sed 's/.*nodes=\([0-9]*\).*/\1/')
    b=$(grep "^EXHAUSTED" object/logs/p2_76/bb2_N76_T35_p${p}of8.log | sed 's/.*nodes=\([0-9]*\).*/\1/')
    grep -q "^exit=0" "$D/p$p.log" && ! grep -q "FOUND" "$D/p$p.log" && [ -n "$a" ] && [ "$a" = "$b" ] && n=$((n+1))
  done
  echo "FULL: N=76 T=35 bb2 parts exhausted with the logged node count: $n of 8"
  [ "$n" = 8 ] || fail "full rerun mismatch"
fi
echo PASS
