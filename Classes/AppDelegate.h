#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include <fstream>
#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();

    virtual void applicationDidEnterBackground();

    virtual void applicationWillEnterForeground();

private:
#ifndef NDEBUG
    std::ofstream coutFile;
#endif
};

#endif // _APP_DELEGATE_H_

