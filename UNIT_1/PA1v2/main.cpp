#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

struct symbolInfo {
    std::string symbol;
    double prob {};
    double fX {};
    double fBar {}; 
    std::string fBarBinary {};
    int codeLength {};
    std::string codeWord {};
};

void shannonFano (std::vector<symbolInfo> &container) {
    for (int i = 0; i < container.size(); i++) {
        if (i == 0) {
            container.at(i).fBar = container.at(i).prob / 2;
        }
        else {
            container.at(i).fBar = container.at(i - 1).fX + container.at(i).prob / 2;
        }

        double fBar = container.at(i).fBar;
        std::string fBarBin {};
        while(fBar) {
            int bit = fBar * 2;
            fBar = fmod(fBar * 2, 1);
            fBarBin.append(std::to_string(bit));
        }
        container.at(i).fBarBinary = fBarBin;
        container.at(i).codeLength = ceil(log2(1 / container.at(i).prob) + 1);

        for (int j = 0; j < container.at(i).codeLength; j++) {
            container.at(i).codeWord += container.at(i).fBarBinary[j];
        }
    }
}

int main() {
    std::vector<symbolInfo> probContainer; // store each symbol and info
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
        symbolInfo probLoader; // used to load each symbol into the main probVector
        probLoader.symbol = symbolsVector.at(i);
        probLoader.prob = probsVector.at(i);
        sumProb += probLoader.prob;
        probLoader.fX = sumProb;
        probContainer.push_back(probLoader);
    }

    shannonFano(probContainer);

    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;
    for (int i = 0; i < probContainer.size(); i++) {
        std::cout << "Symbol " << probContainer.at(i).symbol << ", Code: " << probContainer.at(i).codeWord << std::endl;
        // std::cout << probContainer.at(i).symbol << " " << probContainer.at(i).prob << " " << probContainer.at(i).fX << " " << probContainer.at(i).fBar << " " << probContainer.at(i).fBarBinary << " " << probContainer.at(i).codeLength << " " << probContainer.at(i).codeWord << std::endl;
    }
    return 0;
}