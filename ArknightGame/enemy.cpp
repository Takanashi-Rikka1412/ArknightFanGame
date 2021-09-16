#include "enemy.h"
#include <QDebug>

enemy::enemy()
{
    //设置一些参数
    //e_enemy.load(e_path);

    e_x = 0;
    e_y = 0;
    e_Free = true;
    e_Speed = ENEMY_SPEED;
    e_type=0;
    e_life=4;
    e_dead=false;


}

void enemy::init()
{
    int i=0;
    for(;i<10;i++)
    {
        e_x=1820;
        e_y=20+100*i;

        if(!blocked(Up)||!blocked(Down)||!blocked(Left)||!blocked(Right))
            break;
    }

    e_Free = true;
    e_Speed = ENEMY_SPEED;
    e_type=0;
    e_life=4;
    e_dead=false;
}

void enemy::setType(int type)
{
    e_type=type;
    switch (e_type) {
    case 1:
        e_Rect.setWidth(ENEMY_1_WIDTH);
        e_Rect.setHeight(ENEMY_1_HEIGHT);
        e_Rect.moveTo(e_x-ENEMY_1_RECT_X, e_y-ENEMY_1_RECT_Y);
        break;
    case 2:
        e_Rect.setWidth(ENEMY_2_WIDTH);
        e_Rect.setHeight(ENEMY_2_HEIGHT);
        e_Rect.moveTo(e_x-ENEMY_2_RECT_X, e_y-ENEMY_2_RECT_Y);
        break;
    case 3:
        e_Rect.setWidth(ENEMY_3_WIDTH);
        e_Rect.setHeight(ENEMY_3_HEIGHT);
        e_Rect.moveTo(e_x-ENEMY_3_RECT_X, e_y-ENEMY_3_RECT_Y);
        break;
    default:
        e_Rect.setWidth(ENEMY_1_WIDTH);
        e_Rect.setHeight(ENEMY_1_HEIGHT);
        e_Rect.moveTo(e_x-ENEMY_1_RECT_X, e_y-ENEMY_1_RECT_Y);
        break;
    }
}

void enemy::setPlayer(player& player)
{
    e_player=&player;
}
void enemy::setMap(newMap map)
{
    e_map=map;
}

void enemy::move()
{
    //水平座标相同
    if(e_player->Y() == e_y){
        //左边
        if(e_player->X() < e_x){
            if(!blocked(Left))
                e_x -= ENEMY_SPEED * 1.5;
        }
        //右边
        else if(e_player->X() > e_x){
            if(!blocked(Right))
                e_x += ENEMY_SPEED * 1.5;
        }
        else
            attack();
    }
    //竖直座标相同
    if(e_player->X() == e_x){
        //上边
        if(e_player->Y() < e_y){
            if(!blocked(Up))
                e_y -= ENEMY_SPEED * 1.5;
        }
        //下边
        else if(e_player->Y() > e_y){
            if(!blocked(Down))
                e_y += ENEMY_SPEED * 1.5;
        }
        else
            attack();
    }
    //下侧
    if(e_player->Y() > e_y){
        //左边
        if(e_player->X() < e_x){
            if(!blocked(Down) && !blocked(Left)){
                e_x -= ENEMY_SPEED;
                e_y += ENEMY_SPEED;
            }
            else if(blocked(Down) && !blocked(Left)){
                e_x -= ENEMY_SPEED;
            }
            else if(!blocked(Down) && blocked(Left)){
                e_y += ENEMY_SPEED;
            }
        }
        //右边
        else if(e_player->X() > e_x){
            if(!blocked(Down) && !blocked(Right)){
                e_x += ENEMY_SPEED;
                e_y += ENEMY_SPEED;
            }
            else if(blocked(Down) && !blocked(Right)){
                e_x += ENEMY_SPEED;
            }
            else if(!blocked(Down) && blocked(Right)){
                e_y += ENEMY_SPEED;
            }
        }
    }
    //上侧
    if(e_player->Y() < e_y){
        //左边
        if(e_player->X() < e_x){
            if(!blocked(Up) && !blocked(Left)){
                e_x -= ENEMY_SPEED;
                e_y -= ENEMY_SPEED;
            }
            else if(blocked(Up) && !blocked(Left)){
                e_x -= ENEMY_SPEED;
            }
            else if(!blocked(Up) && blocked(Left)){
                e_y -= ENEMY_SPEED;
            }
        }
        //右边
        else if(e_player->X() > e_x){
            if(!blocked(Up) && !blocked(Right)){
                e_x += ENEMY_SPEED;
                e_y -= ENEMY_SPEED;
            }
            else if(blocked(Up) && !blocked(Right)){
                e_x += ENEMY_SPEED;
            }
            else if(!blocked(Up) && blocked(Right)){
                e_y -= ENEMY_SPEED;
            }
        }
    }
}

bool enemy::blocked(Direction dir)
{
    //判断是否可走
    int x1(0);
    int y1(0);
    int x2(0);
    int y2(0);
    if(dir==Up)
    {
        x1 = e_x - 29;
        x2 = e_x + 29;
        y1 = e_y - 31;
        y2 = y1;
    }
    else if (dir==Down)
    {
        x1 = e_x - 29;
        x2 = e_x + 29;
        y1 = e_y + 30;
        y2 = y1;
    }
    else if(dir==Left)
    {
        y1 = e_y - 29;
        y2 = e_y + 29;
        x1 = e_x - 31;
        x2 = x1;
    }
    else if(dir==Right)
    {
        y1 = e_y - 29;
        y2 = e_y + 29;
        x1 = e_x + 30;
        x2 = x1;
    }

    x1 /= 120;
    y1 =(y1-26)/120;
    x2 /= 120;
    y2 =(y2-26)/120;

    //    判断是否有障碍物
    if(e_map.mymap[y1][x1]==0&&e_map.mymap[y2][x2]==0
            && x1>0 && x1<1920 && x2>0 && x2<1920
            && y1>0 && y1<986 && y2>0 && y2<986
            )
    {
        return false;
    }
    else
    {
        //        qDebug()<<"障碍";
        return true;
    }


}

void enemy::attack()
{

}

void enemy::updatePosition()
{
    if(e_Free) { return; }
    move();
    switch (e_type) {
    case 1:e_Rect.moveTo(e_x-ENEMY_1_RECT_X, e_y-ENEMY_1_RECT_Y);
        break;
    case 2:e_Rect.moveTo(e_x-ENEMY_2_RECT_X, e_y-ENEMY_2_RECT_Y);
        break;
    case 3:e_Rect.moveTo(e_x-ENEMY_3_RECT_X, e_y-ENEMY_3_RECT_Y);
        break;
    default:e_Rect.moveTo(e_x-ENEMY_1_RECT_X, e_y-ENEMY_1_RECT_Y);
        break;
    }

}

void enemy::hurt()
{
    e_life--;
    //emit isHurt();

    if(e_life<=0)
    {
        e_dead=true;
    }


}

