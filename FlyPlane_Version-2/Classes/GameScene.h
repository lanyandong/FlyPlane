#include "cocos2d.h"//<>搜索c++库，“”搜索第三方库
#include "Enemy.h"

USING_NS_CC;

class GameScene : public Scene {//类名首字母大写,方法第一个单词首字母不大写，第二个单词首字母大写
public:
	GameScene():m_offset(Vec2::ZERO), m_divideHeroCount(0),
		isHeroDivide_1(false), isHeroDivide_2(false), m_totalScore(0),
		m_doubleBulletCount(0), m_ufoType(0), m_bombCount(0), m_isOver(false), GroupFlag(0), Round(0), m_hide(0), m_time(5),m_myLife(1){ }
	static Scene* createScene();	

	bool init() override;//子类继承父类，virtual还是virtual，可以不写;override表示1.重写父类函数，帮忙检查正确性，2.子类还可重写

	CREATE_FUNC(GameScene)

	void update(float) override;

	void createBullet(float);
	void createEnemy(const EnemyType&);
	void createSmallEnemy(float);
	void createMiddleEnemy(float);
	void createBigEnemy(float);
	void createUfo(float);
	void sub(float);
	void menuCloseCallback(Ref* pSender);
	void control(float);

	/**********************************/
	void createSmallEnemyGruop1(Vec2, int);
	void createSmallEnemyGruop2(Vec2, int);

	void createEnemyGruop(Vec2); //创建敌机阵容
	void createEnemyGruop1(Vec2);
	void createEnemyGruop2(Vec2);
	void createEnemyGruop3(Vec2);

	void EnemyGroup(float);

	int GroupFlag;
	int Round;

	/*********************************009*/

	Color3B m_color;
	int m_hide;
	int m_time;
	CC_SYNTHESIZE_READONLY(bool, m_isOver, M_isOver);


private:
	Vec2 m_offset;//记录出点与飞机位置的偏移向量
	Vector<Sprite *> m_bullets;   //存放有效的子弹
	Vector<Enemy *> m_enemies;    //存放有效敌人
	int m_totalScore;             //记录本次游戏的总分
	int m_doubleBulletCount;      //双子弹的剩余发射次数
	int m_divideHeroCount;      //僚机的剩余发射次数

	int m_ufoType;                //道具类型
	int m_bombCount;              //炸弹数量                
	int m_myLife;                 //英雄生命值
	//添加功能：英雄战机分身
	bool isHeroDivide_1;
	bool isHeroDivide_2;
	void changeLife();
	void gameOver();
	void changeBomb();
};