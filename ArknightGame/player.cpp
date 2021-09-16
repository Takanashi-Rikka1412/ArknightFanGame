#include "player.h"

player::player()
{
    life=8;
    playerDirection=Right;
    gifChange=false;

    x=60;
    y=4*120+26;

    m_Rect.setWidth(100);
    m_Rect.setHeight(211);
    m_Rect.moveTo(x-50,y-211+20);
}

void player::init()
{
    life=8;
    playerDirection=Right;
    gifChange=false;

    x=60;
    y=4*120+26;

    m_Rect.setWidth(100);
    m_Rect.setHeight(211);
    m_Rect.moveTo(x-50,y-211+20);

    m_bullet.clear();
}

void player::setPosition(int xx, int yy){
    x=xx;
    y=yy;
    m_Rect.moveTo(x,y);
}
void player::SetX(int xx){
    x=xx;
    m_Rect.moveTo(x,y);
}
void player::SetY(int yy){
    y=yy;
    m_Rect.moveTo(x,y);
}

void player::shoot(){
    if(!bullet::m_free) return;


    m_bullet<<bullet();
    m_bullet[m_bullet.length()-1].set(playerDirection);


    if(playerDirection==Up){
        m_bullet[m_bullet.length()-1].m_x = x - 25;
        m_bullet[m_bullet.length()-1].m_y = y - 110;
        m_bullet[m_bullet.length()-1].m_dir = Up;
    }
    if(playerDirection==Down){
        m_bullet[m_bullet.length()-1].m_x = x - 25;
        m_bullet[m_bullet.length()-1].m_y = y - 70;
        m_bullet[m_bullet.length()-1].m_dir = Down;
    }
    if(playerDirection==Left){
        m_bullet[m_bullet.length()-1].m_x = x  - 74; // 再-80
        m_bullet[m_bullet.length()-1].m_y = y - 90; // 再-10
        m_bullet[m_bullet.length()-1].m_dir = Left;
    }
    if(playerDirection==Right){
        m_bullet[m_bullet.length()-1].m_x = x ;   // 再+80
        m_bullet[m_bullet.length()-1].m_y = y - 90;  // 再-10
        m_bullet[m_bullet.length()-1].m_dir = Right;
    }

}

void player::hurt()
{
    life--;
}
