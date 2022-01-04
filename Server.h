/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include "CLI.h"


using namespace std;
class socketIO:public DefaultIO{
    int clientId;
    int clientToHandle;
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

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
