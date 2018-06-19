#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;


#include "RoleController.h"

class MainMenuScene : public cocos2d::Scene , public RoleControllerListenr
{
public:
	LayerColor * role; 
	Vec2 m_velocity;
	Size winSize;

	static cocos2d::Scene* createScene();

	virtual bool init(); 

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float dt); 

	void onControllerTouchBegan(Vec2 velocity); 
	void onControllerTouchMoving(Vec2 velocity);
	void onControllerTouchEnded(Vec2 velocity);
	 
	CREATE_FUNC(MainMenuScene);
};

#endif // __MAIN_MENU_SCENE_H__