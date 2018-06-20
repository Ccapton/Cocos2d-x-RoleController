# Cocos2d-x-RoleController
This a role controller using Cocos2d-x , which can be used in 2d game designing.
.这是一个使用cocos2dx引擎设计的精灵移动控制器，适用于开发2d游戏

![](https://raw.githubusercontent.com/Ccapton/Cocos2d-x-RoleController/master/rolecontroller_demo.gif)

### 引用
1、将Classes内的RoleController.cpp和RoleController.h文件复制到你的Classes目录下，并在你想要的场景或者层中引入。

2、RoleController类的创建和使用,例如
```
// 在你的合适位置初始化控制器即可
bool MainMenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	role = LayerColor::create(Color4B::BLUE);
	role->setContentSize(Size(30,30));
	role->setPosition(winSize.width/2,winSize.height/2);
	addChild(role,0);
 
	// 初始化控制器
	auto controller = RoleController::createController(80,Vec2(50,80));
	controller->setRoleControllerListenr(this);  // MainMenuScene（场景）类要继承RoleControllerListenr接口，然后
	addChild(controller,1); 


	return true;
}

// 控制器父类容器需要继承RoleControllerListenr接口，例如
#include "RoleController.h"

class MainMenuScene : public cocos2d::Scene , public RoleControllerListenr
{
public:
  LayerColor * role; 
  Vec2 m_velocity;
  Size winSize;
  // ... 其他代码
  
  // 声明3个接口的抽象函数，并在你对应的cpp文件中实现它们
  void onControllerTouchBegan(Vec2 velocity); 
  void onControllerTouchMoving(Vec2 velocity);
  void onControllerTouchEnded(Vec2 velocity);
}

// 在对应的cpp文件中实现这3个RoleControllerListenr接口的方法,例如在MainMenuScene.cpp中
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

// 在此更新函数内，编写控制角色位置的逻辑代码
void MainMenuScene::update(float dt) { 
	float speed = 3.0f; // 每一帧角色移动的速度
	// 因为是默认的更新方法，所以dt的值为 1/60 秒，即一秒内定时器会执行60次该方法，
        // 因此要将偏移量除以 60 ,防止因每一帧偏移量太大而使角色位移太快
	Vect newPosition = role->getPosition() + Vec2(this->m_velocity.x, this->m_velocity.y) / 60.0f * speed;
	if (newPosition.x> 0 && newPosition.x < winSize.width 
		&& newPosition.y > 0 && newPosition.y < winSize.height)
	   role->setPosition(newPosition);
}

```
