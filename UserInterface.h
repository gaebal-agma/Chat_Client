#pragma once

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>
#include <iostream>

class UserInterface {
public:
    UserInterface();
    ~UserInterface();

    // 사용자로부터 입력을 받는 함수
    std::string getUserInput() const;

    // 화면에 메시지를 출력하는 함수
    void displayMessage(const std::string& message) const;
};

#endif // USERINTERFACE_H
