//
//  Enemy.h
//  BookBattle
//
//  Created by Simon Enni on 6/18/13.
//
//

#ifndef __BookBattle__Enemy__
#define __BookBattle__Enemy__

#include <iostream>
#include "Attack.h"


class Enemy : public Model
{
private:
    
    Model * player = NULL;
    std::map<int, Attack> attackMap;
    int xpReward = 1100;
    
public:
    Enemy(int a, int b);
    ~Enemy(){}
    void initAttacks();
    void initWithPlayer(Model * player);
    
    void takeTurn();
    void endTurn();
    void rewardPlayer();

    void attack();

    void die();
    
    static void enemyAttack001Init(Enemy * attacker,int atkNumber);
    static void enemyAttack002Init(Enemy * attacker,int atkNumber);
    
};

#endif /* defined(__BookBattle__Enemy__) */
