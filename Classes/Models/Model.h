//
//  Model.h
//  BookBattle
//
//  Created by Simon Enni on 6/12/13.
//
//

#ifndef __BookBattle__Model__
#define __BookBattle__Model__


#include <iostream>
#include <map>
#include "mathExt.h"
#include "cocos2d.h"
#include "CCBAnimationManager.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class Model : public Layer
{
protected :
    
    std::string name = "No name";
    int level = 0;
    int xp = 0;
    int atkStat = 1;
    int dmgStat = 1;
    int defStat = 1;
    int hpStat = 10;
    int hp = 1;
    int baseDmg = 10;
    int variantDmg = 20;
    int x,y = 0;
    
    LabelTTF * healthStatus = LabelTTF::create("Ikke skadet", "Thonburi", 34);
    Model * opponent = NULL;
    
public :
    cocos2d::extension::CCBAnimationManager *animationManager = NULL;
    Node* modelNodes = new Node;
    
    Model(int x, int y);
    ~Model();

    void incAtkStat(){atkStat++;}
    void incDmgStat(){dmgStat++;}
    void incDefStat(){defStat++;}
    void incHpStat(){hpStat++;}
    std::string getName(){return name;}
    int getXp(){return xp;}
    int getLevel(){return level;}
    int getAtkStat(){return atkStat;}
    int getDmgStat(){return dmgStat;}
    int getDefStat(){return defStat;}
    int getHpStat(){return hpStat;}
    int getHp(){return hp;}
    int getBaseDmg(){return baseDmg;}
    int getVariantDmg(){return variantDmg;}
    int getX(){return x;}
    int getY(){return y;}
    void setName(std::string input){name = input;}
    void setXp(int value){xp = value;}
    void setLevel(int value){level = value;}
    void setAtkStat(int stat){atkStat = stat;}
    void setDmgStat(int stat){defStat = stat;}
    void setDefStat(int stat){defStat = stat;}
    void setHpStat(int stat){hpStat = stat;}
    void setHp(int value){hp = value;}
    void setBaseDmg(int value){baseDmg = value;}
    void setVariantDmg(int value){variantDmg = value;}
    virtual void takeTurn(){};
    virtual void endTurn(){};
    virtual void dealDamage(int dmg);
    virtual void takeDamage(int dmg);
    virtual void healDamage(int dmg);
    virtual void gainXp(int xp);
    virtual void die(){}
    cocos2d::Point getCenter();
    virtual void defaultHitAnimation(){}
    virtual void updateHealthStatus();
};



#endif /* defined(__BookBattle__Model__) */
