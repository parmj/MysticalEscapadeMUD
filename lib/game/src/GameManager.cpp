#include "GameManager.h"
#include <boost/algorithm/string.hpp>

GameManager::GameManager() {
    //WorldManager newWorld;
    world = WorldManager{};
    world.generateWorld();
    
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

std::string GameManager::extractCommands(const std::string& connectionID, const std::string& fullCommand) {
    std::vector<std::string> commandParts, splitByColon;
    reassembleCommand(fullCommand, commandParts, splitByColon);

    auto found = tableOfCommands.find(commandParts[0]);
    if(found != tableOfCommands.end()) {
        commandGuideline guideline = found->second;
        if (commandIsValid(commandParts.size(), splitByColon.size(), guideline)) {
            return (this->*guideline.promptReply)(connectionID, commandParts);
        }
        else {
            std::ostringstream answer;
            answer << "Invalid command. Command syntax: " << commandParts[0] << guideline.helpText << "\n";
            return answer.str();
        }
    }
    return "Command not found\n";
}

void GameManager::reassembleCommand(std::string fullCommand, std::vector<std::string>& commandParts, std::vector<std::string>& splitByColon) {
	//Format: <command> <arg> : <optional-text-message> 
    boost::trim_if(fullCommand, boost::is_any_of(" \t"));
    boost::split(splitByColon, fullCommand, boost::is_any_of(":"), boost::token_compress_on);
   
   	//trim any space before and after ":"
    for(auto& text : splitByColon) {
        boost::trim_if(text, boost::is_any_of(" \t"));
    }
    boost::split(commandParts, splitByColon[0], boost::is_any_of(" \t"), boost::token_compress_on);

    //reassemble
    if(splitByColon.size() > 1)
   		commandParts.push_back(splitByColon[1]);
}

bool GameManager::commandIsValid(size_t commandPartsSize, size_t splitByColon, commandGuideline guideline) {
    if(guideline.commandPartArgCount != commandPartsSize - splitByColon)
        return false;
    if(guideline.messagePartArgCount != splitByColon - 1)
        return false;
    return true;
}

std::string GameManager::commandLogin(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
    auto answer = onlineUserManager.login(connectionID, fullCommand[1], fullCommand[2]);
	switch(answer) {
		case usermanager::OnlineUserManager::USER_CODE::USER_LOGGED_IN:
		    //ugly line for testing
		    world.spawn(onlineUserManager.getUserByUsername(fullCommand[1]).getAvatar(), LocationCoordinates{0,0});
			return "You are now logged in.\n";
		case usermanager::OnlineUserManager::USER_CODE::USER_NOT_FOUND:
			return "Error! Username not found. Please try again.\n";
		case usermanager::OnlineUserManager::USER_CODE::USER_ALREADY_LOGGED_IN:
			return "Error! You are already logged in.\n";
        default:
            std::cout << "ERROR SHOULD NOT GET HERE! \n";
        break;
	}
	//swallow
    return "";
}

std::string GameManager::commandLogout(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
    auto answer = onlineUserManager.logout(connectionID);
    auto& avatar = onlineUserManager.getUserById(connectionID).getAvatar();
	switch(answer) {
		case usermanager::OnlineUserManager::USER_CODE::USER_LOGGED_OUT:
		    //ugly line for testing
            world.kick(avatar);
			return "You are now logged out.\n";
		case usermanager::OnlineUserManager::USER_CODE::USER_NOT_ONLINE:
			return "Error! You are not logged in.\n";
		default:
            std::cout << "ERROR SHOULD NOT GET HERE! \n";
        break;
	}
	//swallow
    return "";
}

std::string GameManager::commandCreate(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
    auto answer = onlineUserManager.createUser(fullCommand[1], fullCommand[2]);
	switch(answer) {
		case UserDB::DB_CODE::INVALID_USERNAME:
			return "Error! Invalid username.\n";
		case UserDB::DB_CODE::ACCOUNT_CREATED:
			return "Account created. Please log in to play the game.\n";
		default:
            std::cout << "ERROR SHOULD NOT GET HERE! \n";
        break;
	}
	//swallow
    return "";
}

std::string GameManager::commandAddToActionList(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
    bool success = onlineUserManager.onlineUserAddCommandToList(connectionID, fullCommand);
    if(!success) {
    	return "User is not online.";
    }
    //Do nothing, answer only when executing command 
    return "";
}

std::string GameManager::commandHelp(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
    std::ostringstream answer;
    answer << "--------------------------------------------------\n";
    answer << "Supported commands: \n";
    for (const auto& [command, guideline] : tableOfCommands) {
        answer << command << guideline.helpText << std::endl;
	}   
    return answer.str();
}

std::string GameManager::commandSay(User* user, const std::vector<std::string>& fullCommand) {
    //userNamesInRoom returns null
	// auto& avatar = user->getAvatar();
    // auto& userNamesInRoom = world.getUserNamesInRoom(avatar.getCurrentLocation());
    
    // for(auto name : userNamesInRoom){
    //     userManager.addMessage(name, user->getUserName() + "said: " + fullCommand[1]);
    // }
    std::vector<std::string> nameList = {"test1", "test2", "test3"};

    for(auto name: nameList) {
        onlineUserManager.addMessageToUser(name, user->getUserName() + "said: " + fullCommand[1]);
    }
    onlineUserManager.printTable();

	return "You said: \"" + fullCommand[1] + "\"\n";
}

std::string GameManager::commandYell(User* user, const std::vector<std::string>& fullCommand) {

    auto& avatar = user->getAvatar();
    auto& userNamesInRoom = world.getUserNamesInRange(avatar.getCurrentLocation(), YELL_RANGE);

    //cerr <<"Room List:\n";
    for(auto name : userNamesInRoom){
        //cerr << name << " is in room.\n";
        onlineUserManager.addMessageToUser(name, user->getUserName() + " yelled: " + fullCommand[1] + "\n");
    }

    return "You yelled: \"" + fullCommand[1] + "\"\n";
}

std::string GameManager::commandTell(User* user, const std::vector<std::string>& fullCommand){

    std::string usernameOfListener = fullCommand.at(1);
	std::string message = fullCommand.at(2);
    std::string messageToSendOtherUser = user->getUserName() + " told you: " + message + "\n";

    if(onlineUserManager.addMessageToUser(usernameOfListener, messageToSendOtherUser)) {
        return "You told " + usernameOfListener + ": " + message + "\n";
    }
    return "Failed to find online user with name: " + usernameOfListener + "\n";
}

std::string GameManager::commandMove(User* user, const std::vector<std::string>& fullCommand) {
	auto& avatar = user->getAvatar();
	LocationCoordinates newLocation = world.move(avatar, fullCommand[1]);
	std::ostringstream answer;
	answer << "Current location: Area:" << newLocation.area << ", Room: " << newLocation.room << "\n";
	return answer.str();
}

std::string GameManager::commandLook(User* user, const std::vector<std::string>& fullCommand) {
	auto& avatar = user->getAvatar();
	return world.look(avatar) + "\n";
}

std::string GameManager::commandExamine(User* user, const std::vector<std::string>& fullCommand) {
	auto& avatar = user->getAvatar();
	if(fullCommand[1] == "exits")
	    return world.listExits(avatar) + "\n";
	else
	    return world.look(avatar, fullCommand[1]) + "\n";
}

std::string GameManager::commandError(User* user, const std::vector<std::string>& fullCommand){
    //Intended to be a null-function. Normally it should never reach this.
    return "";
}

std::unique_ptr<std::unordered_map<std::string, std::string>> GameManager::heartbeat() {
    auto map = std::make_unique<std::unordered_map<std::string, std::string>>();

    //process commands
    auto userCommands = onlineUserManager.getOnlineUserCommandList();
    for(auto& element : userCommands) {
    	auto found = tableOfCommands.find((element.second)[0]);
	    commandGuideline guideline = found->second;
    	
    	auto replyMessage = (this->*guideline.heartbeatReply)(&element.first, element.second);
    	auto connectionID = getUserIDByUsername(element.first.getUserName());

    	map->insert(std::make_pair(connectionID, replyMessage));
    }

    //process messages
    auto userMessages = onlineUserManager.getOnlineUserMessageList();
    for (auto& element : userMessages) {
	    auto found = map->find(element.first);
	    if (found != map->end()) {
	      (found->second.append("\n")).append(element.second);
	    }
	    else {
	      map->insert(make_pair(element.first, element.second));
	    }
	  }
    return std::move(map);

    /*TODO:
    	Get list of top commands of all online users
    	Loop through and process each command
    	Gather return messages and put in the table
   	*/

    /* dummyUser implementation for testing
     *
     * std::string userID;
    try {
        userID = std::to_string(dummy.getId());
    }catch(std::out_of_range& e){
        std::cout << e.what();
        return map;
    }

    auto& commandQueue = dummy.getCommands();
    if(!commandQueue.empty()){

        const auto& commandParts = commandQueue.front();
        const auto& commandName = commandParts.at(0);

        auto found = tableOfCommands.find(commandName);
        commandGuideline guideline = found->second;

        //calls the command function
        const auto userMessage = (this->*guideline.heartbeatReply)(&dummy, commandParts);

        map->insert(std::make_pair(userID, userMessage));

        dummy.popCommand();
    }*/
}

//This should just return a User object
std::string GameManager::getUserIDByUsername(const std::string& userName) {
    return onlineUserManager.getConnectionID(userName);
}

user::User GameManager::getUser(const std::string& userName) {
	return onlineUserManager.getUserByUsername(userName);
}

