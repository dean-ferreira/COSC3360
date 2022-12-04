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

int main() {
    std::vector<symbolInfo> probContainer;
    std::string symbolString, probString;
    std::vector<std::string> symbolVector;
    std::vector<double> probVector;

    getline(std::cin, symbolString);
    getline(std::cin, probString);

    std::string symbol;
    std::stringstream symbolStream(symbolString);
    while (symbolStream >> symbol) {
        symbolVector.push_back(symbol);
    }

    std::string prob;
    std::stringstream probStream(probString);
    while (probStream >> prob) {
        probVector.push_back(stod(prob));
    }

    symbolInfo probLoader;
    for (int i = 0; i < probVector.size(); i++) {
        probLoader.symbol = symbolVector.at(i);
        probLoader.prob = probVector.at(i);
        probContainer.push_back(probLoader);
    }

    double sumProb {};
    for (int i = 0; i < probContainer.size(); i++) {
        if (i == 0) {
            probContainer.at(i).fX = probContainer.at(i).prob;
            sumProb += probContainer.at(i).prob;

            probContainer.at(i).fBar = probContainer.at(i).prob / 2;
        }
        else {
            sumProb += probContainer.at(i).prob;
            probContainer.at(i).fX = sumProb;

            probContainer.at(i).fBar = probContainer.at(i - 1).fX + probContainer.at(i).prob / 2;
        }

        double fBar = probContainer.at(i).fBar;
        std::string fBarBin {};
        while (fBar) {
            int bit = fBar * 2;
            fBar = fmod(fBar * 2, 1);
            fBarBin.append(std::to_string(bit));
        }
        probContainer.at(i).fBarBinary = fBarBin;

        probContainer.at(i).codeLength = ceil(log2(1 / probContainer.at(i).prob) + 1);

        for (int j = 0; j < probContainer.at(i).codeLength; j++) {
            probContainer.at(i).codeWord += probContainer.at(i).fBarBinary[j];
        }
    }

    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;
    for (int i = 0; i < probContainer.size(); i++) {
        std::cout << "Symbol " << probContainer.at(i).symbol << ", Code: " << probContainer.at(i).codeWord << std::endl;
        //std::cout << probContainer.at(i).symbol << " " << probContainer.at(i).prob << " " << probContainer.at(i).fX << " " << probContainer.at(i).fBar << " " << probContainer.at(i).fBarBinary << " " << probContainer.at(i).codeLength << " " << probContainer.at(i).codeWord << std::endl;
    }

    return 0;
}