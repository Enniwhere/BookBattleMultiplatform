/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "FileIO.h"
#import <string>
#import "tinyxml2.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

#define KEEP_COMPATABILITY

using namespace std;
using namespace tinyxml2;

NS_CC_BEGIN

/**
 * implements of FileIO
 */

string FileIO::m_sFilePath = string("");
bool FileIO::m_sbIsFilePathInitialized = false;


FileIO::~FileIO()
{
}

FileIO::FileIO()
{
}

bool FileIO::isXMLFileExist()
{
    FILE *fp = fopen(m_sFilePath.c_str(), "r");
	bool bRet = false;
    
	if (fp)
	{
		bRet = true;
		fclose(fp);
	}
    
	return bRet;
}

void FileIO::initXMLFilePath()
{
#ifdef KEEP_COMPATABILITY
    if (! m_sbIsFilePathInitialized)
    {
        // xml file is stored in cache directory before 2.1.2
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        m_sFilePath = [documentsDirectory UTF8String];
        m_sFilePath.append("/");
        
        //m_sFilePath +=  XML_FILE_NAME;
        m_sbIsFilePathInitialized = true;
    }
#endif
}

// create new xml file
bool FileIO::createXMLFile()
{
    return false;
}

const string& FileIO::getXMLFilePath()
{
    return m_sFilePath;
}

void FileIO::savePlayer(std::string filename)
{
    if (! m_sbIsFilePathInitialized)
    {
        initXMLFilePath();
    }
    /*
     #if defined( _MSC_VER ) && defined( DEBUG )
     _CrtMemCheckpoint( &startMemState );
     #endif
     
     #if defined(_MSC_VER) || defined(MINGW32) || defined(__MINGW32__)
     #if defined __MINGW64_VERSION_MAJOR && defined __MINGW64_VERSION_MINOR
     //MINGW64: both 32 and 64-bit
     mkdir( "data/saved_players/" );
     #else
     _mkdir( "data/saved_players/" );
     #endif
     #else
     mkdir( "data/saved_players/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
     #endif
     */
    Player * player = Player::getPlayer();
    XMLDocument* doc = new XMLDocument();
    XMLNode* element = doc->InsertEndChild( doc->NewElement( "player" ) );
    
    XMLElement* sub[10] = { doc->NewElement( "name" ), doc->NewElement( "xp" ), doc->NewElement( "level" ), doc->NewElement( "atk_stat" ), doc->NewElement( "dmg_stat" ), doc->NewElement( "def_stat" ), doc->NewElement( "hp_stat" ), doc->NewElement( "hp" ), doc->NewElement( "base_dmg" ), doc->NewElement( "variant_dmg" ) };
    
    sub[0]->SetAttribute( "attrib", player->getName().c_str() );
    sub[1]->SetAttribute( "attrib", player->getXp() );
    sub[2]->SetAttribute( "attrib", player->getLevel() );
    sub[3]->SetAttribute( "attrib", player->getAtkStat() );
    sub[4]->SetAttribute( "attrib", player->getDmgStat() );
    sub[5]->SetAttribute( "attrib", player->getDefStat() );
    sub[6]->SetAttribute( "attrib", player->getHpStat() );
    sub[7]->SetAttribute( "attrib", player->getHp() );
    sub[8]->SetAttribute( "attrib", player->getBaseDmg() );
    sub[9]->SetAttribute( "attrib", player->getVariantDmg() );
    
    
    element->InsertFirstChild( sub[0] );
    for (int i = 0; i < 9; i++)
    {
        element->InsertAfterChild( sub[i], sub[i+1] );
    }
    
    std::string savePath = FileIO::getXMLFilePath() + filename;
    XMLError error = doc->SaveFile(savePath.c_str());
    if(XML_NO_ERROR == error)
    {
        printf("player saved");
    }
    else
    {
        printf("player not saved");
        doc->PrintError();
    }
}

bool FileIO::loadPlayer(std::string filename)
{
    if (! m_sbIsFilePathInitialized)
    {
        initXMLFilePath();
    }
    XMLDocument doc;
    std::string loadPath = FileIO::getXMLFilePath() + filename;
    Player* player = Player::getPlayer();
    if(XML_NO_ERROR == doc.LoadFile( loadPath.c_str() ))
    {
        int atkStatLoad = 0;
        int defStatLoad = 0;
        
        XMLElement* atkStatElement = doc.FirstChildElement( "player" )->FirstChildElement( "atk_stat" );
        atkStatElement->QueryIntAttribute( "attrib", &atkStatLoad );
        
        XMLElement* defStatElement = doc.FirstChildElement( "player" )->FirstChildElement( "def_stat" );
        defStatElement->QueryIntAttribute( "attrib", &defStatLoad );
        
        player->setAtkStat(atkStatLoad);
        player->setDefStat(defStatLoad);
        printf("player loaded");
        return true;
    }
    printf("player not loaded");
    doc.PrintError();
    return false;
}

bool FileIO::loadPredefined(std::string filepath)
{
    XMLDocument doc;
    Player* player = Player::getPlayer();
    if(XML_NO_ERROR == doc.LoadFile( filepath.c_str() ))
    {
        player = new Player(player->getX(),player->getY());
        int atkStatLoad = 0;
        int defStatLoad = 0;
        
        XMLElement* atkStatElement = doc.FirstChildElement( "player" )->FirstChildElement( "atk_stat" );
        atkStatElement->QueryIntAttribute( "attrib", &atkStatLoad );
        
        XMLElement* defStatElement = doc.FirstChildElement( "player" )->FirstChildElement( "def_stat" );
        defStatElement->QueryIntAttribute( "attrib", &defStatLoad );
        
        player->setAtkStat(atkStatLoad);
        player->setDefStat(defStatLoad);
        printf("player loaded");
        return true;
    }
    printf("player not loaded");
    doc.PrintError();
    return false;

}


NS_CC_END

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
