
#ifndef __SUPPORT_FILEIO_H__
#define __SUPPORT_FILEIO_H__

#include "platform/CCPlatformMacros.h"
#include <string>
#include "Player.h"

NS_CC_BEGIN

class CC_DLL FileIO
{
private:
    FileIO();
    static bool createXMLFile();
    static void initXMLFilePath();

    static std::string m_sFilePath;
    static bool m_sbIsFilePathInitialized;
    
public:
    ~FileIO();
    
    const static std::string& getXMLFilePath();
    static bool isXMLFileExist();
    
    static void savePlayer(std::string filename);
    static bool loadPlayer(std::string filename);
    static bool loadPredefined(std::string filepath);
};


NS_CC_END

#endif // __SUPPORT_FILEIO_H__
