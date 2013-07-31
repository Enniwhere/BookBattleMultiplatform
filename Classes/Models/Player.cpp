//
//  Player.cpp
//  BookBattle
//
//  Created by Simon Enni on 6/12/13.
//
//

#include "Player.h"
#include "LosingScene.h"
#include "LevelUpScene.h"
#include "cocos-ext.h"
#include "mathExt.h"
#include <sstream>
#include <array>

#if defined( _MSC_VER )
    #include <direct.h>		// _mkdir
    #include <crtdbg.h>
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    _CrtMemState startMemState;
    _CrtMemState endMemState;
#elif defined(MINGW32) || defined(__MINGW32__)
    #include <io.h>  // mkdir
#else
    #include <sys/stat.h>	// mkdir
#endif


using namespace cocos2d;
using namespace cocos2d::extension;
using namespace tinyxml2;

std::string Player::saveFilePath = std::string("");
bool Player::isFilePathInitialized = false;

// singleton stuff
static Player *s_CurrentPlayer = NULL;

Player* Player::getPlayer(void)
{
    if (!s_CurrentPlayer)
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        s_CurrentPlayer = new Player(size.width*0.20f,size.height*0.15f);
    }
    
    return s_CurrentPlayer;
}

// Constructor for the player. The layer is initialized in Model's constructor.
Player::Player(int x, int y) : Model(x,y)
{
    // Create a default CCNodeLoaderLibrary. 
    CCNodeLoaderLibrary* nodeLoaderLibrary;
    nodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    // Create a new CCBReader with a default CCNodeLoaderLibrary
    // This can take a lot of parameters to use code connections and more
    CCBReader* ccbReader = new CCBReader(nodeLoaderLibrary);
    
    // Load the main node from the CocosBuilder file
    modelNodes = ccbReader->readNodeGraphFromFile("TestCharBack.ccbi");
    
    // Get the animationmanager so we can animate the thing afterwards
    animationManager = ccbReader->getAnimationManager();
    modelNodes->setPosition( ccp(x, y));
    this->addChild(modelNodes,1); // Add the loaded node to the scene (this)
    this->healthStatus->setPosition( ccp(x+500,y+100));
    this->addChild(healthStatus);
    this->setBaseDmg(40);
    
    // As nobody called ccbReader->autoRelease(), returning now would cause
    // a memory leak. We can call autoRelease or delete it ourselves.
    delete ccbReader;
    initAttacks();
    
}

void Player::initWithEnemy(Model *enemy)
{
    this->opponent = enemy;
    slashLayer = SwordSlashLayer::create();
    this->addChild(slashLayer, 0);
}

void Player::attack()
{

    int attackNumber = getAttackNumber();
    std::map<int,Attack>::iterator it = attackMap.find(attackNumber);
    
    if(it != attackMap.end())
    {
        //element found;
        this->setTouchEnabled(false);
        attackMap[attackNumber].attack(this, opponent);
    }
}

void Player::initAttacks()
{
    Player::playerAttack001Init(this,3);
    Player::playerAttack002Init(this,4);
        
}

void Player::takeTurn()
{
    if (hp < 1)
    {
        die();
    }
    else
    {
        this->setTouchEnabled(true);
    }
}

void Player::checkLevelUp()
{
    if (this->xp/1000 > this->level)
    {
        this->level = this->xp/1000;
        // initialize director
        CCDirector *pDirector = CCDirector::sharedDirector();
        
        // create a scene. it's an autorelease object
        CCScene *pScene = LevelUpScene::scene();
        
        pDirector->pushScene(pScene);
    }
    else{
        CCDirector::sharedDirector()->popToRootScene();
    }
}


void Player::endTurn()
{
    opponent->takeTurn();
}

void Player::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    if (enemyIsHit())
    {
        
        this->attack();
    }
}

bool Player::enemyIsHit()
{
    CCPoint* slashPoints = slashLayer->currentSlash();
    
    // If the two end-points of the slash are too close, it was probably a mistake.
    // In any case it was a puny slash not worthy of the ninja.
    if (mathExt::pointDist(&(slashPoints[0]),&(slashPoints[4])) < 30.0f)
    {
        return false;
    }
    
    CCPoint enemyCenter = opponent->getCenter();
    // Check to see if the attack is inside the enemy. The enemy is currently a circle of radius 150.
    // Fuck that circle up.
    for (int i = 0; i < 5; i++)
    {
        if (mathExt::pointDist(&(slashPoints[i]), &enemyCenter) < 150.0f)
        {
            return true;
        }
    }
    return false;
}

// This function gets the number of the attack to run by labeling the angle-intervals
// from 1 to 8 starting from the x-axis and moving counter-clockwise.
// For an example, a slash from top to bottom is number 3 and a slash from top left to bot right
// is number 4.	
int Player::getAttackNumber()
{
    
    CCPoint* slashPoints = slashLayer->currentSlash();
    float angle = mathExt::angleFromAtoB( new CCPoint(1,0), mathExt::pointSub(&(slashPoints[4]), &(slashPoints[0])));

    if (angle <= -180*15/16 || angle >= 180*15/16)
    {
        return 1;
    }
    for (int i = 2; i <= 8; i++)
    {
        if (angle <= -180*(9-i*2)/8)
        {
            return i;
        }
    }
    return 0;
}

