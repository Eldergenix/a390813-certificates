# Panel seat: NOVELTY (claim erdos-773-sidon-squares-next-term, partial win p2)

context: claude agent session (Claude Agent SDK / Claude Code harness), fresh session, claude-opus-5-5; started unattended as one of three skeptics

Claim under test: n = 76 is a second index (after n = 32) at which no maximum Sidon subset of {1^2..n^2}
contains n^2, i.e. A390813(76) = 35 and g(76) <= 34 (D(76,35) = NO).

## 1. Status sources (one plain curl request each, User-Agent "V3R1T4S-novelty-skeptic/1.0", no retries)
Saved copies are in novelty_fetch/. Times are from `date -u`.

| time (UTC) | URL | HTTP / bytes | sha256 | what it says |
|---|---|---|---|---|
| Sun Nov 22 21:18:51 2026 | https://oeis.org/A390813 | 200 / 20036 | 0d87d205...afb3 | DATA a(1..68), last term 32. Comment, verbatim: "Up to at least n=68, the case of n=32 is the only one where the maximum size cannot be attained with a subset that contains n^2. - Christian Sievers, Nov 27 2025". So the only index stated is n = 32, and the statement covers n <= 68. Extensions: "a(43)-a(68) from Christian Sievers, Nov 27 2025". Status approved. Nothing about n >= 69. |
| Sun Nov 22 21:18:51 2026 | https://oeis.org/A390813/b390813.txt | 200 / 438 | dce8e0fa...6916 | Ends at "68 32". Byte-identical to status/oeis_b390813.txt (same sha256). |
| Sun Nov 22 21:18:52 2026 | https://oeis.org/history?seq=A390813 | 200 / 25211 | b72ed369...fb3 | Latest revision #25 (Sean A. Irvine, Tue Dec 02 2025, proposed to approved). Revisions #20 to #23 (Nov 27 2025) add a(43..68) and the Sievers comment. No pending or later edit, and no a(69+) or other index. |
| Sun Nov 22 21:18:52 2026 | https://erdosproblemaday.com/day/773-square-sidon-frontier | 200 / 51564 | 1c7be380...7afb6 | Title "#773: conditional exact values through a(71) = 33", PARTIAL, Aug 12 2026. It gives a(69)=32, a(70)=33, a(71)=33, conditional on the OEIS prefix. Nothing on n >= 72, and nothing on which maximum sets contain n^2. Byte-identical to the scout's fetch. |
| Sun Nov 22 21:18:52 2026 | https://www.erdosproblems.com/forum/thread/773 | 200 / 59113 | 05650f04...b97b | Comments (11), proof claims (0), "last edited 24 April 2026", new comments suspended. The comments are Croot-Mao-Yip's upper bound, the random-model heuristic, Woett's g(A) remark and Kalogeropoulos's OEIS submission note. There are no finite values beyond the OEIS link and nothing about n^2 membership. |

The hashes of the OEIS entry, history and forum pages differ from the scout's earlier fetches (status/). The extracted text shows the same content: 68 terms, revision #25 as the latest, and 11 comments. The differences look like dynamic page furniture, such as the "Last modified" footer. All five sources were read, so this is not a PENDING STATUS CHECK.

## 2. Literature and code search (last 24 months)
- arXiv, via web search and two abstract fetches (2026-11-22 21:23:47 UTC):
  - 2607.02728, "Large B_2[g] subsets of the first squares" (Jul 2026), is an asymptotic lower bound only.
  - 2605.03181, "On the largest Sidon subset in a finite subset of R^N" (May 2026), is an asymptotic bound that mentions the squares only as an open question.
  - Hits 2608.02667, 2606.17487 and 2602.23282 are about other ambient sets or are asymptotic.
  - No paper gives finite values of A390813 beyond 68 or any statement about maximum sets containing n^2 for n in 69..100.
