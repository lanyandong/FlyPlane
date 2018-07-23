#include "GameScene.h"
#include "Constants.h"
#include "time.h" 
#include "OverScene.h"
#include "AudioEngine.h"

using namespace experimental;

//���庯������������+��������������
Scene* GameScene::createScene(){
	return GameScene::create();
}

bool GameScene::init(){
	//1���ȵ��ø����init
	if (!Scene::init())
	{
		return false;
	}
	
	srand((unsigned int)time(NULL));//����ǰʱ�������Ϊ����������ӣ�����α���
	//AudioEngine::play2d("game_music.mp3",true,0.4f);

	/*
	�Ѿ�ʹ�ú궨��
	auto size = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	*/
	
	//���ؾ��鼯�ļ��������鶼���浽���鼯������
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");//����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");//�ɻ�
	

	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	//���ö�λ��λ������������½�
	bg1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//��������ݹ���
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BACKGROUND_ZORDER, BACKGROUND_TAG_1);

	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bg2->setPositionY(bg1->getContentSize().height); //��bg2���ӵ�bg1�ĺ���
	//��������ݹ���
	bg2->getTexture()->setAliasTexParameters();
	this->addChild(bg2, BACKGROUND_ZORDER, BACKGROUND_TAG_2);

	//���طɻ����鼯�ļ��������鶼���浽���鼯������
	auto hero = Sprite::createWithSpriteFrameName("hero1.png");
	hero->setPosition(VISIBLE_ORIGIN.x + VISIBLE_SIZE.width / 2, VISIBLE_ORIGIN.y + hero->getContentSize().height / 2);
	this->addChild(hero, FOERGROUND_ZORDER, HERO_TAG);
	
	/*******�Ѿ���������ӵ�������*******/
	/*//���ɻ���ӷ��ж���
	//1.��������
	//a.������������
	auto ani = Animation::create();
	//b.��Ӷ���֡���Ӿ���֡�����и������ֲ��ҳ�����
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	//c.���ö����л�ʱ��
	ani->setDelayPerUnit(0.2f);
	//d.����ѭ��������ʹ�ú��ʶ���޴Σ�����ʹ��-1
	ani->setLoops(CC_REPEAT_FOREVER);*/
	
	//ʹ�ö�������
	auto ani = AnimationCache::getInstance()->getAnimation(HERO_FLY_ANIMATION);
	//��������װΪ����
	auto animate = Animate::create(ani);
	//�������ж���
	hero->runAction(animate);
	
		
	//��Ӵ����¼��Ĵ���
	//1.����һ���¼��ļ�������
	auto listener = EventListenerTouchOneByOne::create();
	
	//2.�ֽ��¼��������߼�
	/****lambad���ʽ��������ʾ������Ҳ��������������д����
	*****Ҳ����дһ��[=]�Ⱥţ���ʾ�ⲿ���б�������ֵ���ݽ��������Է����ˣ��������޸�
	*****������дһ��[&]��ַ������ʾ�ⲿ���б����������ô��ݽ������������Է��ʣ��������޸��ⲿ���������ֵ
	*****/
	//a.������ʼʱ��lambad���ʽ��
	listener ->onTouchBegan = [=](Touch* t, Event* e){
		Vec2 touchPos = t->getLocation();
		log("position is x:%.2f,y:%:0.2f", touchPos.x, touchPos.y);
		
		/*  �򵥵��ƶ�ʵ�֣����ǻ�����������
		//hero->setPosition(touchPos);
		auto move = MoveTo::create(0.5f, touchPos);
		hero->runAction(move);
		*/

		//���������ƶ���ʹ��m_offset��¼�ɻ��ʹ������ƫ����
		this ->m_offset = hero->getPosition() - touchPos;
		bool isContains = hero->getBoundingBox().containsPoint(touchPos); //ʹ��bool�����жϴ������Ƿ���hero��
		return isContains && !Director::getInstance()->isPaused() && ! this->m_isOver;;//�жϴ������Ƿ���hero�ϲ���û����ͣ������Ϸû�н���
	};

	//b.�����������ƶ�
	listener->onTouchMoved = [=](Touch* t, Event* e){
		if (Director::getInstance()->isPaused() || this->m_isOver) return;  
		//����ɻ��ɳ���Ļ�ⷵ�ز��������
		hero ->setPosition( t->getLocation() + m_offset);

		//���Ʒɻ��Ļ��Χ
		auto minX = hero->getContentSize().width / 2;
		auto maxX = VISIBLE_SIZE.width - minX;
		auto minY = hero->getContentSize().height / 2;
		auto maxY = VISIBLE_SIZE.height - minY;
		
		//ʹ�ú꣬���Ʒɻ��Ļ��Χ
		auto x = MAX(minX, MIN(maxX, hero->getPositionX())); 
		auto y = MAX(minY, MIN(maxY, hero->getPositionY())); 
		hero->setPosition(x, y);
	};

	//c.��������
	listener->onTouchEnded = [](Touch* t, Event* e){
		log("Touch Ended...");
	};

	//3.ע��������ַ���
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, hero);



	/////////UI,������ʾ
	auto lblScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", this->m_totalScore));
	lblScore->setPosition(lblScore->getContentSize().width, VISIBLE_SIZE.height - lblScore->getContentSize().height);
	this->addChild(lblScore, UI_ZORDER, LABEL_SCORE_TAG);
	lblScore->setColor(Color3B(255,0,0));
	lblScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	/////////ը���˵�
	auto spBomb = Sprite::createWithSpriteFrameName("bomb.png");
	auto itemBomb = MenuItemSprite::create(spBomb, spBomb, [this, lblScore](Ref*){
		if (Director::getInstance()->isPaused()) return;
		for (auto enemy : this->m_enemies){
			enemy->down();
			m_totalScore += enemy->getScore();	
		}
		lblScore->setString(StringUtils::format("%d", this->m_totalScore));//��ը����·���
		this->m_enemies.clear();
		this->m_bombCount--;
		this->changeBomb(); //����ը����UI�������ʾ
		AudioEngine::play2d("use_bomb.mp3");
	});
	itemBomb->setPosition(itemBomb->getContentSize());

	////////��ͣ�˵�,toggle��Ϊ������
	auto spPauseNormal = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	auto spPauseSelected = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto spResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");
	auto spResumeSelected = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto itemPause = MenuItemSprite::create(spPauseNormal, spPauseSelected);
	auto itemResume = MenuItemSprite::create(spResumeNormal, spResumeSelected);
	auto toggle = MenuItemToggle::createWithCallback( [](Ref *sender){
		//��ȡ��ǰѡ������±�
		int index = dynamic_cast<MenuItemToggle *>(sender)->getSelectedIndex();
		if (index)
		{
			Director::getInstance()->pause();
		}
		else
		{
			Director::getInstance()->resume();
		}
		AudioEngine::play2d("button.mp3");
	},itemPause,itemResume, nullptr);
	toggle->setPosition(VISIBLE_SIZE - toggle->getContentSize());


	auto menu = Menu::create();//������Ӳ˵���һ�㣨��itemBomb �� toggle ����Ӷ��˵�����Ӧ��
	menu->addChild(itemBomb, 1, ITEM_BOMB_TAG); //menu->addChild(),��Ҫ���󡢲�������ǩ
	menu->addChild(toggle, 1, TOGGLE_TAG);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, UI_ZORDER, MENU_TAG);
	
	//ը����������ʾ��ʹ��Label
	auto lblBomb = Label::createWithBMFont("font.fnt", StringUtils::format("X%d", this->m_bombCount));
	lblBomb->setPosition(itemBomb->getPosition() + Vec2(40, 0));
	this->addChild(lblBomb,UI_ZORDER, LABEL_BOMB_TAG);
	lblBomb->setColor(Color3B::BLACK);
	lblBomb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

	//��ʼ���˵���ͱ�ǩ����ʾ
	this->changeBomb();


	//ÿ֡����һ��Update����
	scheduleUpdate();

	schedule(schedule_selector(GameScene::createBullet), CREATE_BULLET_INTERVAL);//OC�﷨������ʹ��lambad�﷨
	schedule(schedule_selector(GameScene::createSmallEnemy), CREATE_SMALL_ENEMY_INTERVAL,CC_REPEAT_FOREVER,CREATE_SMALL_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createMiddleEnemy), CREATE_MIDDLE_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_MIDDLE_ENEMY_DELAY);
	//schedule(schedule_selector(GameScene::createBigEnemy), CREATE_BIG_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_BIG_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createUfo), CREATE_UFO_INTERVAL, CC_REPEAT_FOREVER, CREATE_UFO_DELAY);

	schedule(schedule_selector(GameScene::EnemyGroup), CREATE_ENEMY_GROUP_INTERVAL, CC_REPEAT_FOREVER, CREATE_ENEMY_GROUP_DELAY);
	//schedule(schedule_selector(GameScene::createEnemyGruop1), CREATE_ENEMY_GROUP_1_INTERVAL, CC_REPEAT_FOREVER, CREATE_ENEMY_GROUP_1_DELAY);
	//schedule(schedule_selector(GameScene::createEnemyGruop2), CREATE_ENEMY_GROUP_2_INTERVAL, CC_REPEAT_FOREVER, CREATE_ENEMY_GROUP_2_DELAY);

	return true;
}


