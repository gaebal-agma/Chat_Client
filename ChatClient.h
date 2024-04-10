#pragma once


#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#pragma comment (lib, "Ws2_32.lib")

class ChatClient {
public:
    ChatClient();
    ~ChatClient();

    // 서버에 연결을 시도합니다.
    bool connectToServer(const std::string& serverIP, int serverPort);

    // 서버로부터 연결을 해제합니다.
    void disconnect();

    // 서버에 메시지를 전송합니다.
    void sendMessage(const std::string& message);

    // 클라이언트의 메인 루프를 실행합니다.
    void run();

private:
    SOCKET connectSocket;
    std::atomic<bool> connected;
    std::thread receiveThread;

    // 서버로부터 메시지를 수신하는 메서드
    void receiveMessage();

    // 네트워크 초기화를 위한 메서드
    bool initializeNetwork();
    // 네트워크 자원을 정리하는 메서드
    void cleanupNetwork();
};

#endif // CHATCLIENT_H