- GitHub code search "A390813" (gh, 21:22:54 UTC) returned pw/erdosproblemaday-verification, Radcliffe/OEIS-Python, przchojecki/agentic-erdos, techno-optimist/erdos-frontier-atlas, queelius/computational-explorations, ryantuck/erdos-ai, kevinfff08/erdos_ai_solvability_review, zsophiaaa/conjecturehub, Hybrid-Quantum-Exchange, erichou1/erdos-open-problems, Deicyde/WikiLean and the V3R1T4S bank file.
  - I fetched the relevant files and grepped them for a(69..100), "contain", n = 32 and 72..99.
  - None states a value a(n) for n >= 72, and none states that some n in 69..100 lies in no maximum set. erdos-frontier-atlas P773 only records a(68)=32 and the Erdős Problem a Day a(69..71) report as "unverified". agentic-erdos only has random-greedy heuristics.
  - Further code searches ("sidon squares a(76)", "A390813 76", "square sidon frontier 76", "sidon squares exhaustive 76") returned nothing.
- github.com/pw/erdosproblemaday-verification:
  - Folder 773/ has only n69-frontier and n71-frontier. Its last 773 commit is fc14c82c (2026-10-25). There are no releases, and therefore no release assets.
  - Its documents say only that a hypothetical 33-set at 69, or 34-set at 71, must contain the top index (D(69,33) and D(71,34)). They say nothing about indices beyond 71 or about g(n) < a(n).
- github.com/elder-plinius/V3R1T4S:
  - TROPHY_CASE.md has no 773, A390813 or Sidon-squares entry; its only "773" hits are unrelated numbers such as 3773.
  - known-knowns/ has no erdos-773 or sidon-squares folder. Its 46 folders were listed, and the only Sidon file is a helper in bigger-codes.
  - known-unknowns/erdos-and-proofs/erdos-773-sidon-squares-next-term.md is the bank entry that defines this p2 target.
  - Pull requests: I listed 500 PRs (open and closed) and searched for "erdos-773", "sidon-squares", "squares Sidon", "Sievers", "A390813" and "773". There is no 773 / A390813 claim PR. The Sidon PRs are erdos757 and erdos-156.
- github.com/Eldergenix/erdos-773-a390813 is the claimant's own earlier output. It has one commit, c2ce40f8 (2026-11-22T19:00:59Z), with OpenTimestamps stamped 2026-11-22T18:30:48Z.
  - What it states: a(69..76) = 32, 33, 33, 33, 34, 34, 35, 35, with a 35-witness in {1..75} (the same witness as this claim) and an exclusion "no 36-element set in {1..76} containing 76", i.e. D(76,36) logs only.
  - What it explicitly does not state, from packages/a76-.../README.md under "Not claimed": "Nothing about a(77) or later terms, and nothing about which maximum sets contain 76^2."
  - I grepped its logs, its text files and both proof tars. None has an N76_T35 / D(76,35) run, and the repo does not show the 34-element set containing 76.
  - In any case it is the claimant's own work, so it cannot make the present result "already known".

## 3. Does the present claim add something beyond a trivial step?
Yes. From the published or claimant-public facts, a(75) = a(76) = 35 shows that some maximum set avoids 76, and D(76,36) = NO gives a(76) <= 35. Neither says anything about whether a 35-element valid set containing 76 exists.

g(76) <= 34 needs the separate exhaustive exclusion D(76,35). That is a lower target, so it is a larger tree: bb3 is stated to explore 3,755,162,596 nodes, against 442,644,384 for D(76,36) in the public package. D(76,35) does not follow from D(76,36) or from any published value. D(76,36) does follow from D(76,35), not the other way round.

The OEIS comment only covers n <= 68. I found no statement by anyone, the claimant included, that n = 76, or any n in 69..100, is an index where n^2 lies in no maximum set.

Scope of this seat: novelty only. I did not check the correctness of the D(76,35) exclusion, the witnesses or the program independence; those belong to the other seats.

Process note: the GitHub reads used the gh CLI with whatever account was already authenticated on the host. They were read-only, and nothing was posted, submitted or modified.

novelty: CONFIRMED. As of Sun Nov 22 21:24 UTC 2026, the statement "76^2 lies in no maximum Sidon subset of {1^2..76^2} (g(76) = 34 < a(76) = 35)" does not appear in the OEIS entry, its b-file or history, Erdős Problem a Day #773, erdosproblems.com forum thread 773, arXiv, GitHub code or repository search, pw/erdosproblemaday-verification, the V3R1T4S trophy case, known-knowns or PRs, or the claimant's own public repository.

pass: true
