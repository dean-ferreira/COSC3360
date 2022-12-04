#include <iostream>
#include <algorithm>
#include <vector>

// struct symbolInfo {
//     std::string symbol; // A
//     double prob {}; // probability
//     double fX {}; // F(x) 
//     double fBar {}; // Fbar(x)
//     std::string fBarBinary {}; // Fbar in binary
//     int codeLength {}; // the code's length
//     std::string codeWord {}; // the code word
// };

struct Symbol {
    char symbolLetter {};
    int frequency {};

    double prob {};
    double fX {}; // cumalative probability F(x)
    double Fbar {}; // Fbar(x)
};

bool compareByFrequency(const Symbol &a, const Symbol &b) {
    return a.frequency > b.frequency;
}

bool compareByLetter(const Symbol &a, const Symbol &b) {
    return a.symbolLetter < b.symbolLetter;
}

int CalculateTotalEntries(const std::vector<Symbol> &symbolContainer) {
    int totalEntries {};
    for (size_t i = 0; i < symbolContainer.size(); i++) {
        totalEntries += symbolContainer.at(i).frequency;
    }

    return totalEntries;
}

void PrintSymbolContainter(const std::vector<Symbol> &symbolContainer);

void BuildSymbolContainer(std::vector<Symbol> &symbolContainer);
void CalculateSymbolProb(std::vector<Symbol> &symbolContainer);
void CalculateSymbolfX(std::vector<Symbol> &symbolContainer);

int main(int argc, char *argv[]) {
    // std::cout << argv[0] << std::endl; // ./main
    // std::cout << "Hostname: " << argv[1] << std::endl;
    // std::cout << "Port Number: " << argv[2] << std::endl;
    // std::cout << "Input_filename: " << argv[3] << std::endl;
    
    // client program
    std::vector<Symbol> symbolContainer;
    BuildSymbolContainer(symbolContainer);
    CalculateSymbolProb(symbolContainer);
    CalculateSymbolfX(symbolContainer);
    PrintSymbolContainter(symbolContainer);

    return 0;
}

void PrintSymbolContainter(const std::vector<Symbol> &symbolContainer) {
    for (size_t i = 0; i < symbolContainer.size(); i++) {
        std::cout << "Letter: " << symbolContainer.at(i).symbolLetter;
        std::cout << " Freq: " << symbolContainer.at(i).frequency;
        std::cout << " Prob: " << symbolContainer.at(i).prob;
        std::cout << " F(x): " << symbolContainer.at(i).fX;
        std::cout << std::endl;
    }
}
void BuildSymbolContainer(std::vector<Symbol> &symbolContainer) {
    std::string message {};
    std::getline(std::cin, message);
    
    // AAAAAABBBBBCCCCDDDEE
    for (size_t i = 0; i < message.length(); i++) {
        bool letterExists = false;
        int letterPosition {};
        for (int j = 0; j < symbolContainer.size(); j++) {
            if (message.at(i) == symbolContainer.at(j).symbolLetter) {
                letterExists = true;
                letterPosition = j;
            }
        }
        if (letterExists) {
            symbolContainer.at(letterPosition).frequency += 1;
        }
        else {
            struct Symbol newSymbol;
            newSymbol.symbolLetter = message.at(i);
            newSymbol.frequency = 1;
            symbolContainer.push_back(newSymbol);
        }
    }

    std::sort(symbolContainer.begin(), symbolContainer.end(), compareByFrequency);
    std::sort(symbolContainer.begin(), symbolContainer.end(), compareByLetter);

    // for (int i = 0; i < symbolContainer.size(); i++) {
    //     std::cout << "Letter: " << symbolContainer.at(i).symbolLetter << " Frequency: " << symbolContainer.at(i).frequency << std::endl;
    // }   
}

void CalculateSymbolProb(std::vector<Symbol> &symbolContainer) {
    int totalEntries = CalculateTotalEntries(symbolContainer);
    for (size_t i = 0; i < symbolContainer.size(); i++) {
        symbolContainer.at(i).prob = static_cast<double>(symbolContainer.at(i).frequency) / totalEntries;
    }
}

void CalculateSymbolfX(std::vector<Symbol> &symbolContainer) {
    double sumProb {};
    for (size_t i = 0; i < symbolContainer.size(); i++) {
        sumProb += symbolContainer.at(i).prob;
        symbolContainer.at(i).fX = sumProb;
    }
}