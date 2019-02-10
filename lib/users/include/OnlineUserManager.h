#ifndef ONLINE_USER_MANAGER_H
#define ONLINE_USER_MANAGER_H

#include <unordered_map> 
#include <string>
#include <vector>
// #include "../../accountManager/include/User.h"
#include "User.h"
#include "../../userdb/include/UserDB.h"

using user::User; 

namespace usermanager{
class OnlineUserManager{
    public:
    enum class USER_CODE : uint8_t {
      INVALID_USERNAME,
      ACCOUNT_CREATED,
      USER_UPDATED,
      USER_DELETED,
      USER_NOT_FOUND, 
      USER_LOGGED_OUT,
      USER_LOGGED_IN,
      USER_ALREADY_LOGGED_IN,
      USER_NOT_ONLINE   
    };
    private:
        std::unordered_map<std::string, User> onlineUsers;
        // UserDB userDB = UserDB{};
        UserDB userDB {};
    public:
        bool insertUser(const std::string &id, const User &user);
        User removeUser(const std::string& id);
        User& getUserById(const std::string& id);
        User& getUserByUsername(const std::string& userName);
        std::string getConnectionID(const std::string& userName);
        bool onlineUserAddCommandToList(const std::string& id, const std::vector<std::string>& command);
        std::vector<std::pair<User, std::vector<std::string>>> getOnlineUserCommandList();
        std::vector<std::pair<std::string, std::string>> getOnlineUserMessageList();
        void addMessage(const std::string& userName, const std::string& message);
        bool updateUserTimeStamp(const std::string& id, const long timeStamp);
        void printTable();


        // ******* Functions that Uses UserDB *******
        USER_CODE login(const std::string& userName, const std::string& pwd, const std::string& id);
        USER_CODE logout(const std::string& id);  
        UserDB::DB_CODE createUser(const std::string& userName, const std::string& pwd);      

};
}

#endif
