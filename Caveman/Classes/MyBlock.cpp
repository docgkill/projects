#define TOTAL_PUFFS 3


#include "MyBlock.h"

Block::~Block () {
    
    _chimneys.clear();
    _wallTiles.clear();
//    _roofTiles.clear();
    
    CC_SAFE_RELEASE(_puffAnimation);
    CC_SAFE_RELEASE(_puffSpawn);
    CC_SAFE_RELEASE(_puffMove);
    CC_SAFE_RELEASE(_puffFade);
    CC_SAFE_RELEASE(_puffScale);
    
    
}

Block::Block () {
    
    //get screen size
	_screenSize = CCDirector::getInstance()->getWinSize();
    // _tileWidth는 256, 256 * 8 = 2048
    // _tileHeight는 256, 256 * 6 = 1536
    _tileWidth = _screenSize.width / TILE_W_SIZE;
    _tileHeight = _screenSize.height / TILE_H_SIZE;
    _puffing = false;
    this->setVisible(false);
}

Block * Block::create () {
    
    Block * block = new Block();
	if (block && block->initWithSpriteFrameName("blank")) {
        block->getTexture()->setAliasTexParameters();
		block->autorelease();
        block->initBlock();
		return block;
	}
	CC_SAFE_DELETE(block);
	return NULL;
}

void Block::setPuffing (bool value) {
    
    _puffing = value;
    
    if (value) {
    //    this->runAction((Action *) _puffSpawn->clone()->autorelease());
        this->runAction((Action *) _puffSpawn->clone());

        /*
        grossini->runAction( Sequence::create(
                                              MoveBy::create(1, Point(150,0)),
                                              CallFuncN::create(CC_CALLBACK_1(LogicTest::bugMe,this)),
                                              NULL)
                            );
        */
        
        Action * hide = CCSequence::create(
                                           CCDelayTime::create(2.5f),
                                           // 0는 input parameter가 0임을 의미
                                           CallFuncN::create(CC_CALLBACK_0(Block::hidePuffs, this)),
                                           NULL);
        
        this->runAction(hide);
    } else {
        //reset all puffs
        _puffIndex = 0;
        int count = _chimneys.size();
        Sprite * chimney;
        Sprite * puff;
        
        for (int i = 0; i < count; i++) {
            chimney = (Sprite * ) _chimneys.at(i);
            for (int j = 0; j < TOTAL_PUFFS; j++) {
                puff = (Sprite *) chimney->getChildByTag(j);
                puff->setVisible(false);
                puff->stopAllActions();
                puff->setScale(1.0);
                puff->setOpacity(255);
                puff->setPosition(Point(0,0));
            }
        }

    }
    
}

void Block::hidePuffs() {
    setPuffing(false);
}

// type이 kBlockGap이면 해당 블럭은 보이지 않고, width는 갭 넓이, height는 0이됨.
void Block::setupBlock (int width, int height, int type) {
    
    this->setPuffing(false);
    
    _type = type;
//    this->setType(type);
    
    _width = width * _tileWidth;
//    _height = height * _tileHeight + _tileHeight * 0.49f;
    _height = height * _tileHeight;
    this->setPositionY(_height);
    
    SpriteFrame * wallFrame;
//    SpriteFrame * roofFrame = rand() % 10 > 6 ? _roof1 : _roof2;
    
    
    int num_chimneys;
    float chimneyX[] = {0,0,0,0,0};
    
    switch (type) {
        
        case kBlockGap:
            this->setVisible(false);
            return;
            
        case kBlock1:
            wallFrame = _tile1;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.8f;
            num_chimneys = 2;
            break;
        case kBlock2:
            wallFrame = _tile2;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.8f;
            chimneyX[2] = 0.5f;
            num_chimneys = 3;
            break;
        case kBlock3:
            wallFrame = _tile3;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.8f;
            chimneyX[2] = 0.5f;
            num_chimneys = 3;
            
            break;
        case kBlock4:
            wallFrame = _tile4;
            chimneyX[0] = 0.2f;
            chimneyX[1] = 0.5f;
            num_chimneys = 2;
            break;
    }
    
    int i;
    GameSprite * chimney;
    int count;
    count = _chimneys.size();
    for (i = 0; i < count; i++) {
        chimney = (GameSprite *) _chimneys.at(i);
        if (i < num_chimneys) {
            chimney->setPosition( Point (chimneyX[i] * _width, 0) );
            chimney->setVisible(true);
            
        } else {
            chimney->setVisible(false);
        }
    }
    
    this->setVisible(true);
    
    Sprite * tile;
/*
    count = _roofTiles.size();
    for (i  = 0; i < count; i++) {
        tile = (Sprite *) _roofTiles.at(i);
        
        
        if (tile->getPositionX() < _width) {
            tile->setVisible(true);
            tile->getTexture()->setAliasTexParameters();
            tile->setDisplayFrame(roofFrame);
        } else {
            tile->setVisible(false);
        }
    }
  */
    count = _wallTiles.size();
    for (i  = 0; i < count; i++) {
        tile = (Sprite *) _wallTiles.at(i);
        if (tile->getPositionX() < _width && tile->getPositionY() > -_height) {
            tile->getTexture()->setAliasTexParameters();
         //   tile->getTexture()->setAntiAliasTexParameters();
            tile->setVisible(true);
            
            tile->setDisplayFrame(wallFrame);
        } else {
            tile->setVisible(false);
        }
    }
}

