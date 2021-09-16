#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>
#include "config.h"
#include "newmap.h"



class bullet
{

public:
    bullet();  //构造函数


    //数据成员

    QPixmap bulletPic;  //图像文件
    int m_x;
    int m_y;
    int m_speed;
    int m_power;
    QRect m_Rect;  //子弹边框可用于碰撞检测
    Direction m_dir;  //子弹方向
    SETbullet level;  //子弹种类


    //属性

    static bool m_free;  //改成某一个时间点能否发射子弹
    //bool m_init;  //检验该子弹是否已经初始化


    //成员函数

    //更新位置
    void updatePosition();
    //设置速度
    int setspeed(SETbullet);
    //设置伤害
    int setpower(SETbullet);
    //设置图片
    void setimage();
    //设置函数  调用其他设置函数
    void set(Direction dir,SETbullet ch=LOW);
    //碰撞检测
    bool canReachable(int,int,Direction,newMap);
    //初始化
    void init();


    //设置方向
    //void paintbullet(QPainter&);
    //传值，子弹种类和方向
    //void passvalue(SETbullet,Direction);

};

#endif // BULLET_H
