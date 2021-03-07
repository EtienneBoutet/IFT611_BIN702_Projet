#include "HirschBerg.h"
namespace H{
    int Ins(const char & c);
    int Del(const char & c);
    int Sub(const char & c1, const char& c2);

    int * NWScore(const char* beginX, const char* endX, const char* beginY, const char* endY);
    int * NWScoreRev(const char* beginX, const char* endX, const char* beginY, const char* endY);

    ZW NeedlemanWunsch(const char* beginX, const char* endX, const char* beginY, const char* endY);

    int Ins(const char& c) {
        return -1;
    }

    int Del(const char& c) {
        return -1;
    }

    int Sub(const char& c1, const char& c2) {
        return c1 == c2 ? 2 : -1;
    }

    int * NWScore(const char* beginX, const char* endX, const char* beginY, const char* endY){
        int scoresize = (endY - beginY) + 1;
        int (*score)[2] = new int[scoresize][2];
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

        int* lastLine = new int[scoresize];
        for (int j = 0; j < scoresize; j++) {
            lastLine[j] = score[j][1];
        }

        delete [] score;
        return lastLine;
    }

    int * NWScoreRev(const char* beginX, const char* endX, const char* beginY, const char* endY) {
        int scoresize = (endY - beginY) + 1;
        int(*score)[2] = new int[scoresize][2];
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

        int* lastLine = new int[scoresize];
        for (int j = 0; j < scoresize; j++) {
            lastLine[j] = score[scoresize - 1 - j][1];
        }
        delete [] score;
        return lastLine;
    }

    ZW HirschBerg(const char* beginX, const char* endX, const char* beginY, const char* endY) {
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
            zw = NeedlemanWunsch(beginX, endX, beginY, endY);
        }
        else {
            int xlen = endX - beginX;
            int xmid = xlen / 2;
            int ylen = endY - beginY;

            int * ScoreL = NWScore(beginX, beginX + xmid, beginY, endY);
            int * ScoreR = NWScoreRev(beginX + xmid, endX, beginY, endY);

            int ymid = 0;
            for (int i = 0; i < ylen + 1; i++) {
                ScoreL[i] += ScoreR[i];
                ymid = ScoreL[i] > ScoreL[ymid] ? i : ymid;
            }

            delete[] ScoreL;
            delete[] ScoreR;


            zw = HirschBerg(beginX, beginX + xmid, beginY, beginY + ymid)
                + HirschBerg(beginX + xmid, endX, beginY + ymid, endY);
        }
        return zw;
    }

    ZW NeedlemanWunsch(const char* beginX, const char* endX, const char* beginY, const char* endY) {
        ZW zw;
        
        if (endX - beginX == 1) {
            int scoresize = (endY - beginY) + 1;
            int(*score)[2] = new int[scoresize][2];
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

            delete[] score;
        }
        else {
            ZW zwPrime = NeedlemanWunsch(beginY, endY, beginX, endX);
            zw.Z = zwPrime.W;
            zw.W = zwPrime.Z;
        }

        return zw;
        
    }
}