#include "RoleController.h" 

#include <stdlib.h>
#include <math.h>

#define Tag_Bg_Layer 0
#define Tag_Ball_Layer 1

#define Default_ball_d_bg (float)1/2   // 控制器/背景尺寸的比例
#define Default_bg_color Color4F(100, 100, 0, 100)  // 背景颜色
#define Default_ball_color Color4F(100, 100, 0, 50)  // 控制球颜色

/*
  radius �������뾶����ȵ�1/2��
  offset ���������ϵԭ��(0,0)��ƫ����
*/
RoleController* RoleController::createController(float radius, const Vec2 offset) {
	auto sceeenSize = Director::getInstance()->getWinSize();
	auto layer = RoleController::create();  
	layer->setAnchorPoint(Vec2(0,0));
	layer->radius = radius;
	layer->offset = offset;
	layer->bg_resouce_path = "";
	layer->ball_resouce_path = ""; 
	layer->setContentSize(sceeenSize);
	layer->setPosition(Vec2::ZERO);

	layer->createBg();
	layer->createBall();

	auto touchListner = EventListenerTouchOneByOne::create();
	touchListner->onTouchBegan = CC_CALLBACK_2(RoleController::touchBeganCallback, layer);
	touchListner->onTouchMoved = CC_CALLBACK_2(RoleController::touchMovedCallback, layer);
	touchListner->onTouchEnded = CC_CALLBACK_2(RoleController::touchEndedCallback, layer);
	layer->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListner, layer);

	return layer;
}

void RoleController::createBg() {
	this->tempLayer = nullptr;
	if (this->bg_resouce_path == "") {
		auto drawNode = DrawNode::create();
		drawNode->setContentSize(Size(this->radius * 2, this->radius * 2));
		drawNode->drawDot(Vec2(this->radius, this->radius),
			this->radius,
			this->bg_color == Color4F(-1.0f, -1.0f, -1.0f, -1.0f) ? Default_bg_color : this->bg_color);

		this->tempLayer = drawNode;
	}
	else {
		auto resouceSprite = Sprite::create(this->bg_resouce_path);
		this->tempLayer = resouceSprite;
	}
	this->tempLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->tempLayer->setPosition(Vec2(this->radius, this->radius) + this->offset);
	this->addChild(this->tempLayer, 1, Tag_Bg_Layer);
}
void RoleController::createBall() {
	auto drawNode2 = DrawNode::create();
	drawNode2->setContentSize(Size(this->radius * 2, this->radius * 2) * (this->ball_d_bg != 0 ? this->ball_d_bg : Default_ball_d_bg));
	drawNode2->drawDot(Vec2(this->radius * (this->ball_d_bg != 0 ? this->ball_d_bg : Default_ball_d_bg),
		this->radius * (this->ball_d_bg != 0 ? this->ball_d_bg : Default_ball_d_bg)),
		this->radius * (this->ball_d_bg != 0 ? this->ball_d_bg : Default_ball_d_bg),
		this->ball_color == Color4F(-1.0f, -1.0f, -1.0f, -1.0f) ? Default_ball_color : this->ball_color);

	this->tempLayer2 = drawNode2;
	this->tempLayer2->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->tempLayer2->setPosition(Vec2(this->radius, this->radius) + this->offset);
	this->addChild(this->tempLayer2, 2, Tag_Ball_Layer);
}


bool RoleController::touchBeganCallback(Touch* touch, Event* event) {
	Vec2 touchP = touch->getLocation();
	Vec2 centerP = tempLayer->getPosition() ;
	  
	float distance = sqrt(pow(touchP.x - centerP.x, 2) + pow(touchP.y - centerP.y, 2)); 
	if (distance < this->radius) {
		tempLayer2->setPosition(touchP);
		touchIngInside = true;
		if (this->listener != nullptr)
			this->listener->onControllerTouchBegan(touchP - centerP);
	}
	else {
		touchIngInside = false;
	}
	return true;
}
void RoleController::touchMovedCallback(Touch* touch, Event* event) {
	
		Vec2 touchP = touch->getLocation();
		Vec2 centerP = tempLayer->getPosition();
		float distance = sqrt(pow(touchP.x - centerP.x, 2) + pow(touchP.y - centerP.y, 2));
		if (distance < this->radius) {
			tempLayer2->setPosition(touchP);
			if (this->listener != nullptr)
				this->listener->onControllerTouchMoving(touchP - centerP);
		}else {
			if (touchIngInside) {
				float targetX = 0, targetY = 0;
				if (touchP.x >= centerP.x && touchP.y >= centerP.y) {
					targetX = centerP.x + radius * (touchP.x - centerP.x) / distance;
					targetY = centerP.y + radius * (touchP.y - centerP.y) / distance;
				}else if (touchP.x < centerP.x && touchP.y >= centerP.y) {
					targetX = centerP.x - radius * (centerP.x - touchP.x) / distance;
					targetY = centerP.y + radius * (touchP.y - centerP.y) / distance;
				}else if (touchP.x < centerP.x && touchP.y < centerP.y) {
					targetX = centerP.x - radius * (centerP.x - touchP.x) / distance;
					targetY = centerP.y - radius * (centerP.y - touchP.y) / distance;
				}else if (touchP.x >= centerP.x && touchP.y < centerP.y) {
					targetX = centerP.x + radius * (touchP.x - centerP.x) / distance;
					targetY = centerP.y - radius * (centerP.y - touchP.y ) / distance;
				}
				tempLayer2->setPosition(targetX,targetY);
				if (this->listener != nullptr)
					this->listener->onControllerTouchMoving(Vec2(targetX, targetY)-centerP);
			}
		} 
}
void RoleController::touchEndedCallback(Touch* touch, Event* event) {
	touchIngInside = false;
	Vec2 centerP = tempLayer->getPosition() ;
	tempLayer2->setAnchorPoint(Vec2(0.5f, 0.5f));
	tempLayer2->setPosition(centerP);
	if (this->listener != nullptr)
		this->listener->onControllerTouchEnded(Vec2::ZERO);
}

void RoleController::setRoleControllerListenr(RoleControllerListenr * listener)
{
	this->listener = listener;
}
void RoleController::setPositioin(Vec2 position) {
	setOffset(position);
} 

void RoleController::setOffset(Vec2 offset) {
	this->offset = offset; 
	this->tempLayer->setPosition(Vec2(this->radius, this->radius) + this->offset);
	this->tempLayer2->setPosition(Vec2(this->radius, this->radius) + this->offset);
} 

void RoleController::setBgResoucePath(std::string path) {
	this->bg_resouce_path = path;
}
void RoleController::setBallResoucePath(std::string path) {
	this->ball_resouce_path = path;
}


bool RoleController::init() {
	if (!Layer::init()) {
		return false;
	} 
	return true;
}
