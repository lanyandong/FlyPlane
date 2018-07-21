#include "LoadingScene.h"
#include "GameScene.h"
#include "Constants.h"



LoadingScene* LoadingScene::createScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init(){
	//�ȵ��ø���ĳ�ʼ������
	do
	{
		CC_BREAK_IF(!Scene::init());
		//���ؾ��鼯�ļ��������鶼���浽���鼯������
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

		//ҳ������
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
			//������ת
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionSplitRows::create(1, scene));
		}), nullptr);
		loading->runAction(seq);


		////////////////hero�ķ��ж���
		auto aniHeroFly = Animation::create();
		//���Ӷ���֡(�Ӿ���֡�����и������ֲ��ҳ�����)
		aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
		aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
		//���ö����л�ʱ��
		aniHeroFly->setDelayPerUnit(0.2f);
		//����ѭ������
		aniHeroFly->setLoops(CC_REPEAT_FOREVER);//����ѭ��
		//���붯������
		AnimationCache::getInstance()->addAnimation(aniHeroFly, HERO_FLY_ANIMATION);

		
		/////////////////��л��ķ��ж���
		auto aniBigEnemy = Animation::create();
		//b.���Ӷ���֡���Ӿ���֡�����и������ֲ��ҳ�����
		aniBigEnemy->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
		aniBigEnemy->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
		//c.���ö����л�ʱ��
		aniBigEnemy->setDelayPerUnit(0.2f);
		//d.����ѭ��������ʹ�ú��ʶ���޴Σ�����ʹ��-1
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