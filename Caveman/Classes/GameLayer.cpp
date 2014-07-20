/*
 soundtrack:
 Johnny Dodds Trio
 Little Bits
 Written by Jimmy Blythe (1901-1931)
 Performed by Johnny Dodds Trio
 Recording date: 1929
 
 openmusicarchive.org
*/

#include "GameLayer.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;

GameLayer::~GameLayer() {
    
//    CC_SAFE_RELEASE(_jamMove);
//    CC_SAFE_RELEASE(_jamAnimate);
}


Scene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() ) {
        return false;
    }
    
    //get screen size
    _screenSize = CCDirector::getInstance()->getWinSize();
    
    createGameScreen();
    
   
    resetGame();
    
    //listen for touches
    
//    this->setTouchEnabled(true);
    
    auto listener = EventListenerTouchOneByOne::create();
    
    // When "swallow touches" is true, then returning 'true' from the onTouchBegan method will "swallow" the touch event,
    // preventing other listeners from using it.
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    //create main loop
    this->schedule(schedule_selector(GameLayer::update));
//    this->scheduleUpdate();
    
     
    
    return true;
}

void GameLayer::resetGame () {
    
    _score = 0;
    _speedIncreaseInterval = 15;
	_speedIncreaseTimer = 0;
    
    char szValue[100] = {0};
    sprintf(szValue, "%i", (int) _score);
    _scoreDisplay->setString (szValue);
    _scoreDisplay->setAnchorPoint(Point(1,0));
    _scoreDisplay->setPosition(Point(_screenSize.width * 0.95f, _screenSize.height * 0.88f));
    
    _state = kGameIntro;
    
    _intro->setVisible(true);
    _mainMenu->setVisible(true);
    
//    _jam->setPosition(Point(_screenSize.width * 0.19f, _screenSize.height * 0.47f));
//    _jam->setVisible(true);
//    _jam->runAction(_jamAnimate);
    if(_isSound==true)  {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }else {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    

    _running = true;
}


void GameLayer::update(float dt) {
    
    if (!_running) return;
    

    if (_player->getPositionY() < -_player->getHeight() ||
        _player->getPositionX() < -_player->getWidth() * 0.5f) {
        
        if (_state == kGamePlay) {
            
            _running = false;

            //create GAME OVER state
            
            _state = kGameOver;
            
            _tryAgain->setVisible(true);
            _scoreDisplay->setAnchorPoint(Point(0.5f, 0.5f));
            _scoreDisplay->setPosition(Point(_screenSize.width * 0.5f,
                                           _screenSize.height * 0.88f));
            _hat->setPosition(Point(_screenSize.width * 0.2f, -_screenSize.height * 0.1f));
            _hat->setVisible(true);
            Action * rotate = CCRotateBy::create(2.0f, 660);
            Action * jump = CCJumpBy::create(2.0f, Point(0,10), _screenSize.height * 0.8f, 1);
            _hat->runAction(rotate);
            _hat->runAction(jump);
   //         SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            if(_isSound==true)  {
                SimpleAudioEngine::getInstance()->playEffect("crashing.wav");
            }
            
        }
    }
  
    _player->update(dt);
  

    _terrain->move(_player->getVector().x);
    
    if (_player->getState() != kPlayerDying) _terrain->checkCollision(_player);
    
    _player->place();
    

    if (_player->getNextPosition().y > _screenSize.height * 0.6f) {
        _gameBatchNode->setPositionY( (_screenSize.height * 0.6f - _player->getNextPosition().y) * 0.8f);
    } else {
        _gameBatchNode->setPositionY  ( 0 );
    }
    
    
    //update paralax
    if (_player->getVector().x > 0) {
        _background->setPositionX(_background->getPosition().x - _player->getVector().x * 0.25f);
//               _background->setPositionX(_background->getPositionX()-2);
        float diffx;
        
        if (_background->getPositionX() < -_background->getContentSize().width) {
            diffx = fabs(_background->getPositionX()) - _background->getContentSize().width;
            _background->setPositionX(-diffx);
        }
        
        _foreground->setPositionX(_foreground->getPosition().x - _player->getVector().x * 4);
        
        if (_foreground->getPositionX() < -_foreground->getContentSize().width * 4) {
            diffx = fabs(_foreground->getPositionX()) - _foreground->getContentSize().width * 4;
            _foreground->setPositionX(-diffx);
        }
        

//        int count = _clouds->count();
//        int count = _clouds.size();
//        Sprite * cloud;
//        for (int i = 0; i < count; i++) {
// //           cloud = (CCSprite *) _clouds->objectAtIndex(i);
//            cloud = (Sprite *) _clouds.at(i);
//            cloud->setPositionX(cloud->getPositionX() - _player->getVector().x * 0.15f);
//            if (cloud->getPositionX() + cloud->boundingBox().size.width * 0.5f < 0 )
//                cloud->setPositionX(_screenSize.width + cloud->boundingBox().size.width * 0.5f);
//        }
    }
    
//    if (_jam->isVisible()) {
//        if (_jam->getPositionX() < -_screenSize.width * 0.2f) {
//            _jam->stopAllActions();
//            _jam->setVisible(false);
//        }
//    }
    
    
    //update score
    if (_terrain->getStartTerrain() && _player->getVector().x > 0) {
        
        _score += dt * 50;
        char szValue[100] = {0};
        sprintf(szValue, "%i", (int) _score);
        _scoreDisplay->setString (szValue);
        
        _speedIncreaseTimer += dt;
        if (_speedIncreaseTimer > _speedIncreaseInterval) {
            _speedIncreaseTimer = 0;
            _player->setMaxSpeed (_player->getMaxSpeed() + 4);
        }
    }
    
    if (_state > kGameTutorial) {
        if (_state == kGameTutorialJump) {
            if (_player->getState() == kPlayerFalling && _player->getVector().y < 0) {
                _player->stopAllActions();
 //               _jam->setVisible(false);
  //              _jam->stopAllActions();
                _running = false;
                _tutorialLabel->setString("While in the air, tap the screen to flounder.");
                _state = kGameTutorialFloat;
            }
        } else if (_state == kGameTutorialFloat) {
            if (_player->getPositionY() < _screenSize.height * 0.95f) {
                _player->stopAllActions();
                _running = false;
                _tutorialLabel->setString("While flounder, tap the screen again to drop.");
                _state = kGameTutorialDrop;
            }
        } else {
            _tutorialLabel->setString("That's it. Tap the screen to play.");
            _state = kGameTutorial;
        }
    }
    
    
}


