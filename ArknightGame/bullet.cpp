#include "bullet.h"
#include<QMatrix>
#include "newmap.h"
#include"config.h"

bool bullet::m_free = true;
//默认构造
bullet::bullet()
{
      //默认子弹
    m_x = 0;
    m_y = 0;

    //m_init = false;
    m_speed = 3;
    m_power = 10;
    m_dir = Right;
    level = LOW;
    m_Rect.setWidth(74);
    m_Rect.setHeight(50);
    m_Rect.moveTo(m_x,m_y);
}

void bullet::init()
{
    m_x = 0;
    m_y = 0;
    m_speed = 3;
    m_power = 10;
    m_dir = Right;
    level = LOW;
    m_Rect.setWidth(74);
    m_Rect.setHeight(50);
    m_Rect.moveTo(m_x,m_y);
    m_free=true;
}

//更新子弹位置
void bullet::updatePosition(){
    //子弹空闲
    //if(m_free){
    //    return;
    //}
    if (m_dir==Up) {
        m_y-=m_speed*5;
        m_Rect.moveTo(m_x,m_y);
    }
    if(m_dir==Down){
        m_y+=m_speed*5;
        m_Rect.moveTo(m_x,m_y);
    }
    if (m_dir==Right) {
        m_x+=m_speed*5;
        m_Rect.moveTo(m_x,m_y);
    }
    if(m_dir==Left){
        m_x-=m_speed*5;
        m_Rect.moveTo(m_x,m_y);
    }
    //界面检测，
    }
//设定
void bullet::set(Direction dir, SETbullet ch){
    level=ch;
    m_dir=dir;
    m_power=setpower(level);
    m_speed=setspeed(level);
    setimage();
}

//设定伤害
int bullet::setpower(SETbullet ch){
    if(ch==LOW){
        return 10;
    }
    if(ch==MIND){return 15;}
    if(ch==HIGH){return 20;}
    else return -1;
}
//设定速度
int bullet::setspeed(SETbullet ch){
    if(ch==LOW){
        return 3;
    }
    if(ch==MIND){return 5;}
    if(ch==HIGH){return 9;}
    else return -1;
}
//设定图像
void bullet::setimage(){
    //子弹图像
//    if(level==LOW){
//        bulletPic.load(":/cip/zd1.png");
//   }
//    else if(level==MIND){
//        bulletPic.load(":/cip/zd2.png");
//   }
//    else
//       bulletPic.load(":/cip/zd3.png");

    switch (m_dir) {
    case Up:bulletPic.load(":/bullet/bullet_up.png");break;
    case Down:bulletPic.load(":/bullet/bullet_down.png");break;
    case Left:bulletPic.load(":/bullet/bullet_left.png");break;
    case Right:bulletPic.load(":/bullet/bullet_right.png");break;
    default:break;
    }


}

//碰撞检测
bool bullet::canReachable(int x, int y, Direction dir,newMap map){

    int x1(0);
    int y1(0);
    int x2(0);
    int y2(0);
    if(dir==Up)
    {
        x1 = x;
        x2 = x + m_Rect.width();
        y1 = y + 1;
        y2 = y1;
    }
    else if (dir==Down)
    {
        x1 = x;
        x2 = x + m_Rect.width();
        y1 = y + m_Rect.height();
        y2 = y1;
    }
    else if(dir==Left)
    {
        y1 = y;
        y2 = y + m_Rect.height();
        x1 = x - 1;
        x2 = x1;
    }
    else if(dir==Right)
    {
        y1 = y;
        y2 = y + m_Rect.height();
        x1 = x + m_Rect.width();
        x2 = x1;
    }

    //转换map坐标
    x1 /= 120;
    y1 =(y1-26)/120;
    x2 /= 120;
    y2 =(y2-26)/120;

    //判断是否越界或是否有障碍物
    if(x<-1*m_Rect.width() || x>1920 || y<-1*m_Rect.height() || y>982 ||map.mymap[y1][x1]==1||map.mymap[y2][x2]==1)
    {
//        qDebug()<<"子弹越界";
        return false;
    }
    else
        return true;

}