void GameScene::update(float delta){
	auto bg1 = this->getChildByTag(BACKGROUND_TAG_1);
	auto bg2 = this->getChildByTag(BACKGROUND_TAG_2);
	auto hero = getChildByTag(HERO_TAG);
	bg1->setPositionY(bg1->getPositionY() - BACKGROUND_SPEED);
	//�ñ���ͼ2���汳��1�ƶ���ȡ����1��λ���ϼӱ����ĸ߶ȣ�
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
	if (bg2->getPositionY() <= 0) //������������ʵ��ͼƬ����
	{
		bg1->setPositionY(0);
	}

	//�����ӵ����ϣ�ʹ�� foreach ���
	//����һ����ʱ���ϴ��
	Vector<Sprite *> removableBullets;
	for (auto bullet : m_bullets)  
	{
		//�ӵ�����
		bullet->setPositionY(bullet->getPositionY() + BULLET_SPEED);
		//����ӵ��Ƿ񳬳���Ļ
		if (bullet->getPositionY() >= VISIBLE_SIZE.height) 
		{
			this->removeChild(bullet);//���ӵ�����Ⱦ���Ƴ����ڴ���û���Ƴ�
			//�����ڱ�������ʱ�޸ļ��ϵĳ�Ա���������԰���Ч���ӵ���ŵ���ʱ�����У�������Ƴ�
			removableBullets.pushBack(bullet);	
		}
	}
	
	//�����ڴ����ӵ��ķ�������л�
	//������ʱ���ϴ�ţ�ʹ�� foreachѭ��
	Vector<Enemy *> removableEnemies;
	for (auto enemy : m_enemies)  
	{
		enemy->move();
		//���л��Ƿ񳬳���Ļ
		if (enemy->getPositionY() <= -enemy->getContentSize().height / 2) 
		{
			this->removeChild(enemy);
			removableEnemies.pushBack(enemy);
		}	
	}

	//��ײ��� ѭ��Ƕ��
	for (auto enemy : m_enemies)
	{
		//////��Ϸ�ȼ�����
		enemy->setSpeed(enemy->getSpeed() - Round/3);


		for (auto bullet : m_bullets)
		{
			//�ж����������Ƿ��ཻ�����ӵ��͵л��Ƿ���ײ)
			if (enemy ->getBoundingBox().intersectsRect(bullet->getBoundingBox())) 
			{
				//��Ҫ�ж��������������Ƴ�
				if (enemy->hit())
				{
					removableEnemies.pushBack(enemy);
					//���·���
					m_totalScore += enemy->getScore();
					auto nodeScore = getChildByTag(LABEL_SCORE_TAG);
					//��Ҫ��̬ǿת
					auto lblScore = dynamic_cast<Label *>(nodeScore);
					std::string strScore = StringUtils::format("%d", m_totalScore);
					lblScore->setString(strScore);
				}
				removableBullets.pushBack(bullet);
				this->removeChild(bullet);
			}
		}
		//��ɻ�������ײ���
		if (enemy->getBoundingBox().intersectsRect(hero->getBoundingBox()))
		{
			/***����Ϸ������װΪһ���������������ɻ����٣�����ֹͣ����ť����Ӧ��*****/
			this->gameOver();
			enemy->down();
			removableEnemies.pushBack(enemy);
		}
	}

	//ɾ��������Ļ����ӵ�
	//log("%d", m_bullets.size()); //�����Ƿ�������Ƴ������α�������Ч�ӵ��Ƴ�
	for (auto bullet : removableBullets)
	{
		m_bullets.eraseObject(bullet);
	}
	removableBullets.clear();

	//////ɾ��������Ļ��ĵĵл�
	for (auto enemy : removableEnemies)
	{
		m_enemies.eraseObject(enemy);
	}
	removableEnemies.clear();

	//////��Ͷ����,��ȡ���Ƴ�
	/////ʹ����ʱ���ϴ��
	Vector<Sprite *> removableUfos; 
	for (auto ufo : m_ufos)
	{
		if (ufo->getPositionY() <= 0)
		{
			this->removeChild(ufo);
			removableUfos.pushBack(ufo);
		}
	}
	for (auto ufo : m_ufos)
	{
		if (ufo->getBoundingBox().intersectsRect(hero->getBoundingBox()))
		{
			this->removeChild(ufo);
			removableUfos.pushBack(ufo);
			if (this -> m_ufoType)
			{
				AudioEngine::play2d("get_double_laser.mp3");
				this->m_doubleBulletsCount = FULL_DOUBLE_BULLET_COUNT;
			}
		
			else if (this->m_bombCount < 3)
			{
				//ʰȡ��ը������
				AudioEngine::play2d("get_bomb.mp3");
				this->m_bombCount++;
				this->changeBomb();	
			}
		}
	}
	for (auto ufo : removableUfos)
	{
		m_ufos.eraseObject(ufo);
	}
	removableUfos.clear();

}