void Player::die()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    // create a scene. it's an autorelease object
    CCScene *pScene = LosingScene::scene();
    
    pDirector->pushScene(pScene);
    //delete enemy;
    //delete this;
}


void Player::initXMLFilePath()
{
#ifdef KEEP_COMPATABILITY
    if (! m_sbIsFilePathInitialized)
    {
        // xml file is stored in cache directory before 2.1.2
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        saveFilePath = [documentsDirectory UTF8String];
        saveFilePath.append("/");
        
        //saveFilePath +=  XML_FILE_NAME;
        isFilePathInitialized = true;
    }
#endif
}

const std::string& Player::getXMLFilePath()
{
    return saveFilePath;
}

void Player::playerAttack001Init(Player * attacker,int atkNumber)
{
    attacker->attackMap[atkNumber] = Attack();
    Attack* tempAttack = &attacker->attackMap[atkNumber];
    
    // This function is used to initialize all attacks used by the player.
    // Functions used to add new attacks and so on will probably be added later.
    auto atkpart1 = [=](Model * atk,Model * def)->void
    {
        atk->modelNodes->runAction(CCScaleTo::create(1.0, 0.35));
        atk->modelNodes->runAction(CCSequence::create(CCJumpTo::create(1.0, def->getCenter(),250,1),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
    };
    auto atkpart2 = [=](Model * atk,Model * def)->void
    {
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(0.2, ccp(1,2)),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
    };
    auto atkpart3 = [=](Model * atk,Model * def)->void
    {
        attacker->slashLayer->slash();
        attacker->dealDamage(attacker->getBaseDmg()+2*attacker->getDmgStat()+(rand()%attacker->getVariantDmg()+5*attacker->getDmgStat()));
        // This function slashes the enemy
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(1.0, ccp(10,20)),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
        
    };
    auto atkpart4 = [=](Model * atk,Model * def)->void
    {
        // This function moves the player back again
        CCFiniteTimeAction* moveBack = CCMoveTo::create(1.5, ccp(attacker->getX(),attacker->getY()));
        CCFiniteTimeAction* actionMoveDone =
        CCCallFuncN::create( attacker,
                            callfuncN_selector(Model::endTurn));
        atk->modelNodes->runAction(CCScaleBy::create(1.5, 1/0.35));
        // Reset the attack
        tempAttack->resetAttack();
        // Run the sequence of animations
        atk->modelNodes->runAction(CCSequence::create(moveBack,actionMoveDone));
    };
    tempAttack->attackVector.clear();
    tempAttack->attackVector.push_back(atkpart1);
    tempAttack->attackVector.push_back(atkpart2);
    tempAttack->attackVector.push_back(atkpart3);
    tempAttack->attackVector.push_back(atkpart4);
}

void Player::playerAttack002Init(Player * attacker, int atkNumber)
{
    attacker->attackMap[atkNumber] = Attack();
    Attack* tempAttack = &attacker->attackMap[atkNumber];
    auto atkpart1 = [=](Model * atk,Model * def)->void
    {
        //int * nextPart = new int(2);
        atk->animationManager->runAnimationsForSequenceNamed("Ready Mid Timeline");
        atk->modelNodes->runAction(CCScaleTo::create(1.0, 0.35));
        atk->modelNodes->runAction(CCSequence::create(CCMoveTo::create(1.0, def->getCenter()),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
        //testAttack->runPart(*nextPart);
    };
    auto atkpart2 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Slash Mid Timeline");
        // This function slashes the enemy
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(0.2, ccp(1,0)),
                                                      CCCallFunc::create(tempAttack,
                                                      callfunc_selector(Attack::runNextPart))));
        
    };
    auto atkpart3 = [=](Model * atk,Model * def)->void
    {
        attacker->slashLayer->slash();
        attacker->dealDamage(attacker->baseDmg+3*attacker->dmgStat+(rand()%attacker->variantDmg+1*attacker->dmgStat));
        // This function slashes the enemy
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(1.0, ccp(10,0)),
                                                      CCCallFunc::create(tempAttack,
                                                      callfunc_selector(Attack::runNextPart))));
        
    };
    auto atkpart4 = [=](Model * atk,Model * def)->void
    {
        // This function moves the player back again
        CCFiniteTimeAction* moveBack = CCMoveTo::create(1.5, ccp(atk->getX(),atk->getY()));
        CCFiniteTimeAction* actionMoveDone =
        CCCallFuncN::create( attacker,
                            callfuncN_selector(Player::endTurn));
        atk->animationManager->runAnimationsForSequenceNamed("Back Up Mid Timeline");
        atk->modelNodes->runAction(CCScaleBy::create(1.5, 1/0.35));
        // Reset the attack
        tempAttack->resetAttack();
        // Run the sequence of animations
        atk->modelNodes->runAction(CCSequence::create(moveBack,actionMoveDone));
    };
    tempAttack->attackVector.clear();
    tempAttack->attackVector.push_back(atkpart1);
    tempAttack->attackVector.push_back(atkpart2);
    tempAttack->attackVector.push_back(atkpart3);
    tempAttack->attackVector.push_back(atkpart4);

}