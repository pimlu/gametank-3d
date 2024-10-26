#pragma once

#include <cstdint>


namespace graphics {

struct ScreenPos {
    int8_t x, y;
    inline ScreenPos transpose() {
        return {y, x};
    }

    inline ScreenPos flip() {
        return {(int8_t)-y, x};
    }
    inline ScreenPos unflip() {
        return {y, (int8_t)-x};
    }
};

inline void swapPos(ScreenPos &a, ScreenPos &b) {
    int8_t tmp;

    tmp = a.x;
    a.x = a.y;
    a.y = tmp;

    tmp = b.x;
    b.x = b.y;
    b.y = tmp;
}

void output(ScreenPos p);

// https://mcejp.github.io/2020/11/06/bresenham.html
class BresenhamCore{
    int8_t xL;

    uint8_t dx, dy;

    uint8_t E;
    bool isPos;
    bool isExcl;

    void inc() {
        if (isPos) xL++;
        else xL--;
    }

public:

    // needed for annoying copy reasons
    BresenhamCore() : xL(0), dx(0), dy(0), E(0), isPos(true), isExcl(false) {}
    BresenhamCore(ScreenPos a, ScreenPos b, bool flipExcl) {
        xL = a.x;
        dy = b.y - a.y;
        isPos = a.x <= b.x;
        isExcl = isPos;
        if (flipExcl) {
            isExcl = !isExcl;
        }
        if (isPos) {
            dx = b.x - a.x;
        } else {
            dx = a.x - b.x;
        }
        E = dy - dx;
        if (!isExcl && E == 0) {
            // E is "negated" compared to the positive algo
            // unfortunately E can equal 0 in the first step
            E += 2 * dy - 2 * dx;
            inc();
        }
    }
    // assumption: dy >= dx
    BresenhamCore(ScreenPos a, ScreenPos b) : BresenhamCore(a, b, false) {
    }
    int8_t bresenhamIter() {
        // in the real algorithm, the lowest E can get to is -dx.
        // we've established since E is unsigned that the algorithm doesn't trigger
        // the while loop in the first pass. so we can safely "rotate" the phase of the algorithm
        // the point of rotating it is to keep it positive (add before subtracting)
        int8_t ret = xL;
        // if subtracting 2*dx would underflow...
        bool shouldInc = E < 2*dx || (!isExcl && E == 2*dx);
        if (shouldInc) {
            inc();
            E += 2 * dy - 2 * dx;
        } else {
            E -= 2*dx;
        }
        return ret;
    }
    int8_t curxL() {
        return xL;
    }
};

inline int8_t mini8(int8_t a, int8_t b) {
    return a <= b ? a : b;
}
inline uint8_t absDiffi8(int8_t a, int8_t b) {
    if (a >= b) {
        return a - b;
    } else {
        return b - a;
    }
}

class Bresenham {
    bool isSwapped;
    bool swappedIsPos;
    BresenhamCore core;
    int8_t fakeY;
    int8_t prevFakeX;

    static bool checkSwapped(ScreenPos a, ScreenPos b) {
        uint8_t dx = absDiffi8(a.x, b.x), dy = absDiffi8(a.y, b.y);
        return dx > dy;
    }
    


    public:
    Bresenham(ScreenPos a, ScreenPos b) :
        isSwapped(checkSwapped(a, b)) {
            if (isSwapped) {
                /*
                observations
                invariant: output length should be absDiffi8(aT.x, bT.x)
                last run of output.x is not included in output because input.y is exclusive
                uses the last output.x of each run as true Y
                */
                ScreenPos aT = a.flip(), bT = b.flip();
                if (aT.y > bT.y) {
                    swapPos(aT, bT);
                }
                prevFakeX = aT.x;
                // std::cout << "prevFakeX=" << (int) prevFakeX << std::endl;
                swappedIsPos = aT.x >= bT.x;
                fakeY = (swappedIsPos ? aT.y - 1 : bT.y + 1);
                // std::cout << "fakeY=" << (int) fakeY << std::endl;
                core = BresenhamCore(aT, bT);
            } else {
                core = BresenhamCore(a, b);
            }
        }

    int8_t bresenhamIter() {
        if (isSwapped) {

            int8_t fakeX;
            do {
                fakeX = core.bresenhamIter();
                // std::cout << (int) fakeX << " " << (int) fakeY << std::endl;
                if (swappedIsPos) fakeY++;
                else fakeY--;

                // std::cout << "at fX=" << (int) fakeX <<  ", fY=" << (int) fakeY << std::endl;
                // std::cout << (fakeX == prevFakeX ? "continuing." : "breaking.") << std::endl;
            } while(fakeX == prevFakeX);
            prevFakeX = fakeX;

            return fakeY;
        } else {
            return core.bresenhamIter();
        }
    }
    
};

template<typename Bres, typename Fill> 
void fillTriangleGeneric(ScreenPos a, ScreenPos b, ScreenPos c, Fill fill) {
    // sort them so a.y <= b.y <= c.y
    if (a.y > b.y) swapPos(a, b);
    if (b.y > c.y) swapPos(b, c);
    if (a.y > b.y) swapPos(a, b);

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
        if (xLeft < xRight) {
            fill(y, xLeft, xRight);
        }
    }
}


template<typename Fill>
void fillTriangle(ScreenPos a, ScreenPos b, ScreenPos c, Fill fill) {
    fillTriangleGeneric<Bresenham>(a, b, c, fill);
}

}