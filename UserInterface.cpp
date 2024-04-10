#pragma once
#include "UserInterface.h"

UserInterface::UserInterface() {
}

UserInterface::~UserInterface() {
}

std::string UserInterface::getUserInput() const {
    std::string input;
    std::getline(std::cin, input); 
    return input;
}

void UserInterface::displayMessage(const std::string& message) const {
    std::cout << message << std::endl; 
}
