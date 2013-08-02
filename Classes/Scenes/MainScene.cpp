//
//  MainScene.cpp
//  BookBattle
//
//  Created by Simon Enni on 6/12/13.
//
//

#include "MainScene.h"
#include "CombatScene.h"
#include "FileIO.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainScene *layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    
    CCMenuItemLabel * saveLabels[3];
    for (int i = 0; i < 3; i++) {
        std::stringstream ss;//create a stringstream
        ss << "Saved char " << i+1;
        
        saveLabels[i] = CCMenuItemLabel::create(CCLabelTTF::create(ss.str().c_str(), "Thonburi", 30), this, menu_selector(MainScene::saveCharCallback));
        saveLabels[i]->setTag(i+1);
    }
    CCMenuItemImage * closeSaveIcon = CCMenuItemImage::create("closeIcon.gif", "closeIcon.gif",this,menu_selector(MainScene::closeSaveMenu));
    saveMenu = CCMenu::create(saveLabels[0],saveLabels[1],saveLabels[2],closeSaveIcon);
    saveMenu->alignItemsHorizontallyWithPadding(30.0f);
    this->addChild(saveMenu,4);
    saveMenu->setVisible(false);

    CCMenuItemLabel * loadLabels[3];
    for (int i = 0; i < 3; i++) {
        std::stringstream ss;//create a stringstream
        ss << "Saved char " << i+1;
        
        loadLabels[i] = CCMenuItemLabel::create(CCLabelTTF::create(ss.str().c_str(), "Thonburi", 30), this, menu_selector(MainScene::loadCharCallback));
        loadLabels[i]->setTag(i+1);
    }
    CCMenuItemImage * closeLoadIcon = CCMenuItemImage::create("closeIcon.gif", "closeIcon.gif",this,menu_selector(MainScene::closeLoadMenu));
    loadMenu = CCMenu::create(loadLabels[0],loadLabels[1],loadLabels[2],closeLoadIcon);
    loadMenu->alignItemsHorizontallyWithPadding(30.0f);
    this->addChild(loadMenu,4);
    loadMenu->setVisible(false);
    
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(MainScene::menuCloseCallback) );
    pCloseItem->setPosition( Point(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
    CCMenuItemImage *pLoadItem = CCMenuItemImage::create(
                                                          "loadIcon.png",
                                                          "loadIcon.png",
                                                          this,
                                                          menu_selector(MainScene::openLoadMenu) );
    pLoadItem->setPosition( Point(CCDirector::sharedDirector()->getWinSize().width - 80, 20) );
    
    CCMenuItemImage *pSaveItem = CCMenuItemImage::create(
                                                          "saveIcon.jpg",
                                                          "saveIcon.jpg",
                                                          this,
                                                          menu_selector(MainScene::openSaveMenu) );
    pSaveItem->setPosition( Point(CCDirector::sharedDirector()->getWinSize().width - 140, 20) );
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem,pLoadItem,pSaveItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    
    CCMenuItemImage *combatIcon = CCMenuItemImage::create("PortalBrushed.png",
                                                          "PortalBrushed2.png",
                                                          this,
                                                          menu_selector(MainScene::startCombatCallback) );
    combatIcon->setScale(0.4f);
    CCMenu* combatMenu = CCMenu::create(combatIcon,NULL);
    combatMenu->setPosition(300, 550);
    this->addChild(combatMenu, 2);
    
    
    // Check the stats of the player
    Player * player = Player::getPlayer();
    
    std::stringstream ss;//create a stringstream
    ss << "Attack stat: " << player->getAtkStat() << "/n"
        << "Defence stat: " << player->getDefStat();//add number to the stream
    
    CCLabelTTF * statLabel = CCLabelTTF::create(ss.str().c_str(),"Thonburi",24);
    statLabel->setPosition(Point(300, 300));
    //this->addChild(statLabel,3);
    
    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Main Menu", "Thonburi", 34);
    //CCLabelTTF* pLabel2 = CCLabelTTF::create("How are you feeling?", "Thonburi", 24);
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // position the label on the center of the screen
    pLabel->setPosition( Point(size.width / 2, size.height - 20) );
    //pLabel2->setPosition( Point(size.width / 2, size.height - 60) );
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    //this->addChild(pLabel2, 10);
    
    // add background
    CCSprite* background = CCSprite::create("forest-scene3.jpg");
    
    // position the sprite on the center of the screen
    background->setPosition( Point(size.width/2, size.height/2) );
    background->setScale(1.3f);
    
    // add the sprite as a child to this layer
    this->addChild(background, 0);
    
    //Player* player = new Player(0, 60);
    //player->turnLeft();
    //player->addToScene(this);
    
    return true;
}

void MainScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainScene::startCombatCallback(CCObject* pSender)
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    // create a scene. it's an autorelease object
    CCScene *pScene = CombatScene::scene();
    
    // run
    pDirector->pushScene(pScene);
    
}

void MainScene::openSaveMenu(CCObject *pSender)
{
    saveMenu->setVisible(true);
}

void MainScene::saveCharCallback(CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    std::stringstream ss;//create a stringstream
    ss << "player" << tag << ".xml";
    FileIO::savePlayer(ss.str().c_str());
}

void MainScene::closeSaveMenu(cocos2d::CCObject *pSender)
{
    saveMenu->setVisible(false);
}

void MainScene::openLoadMenu(cocos2d::CCObject *pSender)
{
    loadMenu->setVisible(true);
}

void MainScene::loadCharCallback(cocos2d::CCObject *pSender)
{
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int tag = (int)pMenuItem->getTag();
    std::stringstream ss;//create a stringstream
    ss << "player" << tag << ".xml";
    FileIO::loadPlayer(ss.str().c_str());
}

void MainScene::closeLoadMenu(cocos2d::CCObject *pSender)
{
    loadMenu->setVisible(false);
}

