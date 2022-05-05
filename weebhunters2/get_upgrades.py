from z3 import *

v8 = BitVec("v8", 16)
v9 = BitVec("v9", 16)

a = ZeroExt(48, (v8 * v9) & 0xffff) << 32
b = ZeroExt(48, v8 ^ v9)
c = ZeroExt(48, (v9 & v8) + (v9 | v8)) << 16
d = ZeroExt(48, ((v9 & v8) + (v9 | v8) + (v9 ^ v8) + (v8 * v9))&0xffff) << 48
result = a | b | c | d

magics = [
    ("wizard (newspaper)",0x5B3E7B48765369A3),
    ("blacksmith (sword)",0x33C458487662651A),
    ("doctor      (fist)",0xD3385C606D740964),
    ("engineer (whistle)",0xDBE762ED608E186c),
]

for cur,val in magics:
    s = Solver()
    s.add(result == val)

    if s.check() == sat:
        m = s.model()
        print(f"{cur}: v8 = 0x{m[v8].as_long():x}, v9 = 0x{m[v9].as_long():x}")
    else:
        print(f"No {cur} for me :(")

items = ["sword", "newspaper", "whistle"]
for i in range(3):
    s = Solver()
    s.add(result > 0x5B3E7B48765369A3)
    s.add(result < 0x7fffffffffffffff)
    s.add(LShR(a, 32) < 0x08000)
    s.add(LShR(b, 0) < 0x08000)
    s.add(LShR(c, 16) < 0x08000)
    s.add(LShR(d, 48) < 0x08000)
    s.add(result > 0)
    s.add((v8 ^ v9) % 8 != 0)
    s.add(result & 7 == 1)
    s.add((result >> 3) & 3 == i)

    s.add((v8-0xdead) & 0xffff <= 0x7fff)
    s.add((v9-0xbeef) & 0xffff <= 0x7fff)
    s.add((v8-0xdead) & 0xffff >= 0)
    s.add((v9-0xbeef) & 0xffff >= 0)

    if s.check() == sat:
        m = s.model()
        s8 = m[v8].as_long()
        s9 = m[v9].as_long()
        print(f"Here's your {items[i]}: v8 = 0x{s8:x}, v9 = 0x{s9:x}")
        print(hex((s8 - 0xdead) & 0xffff), hex((s9 - 0xbeef) & 0xffff))
    else:
        print("Nope")


s = Solver()
s.add(result > 0x5B3E7B48765369A3)
s.add((v8 ^ v9) & 7 == 0)
if s.check() == sat:
    m = s.model()
    print(f"Fight with value: v8 = 0x{m[v8].as_long():x}, v9 = 0x{m[v9].as_long():x}")
else:
    print("no fights available")


