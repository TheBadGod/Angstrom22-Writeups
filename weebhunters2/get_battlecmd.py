from pwn import *
e = ELF("./weeb_hunters2_release")
for k in range(8):
    m = 0
    print(k,"==> ",end="")
    while u32(e.read(0x6020 + (32 * k + m) * 4, 4)):
        v21 = 0
        v22 = u32(e.read(0x6020 + (32 * k + m) * 4, 4))
        while v22 != 1:
            if v22 & 1 != 0:
                v22 = 3 * v22 + 1
            else:
                v22 >>= 1
            v21 += 1
        print(chr((v21-0x80)&0xff),end="")
        m += 1
    print()
