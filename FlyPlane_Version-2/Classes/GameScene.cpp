#include "GameScene.h"
#include "Constants.h"
#include "time.h"
#include "OverScene.h"
#include "AudioEngine.h"
#include "Enemy.h"

using namespace experimental;


Scene* GameScene::createScene() {
	return GameScene::create();
}

bool GameScene::init() {
	//1.先调用父类的init
	if (!Scene::init()) {
		return false;
	}

	srand((unsigned int)time(NULL));

	AudioEngine::play2d("game_music.mp3", true, 0.4f);

	//通过加载精灵集文件，将精灵都缓存到精灵集缓存中

	/////////////////////////////////背景
	auto bg1 = Sprite::create("background1.jpg");
	//auto bg1 = Sprite::createWithSpriteFrameName("background.png");

	//bg1->setPosition(origin.x+size.width/2,origin.y+size.height/2);
	//bg1->setPosition(origin+size/2);
	//bg1->setPositionX(origin.x + size.width/2);
	//bg1->setPositionY(origin.y + bg->getContentSize().height/2);

	//设置定位点位置在自身的左下角
	bg1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//开启抗锯齿
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BACKGROUND_ZORDER, BACKGROUND_TAG_1);//精灵加到场景中


	auto bg2 = Sprite::create("background1.jpg");
	bg2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bg2->setPositionY(bg1->getContentSize().height);
	addChild(bg2, BACKGROUND_ZORDER, BACKGROUND_TAG_2);
	//开启抗锯齿
	bg2->getTexture()->setAliasTexParameters();

	///////////////////////飞机
//	auto hero = Sprite::createWithSpriteFrameName("hero1.png");
	auto hero = Sprite::create("hero.png");
	hero->setPosition(VISIBLE_ORIGIN.x + VISIBLE_SIZE.width / 2, VISIBLE_ORIGIN.y + hero->getContentSize().height);
	this->addChild(hero, FOREGROUND_ZORDER,HERO_TAG);//精灵加到场景中

//	auto hero1 = Sprite::createWithSpriteFrameName("hero1.png");
	auto hero1 = Sprite::create("heroson.png");
	hero1->setPosition(hero->getPosition()-Vec2(hero->getContentSize().width,0));
	hero1->setVisible(isHeroDivide_1);
	this->addChild(hero1, FOREGROUND_ZORDER, HERO1_TAG);//精灵加到场景中

