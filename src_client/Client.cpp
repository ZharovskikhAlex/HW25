#include "Client.h"

Client::Client()
{
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_family = AF_INET;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int connection = connect(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connection == -1)
    {
        std::cout <<  "Сервер недоступен!" << std::endl;
        exit(1);
    }
    user = nullptr;
    sckt = new Socket(serverSocket);
}

void Client::work()
{
    flag = true;
    while (true)
    {
        startMenu();
        if (flag == false)
        {
            break;
        }
        userSession();
    }
    close(serverSocket);
    delete sckt;
}

void Client::startMenu()
{
    std::string input, msg, login, password;
    while (true)
    {
        std::cout << "1 - Регистрация" << std::endl;
        std::cout << "2 - Авторизация" << std::endl;
        std::cout << "3 - Завершить работу" << std::endl;
        std::cin >> input;
        if (input == "3")
        {
            msg += "3\n";
            sckt->sendMsg(msg);
            flag = false;
            return;   
        }
        else if (input == "2")
        {
            msg += "2\n";
        }
        else if (input == "1")
        {
            std::cout << "Не более 20 символов: " << std::endl;
            msg += "1\n";
        }
        else
        {
            continue;
        }
        std::cout << "Логин: ";
        std::cin >> login;
        std::cout << "Пароль: ";
        std::cin >> password;
        if (login.size() > 20 || password.size() > 20)
        {
            std::cout << "Превышен лимит на количество символов логина или пароля" << std::endl;
            msg.clear();
            continue;
        }
        msg += login + '\n' + password + '\n';
        sckt->sendMsg(msg);
        sckt->getMsg();
        int instruction = sckt->getInstruction();
        switch (instruction)
        {
            case 0:
            {
                std::cout << "Пользователь с таким логином уже зарегестрирован" << std::endl;
                break;
            }
            case 1:
            {
                std::cout << "Регистрация прошла успешно" << std::endl;
                break;
            }
            case 2:
            {
                std::cout << "Неверный логин или пароль" << std::endl;
                break;
            }
            case 3:
            {
                std::cout << "Авторизация прошла успешно" << std::endl;
                return;
            }
        }
    }
}

void Client::userSession()
{
    createUser();
    std::string input;
    while (true)
    {
        std::cout << "1 - Добавить новый контакт" << std::endl;
        std::cout << "2 - Написать сообщение" << std::endl;
        std::cout << "3 - Написать всем контактам" << std::endl;
        std::cout << "0 - Завершить сеанс" << std::endl;
        std::cin >> input;
        switch (input[0])
        {
            case '0':
            {
                delete user;
                user = nullptr;
                return;
            }
            case '1':
            {
                addNewChat();
                break;
            }
            case '2':
            {
                showContacts();
                writeMessage();
                break;
            }
            case '3':
            {
                writeAll();
                break;
            }
        }
    }
}

void Client::createUser()
{
    std::string buf;
    sckt->getString(buf);
    user = new User(buf);
    buf.clear();
    //запрос на список пользователей
    std::string q = "4\n";
    sckt->sendMsg(q);
    getUsersList();
    //запросить чаты пользователя
    q = "5\n";
    sckt->sendMsg(q);
    getUserChats();
}

void Client::getUsersList()
{
    while (true)
    {
        sckt->getMsg();
        std::string str;
        sckt->getString(str);
        if (str == "-------end-------")
        {
            break;
        }
        user->u_list.insert(str);
    }
}

void Client::showUsersList()
{
    std::cout << "Список зарегестрированных пользователей:" << std::endl;
    std::set<std::string>::iterator begin = user->u_list.begin();
    std::set<std::string>::iterator end = user->u_list.end();
    for (begin; begin != end; begin++)
    {
        std::cout << *begin << std::endl;
    }
}

