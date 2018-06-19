#include "MainMenuScene.h"


Scene* MainMenuScene::createScene() {
	return MainMenuScene::create(); 
}

bool MainMenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	role = LayerColor::create(Color4B::BLUE);
	role->setContentSize(Size(30,30));
	role->setPosition(winSize.width/2,winSize.height/2);
	addChild(role,1);

	auto controller = RoleController::createController(80,Vec2(50,80));
	controller->setRoleControllerListenr(this);
	addChild(controller,0); 



	return true;
}

void MainMenuScene::update(float dt) { 
	float speed = 3.0f;
	Vect newPosition = role->getPosition() + Vec2(this->m_velocity.x, this->m_velocity.y) / 60.0f * speed;
	if (newPosition.x> 0 && newPosition.x < winSize.width 
		&& newPosition.y > 0 && newPosition.y < winSize.height)
	   role->setPosition(newPosition);
}

void MainMenuScene::onControllerTouchBegan(Vec2 velocity)
{
	CCLOG("MainMenuScene start %f %f", velocity.x, velocity.y);
	this->m_velocity = velocity;
	scheduleUpdate();
}

void MainMenuScene::onControllerTouchMoving( Vec2 velocity)
{
	CCLOG("MainMenuScene moving %f %f", velocity.x, velocity.y);
	this->m_velocity = velocity;
}

void MainMenuScene::onControllerTouchEnded(Vec2 velocity)
{
	CCLOG("MainMenuScene end %f %f", velocity.x, velocity.y);
	unscheduleUpdate();
}

// a selector callback
void MainMenuScene::menuCloseCallback(cocos2d::Ref* pSender) {

} 