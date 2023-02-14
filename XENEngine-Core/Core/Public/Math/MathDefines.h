#pragma once

#define PI 3.14159265358979323846
#define MAGIC_POW_NUM 1072632447

#define TO_RADIANS(t) ((t) * (PI / 180))
#define TO_DEGREES(t) ((t) * (180 / PI))

inline double FAST_POW(double a, double n) {
    union {
        double d;
        int x[2];
    } u = { a };

    u.x[1] = (int)(n * (u.x[1] - MAGIC_POW_NUM) + MAGIC_POW_NUM);
    u.x[0] = 0;
    return u.d;
}

inline int FAST_POW(int a, int n) {
    int ans = 1;
    while (n > 0) {
        if (int last_bit = (n & 1)) {
            ans *= a;
        }
        a = a * a;
        n = n >> 1;
    }
    return ans;
}

inline double FAST_POW_PRECISE(double a, double n) {
    int e = (int)n;
    union {
        double d;
        int x[2];
    } u = { a };

    u.x[1] = (int)((n - e) * (u.x[1] - MAGIC_POW_NUM) + MAGIC_POW_NUM);
    u.x[0] = 0;

    // exponentiation by squaring with the exponent's integer part
    double r = 1.0;
    while (e) {
        if (e & 1) {
            r *= a;
        }
        a *= a;
        e >>= 1;
    }

    return r * u.d;
}