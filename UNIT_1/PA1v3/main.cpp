#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <unistd.h>

struct symbolInfo {
    std::string symbol;
    double prob {};
    double fX {};
    double fBar {}; 
    std::string fBarBinary {};
    int codeLength {};
    std::string codeWord {};
};

void *SFE(void * params) {
    symbolInfo threadArg = *((symbolInfo *) params);
    double fBar = threadArg.fBar;
    std::string fBarInBinary {};
    while (fBar) {
        int bit = fBar * 2;
        fBar = fmod(fBar * 2, 1);
        fBarInBinary.append(std::to_string(bit));
    }
    threadArg.fBarBinary = fBarInBinary;
    threadArg.codeLength = ceil(log2(1 / threadArg.prob) + 1);

    for (int x = 0; x < threadArg.codeLength; x++) {
        threadArg.codeWord += threadArg.fBarBinary[x];
    }

    return nullptr;
}

int main() {
    std::vector<symbolInfo> symbolContainer; // store each symbol and info
    std::string symbolsString, probsString; // store each line of input

    getline(std::cin, symbolsString); // get input
    getline(std::cin, probsString);

    std::vector<std::string> symbolsVector;
    std::string symbol;
    std::stringstream symbolStream(symbolsString);
    while (symbolStream >> symbol) {
        symbolsVector.push_back(symbol);
    }

    std::vector<double> probsVector;
    std::string prob;
    std::stringstream probStream(probsString);
    while (probStream >> prob) {
        probsVector.push_back(stod(prob));
    }

    double sumProb {};
    for (int i = 0; i < probsVector.size(); i++) {
        symbolInfo symbolLoader; // used to load each symbol into the main symbolContainer
        symbolLoader.symbol = symbolsVector.at(i);
        symbolLoader.prob = probsVector.at(i);
        sumProb += symbolLoader.prob;
        symbolLoader.fX = sumProb;

        if (i == 0) {
            symbolLoader.fBar = symbolLoader.prob / 2;
        }
        else {
            symbolLoader.fBar = symbolContainer.at(i - 1).fX + symbolLoader.prob / 2;
        }

        symbolContainer.push_back(symbolLoader);
    }

    const int NTHREADS = symbolContainer.size();
    pthread_t tid [NTHREADS];

    for (int i = 0; i < NTHREADS; i++) {
        // sFE(symbolContainer.at(i));
        // std::cout << symbolContainer.at(i).symbol << " " << symbolContainer.at(i).prob << " " << symbolContainer.at(i).fX << " " << symbolContainer.at(i).fBar << " " << symbolContainer.at(i).fBarBinary << " " << symbolContainer.at(i).codeLength << " " << symbolContainer.at(i).codeWord << std::endl;

        if (pthread_create(&tid[i], nullptr, SFE, &symbolContainer.at(i)) != 0) {
            std::cerr << "Cannot create thread" << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < NTHREADS;) {
         pthread_join(tid[i], nullptr);
    }

    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;
    for (int i = 0; i < NTHREADS;) {
        std::cout << "Symbol " << symbolContainer.at(i).symbol << ", Code: " << symbolContainer.at(i).codeWord << std::endl;
    }

    return 0;
}