//	auto hero2 = Sprite::createWithSpriteFrameName("hero1.png");
	auto hero2 = Sprite::create("heroson.png");
	hero2->setPosition(hero->getPosition() + Vec2(hero->getContentSize().width, 0));
	hero2->setVisible(isHeroDivide_2);
	this->addChild(hero2, FOREGROUND_ZORDER, HERO2_TAG);//精灵加到场景中

	////////////////////////////////////009
	auto heroColor = hero->getColor();
	this->m_color = heroColor;

	auto menu1 = Menu::create();
	menu1->setPosition(Vec2::ZERO);
	//退出按钮
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(VISIBLE_ORIGIN.x + VISIBLE_SIZE.width - closeItem->getContentSize().width / 2,
		VISIBLE_ORIGIN.y + closeItem->getContentSize().height / 2));
	// create menu, it's an autorelease object
	//	auto menu = Menu::create(closeItem, NULL);
	menu1->addChild(closeItem);
	this->addChild(menu1, 1);

	//创建动画并放入缓存
	//给飞机添加动画
	//1.创建动画
	auto ani = AnimationCache::getInstance()->getAnimation(HERO_FLY_ANIMATION);
	//2.将动画封装为动作：
	auto animate = Animate::create(ani);

	//3.精灵运行动作
	hero->runAction(animate);


	//添加触摸事件的处理
	//1.创建一个事件监听对象
	auto listener = EventListenerTouchOneByOne::create();
	//2.分解事件，处理逻辑
	//a.触摸开始时
	//lambda表达式中括号【】部分表示控制对外部变量的访问，可以一个一个的传递，默认是值传递，可以在变量前加【&】，表示这个变量按引用传递，
	//也可以写一个【=】表示外部所有变量，都按值传递进来，可以访问了，但不能修改。
	//还可以写一个【&】表示外部所有变量都按引用传递进来，不仅可以访问，还可以修改外部变量本身的值。
	listener->onTouchBegan = [=](Touch* t, Event* e) {
		Vec2 touchPos = t->getLocation();
		//log("touch position is x:%f,y:%f", touchPos.x, touchPos.y);

		this->m_offset =  hero->getPosition()- touchPos;
		//触摸点是否在飞机上
		bool isContains = hero->getBoundingBox().containsPoint(touchPos);//只有点击飞机才动，即发生滑动，无跳跃
		//hero->setPosition(touchPos);
		//moveto 绝对运动。moveby相对运动
		//auto move = MoveTo::create(0.5f, touchPos);
		//hero->runAction(move);
		
		return isContains && !Director::getInstance()->isPaused() && !this->m_isOver;
	};
	//b.持续触摸并移动
	listener->onTouchMoved = [=](Touch* t, Event* e) {
		if (Director::getInstance()->isPaused() || this->m_isOver)
		{
			return;
		}
		Vec2 touchPos = t->getLocation();
		//Vec2 deltaPos = t->getDelta();//上一次触摸点与这一次触摸点之间的向量差
		//hero->setPosition(deltaPos + hero->getPosition());
		hero->setPosition(touchPos + m_offset);

		auto minX = hero->getContentSize().width / 2;
		auto minY = hero->getContentSize().height / 2;
		auto maxX = VISIBLE_SIZE.width-minX;
		auto maxY = (VISIBLE_SIZE.height-minY)*4/5;

		auto x = MAX(minX, MIN(maxX, hero->getPositionX()));
		auto y = MIN(maxY, hero->getPositionY());//限制上边界
		y = MAX(minY, y);//限制下边界
		hero->setPosition(x, y);
	
			hero1->setPosition(hero->getPosition() - Vec2(hero->getContentSize().width, 0));
			hero2->setPosition(hero->getPosition() + Vec2(hero->getContentSize().width, 0));
			hero1->setVisible(isHeroDivide_1);
			hero2->setVisible(isHeroDivide_2);


	};

	//3.注册监听到分发器上
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,hero);


	////////////////////////// UI
	///计分标签
	auto lblScore = Label::createWithBMFont("font.fnt",StringUtils::format("%d",this->m_totalScore));
	lblScore->setPosition(20, VISIBLE_SIZE.height -5);
	this->addChild(lblScore, UI_ZORDER, LABEL_SCORE_TAG);
	lblScore->setColor(Color3B::BLACK);
	lblScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	//炸弹菜单
	auto spBomb = Sprite::create("bomb.png");
	auto itemBomb = MenuItemSprite::create(spBomb, spBomb, [this,lblScore](Ref*) {
		//如果游戏处于暂停状态，就不响应
		if(Director::getInstance()->isPaused()||this->m_isOver) return;
		if (this->m_bombCount <= 0) return;

		for (auto enemy : this->m_enemies) {
			enemy->down();
			this->m_totalScore += enemy->getScore();
		}
		this->m_enemies.clear();

		lblScore->setString(StringUtils::format("%d", this->m_totalScore));
		this->m_bombCount--;
		AudioEngine::play2d("use_bomb.mp3", false, 0.6f);
		this->changeBomb();
	});
	itemBomb->setPosition(itemBomb->getContentSize());


	//加命菜单项

	auto spLife = Sprite::create("AddLife.png");
	auto itemLife = MenuItemSprite::create(spLife, spLife, [=](Ref*) {
		if (Director::getInstance()->isPaused() || this->getM_isOver()) return;
		if (this->m_myLife <= 0) return;
		if (!this->getM_isOver())
		{
			this->changeLife();
		}
	});
	itemLife->setPosition(itemBomb->getContentSize().width, itemBomb->getContentSize().height * 2);
	
	



	//////////////隐身菜单项009
	auto spHide = Sprite::create("yinshen.png");
	auto itemHide = MenuItemSprite::create(spHide, spHide, [=](Ref*) {
		if (Director::getInstance()->isPaused() || this->getM_isOver())	return;

		if (!this->getM_isOver())
		{
			auto lblTime = Label::createWithBMFont("font.fnt", StringUtils::format("%d", this->m_time));
			lblTime->setPosition(-10, -18);
			this->addChild(lblTime, UI_ZORDER, LABEL_TIME_TAG);
			if (this->m_time > 0)
			{

				schedule(schedule_selector(GameScene::sub), 1.0f);
				this->m_hide = 1;
				hero->setColor(Color3B::BLACK);
				lblTime->setPosition(VISIBLE_SIZE.width - 20, 180);
				lblTime->setColor(Color3B::BLUE);
				lblTime->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);	//锚点右对齐
				std::string strTime = StringUtils::format("%d", m_time);
				lblTime->setString(strTime);
			}
		}
	});

	itemHide->setPosition(VISIBLE_SIZE.width - itemHide->getContentSize().width / 2, 200);



	//暂停菜单
	auto spPauseNormal = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	auto spPauseSelected = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto spResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");
	auto spResumeSelected = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto itemPause = MenuItemSprite::create(spPauseNormal,spPauseSelected);
	auto itemResume = MenuItemSprite::create(spResumeNormal, spResumeSelected);
	auto toggle = MenuItemToggle::createWithCallback([](Ref *sender) {
		//获取当前选择项的下标（从0开始）
		int index = dynamic_cast<MenuItemToggle *>(sender)->getSelectedIndex();
		if (index)
		{
			AudioEngine::play2d("button.mp3", false, 0.4f);
			Director::getInstance()->pause();
		}
		else
		{
			AudioEngine::play2d("button.mp3", false, 0.4f);
			Director::getInstance()->resume();
		}
	},itemPause,itemResume,nullptr);
	toggle->setPosition(VISIBLE_SIZE - toggle->getContentSize());


	//炸弹菜单项
	auto menu = Menu::create();
	menu->addChild(itemBomb,1,ITEM_BOMB_TAG);
	menu->addChild(toggle, 1,ITEM_TOGGLE_TAG);
	///009///
	menu->addChild(itemHide, UI_ZORDER, ITEM_Hide_TAG);
	///xd///
	menu->addChild(itemLife, 1, ITEM_LIFE_TAG);

	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,UI_ZORDER,MENU_TAG);

	auto lblBomb = Label::createWithBMFont("font.fnt", StringUtils::format("X%d", this->m_bombCount));
	lblBomb->setPosition(itemBomb->getPosition() + Vec2(50, 0));
	this->addChild(lblBomb,UI_ZORDER,LABEL_BOMB_TAG);
	lblBomb->setColor(Color3B(120, 20, 10));
	lblBomb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

	//初始化菜单项和标签的显示
	this->changeBomb();
	this->changeLife();

	//每一帧调用一次Update函数
	scheduleUpdate();
	//定时创建子弹
	schedule(schedule_selector(GameScene::createBullet), CREATE_BULLET_INTERVAL);
	//定时创建敌机
	schedule(schedule_selector(GameScene::createSmallEnemy), CREATE_SMALL_ENEMY_INTERVAL,CC_REPEAT_FOREVER,CREATE_SMALL_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createMiddleEnemy), CREATE_MIDDLE_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_MIDDLE_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createBigEnemy), CREATE_BIG_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_BIG_ENEMY_DELAY);

	////////////////////////道具
	schedule(schedule_selector(GameScene::createUfo),CREATE_UFO_INTERVAL, CC_REPEAT_FOREVER,CREATE_UFO_DELAY);
	////////////////////////阵容
	schedule(schedule_selector(GameScene::EnemyGroup), CREATE_ENEMY_GROUP_INTERVAL, CC_REPEAT_FOREVER, CREATE_ENEMY_GROUP_DELAY);

	//schedule的lambda表示
	/*schedule([this](float) {

		auto hero = getChildByTag(HERO_TAG);

		auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
		bullet->setPosition(hero->getPositionX(), hero->getPositionY() + hero->getContentSize().height / 2);
		this->addChild(bullet, FOREGROUND_ZORDER);
		//将新建的子弹加入集合
		m_bullets.pushBack(bullet); }, 0.2f, "1x");*/
	return true;
}