void Block::initBlock() {

    // 빌딩과 루프에 대한 스프라이트 프레임 레퍼런스 보관
    _tile1 = CCSpriteFrameCache::getInstance()->getSpriteFrameByName ("building_1");
    _tile2 = CCSpriteFrameCache::getInstance()->getSpriteFrameByName ("building_2");
    _tile3 = CCSpriteFrameCache::getInstance()->getSpriteFrameByName ("building_3");
    _tile4 = CCSpriteFrameCache::getInstance()->getSpriteFrameByName ("building_4");
    _tile1->getTexture()->setAliasTexParameters();
    _tile2->getTexture()->setAliasTexParameters();
    _tile3->getTexture()->setAliasTexParameters();
    _tile4->getTexture()->setAliasTexParameters();
    
//    _roof1 = CCSpriteFrameCache::getInstance()->getSpriteFrameByName ("roof_1");
//    _roof2 = CCSpriteFrameCache::getInstance()->getSpriteFrameByName ("roof_2");
//    _roof1->getTexture()->setAliasTexParameters();
//    _roof2->getTexture()->setAliasTexParameters();
    

    //create tiles
    int i;
    
    // 벽은 20개 보관
    _wallTiles.reserve(20);
    //= CCArray::createWithCapacity(20);
//    _wallTiles.retain();

    // 지붕은 5개 보관
//    _roofTiles.reserve(5);
  //  _roofTiles = CCArray::createWithCapacity(5);
  //  _roofTiles->retain();

    
    Sprite * tile;
    
    // 미리 roof_1 스프라이트를 5장 준비하여 보관, x는 5장 옆으로 y는 0, 크기는 138, 즉 -138까지 지붕
    // 미리 building_1 스프라이트를 20장 준비하여 보관, x는 5장 옆으로 y는 47, -120정도 부터 배치
    // 즉 눈에 보이지 않는 화면 아래에 5장 정도로 깔아둔다.
    for (i = 0; i < 5; i++) {
//        tile = CCSprite::createWithSpriteFrameName("roof_1");
//        tile->getTexture()->setAliasTexParameters();
//        tile->setAnchorPoint(Point(0, 1));
//        tile->setPosition(Point(i * _tileWidth, 0));
//        tile->setVisible(false);
//        this->addChild(tile, kMiddleground, kRoofTile);
//        _roofTiles.pushBack(tile);
 //       _roofTiles->addObject(tile);
        
        for (int j = 0; j < 4; j++) {
            tile = Sprite::createWithSpriteFrameName("building_1");
            tile->getTexture()->setAliasTexParameters();
            tile->setAnchorPoint(Point(0, 1));
//            tile->setPosition(Point(i * _tileWidth, -1 * (_tileHeight * 0.47f + j * _tileHeight)));
            tile->setPosition(Point(i * _tileWidth, -1 * (j * _tileHeight)));
            tile->setVisible(false);
            this->addChild(tile, kBackground, kWallTile);
            _wallTiles.pushBack(tile);
 //           _wallTiles->addObject(tile);
        }
        
    }
    
    _chimneys.reserve(5);
//    _chimneys = CCArray::createWithCapacity(5);
//    _chimneys->retain();
    
    
    Sprite * chimney;
    Sprite * puff;
    
    // 타일을 옆으로 5장이니 굴뚝도 5장인가?
    for (i = 0; i < 5; i++) {
        chimney = Sprite::createWithSpriteFrameName("chimney");
        chimney->getTexture()->setAliasTexParameters();
        chimney->setVisible(false);
        this->addChild(chimney, kForeground, kChimney);
 
        _chimneys.pushBack(chimney);
//        _chimneys->addObject(chimney);
        
        for (int j = 0; j < TOTAL_PUFFS; j++) {
            puff = Sprite::createWithSpriteFrameName("puff_1");
            puff->getTexture()->setAliasTexParameters();
            puff->setAnchorPoint(Point(0,-0.5));
            puff->setVisible(false);
            chimney->addChild(puff, -1, j);
        }
        
    }
    
    
    Animation* animation;
    animation = Animation::create();
    SpriteFrame * frame;

    for(i = 1; i <= 4; i++) {
        char szName[100] = {0};
        sprintf(szName, "puff_%i", i);
        frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szName);
        frame->getTexture()->setAliasTexParameters();
        animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(0.75f / 4.0f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);
    _puffAnimation = CCAnimate::create(animation);
    _puffAnimation->retain();

    // docgkill
    _puffSpawn = Repeat::create(Sequence::create(DelayTime::create(0.5f),
                                                 CallFuncN::create( CC_CALLBACK_0(Block::createPuff, this) ),
                                                 NULL
                                                 ),
                                TOTAL_PUFFS
                                );
    _puffSpawn->retain();
    
 //   this->runAction(_puffSpawn->clone());

    _puffMove = MoveBy::create(1.0f, Point(-100,80));
    _puffMove->retain();
    _puffFade = CCFadeOut::create(2.0f);
    _puffFade->retain();
    _puffScale = CCScaleBy::create(1.5f, 1.5);
    _puffScale->retain();
    
    _puffIndex = 0;
}


void Block::createPuff () {

//    int count = _chimneys->count();
    int count = _chimneys.size();
    Sprite * chimney;
    Sprite * puff;
    
    for (int i = 0; i < count; i++) {
//        chimney = (Sprite * ) _chimneys->objectAtIndex(i);
        chimney = _chimneys.at(i);
        if (chimney->isVisible()) {
            puff = (Sprite *) chimney->getChildByTag(_puffIndex);
            puff->setVisible(true);
            puff->stopAllActions();
            puff->setScale(1.0);
            puff->setOpacity(255);
            puff->setPosition(Point(0,0));
            puff->runAction((Action *) _puffAnimation->clone());
            puff->runAction((Action *) _puffMove->clone());

            puff->runAction((Action *) _puffScale->clone());
            
        }
    }
    _puffIndex++;
    
    if (_puffIndex == TOTAL_PUFFS) _puffIndex = 0;

}
