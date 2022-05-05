#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template mask.py --host challs.actf.co --port 31501
from pwn import *

# Set up pwntools for the correct architecture
context.update(arch='i386')
exe = 'mask.py'

#nc challs.actf.co 31501
# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or 'challs.actf.co'
port = int(args.PORT or 31501)

def start_local(argv=[], *a, **kw):
    '''Execute the target binary locally'''
    if args.GDB:
        return gdb.debug([exe] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process(["python", exe] + argv, *a, **kw)

def start_remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io

def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.LOCAL:
        return start_local(argv, *a, **kw)
    else:
        return start_remote(argv, *a, **kw)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
continue
'''.format(**locals())

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
def unmask(mask, row, column):    
    if mask == 0:
        return  (row + column) % 2 == 0
    if mask ==  1: 
        return 	(row) % 2 == 0
    if mask ==  2:
        return	(column) % 3 == 0
    if mask == 3: 
        return	(row + column) % 3 == 0
    if mask == 4: 
        return	( floor(row / 2) + floor(column / 3) ) % 2 == 0
    if mask == 5: 
        return	((row * column) % 2) + ((row * column) % 3) == 0
    if mask == 6: 
        return	( ((row * column) % 2) + ((row * column) % 3) ) % 2 == 0
    if mask == 7: 
        return  ( ((row + column) %2) + ((row * column) % 3) ) % 2 == 0
    assert False


def decode_qr(lines):
    res = []
    for l in lines:
        # every line is actually 2 lines of information
        a = []
        b = []
        for c in l.decode():
            if c == '█':
                a.append(1)
                b.append(1)
            elif c == '▀':
                a.append(1)
                b.append(0)
            elif c == '▄':
                a.append(0)
                b.append(1)
            elif c == '\xa0':
                a.append(0)
                b.append(0)
            else:
                print("Bad char", c)
                assert False
        res.append(a)
        res.append(b)
    return res
    
def to_int(l):
    return int("".join([str(i) for  i in l]), 2)
    
    
BS="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
def to_base(s, b): # from int to str https://stackoverflow.com/questions/2267362/how-to-convert-an-integer-to-a-string-in-any-base
    res = ""
    while s:
        res+=BS[s%b]
        s//= b
    return res[::-1] or "0"
    
io = start()

def do_qr(inp):
    io.recvuntil(b'(in hex): ')
    info("Sending input: %s", inp.hex())
    io.sendline(inp.hex().encode())
    qrcode = io.recvlines(11)
    qrcode_decoded = decode_qr(qrcode)

    # by special I simply mean the format part
    special = []
    special.extend(qrcode_decoded[8][:6])
    special.extend(qrcode_decoded[8][7:9])
    special.append(qrcode_decoded[7][8])

    for i in range(5, -1,-1):
        special.append(qrcode_decoded[i][8])
        
    # decode mask 
    # https://www.thonky.com/qr-code-tutorial/format-version-information
    mask = to_int(special) ^ 0b101010000010010
    error_correction = mask & (2**10 - 1)
    fmt = mask >> 10
    ecc_level = fmt >> 3
    mask_used = fmt & 7

    assert(ecc_level == 1) # error_correction = qrcode.constants.ERROR_CORRECT_L

    info("Mask used: %d", mask_used)
    for y in range(21):
        for x in range(21 if args.LOCAL else 11):
            qrcode_decoded[y][x] ^= unmask(mask_used, y, x)

    #print(*qrcode_decoded,sep="\n")

    def get_downwards_block(x, y):
        return  qrcode_decoded[y][x+1] * 128 + \
            qrcode_decoded[y][x] * 64 + \
            qrcode_decoded[y+1][x+1] * 32 + \
            qrcode_decoded[y+1][x] * 16 + \
            qrcode_decoded[y+2][x+1] * 8 + \
            qrcode_decoded[y+2][x] * 4 + \
            qrcode_decoded[y+3][x+1] * 2 + \
            qrcode_decoded[y+3][x] * 1

    def get_upwards_block(x, y):
        return  qrcode_decoded[y+3][x+1] * 128 + \
            qrcode_decoded[y+3][x] * 64 + \
            qrcode_decoded[y+2][x+1] * 32 + \
            qrcode_decoded[y+2][x] * 16 + \
            qrcode_decoded[y+1][x+1] * 8 + \
            qrcode_decoded[y+1][x] * 4 + \
            qrcode_decoded[y][x+1] * 2 + \
            qrcode_decoded[y][x] * 1


    last_char = get_downwards_block(9, 2)
    info("Last char is: %c", last_char)

    if args.LOCAL:
        first_char = get_upwards_block(19, 11)
        info("First char is: %c", first_char)

    e1 = get_downwards_block(9, 9)
    e2 = get_downwards_block(9, 13)
    e3 = get_downwards_block(9, 17)
    e4 = get_upwards_block(7, 9)
    e5 = get_downwards_block(4, 9)
    e6 = get_upwards_block(2, 9)
    e7 = get_downwards_block(0, 9)

    info("ECC: [0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x]", e1, e2, e3, e4, e5, e6, e7)

msg = b"actf{actf_dummytu"
bitstring = "010000010001"
for c in msg:
    bitstring += bin(c)[2:].rjust(8,'0')
bitstring += "0000"

#assert(len(bitstring) == 19*8)
codewords = []
for i in range(19):
    codewords.append(int(bitstring[8*i:8*i+8], 2))

log_table = {}
antilog_table = {}
cur = 1
for i in range(255):
    antilog_table[i] = cur
    log_table[cur] = i
    cur *= 2
    if cur >= 256:
        cur ^= 285
antilog_table[255] = 1
print(log_table[1])

ecc_poly = [0, 87, 229, 146, 149, 238, 102, 21]
def calc_ecc(codewords):
    assert(len(codewords) == 19)
    codewords = codewords + [0]*7

    for _ in range(19):
        exp = log_table[codewords[0]]
        for i in range(8):
            new_exp = (ecc_poly[i] + exp) % 255
            codewords[i] = antilog_table[new_exp] ^ codewords[i]
        codewords = codewords[1:]
    print([hex(i) for i in codewords])

calc_ecc(codewords)

do_qr(b"\x00"*17)
do_qr(b"\x00"*16+b"\x01")

io.interactive()

