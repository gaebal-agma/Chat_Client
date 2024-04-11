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
        std::cerr << "소켓 생성 실패, Err :" << WSAGetLastError() << std::endl;
        return false;
    }
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(connectSocket, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "서버에 연결하지 못하였습니다. Err :" << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        return false;
    }

    // 서버 연결 성공 후 클라이언트 이름 전송
    int sendResult = send(connectSocket, clientName.c_str(), clientName.length(), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "클라이언트 이름 전송 실패, Err :" << WSAGetLastError() << std::endl;
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
            std::cerr << "메세지 보내기 실패, Err #" << WSAGetLastError() << std::endl;
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
            std::cout << "Server 의 연결이 끊어졌습니다." << std::endl;
            break;
        }
        else {
            std::cerr << "Server 로부터 메세지 수신에 실패하였습니다. Err :" << WSAGetLastError() << std::endl;
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
