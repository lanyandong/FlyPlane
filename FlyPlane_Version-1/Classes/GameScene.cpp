#include "GameScene.h"
#include "Constants.h"
#include "time.h" 
#include "OverScene.h"
#include "AudioEngine.h"

using namespace experimental;

//定义函数：返回类型+（命名）函数名
Scene* GameScene::createScene(){
	return GameScene::create();
}

bool GameScene::init(){
	//1、先调用父类的init
	if (!Scene::init())
	{
		return false;
	}
	
	srand((unsigned int)time(NULL));//将当前时间的秒作为随机数的种子，避免伪随机
	//AudioEngine::play2d("game_music.mp3",true,0.4f);

	/*
	已经使用宏定义
	auto size = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	*/
	
	//加载精灵集文件，将精灵都缓存到精灵集缓存中
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");//背景
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");//飞机
	

	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	//设置定位点位置在自身的左下角
	bg1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//开启抗锯齿功能
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BACKGROUND_ZORDER, BACKGROUND_TAG_1);

	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bg2->setPositionY(bg1->getContentSize().height); //将bg2连接到bg1的后面
	//开启抗锯齿功能
	bg2->getTexture()->setAliasTexParameters();
	this->addChild(bg2, BACKGROUND_ZORDER, BACKGROUND_TAG_2);

	//加载飞机精灵集文件，将精灵都缓存到精灵集缓存中
	auto hero = Sprite::createWithSpriteFrameName("hero1.png");
	hero->setPosition(VISIBLE_ORIGIN.x + VISIBLE_SIZE.width / 2, VISIBLE_ORIGIN.y + hero->getContentSize().height / 2);
	this->addChild(hero, FOERGROUND_ZORDER, HERO_TAG);
	
	/*******已经将动画添加到缓存了*******/
	/*//给飞机添加飞行动画
	//1.创建动画
	//a.创建动画对象
	auto ani = Animation::create();
	//b.添加动画帧（从精灵帧缓存中根据名字查找出来）
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	//c.设置动画切换时长
	ani->setDelayPerUnit(0.2f);
	//d.设置循环次数，使用宏标识无限次，可以使用-1
	ani->setLoops(CC_REPEAT_FOREVER);*/
	
	//使用动画缓存
	auto ani = AnimationCache::getInstance()->getAnimation(HERO_FLY_ANIMATION);
	//将动画封装为动作
	auto animate = Animate::create(ani);
	//精灵运行动作
	hero->runAction(animate);
	
		
	//添加触摸事件的处理
	//1.创建一个事件的监听对象
	auto listener = EventListenerTouchOneByOne::create();
	
	//2.分解事件，处理逻辑
	/****lambad表达式，用语句表示函数，也就是允许函数里面写函数
	*****也可以写一个[=]等号，表示外部所有变量都按值传递进来，可以访问了，但不能修改
	*****还可以写一个[&]地址符，表示外部所有变量都按引用传递进来，不仅可以访问，还可以修改外部变量本身的值
	*****/
	//a.触摸开始时，lambad表达式，
	listener ->onTouchBegan = [=](Touch* t, Event* e){
		Vec2 touchPos = t->getLocation();
		log("position is x:%.2f,y:%:0.2f", touchPos.x, touchPos.y);
		
		/*  简单的移动实现，但是会出现闪现情况
		//hero->setPosition(touchPos);
		auto move = MoveTo::create(0.5f, touchPos);
		hero->runAction(move);
		*/

		//避免闪现移动，使用m_offset记录飞机和触摸点的偏移量
		this ->m_offset = hero->getPosition() - touchPos;
		bool isContains = hero->getBoundingBox().containsPoint(touchPos); //使用bool变量判断触摸点是否在hero上
		return isContains && !Director::getInstance()->isPaused() && ! this->m_isOver;;//判断触摸点是否在hero上并且没有暂停并且游戏没有结束
	};

	//b.持续触摸并移动
	listener->onTouchMoved = [=](Touch* t, Event* e){
		if (Director::getInstance()->isPaused() || this->m_isOver) return;  
		//避免飞机飞出屏幕外返回不跟随鼠标
		hero ->setPosition( t->getLocation() + m_offset);

		//控制飞机的活动范围
		auto minX = hero->getContentSize().width / 2;
		auto maxX = VISIBLE_SIZE.width - minX;
		auto minY = hero->getContentSize().height / 2;
		auto maxY = VISIBLE_SIZE.height - minY;
		
		//使用宏，限制飞机的活动范围
		auto x = MAX(minX, MIN(maxX, hero->getPositionX())); 
		auto y = MAX(minY, MIN(maxY, hero->getPositionY())); 
		hero->setPosition(x, y);
	};

	//c.触摸结束
	listener->onTouchEnded = [](Touch* t, Event* e){
		log("Touch Ended...");
	};

	//3.注册监听到分发器
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, hero);



	/////////UI,分数显示
	auto lblScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", this->m_totalScore));
	lblScore->setPosition(lblScore->getContentSize().width, VISIBLE_SIZE.height - lblScore->getContentSize().height);
	this->addChild(lblScore, UI_ZORDER, LABEL_SCORE_TAG);
	lblScore->setColor(Color3B(255,0,0));
	lblScore->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	/////////炸弹菜单
	auto spBomb = Sprite::createWithSpriteFrameName("bomb.png");
	auto itemBomb = MenuItemSprite::create(spBomb, spBomb, [this, lblScore](Ref*){
		if (Director::getInstance()->isPaused()) return;
		for (auto enemy : this->m_enemies){
			enemy->down();
			m_totalScore += enemy->getScore();	
		}
		lblScore->setString(StringUtils::format("%d", this->m_totalScore));//爆炸后更新分数
		this->m_enemies.clear();
		this->m_bombCount--;
		this->changeBomb(); //更新炸弹在UI界面的显示
		AudioEngine::play2d("use_bomb.mp3");
	});
	itemBomb->setPosition(itemBomb->getContentSize());

	////////暂停菜单,toggle即为触碰键
	auto spPauseNormal = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	auto spPauseSelected = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto spResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");
	auto spResumeSelected = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto itemPause = MenuItemSprite::create(spPauseNormal, spPauseSelected);
	auto itemResume = MenuItemSprite::create(spResumeNormal, spResumeSelected);
	auto toggle = MenuItemToggle::createWithCallback( [](Ref *sender){
		//获取当前选择项的下标
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


	auto menu = Menu::create();//必须添加菜单这一层（将itemBomb 和 toggle 都添加都菜单上响应）
	menu->addChild(itemBomb, 1, ITEM_BOMB_TAG); //menu->addChild(),需要对象、层数、标签
	menu->addChild(toggle, 1, TOGGLE_TAG);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, UI_ZORDER, MENU_TAG);
	
	//炸弹的数量显示，使用Label
	auto lblBomb = Label::createWithBMFont("font.fnt", StringUtils::format("X%d", this->m_bombCount));
	lblBomb->setPosition(itemBomb->getPosition() + Vec2(40, 0));
	this->addChild(lblBomb,UI_ZORDER, LABEL_BOMB_TAG);
	lblBomb->setColor(Color3B::BLACK);
	lblBomb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

	//初始化菜单项和标签的显示
	this->changeBomb();


	//每帧调用一次Update函数
	scheduleUpdate();

	schedule(schedule_selector(GameScene::createBullet), CREATE_BULLET_INTERVAL);//OC语法，可以使用lambad语法
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
	//让背景图2跟随背景1移动（取背景1的位置上加背景的高度）
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
	if (bg2->getPositionY() <= 0) //背景滚动，其实是图片跳动
	{
		bg1->setPositionY(0);
	}

	//遍历子弹集合，使用 foreach 语句
	//创建一个临时集合存放
	Vector<Sprite *> removableBullets;
	for (auto bullet : m_bullets)  
	{
		//子弹滚动
		bullet->setPositionY(bullet->getPositionY() + BULLET_SPEED);
		//检查子弹是否超出屏幕
		if (bullet->getPositionY() >= VISIBLE_SIZE.height) 
		{
			this->removeChild(bullet);//将子弹从渲染中移除，内存中没有移除
			//不能在遍历集合时修改集合的成员数量，所以把无效的子弹存放到临时集合中，最后在移除
			removableBullets.pushBack(bullet);	
		}
	}
	
	//类似于处理子弹的方法处理敌机
	//创建临时集合存放，使用 foreach循环
	Vector<Enemy *> removableEnemies;
	for (auto enemy : m_enemies)  
	{
		enemy->move();
		//检查敌机是否超出屏幕
		if (enemy->getPositionY() <= -enemy->getContentSize().height / 2) 
		{
			this->removeChild(enemy);
			removableEnemies.pushBack(enemy);
		}	
	}

	//碰撞检测 循环嵌套
	for (auto enemy : m_enemies)
	{
		//////游戏等级加速
		enemy->setSpeed(enemy->getSpeed() - Round/3);


		for (auto bullet : m_bullets)
		{
			//判断两个矩形是否相交（即子弹和敌机是否碰撞)
			if (enemy ->getBoundingBox().intersectsRect(bullet->getBoundingBox())) 
			{
				//需要判断死亡，死亡才移除
				if (enemy->hit())
				{
					removableEnemies.pushBack(enemy);
					//更新分数
					m_totalScore += enemy->getScore();
					auto nodeScore = getChildByTag(LABEL_SCORE_TAG);
					//需要动态强转
					auto lblScore = dynamic_cast<Label *>(nodeScore);
					std::string strScore = StringUtils::format("%d", m_totalScore);
					lblScore->setString(strScore);
				}
				removableBullets.pushBack(bullet);
				this->removeChild(bullet);
			}
		}
		//与飞机进行碰撞检测
		if (enemy->getBoundingBox().intersectsRect(hero->getBoundingBox()))
		{
			/***将游戏结束封装为一个函数，包括（飞机销毁，界面停止，按钮不响应）*****/
			this->gameOver();
			enemy->down();
			removableEnemies.pushBack(enemy);
		}
	}

	//删除超出屏幕外的子弹
	//log("%d", m_bullets.size()); //测试是否是真的移除，两次遍历把无效子弹移除
	for (auto bullet : removableBullets)
	{
		m_bullets.eraseObject(bullet);
	}
	removableBullets.clear();

	//////删除超出屏幕外的的敌机
	for (auto enemy : removableEnemies)
	{
		m_enemies.eraseObject(enemy);
	}
	removableEnemies.clear();

	//////空投滚动,获取，移除
	/////使用临时集合存放
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
				//拾取到炸弹道具
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
		//创建双发子弹
		auto bulletL = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletL->setPosition(hero->getPosition() + Vec2(-hero->getContentSize().width / 3 , hero->getContentSize().height / 3));
		this->addChild(bulletL, FOERGROUND_ZORDER);
		m_bullets.pushBack(bulletL);
	
		auto bulletR = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletR->setPosition(hero->getPosition() + Vec2(hero->getContentSize().width / 3 , hero->getContentSize().height / 3));
		this->addChild(bulletR, FOERGROUND_ZORDER);
		//将新建的子弹加入集合
		m_bullets.pushBack(bulletR);
		this->m_doubleBulletsCount--; //消耗双子单
	}
	else
	{
		//创建单发子弹
		//从精灵帧缓存中读取子弹，并且给子弹定位
		auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
		bullet->setPosition(hero->getPositionX(), hero->getPositionY() + hero->getContentSize().height / 2);
		this->addChild(bullet, FOERGROUND_ZORDER);
		//将新建的子弹加入集合
		m_bullets.pushBack(bullet);
	}
}


void GameScene::createEnemy(const EnemyType& type){
	//创建敌机集合，设置敌机出现的位置，使用了随机数，让敌机出现的位置随机
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





/********竖排小敌机************/
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

/********横排小敌机*********/
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

/***********小敌机阵容*************/
void GameScene::createEnemyGruop(Vec2 vec){
	
	/***第一架飞机的出现位置随机 + 范围确定 ***/
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


/***********中型敌机阵容*************/
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


/***********大敌机 + 小敌机阵容*************/
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




//***********小敌机环绕大敌机*************//
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


//////////创建空投
void GameScene::createUfo(float){
	//auto frameName = StringUtils::format("ufo%d.png", rand()%2 + 1); //两句效果一样
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
		//场景跳转
		Director::getInstance()->replaceScene(scene);
	}), nullptr);
	hero->runAction(seq);
	this->unscheduleAllCallbacks();

	//7.禁用暂停菜单
	auto menu = this->getChildByTag(MENU_TAG);
	auto toggle = menu->getChildByTag(TOGGLE_TAG);
	dynamic_cast<MenuItemToggle*>(toggle)->setEnabled(false);
	AudioEngine::play2d("game_over.mp3");
}

