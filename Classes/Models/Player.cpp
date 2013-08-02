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
        Size size = Director::getInstance()->getWinSize();
        s_CurrentPlayer = new Player(size.width*0.20f,size.height*0.15f);
    }
    
    return s_CurrentPlayer;
}

// Constructor for the player. The layer is initialized in Model's constructor.
Player::Player(int x, int y) : Model(x,y)
{
    // Create a default NodeLoaderLibrary. 
    NodeLoaderLibrary* nodeLoaderLibrary;
    nodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    
    // Create a new CCBReader with a default CCNodeLoaderLibrary
    // This can take a lot of parameters to use code connections and more
    CCBReader* ccbReader = new CCBReader(nodeLoaderLibrary);
    
    // Load the main node from the CocosBuilder file
    modelNodes = ccbReader->readNodeGraphFromFile("TestCharBack.ccbi");
    
    // Get the animationmanager so we can animate the thing afterwards
    animationManager = ccbReader->getAnimationManager();
    modelNodes->setPosition( Point(x, y));
    this->addChild(modelNodes,1); // Add the loaded node to the scene (this)
    this->healthStatus->setPosition( Point(x+500,y+100));
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
    std::map<int,std::function<void(Model*,Model*)>>::iterator it = attackMap.find(attackNumber);
    
    if(it != attackMap.end())
    {
        //element found;
        this->setTouchEnabled(false);
        attackMap[attackNumber](this,opponent);
    }
}

void Player::initAttacks()
{
    const std::function<int(Model*)> dmgFunc3 = [] (Model* atk)->int {return atk->getBaseDmg()+1*atk->getDmgStat()+(rand()%2*atk->getVariantDmg()+2*atk->getDmgStat());};
    attackMap[3] = Player::playerAttack001Init(dmgFunc3,0.35f);
    const std::function<int(Model*)> dmgFunc4 = [] (Model* atk)->int {return atk->getBaseDmg()+3*atk->getDmgStat()+(rand()%atk->getVariantDmg()+atk->getDmgStat());};
    attackMap[4] = Player::playerAttack002Init(dmgFunc4,0.35f);
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
        Director *pDirector = Director::getInstance();
        
        // create a scene. it's an autorelease object
        Scene *pScene = LevelUpScene::scene();
        
        pDirector->pushScene(pScene);
    }
    else{
        Director::getInstance()->popToRootScene();
    }
}


void Player::endTurn()
{
    opponent->takeTurn();
}

void Player::ccTouchesEnded(Set* touches, Event* event)
{
    if (enemyIsHit())
    {
        
        this->attack();
    }
}

bool Player::enemyIsHit()
{
    Point* slashPoints = slashLayer->currentSlash();
    
    // If the two end-points of the slash are too close, it was probably a mistake.
    // In any case it was a puny slash not worthy of the ninja.
    if (mathExt::pointDist(&(slashPoints[0]),&(slashPoints[4])) < 30.0f)
    {
        return false;
    }
    
    Point enemyCenter = opponent->getCenter();
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
    
    Point* slashPoints = slashLayer->currentSlash();
    float angle = mathExt::angleFromAtoB( new Point(1,0), mathExt::pointSub(&(slashPoints[4]), &(slashPoints[0])));

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
    Director *pDirector = Director::getInstance();
    
    // create a scene. it's an autorelease object
    Scene *pScene = LosingScene::scene();
    
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

std::function<void(Model*,Model*)> Player::playerAttack001Init(std::function<int(Model*)> dmgFunc, float scaleFactor)
{
    
    auto atkpart4 = [=](Model * atk,Model * def)->void
    {
        // This function moves the player back again
        atk->animationManager->runAnimationsForSequenceNamed("Back 12 Jump");
        FiniteTimeAction* moveBack = MoveTo::create(1.5, Point(atk->getX(),atk->getY()));
        const std::function<void()> callback = [=] ()->void {atk->endTurn();};

        atk->modelNodes->runAction(ScaleBy::create(1.5, 1/scaleFactor));
        // Run the sequence of animations
        atk->modelNodes->runAction(Sequence::create(moveBack,CallFunc::create( callback ), NULL));
    };
    auto atkpart3 = [=](Model * atk,Model * def)->void
    {
        atk->defaultHitAnimation();
        atk->dealDamage(dmgFunc(atk));
        // This function slashes the enemy
        const std::function<void()> callback = [=] ()->void {atkpart4(atk,def);};
        atk->modelNodes->runAction(Sequence::create(MoveBy::create(1.0, Point(10,20)),
                                                    CallFunc::create( callback ), NULL));
        
    };
    
    auto atkpart2 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Strike 12 Jump");
        const std::function<void()> callback = [=] ()->void {atkpart3(atk,def);};
        atk->modelNodes->runAction(Sequence::create(MoveBy::create(0.2, Point(1,2)),
                                                    CallFunc::create( callback ), NULL));
    };
    
    auto atkpart1 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Ready 12 Jump");
        atk->modelNodes->runAction(ScaleTo::create(1.0, scaleFactor));
        const std::function<void()> callback = [=] ()->void {atkpart2(atk,def);};
        atk->modelNodes->runAction(Sequence::create(JumpTo::create(1.0, def->getCenter(),250,1),
                                                    CallFunc::create( callback ), NULL));
    };
    return atkpart1;
}

std::function<void(Model*,Model*)> Player::playerAttack002Init(std::function<int(Model*)> dmgFunc, float scaleFactor)
{
    
    auto atkpart4 = [=](Model * atk,Model * def)->void
    {
        // This function moves the player back again
        FiniteTimeAction* moveBack = MoveTo::create(1.5, Point(atk->getX(),atk->getY()));
        const std::function<void()> callback = [=] ()->void {atk->endTurn();};
        atk->animationManager->runAnimationsForSequenceNamed("Back 10");
        atk->modelNodes->runAction(ScaleBy::create(1.5, 1/scaleFactor));
        // Run the sequence of animations
        atk->modelNodes->runAction(Sequence::create(moveBack,CallFunc::create( callback ), NULL));
    };
    auto atkpart3 = [=](Model * atk,Model * def)->void
    {
        atk->defaultHitAnimation();
        atk->dealDamage(dmgFunc(atk));
        // This function slashes the enemy
        const std::function<void()> callback = [=] ()->void {atkpart4(atk,def);};
        atk->modelNodes->runAction(Sequence::create(MoveBy::create(1.0, Point(10,0)),
                                                      CallFunc::create( callback ), NULL));
        
    };
    auto atkpart2 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Strike 10");
        // This function slashes the enemy
        const std::function<void()> callback = [=] ()->void {atkpart3(atk,def);};
        atk->modelNodes->runAction(Sequence::create(MoveBy::create(0.2, Point(1,0)),
                                                    CallFunc::create( callback ),NULL));
        
    };
    auto atkpart1 = [=](Model * atk,Model * def)->void
    {
        //int * nextPart = new int(2);
        atk->animationManager->runAnimationsForSequenceNamed("Ready 10");
        atk->modelNodes->runAction(ScaleTo::create(1.0, scaleFactor));
        const std::function<void()> callback = [=] ()->void {atkpart2(atk,def);};
        atk->modelNodes->runAction(Sequence::create(MoveTo::create(1.0, def->getCenter()),
                                                    CallFunc::create( callback ),NULL));
        //testAttack->runPart(*nextPart);
    };
    return atkpart1;
}