#include "graphics/bresenham.h"


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>

class BresenhamReferencePos{
    ScreenPos a, b;

    int E, X_left;

public:
    BresenhamReferencePos(ScreenPos a, ScreenPos b) : a(a), b(b), E((b.y - a.y) - (b.x - a.x)), X_left(a.x){}

    int8_t bresenhamIter() {
        while (E < 0) {
            X_left ++;
            E += 2 * (b.y - a.y);
        }

        E -= 2 * (b.x - a.x);

        return (int8_t) X_left;
    }
};

// negate -(b.y - a.y) - (b.x - a.x)
// (b.y - a.y) - (a.x - b.x)
class BresenhamReferenceNeg{
    ScreenPos a, b;


    int E, X_left;

public:
    BresenhamReferenceNeg(ScreenPos a, ScreenPos b) : a(a), b(b), E(-(b.y - a.y) - (b.x - a.x)), X_left(a.x){}

    int8_t bresenhamIter() {
        while (E >= 0) {
            X_left --;
            E -= 2 * (b.y - a.y);
        }

        E -= 2 * (b.x - a.x);

        return (int8_t) X_left;
    }
};


class BresenhamReference {
    BresenhamReferencePos pos;
    BresenhamReferenceNeg neg;

    bool isPos;
public:
    BresenhamReference(ScreenPos a, ScreenPos b) :
        isPos(b.x >= a.x), pos(a, b), neg(a, b) {}

    int8_t bresenhamIter() {
        if (isPos) {
            return pos.bresenhamIter();
        } else {
            return neg.bresenhamIter();
        }
    }

};


void output(ScreenPos p) {
    std::cout << "{" << (int)p.x << "," << (int) p.y << "}";
}
void output(ScreenPos a, ScreenPos b, ScreenPos c) {
    output(a);
    std::cout << ", ";
    output(b);
    std::cout << ", ";
    output(c);
    std::cout << std::endl;
}

struct BresTest {
    size_t w, h;
    std::string answerStr;
    ScreenPos a, b, c;

    template<typename Bres>
    bool testCombo(ScreenPos a, ScreenPos b, ScreenPos c) {
        std::vector<std::vector<bool>> bits(h, std::vector<bool>(w,false));

        
        fillTriangleGeneric<Bres>(a, b, c, [&](int8_t y, int8_t xLeft, int8_t xRight) {
            for (int x = xLeft; x < xRight; x++) {
                bits[y][x] = true;
            }
        });

        std::stringstream ss;

        ss << "\n";


        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x++) {
                ss << (bits[y][x] ? "#" : ".");
            }
            ss << "\n";
        }

        std::string res = ss.str();
        if (res == answerStr) {
            return true;
        }
        std::cout << "OOPS. ";
        output(a, b, c);
        std::cout  << "expected:";
        std::cout << answerStr << "actual:" << res << std::flush;
        return false;
    }

    template<typename Bres>
    bool testAllCombos() {
        if (!testCombo<Bres>(a, b, c)) return false;
        if (!testCombo<Bres>(b, c, a)) return false;
        if (!testCombo<Bres>(c, a, b)) return false;
        if (!testCombo<Bres>(c, b, a)) return false;
        if (!testCombo<Bres>(a, c, b)) return false;
        return true;
    }
};



static std::vector<BresTest> suite = {
    {
        6, 4,
        R"(
......
.##...
.####.
..#...
)",
        {1, 1},
        {2,4},
        {6,2}
    },
    {
        4, 4,
        R"(
....
.#..
....
....
)",
        {1, 1},
        {1,3},
        {3,1}
    },
    {
        4, 4,
        R"(
....
..#.
.##.
....
)",
        {3, 1},
        {1,3},
        {3,3}
    },
    {
        7, 3,
        R"(
....#..
.###...
.......
)",
        {0, 2},
        {4, 2},
        {6,0}
    },
    {
        7, 3,
        R"(
.....#.
....##.
.....##
)",
        {6,0},
        {4, 2},
        {7,3}
    },
    {
        4, 4,
        R"(
....
#...
##..
..#.
)",
        {0,1},
        {0, 3},
        {4,4}
    }
};


struct LineResult {
    int8_t y, xLeft, xRight;
    void write() {
        std::cout << "(" << (int) y << ", " << (int) xLeft << ", " << (int) xRight << ")" << std::endl;
    }
};


template<typename Bres1, typename Bres2>
bool testSameAlgo(ScreenPos a, ScreenPos b) {
    if (a.y > b.y) std::swap(a, b);
    // output(a);
    // std::cout << ",";
    // output(b);
    // std::cout << std::endl;

    Bres1 bres1(a, b);
    Bres2 bres2(a, b);

    bool good = true;
    for (int8_t y = a.y; y < b.y; y++) {
        int8_t res1 = bres1.bresenhamIter();
        int8_t res2 = bres2.bresenhamIter();
        if (res1 != res2) {
            output(a);
            std::cout << ",";
            output(b);
            std::cout << "differing at y=" << (int) y  << ": " << (int) res1 << " vs " << (int) res2 << std::endl;
            // return false;
            good = false;
        } else {
            // std::cout << "ref y=" << (int) y  << ": " << (int) res1 << std::endl;
        }
    }
    return good;
}


