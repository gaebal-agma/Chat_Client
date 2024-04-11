#include "ChatClient.h"

ChatClient::ChatClient() : connectSocket(INVALID_SOCKET), connected(false) {
    initializeNetwork();
}

ChatClient::~ChatClient() {
    disconnect();
    cleanupNetwork();
}

bool ChatClient::initializeNetwork() {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsInit = WSAStartup(ver, &wsData);
    return wsInit == 0;
}

void ChatClient::cleanupNetwork() {
    WSACleanup();
}
bool ChatClient::connectToServer(const std::string& serverIP, int serverPort, const std::string& clientName) {
    connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "���� ���� ����, Err :" << WSAGetLastError() << std::endl;
        return false;
    }
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(connectSocket, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "������ �������� ���Ͽ����ϴ�. Err :" << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        return false;
    }

    // ���� ���� ���� �� Ŭ���̾�Ʈ �̸� ����
    int sendResult = send(connectSocket, clientName.c_str(), clientName.length(), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Ŭ���̾�Ʈ �̸� ���� ����, Err :" << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        return false;
    }

    connected = true;
    receiveThread = std::thread(&ChatClient::receiveMessage, this);
    return true;
}

void ChatClient::disconnect() {
    if (connected) {
        connected = false;
        closesocket(connectSocket);
        if (receiveThread.joinable()) {
            receiveThread.join();
        }
    }
}

void ChatClient::sendMessage(const std::string& message) {
    if (!message.empty() && connected) {
        int sendResult = send(connectSocket, message.c_str(), message.length(), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "�޼��� ������ ����, Err #" << WSAGetLastError() << std::endl;
        }
    }
}

void ChatClient::receiveMessage() {
    char buf[4096];
    while (connected) {
        ZeroMemory(buf, 4096);
        int bytesReceived = recv(connectSocket, buf, 4096, 0);
        if (bytesReceived > 0) {
            // Echo response to console
            std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
        }
        else if (bytesReceived == 0) {
            std::cout << "Server �� ������ ���������ϴ�." << std::endl;
            break;
        }
        else {
            std::cerr << "Server �κ��� �޼��� ���ſ� �����Ͽ����ϴ�. Err :" << WSAGetLastError() << std::endl;
            break;
        }
    }
}

void ChatClient::run() {
    std::string userInput;
    do {
        std::getline(std::cin, userInput);
        sendMessage(userInput);
    } while (userInput.size() > 0 && connected);
}
