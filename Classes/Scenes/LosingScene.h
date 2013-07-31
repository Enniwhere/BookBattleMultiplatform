//
//  LosingScene.h
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/6/13.
//
//

#ifndef __BookBattleAlpha__LosingScene__
#define __BookBattleAlpha__LosingScene__

#include <iostream>

#include "cocos2d.h"

using namespace cocos2d;

class LosingScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(LosingScene);
};

#endif /* defined(__BookBattleAlpha__LosingScene__) */
