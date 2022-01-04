
#include "Server.h"
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SA struct sockaddr

Server::Server(int port)throw (const char*) {
    this->stopped = false;
    int sock;
    struct sockaddr_in servAddr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw ("socket failed.\n");
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    if ((bind(sock, (SA*)&servAddr, sizeof(servAddr))) != 0) {
        throw ("socket bind failed.\n");
    }
    if (listen(sock, 3) !=0){
        throw ("socket listen failed\n");
    }
    this->serverSocket = sock;
}

void Server::start(ClientHandler& ch)throw(const char*) {
    
}

void Server::stop(){
    this->stopped = true;
	t->join(); // do not delete this!
}

Server::~Server() {}

string socketIO:: read() {
    char c = 0;
    string s = "";
    while (c != '\n') {
        recv(this->clientId, &c, sizeof(char), 0);
        s+=c;
    }
    return s;
}

void socketIO::write(string text) {
    const char* buffer = text.c_str();
    send(this->clientId,buffer,text.length(), 0);
    return;
}

void socketIO::write(float f) {
    string str = to_string(f);
    write(str);
}

void socketIO:: read(float* f) {
    recv(this->clientId, &f, sizeof(float), 0);
}

void AnomalyDetectionHandler::handle(int clientID) {
    socketIO sio = socketIO(clientID);
    CLI cli = CLI(&sio);
    cli.start();
}