#include "HirschBerg.h"
#include <vector>


int verify(std::string S, std::string T, std::string expected_S, std::string expected_T);

int main(){
    std::vector<std::vector<std::string>> tests = {
        {"AAAABB", "BB", "AAAABB", "----BB"},
        {"AATCG", "AACG", "AATCG", "AA-CG"},
        {"AATCGAATG", "AACGCGTA", "AATCGAATG", "AA-CGCGTA"}
    };

    int passed_tests = 0;

    for (auto test : tests) {
        passed_tests += verify(test[0],test[1],test[2],test[3]);
    }

    std::cout << "Passed tests : " << passed_tests << "/" << tests.size() << std::endl;

    return 0;
}

int verify(std::string S, std::string T, std::string expected_S, std::string expected_T) {
    H::ZW zw = H::HirschBerg(S.c_str(), S.c_str() + S.length(), T.c_str(), T.c_str() + T.length());

    if (expected_S == zw.Z && expected_T == zw.W) {
        return 1;
    }
    
    std::cout << "Excepted S : " << expected_S << "\t but received : " << zw.Z << std::endl;
    std::cout << "Excepted T : " << expected_T << "\t but received : " << zw.W << std::endl;

    return 0;
}