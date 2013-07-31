//
//  SwordSlashLayer.h
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/3/13.
//
//

#ifndef __BookBattleAlpha__SwordSlashLayer__
#define __BookBattleAlpha__SwordSlashLayer__

#include <iostream>

#include "cocos2d.h"

class CCBlade;

class SwordSlashLayer : public cocos2d::CCLayer {
private:
    std::map< cocos2d::CCTouch *, CCBlade * >  _map;
    cocos2d::CCPoint touchPoints[5];
    int currentTouch = 0;
    cocos2d::CCTouch* touch;
private:
    SwordSlashLayer();
    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
public:
    static SwordSlashLayer* create();
    void slash();
    cocos2d::CCPoint* currentSlash();
};


#endif /* defined(__BookBattleAlpha__SwordSlashLayer__) */
