#include "cocos2d.h" //头文件使用“ ”导入第三方库
#include "Enemy.h"

USING_NS_CC;

class GameScene : public Scene{
public:
	GameScene() :m_offset(Vec2::ZERO), m_totalScore(0), m_doubleBulletsCount(0), m_ufoType(0), m_bombCount(0), m_isOver(0), GroupFlag(0), Round(0){}; //初始化
	static Scene* createScene();

	bool init() override;  //定义函数，返回类型+函数名+参数列表
	void update(float delta);

	int GroupFlag;
	int Round;

	CREATE_FUNC(GameScene)



private:
	Vec2 m_offset;//记录触摸点与飞机位置的偏移量

	Vector<Sprite *> m_bullets; //存放有效子弹...
	Vector<Enemy *> m_enemies; //存放有效飞机...
	Vector<Sprite *> m_ufos;  //存放Ufo

	Vector<Enemy *> m_enemy;
	
	int m_totalScore;         //统计分数
	int m_doubleBulletsCount; //双子单的剩余发射次数
	int m_ufoType;   //道具类型
	int m_bombCount; //炸弹数量
	bool m_isOver;

	void createBullet(float); //创建子弹集合
	void createEnemy(const EnemyType& ); 

	void createSmallEnemy(float);
	void createMiddleEnemy(float);
	void createBigEnemy(float);
	void createUfo(float); //创建空投
	
	/**********************************/
	void createSmallEnemyGruop1(Vec2, int);
	void createSmallEnemyGruop2(Vec2, int);

	void createEnemyGruop(Vec2); //创建敌机阵容
	void createEnemyGruop1(Vec2);
	void createEnemyGruop2(Vec2);
	void createEnemyGruop3(Vec2);

	void EnemyGroup(float);

	/*********************************/
	void changeBomb();
	void gameOver();


};