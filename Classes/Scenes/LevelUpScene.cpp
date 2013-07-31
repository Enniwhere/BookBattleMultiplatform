//
//  LevelUpScene.cpp
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/9/13.
//
//

#include "LevelUpScene.h"

#include "FileIO.h"
#include <sstream>



using namespace cocos2d;

CCScene* LevelUpScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LevelUpScene *layer = LevelUpScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool LevelUpScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    std::ostringstream oss;
    oss << statPoints;
    CCLabelTTF* pLabel = CCLabelTTF::create("Level UP!", "Thonburi", 100);
    //CCLabelTTF* pLabel2 = CCLabelTTF::create("How are you feeling?", "Thonburi", 24);
    std::string labelText = "Stat points: " + oss.str();
    statPointsLabel = CCLabelTTF::create(labelText.c_str(), "Thonburi", 40);
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height-40) );
    statPointsLabel->setPosition( ccp(size.width/2,size.height/2));
    //pLabel2->setPosition( ccp(size.width / 2, size.height - 60) );
    
    // add the label as a child to the scene
    this->addChild(pLabel, 2);
    this->addChild(statPointsLabel);
    this->setTouchEnabled(true);
    CCMenuItemImage *atkStatIcon = CCMenuItemImage::create("atkStatIcon.jpeg",
                                                           "atkStatIcon.jpeg",
                                                          this,
                                                          menu_selector(LevelUpScene::incAtkStat) );
    CCMenuItemImage *dmgStatIcon = CCMenuItemImage::create("dmgStatIcon.png",
                                                           "dmgStatIcon.png",
                                                           this,
                                                           menu_selector(LevelUpScene::incDmgStat));
    CCMenuItemImage *defStatIcon = CCMenuItemImage::create("defStatIcon.png",
                                                           "defStatIcon.png",
                                                           this,
                                                           menu_selector(LevelUpScene::incDefStat) );
    CCMenuItemImage *hpStatIcon = CCMenuItemImage::create("hpStatIcon.png",
                                                          "hpStatIcon.png",
                                                           this,
                                                           menu_selector(LevelUpScene::incHpStat) );
    CCMenu* statMenu = CCMenu::create(atkStatIcon,dmgStatIcon,defStatIcon,hpStatIcon,NULL);
    statMenu->alignItemsHorizontallyWithPadding(40.0f);
    statMenu->setPosition(size.width/2, 100);
    this->addChild(statMenu, 2);

    return true;
}

void LevelUpScene::incAtkStat()
{
    Player::getPlayer()->incAtkStat();
    updateStatPoints();
}

void LevelUpScene::incDmgStat()
{
    Player::getPlayer()->incDmgStat();
    updateStatPoints();
}

void LevelUpScene::incDefStat()
{
    Player::getPlayer()->incDefStat();
    updateStatPoints();
}

void LevelUpScene::incHpStat()
{
    Player::getPlayer()->incHpStat();
    updateStatPoints();
}

void LevelUpScene::updateStatPoints()
{
    statPoints--;
    std::ostringstream oss;
    oss << statPoints;
    std::string labelText = "Stat points: " + oss.str();
    statPointsLabel->setString(labelText.c_str());

    if (statPoints < 1)
    {
        endLevelUp();
    }
}
void LevelUpScene::endLevelUp()
{
    FileIO::savePlayer("player1.xml");
    CCDirector::sharedDirector()->popToRootScene();
}