#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

//����
#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define VISIBLE_ORIGIN Director::getInstance()->getVisibleOrigin()


//��ǩ
#define BACKGROUND_TAG_1 1  //ע��Ϸ���ʶ��
#define BACKGROUND_TAG_2 2
#define HERO_TAG 3
#define LABEL_SCORE_TAG 10
#define UFO_TAG 4
#define MENU_TAG 11
#define ITEM_BOMB_TAG 12
#define LABEL_BOMB_TAG 13
#define TOGGLE_TAG 14



//ZORDER �ڵ�
#define BACKGROUND_ZORDER -1
#define DEFAULT_ZORDER 0
#define FOERGROUND_ZORDER 1
#define UI_ZORDER 99

//�ٶ�
#define BACKGROUND_SPEED 0.5f
#define BULLET_SPEED 8
#define SMALL_ENEMY_SPEED -4
#define MIDDELE_ENEMY_SPEED -1
#define BIG_ENEMY_SPEED -0.8f
#define UFO_SPEED -2


//��ʱ�����
#define CREATE_BULLET_INTERVAL 0.2f
#define CREATE_SMALL_ENEMY_INTERVAL 1.4f
#define CREATE_MIDDLE_ENEMY_INTERVAL 8
#define CREATE_BIG_ENEMY_INTERVAL 20
#define CREATE_UFO_INTERVAL 10 

#define CREATE_SMALL_ENEMY_DELAY  3
#define CREATE_MIDDLE_ENEMY_DELAY 5
#define CREATE_BIG_ENEMY_DELAY 10
#define CREATE_UFO_DELAY 5

//˫�ӵ��������
#define FULL_DOUBLE_BULLET_COUNT 3

#endif 