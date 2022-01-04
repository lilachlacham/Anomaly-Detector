
#include "Server.h"

Server::Server(int port)throw (const char*) {
    this->stopped = false;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw ("socket failed.");
    }

    this->serverStruct.sin_family = AF_INET;
    this->serverStruct.sin_addr.s_addr =INADDR_ANY;
    this->serverStruct.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverStruct, sizeof(serverStruct)) < 0) {
        throw ("socket bind failed.");
    }
    if (listen(serverSocket, 3) < 0){
        throw ("socket listen failed");
    }
}

void sigHandler(int sigNum){
    cout<<"sidH"<<endl;
}

void Server::start(ClientHandler& ch)throw(const char*) {
    this->t = new thread([&ch, this](){
        signal(SIGALRM,sigHandler);
            while(!this->stopped) {
                alarm(1);
                socklen_t sizeClient = sizeof(this->clientStruct);
                int clientId = accept(this->serverSocket, (struct sockaddr*)&this->clientStruct,&sizeClient);
                if (clientId > 0) {
                    ch.handle(clientId);
                    close(clientId);
                }
                alarm(0);
            }
        close(this->serverSocket);
    });
}

void Server::stop(){
    this->stopped = true;
	this->t->join(); // do not delete this!
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
    send(this->clientId,buffer,strlen(buffer), 0);
}

/*void socketIO::write(float f) {
    string str = to_string(f);
    write(str);
}*/

void socketIO::write(float f){
    ostringstream ss;
    ss <<f;
    string s(ss.str());
    write(s);
}

void socketIO:: read(float* f) {
    recv(this->clientId, f, sizeof(float), 0);
}

void AnomalyDetectionHandler::handle(int clientID) {
    socketIO sio(clientID);
    CLI cli(&sio);
    cli.start();
}