void GameScene::createBullet(float){
	AudioEngine::play2d("bullet.mp3");
	auto hero = getChildByTag(HERO_TAG);
	if (m_doubleBulletsCount > 0)
	{
		//����˫���ӵ�
		auto bulletL = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletL->setPosition(hero->getPosition() + Vec2(-hero->getContentSize().width / 3 , hero->getContentSize().height / 3));
		this->addChild(bulletL, FOERGROUND_ZORDER);
		m_bullets.pushBack(bulletL);
	
		auto bulletR = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletR->setPosition(hero->getPosition() + Vec2(hero->getContentSize().width / 3 , hero->getContentSize().height / 3));
		this->addChild(bulletR, FOERGROUND_ZORDER);
		//���½����ӵ����뼯��
		m_bullets.pushBack(bulletR);
		this->m_doubleBulletsCount--; //����˫�ӵ�
	}
	else
	{
		//���������ӵ�
		//�Ӿ���֡�����ж�ȡ�ӵ������Ҹ��ӵ���λ
		auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
		bullet->setPosition(hero->getPositionX(), hero->getPositionY() + hero->getContentSize().height / 2);
		this->addChild(bullet, FOERGROUND_ZORDER);
		//���½����ӵ����뼯��
		m_bullets.pushBack(bullet);
	}
}


