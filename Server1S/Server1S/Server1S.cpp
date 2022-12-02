// Server1S.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include "winsock2.h"
#include <string>
#include <string.h>
#include <fstream>
#include <chrono>

#pragma warning(disable: 4996)

SOCKET Connections[100];
int Counter = 0;
int numCounter = 0;
int simpleCounter = 0;
int min = 65536;
int max = 0;

std::ofstream logFile("D:\\Server1S.txt", std::ios::app);

bool OnlyNumStr(std::string msg);
bool IsSimple(int value);
int ConvertString(char* msg, int size);

void ClientHandler(int index) {
    int msg_size;
    int value = 0;
    bool isSimple = false;
    std::string msgr;
    std::string s_msg;
    int msgr_size;

    while (true) {
        
        recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connections[index], msg, msg_size, NULL);
        std::cout << msg << std::endl;
        s_msg = msg;




        if (s_msg == "Who") {
            msgr = "Panchenko Olexandr. K-25. Var-28. Simple number filter. Port-1053.";
            msgr_size = msgr.size();
            std::cout << msgr << std::endl;
        }
        else if(s_msg == "End") {
            msgr = "Total numbers sent: " + std::to_string(numCounter) + "\n" + "Simple numbers sent: " + std::to_string(simpleCounter) + "\n" + "Min: " + std::to_string(min) + "  Max: " + std::to_string(max);
            msgr_size = msgr.size();
            std::cout << msgr << std::endl;
        }
        else if (OnlyNumStr(s_msg)) {
            value = ConvertString(msg, msg_size);
            isSimple = IsSimple(value);
            msgr = msg;
            if (isSimple) {
                msgr += " Is Simple";
            }
            else {
                msgr += " Is not Simple";
            }
            msgr_size = msgr.size();
            std::cout << msgr << std::endl;
        }
        else {
            msgr = "Incorrect input. Try again.";
            msgr_size = msgr.size();
            std::cout << msgr << std::endl;
        }


        if (logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(now);

            logFile << "Received DateTime: " << std::ctime(&end_time) << msgr << std::endl;
        }

        for (int i = 0; i < Counter; i++) {
            send(Connections[i], (char*)&msgr_size , sizeof(int), NULL);
            send(Connections[i], msgr.c_str(), msgr_size, NULL);
        }
        delete[] msg;
    }
}

int main(int argc, char* argv[])
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error!" << std::endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1053);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

        if (newConnection == 0) {
            std::cout << "Error#2!" << std::endl;
        }
        else {
            std::cout << "Client Connected!" << std::endl;

            Connections[i] = newConnection;
            Counter++;

            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
        }
    }
    

    system("pause");
    return 0;
}


bool OnlyNumStr(std::string msg) {
    for (int i = 0; i < msg.length(); i++) {
        if (msg[i] < 48 || msg[i] > 57) {
            return false;
        }
    }
    return true;
}

bool IsSimple(int value) {
    numCounter++;
    bool result = true;
    if (min > value)
        min = value;
    if (max < value)
        max = value;
    for (int i = 2; i < sqrt(value) + 1; i++)
    {
        if (value % i == 0) {
            result = false;
            return result;
        }
    }
    simpleCounter++;
    return result;
}

int ConvertString(char* message, int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        //std::cout << "Result is: " << result <<  std::endl;
        result += (message[i] - '0') * pow(10, size - 1 - i);
    }
    return result;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
