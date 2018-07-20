#include "Enemy.h"
#include "Constants.h"

bool Enemy::initWithEnemyType(const EnemyType& type){
	//用成员变量保存类型，记住之后后续可以随时访问
	m_type = type;
	std::string frameName ="enemy1.png";
	this->m_speed = SMALL_ENEMY_SPEED;
	this->m_hp = 1;
	this->m_score = 10;
	switch (m_type)
	{
	case EnemyType::MIDDLE_ENEMY:
		frameName = "enemy2.png";
		this->m_speed = MIDDELE_ENEMY_SPEED;
		this->m_hp = 5;
		this->m_score = 100;
		break;
	case EnemyType::BIG_ENEMY:
		frameName = "enemy3_n1.png";
		this->m_speed = BIG_ENEMY_SPEED;
		this->m_hp = 25;
		this->m_score = 1000;
		break;
	default:
			break;
	}
	//不要调用成 createWithSpriteFrameName
	if (!Sprite::initWithSpriteFrameName(frameName))
	{
		return false;
	}

	if (m_type == EnemyType::BIG_ENEMY)
	{
		auto ani = Animation::create();
		//b.添加动画帧（从精灵帧缓存中根据名字查找出来）
		ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
		ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
		//c.设置动画切换时长
		ani->setDelayPerUnit(0.2f);
		//d.设置循环次数，使用宏标识无限次，可以使用-1
		ani->setLoops(CC_REPEAT_FOREVER);
		//2.将动画封装为动作
		//3.精灵运行动作
		this->runAction(Animate::create(ani));
		
	}
	return true;
}

Enemy* Enemy::create(const EnemyType& type){
	auto enemy = new Enemy();
	if (enemy && enemy->initWithEnemyType(type))
	{
		enemy->autorelease(); //放入自动计数池
		return enemy;

	}
	delete enemy;
	enemy = nullptr;
	return nullptr;
}


void Enemy::down(){
	//碰撞，实现敌机销毁动画
	//1.创建动画
	//a.创建动画对象
	//b.添加动画帧（从精灵帧缓存中根据名字查找出来）
	//c.设置动画切换时长
	//d.设置循环次数
	auto ani = Animation::create();
	switch (m_type)
	{
	case EnemyType::SMALL_ENEMY:
		for (auto i = 0; i < 4; i++)
		{
			auto frameName = StringUtils::format("enemy1_down%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		}
		break;
	case EnemyType::MIDDLE_ENEMY:
		for (auto i = 0; i < 4; i++)
		{
			auto frameName = StringUtils::format("enemy2_down%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		}
		break;
	case EnemyType::BIG_ENEMY:
		for (auto i = 0; i < 6; i++)
		{
			auto frameName = StringUtils::format("enemy3_down%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		}
		break;
	default:
		break;
	}

	ani->setDelayPerUnit(0.2f);
	//2.将动画封装为动作
	auto animate = Animate::create(ani);
	/*
	//方法一，顺序执行语句，按参数顺序执行，最后必须为NULL
	auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node){
		this->removeChild(node);
	}), nullptr);*/

	/*方法二
	auto seq = Sequence::create(animate, CallFuncN::create([](Node *node){
	node->removeFromParentAndCleanup(true);
	}), nullptr);
	*/
	//方法三,简单方法
	auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
	//3.运行动作,对于碰撞过得敌机和子弹需要及时删除
	this->stopAllActions();
	this->runAction(seq);
}

void Enemy::move(){
	this->setPositionY(this->getPositionY() + this->m_speed);
}


/*

bool @return 减血后是否死亡了
*/
bool Enemy::hit(){
	//已经死亡直接退出
	if (this->m_hp <= 0) return true;
	this->m_hp--;
	//此刻是否死亡
	if (this->m_hp <= 0)
	{
		this->down();
		return true;
	}
	else
	{
		auto ani = Animation::create();
		switch (m_type)
		{
		case EnemyType::MIDDLE_ENEMY:
		{	
		    ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_hit.png"));
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png"));
			ani->setDelayPerUnit(0.2f);
			runAction(Animate::create(ani));
		}
		break;
		case EnemyType::BIG_ENEMY:
		{
		    ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_hit.png"));
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
			ani->setDelayPerUnit(0.2f);
		    runAction(Animate::create(ani));
		}
		break;
		default:
			break;
		}
		
	}
	return false;
	
}
