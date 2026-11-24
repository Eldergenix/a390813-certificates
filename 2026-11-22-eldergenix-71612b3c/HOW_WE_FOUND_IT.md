# How we found it (contributor text, unverified)

While extending A390813 past n = 72, a lens noticed that at n = 76 the best set it could find containing 76 had 34
elements, one fewer than the 35-element set living inside {1..75}. Sievers' OEIS comment says n = 32 is the only
such index up to 68, so a second one would be worth certifying. A round-2 lens then ran the question "is there a
35-element valid set containing 76?" to exhaustion with three programs (bb2, and the pair bb3 / sq, which turned
out to walk the same tree), after re-deriving a(69..75) from the OEIS prefix with the same programs. A referee
session wrote a fourth program blind and got the same answer, and the panel's reproduction skeptic wrote a fifth
and also reran bb2 completely.

What failed on the way: SAT with DRAT proofs and ILP / CP-SAT on the collision model (too slow beyond n of about 55
to 66), so there is no certificate of a different kind at n = 76. A first panel was lost to a machine reboot and
had to be run again from scratch.
