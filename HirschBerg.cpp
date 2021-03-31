#include "HirschBerg.h"

namespace H{
    class TypeScoreTable{};
    class TypeLastLine{};

    class HMemory{
        int * mem;

        //for score table
        int * memscoreTable;


        //for lastlines
        bool dispolastlines[2]; 
        int * memlastlines[2];
    public:
        HMemory(size_t size){
            mem = new int[size*4];
            memscoreTable = mem;
            for(int i = 0; i < 2; i++){
                memlastlines[i] = mem + (2 + i) * size;
            }
            dispolastlines[0] = dispolastlines[1] = true;
        }

        int * allouerScoreTable(){
            return memscoreTable;
        }

        int * allouerLastline(){
            for (int i = 0; i < 2; i++)
            {
                if (dispolastlines[i]){
                    dispolastlines[i] = false;
                    return memlastlines[i];
                }
            }
            return nullptr; 
        }

        void releaseLastline(void * p){
            for (int i = 0; i < 2; i++)
            {
                if (p == memlastlines[i]){
                    dispolastlines[i] = true;
                }
            }
        }

        ~HMemory(){
            delete [] mem;
        }
    };
}

void * operator new[](size_t n, H::HMemory &ges, H::TypeLastLine) {
    return ges.allouerLastline();
}

void operator delete[](void *p, H::HMemory &ges, H::TypeLastLine) {
    ges.releaseLastline(p);
}

void * operator new[](size_t n, H::HMemory &ges, H::TypeScoreTable) {
    return ges.allouerScoreTable();
}

void operator delete[](void *p, H::HMemory &ges, H::TypeScoreTable) {
}

namespace H{
    int Ins(const char & c);
    int Del(const char & c);
    int Sub(const char & c1, const char& c2);

    int * NWScore(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem);
    int * NWScoreRev(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem);

    ZW NeedlemanWunsch(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem);

    ZW HirschBerg(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem);

    int Ins(const char& c) {
        return -1;
    }

    int Del(const char& c) {
        return -1;
    }

    int Sub(const char& c1, const char& c2) {
        return c1 == c2 ? 2 : -1;
    }

    int * NWScore(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem){
        int scoresize = (endY - beginY) + 1;
        int (*score)[2] = new(mem, TypeScoreTable{}) int[scoresize][2];
        score[0][0] = 0;
        
        for (int j = 1; j < scoresize; j++) {
            score[j][0] = score[j - 1][0] + Ins(beginY[j - 1]);
        }

        for (int i = 1; i <= endX - beginX; i++) {
            score[0][1] = score[0][0] + Del(beginX[i - 1]);
            for (int j = 1; j < scoresize; j++) {
                int scoreSub = score[j - 1][0] + Sub(beginX[i - 1], beginY[j - 1]);
                int scoreDel = score[j][0] + Del(beginX[i - 1]);
                int scoreIns = score[j - 1][1] + Ins(beginY[j - 1]);
                score[j][1] = std::max(std::max(scoreSub, scoreDel), scoreIns);
            }
            for (int j = 0; j < scoresize; j++) {
                score[j][0] = score[j][1];
            }
        }

        int* lastLine = new(mem, TypeLastLine{}) int[scoresize];
        for (int j = 0; j < scoresize; j++) {
            lastLine[j] = score[j][1];
        }

        ::operator delete[](score, mem, TypeScoreTable{});
        return lastLine;
    }

    int * NWScoreRev(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem) {
        int scoresize = (endY - beginY) + 1;
        int(*score)[2] = new(mem, TypeScoreTable{}) int[scoresize][2];
        score[0][0] = 0;

        for (int j = 1; j < scoresize; j++) {
            score[j][0] = score[j - 1][0] + Ins(endY[-j]);
        }

        for (int i = 1; i <= endX - beginX; i++) {
            score[0][1] = score[0][0] + Del(endX[-1]);
            for (int j = 1; j < scoresize; j++) {
                int scoreSub = score[j - 1][0] + Sub(endX[-i], endY[-j]);
                int scoreDel = score[j][0] + Del(endX[-i]);
                int scoreIns = score[j - 1][1] + Ins(endY[-j]);
                score[j][1] = std::max(std::max(scoreSub, scoreDel), scoreIns);
            }
            for (int j = 0; j < scoresize; j++) {
                score[j][0] = score[j][1];
            }
        }

        int* lastLine = new(mem, TypeLastLine{}) int[scoresize];
        for (int j = 0; j < scoresize; j++) {
            lastLine[j] = score[scoresize - 1 - j][1];
        }
        ::operator delete[](score, mem, TypeScoreTable{});
        return lastLine;
    }

    
    ZW HirschBerg(const char* beginX, const char* endX, const char* beginY, const char* endY) {
        HMemory mem(endX - beginX + endY - beginY);
        return HirschBerg(beginX, endX, beginY, endY, mem);
    }

