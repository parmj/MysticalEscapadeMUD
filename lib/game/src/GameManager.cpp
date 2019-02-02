#include <unordered_map>
#include <iostream>
#include "GameManager.h"
#include "AccountManager.h"
#include "User.h"
#include "OnlineUserManager.h"
#include <boost/algorithm/string.hpp>

using usermanager::OnlineUserManager;
using accountmanager::AccountManager;

GameManager::GameManager() {
    WorldManager newWorld;
    world = &newWorld;
    world->generateWorld();

    createTableOfCommands();
}

void GameManager::createTableOfCommands() {
    commandGuideline login = {&GameManager::commandLogin, &GameManager::commandError, 2, 0, " [username] [password]"};
    commandGuideline logout = {&GameManager::commandLogout, &GameManager::commandError, 0, 0, ""};
    commandGuideline create = {&GameManager::commandCreate, &GameManager::commandError, 2, 0, " [username] [password]"};
    commandGuideline say = {&GameManager::commandAddToActionList, &GameManager::commandSay, 0, 1, ": [message]"};
    commandGuideline yell = {&GameManager::commandAddToActionList, &GameManager::commandYell, 0, 1, ": [message]"};
    commandGuideline tell = {&GameManager::commandAddToActionList, &GameManager::commandTell, 1, 1, " [other-username]: [message]"};
    commandGuideline move = {&GameManager::commandAddToActionList, &GameManager::commandMove, 1, 0, " [direction]"};
    commandGuideline look = {&GameManager::commandAddToActionList, &GameManager::commandLook, 0, 0, ""};
    commandGuideline examine = {&GameManager::commandAddToActionList, &GameManager::commandExamine, 1, 0, " [object/username]"};
    commandGuideline help = {&GameManager::commandHelp, &GameManager::commandError, 0, 0, ""};
    
    tableOfCommands.insert({"log-in", login});
    tableOfCommands.insert({"log-out", logout});
    tableOfCommands.insert({"create-account", create});
    tableOfCommands.insert({"say", say});
    tableOfCommands.insert({"yell", yell});
    tableOfCommands.insert({"tell", tell});
    tableOfCommands.insert({"move", move});
    tableOfCommands.insert({"look", look});
    tableOfCommands.insert({"examine", examine});
    tableOfCommands.insert({"help", help});
}

//tell a : a;

std::string GameManager::extractCommands(const std::string& connectionID, std::string fullCommand) {
    std::vector<std::string> commandParts, splitByColon;
    reassembleCommand(fullCommand, commandParts, splitByColon);

    auto found = tableOfCommands.find(commandParts[0]);
    if(found != tableOfCommands.end()) {
        commandGuideline guideline = found->second;
        if (commandIsValid(commandParts.size(), splitByColon.size(), guideline))
            return (this->*guideline.promptReply)(connectionID, commandParts);
        else {
            std::ostringstream answer;
            answer << "Invalid command. Command syntax: " << commandParts[0] << guideline.helpText << "\n";
            return answer.str();
        }
    }
    return "Command not found";
}

void GameManager::reassembleCommand(std::string fullCommand, std::vector<std::string>& commandParts, std::vector<std::string>& splitByColon) {
    boost::trim_if(fullCommand, boost::is_any_of(" \t"));
    boost::split(splitByColon, fullCommand, boost::is_any_of(":"), boost::token_compress_on);
   
    for(auto& text : splitByColon) {
        boost::trim_if(text, boost::is_any_of(" \t"));
    }
    boost::split(commandParts, splitByColon[0], boost::is_any_of(" \t"), boost::token_compress_on);

    for(auto& text : splitByColon) {
        commandParts.push_back(text);
    }
}

bool GameManager::commandIsValid(size_t commandPartsSize, size_t splitByColon, commandGuideline guideline) {
    if(guideline.commandPartArgCount != commandPartsSize - 1 - splitByColon)
        return false;
    if(guideline.messagePartArgCount != splitByColon - 1)
        return false;
    return true;
}

