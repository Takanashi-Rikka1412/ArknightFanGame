#ifndef ENEMY_H
#define ENEMY_H



#include "player.h"     //玩家角色类
#include "newmap.h"        //地图类
#include <QRect>
#include <QPixmap>
#include "config.h"


class enemy
{

public:
    enemy();
    //两个可能类名和变量名需要调整的
    player *e_player;   //玩家
    newMap e_map;
    int e_x, e_y;   //坐标
    int e_Speed;    //速度
    QRect e_Rect;   //矩阵块，碰撞检测
    int e_life;    //血量
    int e_type;    //敌人类型


    //属性

    bool e_Free;    //判断是否空闲（是否在场上）
    bool e_dead;   //判断是否死亡


    //QPixmap e_enemy;//资源对象（敌人对象）




    //成员函数

    void move();//移动
    bool blocked(Direction dir); //是否撞墙判断
    void attack();  //攻击
    void updatePosition();
    void setType(int type);
    void setPlayer(player& player);
    void setMap(newMap map);

    void hurt(); //受到攻击

    void init(); //初始化

};


#endif // ENEMY_H