//滑动
void GameScene::update(float delta) {
	//////////009
	if (this->m_time < 0)
	{
		this->unschedule(schedule_selector(GameScene::sub));
	}
	auto bg1 = this->getChildByTag(1);
	auto bg2 = getChildByTag(2);
	auto hero = getChildByTag(HERO_TAG);
	auto ufo = getChildByTag(UFO_TAG);
	auto hero1 = getChildByTag(HERO1_TAG);
	auto hero2 = getChildByTag(HERO2_TAG);


	bg1->setPositionY(bg1->getPositionY() - BACKGROUND_SPEED);
	//让背景2跟随背景1移动（取背景1的位置加上背景1的高度）
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);

	//当图2到达或超出底部边界，则更新图1的位置y到0
	if (bg2->getPositionY() <= 0) {
		bg1->setPositionY(0);
	}

	//遍历子弹集合
	Vector <Sprite* >removableBullets;
	for (auto bullet : m_bullets) {
		bullet->setRotation(bullet->getRotation()+20);
		bullet->setPositionY(bullet->getPositionY() + BULLET_SPEED);
		//检查这颗子弹有没有超出屏幕上方
		if (bullet->getPositionY() >= VISIBLE_SIZE.height + bullet->getContentSize().height / 2) {
			this->removeChild(bullet);//从场景中删除子弹
			//不能在遍历某个集合时，修改这个集合的成员数量，所以仅仅是把无效的集合存放到临时集合中，等便利结束之后再从集合中移除
			removableBullets.pushBack(bullet);
		}
	}


	//遍历敌机
	Vector <Enemy* >removableEnemies;
	for (auto enemy : m_enemies) {
		enemy->move();
		///////////////009
		if (enemy->getPositionY() <= -enemy->getContentSize().height || enemy->getPositionX()<enemy->getContentSize().width / 2 || enemy->getPositionX()>VISIBLE_SIZE.width + enemy->getContentSize().width / 2)
		{

			this->removeChild(enemy);
			removableEnemies.pushBack(enemy);

		}

		/*廖隆权if (enemy->getPositionY() <= -enemy->getContentSize().height/2) {
			this->removeChild(enemy);//从渲染树上删除
			removableEnemies.pushBack(enemy);
			}*/
	}

	Vector<Enemy*> addEnemy;
	//碰撞检测
	for (auto enemy : m_enemies) {

		//***********//
		//////游戏等级加速
		enemy->setSpeed(enemy->getSpeed() - Round / 3);

		for (auto bullet : m_bullets) {

			if (enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox())) {
				if (enemy->hit()) {
					removableEnemies.pushBack(enemy);
					//更新积分
					m_totalScore += enemy->getScore();
					auto nodeScore = this->getChildByTag(LABEL_SCORE_TAG);
					auto lblScore = dynamic_cast<Label *>(nodeScore);
					std::string strScore = StringUtils::format("%d", m_totalScore);
					lblScore->setString(strScore);
					if (enemy->getMkind() == 3)
					{
						Enemy* Midlle_1 = enemy->create(EnemyType::MIDDLE_ENEMY);
						Midlle_1->setPosition(enemy->getPositionX() - enemy->getContentSize().width / 3, enemy->getPositionY() - 5);
						addEnemy.pushBack(Midlle_1);
						this->addChild(Midlle_1, FOREGROUND_ZORDER, ENEMY2_TAG);
						Enemy* Midlle_2 = enemy->create(EnemyType::MIDDLE_ENEMY);
						Midlle_2->setPosition(enemy->getPositionX() + enemy->getContentSize().width / 3, enemy->getPositionY() - 5);
						addEnemy.pushBack(Midlle_2);
						this->addChild(Midlle_2, FOREGROUND_ZORDER, ENEMY2_TAG);

					}
				}

				removableBullets.pushBack(bullet);
				this->removeChild(bullet);
			}
		}

		//僚机碰撞测试
		if (enemy->getBoundingBox().intersectsRect(hero1->getBoundingBox())) {
			this->isHeroDivide_1 = false;
		}
		if (enemy->getBoundingBox().intersectsRect(hero2->getBoundingBox())) {
			this->isHeroDivide_2 = false;
		}

		//与飞机进行碰撞检测
		///////009
		if (enemy->getBoundingBox().intersectsRect(hero->getBoundingBox()))
		{
			//判断是否处于隐身状态，如果不是
			if (!this->m_hide)
			{
		
				if (this->m_myLife <= 1)
				{
					this->gameOver();
					//5.销毁敌机
					removableEnemies.pushBack(enemy);
					if (enemy->m_type == EnemyType::BIG_ENEMY){
			
					}
					enemy->down();
				}
				else{
					this->m_myLife--;
					this->changeLife();
					//TODO 英雄受伤动画

					removableEnemies.pushBack(enemy);
					enemy->down();
				}
			}
		}

	}

	for (auto enemy : addEnemy)
	{
		m_enemies.pushBack(enemy);
	}

	if (ufo && ((hero->getBoundingBox().intersectsRect(ufo->getBoundingBox()) ||
		(hero1->getBoundingBox().intersectsRect(ufo->getBoundingBox()))||
		(hero2->getBoundingBox().intersectsRect(ufo->getBoundingBox())))))

	{
		ufo->removeFromParent();
		if (this->m_ufoType ==1)
		{
			//填充满双子弹的发射次数
			this->m_doubleBulletCount = FULL_DOUBLE_BULLET_COUNT;
			AudioEngine::play2d("get_double_laser.mp3", false, 0.5f);
			auto director = Director::getInstance();
			director->setAnimationInterval(1.0f / 180);

		}
		//英雄战机分身
		else if (m_ufoType == 2) {
			isHeroDivide_1 = true; 
			isHeroDivide_2 = true;
			this->m_divideHeroCount = DIVIDE_HERO_COUNT;
		}
		else if (m_ufoType ==0 && this->m_bombCount < 3)
		{
			//拾取到炸弹道具
			this->m_bombCount++;
			AudioEngine::play2d("get_bomb.mp3", false, 0.5f);
			this->changeBomb();
		}
		else if (m_ufoType == 3 && m_myLife < 2)
		{
			//拾到加生命道具
			this->m_myLife=2;
			this->changeLife();
		}
	}

	for (auto bullet : removableBullets) {
		m_bullets.eraseObject(bullet);
	}
	removableBullets.clear();

	for (auto enemy : removableEnemies) {
		m_enemies.eraseObject(enemy);
	}
	removableEnemies.clear();
	
}

