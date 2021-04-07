#include "HirschBerg.h"
#include <vector>
#include <numeric>
#include <chrono>

using namespace std::chrono;

int verify(std::string S, std::string T, std::string expected_S, std::string expected_T);
void verifyTimeOnNChar(int n);

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

    verifyTimeOnNChar(4);

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


void verifyTimeOnNChar(int n){
    char cs[4] = {'A', 'B', 'G', 'T'};
    size_t size = 4;
    int * arr = new int[n * 2];

    for(int i = 0; i < n * 2; i++){
        arr[i] = 0;
    }

    while (true)
    {
        int i = 0;
        std::string s = "";
        for(int i = 0; i < n * 2; i++){
            s += cs[arr[i]];
        }
        std::cout << s << std::endl;
        auto pre = high_resolution_clock::now();
        H::ZW zw = H::HirschBerg(s.c_str(), s.c_str() + n, s.c_str() + n, s.c_str() + n*2);
        auto post = high_resolution_clock::now();
        auto time = post - pre;
        std::cout << "time : " << duration_cast<microseconds>(time).count() << std::endl;
        std::cout << "received : " << zw.Z << std::endl;
        std::cout << "received : " << zw.W << std::endl;
        for (; i < n * 2;){
            arr[i]++;
            if (arr[i] == size){
                arr[i] = 0;
                i++;
                
            }
            else{
                break;
            }
        }
        if (i == n * 2){
            break;
        }
    }
}

