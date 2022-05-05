#include <stdio.h>
#include <stdlib.h>

#define SWORD   0x01
#define NEWS    0x02
#define WHIST   0x04
#define FIST    0x08
#define SWORD2  0x10
#define NEWS2   0x20
#define WHIST2  0x40
#define FIST2   0x80

int user_inputs[100];

const char* colors[8] = {
    "\x01b[31m",
    "\x01b[38;5;17m",
    "\x01b[37m",
    "\x01b[36m",
    "\x01b[35m",
    "\x01b[34m",
    "\x01b[32m",
    "\x01b[33m"
};

//#define NUM_CHARS 4
//char chars[] = "0aA}_%-9]";

#define NUM_CHARS 16
char chars[] = "0123456789abcdefghijklmnopqrstuvwxyz";
//char chars[] = "7a895012346bcdefghijklmnopqrstuvwxyz";
//char chars[] = { 66, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126 };
//char chars[] = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126 };

int popcnt(unsigned char num) {
  int cnt = 0;
  while(num) {
      ++cnt;
      num>>=1;
      while(num && !(num&1)) num>>=1;
  }
  return cnt;
}

int solve(int seed_mod, short v8, short v9, int depth, int gotten, int freejump) {
    short v8_tgt = 0;
    short v9_tgt = 0;
    short v8_orig_tgt = v8;
    short v9_orig_tgt = v9;
    int act_gotten = gotten;


    if(gotten == 0xff && !v8 && !v9) {
        printf("%*sSolve string: %02x %s\n", depth<<1, "", gotten, (char*)user_inputs);
        exit(0);
    } else if(gotten == 0xff && !v9) {
        // targets correct...
    } else if(gotten == 0xff) {
        v8_tgt = -1; // don't change it just yet
        freejump &= 2;
        freejump = 0;
    } else if(!(gotten & NEWS2) && v9 != 0x46fb) {
        v8_tgt = -1; // don't change it just yet
        v9_tgt = 0x46fb;
        freejump &= 2;
    } else if(!(gotten & NEWS2)) {
        v8_tgt = 0x2f58;
        v9_tgt = v9; // already correct
        gotten |= NEWS2;
    } else if(!(gotten & SWORD2) && v9 != (short) 0x88b6) {
        v8_tgt = -1; // don't change it just yet
        v9_tgt = 0x88b6;
        freejump &= 2;
    } else if(!(gotten & SWORD2)) {
        v8_tgt = 0xedac;
        v9_tgt = v9; // already correct
        gotten |= SWORD2;
    } else if((gotten & FIST) && !(gotten & FIST2) && v9 == (short) 0x88b6) {
        v8_tgt = 0x4300;
        v9_tgt = -1;
        v9_orig_tgt = 0xb32c;
    } else if((gotten & FIST) && !(gotten & FIST2) && v9 != (short) 0xb32c) {
        v8_tgt = -1; // don't change it just yet
        v9_tgt = 0xb32c;
        freejump &= 2;
    } else if((gotten & FIST) && !(gotten & FIST2)) {
        v8_tgt = 0xba48;
        v9_tgt = v9; // already correct
        gotten |= FIST2;
    } else if(!(gotten & WHIST2) && v8 == (short) 0xba48) {
        v8_tgt = -1;
        v9_tgt = 0x3001;
        v8_orig_tgt = 0xbc55;
    } else if(!(gotten & WHIST2) && v8 != (short) 0xbc55) {
        v9_tgt = -1;
        v8_tgt = 0xbc55;
        freejump &= 2;
    } else if(!(gotten & WHIST2)) {
        v8_tgt = v8;
        v9_tgt = 0xa439;
        gotten |= WHIST2;
    }

    if((v9_tgt != -1 && (short)(v9_tgt - v9) < 0) || (v8_tgt != -1 && (short)(v8_tgt - v8) < 0)) return 0;
    printf("%s%*sSolve @ %02d, %02x %04x:%04x, %04x:%04x, %04x:%04x %s\n", 
            colors[popcnt(act_gotten)-1], depth<<1, "", depth, act_gotten, 
            (unsigned short) v8, (unsigned short) v9,
            (unsigned short) v8_orig_tgt, (unsigned short) v9_orig_tgt,
            (unsigned short) v8_tgt, (unsigned short) v9_tgt, (char*)user_inputs);
    if(depth > 20) return 0;

    for(int x1 = 0; x1 < NUM_CHARS; x1++) {
        for(int x2 = 0; x2 < NUM_CHARS; x2++) {
            for(int x3 = 0; x3 < NUM_CHARS; x3++) {
                for(int x4 = 0; x4 < NUM_CHARS; x4++) {
                    unsigned long long ss = (chars[x4]<<24)|(chars[x3]<<16)|(chars[x2]<<8)|chars[x1];
                    user_inputs[depth] = ss;
                    
                    srand((unsigned int) ss ^ seed_mod);
                    short aa = v8;
                    short bb = v9;

                    short a = rand();
                    short b = rand();
                    if(a<=0 && b<=0) continue;
                    int kek1 = rand(); // waste it
                    int kek2 = rand(); // waste it
                    unsigned int next = rand();
                    a = a > 0 ? a + aa : aa;
                    b = b > 0 ? b + bb : bb;

                    short r1 = a * b;
                    short r2 = a ^ b;
                    short r3 = (a & b) + (a | b);
                    long long result = 
                        (((long long) (r1)) << 32) | 
                        ((long long) (r2)) |
                        (((long long) (r3)) << 16) |
                        (((long long) ((r1 + r2 + r3))) << 48);

                    if(!a && !b) {
                        printf("%*sSolved: %02x %s\n", depth<<1, "", gotten, (char*)user_inputs);
                        exit(0);
                    } 

                    if(!(((unsigned char) a ^ (unsigned char) b) & 7)) {
                        if(!(gotten & FIST) && (act_gotten >> 4)){
                          solve(next, a, b, depth+1, gotten | FIST, freejump);
                        }
                    } else if(result > 0x5B3E7B48765369A3) {
                            if  (
                                    ((v8_tgt != -1 && a == v8_tgt) || 
                                    (v8_tgt == -1 && (short)(v8_orig_tgt - a) >= 0)) 
                                    && 
                                    ((v9_tgt != -1 && b == v9_tgt) || 
                                    (v9_tgt == -1 && (short)(v9_orig_tgt - b) >= 0))
                                )
                                        solve(next, a, b, depth+1, gotten, freejump);
                    } else if(
                       ((v8_tgt != -1 && a == v8_tgt) || 
                        (v8_tgt == -1 && (short)(v8_orig_tgt - a) >= 0)) 
                       &&
                       ((v9_tgt != -1 && b == v9_tgt) || 
                        (v9_tgt == -1 && (short)(v9_orig_tgt - b) >= 0))
                    ) {
                        solve(next, a, b, depth+1, gotten, freejump);
                    }

                    
                }
            }
        }
    }
    user_inputs[depth] = 0;
    return 0;
}


