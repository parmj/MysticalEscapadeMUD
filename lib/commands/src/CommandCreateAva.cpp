#include "CommandCreateAva.h"
#include <boost/algorithm/string.hpp>

std::string CommandCreateAva::executePromptReply(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
	//Format: create-avatar <name> NPC
    // name has to be the same as the NPC's name in the room.
    // This command will make additional NPC which is currently in the room
    if(fullCommand.size() == 3) {
        auto username = onlineUserManager.getUsernameFromConnectionID(connectionID);
        auto role = onlineUserManager.getUserRole(username);
        switch(role) {
            case usermanager::OnlineUserManager::USER_CODE::USER_NOT_FOUND: {
                return "Please log in again.\n";
            }
            case usermanager::OnlineUserManager::USER_CODE::USER_NORMAL_USER: {
                //don't let normal user know that this syntax exists
                return "Wrong command syntax. Please enter \"help\" to see the syntax.\n";
            }
            //Creating non-user characters
            case usermanager::OnlineUserManager::USER_CODE::USER_ADMIN: {
                auto location = characterManager.getCharacterLocation(username);
                auto& currentArea = worldManager.getAreaByLocation(location);

                Character characterNPC{fullCommand[1]};
                characterNPC.setShortdesc(fullCommand[1]);
                characterNPC.setLongdesc(fullCommand[1]);
                characterNPC.setDescription(fullCommand[1]);
            
                characterManager.addNPC(characterNPC);
                currentArea.addNPCtoRooms(characterNPC.getName(), location.room);
                
            }
            case usermanager::OnlineUserManager::USER_CODE::INVALID_USERNAME: {} 
            case usermanager::OnlineUserManager::USER_CODE::ACCOUNT_CREATED: {} 
            case usermanager::OnlineUserManager::USER_CODE::USER_UPDATED: {} 
            case usermanager::OnlineUserManager::USER_CODE::USER_DELETED: {} 
            case usermanager::OnlineUserManager::USER_CODE::USER_LOGGED_OUT: {} 
            case usermanager::OnlineUserManager::USER_CODE::USER_LOGGED_IN: {}
            case usermanager::OnlineUserManager::USER_CODE::USER_ALREADY_LOGGED_IN: {}
            case usermanager::OnlineUserManager::USER_CODE::USER_NOT_ONLINE: {} 
            default:
                //swallow, log error state
                return "";
        }
    } else {
        //Format: create-avatar <name>
        auto username = onlineUserManager.getUsernameFromConnectionID(connectionID);
        //kicked for being idle for too long
        if(username == "") {
            return "Please log in again.\n";
        }

        //TODO: split avatar name and username (now only use username)
        auto answer = characterManager.createCharacter(username);
        switch(answer) {
            case charactermanager::CharacterManager::CHARACTER_CODE::CHARACTER_CREATED: {
                auto roomToSpawnUser = worldManager.getRoomToSpawnUser();
                characterManager.spawnCharacter(username, LocationCoordinates{"Mirkwood", roomToSpawnUser});
                return "Avatar created.\nPlease enter \"edit-avatar shortdesc: [value]\" to customize your character.\n";
            }
            default:
            //error
                return "";
        }
    }
}

std::vector<std::string> CommandCreateAva::reassembleCommand(std::string& fullCommand, bool& commandIsValid) {
    std::vector<std::string> processedCommand;

    //Format: create-avatar <name>
    //Format: create-avatar <name> NPC
    boost::trim_if(fullCommand, boost::is_any_of(" \t"));

    //split by " " and compress all long spaces
    boost::split(processedCommand, fullCommand, boost::is_any_of(" \t"), boost::token_compress_on);
    if(processedCommand.size() == 3) {
        if(processedCommand[2] == "npc"){
            std::cout << processedCommand[2] << "\n";
            commandIsValid = true;
        }
    } else {
        commandIsValid = (processedCommand.size() == 2);
    }
   
    return processedCommand;
}