void GameScene::createEnemy(const EnemyType& type){
	//�����л����ϣ����õл����ֵ�λ�ã�ʹ������������õл����ֵ�λ�����
	//auto enemy = Sprite::createWithSpriteFrameName("enemy1.png");
	auto enemy = Enemy::create(type);

	float minX = enemy->getContentSize().width / 2;
	float maxX = VISIBLE_SIZE.width - minX;
	float x = rand() % (int)(maxX - minX + 1) + minX;
	enemy->setPosition(x, VISIBLE_SIZE.height + enemy->getContentSize().height / 2);
	this->addChild(enemy, FOERGROUND_ZORDER);
	m_enemies.pushBack(enemy);
}


void GameScene::createSmallEnemy(float){
	this->createEnemy(EnemyType::SMALL_ENEMY);
}

void GameScene::createMiddleEnemy(float){
	this->createEnemy(EnemyType::MIDDLE_ENEMY);
}

void GameScene::createBigEnemy(float){
	this->createEnemy(EnemyType::BIG_ENEMY);
	AudioEngine::play2d("big_spaceship_flying.mp3");


}





/********����С�л�************/
void GameScene::createSmallEnemyGruop1(Vec2 vec, int speed){
	Vector<Enemy *> m_enemygroup;
	for (auto i = 0; i < 5; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height + enemy->getContentSize().height * i);
		m_enemygroup.pushBack(enemy);

	}
	for (auto enemy : m_enemygroup)
	{
		enemy->setSpeed(speed);
		enemy->setPosition(enemy->getPosition() + vec);
		this->addChild(enemy);
		m_enemies.pushBack(enemy);

	}
	m_enemygroup.clear();
}

