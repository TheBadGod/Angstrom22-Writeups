# Super Flag Bros

1. Open binary in ida
2. Find that it compares four bytes with `ACTF`
3. Trace back what it does

Reversed the part directly before that is:

```c
for(int X = 0; X < 0x40; X++) {
  Y = X & 0xF
  Y = mem[0x34+Y] + mem[0x22+Y];
  mem[0x44+X] = mem[0x8597+X] ^ mem[0x8000+Y]
}
```

With `mem[0x44+X]` being the aforementioned string being compared. Given the
flag format and the fact that the flag is not the entire 64 bytes but also some
spaces/null bytes we can recover the whole flag without playing the game at all.
(Sorry but dynamic analysis is not my thing)

`actf{this_story_is_happy_end_thank_you_a1f6096972}`
