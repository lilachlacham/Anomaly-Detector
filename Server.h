

#ifndef SERVER_H_
#define SERVER_H_
#include "CLI.h"
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include<signal.h>
#include <sstream>


using namespace std;
class socketIO:public DefaultIO{
    int clientId;

public:
    socketIO(int clientId):DefaultIO(){this->clientId = clientId;}
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID);
};


// implement on Server.cpp
class Server {
    bool stopped;
    int serverSocket;
	thread* t; // the thread to run the start() method in
    sockaddr_in serverStruct;
    sockaddr_in clientStruct;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
    void connectToClient(ClientHandler& ch, Server server);
    bool getStopped();
    sockaddr_in getStructClient();
    int getServerSocket();
};

#endif /* SERVER_H_ */
