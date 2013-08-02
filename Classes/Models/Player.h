//
//  Player.h
//  BookBattle
//
//  Created by Simon Enni on 6/12/13.
//
//

#ifndef __BookBattle__Player__
#define __BookBattle__Player__


#include "Model.h"
#include "CCBAnimationManager.h"
#include "SwordSlashLayer.h"


class Player : public Model
{
private:
    SwordSlashLayer * slashLayer = NULL;

public:
    std::map<int, std::function<void(Model*,Model*)>> attackMap;
    Player(int a, int b);
    ~Player(){};
    
    static Player * getPlayer();
    
    void initWithEnemy(Model * enemy);
    void initAttacks();

    void attack();
    void ccTouchesEnded(Set* touches, Event* event);

    
    void checkLevelUp();
    int getAttackNumber();
    bool enemyIsHit();
    void takeTurn();
    void endTurn();
    void die();

    void defaultHitAnimation(){slashLayer->slash();}
    
    static std::function<void(Model*,Model*)> playerAttack001Init(std::function<int(Model*)> dmgFunc, float scaleFactor);
    static std::function<void(Model*,Model*)> playerAttack002Init(std::function<int(Model*)> dmgFunc, float scaleFactor);
    
    static const std::string& getXMLFilePath();
    static void initXMLFilePath();
    
    static std::string saveFilePath;
    static bool isFilePathInitialized;
};



#endif /* defined(__BookBattle__Player__) */
