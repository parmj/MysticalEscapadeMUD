#ifndef CommandMinigame_h
#define CommandMinigame_h

#include "Command.h"

using usermanager::OnlineUserManager;
using charactermanager::CharacterManager;

class CommandMinigame : public Command
{
public:
	CommandMinigame(CharacterManager& c, OnlineUserManager& u, WorldManager& w): Command(c, u, w) {}
	
	virtual void executeInHeartbeat(const std::string& username, const std::vector<std::string>& fullCommand);
	virtual std::vector<std::string> reassembleCommand(std::string& fullCommand, bool& commandIsValid);

private:
	std::vector<std::string> reassembleMinigameMove(std::vector<std::string>& splitByComma, bool& commandIsValid);
};


#endif