void GameScene::createBullet(float) {

	auto hero = getChildByTag(HERO_TAG);
	auto hero1 = getChildByTag(HERO1_TAG);
	auto hero2 = getChildByTag(HERO2_TAG);
	
	if (m_doubleBulletCount>0)
	{
		//创建双子弹
		auto bulletL = Sprite::create("bullet2.png");
		AudioEngine::play2d("bullet.mp3", false, 0.4f);

		bulletL->setPosition(hero->getPosition() + Vec2(-hero->getContentSize().width/3, hero->getContentSize().height / 3));
		this->addChild(bulletL, FOREGROUND_ZORDER);
		//将新建的子弹加入集合
		m_bullets.pushBack(bulletL);
		auto bulletR = Sprite::create("bullet2.png");
		AudioEngine::play2d("bullet.mp3", false, 0.4f);

		bulletR->setPosition(hero->getPosition() + Vec2(hero->getContentSize().width / 3, hero->getContentSize().height / 3));
		this->addChild(bulletR, FOREGROUND_ZORDER);
		//将新建的子弹加入集合
		m_bullets.pushBack(bulletR);
		this->m_doubleBulletCount--;//消耗掉一次双子弹次数		
	}

	//分身战机子弹
	if (this->m_divideHeroCount == 0) {
		this->isHeroDivide_1 = false;
		this->isHeroDivide_2 = false;
	}

	if (m_divideHeroCount>0) {
		auto director = Director::getInstance();
		director->setAnimationInterval(1.0f / 60);
		if (this->isHeroDivide_1) {
			auto bullet1 = Sprite::create("bullet1.png");
			AudioEngine::play2d("bullet.mp3", false, 0.4f);
			//bullet->setPosition(hero->getPositionX(), hero->getPositionY() + hero->getContentSize().height / 2);
			bullet1->setPosition(hero1->getPosition() + Vec2(0, hero1->getContentSize().height / 2));
			this->addChild(bullet1, FOREGROUND_ZORDER);
			//将新建的子弹加入集合
			m_bullets.pushBack(bullet1);
			this->m_divideHeroCount--;
		}

		if (this->isHeroDivide_2) {
			auto bullet2 = Sprite::create("bullet1.png");
			AudioEngine::play2d("bullet.mp3", false, 0.4f);
			//bullet->setPosition(hero->getPositionX(), hero->getPositionY() + hero->getContentSize().height / 2);
			bullet2->setPosition(hero2->getPosition() + Vec2(0, hero2->getContentSize().height / 2));
			this->addChild(bullet2, FOREGROUND_ZORDER);
			//将新建的子弹加入集合
			m_bullets.pushBack(bullet2);
			this->m_divideHeroCount--;
		}
		
	}


	if(m_doubleBulletCount==0)
	{
		auto director = Director::getInstance();
		director->setAnimationInterval(1.0f / 60);
		//创建单发子弹
		auto bullet = Sprite::create("bullet1.png");
		AudioEngine::play2d("bullet.mp3", false, 0.4f);
		//bullet->setPosition(hero->getPositionX(), hero->getPositionY() + hero->getContentSize().height / 2);
		bullet->setPosition(hero->getPosition() + Vec2(0, hero->getContentSize().height / 2));
		this->addChild(bullet, FOREGROUND_ZORDER);
		//将新建的子弹加入集合
		m_bullets.pushBack(bullet);
	}

}
void GameScene::createEnemy(const EnemyType& type) {
	//auto enemy = Sprite::createWithSpriteFrameName("enemy1.png");
	auto enemy = Enemy::create(type);
	float minX = enemy->getContentSize().width / 2;
	float maxX = VISIBLE_SIZE.width - minX;
	//随机数 % (max - min + 1)+ min
	//6~9:2%(4)+6=8
	//3%4+6=9
	//4%4+6=6
	float x = rand() % (int)(maxX - minX+1) + minX;
	enemy->setPosition(x,VISIBLE_SIZE.height+enemy->getContentSize().height/2);
	this->addChild(enemy, FOREGROUND_ZORDER);
	m_enemies.pushBack(enemy);
}
void GameScene::createSmallEnemy(float) {
	this->createEnemy(EnemyType::SMALL_ENEMY);
}
void GameScene::createMiddleEnemy(float) {
	this->createEnemy(EnemyType::MIDDLE_ENEMY);
}
void GameScene::createBigEnemy(float) {
	this->createEnemy(EnemyType::BIG_ENEMY);
}