template<typename Bres>
std::vector<ScreenPos> getBresData(ScreenPos a, ScreenPos b) {
    if (a.y > b.y) std::swap(a, b);

    output(a);
    std::cout << ",";
    output(b);
    std::cout << std::endl;
    Bres bres(a, b);

    std::vector<ScreenPos> out;

    for (int8_t y = a.y; y < b.y; y++) {
        int8_t x = bres.bresenhamIter();
        out.push_back({x, y});
    }
    return out;
}

void writeVec(std::vector<ScreenPos> &vec) {
    std::cout << "[";
    for (auto &p : vec) {
        output(p);
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

template<typename Bres>
bool validateTranspose(ScreenPos a, ScreenPos b) {

    std::cout << "regular" << std::endl;
    auto regular = getBresData<Bres>(a, b);

    writeVec(regular);

    std::cout << "transposed" << std::endl;
    auto transposed = getBresData<Bres>(a.transpose(), b.transpose());
    
    writeVec(transposed);
/*
observations
invariant: output length should be absDiffi8(aT.x, bT.x)
last run of output.x is not included in output because input.y is exclusive
uses the last output.x of each run as true Y
*/

    return true;
}


const size_t ITERS = 1000000;
int main(int argc, char** argv) {

    std::cout << "running fixed test suite" << std::endl;

    for (auto &test : suite) {
        if (!test.testAllCombos<BresenhamReference>()) {
            return 1;
        }
    }

    // validateTranspose<BresenhamReference>({10,11},{7,13});
    // std::cout << "##############" << std::endl;
    // if (!testSameAlgo<BresenhamReference, Bresenham>({10,11},{7,13})) {
    //     return 1;
    // }

    // validateTranspose<BresenhamReference>({10,11},{4,15});
    // std::cout << "##############" << std::endl;
    // if (!testSameAlgo<BresenhamReference, Bresenham>({10,11},{4,15})) {
    //     return 1;
    // }

    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int8_t> uniform_dist(-64, 63);
    auto randPos = [&]() { return ScreenPos { uniform_dist(e1), uniform_dist(e1)};};


    std::cout << "fuzzing against reference... (" << ITERS << " iters)" << std::endl;


    // std::cout << "CorePos" << std::endl;
    // for (size_t i = 0; i < ITERS; i++) {
    //     ScreenPos a = randPos(), b = randPos();
    //     // make dy bigger
    //     int8_t x = a.x + std::min(b.x, b.y);
    //     int8_t y = a.y + std::max(b.x, b.y);
    //     if (!testSameAlgo<BresenhamReference, BresenhamCore>(a, {x, y})) {
    //         return 1;
    //     }
    // }

    // std::cout << "CoreNeg" << std::endl;
    // for (size_t i = 0; i < ITERS; i++) {
    //     ScreenPos a = randPos(), b = randPos();
    //     // make dy bigger
    //     int8_t x = a.x - std::min(b.x, b.y);
    //     int8_t y = a.y + std::max(b.x, b.y);
    //     if (!testSameAlgo<BresenhamReference, BresenhamCore>(a, {x, y})) {
    //         return 1;
    //     }
    // }

    
    // std::cout << "CorePosT" << std::endl;
    // for (size_t i = 0; i < ITERS; i++) {
    //     ScreenPos a = randPos(), b = randPos();
    //     // make dx bigger
    //     int8_t x = a.x + std::max(b.x, b.y);
    //     int8_t y = a.y + std::min(b.x, b.y);
    //     if (!testSameAlgo<BresenhamReference, Bresenham>(a, {x, y})) {
    //         return 1;
    //     }
    // }

    
    // std::cout << "CoreNegT" << std::endl;
    // for (size_t i = 0; i < ITERS; i++) {
    //     ScreenPos a = randPos(), b = randPos();
    //     // make dx bigger
    //     int8_t x = a.x - std::max(b.x, b.y);
    //     int8_t y = a.y + std::min(b.x, b.y);
    //     if (!testSameAlgo<BresenhamReference, Bresenham>(a, {x, y})) {
    //         return 1;
    //     }
    // }

    for (size_t i = 0; i < ITERS; i++) {
        ScreenPos a = randPos(), b = randPos();
        // make y bigger for b
        if (a.y > b.y) {
            std::swap(a, b);
        }
        if (!testSameAlgo<BresenhamReference, Bresenham>(a, b)) {
            return 1;
        }
    }


    std::cout << "passed all tests" << std::endl;
    

    return 0;
}