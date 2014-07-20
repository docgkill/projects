#include "AppDelegate.h"

#include "SimpleAudioEngine.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    Size screenSize = glview->getFrameSize();
    Size designSize = Size(2048.0f, 1536.0f);
    
    // http://www.cocos2d-x.org/wiki/Detailed_explanation_of_Cocos2d-x_Multi-resolution_adaptation
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);

    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths;
    
    // ipadhd등을 뒤진 다음에 resources아래를 뒤지나?
    if (screenSize.height > 768.0f) {
        searchPaths.push_back("ipadhd");
        director->setContentScaleFactor(1536.0f/designSize.height);
        log("ipadhd");
    } else if (screenSize.height > 320.0f) {
        log("ipad");
        searchPaths.push_back("ipad");
        director->setContentScaleFactor(768.0f/designSize.height);
    } else {
        log("iphone");
        searchPaths.push_back("iphone");
        director->setContentScaleFactor(380.0f/designSize.height);
    }
    
    fileUtils->setSearchPaths(searchPaths);
    
    // 파일이 없어도 에러가 뜨지는 않네..
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fileUtils->fullPathForFilename("background.mp3").c_str());
    
    SimpleAudioEngine::getInstance()->preloadEffect(fileUtils->fullPathForFilename("falling.wav").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(fileUtils->fullPathForFilename("hitBuilding.wav").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(fileUtils->fullPathForFilename("jump.wav").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(fileUtils->fullPathForFilename("crashing.wav").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(fileUtils->fullPathForFilename("start.wav").c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(fileUtils->fullPathForFilename("openUmbrella.wav").c_str());
    
    
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);

    // turn on display FPS
//    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    
    // create a scene. it's an autorelease object
    auto scene = GameLayer::scene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
