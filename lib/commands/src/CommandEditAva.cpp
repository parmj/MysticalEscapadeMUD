#include "CommandEditAva.h"
#include <boost/algorithm/string.hpp>

using internationalization::Internationalization;

std::string CommandEditAva::executePromptReply(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
	//Format: edit-avatar <name-of-NPC> <what-to-edit>: <value>
    if(fullCommand.size() == 4) {
        auto username = onlineUserManager.getUsernameFromConnectionID(connectionID);
        auto role = onlineUserManager.getUserRole(username);
        switch(role) {
            case usermanager::OnlineUserManager::USER_CODE::USER_NOT_FOUND:
                return stringManager.getString(Internationalization::STRING_CODE::PLEASE_LOG_IN_AGAIN);
            case usermanager::OnlineUserManager::USER_CODE::USER_NORMAL_USER:
                //don't let normal user know that this syntax exists
                return (stringManager.getString(Internationalization::STRING_CODE::WRONG_COMMAND_SYNTAX), 
                       " ", 
                       stringManager.getString(Internationalization::STRING_CODE::PLEASE_ENTER_HELP_SYNTAX));
            case usermanager::OnlineUserManager::USER_CODE::USER_ADMIN:
                //TODO: fill this




            
                return "test answer";
            default:
                //swallow, log error state
                return "";
        }
    } else {
        //Format: edit-avatar <what-to-edit>: <value>
        auto username = onlineUserManager.getUsernameFromConnectionID(connectionID);
        //kicked for being idle for too long
        if(username == "") {
            return stringManager.getString(Internationalization::STRING_CODE::PLEASE_LOG_IN_AGAIN);
        }
        std::stringstream answer;
        answer << editUserAvatar(username, fullCommand);
        
        auto avatarCustomizationStatus = characterManager.isCharacterFullyCustomized(username);
        switch(avatarCustomizationStatus) {
            case CharacterManager::CHARACTER_CODE::CHARACTER_CUSTOMIZED_MISSING_SHORT_DESC: {
                answer << stringManager.getString(Internationalization::STRING_CODE::EDIT_AVATAR_SHORTDESC);
                return answer.str();
            }
            case CharacterManager::CHARACTER_CODE::CHARACTER_CUSTOMIZED_MISSING_LONG_DESC: {
                answer << stringManager.getString(Internationalization::STRING_CODE::EDIT_AVATAR_LONGDESC);
                return answer.str();
            }
            case CharacterManager::CHARACTER_CODE::CHARACTER_CUSTOMIZED_MISSING_DESCRIPTION: {
                answer << stringManager.getString(Internationalization::STRING_CODE::EDIT_AVATAR_DESC);
                return answer.str();
            }
            case CharacterManager::CHARACTER_CODE::CHARACTER_IS_CUSTOMIZED: {
                answer << spawnAvatar(username);
                answer << stringManager.getString(Internationalization::STRING_CODE::AVATAR_FULLY_CUSTOMIZED);
                return answer.str();
            }
            default:
                //do nothing
                return "";
        }
    } 
}

std::vector<std::string> CommandEditAva::reassembleCommand(std::string& fullCommand, bool& commandIsValid) {
    std::vector<std::string> processedCommand;
    commandIsValid = false;

    //Format: edit-avatar <what-to-edit>: <value>
    //Format: edit-avatar <name-of-NPC> <what-to-edit>: <value>
    boost::trim_if(fullCommand, boost::is_any_of(" \t"));

    //Split by ":"
    std::vector<std::string> splitByColon;
    boost::split(splitByColon, fullCommand, boost::is_any_of(":"), boost::token_compress_on);
    if(splitByColon.size() != 2) {
        return processedCommand;
    }

    //trim any space before and after ":"
    boost::trim_if(splitByColon[0], boost::is_any_of(" \t"));
    boost::trim_if(splitByColon[1], boost::is_any_of(" \t"));

    //split the left side of ":" by " "
    boost::split(processedCommand, splitByColon[0], boost::is_any_of(" \t"), boost::token_compress_on);
    if(processedCommand.size() != 2 && processedCommand.size() != 3) {
        return processedCommand;
    }

    //reassemble the command
    processedCommand.push_back(splitByColon[1]);
    commandIsValid = true;

    return processedCommand;
}

std::string CommandEditAva::editUserAvatar(const std::string& username, const std::vector<std::string>& fullCommand) {
    //TODO: split avatar name and username (now only use username)
    auto& attribute = fullCommand[1];
    auto& value = fullCommand[2];
    auto answer = characterManager.editCharacter(username, attribute, value);
    switch(answer) {
        case charactermanager::CharacterManager::CHARACTER_CODE::CHARACTER_UPDATED:
            return stringManager.getString(Internationalization::STRING_CODE::AVATAR_UPDATED);
        case charactermanager::CharacterManager::CHARACTER_CODE::CHARACTER_FAILED:
            return stringManager.getString(Internationalization::STRING_CODE::ATTRIBUTE_TO_BE_UPDATED);
        case charactermanager::CharacterManager::CHARACTER_CODE::CHARACTER_NOT_FOUND:
            return stringManager.getString(Internationalization::STRING_CODE::PLEASE_LOG_IN_AGAIN);
        default:
        //error
            return "";
    }
}

std::string CommandEditAva::spawnAvatar(const std::string& username) {
    //spawn avatar when first time set up is finished
    std::stringstream answer;
    answer << "";
    auto isFirstTime = characterManager.isThisFirstTimeSetup(username);
    if(isFirstTime == CharacterManager::CHARACTER_CODE::CHARACTER_THIS_IS_FIRST_TIME_SET_UP) {
        auto spawnLocation = characterManager.spawnCharacter(username);
        worldManager.spawn(username, spawnLocation);      
        answer << 
        (stringManager.getString(Internationalization::STRING_CODE::CURRENT_LOCATION), " ", stringManager.getString(Internationalization::STRING_CODE::AREA), ":") << 
        spawnLocation.area << 
        (", ", stringManager.getString(Internationalization::STRING_CODE::ROOM), ": ") << 
        spawnLocation.room << 
        "\n";
    }
    return answer.str();
}