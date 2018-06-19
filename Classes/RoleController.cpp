#include "RoleController.h" 

#include <stdlib.h>
#include <math.h>

#define Tag_Bg_Layer 0
#define Tag_Ball_Layer 1

#define Default_ball_d_bg (float)1/2
#define Default_bg_color Color4F(100, 100, 0, 100)
#define Default_ball_color Color4F(100, 100, 0, 50)

RoleController* RoleController::createController(float radius, const Vec2 offset = Vec2::ZERO) {
	auto sceeenSize = Director::getInstance()->getWinSize();
	auto layer = RoleController::create();  
	layer->setAnchorPoint(Vec2(0,0));
	layer->radius = radius;
	layer->offset = offset;
	layer->bg_resouce_path = "";
	layer->ball_resouce_path = ""; 
	layer->setContentSize(sceeenSize);
	layer->setPosition(Vec2::ZERO);

	layer->tempLayer = nullptr;
	if (layer->bg_resouce_path == "") {
		auto drawNode = DrawNode::create();
		drawNode->setContentSize(Size(layer->radius * 2, layer->radius * 2));
		drawNode->drawDot(Vec2(layer->radius, layer->radius), 
			layer->radius, 
			layer->bg_color == Color4F(-1.0f, -1.0f, -1.0f, -1.0f)? Default_bg_color : layer->bg_color);
		
		layer->tempLayer = drawNode; 
	}
	else {
		auto resouceSprite = Sprite::create(layer->bg_resouce_path);
		layer->tempLayer = resouceSprite;
	}
	layer->tempLayer->setAnchorPoint(Vec2(0.5f, 0.5f)); 
	layer->tempLayer->setPosition(Vec2(layer->radius, layer->radius) + layer->offset); 


	auto drawNode2 = DrawNode::create(); 
	drawNode2->setContentSize(Size(layer->radius * 2, layer->radius * 2) * (layer->ball_d_bg != 0 ? layer->ball_d_bg : Default_ball_d_bg));
	drawNode2->drawDot(Vec2(layer->radius * (layer->ball_d_bg != 0 ? layer->ball_d_bg : Default_ball_d_bg),
		                    layer->radius * (layer->ball_d_bg != 0 ? layer->ball_d_bg : Default_ball_d_bg)),
		layer->radius * (layer->ball_d_bg!=0? layer->ball_d_bg: Default_ball_d_bg),
		layer->ball_color == Color4F(-1.0f, -1.0f, -1.0f, -1.0f) ? Default_ball_color : layer->ball_color);
	
	layer->tempLayer2 = drawNode2;   
	layer->tempLayer2->setAnchorPoint(Vec2(0.5f, 0.5f));
	layer->tempLayer2->setPosition(Vec2(layer->radius, layer->radius) + layer->offset);
	 
	layer->addChild(layer->tempLayer, 1, Tag_Bg_Layer);
	layer->addChild(layer->tempLayer2, 2, Tag_Ball_Layer);

	auto touchListner = EventListenerTouchOneByOne::create();
	touchListner->onTouchBegan = CC_CALLBACK_2(RoleController::touchBeganCallback, layer);
	touchListner->onTouchMoved = CC_CALLBACK_2(RoleController::touchMovedCallback, layer);
	touchListner->onTouchEnded = CC_CALLBACK_2(RoleController::touchEndedCallback, layer);
	layer->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListner, layer);

	return layer;
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
				if (touchP.x > centerP.x && touchP.y > centerP.y) {
					targetX = centerP.x + radius * (touchP.x - centerP.x) / distance;
					targetY = centerP.y + radius * (touchP.y - centerP.y) / distance;
				}else if (touchP.x < centerP.x && touchP.y > centerP.y) {
					targetX = centerP.x - radius * (centerP.x - touchP.x) / distance;
					targetY = centerP.y + radius * (touchP.y - centerP.y) / distance;
				}else if (touchP.x < centerP.x && touchP.y < centerP.y) {
					targetX = centerP.x - radius * (centerP.x - touchP.x) / distance;
					targetY = centerP.y - radius * (centerP.y - touchP.y) / distance;
				}else if (touchP.x > centerP.x && touchP.y < centerP.y) {
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

void RoleController::setControllerPosition(Vec2 position) {
	this->setPosition(position);
}
void RoleController::setControllerSize(float radius) {
	this->setContentSize(Size(radius, radius));
}

void RoleController::setBgResoucePath(std::string path) {
	this->bg_resouce_path = path;
}
void RoleController::setBallResoucePath(std::string path) {
	this->ball_resouce_path = path;
}

void RoleController::setBall_D_Bg(float proportion) {
	this->ball_d_bg = proportion;
}
void RoleController::setBgColor(Color4F color) {
	this->bg_color = color;
}
void RoleController::setBallColor(Color4F color) {
	this->ball_color = color;
}


bool RoleController::init() {
	if (!Layer::init()) {
		return false;
	} 
	return true;
}