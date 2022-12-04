// A simple server in the internet domain using TCP
// The port nu1mber is passed as an argument

// Please note this is a C program
// It compiles without warnings with gcc

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void fireman(int) {
    // waitpid is a version of wait that waits for a particular pid value (first param)
    // -1 waits for ANY child process
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        // std::cout << "A child process ended" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    signal(SIGCHLD, fireman);
    if (argc < 2)
    {
        std::cerr << "ERROR, no port provided" << std::endl;
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket" << std::endl;
        exit(1);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR on binding" << std::endl;
        exit(1);
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (true)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
        if (fork() == 0)
        {
            if (newsockfd < 0)
            {
                std::cerr << "ERROR on accept" << std::endl;
                exit(1);
            }

            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);
            if (n < 0)
            {
                std::cerr << "ERROR reading from socket" << std::endl;
                exit(1);
            }
            std::cout << "Here is the message: " << buffer << std::endl;
            n = write(newsockfd, "I got your message", 18);
            if (n < 0)
            {
                std::cerr << "ERROR writing to socket" << std::endl;
                exit(1);
            }
            close(newsockfd);
            _exit(0);
        }
    }

    close(sockfd);
    return 0;
}