//void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* event) {
bool GameLayer::onTouchBegan(Touch* touch, Event* event) {

//	CCTouch *touch = (CCTouch *)pTouches->anyObject();
    log("ontouch");

    if (touch) {
	    
	    Point tap = touch->getLocation();

        if (_sound->boundingBox().containsPoint(tap)) {
            if(_isSound == true)  {
                makeSoundOff();
            }else {
                makeSoundOn();
            }
            return true;
        }
   
        switch (_state) {
            
            case kGameIntro:
                break;
            case kGameOver:
                
                if (_tryAgain->boundingBox().containsPoint(tap)) {
                    _hat->setVisible(false);
                    _hat->stopAllActions();
                    _tryAgain->setVisible(false);
                    _terrain->reset();
                    _player->reset();
                    
                    resetGame();
                }
                break;
                
            case kGamePlay:
                
                if (_player->getState() == kPlayerFalling) {
                    _player->setFloating ( _player->getFloating() ? false : true );
                
                } else {
                    if (_player->getState() !=  kPlayerDying) {
                        if(_isSound == true)  {
                            SimpleAudioEngine::getInstance()->playEffect("jump.wav");
                        }

                        _player->setJumping(true);
                    }
                }
            
                _terrain->activateChimneysAt(_player);
                
                break;
            case kGameTutorial:
                _tutorialLabel->setString("");
                _tutorialLabel->setVisible(false);
                _terrain->setStartTerrain ( true );
                _state = kGamePlay;
                break;
            case kGameTutorialJump:
                if (_player->getState() == kPlayerMoving) {
                    if(_isSound==true)  {
                        SimpleAudioEngine::getInstance()->playEffect("jump.wav");
                    }
                    _player->setJumping(true);
                }
                break;
            case kGameTutorialFloat:
                if (!_player->getFloating()) {
                    _player->setFloating (true);
                    _running = true;
                }
                break;
            case kGameTutorialDrop:
                _player->setFloating (false);
                _running = true;
                break;
        }
    }
    return true;
}

void GameLayer::onTouchEnded(Touch* touch, Event* event) {
    log("ontouchend");
    if (_state == kGamePlay) {
        _player->setJumping(false);
    }
    
}

