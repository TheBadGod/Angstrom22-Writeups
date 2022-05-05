from z3 import *
s = Solver()

log_func = Function("log", BitVecSort(8), BitVecSort(8))
antilog_func = Function("antilog", BitVecSort(8), BitVecSort(8))
cur = 1
for i in range(255):
    s.add(antilog_func(i) == cur)
    s.add(log_func(cur) == i)
    cur *= 2
    if cur >= 256:
        cur ^= 285
s.add(antilog_func(255) == 1)

ecc_poly = [0, 87, 229, 146, 149, 238, 102, 21]
def calc_ecc(log_func, antilog_func, codewords):
    assert(len(codewords) == 19)
    codewords = codewords + [0]*7
    for _ in range(19):
        exp = log_func(codewords[0])
        for i in range(8):
            new_exp = Extract(7, 0, (ecc_poly[i] + ZeroExt(8, exp)) % 255)
            codewords[i] ^= antilog_func(new_exp)
        codewords = codewords[1:]
    return codewords


input_chars = [BitVec(f"i_{i}", 8) for i in range(19)]

# add known stuff in the beginning
msg = b"actf{"
bitstring = "010000010001"
for c in msg:
    bitstring += bin(c)[2:].rjust(8,'0')
for i in range(len(bitstring)//8):
    print("Fixing block ", i)
    s.add(input_chars[i] == int(bitstring[8*i:8*i+8],2))

s.add((input_chars[6] >> 4)&0xf == ord("{")&0xf)

print("Adding char constraints")
for i in range(7, 17):
    c = input_chars[i]
    d = input_chars[i+1]
    s.add(Or(Or(
        And(c & 0xf == 5, (d>>4)&0xf == 0xf),
        And(c & 0xf == 6, (d>>4)&0xf > 0)), 
        And(c & 0xf == 7, (d>>4)&0xf < 0xb)))

print("adding known stuff in the end")
s.add(input_chars[17] == 0b00110111) # second to last char is either c or s
s.add(input_chars[18] == 0b11010000) # lower half of "}" and terminator

print("Adding expected ecc for normal message")
out = calc_ecc(log_func, antilog_func, input_chars[:])
expected = [0x3, 0x32, 0x93, 0xad, 0xed, 0x2e, 0x8]
for i in range(7):
    s.add(out[i] == expected[i])

print("Adding expected ecc for message ^ 0x01")
out = calc_ecc(log_func, antilog_func, input_chars[:-1] + [c ^ 0x10 for c in input_chars[-1:]])
expected = [0xa0, 0xc1, 0xc6, 0x3f, 0x5d, 0x1a, 0xb]
for i in range(7):
    s.add(out[i] == expected[i])

print("Solving")
while s.check() == sat:
    m = s.model()
    binstring = ""
    for c in input_chars:
        binstring += bin(m[c].as_long())[2:].rjust(8,"0")
    binstring = binstring[4+8:]
    print(binstring)
    while len(binstring):
        print(chr(int(binstring[:8],2)),end="")
        binstring = binstring[8:]
    print()
    exit(0)
    s.add(Or(*[c != m[c].as_long() for c in input_chars]))
else:
    print("oof")
