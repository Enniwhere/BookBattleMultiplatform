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

Scene* LevelUpScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
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
    if ( !Layer::init() )
    {
        return false;
    }
    
    std::ostringstream oss;
    oss << statPoints;
    LabelTTF* pLabel = LabelTTF::create("Level UP!", "Thonburi", 100);
    //LabelTTF* pLabel2 = LabelTTF::create("How are you feeling?", "Thonburi", 24);
    std::string labelText = "Stat points: " + oss.str();
    statPointsLabel = LabelTTF::create(labelText.c_str(), "Thonburi", 40);
    
    // ask director the window size
    Size size = Director::getInstance()->getWinSize();
    
    // position the label on the center of the screen
    pLabel->setPosition( Point(size.width / 2, size.height-40) );
    statPointsLabel->setPosition( Point(size.width/2,size.height/2));
    //pLabel2->setPosition( Point(size.width / 2, size.height - 60) );
    
    // add the label as a child to the scene
    this->addChild(pLabel, 2);
    this->addChild(statPointsLabel);
    this->setTouchEnabled(true);
    const ccMenuCallback atkStatCallback = [=] (Object* obj)->void {this->incAtkStat();};
    MenuItemImage *atkStatIcon = MenuItemImage::create("atkStatIcon.jpeg",
                                                       "atkStatIcon.jpeg",
                                                       atkStatCallback);
    const ccMenuCallback dmgStatCallback = [=] (Object* obj)->void {this->incDmgStat();};
    MenuItemImage *dmgStatIcon = MenuItemImage::create("dmgStatIcon.png",
                                                           "dmgStatIcon.png",
                                                           dmgStatCallback);
    const ccMenuCallback defStatCallback = [=] (Object* obj)->void {this->incDefStat();};
    MenuItemImage *defStatIcon = MenuItemImage::create("defStatIcon.png",
                                                           "defStatIcon.png",
                                                           defStatCallback);
    const ccMenuCallback hpStatCallback = [=] (Object* obj)->void {this->incHpStat();};
    MenuItemImage *hpStatIcon = MenuItemImage::create("hpStatIcon.png",
                                                          "hpStatIcon.png",
                                                           hpStatCallback);
    Menu* statMenu = Menu::create(atkStatIcon,dmgStatIcon,defStatIcon,hpStatIcon,NULL);
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
    Director::getInstance()->popToRootScene();
}