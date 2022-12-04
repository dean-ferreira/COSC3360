#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <unistd.h>

struct symbolInfo {
    std::string symbol; // A
    double prob {}; // probability
    double fX {}; // F(x) 
    double fBar {}; // Fbar(x)
    std::string fBarBinary {}; // Fbar in binary
    int codeLength {}; // the code's length
    std::string codeWord {}; // the code word
};

void *SFE(void * void_ptr) {
    symbolInfo *arg = (struct symbolInfo *)void_ptr; // cast the void pointer to a pointer of type struct symbolInfo
    arg->fBar = (arg->fX - arg->prob) + arg->prob / 2; // calculate Fbar(x)
    double fBar = arg->fBar;
    std::string fBarInBinary {};
    while (fBar) {
        int bit = fBar * 2;
        fBar = fmod(fBar * 2, 1);
        fBarInBinary.append(std::to_string(bit)); // find Fbar(x) in binary
    }
    arg->fBarBinary = fBarInBinary;
    arg->codeLength = ceil(log2(1 / arg->prob) + 1); // calculate the code length

    for (int x = 0; x < arg->codeLength; x++) {
        arg->codeWord += arg->fBarBinary[x]; // assemble the code word based on the code length
    }

    return nullptr;
}

std::vector<symbolInfo> symbolContainer; // store each symbol and info

int main() {
    std::string symbolsString, probsString; // strings to store each line of input

    getline(std::cin, symbolsString); // get input for symbols
    getline(std::cin, probsString); // get input for probabilities

    std::vector<std::string> symbolsVector;
    std::string symbol;
    std::stringstream symbolStream(symbolsString);
    while (symbolStream >> symbol) {
        symbolsVector.push_back(symbol); // push all symbols to the vector
    }

    std::vector<double> probsVector;
    std::string prob;
    std::stringstream probStream(probsString);
    while (probStream >> prob) {
        probsVector.push_back(stod(prob)); // push all probabilities to the vector
    }

    double sumProb {};
    for (int i = 0; i < probsVector.size(); i++) {
        symbolInfo symbolLoader; // struct used to load each symbol and corresponding information to the vector
        symbolLoader.symbol = symbolsVector.at(i);
        symbolLoader.prob = probsVector.at(i);
        sumProb += symbolLoader.prob;
        symbolLoader.fX = sumProb;

        symbolContainer.push_back(symbolLoader); // push the struct loader onto the main vector
    }

    const int NTHREADS = symbolContainer.size(); // NTHREADS is determined by the number of symbols
    pthread_t tid [NTHREADS];

    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_create(&tid[i], nullptr, SFE, &symbolContainer.at(i)) != 0) { // create a thread for each symbol and call the SFE function
            std::cerr << "Cannot create thread" << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < NTHREADS; i++) {
         pthread_join(tid[i], nullptr); // join threads
    }

    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;
    for (int i = 0; i < NTHREADS; i++) {
        std::cout << "Symbol " << symbolContainer.at(i).symbol << ", Code: " << symbolContainer.at(i).codeWord << std::endl;
    }
    return 0;
}