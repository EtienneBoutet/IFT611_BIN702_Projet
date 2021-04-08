#include "HirschBerg.h"
#include <vector>
#include <numeric>
#include <chrono>
#include <utility>
#include <stdlib.h> 
#include <numeric>
#include <math.h> 

using namespace std::chrono;

int verify(std::string S, std::string T, std::string expected_S, std::string expected_T);
void verifyTimeOnNChar(int n);

std::vector<std::pair<
    std::string, std::string
>> generateDnaStringPairs (int length, int how_many) {
    
    std::string lts[4] = {"A", "B", "G", "T"};
    auto generateDnaString = [&lts](int length) {
        std::string str = "";
        for (int i = 0; i < length; i++) {
            str.append(lts[rand() % 4]);
        }
        return str;
    };

    std::vector<std::pair<std::string, std::string>> res;
    for (int i = 0; i < how_many; i++) {
        res.push_back(
            { generateDnaString(length), generateDnaString(length) }
        );
    }

    return res;
}

int main(){
    std::vector<std::vector<std::string>> tests = {
        {"AAAABB", "BB", "AAAABB", "----BB"},
        {"AATCG", "AACG", "AATCG", "AA-CG"},
        {"AATCGAATG", "AACGCGTA", "AATCGAATG", "AA-CGCGTA"}
    };

    std::cout << "Unit tests : " << std::endl;
    int passed_tests = 0;

    for (auto test : tests) {
        passed_tests += verify(test[0],test[1],test[2],test[3]);
    }

    std::cout << "Passed tests : " << passed_tests << "/" << tests.size() << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "Stress test : " << std::endl;
    std::vector<int64_t> times;
    
    for (auto pair : generateDnaStringPairs(50, 50'000)) {
        H::ZW zw;
        std::string s_1 = pair.first;
        std::string s_2 = pair.second;

        auto pre = high_resolution_clock::now();
        zw = H::HirschBerg(s_1.c_str(), s_1.c_str() + s_1.length(),
                        s_2.c_str(), s_2.c_str() + s_2.length());
        auto post = high_resolution_clock::now();

        times.push_back(
            duration_cast<microseconds>(post-pre).count()
        );
    }

    auto size = times.size();

    double mean = std::accumulate(times.begin(), times.end(), 0.0) / size;

    auto variance_func = [&mean, &size](int64_t accumulator, const int64_t& val) {
        return accumulator + ((val - mean)*(val - mean) / ( size - 1));
    };

    auto variance = std::accumulate(times.begin(), times.end(), 0.0, variance_func);

    std::cout << "Calcul sur 50'000 strings de 50 caractères : " << std::endl; 
    std::cout << "Mean : "<< mean << " us"<< std::endl;
    std::cout << "Std : "<< sqrt(variance) << " us" << std::endl;

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