/////////////////////////生成道具
void GameScene::createUfo(float) {
	/////////////////////////////////////道具
	this->m_ufoType = rand() % 4;
	//this->m_ufoType = 3;
	std::string frameName;
	if (m_ufoType == 3){
		frameName = "AddLifeson.png";
	}
	else if (m_ufoType == 2) {
		frameName = "son.png";
	}
	else if (m_ufoType == 0) {
		frameName = "Bombson.png";
	}
	else if (m_ufoType == 1) {
		frameName = "Double.png";
	}
	
	auto ufo = Sprite::create(frameName);
	auto minX = ufo->getContentSize().width / 2;
	auto maxX = VISIBLE_SIZE.width - minX;
	ufo->setPosition(rand() % (int)(maxX - minX + 1) + minX, VISIBLE_SIZE.height + ufo->getContentSize().height / 2);
	this->addChild(ufo, FOREGROUND_ZORDER, UFO_TAG);
	auto move1 = MoveBy::create(1, Vec2(0, -300));
	auto move2 = MoveTo::create(1.5f, Vec2(ufo->getPositionX(), -ufo->getContentSize().height));
	ufo->runAction(Sequence::create(move1, move1->reverse(), move2,RemoveSelf::create(),  nullptr));
}


/*
	1.当炸弹数为0时，菜单项和标签都不显示
	2.当炸弹数为1时，只显示菜单项
	3.当炸弹数大于1时，显示菜单项和标签，且更新标签显示内容
*/
void GameScene::changeBomb() {
	auto menu = this->getChildByTag(MENU_TAG);
	auto itemBomb = menu->getChildByTag(ITEM_BOMB_TAG);
	auto lblBomb = this->getChildByTag(LABEL_BOMB_TAG);
	if (this->m_bombCount<=0)
	{
		itemBomb->setVisible(false);
		lblBomb->setVisible(false);
	}
	else if(this->m_bombCount == 1)
	{
		itemBomb->setVisible(true);
		lblBomb->setVisible(false);
	}
	else
	{
		itemBomb->setVisible(true);
		lblBomb->setVisible(true);
		dynamic_cast<Label *>(lblBomb)->setString(StringUtils::format("X%d", this->m_bombCount));
	}
}

