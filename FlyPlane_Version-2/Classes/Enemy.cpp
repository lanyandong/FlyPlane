#include "Enemy.h"
#include "Constants.h"
#include "AudioEngine.h"

using namespace experimental;


bool Enemy::initWithEnemyType(const EnemyType &type) {
	//�ó�Ա�����������ͣ���ס֮�����������ʱ����
	m_type = type;
	std::string frameName = "enemy1.png";
	this->m_speed = -SMALL_ENEMY_SPEED;
	this->m_hp = SMALL_ENEMY_BLOOD;
	this->m_score = 100;
	this->kind = 1;
	this->m_tag = ENEMY1_TAG;

	switch (m_type)
	{
	case EnemyType::MIDDLE_ENEMY:
		frameName = "enemy2.png";
		this->m_speed = -MIDDLE_ENEMY_SPEED;
		this->m_score = 2000;
		this->m_hp = MIDDLE_ENEMY_BLOOD;
		this->kind = 2;
		break;
	case EnemyType::BIG_ENEMY:
		frameName = "enemy3.png";
	//	frameName = "Boss.png";
		this->m_speed = -BIG_ENEMY_SPEED;
		this->m_score = 8000;
		this->m_hp = BIG_ENEMY_BLOOD;
		this->kind = 3;
		break;
	default:
		break;
	}

	//���ﲻҪ����createWithSpriteFrameName������
	if (!Sprite::initWithFile(frameName)) {
		return false;
	}

	//��л��ķ��ж���
	if (this->m_type == EnemyType::BIG_ENEMY) {
		AudioEngine::play2d("big_spaceship_flying.mp3", 0.4f);

		auto ani = AnimationCache::getInstance()->getAnimation(BIG_ENEMY_FLY_ANIMATION);
		this->runAction(Animate::create(ani));
	}

	return true;
}

Enemy* Enemy::create(const EnemyType& type) {
	auto enemy = new Enemy();
	if (enemy && enemy->initWithEnemyType(type)) {
		enemy->autorelease();//�����Զ�������
		return enemy;
	}

	delete enemy;
	enemy = nullptr;
	return nullptr;
}
void Enemy::down() {
	auto ani = AnimationCache::getInstance()->getAnimation(SMALL_ENEMY_DOWN_ANIMATION);
	switch (m_type)
	{
	case EnemyType::SMALL_ENEMY:
		AudioEngine::play2d("enemy1_down.mp3", false, 0.5f);
	//	ani = AnimationCache::getInstance()->getAnimation(SMALL_ENEMY_DOWN_ANIMATION);
		break;
	case EnemyType::MIDDLE_ENEMY:
		AudioEngine::play2d("enemy2_down.mp3", false,0.5f);
		ani = AnimationCache::getInstance()->getAnimation(MIDDLE_ENEMY_DOWN_ANIMATION);
		break;
	case EnemyType::BIG_ENEMY:
		AudioEngine::play2d("enemy3_down.mp3", false, 0.5f);
		ani = AnimationCache::getInstance()->getAnimation(BIG_ENEMY_DOWN_ANIMATION);
		break;
	default:
		break;
	}
	auto animate = Animate::create(ani);
	//auto seq = Sequence::create(animate, CallFuncN::create([](Node *node) {//˳��ִ�ж���
	//this->removeChild(node);
	//	node->removeFromParentAndCleanup(true);
	//}),nullptr);
	//���ж�������ݲ������Ⱥ�����˳��ִ�У�ע�����һ������������NULL
	auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);

	//ִ���¶���֮ǰ��ֹͣ��������
	this->stopAllActions();
	this->runAction(seq);
}

/*void Enemy::move() {
	this->setPositionY(this->getPositionY() + this->m_speed);
}*/
void Enemy::move() {
	auto chooseEnemy = random(0.1f, 100.1f);
	//�õл�����
	if (chooseEnemy < 0.8f && this->m_type != EnemyType::BIG_ENEMY)
	{
		auto choosePosY = VISIBLE_SIZE.height / 1.2 - random(0.8f, VISIBLE_SIZE.height / 3);
		int  flag = (this->getPositionY() <= choosePosY) ? 1 : 0;
		if (flag)
		{
			auto chooseDir = rand() % 2;
			if (chooseDir)
			{
				this->runAction(MoveBy::create(1.2f, Vec2(this->getPositionX() + 0.6f, -this->getPositionY())));
			}
			else
			{
				this->runAction(MoveBy::create(1.2f, Vec2(-(this->getPositionX() + 0.6f), -this->getPositionY())));
			}

			//this->setPositionX(this->getPositionX() + 10.5f);
		}
		else
		{
			this->setPositionY(this->getPositionY() + m_speed);
		}
	}
	else
	{
		this->setPositionY(this->getPositionY() + m_speed);
	}


}


/*
	���˺�����Ĭ������һ�ε�һ��Ѫ
	@return ��Ѫ���Ƿ�������
*/
bool Enemy::hit() {
	//�Ƿ��Ѿ�������ֱ���˳�
	if (this->m_hp <= 0)  return true;
	this->m_hp--;

	//�˿��Ƿ�������
	if (this->m_hp <= 0)
	{
		this->down();
		return true;
	}
	else if(this->m_type != EnemyType::SMALL_ENEMY)
	{
		//���˶���
		switch (m_type)
		{
		case EnemyType::MIDDLE_ENEMY: {
			auto ani = AnimationCache::getInstance()->getAnimation(MIDDLE_ENEMY_HIT_ANIMATION);
			runAction(Animate::create(ani));
		}
			break;
		case EnemyType::BIG_ENEMY: {
			auto ani = AnimationCache::getInstance()->getAnimation(BIG_ENEMY_HIT_ANIMATION);
			runAction(Animate::create(ani));
		}
			break;
		default:
			break;
		}

	}

	return false;
}