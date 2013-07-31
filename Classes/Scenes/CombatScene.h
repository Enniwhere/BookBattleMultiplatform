//
//  CombatScene.h
//  BookBattle
//
//  Created by Simon Enni on 6/20/13.
//
//

#ifndef __BookBattle__CombatScene__
#define __BookBattle__CombatScene__

#include <iostream>

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "SwordSlashLayer.h"
#include "TouchTrailLayer.h"

class CombatScene : public cocos2d::CCLayer
{
private:
    SwordSlashLayer* slashLayer = NULL;
    bool playerTurnFlag = true;
public:
    
    Player* player = NULL;
    Enemy* enemy = NULL;
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CombatScene);
};


#endif /* defined(__BookBattle__CombatScene__) */