int check(int seed_mod, short v8, short v9, int depth, int gotten) {
    printf("%*sGotten: %02x %s\n", depth<<1, "", gotten, (char*)user_inputs);
    if((gotten & 0x7) == 0x7) {
      solve(seed_mod, v8, v9, depth, gotten, 0);
      return 0;
    }
    if(depth > 4) return 0;

#ifdef kek
    for(int x1 = (!depth ? 7 : depth == 1 ? 8 : 0); x1 < NUM_CHARS; x1++) {
        for(int x2 = (!depth ? 10 : depth == 1 ? 9 : 0); x2 < NUM_CHARS; x2++) {
#else
    for(int x1 = 0; x1 < NUM_CHARS; x1++) {
        for(int x2 = 0; x2 < NUM_CHARS; x2++) {
#endif
            for(int x3 = 0; x3 < NUM_CHARS; x3++) {
                for(int x4 = 0; x4 < NUM_CHARS; x4++) {
                    unsigned long long ss = (chars[x4]<<24)|(chars[x3]<<16)|(chars[x2]<<8)|chars[x1];
                    user_inputs[depth] = ss;
                    
                    srand((unsigned int) ss ^ seed_mod);
                    short aa = v8;
                    short bb = v9;

                    short a = rand();
                    short b = rand();
                    if(a<=0 && b<=0) continue;
                    int kek1 = rand(); // waste it
                    int kek2 = rand(); // waste it
                    unsigned int next = rand();
                    a = a > 0 ? a + aa : aa;
                    b = b > 0 ? b + bb : bb;

                    short r1 = a * b;
                    short r2 = a ^ b;
                    short r3 = (a & b) + (a | b);
                    long long result = 
                        (((long long) (r1)) << 32) | 
                        ((long long) (r2)) |
                        (((long long) (r3)) << 16) |
                        (((long long) ((r1 + r2 + r3))) << 48);

                    if(result > 0x5B3E7B48765369A3) {
                        if((((unsigned char) a ^ (unsigned char) b) & 7) != 0) {
                            if(result % 8 == 1) {
                                if(((result >> 3) & 3) == 0 && !(gotten & SWORD)) {
                                    check(next, a, b, depth+1, gotten | SWORD);
                                }
                                if(((result >> 3) & 3) == 1 && !(gotten & NEWS)) {
                                    check(next, a, b, depth+1, gotten | NEWS);
                                }
                                if(((result >> 3) & 3) == 2 && !(gotten & WHIST)) {
                                    check(next, a, b, depth+1, gotten | WHIST);
                                }
                            }
                        } else {
                            if(!(gotten & FIST) && (gotten >> 4))
                                check(next, a, b, depth+1, gotten | FIST);
                        }
                    } else if(result == 0x5B3E7B48765369A3 && (gotten & NEWS)) {
                        printf("[!] WIZARD: %s, {0x%08x,0x%04x,0x%04x},\n", &ss, next, a, b);
                        check(next, a, b, depth+1, gotten | NEWS2);
                    } else if(result == 0x33C458487662651A && (gotten & SWORD)) {
                        printf("[!] BLACKSMITH: %s, {0x%08x,0x%04x,0x%04x},\n", &ss, next, a, b);
                        check(next, a, b, depth+1, gotten | SWORD2);
                    } else if(result == -0x2CC7A39F928BF69C && (gotten & FIST)) {
                        printf("[!] DOCTOR: %s, {0x%08x,0x%04x,0x%04x},\n", &ss, next, a, b);
                        check(next, a, b, depth+1, gotten | FIST2);
                    } else if(result == -0x24189D129F71E794 && (gotten & WHIST)) {
                        printf("[!] ENGINEER: %s, {0x%08x,0x%04x,0x%04x},\n", &ss, next, a, b);
                        check(next, a, b, depth+1, gotten | WHIST2);
                    }                
                }
            }
        }
    }
    user_inputs[depth] = 0;
    return 0;
}



int main(void) {
    check(0x5f10afd5, 0xdead, 0xbeef, 0, 0);
    return 0;
}