    ZW HirschBerg(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem) {
        ZW zw;
        if (!(endX - beginX)) {
            std::for_each(beginY, endY, [&zw](const char& c) {
                zw.Z += "-";
                zw.W += c;
                });
        }
        else if (!(endY - beginY)) {
            std::for_each(beginX, endX, [&zw](const char& c) {
                zw.Z += c;
                zw.W += "-";
                });
        }
        else if ((endX - beginX) == 1 || (endY - beginY) == 1) {
            zw = NeedlemanWunsch(beginX, endX, beginY, endY, mem);
        }
        else {
            int xlen = endX - beginX;
            int xmid = xlen / 2;
            int ylen = endY - beginY;

            int * ScoreL = NWScore(beginX, beginX + xmid, beginY, endY, mem);
            int * ScoreR = NWScoreRev(beginX + xmid, endX, beginY, endY, mem);

            int ymid = 0;
            for (int i = 0; i < ylen + 1; i++) {
                ScoreL[i] += ScoreR[i];
                ymid = ScoreL[i] > ScoreL[ymid] ? i : ymid;
            }

            ::operator delete[](ScoreL, mem, TypeLastLine{});
            ::operator delete[](ScoreR, mem, TypeLastLine{});


            zw = HirschBerg(beginX, beginX + xmid, beginY, beginY + ymid, mem)
                + HirschBerg(beginX + xmid, endX, beginY + ymid, endY, mem);
        }
        return zw;
    }

    ZW NeedlemanWunsch(const char* beginX, const char* endX, const char* beginY, const char* endY, HMemory& mem) {
        ZW zw;
        
        if (endX - beginX == 1) {
            int scoresize = (endY - beginY) + 1;
            int(*score)[2] = new(mem, TypeScoreTable{}) int[scoresize][2];
            score[0][0] = 0;

            for (int j = 1; j < scoresize; j++) {
                score[j][0] = score[j - 1][0] + Ins(beginY[j - 1]);
            }
        
            score[0][1] = score[0][0] + Del(beginX[0]);
            for (int j = 1; j < scoresize; j++) {
                int scoreSub = score[j - 1][0] + Sub(beginX[0], beginY[j - 1]);
                int scoreDel = score[j][0] + Del(beginX[0]);
                int scoreIns = score[j - 1][1] + Ins(beginY[j - 1]);
                score[j][1] = std::max(std::max(scoreSub, scoreDel), scoreIns);
            }

            int i = 1;
            int j = scoresize - 1;

            while (i || j) {
                if (score[j - 1][i] + Ins(beginY[j - 1]) == score[j][i]) {
                    zw.Z = '-' + zw.Z;
                    zw.W = beginY[j - 1] + zw.W;
                    j--;
                }
                else if (score[j - 1][0] + Sub(beginX[0], beginY[j - 1]) == score[j][i]) {
                    zw.Z = beginX[i - 1] + zw.Z;
                    zw.W = beginY[j - 1] + zw.W;
                    i--;
                    j--;
                }
                else if (score[j][0] + Del(beginX[0]) == score[j][i]) {
                    zw.Z = beginX[i - 1] + zw.Z;
                    zw.W = '-' + zw.W;
                    i--;
                }
            }

            ::operator delete[](score, mem, TypeScoreTable{});
        }
        else {
            ZW zwPrime = NeedlemanWunsch(beginY, endY, beginX, endX, mem);
            zw.Z = zwPrime.W;
            zw.W = zwPrime.Z;
        }

        return zw;
        
    }
}