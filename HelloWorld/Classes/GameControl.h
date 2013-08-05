#pragma

#include "cocos2d.h"
#include "CCGeometry.h"

class PauseLayer : public cocos2d::CCLayerColor
{
public:
	PauseLayer();
	~PauseLayer();
	virtual bool init();  
	CREATE_FUNC(PauseLayer);
};
