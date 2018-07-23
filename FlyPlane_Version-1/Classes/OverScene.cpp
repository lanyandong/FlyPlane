#include "OverScene.h"
#include "Constants.h"
#include "GameScene.h"
#include "AudioEngine.h"

using namespace experimental;

Scene* OverScene::createScene(int score)
{
	auto scene = new OverScene;
	if (scene && scene->init(score))
	{
		scene->autorelease();
		return scene;

	}
	delete scene;
	scene = nullptr;
	return nullptr;
}

bool OverScene::init(int score){
	do{
		CC_BREAK_IF(!Scene::init());

		auto bg = Sprite::createWithSpriteFrameName("gameover.png");
		bg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		bg->setPosition(0, VISIBLE_ORIGIN.y + VISIBLE_SIZE.height);
		this->addChild(bg, BACKGROUND_ZORDER);


		auto highScore = UserDefault::getInstance()->getIntegerForKey(HIGH_SCORE_KEY, 0);

		auto lblHightScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", highScore));
		lblHightScore->setPosition(VISIBLE_ORIGIN + Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 5 * 4));
		this->addChild(lblHightScore, UI_ZORDER);
		lblHightScore->setColor(Color3B::BLACK);
		if (highScore < score)
		{
			highScore = score;
			UserDefault::getInstance()->getIntegerForKey(HIGH_SCORE_KEY, highScore);
			auto seq = Sequence::create(
				ScaleTo::create(1,5),
				CallFuncN::create([highScore](Node * node) {
				dynamic_cast<Label*>(node)->setString(StringUtils::format("%d", highScore));
				AudioEngine::play2d("achievement.mp3");
			}),
			ScaleTo::create(1,1), 
			nullptr);
			lblHightScore->runAction(seq);
		}

		auto lblScore = Label::createWithBMFont("font.fnt",StringUtils::format("%d",score));
		lblScore->setPosition(VISIBLE_ORIGIN + Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 3));
		this->addChild(lblScore, UI_ZORDER);
		lblScore->setColor(Color3B::BLACK);
		lblScore->setBMFontSize(68);


		auto spRestart = Sprite::createWithSpriteFrameName("btn_finish.png");
		auto itemRestart = MenuItemSprite::create(spRestart, spRestart, [](Ref*){
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(scene);
		});
		itemRestart->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		itemRestart->setPosition(VISIBLE_SIZE.width - 20, 20);

		auto  menu = Menu::create(itemRestart, nullptr);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1, MENU_TAG);

		return true;
	} while (0);
	return false;
}