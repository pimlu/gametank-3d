#pragma once

#include <cstdint>
#include <algorithm>

struct ScreenPos {
    int8_t x, y;
    inline ScreenPos transpose() {
        return {y, x};
    }
};

// https://mcejp.github.io/2020/11/06/bresenham.html
class BresenhamCore{
    int8_t xL;

    uint8_t dx, dy;

    uint8_t E;
    bool isPos;

    int8_t bresenhamIterPos() {
        // in the real algorithm, the lowest E can get to is -dx.
        // we've established since E is unsigned that the algorithm doesn't trigger
        // the while loop in the first pass. so we can safely "rotate" the phase of the algorithm
        // the point of rotating it is to keep it positive (add before subtracting)
        int8_t ret = xL;
        // if subtracting 2*dx would underflow...
        if (E < 2*dx) {
            xL++;
            E += 2 * dy - 2 * dx;
        } else {
            E -= 2*dx;
        }
        return ret;
    }
    int8_t bresenhamIterNeg() {
        // same approach as bresenhamIterPos, but E is negated
        int8_t ret = xL;

        // if subtracting 2*dx would underflow...
        if (E <= 2*dx) {
            xL--;
            E += 2 * dy - 2 * dx;
        } else {
            E -= 2 * dx;
        }
        return ret;
    }
public:
    // assumption: dy >= dx
    BresenhamCore(ScreenPos a, ScreenPos b) {
        xL = a.x;
        dy = b.y - a.y;
        isPos = a.x <= b.x;
        if (isPos) {
            dx = b.x - a.x;
        } else {
            dx = a.x - b.x;
        }
        E = dy - dx;
        if (!isPos) {
            // E is "negated" compared to the positive algo
            // unfortunately E can equal 0 in the first step
            if (E == 0) {
                xL--;
                E += 2 * dy - 2 * dx;
            }
        }
    }
    int8_t bresenhamIter() {
        return isPos ? bresenhamIterPos() : bresenhamIterNeg();
    }
    int8_t curxL() {
        return xL;
    }
};

inline int8_t mini8(int8_t a, int8_t b) {
    return a >= b ? a : b;
}
inline uint8_t absDiffi8(int8_t a, int8_t b) {
    if (a >= b) {
        return a - b;
    } else {
        return b - a;
    }
}

/*



2..
5  ...
6     .

0. 
0.
1 .
1 .
1 .
1  .

*/

class Bresenham {
    bool isSwapped;
    BresenhamCore core;
    int8_t y, prevX;

    static bool checkSwapped(ScreenPos a, ScreenPos b) {
        return false;
        uint8_t dx = absDiffi8(a.x, b.x), dy = absDiffi8(a.y, b.y);
        return dx > dy;
    }

    static BresenhamCore transposedCore(ScreenPos a, ScreenPos b) {
        ScreenPos aTrans = a.transpose(), bTrans = b.transpose();
        if (a.y > b.y) {
            std::swap(a, b);
        }
        return BresenhamCore(a, b);
    }

    public:
    Bresenham(ScreenPos a, ScreenPos b) :
        isSwapped(checkSwapped(a, b)),
        core(isSwapped ?
            transposedCore(a, b) :
            BresenhamCore(a, b)
        ) {
            if (isSwapped) {
                prevX = core.curxL();
                y = mini8(a.x, b.x);
            } else {
                prevX = 0;
                y = 0;
            }
        }

    int8_t bresenhamIter() {
        if (isSwapped) {
            // it's transposed, so instead we need to "count the runs of the fake-X output"
            int8_t x;
            do {
                x = core.bresenhamIter();
                y++;
            } while(x == prevX);
            prevX = x;
            return y;
        } else {
            return core.bresenhamIter();
        }
    }
    
};

template<typename Bres, typename Fill> 
void fillTriangle(ScreenPos a, ScreenPos b, ScreenPos c, Fill fill) {
    // sort them so a.y <= b.y <= c.y
    if (a.y > b.y) std::swap(a, b);
    if (b.y > c.y) std::swap(b, c);
    if (a.y > b.y) std::swap(a, b);

    bool bIsLeft = b.x <= c.x;

    ScreenPos &left = bIsLeft ? b : c;
    ScreenPos &right = bIsLeft ? c : b;


    // first, fill the "flat bottom triangle"
    Bres leftBres(a, left);
    Bres rightBres(a, right);

    int8_t y = a.y;
    for (; y < b.y; y++) {
        int8_t xLeft = leftBres.bresenhamIter();
        int8_t xRight = rightBres.bresenhamIter();
        fill(y, xLeft, xRight);
    }

    // we've now reached b (vertically speaking) and need to replace it with c
    if (bIsLeft) {
        leftBres = Bres(b, c);
    } else {
        rightBres = Bres(b, c);
    
    }

    // now, fill the "flat top triangle"
    for (; y < c.y; y++) {
        int8_t xLeft = leftBres.bresenhamIter();
        int8_t xRight = rightBres.bresenhamIter();
        fill(y, xLeft, xRight);
    }
}
// template<typename F>
// void doubleBresenham(int8_t ylo, int8_t yhi, int8_t x0lo, int8_t x0hi,)

// template<typename F>