/********����С�л�*********/
void GameScene::createSmallEnemyGruop2(Vec2 vec, int speed){
	Vector<Enemy *> m_enemygroup;
	for (auto i = 0; i < 6; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width / 2 + enemy->getContentSize().width * i, VISIBLE_SIZE.height);
		m_enemygroup.pushBack(enemy);
	}
	for (auto enemy : m_enemygroup)
	{
		enemy->setSpeed(speed);
		enemy->setPosition(enemy->getPosition() + vec);
		this->addChild(enemy);
		m_enemies.pushBack(enemy);
	}
	m_enemygroup.clear();
}

/***********С�л�����*************/
void GameScene::createEnemyGruop(Vec2 vec){
	
	/***��һ�ܷɻ��ĳ���λ����� + ��Χȷ�� ***/
	Vector<Enemy *> m_enemygroup;
	for (auto i = 0; i < 5; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width / 2 + enemy->getContentSize().width / 2 * i, VISIBLE_SIZE.height + enemy->getContentSize().height  * i);
		m_enemygroup.pushBack(enemy);	
	}
	for (auto i = 0; i < 5; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width / 2 - enemy->getContentSize().width / 2 * i, VISIBLE_SIZE.height + enemy->getContentSize().height  * i);
		m_enemygroup.pushBack(enemy);

	}
	for (auto enemy : m_enemygroup)
	{
		enemy->setPosition(enemy->getPosition() + vec );
		this->addChild(enemy);
		m_enemies.pushBack(enemy);
	}
	m_enemygroup.clear();
}


/***********���͵л�����*************/
void GameScene::createEnemyGruop1(Vec2 vec){
	Vector<Enemy *> m_enemygroup;
	for (auto i = 0; i < 2; i++)
	{
		auto enemy = Enemy::create(EnemyType::MIDDLE_ENEMY);
		enemy->setPosition(-enemy->getContentSize().width / 2 - enemy->getContentSize().width * i, VISIBLE_SIZE.height / 6 * 5);
		m_enemygroup.pushBack(enemy);
		auto move1 = MoveBy::create(3.0f, Vec2( VISIBLE_SIZE.width / 2, 0));
		enemy->runAction(move1);
	}

	for (auto i = 0; i < 2; i++)
	{
		auto enemy = Enemy::create(EnemyType::MIDDLE_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width + enemy->getContentSize().width / 2 + enemy->getContentSize().width * i, VISIBLE_SIZE.height / 6 * 5);
		m_enemygroup.pushBack(enemy);
		auto move2 = MoveBy::create(3.0f, Vec2(- VISIBLE_SIZE.width / 2, 0));
		enemy->runAction(move2);
	}

	for (auto enemy : m_enemygroup)
	{
		enemy->setPosition(enemy->getPosition() + vec);
		this->addChild(enemy);
		m_enemies.pushBack(enemy);
	}
	m_enemygroup.clear();
}


/***********��л� + С�л�����*************/
void GameScene::createEnemyGruop2(Vec2 vec){
	Vector<Enemy *> m_enemygroup;

	auto bigenemy = Enemy::create(EnemyType::BIG_ENEMY);
	bigenemy->setPosition(VISIBLE_SIZE.width /2 , VISIBLE_SIZE.height + bigenemy->getContentSize().height / 2);
	m_enemygroup.pushBack(bigenemy);
	for (auto i = 0; i < 2; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(bigenemy->getPositionX() - bigenemy->getContentSize().width / 2 - enemy->getContentSize().width / 2, bigenemy->getPositionY() - bigenemy->getContentSize().height / 2 + enemy->getContentSize().height * i * 4);
		m_enemygroup.pushBack(enemy);

	}
	for (auto i = 0; i < 2; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(bigenemy->getPositionX() + bigenemy->getContentSize().width / 2 + enemy->getContentSize().width / 2, bigenemy->getPositionY() - bigenemy->getContentSize().height / 2 + enemy->getContentSize().height * i * 4);
		m_enemygroup.pushBack(enemy);
	}
	for (auto enemy : m_enemygroup)
	{
		enemy->setSpeed(-0.5);
		enemy->setPosition(enemy->getPosition() + vec);
		this->addChild(enemy);
		m_enemies.pushBack(enemy);

	}
	m_enemygroup.clear();
}




