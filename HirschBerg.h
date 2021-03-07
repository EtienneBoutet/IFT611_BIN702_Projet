#include <iostream>
#include <algorithm>

namespace H{
    struct ZW {
        std::string Z;
        std::string W;

        ZW(std::string Z = "", std::string W = ""): Z(Z), W(W) {}

        ZW operator+(const ZW & zw) {
            return ZW(Z + zw.Z, W + zw.W);
        }
    };

    ZW HirschBerg(const char* beginX, const char* endX, const char* beginY, const char* endY);
}