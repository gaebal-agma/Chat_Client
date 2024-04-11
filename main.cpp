#include "ChatClient.h"
#include "UserInterface.h"
#include <iostream>
#include <string>

int main() {
    UserInterface ui;
    ChatClient client;

    std::string serverIP;
    std::string clientName;
    int serverPort;

    ui.displayMessage("채팅 클라이언트를 시작합니다.");
    ui.displayMessage("서버 IP를 입력하세요: ");
    serverIP = ui.getUserInput();

    ui.displayMessage("서버 포트를 입력하세요: ");
    serverPort = std::stoi(ui.getUserInput());

    ui.displayMessage("사용할 이름을 입력하세요: ");
    clientName = ui.getUserInput();

    if (client.connectToServer(serverIP, serverPort, clientName)) {
        ui.displayMessage("서버에 연결되었습니다. 메시지를 입력하거나 종료하려면 'exit'를 입력하세요.");

        std::thread sendThread([&client, &ui]() {
            std::string message;
            do {
                message = ui.getUserInput();
                if (message != "exit") {
                    client.sendMessage(message);
                }
            } while (message != "exit");
            });

        // 메시지 수신 및 처리는 ChatClient 클래스 내부에서 별도의 스레드에서 이미 수행되고 있음
        // 따라서 여기서는 사용자가 'exit' 입력할 때까지 기다린 후 종료

        if (sendThread.joinable()) {
            sendThread.join();
        }

        client.disconnect();
        ui.displayMessage("서버로부터 연결이 종료되었습니다.");
    }
    else {
        ui.displayMessage("서버에 연결할 수 없습니다. 프로그램을 종료합니다.");
    }

    return 0;
}
