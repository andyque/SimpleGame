//
//  GameOverScene.h
//  SimpleGame
//
//  Created by guanghui on 3/14/15.
//
//

#ifndef __SimpleGame__GameOverScene__
#define __SimpleGame__GameOverScene__

#include "cocos2d.h"

using namespace cocos2d;

class GameOverScene : public Scene
{
public:
    static GameOverScene* create(Size size, bool won);
    
protected:
    bool init(Size size, bool won);
};

#endif /* defined(__SimpleGame__GameOverScene__) */
