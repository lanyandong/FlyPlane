#pragma once
#include "cocos2d.h"


USING_NS_CC;

enum class EnemyType {
	SMALL_ENEMY,
	MIDDLE_ENEMY,
	BIG_ENEMY
};

class Enemy: public Sprite {
public:
	Enemy() :m_type(EnemyType::SMALL_ENEMY), m_speed(0), m_hp(0), m_score(0) {}
	bool initWithEnemyType(const EnemyType&);
	static Enemy* create(const EnemyType& type);
	void move();
	bool hit();
	void down();

	//移动速度
	CC_SYNTHESIZE(float, m_speed, Speed);
	//生命值
	CC_SYNTHESIZE_READONLY(int, m_hp, HP);
	//奖励分值
	CC_SYNTHESIZE_READONLY(int, m_score, Score);

	/////////********************////////
	CC_SYNTHESIZE_READONLY(int, m_tag, Mtag);
	CC_SYNTHESIZE_READONLY(int, kind, Mkind);


	EnemyType m_type;
	

	///////////
	int size;
};
