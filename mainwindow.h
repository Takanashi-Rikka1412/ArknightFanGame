#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QDialog>
#include <QMovie>
#include <QPushButton>
#include <QProcess>
#include <QImage>
#include <QPushButton>
#include <QTimer>
#include <QRect>
#include <QFrame>
#include <QSize>

#include <QtCore/QCoreApplication>
#include <QtMultimedia/QMediaPlayer>
#include <QGraphicsOpacityEffect>
#include <QMediaPlaylist>

#include "player.h"
#include "newmap.h"
#include "bullet.h"
#include "config.h"
#include "enemy.h"
#include "cimagebutton.h"
#include "text.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initialize();  //程序初始化

    void Start(int gameLevel); //开始一关的战斗

    void End(); //结束一关的战斗（包括中途退出）

    void resetGif();  //重置主角的gif（置为站立状态）

    void resetEnemyGif(); //重置敌人的gif（置为行走状态）

    inline void movePlayerLabel();  //主角label移动

    void newEnemy();  //产生新的敌人

    void move(Direction dir);  //主角移动

    void playerAction();  //主角的行动

    void enemyAction();   //敌人的行动

    void bulletAction();  //子弹的行动

    void attack(Direction dir);   //主角攻击

    void enemyAttack();   //敌人攻击

    bool is_accessable(int x,int y,Direction dir);  //障碍物检测

    void enemyHurt();  //判断敌人受击

    bool isOverlap(const QRect &rc1, const QRect &rc2);  //矩形是否重叠

    void gameWin();  //一关战斗获得胜利

    void gameOver(); //一关战斗失败

    void save(int level);  //存档

    int load();   //读档

    void updateMapLevel();  //更新关卡的解锁状态

    void fade_in(QTimer &qtimer,QLabel *label,float &opacity,QGraphicsOpacityEffect *effect);  //渐入
    void fade_out(QTimer &qtimer,QLabel *label,float &opacity,QGraphicsOpacityEffect *effect); //渐出


    void story(int i);


public slots:
    //槽函数
    void slotStart();
    void slotQuit();
    void slotSetUp();
    void slotBack();//首页设置返回
    void slotReturnM(); //回到主菜单
    void slot1();
    void slot2();
    void slot3();
    void slot4();
    void slot5();  //1到5关
    void slotPause();//暂停槽函数
    void slotContinue();//返回槽函数

    void changeVolume_1();//音量槽函数
    void changeVolume_2();

    void storyChange();


signals:
    void attackGifTimeOut();

private:
    Ui::MainWindow *ui;
    QLabel *bglabel;
    QLabel *playerlabel;
    QLabel *storylabel;
    //QDialog *storydialog;
    QLabel *bulllabel;
    player player_1;
    QList<Key> keypress;  //按键（可以同时按多个）
    QList<QLabel*> bulletlabel; //子弹label链表
    enemy enemyList[ENEMY_NUM];
    QLabel enemyLabel[ENEMY_NUM];
    QMovie *movie;
    QMovie *enemyMovie;
    newMap map;
    QMediaPlayer *music;
    QMediaPlaylist *playlist;

    QGraphicsOpacityEffect *effect;
    QGraphicsOpacityEffect *effect_2;
    QGraphicsOpacityEffect *effect_3;
    QGraphicsOpacityEffect *effect_4;

    //剧情文本类
    Text text;
    int wordindex;
    QString textlist[100];
    int storyIndex;



    int enemyIndex;  //敌人序号


    float opacity;  //透明度
    float opacity_2;  //透明度（备用）
    float opacity_3;
    float opacity_4;
    bool opacity_state;  //透明度状态（1为增大，0为减小）
    bool opacity_2_state;
    bool opacity_3_state;
    bool opacity_4_state;


    int lastWinLevel;  //存档中的上一次通关记录

    //定时器
    QTimer storyTimer;
    QTimer playerTimer;
    QTimer playerAttackTimer;
    QTimer enemyTimer;
    QTimer enemyAttackTimer;
    QTimer winTimer;
    QTimer loseTimer;
    QTimer winOrLoseTimer;
    QTimer CGTimer;
    QTimer CGPeopleTimer;
    QTimer CGPeopleTimer_2;
    QTimer CGPeopleTimer_3;




    //按钮
    CImageButton *pb_Start;
    CImageButton *pb_SetUp;
    CImageButton *pb_Quit;
    CImageButton *pb_back;
    CImageButton *pb_1;
    CImageButton *pb_2;
    CImageButton *pb_3;
    CImageButton *pb_4;
    CImageButton *pb_5;
    CImageButton *pb_ReturnM;
    CImageButton *pb_ReturnM_2;
    CImageButton *pb_Pause;
    CImageButton *pb_Continue;
    CImageButton *pb_skip;




protected:
    void keyPressEvent(QKeyEvent *event);   //键盘按下事件
    void keyReleaseEvent(QKeyEvent *event); //键盘抬起事件
    //void timerEvent(QTimerEvent *event);    //定时器事件
    void resizeEvent(QResizeEvent *event);  //窗口大小改变事件
    //void paintEvent(QPaintEvent *event);    //绘图事件
    void mousePressEvent(QMouseEvent *event);  //鼠标事件

};
#endif // MAINWINDOW_H
