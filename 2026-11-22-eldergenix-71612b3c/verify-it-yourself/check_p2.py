#!/usr/bin/env python3
"""Standalone check for the claim "no maximum Sidon-in-squares subset of {1..76} contains 76" (OEIS A390813, n = 76).
Definition: a set X of positive integers is valid iff all sums x^2 + y^2 with x <= y, x, y in X (x = y included) are
distinct; a(n) is the largest size of a valid subset of {1..n}.
What this script does (standard library only), from the package folder:
  1. checks both witnesses in object/a_p2_n76.json from the definition (35 elements inside {1..75}; 34 elements
     containing 76);
  2. audits the saved logs in object/logs/: for each exclusion D(N,T) of the chain and the decisive D(76,35), and each
     of the programs bb2, bb3, sq: all 8 parts present, the recorded command line carries exactly the prefix
     a(1..68) from the OEIS b-file followed by the chain values, one EXHAUSTED line for the right N, T and part,
     "# exit=0", no FOUND; node totals at D(76,35) equal the ones in the object file; bb3 and sq equal per part;
  3. audits the fourth program's log (280 parts of D(76,35)).
It does NOT rerun the long searches (run_checks.sh does that with FULL=1)."""
import json, re, sys, os
from itertools import combinations_with_replacement
def valid(w, n, size):
    if len(w) != size or len(set(w)) != size or not all(isinstance(x, int) and 1 <= x <= n for x in w): return False
    s = [x * x + y * y for x, y in combinations_with_replacement(sorted(w), 2)]
    return len(s) == len(set(s)) == size * (size + 1) // 2
def die(msg):
    print("BAD: " + msg); sys.exit(1)
o = json.load(open("object/a_p2_n76.json"))
chain = {int(k): v for k, v in o["chain"].items()}
if not (o["n"] == 76 and o["a_n"] == 35 and o["g_n"] == 34
        and chain == {69: 32, 70: 33, 71: 33, 72: 33, 73: 34, 74: 34, 75: 35, 76: 35}
        and o["exclusions"] == [[69, 33], [71, 34], [72, 34], [74, 35], [76, 35]]):
    die("object constants")
w35, w34 = o["witness_35_in_1_75"], o["witness_34_containing_76"]
if not valid(w35, 75, 35): die("35-element witness")
if not (valid(w34, 76, 34) and 76 in w34): die("34-element witness containing 76")
bfile = [int(l.split()[1]) for l in open("object/oeis_b390813_prefix.txt") if l.strip() and l.split()[0].isdigit()]
if len(bfile) != 68 or bfile[-1] != 32: die("b-file prefix")
full = bfile + [chain[k] for k in range(69, 76)]          # A_1..A_75 (upper bounds used for pruning)
tot = {}
for N, T in o["exclusions"]:
    d = "object/logs/p2_76" if N == 76 else "object/logs/chain"
    per = {}
    for prog in ("bb2", "bb3", "sq"):
        nodes = []
        for p in range(8):
            f = "%s/%s_N%d_T%d_p%dof8.log" % (d, prog, N, T, p)
            if not os.path.isfile(f): die("missing " + f)
            t = open(f).read(); lines = t.splitlines()
            m = re.search(r"cmd: \./%s ((?:\d+ ?)+)" % prog, lines[0])
            if not m: die("no command line in " + f)
            a = [int(x) for x in m.group(1).split()]
            if a[:5] != [N, T, 8, 8, p] or a[5:] != full[:N - 1]: die("arguments or prefix in " + f)
            ex = [l for l in lines if l.startswith("EXHAUSTED")]
            if len(ex) != 1 or "FOUND" in t or not any(l.startswith("# exit=0") for l in lines): die("not exhausted cleanly: " + f)
            e = ex[0]
            if not (re.search(r"\bN=%d\b" % N, e) and re.search(r"\bT=%d\b" % T, e) and re.search(r"\bpart=%d\b" % p, e)): die("wrong EXHAUSTED line in " + f)
            nodes.append(int(re.search(r"nodes=(\d+)", e).group(1)))
        per[prog] = nodes
    if per["bb3"] != per["sq"]: die("bb3 and sq differ at N=%d" % N)
    if N == 76: tot = {k: sum(v) for k, v in per.items()}
if tot != o["nodes_D76_35"]: die("node totals at D(76,35) differ from the object file: %r" % tot)
t = open("object/logs/search4_dsearch_D76_35_all_parts.log").read()
idx = sorted(int(x) for x in re.findall(r"split_depth=8 parts=280 idx=(\d+)", t))
ex = re.findall(r"EXHAUSTED part (\d+)/280: D\(76,35\)=NO in this part", t)
if idx != list(range(280)) or sorted(int(x) for x in ex) != list(range(280)) or "FOUND" in t: die("fourth program's log")
print("witnesses 35@75 and 34@76 valid; logs complete for D(69,33) D(71,34) D(72,34) D(74,35) D(76,35) x 3 programs; "
      "D(76,35) nodes bb2=%d bb3=%d sq=%d; fourth program 280 of 280 parts: OK" % (tot["bb2"], tot["bb3"], tot["sq"]))
