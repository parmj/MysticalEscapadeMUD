#include <iostream>
#include <unordered_map> 
#include <string>
#include "OnlineUserManager.h"
// #include "../../accountManager/include/User.h"
#include "User.h"

using user::User;
using usermanager::OnlineUserManager;

bool OnlineUserManager::inserUser(std::string id, const User& user){
    return onlineUsers.insert(std::make_pair(id, user)).second;
}

User OnlineUserManager::removeUser(std::string id){
    auto search = onlineUsers.find(id);
    if (search != onlineUsers.end()) {
        onlineUsers.erase(id);
        return search->second;
    }else{
        User user {"",""};
        return user;
    }
}

User OnlineUserManager::getUser(std::string id){
    auto search = onlineUsers.find(id);
    if (search != onlineUsers.end()) {
        return search->second;
    }else{
        User user {"",""};
        return user;
    }
}

void OnlineUserManager::updateUserTimeStamp(std::string id, int timeStamp) {
    auto user = removeUser(id);
    user.setRoomID(timeStamp);
    inserUser(id, user);
}

void OnlineUserManager::printTable() {
    for(auto& p: onlineUsers)
        std::cout << p.first << " => " << p.second.getUserName() << " "
                  << p.second.getRoomID() << "\n";
}

