#pragma once

#include <cstdint>
#include <new>

#include "types.h"
#include "system/i8helpers.h"
#include "system/imul.h"

namespace graphics {

inline void swapPos(ScreenPos &a, ScreenPos &b) {
    int8_t tmp;

    tmp = a.x;
    a.x = b.x;
    b.x = tmp;

    tmp = a.y;
    a.y = b.y;
    b.y = tmp;
}

void output(ScreenPos p);

// https://mcejp.github.io/2020/11/06/bresenham.html
class BresenhamCore{
    int8_t xL;

    uint8_t twoDx, twoDySubTwoDx;

    uint8_t E;
    bool isPos;
    bool isExcl;

    void inc() {
        if (isPos) xL++;
        else xL--;
    }

public:
    // don't use this lol
    BresenhamCore() {}
    BresenhamCore(ScreenPos a, ScreenPos b, bool flipExcl) {
        xL = a.x;
        uint8_t dy = b.y - a.y;
        isPos = a.x <= b.x;
        isExcl = isPos;
        if (flipExcl) {
            isExcl = !isExcl;
        }
        uint8_t dx;
        if (isPos) {
            dx = b.x - a.x;
        } else {
            dx = a.x - b.x;
        }
        E = dy - dx;

        twoDySubTwoDx = 2*dy - 2*dx;
        twoDx = 2*dx;

        if (!isExcl && E == 0) {
            // E is "negated" compared to the positive algo
            // unfortunately E can equal 0 in the first step
            E += twoDySubTwoDx;
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
        bool shouldInc = E < twoDx || (!isExcl && E == twoDx);
        if (shouldInc) {
            inc();
            E += twoDySubTwoDx;
        } else {
            E -= twoDx;
        }
        return ret;
    }
    int8_t curxL() {
        return xL;
    }
};


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
    
    // friends don't let friends memcpy bresenham
    Bresenham(const Bresenham&) = delete;
    Bresenham(Bresenham&&) = delete;


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

static int iter = 0;

template<typename Bres, typename Fill> 
void fillTriangleGeneric(ScreenPos a, ScreenPos b, ScreenPos c, Fill &&fill) {
    // sort them so a.y <= b.y <= c.y
    if (a.y > b.y) swapPos(a, b);
    if (b.y > c.y) swapPos(b, c);
    if (a.y > b.y) swapPos(a, b);

    bool bIsLeft;
    if (b.y != a.y) {
        // annoying slope comparison to check if the left side is AB or AC
        // this is the thing where a/b < c/d turns into a*d < c*b
        bIsLeft = imul8To16(b.x - a.x, c.y - a.y) < imul8To16(c.x - a.x, b.y - a.y);
    } else {
        // annoying edge case: if there is no flat top triangle,
        // left side vs right side is determined by whether B is to the left
        // or right of A.
        bIsLeft = b.x < a.x;
    }

    ScreenPos &left = bIsLeft ? b : c;
    ScreenPos &right = bIsLeft ? c : b;

    // if a.y == b.y, need to skip [a, b]

    // first, fill the "flat top triangle"
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
        // I just want to avoid a memcpy lol...
        // leftBres = Bres(b, c);
        leftBres.~Bres();
        new (&leftBres) Bres(b, c);
    } else {
        // rightBres = Bres(b, c);
        rightBres.~Bres();
        new (&rightBres) Bres(b, c);
    }

    // now, fill the "flat bottom triangle"
    for (; y < c.y; y++) {
        int8_t xLeft = leftBres.bresenhamIter();
        int8_t xRight = rightBres.bresenhamIter();
        fill(y, xLeft, xRight);
    }
}

}