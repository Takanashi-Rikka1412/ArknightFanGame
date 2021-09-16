#ifndef CONFIG_H
#define CONFIG_H

//子弹等级和方向枚举接口
enum SETbullet {LOW,MIND,HIGH};
enum Direction {Up,Down,Left,Right};
//键盘按键枚举
enum Key {KeyUp,KeyDown,KeyLeft,KeyRight,KeySpace};

//用于窗口大小改变事件
#define RESIZE_FACTOR_WIDTH   this->width()/1920.0
#define RESIZE_FACTOR_HEIGHT  this->height()/986.0

//#define PLAYER_ATTACK_TIME 1160
#define UPDATE_TIME 20


//人物\敌人

#define ENEMY_SPEED 3    //敌人移动速度
#define ENEMY_NUM 5    //敌人数量

//相对于QLabel左上角的坐标
#define ENEMY_1_LABEL_X 100
#define ENEMY_1_LABEL_Y 191
//相对于Rect左上角的坐标
#define ENEMY_1_RECT_X  50
#define ENEMY_1_RECT_Y  80
//宽高
#define ENEMY_1_WIDTH   100
#define ENEMY_1_HEIGHT  100

#define ENEMY_2_LABEL_X 70
#define ENEMY_2_LABEL_Y 191
#define ENEMY_2_RECT_X  50
#define ENEMY_2_RECT_Y  160
#define ENEMY_2_WIDTH   100
#define ENEMY_2_HEIGHT  180

#define ENEMY_3_LABEL_X 100
#define ENEMY_3_LABEL_Y 191
#define ENEMY_3_RECT_X  50
#define ENEMY_3_RECT_Y  160
#define ENEMY_3_WIDTH   100
#define ENEMY_3_HEIGHT  180



#endif // CONFIG_H
