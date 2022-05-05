# Self

## Disassembler

```py
from pwn import *

e = ELF("./chall")

bbs = set()

code_ptr = 0x4038
for i in range(0x1000):
    op = u32(e.read(code_ptr + 4*i,4))

    x = op >> 24
    
    if x == 0x69:
        dst = (op >> 12) & 0xfff
        src = (op) & 0xfff
        dst = (i + src) & 0xfff
        bbs.add(dst)
    elif x == 0xa6:
        dst = (op >> 12) & 0xfff
        dst += 1
        bbs.add(dst)



code_ptr = 0x4038
for i in range(0x1000):
    if i in bbs:
        print()
    op = u32(e.read(code_ptr + 4*i,4))
    op2 = u32(e.read(code_ptr + 4*i+4,4))


    x = op >> 24
    if x == 0:
        continue
    print(f"{i:4x}: {op:8x} ",end="")

    if op > 0xf7ffffff:
        print("exit")
        continue

    
    if x == 1:
        print("dbg")
        print()
    elif x == 0x16:
        dst = (op >> 12) & 0xfff
        src = (op) & 0xfff
        print(f"add mem[{dst:3x}], mem[{src:3x}]")
    elif x == 0x17:
        dst = (op >> 12) & 0xfff
        src = (op) & 0xfff
        print(f"sub mem[{dst:3x}], mem[{src:3x}]")
    elif x == 0x18:
        dst = (op >> 12) & 0xfff
        src = (op) & 0xfff
        print(f"xor mem[{dst:3x}], mem[{src:3x}]")
    elif x == 0x69:
        dst = (op >> 12) & 0xfff
        src = (op) & 0xfff
        src = (src+i) & 0xfff
        print(f" jz mem[{dst:3x}], {src:3x}")
    elif x == 0xa6:
        dst = (op >> 12) & 0xfff
        dst += 1
        print(f"jmp 0x{dst:x}")
    elif x == 0xd6:
        dst = (op >> 12) & 0xfff
        src = (op) & 0xfff
        print(f"mov mem[{dst:3x}], mem[{src:3x}]")
    elif x == 0xd8:
        dst = (op >> 12) & 0xfff
        src = (op) & 0xfff
        print(f"mov mem[{dst:3x}], 0x{src:3x}")
    elif x == 0xf6:
        dst = (op >> 12) & 0xfff
        print(f"putc mem[{dst}]");
    elif x == 0xf7:
        dst = (op >> 12) & 0xfff
        print(f"getc mem[{dst}]");
    else:
        print("unknown op: ", hex(op), hex(op2))
        #exit(0)
```

## The code

