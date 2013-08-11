#pragma
#include "cocos2d.h"
#include "CCGeometry.h"
#include "SimpleAudioEngine.h"


class Effects 
{
public:
	Effects();
	~Effects();
	static Effects* GetInstance();

public:
	cocos2d::CCParticleSystem*  popEffect;
	cocos2d::CCAnimationCache*  AniPool;
};