void GameLayer::showTutorial (Ref* pSender) {
    _tutorialLabel->setString("Tap the screen to make the player jump.");
    _state = kGameTutorialJump;
//    _jam->runAction(_jamMove);
    _intro->setVisible(false);
    _mainMenu->setVisible(false);
//    SimpleAudioEngine::getInstance()->playEffect("start.wav");
    _tutorialLabel->setVisible(true);
    
}

void GameLayer::startGame (Ref* pSender) {
    _tutorialLabel->setVisible(false);
    _intro->setVisible(false);
    _mainMenu->setVisible(false);
    
//    _jam->runAction(_jamMove);
 //   SimpleAudioEngine::getInstance()->playEffect("start.wav");
    _terrain->setStartTerrain ( true );
    _state = kGamePlay;
}

void GameLayer::createGameScreen () {
    
    
    Sprite * bg = Sprite::create("bg.png");
    bg->getTexture()->setAliasTexParameters();
    bg->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg, kBackground);
    
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("caveman_UntitledSheet.plist");
    _gameBatchNode = SpriteBatchNode::create("caveman_UntitledSheet.png", 200);
    this->addChild(_gameBatchNode, kMiddleground);
    
    Sprite * repeat;
    
    _background = CCSprite::createWithSpriteFrameName("background_new");
    _background->setAnchorPoint(Point(0,0));
    _gameBatchNode->addChild(_background, kBackground);
//    _background->getTexture()->setAliasTexParameters();
    
    repeat = CCSprite::createWithSpriteFrameName("background_new");
//    repeat->getTexture()->setAliasTexParameters();
    
    repeat->setAnchorPoint(Point(0,0));

    repeat->setPosition(Point(repeat->getContentSize().width - 1, 0));

    _background->addChild(repeat, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("background_new");
//    repeat->getTexture()->setAliasTexParameters();

    repeat->setAnchorPoint(Point(0,0));

    repeat->setPosition(Point(2 * (repeat->getContentSize().width - 1), 0));
    _background->addChild(repeat, kBackground);
    
    _foreground = CCSprite::createWithSpriteFrameName("light");
    _foreground->getTexture()->setAliasTexParameters();
    _foreground->setAnchorPoint(Point(0,0));
    _gameBatchNode->addChild(_foreground, kForeground);
    
    repeat = CCSprite::createWithSpriteFrameName("light");
    repeat->getTexture()->setAliasTexParameters();
    repeat->setAnchorPoint(Point(0,0));
    repeat->setPosition(Point(repeat->getContentSize().width * 4, 0));
    _foreground->addChild(repeat, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("light");
    repeat->getTexture()->setAliasTexParameters();
    repeat->setAnchorPoint(Point(0,0));
    repeat->setPosition(Point(repeat->getContentSize().width * 8, 0));
    _foreground->addChild(repeat, kBackground);
    
    
    //add clouds
//    Sprite * cloud;
//    _clouds = CCArray::createWithCapacity(4);
//    _clouds.reserve(4);
//    _clouds->retain();
//    float cloud_y;
//    for (int i = 0; i < 4; i++) {
//        cloud_y = i % 2 == 0 ? _screenSize.height * 0.7f : _screenSize.height * 0.8f;
//        cloud = CCSprite::createWithSpriteFrameName("cloud");
//        cloud->getTexture()->setAliasTexParameters();
//        cloud->setPosition(Point (_screenSize.width * 0.15f + i * _screenSize.width * 0.25f,  cloud_y));
//        _gameBatchNode->addChild(cloud, kBackground);
////        _clouds->addObject(cloud);
////        _clouds.pushBack(cloud);
//    }
    
    
    _terrain = Terrain::create();

    _gameBatchNode->addChild(_terrain, kMiddleground);
    
    _player = Player::create();
    _gameBatchNode->addChild(_player, kBackground);
    
    _intro = CCSprite::createWithSpriteFrameName("logo");
    _intro->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.7f));
    _gameBatchNode->addChild(_intro, kForeground);
    
    
    _tryAgain = CCSprite::createWithSpriteFrameName("label_tryagain");
    _tryAgain->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.7f));
    _tryAgain->setVisible(false);
    this->addChild(_tryAgain, kForeground);
    
    _isSound = UserDefault::getInstance()->getBoolForKey("sound", false);
    
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);

    
    if(_isSound==true)  {
        _sound = CCSprite::createWithSpriteFrameName("sound_on");
    }else {
        _sound = CCSprite::createWithSpriteFrameName("sound_off");
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

    }
