//
//  MainScene.h
//  BookBattle
//
//  Created by Simon Enni on 6/12/13.
//
//

#ifndef BookBattle_MainScene_h
#define BookBattle_MainScene_h





#include "cocos2d.h"
using namespace cocos2d;

class MainScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    CCMenu * saveMenu = NULL;
    CCMenu * getSaveMenu(){return saveMenu;}
    CCMenu * loadMenu = NULL;
    CCMenu * getLoadMenu(){return loadMenu;}
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void startCombatCallback(CCObject* pSender);
    
    void openSaveMenu(CCObject* pSender);
    void saveCharCallback(CCObject* pSender);
    void closeSaveMenu(CCObject* pSender);
    
    void openLoadMenu(CCObject* pSender);
    void loadCharCallback(CCObject * pSender);
    void closeLoadMenu(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainScene);
};


#endif   /// Main Scene header