#include "CLI.h"


CLI::CLI(DefaultIO* dio) {
    this->createCommands();
    this->dio = dio;
}

void CLI::start() {
    CurrentData currentData;
    string startOfMenu = "Welcome to the Anomaly Detection Server.\n Please choose an option:\n";
    this->dio->write(startOfMenu);
    int numOfCommands = sizeof(this->commands)/ sizeof(Command);
    for (int i = 0; i < numOfCommands;i++) {
        string commandToPrint = to_string((i+1)) + ". " + this->commands[i]->description + "\n";
    }

    string choose = dio->read();
    if (choose > "6" || choose < "1") {
        exit(1);
    }
    this->commands[stoi(choose)]->execute(currentData);
}

void CLI::createCommands() {


}

//delete all the commands
CLI::~CLI() {

}

