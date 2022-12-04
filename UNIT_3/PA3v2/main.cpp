#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <algorithm>

struct Symbol {
    char symbolLetter; // A
    double frequency; // number of times letter appears in input string
    double prob {}; // probability
    double fX {}; // F(x) 

    pthread_mutex_t* semaphore1;
    pthread_mutex_t* semaphore2;
    pthread_cond_t* turn;
    int tid;
    int turnSignal;
};

bool compareByFrequency(const Symbol &a, const Symbol &b) { return a.frequency > b.frequency; } // Comparison to be used when sorting by increasing frequency
bool compareByLetter(const Symbol &a, const Symbol &b) { return a.symbolLetter < b.symbolLetter; } // Comparison to be used when sorting by lexicographic order

void BuildSymbolContainer(std::vector<Symbol> &symbolContainer); // Reads input from STDIN, determines the alphabet & frequence of each symbol, and sorts based on frequency and lexicographic order
void CalculateSymbolProb(std::vector<Symbol> &symbolContainer); // Calculates the probability for each symbol in the container
void CalculateSymbolfX(std::vector<Symbol> &symbolContainer); // Calculates the value of f(x) for each symbol in the container
int CalculateTotalEntries(const std::vector<Symbol> &symbolContainer); // Returns the sum of each symbol's frequency

void *SFE(void * void_ptr); // Shannon-Fano-Elias code generator

int main() {
    std::vector<Symbol> symbolContainer; // store each symbol and info
    BuildSymbolContainer(symbolContainer);
    CalculateSymbolProb(symbolContainer);

    const int NTHREADS = symbolContainer.size(); // NTHREADS is determined by the number of symbols
    pthread_t tid [NTHREADS];

    static pthread_mutex_t mutex1;
    pthread_mutex_init(&mutex1, nullptr);
    static pthread_mutex_t mutex2;
    pthread_mutex_init(&mutex2, nullptr);

    static pthread_cond_t turn = PTHREAD_COND_INITIALIZER;

    Symbol currentSymbol;
    currentSymbol.semaphore1 = &mutex1;
    currentSymbol.semaphore2 = &mutex2;
    currentSymbol.turn = &turn;
    currentSymbol.turnSignal = 0;

    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;
    for (int i = 0; i < NTHREADS; i++) {
        pthread_mutex_lock(&mutex1);
        currentSymbol.prob = symbolContainer.at(i).prob;
        currentSymbol.symbolLetter = symbolContainer.at(i).symbolLetter;
        currentSymbol.tid = i;
        if (pthread_create(&tid[i], nullptr, SFE, &currentSymbol) != 0) { // create a thread for each symbol and call the SFE function
            std::cerr << "Cannot create thread" << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < NTHREADS; i++) {
         pthread_join(tid[i], nullptr); // join threads
    }
    return 0;
}

// Shannon-Fano-Elias code generator
void *SFE(void * void_ptr) {
    Symbol* arg = (struct Symbol*)void_ptr; // cast the void pointer to a pointer of type struct symbolInfo
    char currentSymbol = arg->symbolLetter;
    double currentProb = arg->prob;
    double currentFX = arg->fX + currentProb;
    arg->fX = currentFX;
    int tid = arg->tid;

    pthread_mutex_unlock(arg->semaphore1);
    double fBar = (currentFX - currentProb) + currentProb / 2; // calculate Fbar(x)
    std::string fBarInBinary {};
    while (fBar) {
        int bit = fBar * 2;
        fBar = fmod(fBar * 2, 1);
        fBarInBinary.append(std::to_string(bit)); // find Fbar(x) in binary
    }

    int codeLength = ceil(log2(1 / currentProb) + 1); // calculate the code length
    std::string codeWord {};
    for (int x = 0; x < codeLength; x++) {
        codeWord += fBarInBinary[x]; // assemble the code word based on the code length
    }

    pthread_mutex_lock(arg->semaphore2);
    while(arg->turnSignal != tid){
        pthread_cond_wait(arg->turn, arg->semaphore2);
    }

    pthread_mutex_unlock(arg->semaphore2);
    std::cout << "Symbol " << currentSymbol << ", Code: " << codeWord << std::endl;
    pthread_mutex_lock(arg->semaphore2);
    (arg->turnSignal) = (arg->turnSignal) + 1;
    pthread_cond_broadcast(arg->turn);
    pthread_mutex_unlock(arg->semaphore2);
    return nullptr;
}

// Reads input from STDIN, determines the alphabet & frequence of each symbol, and sorts based on frequency and lexicographic order
void BuildSymbolContainer(std::vector<Symbol> &symbolContainer) {
    std::string message {};
    std::getline(std::cin, message);
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
}

// Calculates the probability for each symbol in the container
void CalculateSymbolProb(std::vector<Symbol> &symbolContainer) {
    int totalEntries = CalculateTotalEntries(symbolContainer);
    for (size_t i = 0; i < symbolContainer.size(); i++) {
        symbolContainer.at(i).prob = static_cast<double>(symbolContainer.at(i).frequency) / totalEntries;
    }
}

// Returns the sum of each symbol's frequency
int CalculateTotalEntries(const std::vector<Symbol> &symbolContainer) {
    int totalEntries {};
    for (size_t i = 0; i < symbolContainer.size(); i++) {
        totalEntries += symbolContainer.at(i).frequency;
    }
    return totalEntries;
}