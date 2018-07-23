#include "LoadingScene.h"
#include "Constants.h"
#include "GameScene.h"
#include"AudioEngine.h"  //声音引擎

using namespace experimental;


LoadingScene* LoadingScene::createScene() {
	return LoadingScene::create();
}
bool LoadingScene::init() {
	do
	{
		CC_BREAK_IF(!Scene::init());



		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

		//页面内容
		auto bg = Sprite::create("front.png");
		bg->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2);
		this->addChild(bg,BACKGROUND_ZORDER);

		//auto title = Sprite::createWithSpriteFrameName("shoot_copyright.png");
		//title->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2 + Vec2(0, 100));
		//this->addChild(title);

		auto start = Sprite::create("start.png");
		auto itemStart = MenuItemSprite::create(start, start, [=](Ref*) {
			auto loading = Sprite::create();
			loading->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2 + Vec2(0, -100));
			this->addChild(loading);
			auto ani = Animation::create();

			for (auto i = 0; i < 4; i++)
			{
				auto png = StringUtils::format("game_loading%d.png", i + 1);
				ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(png));
			}
			ani->setDelayPerUnit(0.2f);
			ani->setLoops(2);
			auto seq = Sequence::create(Animate::create(ani), CallFunc::create([]() {
				//场景跳转
				auto scene = GameScene::createScene();
				Director::getInstance()->replaceScene(TransitionSplitRows::create(1, scene));
			}), nullptr);
			loading->runAction(seq);
		});
		itemStart->setPosition(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 3);
		auto menu = Menu::create();
		menu->addChild(itemStart, 1);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, UI_ZORDER);


		//a.创建动画对象
		auto aniHeroFly = Animation::create();
		//b.添加动画帧(从精灵帧缓存中根据名字查找出来的）
		aniHeroFly->addSpriteFrameWithFileName("hero0.png");
		aniHeroFly->addSpriteFrameWithFileName("hero.png");
	//	aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	//	aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	//	aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	//	aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));

		//c.设置动画切换时长
		aniHeroFly->setDelayPerUnit(0.2f);
		//d.设置循环次数
		aniHeroFly->setLoops(CC_REPEAT_FOREVER);//无限循环
		//e.放入动画缓存
		AnimationCache::getInstance()->addAnimation(aniHeroFly, HERO_FLY_ANIMATION);

		auto aniBigEnemy = Animation::create();
		aniBigEnemy->addSpriteFrameWithFileName("enemyfly0.png");
		aniBigEnemy->addSpriteFrameWithFileName("enemyfly.png");
		aniBigEnemy->setDelayPerUnit(0.2f);
		aniBigEnemy->setLoops(CC_REPEAT_FOREVER);
		AnimationCache::getInstance()->addAnimation(aniBigEnemy, BIG_ENEMY_FLY_ANIMATION);

		auto aniSmallDown = Animation::create();
		for (auto i = 0; i < 4; i++) {
			auto frameName = StringUtils::format("enemy1_down%d.png", i + 1);
			aniSmallDown->addSpriteFrameWithFileName(frameName);
		}
		aniSmallDown->setDelayPerUnit(0.2f);
		AnimationCache::getInstance()->addAnimation(aniSmallDown, SMALL_ENEMY_DOWN_ANIMATION);

		auto aniMiddleDown = Animation::create();
		for (auto i = 0; i < 4; i++) {
			auto frameName = StringUtils::format("enemy2_down%d.png", i + 1);
			aniMiddleDown->addSpriteFrameWithFileName(frameName);
		}
		aniMiddleDown->setDelayPerUnit(0.2f);
		AnimationCache::getInstance()->addAnimation(aniMiddleDown, MIDDLE_ENEMY_DOWN_ANIMATION);

		auto aniBigDown = Animation::create();
		for (auto i = 0; i < 4; i++) {
			auto frameName = StringUtils::format("enemy3_down%d.png", i + 1);
			aniBigDown->addSpriteFrameWithFileName(frameName);
		}
		aniBigDown->setDelayPerUnit(0.2f);
		AnimationCache::getInstance()->addAnimation(aniBigDown, BIG_ENEMY_DOWN_ANIMATION);

		auto aniMiddleHit = Animation::create();
		aniMiddleHit->addSpriteFrameWithFileName("enemy2_down1.png");
		aniMiddleHit->addSpriteFrameWithFileName("enemy2.png");
		aniMiddleHit->setDelayPerUnit(0.2f);
		AnimationCache::getInstance()->addAnimation(aniMiddleHit, MIDDLE_ENEMY_HIT_ANIMATION);

		auto aniBigHit = Animation::create();
		aniBigHit->addSpriteFrameWithFileName("enemy3_down1.png");
		aniBigHit->addSpriteFrameWithFileName("enemy3.png");
		aniBigHit->setDelayPerUnit(0.2f);
		AnimationCache::getInstance()->addAnimation(aniBigHit, BIG_ENEMY_HIT_ANIMATION);

		//Vector<SpriteFrame*> frames;
		auto aniHeroDown = Animation::create();
		for (auto i = 0; i < 4; i++)
		{
			auto frameName = StringUtils::format("hero%d.png", i + 1);
			aniHeroDown->addSpriteFrameWithFileName(frameName);
			
		}
		aniHeroDown->setDelayPerUnit(0.2f);
		AnimationCache::getInstance()->addAnimation(aniHeroDown, HERO_DOWN_ANIMATION);


		//预加载音效文件
		AudioEngine::preload("achievement.mp3");
		AudioEngine::preload("big_spaceship_flying.mp3");
		AudioEngine::preload("bullet.mp3");
		AudioEngine::preload("button.mp3");
		AudioEngine::preload("enemy1_down.mp3");
		AudioEngine::preload("enemy2_down.mp3");
		AudioEngine::preload("enemy3_down.mp3");
		AudioEngine::preload("game_music.mp3");
		AudioEngine::preload("game_over.mp3");
		AudioEngine::preload("get_bomb.mp3");
		AudioEngine::preload("get_double_laser.mp3");
		AudioEngine::preload("out_porp.mp3");
		AudioEngine::preload("use_bomb.mp3");
	

		return true;
	} while (0);

	return false;
}