void testAccountManager(){
    std::cout << "*** AccountManager TEST ***\n";
    
    AccountManager accountManager{};
    std::string userName = "user9";
    std::string pwd = "pwd";
    std::string id = "123";

    assert( accountManager.createUser(userName, pwd) == AccountManager::ACCOUNT_CODE::ACCOUNT_CREATED);
    // assert( accountManager.login("123","user","test") == AccountManager::ACCOUNT_CODE::INVALID_PASSWORD);
    assert( accountManager.login(id,userName,pwd) == AccountManager::ACCOUNT_CODE::SUCCESFUL_LOGIN);
    assert( accountManager.login(id,userName,pwd) == AccountManager::ACCOUNT_CODE::USER_ALREADY_LOGGED_IN);

}
std::string GameManager::commandLogin(std::string connectionID, std::vector<std::string> fullCommand) {
    return "log-in test";
}

std::string GameManager::commandLogout(std::string connectionID, std::vector<std::string> fullCommand) {
    return "log-out test";
}

std::string GameManager::commandCreate(std::string connectionID, std::vector<std::string> fullCommand) {
    return "create-acc test";
}

std::string GameManager::commandAddToActionList(std::string connectionID, std::vector<std::string> fullCommand) {
    std::string combined;
    for (const auto &commandPart : fullCommand) combined += commandPart;
    return "command-add-test";
}

std::string GameManager::commandHelp(std::string connectionID, std::vector<std::string> fullCommand) {
    std::ostringstream answer;
    answer << "--------------------------------------------------\n";
    answer << "Supported commands: \n";
    for (const auto& [command, guideline] : tableOfCommands) {
        answer << command << guideline.helpText << std::endl;
}   
    return answer.str();
}

void GameManager::commandSay(User* user, std::vector<std::string> fullCommand) {

}

void GameManager::commandYell(User* user, std::vector<std::string> fullCommand) {

}

void GameManager::commandTell(User* user, std::vector<std::string> fullCommand){

}

void GameManager::commandMove(User* user, std::vector<std::string> fullCommand) {

}

void GameManager::commandLook(User* user, std::vector<std::string> fullCommand) {

}

void GameManager::commandExamine(User* user, std::vector<std::string> fullCommand) {

}

void GameManager::commandError(User* user, std::vector<std::string> fullCommand){
    //Intended to be a null-function. Normally it should never reach this.
}

std::unordered_map<std::string, std::string> GameManager::heartbeat() {
    //fill this
    std::unordered_map<std::string, std::string> heartbeatMessagesToUsers;
    return heartbeatMessagesToUsers;
}

//This should just return a User object
User* GameManager::getUser(const std::string userName) const {
    User user{"",""};
    return &user; //Note: nullptr = not online, processed in the upper level
}


/*
    std::vector<std::string> messageParts;
    boost::split(messageParts, fullMessage, boost::is_any_of(" "));

    try {
        auto command = messageParts.at(0);
        std::string result;

        //will be converted into array of functions later once we have finalized the commands
        //since different commands require different ways to deal with
        if(command == "LogIn") {
            auto username = messageParts.at(1);
            auto password = messageParts.at(2);

            //auto answer = userManager.login(username, password);
            //return answer;
            return "test";
        }
        else if(command == "CreateAcc") {
            auto username = messageParts.at(1);
            auto password = messageParts.at(2);

            //auto answer = userManager.createUser(username, password);
            //return answer;

            return "test";
        }
        else if(command == "Move") {
            auto user = getUser(messageParts.at(1));
            auto direction = messageParts.at(2);

            if(user == nullptr) {
                result = "Please log in to play.";
            }
            else {
                result = "Moved to ";
                // result.append(std::to_string(world->move(user, 0)));
            }
        }
        else if(command == "Look") {
            result = world->look(0);
        }else if(command == "test") {
            testOnlineUser();
        }
        return result;

    }
    catch (exception& e) {
        return "Invalid command. Please try again.";
    }*/