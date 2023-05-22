#include "Socket.h"

Socket::Socket(int fd) : fd_socket(fd) {}

void Socket::getMsg()
{
    msg.clear();
    recv(fd_socket, buffer, SIZE, 0);
    msg += buffer;
    ptr = 0;
}

void Socket::getString(std::string & str)
{
    while (true)
    {
        if (msg[ptr] == '\n')
        {
            ptr++;
            break;
        }
        str.push_back(msg[ptr]);
        ptr++;
    }
}

void Socket::sendMsg(std::string & str)
{
    strncpy (buffer, &str[0], str.size());
    send(fd_socket, buffer, SIZE, 0);
    str.clear();
}

int Socket::getInstruction()
{
    std::string instruction;
    getString(instruction);
    return atoi(&instruction[0]);
}