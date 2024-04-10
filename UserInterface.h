#pragma once

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>
#include <iostream>

class UserInterface {
public:
    UserInterface();
    ~UserInterface();

    // ����ڷκ��� �Է��� �޴� �Լ�
    std::string getUserInput() const;

    // ȭ�鿡 �޽����� ����ϴ� �Լ�
    void displayMessage(const std::string& message) const;
};

#endif // USERINTERFACE_H