//***********С�л����ƴ�л�*************//
void GameScene::createEnemyGruop3(Vec2 vec){
	Vector<Enemy *> m_enemygroup;

	auto bigenemy = Enemy::create(EnemyType::BIG_ENEMY);
	bigenemy->setPosition(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height);
	m_enemygroup.pushBack(bigenemy);

	createSmallEnemyGruop1(Vec2(-bigenemy->getContentSize().width / 2 - 30, -100), -1);
	createSmallEnemyGruop1(Vec2(+bigenemy->getContentSize().width / 2 + 30, -100), -1);
	createSmallEnemyGruop2(Vec2(-130, -160), -1);

	for (auto enemy : m_enemygroup)
	{
		enemy->setSpeed(-1);
		enemy->setPosition(enemy->getPosition() + vec);
		this->addChild(enemy);
		m_enemies.pushBack(enemy);

	}
	m_enemygroup.clear();

}


/////////////////////////////
void GameScene::EnemyGroup(float){
	if (GroupFlag == 0)
	{
	
		this->createEnemyGruop(Vec2(0,0));
		this->createEnemyGruop(Vec2(0,200));

	}
	else if (GroupFlag == 1){
		this->createEnemyGruop1(Vec2(0, 0));
		this->createEnemyGruop1(Vec2(0, 200));
	}
	else if (GroupFlag == 2){
		this->createEnemyGruop2(Vec2(0, 0));

	}
	else if (GroupFlag == 3){
		this->createEnemyGruop3(Vec2(0, 0));
	}
	else{
		GroupFlag = -1;
		Round++;
		if (Round == 3)
		{
			Round = 0;
		}
	}
	GroupFlag++;
	
}


//////////������Ͷ
void GameScene::createUfo(float){
	//auto frameName = StringUtils::format("ufo%d.png", rand()%2 + 1); //����Ч��һ��
	this->m_ufoType = rand() % 2;
	std::string frameName = m_ufoType ? "ufo1.png" : "ufo2.png";
	auto  ufo = Sprite::createWithSpriteFrameName(frameName);
	float minX = ufo->getContentSize().width / 2;
	float maxX = VISIBLE_SIZE.width - minX;
	float x = rand() % (int)(maxX - minX + 1) + minX;
	ufo->setPosition(x, VISIBLE_SIZE.height + ufo->getContentSize().height / 2);
	this->addChild(ufo, FOERGROUND_ZORDER, UFO_TAG);
	m_ufos.pushBack(ufo);
	auto move1 = MoveBy::create(2, Vec2(0, -200));
	auto move2 = MoveTo::create(1.5f, Vec2(ufo->getPositionX(), -ufo->getContentSize().height) );
	ufo->runAction(Sequence::create(move1,move1->reverse(),move2,nullptr));
	AudioEngine::play2d("out_porp.mp3");
}

void GameScene::changeBomb(){
	auto menu =this->getChildByTag(MENU_TAG);
	auto itemBomb = menu->getChildByTag(ITEM_BOMB_TAG);
	auto lblBomb = this->getChildByTag(LABEL_BOMB_TAG);
	if (this->m_bombCount <= 0)
	{
		itemBomb->setVisible(false);
		lblBomb->setVisible(false);
	}
	else if (this->m_bombCount == 1)
	{
		itemBomb->setVisible(true);
		lblBomb->setVisible(false);

	}
	else
	{
		itemBomb->setVisible(true);
		lblBomb->setVisible(true);
		dynamic_cast<Label*>(lblBomb)->setString(StringUtils::format("X%d", this->m_bombCount));
	}
}


void GameScene::gameOver(){
	auto hero = this->getChildByTag(HERO_TAG);
	this->m_isOver = true;

	for (auto node : this->getChildren()){
		node->stopAllActions();
	}
	auto ani = AnimationCache::getInstance()->getAnimation(HERO_DOWN_ANIMATION);
	auto seq = Sequence::create(Animate::create(ani), CallFuncN::create([this](Node *node){
		auto scene = OverScene::createScene(this->m_totalScore);
		//������ת
		Director::getInstance()->replaceScene(scene);
	}), nullptr);
	hero->runAction(seq);
	this->unscheduleAllCallbacks();

	//7.������ͣ�˵�
	auto menu = this->getChildByTag(MENU_TAG);
	auto toggle = menu->getChildByTag(TOGGLE_TAG);
	dynamic_cast<MenuItemToggle*>(toggle)->setEnabled(false);
	AudioEngine::play2d("game_over.mp3");
}

