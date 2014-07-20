#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Terrain.h"
#include "Player.h"

USING_NS_CC;
using namespace CocosDenshion;


typedef enum {
    kGameIntro,
    kGamePlay,
    kGameOver,
    kGameTutorial,
    kGameTutorialJump,
    kGameTutorialFloat,
    kGameTutorialDrop
    
} GameState;


class GameLayer : public cocos2d::Layer
{
    
    Terrain * _terrain;
    Player * _player;
    Label * _scoreDisplay;
    
    Sprite * _intro;
    Sprite * _tryAgain;
    Sprite * _background;
    Sprite * _foreground;
    Sprite * _hat;
    Sprite * _jam;
    Action * _jamAnimate;
    Action * _jamMove;
    
    Vector<Sprite*> _clouds;
    
    SpriteBatchNode * _gameBatchNode;
    Label * _tutorialLabel;
    Menu* _mainMenu;
    
    Size _screenSize;
    
    GameState _state;
    
    bool _running;
    float _score;
    int _speedIncreaseInterval;
	float _speedIncreaseTimer;
    
    void createGameScreen();
    void resetGame();
    
    
    
public:
    
    ~GameLayer();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::Scene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);
    
    void update (float dt);
    
//    virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
//    virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);
    virtual bool onTouchBegan(Touch* pTouch, Event* event);
    virtual void onTouchEnded(Touch* pTouch, Event* event);

    
    void showTutorial (Ref* pSender);
    void startGame (Ref* pSender);
};


#endif // __GAMESCENE_H__


