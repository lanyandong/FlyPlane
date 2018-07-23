#include "cocos2d.h"//<>����c++�⣬����������������
#include "Enemy.h"

USING_NS_CC;

class GameScene : public Scene {//��������ĸ��д,������һ����������ĸ����д���ڶ�����������ĸ��д
public:
	GameScene():m_offset(Vec2::ZERO), m_divideHeroCount(0),
		isHeroDivide_1(false), isHeroDivide_2(false), m_totalScore(0),
		m_doubleBulletCount(0), m_ufoType(0), m_bombCount(0), m_isOver(false), GroupFlag(0), Round(0), m_hide(0), m_time(5),m_myLife(1){ }
	static Scene* createScene();	

	bool init() override;//����̳и��࣬virtual����virtual�����Բ�д;override��ʾ1.��д���ຯ������æ�����ȷ�ԣ�2.���໹����д

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

	void createEnemyGruop(Vec2); //�����л�����
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
	Vec2 m_offset;//��¼������ɻ�λ�õ�ƫ������
	Vector<Sprite *> m_bullets;   //�����Ч���ӵ�
	Vector<Enemy *> m_enemies;    //�����Ч����
	int m_totalScore;             //��¼������Ϸ���ܷ�
	int m_doubleBulletCount;      //˫�ӵ���ʣ�෢�����
	int m_divideHeroCount;      //�Ż���ʣ�෢�����

	int m_ufoType;                //��������
	int m_bombCount;              //ը������                
	int m_myLife;                 //Ӣ������ֵ
	//��ӹ��ܣ�Ӣ��ս������
	bool isHeroDivide_1;
	bool isHeroDivide_2;
	void changeLife();
	void gameOver();
	void changeBomb();
};