//    _sound = CCSprite::createWithSpriteFrameName("sound_on");
    _sound->setPosition(Point(_screenSize.width * 0.9f, _screenSize.height * 0.1f));
    this->addChild(_sound, kForeground);
    
    
//    _scoreDisplay = CCLabelBMFont::create("000000", "font.fnt", _screenSize.width * 0.3f, kCCTextAlignmentCenter);
    _scoreDisplay = Label::createWithBMFont("font.fnt", "000000",  TextHAlignment::CENTER, _screenSize.width * 0.3f);
    
    _scoreDisplay->setAnchorPoint(Point(1,0));
    _scoreDisplay->setPosition(Point(_screenSize.width * 0.95f, _screenSize.height * 0.88f));
//    this->addChild(_scoreDisplay, kBackground);
    this->addChild(_scoreDisplay, kMiddleground);
    
    
    _hat = CCSprite::createWithSpriteFrameName("axe");
    _hat->setVisible(false);
    _gameBatchNode->addChild(_hat, kMiddleground);
    
    
   
//    _jam = CCSprite::createWithSpriteFrameName("jam_1");
    
    Animation* animation;
    animation = CCAnimation::create();
    SpriteFrame * frame;
    int i;
    for(i = 1; i <= 3; i++) {
        char szName[100] = {0};
        sprintf(szName, "jam_%i", i);
        frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szName);
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(0.2f / 3.0f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);
    
//    _jamAnimate = CCAnimate::create(animation);
//    _jamAnimate->retain();
//    _gameBatchNode->addChild(_jam, kBackground);
    
//    _jam->setPosition(Point(_screenSize.width * 0.19f, _screenSize.height * 0.47f));
//    _jamMove = CCMoveTo::create(6.0f, Point(-_screenSize.width * 0.3f, _jam->getPositionY()));
//    _jamMove->retain();
    
    
    
    //add menu
    Sprite * menuItemOn;
    Sprite * menuItemOff;
    
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_new_on");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_new_off");
    
/*    MenuItemSprite * starGametItem = MenuItemSprite::create(
                                                                menuItemOff,
                                                                menuItemOn,
                                                                this,
                                                                menu_selector(GameLayer::startGame));
    
*/
/*       auto item1 = MenuItemSprite::create(spriteNormal, spriteSelected, spriteDisabled, CC_CALLBACK_1(MenuLayerMainMenu::menuCallback, this) );
 */
    
    auto starGametItem = MenuItemSprite::create(
                                                            menuItemOff,
                                                            menuItemOn,
                                                            CC_CALLBACK_1(GameLayer::startGame, this));
    
    
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_howto_on");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_howto_off");
    
    /*
    MenuItemSprite * howToItem = MenuItemSprite::create(
                                                            menuItemOff,
                                                            menuItemOn,
                                                            this,
                                                            menu_selector(GameLayer::showTutorial));
    */
    
    auto howToItem = MenuItemSprite::create(
                                                            menuItemOff,
                                                            menuItemOn,
                                                            CC_CALLBACK_1(GameLayer::showTutorial, this));
    
    
    _mainMenu = Menu::create(howToItem, starGametItem, NULL);
    _mainMenu->alignItemsHorizontallyWithPadding(120);
    //_mainMenu->alignItemsVerticallyWithPadding(50);
    _mainMenu->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.54));

    this->addChild(_mainMenu, kForeground);
      
//    _tutorialLabel = CCLabelTTF::create("", "Times New Roman", 80);
    _tutorialLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 80);
    _tutorialLabel->setPosition(Point (_screenSize.width * 0.5f, _screenSize.height * 0.6f) );
    this->addChild(_tutorialLabel, kForeground);
    _tutorialLabel->setVisible(false);
    
}

void GameLayer::makeSoundOn()  {
    _isSound = true;
    UserDefault::getInstance()->setBoolForKey("sound", true);
    SpriteFrame * frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("sound_on");
    _sound->setSpriteFrame(frame);
//    _sound->setTexture(Director::getInstance()->getTextureCache()->addImage("sound_on"));

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

 //   ("background.mp3", true);
    
}

void GameLayer::makeSoundOff()  {
    _isSound = false;
    UserDefault::getInstance()->setBoolForKey("sound", false);
   SpriteFrame * frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("sound_off");
    _sound->setSpriteFrame(frame);
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
}