void Client::addNewChat()
{
    std::string input;
    showUsersList();
    std::cout << "Ввод: ";
    std::cin >> input;
    if (user->u_list.find(input) == user->u_list.end())
    {
        std::cout << "Пользователь с таким логином не зарегестрирован!" << std::endl;
        return;
    }
    if (user->u_chats.find(input) != user->u_chats.end())
    {
        std::cout << "Пользователь уже есть в списке контактов!" << std::endl;
        return;
    }
    if (input == user->login)
    {
        std::cout << "Введен собственный логин!" << std::endl;
        return;
    }
    std::string package = "6\n";
    Chat newChat(user->login, input);
    package += newChat.user1 + '\n' + newChat.user2 + '\n' + "-------end-------\n";
    sckt->sendMsg(package);
    sckt->getMsg();
    sckt->getString(newChat.c_id);
    Message newMessage(user->login, "Привет!", newChat.c_id);
    package = "7\n";
    package += newMessage.author_ + '\n' + newMessage._message + '\n' + newMessage.c_id + '\n';
    sckt->sendMsg(package);
    newChat.addMessage(newMessage);
    std::pair<std::string, Chat> p = std::make_pair(input, newChat);
    user->u_chats.insert(p);
}

void Client::getUserChats()
{
    while (true)
    {
        std::string c_id, user1, user2;
        sckt->getMsg();
        sckt->getString(c_id);
        if (c_id == "-------end-------")
        {
            break;
        }
        sckt->getString(user1);
        sckt->getString(user2);
        Chat chat(user1, user2);
        chat.c_id = c_id;
        std::string key;
        if (user->login == user1)
        {
            key = user2;
        }
        else
        {
            key = user1;
        }
        std::pair<std::string, Chat> p = std::make_pair(key, chat);
        user->u_chats.insert(p);
    }
}

void Client::showContacts()
{
    std::cout << "Список контактов:" << std::endl;
    std::map<std::string, Chat>::iterator begin = user->u_chats.begin();
    std::map<std::string, Chat>::iterator end = user->u_chats.end();
    for (begin; begin != end; begin++)
    {
        std::cout << begin->first << std::endl;
    }
}

void Client::writeMessage()
{
    std::string input;
    std::cout << "Кому написать: ";
    std::cin >> input;
    std::map<std::string, Chat>::iterator ch = user->u_chats.find(input);
    if (ch == user->u_chats.end())
    {
        std::cout << "Пользователя нет в списке контактов!" << std::endl;
        return;
    }
    showMessChat(ch->second);
    while (true)
    {
        std::cout << "Написать сообщение?" << std::endl;
        std::cout << "1 - Да" << std::endl;
        std::cout << "2 - Нет" << std::endl;
        std::cin >> input;
        if (input != "1")
        {
            return;
        }
        if (inputMessage(input) == false)
        {
            return;
        }
        Message message(user->login, input, ch->second.c_id);
        std::string package = "7\n";
        package += message.author_ + '\n' + message._message + '\n' + message.c_id + '\n';
        sckt->sendMsg(package);
        ch->second.addMessage(message);
    }
}

void Client::showMessChat(Chat & chat)
{
    if (chat.messages.size() == 0)
    {
        std::string package = "8\n";
        package += chat.c_id + '\n';
        sckt->sendMsg(package);
        while (true)
        {
            std::string author, message;
            sckt->getMsg();
            sckt->getString(author);
            if (author == "-------end-------")
            {
                break;
            }
            sckt->getString(message);
            Message m(author, message, chat.c_id);
            chat.messages.push_back(m);
        }
    }
    std::list<Message>::iterator begin = chat.messages.begin();
    std::list<Message>::iterator end = chat.messages.end();
    for (begin; begin != end; begin++)
    {
        std::cout << begin->author_ << ": ";
        std::cout << begin->_message << std::endl;
    }
}

void Client::writeAll()
{
    std::string input;
    if (inputMessage(input) == false)
    {
        return;
    }
    std::map<std::string, Chat>::iterator begin = user->u_chats.begin();
    std::map<std::string, Chat>::iterator end = user->u_chats.end();
    std::string package;
    for (begin; begin != end; begin++)
    {
        Message message(user->login, input, begin->second.c_id);
        package += "7\n";
        package += message.author_ + '\n' + message._message + '\n' + message.c_id + '\n';
        sckt->sendMsg(package);
        begin->second.addMessage(message);
    }
}

bool Client::inputMessage(std::string & mess)
{
    std::string input;
    std::cout << "Сообщение(не более 400 символов): ";
    std::getline(std::cin, input);//очистка потока ввода
    std::getline(std::cin, input);//ввод сообщения
    if (input.size() > 400)
    {
        std::cout << "Недопустимый размер сообщения!" << std::endl;
        return false;
    }
    mess = input;
    return true;
}