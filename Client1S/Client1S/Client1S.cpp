// Client1S.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include "winsock2.h"
#include <string>
#include <fstream>
#include <chrono>

#pragma warning(disable: 4996)

SOCKET Connection;

std::ofstream logFile("D:\\Client1S.txt", std::ios::app);
std::ofstream simpleFile("D:\\SimpleClient1S.txt", std::ios::app);

void ClientHandler() {
    int msg_size;
    std::string cstr;
    while (true) {
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, NULL);
        std::cout << msg << std::endl;

        cstr = msg;
        bool found = cstr.find("not") != std::string::npos;
        if (found) {
            
        }
        else {
            if (simpleFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);

                simpleFile << "Simple Number Received DateTime: " << std::ctime(&end_time) << msg << std::endl;
            }
        }

        if (logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(now);

            logFile << "Received DateTime: " << std::ctime(&end_time) << msg << std::endl;
        }
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

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr))) {
        std::cout << "Error: dailed to connect to the server." << std::endl;
        return 1;
    }
    std::cout << "Connected!" << std::endl;
    
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

    std::string msgl;
    while (true) {
        std::getline(std::cin, msgl);
        if (msgl == "Random")
        {
            for (int i = 0; i < 50; i++) {
                int i_msgr = rand();
                std::string s_msgr = std::to_string(i_msgr);
                int msg_size = s_msgr.size();
                std::cout << s_msgr << std::endl;

                if (logFile.is_open()) {
                    auto now = std::chrono::system_clock::now();
                    std::time_t end_time = std::chrono::system_clock::to_time_t(now);

                    logFile << "Sent DateTime: " << std::ctime(&end_time) << s_msgr << std::endl;
                }

                send(Connection, (char*)&msg_size, sizeof(int), NULL);
                send(Connection, s_msgr.c_str(), msg_size, NULL);
                Sleep(10);
            }
            continue;
        }
        int msg_size = msgl.size();

        if (logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(now);

            logFile << "Sent DateTime: " << std::ctime(&end_time) << msgl << std::endl;
        }

        send(Connection, (char*)&msg_size, sizeof(int), NULL);
        send(Connection, msgl.c_str(), msg_size, NULL);
        Sleep(10);
    }

    system("pause");
    return 0;
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
