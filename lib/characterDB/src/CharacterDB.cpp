#include "CharacterDB.h"

CharacterDB::CharacterDB(){
    characters_json = jsonParser.processJSON(json_filePath);
    // cout<< "USER DB CONS()\n";

    if(characters_json == nullptr){
        cout<< "JSON NOT FOUND\n";
        std::ofstream("charaters_db.json");
    }
}

CharacterDB::~CharacterDB(){
    jsonParser.saveJSON(characters_json, json_filePath);
}

bool CharacterDB::createCharacter(const std::string& name){
    if(characters_json[name] != nullptr){
        return false;
    }
    else{
        characters_json[name]["shortdesc"] = "";
        characters_json[name]["longdesc"] = "";
        characters_json[name]["description"] = "";
        cout << characters_json << "\n";
        jsonParser.saveJSON(characters_json, json_filePath);
        return true;
    }
}

bool CharacterDB::updateCharacter(Character& character){

    auto characterName = character.getName();
    if(characters_json[characterName] == nullptr){

        return false;
    }
    else{
        characters_json[characterName]["shortdesc"] = character.getShortdesc();
        characters_json[characterName]["longdesc"] = character.getLongdesc();
        characters_json[characterName]["description"] = character.getDescription();

        // UPDATE THE Character IN DB HERE
        cout << characters_json << "\n";
        jsonParser.saveJSON(characters_json, json_filePath);

        return true;
    }
}

Character CharacterDB::getCharacter(const std::string& name){

    if(characters_json[name] == nullptr){
        return Character{""};
    }
    else{
        cout<<"Character FOUND\n";
        cout << characters_json ;
        Character character{name};
        character.setLongdesc(characters_json[name]["longdesc"]);
        character.setShortdesc(characters_json[name]["shortdesc"]);
        character.setDescription(characters_json[name]["description"]);

        cout << character.getName() << " \n";
        cout << character.getShortdesc() << " \n";
        cout << character.getLongdesc() << " \n";
        cout << character.getDescription() << " \n";

        return Character{name};
    }
}



