#pragma once

#include <map>
#include <iostream>
#include <unistd.h>
#include <iterator>
#include <list>
#include "DataBase.h"
#include "../src_shared/Socket.h"

#define PORT 7777

class Server
{
private:
    int fdServerSocket, fdClientSocket;
    struct sockaddr_in  serverAddress, clientAddress;
    socklen_t length;
    bool flag;
    std::string activeUser;
    Socket* sckt;
    DataBase db;
public:
    Server();
    ~Server() = default;
    void listenPort();
    void runInstruction(int instruction);
    void registration();
    void authorization();
    void sendUsersList();
    void sendChatsList();
    void addNewChat();
    void addNewMessage();
    void sendMessagesList();
};
