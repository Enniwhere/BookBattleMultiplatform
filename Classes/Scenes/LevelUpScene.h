//
//  LevelUpScene.h
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/9/13.
//
//

#ifndef __BookBattleAlpha__LevelUpScene__
#define __BookBattleAlpha__LevelUpScene__

#include <iostream>

#include "cocos2d.h"

using namespace cocos2d;

class LevelUpScene : public cocos2d::CCLayer
{
private:
    CCLabelTTF * statPointsLabel;
    int statPoints = 5;
public:
    
    void incAtkStat();
    void incDmgStat();
    void incDefStat();
    void incHpStat();
    void updateStatPoints();
    void endLevelUp();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();

        // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(LevelUpScene);
};


#endif /* defined(__BookBattleAlpha__LevelUpScene__) */