//生命值处理
void GameScene::changeLife()
{
	auto menu = this->getChildByTag(MENU_TAG);
	auto itemLife = menu->getChildByTag(ITEM_LIFE_TAG);
	if (this->m_myLife <2)
	{
		itemLife->setVisible(false);
	}
	else if (this->m_myLife == 2)
	{
		itemLife->setVisible(true);
	}
}


void GameScene::gameOver() {
	auto hero = this->getChildByTag(HERO_TAG);
	//1.设置成员变量m_isOver为true
	this->m_isOver = true;
	//6.道具还在跑
	for (auto node : this->getChildren()) {
		node->stopAllActions();
	}
	//2.执行爆炸动画

	AudioEngine::play2d("game_over.mp3", false, 0.5f);     //游戏结束音乐
	auto ani = AnimationCache::getInstance()->getAnimation(HERO_DOWN_ANIMATION);

	auto seq = Sequence::create(Animate::create(ani), CallFunc::create([this]() {
		//4.跳转场景
		auto scene = OverScene::createScene(this->m_totalScore);
		Director::getInstance()->replaceScene(scene);
	}),nullptr);
	hero->runAction(seq);
	//3.停止所有定时器
	this->unscheduleAllCallbacks();
	//7.禁用暂停菜单
	auto menu = this->getChildByTag(MENU_TAG);
	auto toggle = menu->getChildByTag(ITEM_TOGGLE_TAG);
	dynamic_cast<MenuItemToggle *>(toggle)->setEnabled(false);
	
}


