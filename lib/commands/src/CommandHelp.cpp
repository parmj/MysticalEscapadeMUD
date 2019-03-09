#include "CommandHelp.h"

std::string CommandHelp::executePromptReply(const std::string& connectionID, const std::vector<std::string>& fullCommand) {
    //TODO: check user's role to print out appropriate text
    //test role
    std::string userRole = "admin";

    std::stringstream answer;

    if(fullCommand.size() == 1) {
        answer << printAccountCommands(userRole);
        answer << printAvatarCommands(userRole);
        answer << printCommunicationCommands(userRole);
        answer << printWorldInteractionCommands(userRole);
        answer << printInventoryCommands(userRole);
        answer << printMinigameCommands(userRole);
        answer << printCombatCommands(userRole);
        answer << printUtilityCommands(userRole);
    } else {
        if(fullCommand[1] == "account") {
            answer << printAccountCommands(userRole);
        } else if (fullCommand[1] == "avatar") {
            answer << printAvatarCommands(userRole);
        } else if (fullCommand[1] == "communication") {
            answer << printCommunicationCommands(userRole);
        } else if (fullCommand[1] == "world-interaction") {
            answer << printWorldInteractionCommands(userRole);
        } else if (fullCommand[1] == "inventory") {
            answer << printInventoryCommands(userRole);
        } else if (fullCommand[1] == "minigame") {
            answer << printMinigameCommands(userRole);
        } else if (fullCommand[1] == "combat") {
            answer << printCombatCommands(userRole);
        } else if (fullCommand[1] == "utility") {
            answer << printUtilityCommands(userRole);
        } else {
            answer << "Command type not found. Please enter \"help utility\" to see the supported syntax.\n";
        }
    }
    return answer.str();
}

std::string CommandHelp::printAccountCommands(const std::string& userRole) {
    std::stringstream answer;
    answer << "Account\n";
    answer << "--------------------------------------------------\n";
    answer << "login [username] [password]\n";
    answer << "logout\n";
    answer << "create-account [username] [password]\n";
    answer << "--------------------------------------------------\n\n";
    return answer.str();
}

std::string CommandHelp::printAvatarCommands(const std::string& userRole) {
    //needs enum in User
    std::string admin = "admin";

    std::stringstream answer;
    answer << "Avatar\n";
    answer << "--------------------------------------------------\n";
    answer << "create-avatar [name]\n";
    if(userRole == admin) {
        answer << "create-avatar [name] NPC\n";
    }
    answer << "edit-avatar: [what-to-edit] [value]\n";
    if(userRole == admin) {
        answer << "edit-avatar [name-of-NPC] [what-to-edit]: [value]\n";
    }
    answer << "--------------------------------------------------\n\n";
    return answer.str();
}

std::string CommandHelp::printCommunicationCommands(const std::string& userRole) {
    std::stringstream answer;
    answer << "Communication\n";
    answer << "--------------------------------------------------\n";
    answer << "say: [message]\n";
    answer << "yell: [message]\n";
    answer << "tell [other-username]: [message]\n";
    answer << "--------------------------------------------------\n\n";
    return answer.str();
}

std::string CommandHelp::printWorldInteractionCommands(const std::string& userRole) {
    //needs enum in User
    std::string admin = "admin";

    std::stringstream answer;
    answer << "World Interaction\n";
    answer << "--------------------------------------------------\n";    
    answer << "move [direction]\n";
    answer << "look\n";
    answer << "look [object/username]\n";
    answer << "look [object/username] [object/username-index]\n";
    answer << "examine [object/username]\n";
    if(userRole == admin) {
        answer << "create-room [direction] [name]\n";
        answer << "edit-room [what-to-edit]: [value]\n";
    }
    answer << "--------------------------------------------------\n\n";
    return answer.str();
}

std::string CommandHelp::printInventoryCommands(const std::string& userRole) {
    std::stringstream answer;
    answer << "Inventory\n";
    answer << "--------------------------------------------------\n";    
    answer << "use [object-in-inventory]\n";
    answer << "use [object-in-inventory] [object-index]\n";
    answer << "equip [object-in-inventory]\n";
    answer << "equip [object-in-inventory] [object-index]\n";
    answer << "pickup [object]\n";
    answer << "pickup [object] [object-index]\n";
    answer << "drop [object-in-inventory]\n";
    answer << "drop [object-in-inventory] [object-index]\n";
    answer << "put [container-name]\n";
    answer << "put [container-name] [container-index]  \n";
    answer << "move [direction]\n";
    answer << "move [direction]\n";
    answer << "--------------------------------------------------\n\n"; 
    return answer.str();
}

std::string CommandHelp::printMinigameCommands(const std::string& userRole) {
    std::stringstream answer;
    answer << "Minigame\n";
    answer << "--------------------------------------------------\n";    
    answer << "minigame challenge [username]\n";
    answer << "minigame accept\n";
    answer << "minigame join [username]\n";
    answer << "minigame move [location-of-piece-to-be-moved],[new-location]\n";
    answer << "--------------------------------------------------\n\n"; 
    return answer.str();
}

std::string CommandHelp::printCombatCommands(const std::string& userRole) {
    std::stringstream answer;
    answer << "Combat\n";
    answer << "--------------------------------------------------\n";    
    answer << "combat challenge [username]\n";
    answer << "combat accept\n";
    answer << "combat accept [username]\n";
    answer << "combat join [username]\n";
    answer << "attack\n";
    answer << "flee\n";
    answer << "cast [spell-name]\n";
    answer << "--------------------------------------------------\n\n"; 
    return answer.str();
}

std::string CommandHelp::printUtilityCommands(const std::string& userRole) {
    std::stringstream answer;
    answer << "Utility\n";
    answer << "--------------------------------------------------\n";
    answer << "help Account\n";
    answer << "help Avatar\n";
    answer << "help Communication\n";
    answer << "help World-interaction\n";
    answer << "help Inventory\n";
    answer << "help Minigame\n";
    answer << "help Combat\n";
    answer << "help Utility\n";
    answer << "--------------------------------------------------\n\n";
    return answer.str();
}

std::vector<std::string> CommandHelp::reassembleCommand(std::string& fullCommand, bool& commandIsValid) {
    std::vector<std::string> processedCommand;

    //Format: help
    boost::trim_if(fullCommand, boost::is_any_of(" \t"));

    //split by " " and compress all long spaces
    boost::split(processedCommand, fullCommand, boost::is_any_of(" \t"), boost::token_compress_on);
    commandIsValid = (processedCommand.size() == 1 || processedCommand.size() == 2);
   
    return processedCommand;
}