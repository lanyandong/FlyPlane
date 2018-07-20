#include "Enemy.h"
#include "Constants.h"

bool Enemy::initWithEnemyType(const EnemyType& type){
	//�ó�Ա�����������ͣ���ס֮�����������ʱ����
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
	//��Ҫ���ó� createWithSpriteFrameName
	if (!Sprite::initWithSpriteFrameName(frameName))
	{
		return false;
	}

	if (m_type == EnemyType::BIG_ENEMY)
	{
		auto ani = Animation::create();
		//b.��Ӷ���֡���Ӿ���֡�����и������ֲ��ҳ�����
		ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
		ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
		//c.���ö����л�ʱ��
		ani->setDelayPerUnit(0.2f);
		//d.����ѭ��������ʹ�ú��ʶ���޴Σ�����ʹ��-1
		ani->setLoops(CC_REPEAT_FOREVER);
		//2.��������װΪ����
		//3.�������ж���
		this->runAction(Animate::create(ani));
		
	}
	return true;
}

Enemy* Enemy::create(const EnemyType& type){
	auto enemy = new Enemy();
	if (enemy && enemy->initWithEnemyType(type))
	{
		enemy->autorelease(); //�����Զ�������
		return enemy;

	}
	delete enemy;
	enemy = nullptr;
	return nullptr;
}


void Enemy::down(){
	//��ײ��ʵ�ֵл����ٶ���
	//1.��������
	//a.������������
	//b.��Ӷ���֡���Ӿ���֡�����и������ֲ��ҳ�����
	//c.���ö����л�ʱ��
	//d.����ѭ������
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
	//2.��������װΪ����
	auto animate = Animate::create(ani);
	/*
	//����һ��˳��ִ����䣬������˳��ִ�У�������ΪNULL
	auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node){
		this->removeChild(node);
	}), nullptr);*/

	/*������
	auto seq = Sequence::create(animate, CallFuncN::create([](Node *node){
	node->removeFromParentAndCleanup(true);
	}), nullptr);
	*/
	//������,�򵥷���
	auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
	//3.���ж���,������ײ���õл����ӵ���Ҫ��ʱɾ��
	this->stopAllActions();
	this->runAction(seq);
}

void Enemy::move(){
	this->setPositionY(this->getPositionY() + this->m_speed);
}


/*

bool @return ��Ѫ���Ƿ�������
*/
bool Enemy::hit(){
	//�Ѿ�����ֱ���˳�
	if (this->m_hp <= 0) return true;
	this->m_hp--;
	//�˿��Ƿ�����
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