/********************************************************************/

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
	for (auto i = 0; i < 4; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width / 2 + enemy->getContentSize().width /2 * i, VISIBLE_SIZE.height + enemy->getContentSize().height  * i);
		m_enemygroup.pushBack(enemy);
	}
	for (auto i = 0; i < 4; i++)
	{
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width / 2 - enemy->getContentSize().width /2 * i, VISIBLE_SIZE.height + enemy->getContentSize().height  * i);
		m_enemygroup.pushBack(enemy);

	}
	for (auto enemy : m_enemygroup)
	{
		enemy->setPosition(enemy->getPosition() + vec);
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
		auto move1 = MoveBy::create(2.0f, Vec2(VISIBLE_SIZE.width / 2, 0));
		enemy->runAction(move1);
	}

	for (auto i = 0; i < 2; i++)
	{
		auto enemy = Enemy::create(EnemyType::MIDDLE_ENEMY);
		enemy->setPosition(VISIBLE_SIZE.width + enemy->getContentSize().width / 2 + enemy->getContentSize().width * i, VISIBLE_SIZE.height / 6 * 5);
		m_enemygroup.pushBack(enemy);
		auto move2 = MoveBy::create(2.0f, Vec2(-VISIBLE_SIZE.width / 2, 0));
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
	bigenemy->setPosition(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height + bigenemy->getContentSize().height / 2);
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
		enemy->setSpeed(-0.8);
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
		//if (Round == 0)
		//{
			//this->createSmallEnemyGruop1(Vec2(0,0), -4);
		//}
		this->createEnemyGruop(Vec2(0, 0));
		this->createEnemyGruop(Vec2(0, 200));

	}
	else if (GroupFlag == 1){
		this->createSmallEnemyGruop1(Vec2(-80, 0), -1.5f);
		this->createSmallEnemyGruop1(Vec2(80, 0), -1.5f);
		this->createSmallEnemyGruop1(Vec2(-150, 0), -1.5f);
		this->createSmallEnemyGruop1(Vec2(150, 0), -1.5f);

	}
	else if (GroupFlag == 2){
		this->createSmallEnemyGruop2(Vec2(-200,0),-1.5f);
		this->createSmallEnemyGruop2(Vec2(-200, 50), -1.5f);
		
	
	}
	else if (GroupFlag == 3){
		this->createEnemyGruop1(Vec2(0, 100));
		this->createEnemyGruop1(Vec2(0, 150));


	}
	else if (GroupFlag == 4){
		this->createSmallEnemyGruop2(Vec2(-200, 0), -1.5f);
		this->createEnemyGruop2(Vec2(0, 50));

	}
	else if (GroupFlag == 5){
		this->createSmallEnemyGruop2(Vec2(-200, 0), -1.0f);
		this->createSmallEnemyGruop2(Vec2(-200, 50), -1.0f);
		this->createEnemyGruop3(Vec2(0, 200));
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



/***************************************009*******/
//隐身
void GameScene::sub(float) {
	auto hero = this->getChildByTag(3);
	this->m_time--;
	auto nodeTime = this->getChildByTag(LABEL_TIME_TAG);
	auto lblTime = dynamic_cast<Label*>(nodeTime);
	if (this->m_time>0)
	{
		log("xxxxxxxxxxxx%d", this->m_time);
		std::string strTime = StringUtils::format("%d", m_time);
		lblTime->setString(strTime);
	}

	if (this->m_time <= 0)
	{
		this->m_hide = 0;
		hero->setColor(this->m_color);
		this->m_time = 5;
		this->unschedule(schedule_selector(GameScene::sub));
		auto menu = dynamic_cast<Menu*>(this->getChildByTag(MENU_TAG));
		auto itemHide = menu->getChildByTag(ITEM_Hide_TAG);
		itemHide->setVisible(false);
		this->removeChild(lblTime);
		//停止5秒计数后要有20秒冷却时间
		//执行控制冷却时间的计数器
		scheduleOnce(schedule_selector(GameScene::control), 20.0f);
		return;

	}
}

void GameScene::control(float) {
	//控制隐身冷却时间
	auto menu = dynamic_cast<Menu*>(this->getChildByTag(MENU_TAG));
	auto itemHide = menu->getChildByTag(ITEM_Hide_TAG);
	itemHide->setVisible(true);
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}