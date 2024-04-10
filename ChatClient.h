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

    // ������ ������ �õ��մϴ�.
    bool connectToServer(const std::string& serverIP, int serverPort);

    // �����κ��� ������ �����մϴ�.
    void disconnect();

    // ������ �޽����� �����մϴ�.
    void sendMessage(const std::string& message);

    // Ŭ���̾�Ʈ�� ���� ������ �����մϴ�.
    void run();

private:
    SOCKET connectSocket;
    std::atomic<bool> connected;
    std::thread receiveThread;

    // �����κ��� �޽����� �����ϴ� �޼���
    void receiveMessage();

    // ��Ʈ��ũ �ʱ�ȭ�� ���� �޼���
    bool initializeNetwork();
    // ��Ʈ��ũ �ڿ��� �����ϴ� �޼���
    void cleanupNetwork();
};

#endif // CHATCLIENT_H
