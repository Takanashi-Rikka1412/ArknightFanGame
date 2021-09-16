#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include "config.h"

//人物子弹函数关联
#include"bullet.h"

class player
{

public:
    player();
    int life;   //剩余生命
    Direction playerDirection;  //角色朝向
    bool gifChange; //角色的gif是否发生改变
    //子弹设定（弹夹）
    QList<bullet> m_bullet;
    QRect m_Rect;


    int X(){return x;}
    int Y(){return y;}
    void SetX(int xx);
    void SetY(int yy);
    //人物位置
    void setPosition(int xx, int yy);

    void init();//初始化

    //人物子弹发射
    void shoot();

    //受击
    void hurt();

private:
    int x;
    int y;

};


#endif // PLAYER_H
