#ifndef __ROLE_CONTROLLER_H__
#define __ROLE_CONTROLLER_H__

#include "cocos2d.h"
USING_NS_CC;

#include "string"


class RoleControllerListenr {
public:
   virtual void onControllerTouchBegan(Vec2 velocity) = 0;
   virtual void onControllerTouchMoving(Vec2 velocity) = 0;
   virtual void onControllerTouchEnded(Vec2 velocity) = 0;
};

class RoleController : public Layer {
public:
	bool touchIngInside = false;
	float radius = 0;
	float ball_d_bg = 0;
	Vec2 offset;
	Color4F bg_color = Color4F(-1.0f,-1.0f,-1.0f,-1.0f);
	Color4F ball_color = Color4F(-1.0f, -1.0f, -1.0f, -1.0f);;
	Node * tempLayer;
	Node * tempLayer2;
	std::string bg_resouce_path;
	std::string ball_resouce_path;
	static RoleController* createController(float radius,const Vec2 offset);

	RoleControllerListenr * listener;

	virtual bool init(); 

	void setControllerPosition(Vec2 position); 
	void setControllerSize(float radius);

	void setBgResoucePath(std::string path);
	void setBallResoucePath(std::string path);

	void setBall_D_Bg(float proportion);
	void setBgColor(Color4F color);
	void setBallColor(Color4F color);

	bool touchBeganCallback(Touch* touch, Event* event);
	void touchMovedCallback(Touch* touch, Event* event);
	void touchEndedCallback(Touch* touch, Event* event);

	void setRoleControllerListenr(RoleControllerListenr * listener);

	CREATE_FUNC(RoleController);
};


#endif