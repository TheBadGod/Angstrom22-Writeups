# Weeb Hunters 2

## Analysis

After the initial analysis it was pretty clear what the binary does, as there
wasn't really too much inside the binary. We were already able to decode all the
battle commands, as that was just a bit of collatz which we re-implemented to
get all the correct strings.

Then we determined that the actual game looked something like this:

```c
short v8 = 0xdead;
short v9 = 0xbeef;

rand(time() & 0xff); // not really, but simplifed

while(true) {
  char *in = get_input();
  srand(*(int*)in) ^ rand());
  
  short a = rand();
  short b = rand();
  
  if(a > 0) v8 += a;
  if(b > 0) v9 += b;

  // check specific values for v8/v9
}
```

After some more analysis (including a bit of dynmic analysis actually) we found
that the initial seed actually doesn't depend on the time, but is a constant
66 due to the way it was calculated (We took the time and then put it into a for
loop which shifts the value right once and xors it with a known byte. After 32
iterations the initial "random" time was gone obviously and we only got the
static result).

The rest was pretty easy to reverse, we have to play a game and win to defeat
the final boss. To win against the final boss we need to have four upgraded
weapons, three of which we first need to randomly find the base item and then
the person to upgrade it. For the last one we need to win a fight against a
random weeb (Who for some reason has our fist).

The only thing we could influence is the seed of the random number generator
with the first four characters of our input (as those were cast to an integer
and used as seed after xoring it with a random value from the previous seed).
Also there was a constraint on the input, we couldn't use any non-printable
characters (Which means everything below 0x20 / space and nothing above the
0x7e / tilde), which made the whole thing a bit harder.

The actual calculation which event should happen is pretty straightforward:
First we calculate a "pseudorandom" value:
```c
int64_t a = ((v8 * v9) & 0xffff) << 32;
int64_t b = (v8 ^ v9);
int64_t c = ((v9 & v8) + (v9 | v8)) << 16;
int64_t d = (((v9 & v8) + (v9 | v8) + (v9 ^ v8) + (v8 * v9))&0xffff) << 48;
int64_t result = a|b|c|d;
```

This result value is then compared to a few specific 64-bit values to see if 
we got an item upgrade person (Values can be seen in the script to get the
upgrade values).

If none matched, we then check if the lowest three bits of the two values (v8
and v9) match, if they do we start a fight with a random enemy. Else we check if
the lowest 3 bits of the result value are `001` and if they are we might get a
random item (depending on bits 3 and 4 of the result value).

Finally we need to get both v8 and v9 to zero to be able to summon the final
boss with the magic string `a12caf3cba1e5f13`.

## Solve

We started solving the constraints for the upgrades first, since they were
unique and thus required a bit of z3 code (See `get_upgrades.py`). The values
for v8 and v9 were the values we wanted. For the initial item collection these
were not the only solutions, as the requirement was only about part of the
result of a calculation using v8 and v9.

Following that we started to implement a recursive solver which goes deeper if
we get a new item / upgrade an item. There were some additional constraints,
like we want an upgraded item before going into a fight else we won't survive
the final boss (Since we can only deal like 8 damage with a sword which is the
maximum damage of unupgraded weapons, which means we can get attacked for 20
damage. Since we start with 100 and need to be able to withstand 4 attacks of
the boss, this was not acceptable). And we need the base item before going to 
a person which upgrades that item.

Well, not surprising this was inefficient and took way too much time as we would
search through all possible inputs and determine we can't get to a new item in
just one step, before going into the next branch, which would probably repeat
the same spiel. So we limited the charset to only hex chars and got almost all
items except for a few upgrades and

After quite some time we optimized the bruteforcer such that it can take
"unneeded" steps to manipulate the random numbers in a favourable way.

With this naive way we actually solved the challenge and one can clearly see
at which points in the bruteforcing we restarted using the furthest state we
got, to change the charset:

```
a
7a5a
89c7
1250
4fd3
017c
111c7ab93a8cbcae
7acb
auo8
dlaz
s4ax
0hpr
02a2
eeae
3462
d36e
0000
29f5
1F<D
111c7ab93a8cbcae
a12caf3cba1e5f13
54fcfbc921e8b735
111c7ab93a8cbcae
a8cdae3240310bdd
fist
```

## Afterthoughts

2 hours after I solved it, I realized what a huge dumbass I was. I only tried to
bruteforce the whole 32 bits where v8 and v9 are combined into one 32 bit value.
I skipped all branches where either increment was less than zero (which wouldn't
increase the value). This means when I was looking for the correct values which
required one specific target (The upgrades and the final boss), I essentially
had a 1 in `2^32` chance to get that right (assuming perfect randomness).

After some though I realized that actually I can try to get one of the values
negative (chance 1/2) and the other to the specific increment I needed. This
would result in about a 1 in `2^17` for the first value and for the second I
needed that same chance, but since it should be in the correct order we have to
divide that probability by two. Thus we get 1 in `(2^17 + 2^18)` which obviously
is way better than before.

So I started optimizing the old code, for the upgrade / final boss targets I
wrote a new function which would only go deeper into the branch if there was
actually a chance of getting that specific item, then I created a path to get
all the items (I have no idea if it's the most optimal, all I ensured was that
we could get to the next upgrade / boss in 3 steps (make sure both v8 and v9 can
be reached in one step, set v9, set v8).

After the rewrite (Code in `solve.c`), compiled with O3 and choosing only hex
chars as charset we get a solution in around 2 seconds, which is considerably
better than the multiple hours of solvetime to bruteforce the values before.

Now I, as a student of Algorithms and Datastructures, striving for better
runtimes, can now finally rest.

Here a few solve strings with different charsets:

```
a
AAKE
AEEE
AAQX
DOMV
COLK
AAAR
a8cdae3240310bdd
ATEZ
KYMJ
BUET
AAAG
DKTO
AIKQ
VCVQ
AATS
OZSY
CXLN
ERON
EOVL
a8cdae3240310bdd
a12caf3cba1e5f13
a8cdae3240310bdd
54fcfbc921e8b735
111c7ab93a8cbcae
fist
```

And here's one where I annotated what each of the commands does:

```
0c33 ; whistle
0dfc ; sword
9eb6 ; news
0d5d ; <rng manip>
c9cf ; news 2
00c8 ; fight
a8cdae3240310bdd ; kill with news 2
8c72 ; <rng manip>
f87a ; sword 2
150f ; <rng manip>
003f ; <rng manip>
bc5b ; <rng manip>
0938 ; fist 2
6f02 ; <rng manip>
01a5 ; <rng manip>
06ab ; <rng manip>
5b4c ; whistle 2
ffc1 ; <rng manip>
2d85 ; fight (cuz 0, 0)
54fcfbc921e8b735 ; kill random enemy
a12caf3cba1e5f13 ; Summon boss
a8cdae3240310bdd ; -100
54fcfbc921e8b735 ; -100
111c7ab93a8cbcae ; -100
fist ; win
```

Entering any of these sequences should result in:

`actf{sorry_guys_no_double_free_this_time}`
