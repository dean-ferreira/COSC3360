#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <algorithm>
#include <vector>

struct Symbol {
    char symbolLetter {}; // A
    int frequency {}; // the number of occurences from the input message
    double prob {}; // probability = frequency / total number of entries
    double fX {}; // cumalative probability F(x)
};

bool compareByFrequency(const Symbol &a, const Symbol &b) { return a.frequency > b.frequency; } // Comparison to be used when sorting by increasing frequency
bool compareByLetter(const Symbol &a, const Symbol &b) { return a.symbolLetter < b.symbolLetter; } // Comparison to be used when sorting by lexicographic order
void BuildSymbolContainer(std::vector<Symbol> &symbolContainer); // Reads input from STDIN, determines the alphabet & frequence of each symbol, and sorts based on frequency and lexicographic order
void CalculateSymbolProb(std::vector<Symbol> &symbolContainer); // Calculates the probability for each symbol in the container
void CalculateSymbolfX(std::vector<Symbol> &symbolContainer); // Calculates the value of f(x) for each symbol in the container
int CalculateTotalEntries(const std::vector<Symbol> &symbolContainer); // Returns the sum of each symbol's frequency

int main(int argc, char *argv[])
{
    // 1. Create a socket to communicate with the server program.
    // This code has been referenced from the in-lecture program
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3)
    {
        std::cerr << "usage " << argv[0] << "hostname port\n";
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cerr << "ERROR opening socket";
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        std::cerr << "ERROR, no such host\n";
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR connecting";
        exit(1);
    }

    std::vector<Symbol> symbolContainer; // vector used to store each symbol and info
    BuildSymbolContainer(symbolContainer);
    CalculateSymbolProb(symbolContainer);
    CalculateSymbolfX(symbolContainer);

    double symbolProb = symbolContainer.at(0).prob;
    double symbolFx = symbolContainer.at(0).fX;
    
    n = write(sockfd, &symbolProb, sizeof(double)); // sending the symbol's probability to the server
    if (n < 0)
    {
        std::cerr << "ERROR writing to socket";
        exit(1);
    }
    n = write(sockfd, &symbolFx, sizeof(double)); // sending the symbol's value of f(x) to the server
    if (n < 0)
    {
        std::cerr << "ERROR writing to socket";
        exit(1);
    }
    int size; // used to store the size of the char array
    n = read(sockfd, &size, sizeof(int)); // receiving the size
    if (n < 0)
    {
        std::cerr << "ERROR reading from socket";
        exit(1);
    }
    char *codeWord = new char[size + 1]; // dynamic char array  that will store the received code word
    bzero(codeWord, size + 1);
    n = read(sockfd, codeWord, size); // receiving the char array
    std::cout << codeWord << std::endl;
    delete [] codeWord;
    close(sockfd);
    return 0;
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

// Calculates the value of f(x) for each symbol in the container
void CalculateSymbolfX(std::vector<Symbol> &symbolContainer) {
    double sumProb {};
    for (size_t i = 0; i < symbolContainer.size(); i++) {
        sumProb += symbolContainer.at(i).prob;
        symbolContainer.at(i).fX = sumProb;
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
