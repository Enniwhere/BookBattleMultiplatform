//
//  Player.h
//  BookBattle
//
//  Created by Simon Enni on 6/12/13.
//
//

#ifndef __BookBattle__Player__
#define __BookBattle__Player__

#include <iostream>
#include <map>
#include "Attack.h"
#include "CCBAnimationManager.h"
#include "SwordSlashLayer.h"


class Player : public Model
{
private:
    SwordSlashLayer * slashLayer = NULL;

public:
    std::map<int, Attack> attackMap;
    Player(int a, int b);
    ~Player(){};
    
    static Player * getPlayer();
    
    void initWithEnemy(Model * enemy);
    void initAttacks();

    void attack();
    void ccTouchesEnded(CCSet* touches, CCEvent* event);

    
    void checkLevelUp();
    int getAttackNumber();
    bool enemyIsHit();
    void takeTurn();
    void endTurn();
    void die();

    void defaultHitAnimation(){slashLayer->slash();}
    
    static void playerAttack001Init(Player * attacker,int atkNumber);
    static void playerAttack002Init(Player * attacker,int atkNumber);
    
    static const std::string& getXMLFilePath();
    static void initXMLFilePath();
    
    static std::string saveFilePath;
    static bool isFilePathInitialized;
};



#endif /* defined(__BookBattle__Player__) */
