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

    ui.displayMessage("ä�� Ŭ���̾�Ʈ�� �����մϴ�.");
    ui.displayMessage("���� IP�� �Է��ϼ���: ");
    serverIP = ui.getUserInput();

    ui.displayMessage("���� ��Ʈ�� �Է��ϼ���: ");
    serverPort = std::stoi(ui.getUserInput());

    ui.displayMessage("����� �̸��� �Է��ϼ���: ");
    clientName = ui.getUserInput();

    if (client.connectToServer(serverIP, serverPort, clientName)) {
        ui.displayMessage("������ ����Ǿ����ϴ�. �޽����� �Է��ϰų� �����Ϸ��� 'exit'�� �Է��ϼ���.");

        std::thread sendThread([&client, &ui]() {
            std::string message;
            do {
                message = ui.getUserInput();
                if (message != "exit") {
                    client.sendMessage(message);
                }
            } while (message != "exit");
            });

        // �޽��� ���� �� ó���� ChatClient Ŭ���� ���ο��� ������ �����忡�� �̹� ����ǰ� ����
        // ���� ���⼭�� ����ڰ� 'exit' �Է��� ������ ��ٸ� �� ����

        if (sendThread.joinable()) {
            sendThread.join();
        }

        client.disconnect();
        ui.displayMessage("�����κ��� ������ ����Ǿ����ϴ�.");
    }
    else {
        ui.displayMessage("������ ������ �� �����ϴ�. ���α׷��� �����մϴ�.");
    }

    return 0;
}