```
 800: d8000000 mov mem[  0], 0x  0
 801: d8001001 mov mem[  1], 0x  1
 802: d80031ff mov mem[  3], 0x1ff
 803: d8010000 mov mem[ 10], 0x  0
 804: d8011000 mov mem[ 11], 0x  0
 805: d8012000 mov mem[ 12], 0x  0
 806: d8013000 mov mem[ 13], 0x  0
 807: d8014000 mov mem[ 14], 0x  0
 808: d8015000 mov mem[ 15], 0x  0
 809: d8016000 mov mem[ 16], 0x  0
 80a: d8017000 mov mem[ 17], 0x  0
 80b: d8018000 mov mem[ 18], 0x  0
 80c: d8019000 mov mem[ 19], 0x  0
 80d: d801a000 mov mem[ 1a], 0x  0
 80e: d801b000 mov mem[ 1b], 0x  0
 80f: d801c000 mov mem[ 1c], 0x  0
 810: d801d000 mov mem[ 1d], 0x  0
 811: d801e000 mov mem[ 1e], 0x  0
 812: d801f000 mov mem[ 1f], 0x  0
 813: d801afff mov mem[ 1a], 0xfff      ; mem[1a] = 0xfff

   814: 6901a054  jz mem[ 1a], 868        ; if !mem[1a]--: break;
   815: 1701a001 sub mem[ 1a], mem[  1]   ; 
   816: d8019fff mov mem[ 19], 0xfff      ; mem[19] = 0xfff

     817: 69019050  jz mem[ 19], 867        ; if !mem[19]--: break;
     818: 17019001 sub mem[ 19], mem[  1]
     819: d8018fff mov mem[ 18], 0xfff      ; mem[18] = 0xfff

       81a: 6901804c  jz mem[ 18], 866
       81b: 17018001 sub mem[ 18], mem[  1]   ; if !mem[18]--: break;
       81c: d8017fff mov mem[ 17], 0xfff      ; mem[17] = 0xfff

         81d: 69017048  jz mem[ 17], 865
         81e: 17017001 sub mem[ 17], mem[  1]   ; if !mem[17]--: break;
         81f: d801f074 mov mem[ 1f], 0x 74      ; mem[1f] = 0x74
         820: d801e8dd mov mem[ 1e], 0x8dd      ; mem[1e] = 0x8dd
         821: d801d950 mov mem[ 1d], 0x950      ; mem[1d] = 0x950
         822: d801c010 mov mem[ 1c], 0x 10      ; mem[1c] = 0x10
         823: d801b001 mov mem[ 1b], 0x  1      ; mem[1b] = 1
         824: d601001c mov mem[ 10], mem[ 1c]

         ; mem[11] += mem[1e] * mem[1c]
         825: 6901c004  jz mem[ 1c], 829
           826: 1701c001 sub mem[ 1c], mem[  1]
           827: 1601101e add mem[ 11], mem[ 1e]
         828: 69000ffd  jz mem[  0], 825

         829: d601e011 mov mem[ 1e], mem[ 11]
         82a: d6011000 mov mem[ 11], mem[  0]
         82b: d601c010 mov mem[ 1c], mem[ 10]
         82c: d6010000 mov mem[ 10], mem[  0]
         82d: d601001c mov mem[ 10], mem[ 1c]

         ; mem[11] += mem[1e] * mem[1c]
         82e: 6901c004  jz mem[ 1c], 832
           82f: 1701c001 sub mem[ 1c], mem[  1]
           830: 1601101e add mem[ 11], mem[ 1e]
         831: 69000ffd  jz mem[  0], 82e

         832: d601e011 mov mem[ 1e], mem[ 11]
         833: d6011000 mov mem[ 11], mem[  0]
         834: d601c010 mov mem[ 1c], mem[ 10]
         835: d6010000 mov mem[ 10], mem[  0]
         836: d601001c mov mem[ 10], mem[ 1c]

         ; mem[11] += mem[1e] * mem[1c]
         837: 6901c004  jz mem[ 1c], 83b
           838: 1701c001 sub mem[ 1c], mem[  1]
           839: 1601101e add mem[ 11], mem[ 1e]
         83a: 69000ffd  jz mem[  0], 837

         83b: d601e011 mov mem[ 1e], mem[ 11]
         83c: d6011000 mov mem[ 11], mem[  0]
         83d: d601c010 mov mem[ 1c], mem[ 10]
         83e: d6010000 mov mem[ 10], mem[  0]
         83f: d601001c mov mem[ 10], mem[ 1c]

         ; mem[11] += mem[1b] * mem[1c]
         840: 6901c004  jz mem[ 1c], 844
           841: 1701c001 sub mem[ 1c], mem[  1]
           842: 1601101b add mem[ 11], mem[ 1b]
         843: 69000ffd  jz mem[  0], 840

         844: d601b011 mov mem[ 1b], mem[ 11]
         845: d6011000 mov mem[ 11], mem[  0]
         846: d601c010 mov mem[ 1c], mem[ 10]
         847: d6010000 mov mem[ 10], mem[  0]
         848: d601001c mov mem[ 10], mem[ 1c]

         ; mem[11] += mem[1b] * mem[1c]
         849: 6901c004  jz mem[ 1c], 84d
           84a: 1701c001 sub mem[ 1c], mem[  1]
           84b: 1601101b add mem[ 11], mem[ 1b]
         84c: 69000ffd  jz mem[  0], 849

         ; set up stuff
         84d: d601b011 mov mem[ 1b], mem[ 11]
         84e: d6011000 mov mem[ 11], mem[  0]
         84f: d601c010 mov mem[ 1c], mem[ 10]
         850: d6010000 mov mem[ 10], mem[  0]
         851: d601001c mov mem[ 10], mem[ 1c]

         ; while(mem[1c]--) mem[11] += mem[1b];
         ; same as mem[11] += mem[1b] * mem[1c]

         852: 6901c004  jz mem[ 1c], 856
           853: 1701c001 sub mem[ 1c], mem[  1]
           854: 1601101b add mem[ 11], mem[ 1b]
         855: 69000ffd  jz mem[  0], 852

         856: d601b011 mov mem[ 1b], mem[ 11]
         857: d6011000 mov mem[ 11], mem[  0]
         858: d601c010 mov mem[ 1c], mem[ 10]
         859: d6010000 mov mem[ 10], mem[  0]


         85a: 6901f00a  jz mem[ 1f], 864
           85b: 1701f001 sub mem[ 1f], mem[  1]
           85c: 1701e01b sub mem[ 1e], mem[ 1b]
           85d: 1701d001 sub mem[ 1d], mem[  1]
           85e: 1686001e add mem[860], mem[ 1e]     ; idx1
           85f: 1686001d add mem[860], mem[ 1d]     ; idx2
           860: 18000000 xor mem[  0], mem[  0]     ; <-- self-modified
           861: 1786001e sub mem[860], mem[ 1e]
           862: 1786001d sub mem[860], mem[ 1d]
         863: a6859000 jmp 0x85a

         864: a681c000 jmp 0x81d

       865: a6819000 jmp 0x81a

     866: a6816000 jmp 0x817

   867: a6813000 jmp 0x814

 868: ffffffff exit
```

Oh, we just do `arr[x] ^= arr2[x]` around 2 to the 64 times.

So I just patched the binary to be one iteration instead of 0xfff,
which due to it being xor had the same effect as doing it many times.

Running the patched binary spat out the flag `actf{the_flag_was_you_all_along}`.
