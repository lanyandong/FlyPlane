#include "LoadingScene.h"
#include "GameScene.h"
#include "Constants.h"



LoadingScene* LoadingScene::createScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init(){
	//先调用父类的初始化函数
	do
	{
		CC_BREAK_IF(!Scene::init());
		//加载精灵集文件，将精灵都缓存到精灵集缓存中
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

		//页面内容
		auto bg = Sprite::createWithSpriteFrameName("background.png");
		bg->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2);
		this->addChild(bg);
		auto title = Sprite::createWithSpriteFrameName("shoot_copyright.png");
		title->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2 + Vec2(0, 100));
		this->addChild(title);

		auto loading = Sprite::create();
		loading->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2 + Vec2(0, -200));
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


		////////////////hero的飞行动画
		auto aniHeroFly = Animation::create();
		//添加动画帧(从精灵帧缓存中根据名字查找出来的)
		aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
		aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
		//设置动画切换时长
		aniHeroFly->setDelayPerUnit(0.2f);
		//设置循环次数
		aniHeroFly->setLoops(CC_REPEAT_FOREVER);//无限循环
		//放入动画缓存
		AnimationCache::getInstance()->addAnimation(aniHeroFly, HERO_FLY_ANIMATION);

		
		/////////////////大敌机的飞行动画
		auto aniBigEnemy = Animation::create();
		//b.添加动画帧（从精灵帧缓存中根据名字查找出来）
		aniBigEnemy->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
		aniBigEnemy->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
		//c.设置动画切换时长
		aniBigEnemy->setDelayPerUnit(0.2f);
		//d.设置循环次数，使用宏标识无限次，可以使用-1
		aniBigEnemy->setLoops(CC_REPEAT_FOREVER);
		AnimationCache::getInstance()->addAnimation(aniBigEnemy, BIG_ENEMY_FLY_ANIMATION);


		Vector<SpriteFrame*> frames;
		for (auto i = 0; i < 4; i++)
		{
			auto png = StringUtils::format("hero_blowup_n%d.png", i + 1);
			frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(png));
		}
		auto aniHeroDown = Animation::createWithSpriteFrames(frames, 0.2f);
		AnimationCache::getInstance()->addAnimation(aniHeroDown, HERO_DOWN_ANIMATION);
		return true;

	} while (0);
	return false;
}