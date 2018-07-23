#pragma once
//避免重复定义

#include "cocos2d.h"

USING_NS_CC;

//枚举类
enum class EnemyType{
	SMALL_ENEMY,
	MIDDLE_ENEMY,
	BIG_ENEMY
};

class Enemy : public Sprite{
public:
	Enemy() :m_type(EnemyType::SMALL_ENEMY), m_speed(0), m_hp(0), m_score(0){};
	bool initWithEnemyType(const EnemyType&);
	static Enemy* create(const EnemyType& type);
	void move();
	bool hit();	//敌机是否销毁
	void down(); //敌机爆炸动画

	//移动速度，一个属性，两种方法
	CC_SYNTHESIZE(float, m_speed, Speed);
	
	//生命值
	CC_SYNTHESIZE_READONLY(int, m_hp, HP);

	//奖励分值
	CC_SYNTHESIZE_READONLY(int, m_score, Score);

private:
	EnemyType m_type;
	//float m_speed;
};