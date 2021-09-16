#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    initialize();

    connect(pb_Start, &QPushButton::clicked, this, &MainWindow::slotStart);
    connect(pb_Quit, &QPushButton::clicked, this, &MainWindow::slotQuit);
    connect(pb_SetUp, &QPushButton::clicked, this, &MainWindow::slotSetUp);
    connect(pb_back,&QPushButton::clicked, this, &MainWindow::slotBack);
    connect(ui->SliderVolume,SIGNAL(valueChanged(int)),this,SLOT(changeVolume_1()));
    connect(ui->SliderVolume_2,SIGNAL(valueChanged(int)),this,SLOT(changeVolume_2()));
    connect(pb_ReturnM, &QPushButton::clicked, this, &MainWindow::slotReturnM);
    connect(pb_1, &QPushButton::clicked, this, &MainWindow::slot1);
    connect(pb_2, &QPushButton::clicked, this, &MainWindow::slot2);
    connect(pb_3, &QPushButton::clicked, this, &MainWindow::slot3);
    connect(pb_4, &QPushButton::clicked, this, &MainWindow::slot4);
    connect(pb_5, &QPushButton::clicked, this, &MainWindow::slot5);
    connect(pb_Pause, &QPushButton::clicked, this, &MainWindow::slotPause);
    connect(pb_Continue, &QPushButton::clicked, this, &MainWindow::slotContinue);
    connect(pb_ReturnM_2, &QPushButton::clicked, this, &MainWindow::slotReturnM);
    connect(pb_skip,&QPushButton::clicked,this,[=]()
    {
        storyTimer.stop();
        text.setText(" ");
        text.showAll();
        opacity=0.0;
        opacity_2=0.0;
        opacity_3=0.0;
        opacity_4=0.0;
        ui->lb_storyBg->hide();
        ui->lb_people->hide();
        ui->lb_people_2->hide();
        ui->lb_people_3->hide();
        switch (storyIndex) {
        case 1:
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case 2:
            ui->stackedWidget->setCurrentIndex(1);
            updateMapLevel();
            playlist->setCurrentIndex(0);
            music->play();
            break;
        case 3:
            ui->stackedWidget->setCurrentIndex(3);
            Start(1);
            break;
        case 4:
            ui->stackedWidget->setCurrentIndex(0);
            playlist->setCurrentIndex(0);
            music->play();
            break;
        default:break;
        }});
    connect(this,&MainWindow::attackGifTimeOut,this,[=]()
    {
        playerAttackTimer.stop();
        player_1.gifChange=true;
        bullet::m_free=true;
    });
    connect(&enemyTimer,&QTimer::timeout,this,&MainWindow::enemyAction);
    connect(&playerTimer,&QTimer::timeout,this,[=]()
    {
        playerAction();
        bulletAction();
    });
    connect(&enemyAttackTimer,&QTimer::timeout,this,&MainWindow::enemyAttack);
    connect(&winTimer,&QTimer::timeout,this,[=]()
    {
        winTimer.stop();
        gameWin();
    });
    connect(&loseTimer,&QTimer::timeout,this,[=]()
    {
        loseTimer.stop();
        gameOver();
    });
    connect(&winOrLoseTimer,&QTimer::timeout,this,[=]()
    {
        fade_in(winOrLoseTimer,ui->winOrLose_label,opacity,effect);
    });
    connect(&storyTimer,&QTimer::timeout,this,&MainWindow::storyChange);
    connect(&CGTimer,&QTimer::timeout,this,[=]()
    {
        if(opacity_state)
            fade_in(CGTimer,ui->lb_storyBg,opacity,effect);
        else
            fade_out(CGTimer,ui->lb_storyBg,opacity,effect);
    });
    connect(&CGPeopleTimer,&QTimer::timeout,this,[=]()
    {
        if(opacity_2_state)
            fade_in(CGPeopleTimer,ui->lb_people,opacity_2,effect_2);
        else
            fade_out(CGPeopleTimer,ui->lb_people,opacity_2,effect_2);
    });
    connect(&CGPeopleTimer_2,&QTimer::timeout,this,[=]()
    {
        if(opacity_3_state)
            fade_in(CGPeopleTimer_2,ui->lb_people_2,opacity_3,effect_3);
        else
            fade_out(CGPeopleTimer_2,ui->lb_people_2,opacity_3,effect_3);
    });
    connect(&CGPeopleTimer_3,&QTimer::timeout,this,[=]()
    {
        if(opacity_4_state)
            fade_in(CGPeopleTimer_3,ui->lb_people_3,opacity_4,effect_4);
        else
            fade_out(CGPeopleTimer_3,ui->lb_people_3,opacity_4,effect_4);
    });



}

MainWindow::~MainWindow()
{
    delete ui;
    delete bglabel;
    delete playerlabel;
    //delete life;
    //delete storydialog;
    delete movie;
    delete bulllabel;
    delete enemyMovie;
    delete music;
    delete effect;
    delete effect_2;
    delete effect_3;
    delete effect_4;

    keypress.clear();
    bulletlabel.clear();
    player_1.m_bullet.clear();



    delete pb_Start;
    delete pb_SetUp;
    delete pb_Quit;
    delete pb_back;
    delete pb_1;
    delete pb_2;
    delete pb_3;
    delete pb_4;
    delete pb_5;
    delete pb_ReturnM;
    delete pb_ReturnM_2;
    delete pb_Pause;
    delete pb_Continue;
    delete pb_skip;


}

//程序初始化
void MainWindow::initialize()
{
    //基础设定
    ui->setupUi(this);
    movie=nullptr;
    enemyMovie=nullptr;
    bulllabel=nullptr;
    for(int i=0;i<bulletlabel.length();i++)
        bulletlabel[i]=nullptr;


    playlist = new QMediaPlaylist;
    playlist->addMedia(QUrl::fromLocalFile("../OurGame/music/bgm_title.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("../OurGame/music/bgm_firststory.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("../OurGame/music/bgm_battle.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("../OurGame/music/bgm_last.mp3"));


    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    music=new QMediaPlayer;
    music->setPlaylist(playlist);
    playlist->setCurrentIndex(0);
    music->play();

    effect=new QGraphicsOpacityEffect();
    effect_2=new QGraphicsOpacityEffect();
    effect_3=new QGraphicsOpacityEffect();
    effect_4=new QGraphicsOpacityEffect();


    lastWinLevel=load();


    //首页设定
    ui->stackedWidget->setCurrentIndex(0);

    if(lastWinLevel<5)
        ui->lb_TitleBg->setPixmap(QString(":/CG/title.jpg"));
    else
        ui->lb_TitleBg->setPixmap(QString(":/CG/title_2.jpg"));

    ui->lb_TitleBg->setScaledContents(true);
    ui->lb_TitleBg->lower();

    pb_Start=new CImageButton(ui->stackedWidget->currentWidget());
    QPixmap *pixb_Start_1=new QPixmap(QString(":/button/Start.png"));
    QPixmap *pixb_Start_2=new QPixmap(QString(":/button/Start_hover.png"));
    QPixmap *pixb_Start_3=new QPixmap(QString(":/button/Start_lighted.png"));
    pb_Start->SetPixmap(pixb_Start_1,pixb_Start_2,pixb_Start_3);
    pb_Start->setGeometry(0,0,pixb_Start_1->width(),pixb_Start_1->height());

    pb_Quit=new CImageButton(ui->stackedWidget->currentWidget());
    QPixmap *pixb_Quit_1=new QPixmap(QString(":/button/Quit.png"));
    QPixmap *pixb_Quit_2=new QPixmap(QString(":/button/Quit_hover.png"));
    QPixmap *pixb_Quit_3=new QPixmap(QString(":/button/Quit_lighted.png"));
    pb_Quit->SetPixmap(pixb_Quit_1,pixb_Quit_2,pixb_Quit_3);
    pb_Quit->setGeometry(0,0,pixb_Quit_1->width(),pixb_Quit_1->height());

    pb_SetUp=new CImageButton(ui->stackedWidget->currentWidget());
    QPixmap *pixb_SetUp_1=new QPixmap(QString(":/button/SetUp.png"));
    QPixmap *pixb_SetUp_2=new QPixmap(QString(":/button/SetUp_hover.png"));
    QPixmap *pixb_SetUp_3=new QPixmap(QString(":/button/SetUp_lighted.png"));
    pb_SetUp->SetPixmap(pixb_SetUp_1,pixb_SetUp_2,pixb_SetUp_3);
    pb_SetUp->setGeometry(0,0,pixb_SetUp_1->width(),pixb_SetUp_1->height());


    ui->frameSetUp->setObjectName("frameSetUp");
    ui->frameSetUp->setStyleSheet("QFrame#frameSetUp{border-image:url(:/background/SetUpRect.png)}");
    ui->frameSetUp->raise();
    ui->frameSetUp->hide();

    pb_back=new CImageButton(ui->frameSetUp);
    QPixmap *pixb_back_1=new QPixmap(QString(":/button/back.png"));
    QPixmap *pixb_back_2=new QPixmap(QString(":/button/back_hover.png"));
    QPixmap *pixb_back_3=new QPixmap(QString(":/button/back_lighted.png"));
    pb_back->SetPixmap(pixb_back_1,pixb_back_2,pixb_back_3);
    pb_back->setGeometry(0,0,pixb_back_1->width(),pixb_back_1->height());


    pb_Start->setFocusPolicy(Qt::NoFocus);
    pb_SetUp->setFocusPolicy(Qt::NoFocus);
    pb_Quit->setFocusPolicy(Qt::NoFocus);
    pb_back->setFocusPolicy(Qt::NoFocus);




    //选关页设定
    ui->stackedWidget->setCurrentIndex(1);

    ui->lb_bg_select->setPixmap(QString(":/CG/bg_select.png"));
    ui->lb_bg_select->setScaledContents(true);

    pb_ReturnM=new CImageButton(ui->stackedWidget->currentWidget(),2);
    QPixmap *pixb_ReturnM_1=new QPixmap(QString(":/button/ReturnM.png"));
    QPixmap *pixb_ReturnM_2=new QPixmap(QString(":/button/ReturnM_hover.png"));
    QPixmap *pixb_ReturnM_3=new QPixmap(QString(":/button/ReturnM_lighted.png"));
    pb_ReturnM->SetPixmap(pixb_ReturnM_1,pixb_ReturnM_2,pixb_ReturnM_3);
    pb_ReturnM->setGeometry(0,0,pixb_ReturnM_1->width(),pixb_ReturnM_1->height());

    pb_1=new CImageButton(ui->stackedWidget->currentWidget());
    QPixmap *pixb_1_1=new QPixmap(QString(":/button/1.png"));
    QPixmap *pixb_1_2=new QPixmap(QString(":/button/1_hover.png"));
    QPixmap *pixb_1_3=new QPixmap(QString(":/button/1_lighted.png"));
    pb_1->SetPixmap(pixb_1_1,pixb_1_2,pixb_1_3);
    pb_1->setGeometry(0,0,pixb_1_1->width(),pixb_1_1->height());


    pb_2=new CImageButton(ui->stackedWidget->currentWidget(),2);
    QPixmap *pixb_2_1=new QPixmap(QString(":/button/not2.png"));
    pb_2->SetPixmap(pixb_2_1,pixb_2_1,pixb_2_1);
    pb_2->setGeometry(0,0,pixb_2_1->width(),pixb_2_1->height());

    pb_3=new CImageButton(ui->stackedWidget->currentWidget(),2);
    QPixmap *pixb_3_1=new QPixmap(QString(":/button/not3.png"));
    pb_3->SetPixmap(pixb_3_1,pixb_3_1,pixb_3_1);
    pb_3->setGeometry(0,0,pixb_3_1->width(),pixb_3_1->height());

    pb_4=new CImageButton(ui->stackedWidget->currentWidget(),2);
    QPixmap *pixb_4_1=new QPixmap(QString(":/button/not4.png"));
    pb_4->SetPixmap(pixb_4_1,pixb_4_1,pixb_4_1);
    pb_4->setGeometry(0,0,pixb_4_1->width(),pixb_4_1->height());

    pb_5=new CImageButton(ui->stackedWidget->currentWidget(),2);
    QPixmap *pixb_5_1=new QPixmap(QString(":/button/not5.png"));
    pb_5->SetPixmap(pixb_5_1,pixb_5_1,pixb_5_1);
    pb_5->setGeometry(0,0,pixb_5_1->width(),pixb_5_1->height());

    pb_2->setDisabled(true);
    pb_3->setDisabled(true);
    pb_4->setDisabled(true);
    pb_5->setDisabled(true);

    pb_ReturnM->setFocusPolicy(Qt::NoFocus);
    pb_1->setFocusPolicy(Qt::NoFocus);
    pb_2->setFocusPolicy(Qt::NoFocus);
    pb_3->setFocusPolicy(Qt::NoFocus);
    pb_4->setFocusPolicy(Qt::NoFocus);
    pb_5->setFocusPolicy(Qt::NoFocus);






    //剧情页设定
    ui->stackedWidget->setCurrentIndex(2);



    text.setParent(ui->stackedWidget->currentWidget());
    text.setStyleSheet("QLabel{color:white}");
    text.raise();
    storyIndex=0;

    ui->lb_textBg->setPixmap(QString(":/CG/text.png"));
    ui->lb_textBg->setScaledContents(true);

    pb_skip=new CImageButton(ui->stackedWidget->currentWidget(),2);
    QPixmap *pixb_skip=new QPixmap(QString(":/button/skip.png"));
    pb_skip->SetPixmap(pixb_skip,pixb_skip,pixb_skip);
    pb_skip->setGeometry(0,0,pixb_skip->width(),pixb_skip->height());

    ui->stackedWidget->currentWidget()->setObjectName("stackedW_2");
    ui->stackedWidget->currentWidget()->setStyleSheet("QWidget#stackedW_2{background-color:black}");

    ui->lb_storyBg->setScaledContents(true);

    ui->lb_people->setScaledContents(true);
    ui->lb_people_2->setScaledContents(true);
    ui->lb_people_3->setScaledContents(true);
    ui->lb_storyBg->lower();
    ui->lb_people->stackUnder(ui->lb_textBg);
    ui->lb_people_2->stackUnder(ui->lb_textBg);
    ui->lb_people_3->stackUnder(ui->lb_textBg);
    ui->lb_storyBg->hide();
    ui->lb_people->hide();
    ui->lb_people_2->hide();
    ui->lb_people_3->hide();

    opacity=0.0;
    effect->setOpacity(opacity);
    ui->lb_storyBg->setGraphicsEffect(effect);
    opacity_2=0.0;
    effect_2->setOpacity(opacity_2);
    ui->lb_people->setGraphicsEffect(effect_2);
    opacity_3=0.0;
    effect_3->setOpacity(opacity_3);
    ui->lb_people_2->setGraphicsEffect(effect_3);
    opacity_4=0.0;
    effect_4->setOpacity(opacity_4);
    ui->lb_people_3->setGraphicsEffect(effect_4);



    textlist[0]=QString("“你是什么人？”");
    textlist[1]=QString("“可以是任何人。”");
    textlist[2]=QString("“你在为谁而战？”");
    textlist[3]=QString("“为所有人。”");

    textlist[4]=QString("？？？      12月23日。");
    textlist[5]=QString("？？？      这一天对我来说，究竟意味着什么？");
    textlist[6]=QString("？？？      我承诺了给大家带来数不尽的胜利，但却也不在乎那数不尽的代价。");
    textlist[7]=QString("？？？      也许我眼中只留有胜利。");
    textlist[8]=QString("？？？      除此之外，什么也没有。");
    textlist[9]=QString("？？？      照片上的“×”越来越多，他们都因我那冷酷残忍的所谓的胜利，因我而牺牲。");
    textlist[10]=QString("？？？      我做到了，我做不到。");
    textlist[11]=QString("？？？      也许我已面目全非，又或许迷失了方向。");
    textlist[12]=QString("？？？      但如果你能帮我，不管三年还是五年，甚至永不复苏。");
    textlist[13]=QString("？？？      那么，我接受你。");
    textlist[14]=QString("1093年    罗德岛    切城拯救行动前");
    textlist[15]=QString("近卫干员    阿米娅！我们找到博士的下落了！");
    textlist[16]=QString("阿米娅      真的吗？太好了！博士现在在哪里？");
    textlist[17]=QString("近卫干员    在切尔诺伯格的一家诊所地下室。");
    textlist[18]=QString("近卫干员    根据线人的情报，诊所所长这一个月以来的行动十分诡异。");
    textlist[19]=QString("近卫干员    在整合运动挑起战争后，所长一家人紧急逃难。");
    textlist[20]=QString("近卫干员    线人才在诊所的地下室的发现了博士的踪迹。");
    textlist[21]=QString("近卫干员    但博士现在正处在昏迷状态，整合运动也在切城，我们……");
    textlist[22]=QString("阿米娅      我们现在就出发前往切城，救出博士！");
    textlist[23]=QString("近卫干员    但是……");
    textlist[24]=QString("SCOUT      我反对。");
    textlist[25]=QString("SCOUT      以罗德岛现在的状态，显然是无法在那群整合疯子眼皮底下救出博士的。");
    textlist[26]=QString("SCOUT      况且博士他……那个人给我们带来多少损失。");
    textlist[27]=QString("SCOUT      阿米娅你全忘了吗？");
    textlist[28]=QString("阿米娅      但是他也给我们带来过胜利。");
    textlist[29]=QString("阿米娅      这三年里，罗德岛的损失也不小。");
    textlist[30]=QString("阿米娅      失去了博士的我们，并不能以一己之力，获得过哪怕一场完全胜利。");
    textlist[31]=QString("阿米娅      更多的精英干员们在罗德岛这三年的没落中牺牲了。");
    textlist[32]=QString("阿米娅      我相信博士。不仅是以前给我们带来胜利的博士。");
    textlist[33]=QString("阿米娅      也是这三年来做出选择，做出改变的博士。");
    textlist[34]=QString("ACE         我也赞成这次行动。");
    textlist[35]=QString("ACE         仅凭我们现有的能力，并不能打破罗德岛的僵局。");
    textlist[36]=QString("ACE         我们只有相信博士这一种出路。");
    textlist[37]=QString("阿米娅      谢谢你！ACE。");
    textlist[38]=QString("凯尔希      我投反对票。");
    textlist[39]=QString("凯尔希      阿米娅，你要知道，选择在这个时候进入切城是不明智之举。");
    textlist[40]=QString("凯尔希      我们面对的可能是来自整合运动和乌萨斯双方的袭击。");
    textlist[41]=QString("阿米娅      凯尔希医生…");
    textlist[42]=QString("凯尔希      但既然这是你发自内心的选择，我也不会阻拦。");
    textlist[43]=QString("凯尔希      你要知道，你背负着整个罗德岛的生死存亡，你要担负巨大的责任。");
    textlist[44]=QString("凯尔希      迅速组织小队，争取能躲过整合运动的视线。确保全员安全归来。");
    textlist[45]=QString("阿米娅      谢谢你……凯尔希医生。");

    textlist[46]=QString("杜宾        阿米娅，我很佩服你。");
    textlist[47]=QString("阿米娅      杜宾教官？");
    textlist[48]=QString("杜宾        能够相信那个人相信到这种程度。");
    textlist[49]=QString("阿米娅      相信博士……吗？");
    textlist[50]=QString("杜宾        你也并不是不知道，他的眼中只有纯粹的胜利而已。");
    textlist[51]=QString("杜宾        从他的眼中，我看不到其他任何东西。");
    textlist[52]=QString("阿米娅      也许我只是想让罗德岛，不再遭受这三年来的挫折吧。");
    textlist[53]=QString("阿米娅      嗯？等等，那是……");
    textlist[54]=QString("杜宾        源石虫？这片荒漠也终于被侵蚀了吗……");
    textlist[55]=QString("杜宾        阿米娅，距离上次行动也过去挺久了。");
    textlist[56]=QString("杜宾        我相信你还没忘记我教给你的战斗方法。");
    textlist[57]=QString("阿米娅      是的，杜宾教官。我还都记得。");
    textlist[58]=QString("（方向键上下左右进行移动，空格攻击。障碍物会阻挡人物的移动和法术的施放。）");
    textlist[59]=QString("（源石虫会近距离攻击，注意躲避。）");
    textlist[60]=QString("杜宾        阿米娅，带领小队，速战速决！");
    textlist[61]=QString("阿米娅      是！");

    textlist[62]=QString("？？？      不知过去了多久。");
    textlist[63]=QString("？？？      也许是三年，五年，或者是……");
    textlist[64]=QString("？？？      我是谁？");
    textlist[65]=QString("？？？      我身在何处？");
    textlist[66]=QString("？？？      我将去往何处？");
    textlist[67]=QString("？？？      ……");
    textlist[68]=QString("？？？      原来如此。");
    textlist[69]=QString("？？？      这就是你给我的答复吗？");
    textlist[70]=QString("？？？      谢谢。");
    textlist[71]=QString("            ");
    textlist[72]=QString("（心电仪的声音）");
    textlist[73]=QString("？？？      ……博士……");
    textlist[74]=QString("？？？      ……紧急……救……");
    textlist[75]=QString("？？？      ……医…...快……");
    textlist[76]=QString("？？？      ……他醒……");
    textlist[77]=QString("？？？      ……抓紧……");
    textlist[78]=QString("？？？      ……手！！");
    textlist[79]=QString("？？？      抓紧我的手！！");
    textlist[80]=QString("阿米娅      抓紧我的手！！");
    textlist[81]=QString("阿米娅      博士，博士！！");
    textlist[82]=QString("阿米娅      你终于……回来了！");
    textlist[83]=QString("            ");




    //战斗页设定
    ui->stackedWidget->setCurrentIndex(3);

    pb_Pause=new CImageButton(ui->stackedWidget->currentWidget(),2);
    QPixmap *pixb_Pause=new QPixmap(QString(":/button/Pause.png"));
    pb_Pause->SetPixmap(pixb_Pause,pixb_Pause,pixb_Pause);
    pb_Pause->setGeometry(0,0,pixb_Pause->width(),pixb_Pause->height());


    ui->framePause->setObjectName("framePause");
    ui->framePause->setStyleSheet("QFrame#framePause{border-image:url(:/background/SetUpRect.png)}");
    ui->framePause->raise();
    ui->framePause->hide();

    pb_Continue=new CImageButton(ui->framePause);
    QPixmap *pixb_Continue_1=new QPixmap(QString(":/button/Continue.png"));
    QPixmap *pixb_Continue_2=new QPixmap(QString(":/button/Continue_hover.png"));
    QPixmap *pixb_Continue_3=new QPixmap(QString(":/button/Continue_lighted.png"));
    pb_Continue->SetPixmap(pixb_Continue_1,pixb_Continue_2,pixb_Continue_3);
    pb_Continue->setGeometry(0,0,pixb_Continue_1->width(),pixb_Continue_1->height());

    pb_ReturnM_2=new CImageButton(ui->framePause);
    QPixmap *pixb_ReturnM2_1=new QPixmap(QString(":/button/ReturnM_2.png"));
    QPixmap *pixb_ReturnM2_2=new QPixmap(QString(":/button/ReturnM_2_hover.png"));
    QPixmap *pixb_ReturnM2_3=new QPixmap(QString(":/button/ReturnM_2_lighted.png"));
    pb_ReturnM_2->SetPixmap(pixb_ReturnM2_1,pixb_ReturnM2_2,pixb_ReturnM2_3);
    pb_ReturnM_2->setGeometry(0,0,pixb_ReturnM2_1->width(),pixb_ReturnM2_1->height());

    pb_Pause->setFocusPolicy(Qt::NoFocus);
    pb_Continue->setFocusPolicy(Qt::NoFocus);
    pb_ReturnM_2->setFocusPolicy(Qt::NoFocus);




    for(int i=0;i<ENEMY_NUM;i++)
    {
        enemyLabel[i].setParent(ui->stackedWidget->currentWidget());
        enemyLabel[i].stackUnder(ui->framePause);
        enemyLabel[i].hide();
        enemyLabel[i].setScaledContents(true);

    }

    //enemyLabel->lower();

    //bulllabel->setParent(ui->stackedWidget->currentWidget());
    //bulllabel->lower();

    bglabel=ui->m_label_bg;//背景图片
    bglabel->lower();
    bglabel->setScaledContents(true);




    playerlabel=ui->player;//人物gif
    //playerlabel->resize(200,211);
    playerlabel->setScaledContents(true);


    enemyIndex=-1;



    ui->life_label->setText(QString("阿米娅剩余生命：8"));
    ui->life_e_label->setText(QString("敌人剩余生命：4"));
    ui->winOrLose_label->setScaledContents(true);
    ui->winOrLose_label->hide();




    effect->setOpacity(1);   //透明范围为0.0-1.0，默认为0.7
    ui->winOrLose_label->setGraphicsEffect(effect);





    //手动调用窗口大小改变事件
    this->resize(this->size() - QSize(1,1));
    this->resize(this->size() + QSize(1,1));
    ui->stackedWidget->setCurrentIndex(2);
    story(1);









}




//槽函数------------------------------------------------

void MainWindow::slotStart()
{
    //若首次游戏未通关第一关，跳转到剧情，否则直接进入选关界面
    if(lastWinLevel<=0)
    {
        ui->stackedWidget->setCurrentIndex(2);
        story(2);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
        updateMapLevel();
    }
}
void MainWindow::slotQuit()
{
    this->close();
}
void MainWindow::slotSetUp()
{
    //进入设置时，其他按钮不能操作
    pb_Start->setDisabled(true);
    pb_SetUp->setDisabled(true);
    pb_Quit->setDisabled(true);
    ui->frameSetUp->show();
}
void MainWindow::slotBack()
{
    //从设置中返回时，可以操作其他按钮
    pb_Start->setDisabled(false);
    pb_SetUp->setDisabled(false);
    pb_Quit->setDisabled(false);
    ui->frameSetUp->hide();
}
void MainWindow::slotReturnM()
{
    ui->framePause->hide();
    End();  //停止游戏战斗
    ui->stackedWidget->setCurrentIndex(0);
    playlist->setCurrentIndex(0);
    music->play();
}
void MainWindow::slot1()
{
    ui->stackedWidget->setCurrentIndex(2);
    story(3);
    //ui->stackedWidget->setCurrentIndex(3);
    //Start(1);
}
void MainWindow::slot2()
{
    ui->stackedWidget->setCurrentIndex(3);
    Start(2);
}
void MainWindow::slot3()
{
    ui->stackedWidget->setCurrentIndex(3);
    Start(3);
}
void MainWindow::slot4()
{
    ui->stackedWidget->setCurrentIndex(3);
    Start(4);
}
void MainWindow::slot5()
{
    ui->stackedWidget->setCurrentIndex(3);
    Start(5);
}
void MainWindow::slotPause()
{
    ui->framePause->show();
    playerTimer.stop();
    enemyTimer.stop();

}
void MainWindow::slotContinue()
{
    keypress.clear();
    playerTimer.start();
    enemyTimer.start();
    ui->framePause->hide();
}


//音量改变
void MainWindow::changeVolume_1()
{
    ui->SliderVolume_2->setValue(ui->SliderVolume->value());
    music->setVolume(ui->SliderVolume->value());
}
void MainWindow::changeVolume_2()
{
    ui->SliderVolume->setValue(ui->SliderVolume_2->value());
    music->setVolume(ui->SliderVolume_2->value());
}









//事件-----------------------------------------------------------

//窗口大小改变事件
void MainWindow::resizeEvent(QResizeEvent *){
    ui->stackedWidget->move(0,0);
    ui->stackedWidget->resize(this->size());


    //首页
    ui->lb_TitleBg->move(0,0);
    ui->lb_TitleBg->resize(this->size());

    pb_Start->resize(322*RESIZE_FACTOR_WIDTH,231*RESIZE_FACTOR_HEIGHT);
    pb_Start->move(316*RESIZE_FACTOR_WIDTH,345*RESIZE_FACTOR_HEIGHT);
    pb_SetUp->resize(322*RESIZE_FACTOR_WIDTH,230*RESIZE_FACTOR_HEIGHT);
    pb_SetUp->move(1282*RESIZE_FACTOR_WIDTH,461*RESIZE_FACTOR_HEIGHT);
    pb_Quit->resize(322*RESIZE_FACTOR_WIDTH,228*RESIZE_FACTOR_HEIGHT);
    pb_Quit->move(316*RESIZE_FACTOR_WIDTH,576*RESIZE_FACTOR_HEIGHT);
    float fw=RESIZE_FACTOR_WIDTH;
    float fh=RESIZE_FACTOR_HEIGHT;
    pb_Start->resizeFactor(fw,fh);
    pb_SetUp->resizeFactor(fw,fh);
    pb_Quit->resizeFactor(fw,fh);

    ui->frameSetUp->resize(1000*RESIZE_FACTOR_WIDTH,500*RESIZE_FACTOR_HEIGHT);
    ui->frameSetUp->move(460*RESIZE_FACTOR_WIDTH,243*RESIZE_FACTOR_HEIGHT);

    ui->lb_volume->resize(170*RESIZE_FACTOR_WIDTH,100*RESIZE_FACTOR_HEIGHT);
    ui->lb_volume->move(200*RESIZE_FACTOR_WIDTH,140*RESIZE_FACTOR_HEIGHT);
    ui->SliderVolume->resize(400*RESIZE_FACTOR_WIDTH,80*RESIZE_FACTOR_HEIGHT);
    ui->SliderVolume->move(400*RESIZE_FACTOR_WIDTH,150*RESIZE_FACTOR_HEIGHT);

    QFont ft;
    ft.setPointSize(30*RESIZE_FACTOR_WIDTH);
    ui->lb_volume->setFont(ft);

    pb_back->resize(306*RESIZE_FACTOR_WIDTH,208*RESIZE_FACTOR_HEIGHT);
    pb_back->move(350*RESIZE_FACTOR_WIDTH,300*RESIZE_FACTOR_HEIGHT);
    pb_back->resizeFactor(fw,fh);




    //选关页
    ui->lb_bg_select->move(0,0);
    ui->lb_bg_select->resize(this->size());
    pb_ReturnM->resize(400*RESIZE_FACTOR_WIDTH,400*RESIZE_FACTOR_HEIGHT);
    pb_ReturnM->move(0,0);
    pb_1->resize(44*RESIZE_FACTOR_WIDTH,128*RESIZE_FACTOR_HEIGHT);
    pb_1->move(372*RESIZE_FACTOR_WIDTH,429*RESIZE_FACTOR_HEIGHT);
    pb_2->resize(88*RESIZE_FACTOR_WIDTH,128*RESIZE_FACTOR_HEIGHT);
    pb_2->move(626*RESIZE_FACTOR_WIDTH,429*RESIZE_FACTOR_HEIGHT);
    pb_3->resize(88*RESIZE_FACTOR_WIDTH,128*RESIZE_FACTOR_HEIGHT);
    pb_3->move(916*RESIZE_FACTOR_WIDTH,429*RESIZE_FACTOR_HEIGHT);
    pb_4->resize(88*RESIZE_FACTOR_WIDTH,128*RESIZE_FACTOR_HEIGHT);
    pb_4->move(1204*RESIZE_FACTOR_WIDTH,429*RESIZE_FACTOR_HEIGHT);
    pb_5->resize(88*RESIZE_FACTOR_WIDTH,128*RESIZE_FACTOR_HEIGHT);
    pb_5->move(1504*RESIZE_FACTOR_WIDTH,429*RESIZE_FACTOR_HEIGHT);
    pb_1->resizeFactor(fw,fh);
    pb_2->resizeFactor(fw,fh);
    pb_3->resizeFactor(fw,fh);
    pb_4->resizeFactor(fw,fh);
    pb_5->resizeFactor(fw,fh);



    //剧情页
    text.resize(1520*RESIZE_FACTOR_WIDTH,180*RESIZE_FACTOR_HEIGHT);
    text.move(200*RESIZE_FACTOR_WIDTH,800*RESIZE_FACTOR_HEIGHT);
    QFont ft_1;
    ft_1.setPointSize(17*RESIZE_FACTOR_WIDTH);
    text.setFont(ft_1);

    ui->lb_textBg->resize(1920*RESIZE_FACTOR_WIDTH,986*RESIZE_FACTOR_HEIGHT);
    ui->lb_textBg->move(0,0);

    pb_skip->resize(187*RESIZE_FACTOR_WIDTH,67*RESIZE_FACTOR_HEIGHT);
    pb_skip->move(1645*RESIZE_FACTOR_WIDTH,90*RESIZE_FACTOR_HEIGHT);

    ui->lb_storyBg->resize(1920*RESIZE_FACTOR_WIDTH,986*RESIZE_FACTOR_HEIGHT);
    ui->lb_storyBg->move(0,0);

    ui->lb_people->resize(1024*RESIZE_FACTOR_WIDTH,1024*RESIZE_FACTOR_HEIGHT);
    ui->lb_people->move(800*RESIZE_FACTOR_WIDTH,80*RESIZE_FACTOR_HEIGHT);
    ui->lb_people_2->resize(1024*RESIZE_FACTOR_WIDTH,1024*RESIZE_FACTOR_HEIGHT);
    ui->lb_people_2->move(96*RESIZE_FACTOR_WIDTH,80*RESIZE_FACTOR_HEIGHT);
    ui->lb_people_3->resize(1024*RESIZE_FACTOR_WIDTH,1024*RESIZE_FACTOR_HEIGHT);
    ui->lb_people_3->move(448*RESIZE_FACTOR_WIDTH,80*RESIZE_FACTOR_HEIGHT);




    //战斗页
    pb_Pause->resize(140*RESIZE_FACTOR_WIDTH,140*RESIZE_FACTOR_HEIGHT);
    pb_Pause->move(30*RESIZE_FACTOR_WIDTH,56*RESIZE_FACTOR_HEIGHT);

    ui->framePause->resize(1000*RESIZE_FACTOR_WIDTH,500*RESIZE_FACTOR_HEIGHT);
    ui->framePause->move(460*RESIZE_FACTOR_WIDTH,243*RESIZE_FACTOR_HEIGHT);

    ui->lb_volume_2->resize(170*RESIZE_FACTOR_WIDTH,100*RESIZE_FACTOR_HEIGHT);
    ui->lb_volume_2->move(200*RESIZE_FACTOR_WIDTH,140*RESIZE_FACTOR_HEIGHT);
    ui->SliderVolume_2->resize(400*RESIZE_FACTOR_WIDTH,80*RESIZE_FACTOR_HEIGHT);
    ui->SliderVolume_2->move(400*RESIZE_FACTOR_WIDTH,150*RESIZE_FACTOR_HEIGHT);

    ft.setPointSize(30*RESIZE_FACTOR_WIDTH);
    ui->lb_volume_2->setFont(ft);

    pb_Continue->resize(306*RESIZE_FACTOR_WIDTH,208*RESIZE_FACTOR_HEIGHT);
    pb_Continue->move(114*RESIZE_FACTOR_WIDTH,300*RESIZE_FACTOR_HEIGHT);
    pb_Continue->resizeFactor(fw,fh);

    pb_ReturnM_2->resize(306*RESIZE_FACTOR_WIDTH,208*RESIZE_FACTOR_HEIGHT);
    pb_ReturnM_2->move(580*RESIZE_FACTOR_WIDTH,300*RESIZE_FACTOR_HEIGHT);
    pb_ReturnM_2->resizeFactor(fw,fh);


    bglabel->move(0,0);
    bglabel->resize(this->size());
    playerlabel->resize(200*RESIZE_FACTOR_WIDTH,211*RESIZE_FACTOR_HEIGHT);
    movePlayerLabel();

    for(int i=0;i<ENEMY_NUM;i++)
    {
        enemyLabel[i].resize(200*RESIZE_FACTOR_WIDTH,
                             211*RESIZE_FACTOR_HEIGHT);
        enemyLabel[i].move(enemyList[i].e_x*RESIZE_FACTOR_WIDTH-ENEMY_1_LABEL_X,
                           enemyList[i].e_y*RESIZE_FACTOR_HEIGHT-ENEMY_1_LABEL_Y);
    }


    ui->life->move(0,0);
    ui->life->resize(500*RESIZE_FACTOR_WIDTH,26*RESIZE_FACTOR_HEIGHT);
    ui->life_e->move(1670*RESIZE_FACTOR_WIDTH,0);
    ui->life_e->resize(250*RESIZE_FACTOR_WIDTH,26*RESIZE_FACTOR_HEIGHT);
    ui->life_label->move(530*RESIZE_FACTOR_WIDTH,0);
    ui->life_label->resize(200*RESIZE_FACTOR_WIDTH,26*RESIZE_FACTOR_HEIGHT);
    ui->life_e_label->move(1500*RESIZE_FACTOR_WIDTH,0);
    ui->life_e_label->resize(200*RESIZE_FACTOR_WIDTH,26*RESIZE_FACTOR_HEIGHT);


    ui->winOrLose_label->move(690*RESIZE_FACTOR_WIDTH,393*RESIZE_FACTOR_HEIGHT);
    ui->winOrLose_label->resize(540*RESIZE_FACTOR_WIDTH,200*RESIZE_FACTOR_HEIGHT);

}

//键盘按下事件（将按键存入链表keypress里）
void MainWindow::keyPressEvent(QKeyEvent *event){//上下左右

    if(keypress.length()>=2) return;

    switch (event->key()) {
    case Qt::Key_Up:
        if(keypress.length()==0||keypress.contains(KeySpace))
        {
            keypress<<KeyUp;
            player_1.gifChange=true;
        }
        break;
    case Qt::Key_Down:
        if(keypress.length()==0||keypress.contains(KeySpace))
        {
            keypress<<KeyDown;
            player_1.gifChange=true;
        }
        break;
    case Qt::Key_Left:
        if(keypress.length()==0||keypress.contains(KeySpace))
        {
            keypress<<KeyLeft;
            player_1.gifChange=true;
        }
        break;
    case Qt::Key_Right:
        if(keypress.length()==0||keypress.contains(KeySpace))
        {
            keypress<<KeyRight;
            player_1.gifChange=true;
        }
        break;
    case Qt::Key_Space:
        if(!keypress.contains(KeySpace))
        {
            keypress<<KeySpace;
            if(!playerAttackTimer.isActive())
                player_1.gifChange=true;
        }
        break;
    default:
        break;
    }

}

//键盘抬起事件（从链表keypress里取出释放的按键）
void MainWindow::keyReleaseEvent(QKeyEvent *event){

    if(event->isAutoRepeat())return;

    switch (event->key()) {
    case Qt::Key_Up:
        if(keypress.removeOne(KeyUp))
            player_1.gifChange=true;
        break;
    case Qt::Key_Down:
        if(keypress.removeOne(KeyDown))
            player_1.gifChange=true;
        break;
    case Qt::Key_Left:
        if(keypress.removeOne(KeyLeft))
            player_1.gifChange=true;
        break;
    case Qt::Key_Right:
        if(keypress.removeOne(KeyRight))
            player_1.gifChange=true;
        break;
    case Qt::Key_Space:
        if(keypress.removeOne(KeySpace))
            player_1.gifChange=true;
        break;
    default:
        break;
    }
}

//鼠标事件（用于剧情文本）
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->MouseButtonPress)
    {
        if(text.finish)
        {
            wordindex++;
            text.setText(textlist[wordindex]);
        }
        else
            text.showAll();

    }

}







//战斗调用函数---------------------------------------------------

//开始一关的战斗
void MainWindow::Start(int gameLevel)
{
    //save(gameLevel);//测试用
    switch (gameLevel) {
    case 1:bglabel->setPixmap(QPixmap(":/map/map1.jpg"));break;
    case 2:bglabel->setPixmap(QPixmap(":/map/map2.jpg"));break;
    case 3:bglabel->setPixmap(QPixmap(":/map/map3.jpg"));break;
    case 4:bglabel->setPixmap(QPixmap(":/map/map4.jpg"));break;
    case 5:bglabel->setPixmap(QPixmap(":/map/map5.jpg"));break;
    default:break;
    }

    map.choose(gameLevel);

    opacity=0.0;

    playlist->setCurrentIndex(2);
    music->play();

    player_1.init();
    movePlayerLabel();
    playerTimer.start(UPDATE_TIME);
    resetGif();

    ui->life->setValue(player_1.life);
    ui->life_label->setText(QString("阿米娅剩余生命：")+QString::number(player_1.life));




    for(int i=0;i<ENEMY_NUM;i++)
    {
        enemyList[i].setPlayer(player_1);
        enemyList[i].setMap(map);
        enemyList[i].init();
        enemyList[i].setType(1);
    }

    enemyTimer.start(UPDATE_TIME);

    newEnemy();



}

//结束一关的战斗（获胜、失败、或回到主菜单）
void MainWindow::End()
{
    playerTimer.stop();
    enemyTimer.stop();
    playerAttackTimer.stop();
    enemyAttackTimer.stop();
    if(movie!=nullptr)
        movie->stop();
    if(enemyMovie!=nullptr)
        enemyMovie->stop();
    delete movie;
    movie=nullptr;
    delete enemyMovie;
    enemyMovie=nullptr;
    keypress.clear();
    enemyIndex=-1;
    for(int i=0;i<bulletlabel.length();i++)
    {
        bulletlabel[i]->close();
        bulletlabel.removeAt(i);
        //delete bulletlabel[i];
        //bulletlabel[i]=nullptr;
    }

    //player_1.m_bullet.clear();
    //bulletlabel[i]->close();
    //bulletlabel.removeAt(i);

    for(int i=0;i<ENEMY_NUM;i++)
    {
        enemyLabel[i].hide();
        enemyList[i].e_Free=true;
        enemyList[i].e_dead=false;
        enemyList[i].e_life=4;
    }
    bullet::m_free=true;



}

//重置主角的gif（置为站立状态）
void MainWindow::resetGif()
{

    delete movie;
    movie=nullptr;

    switch (player_1.playerDirection) {
    case Up:
    case Left:
        movie=new QMovie(":/playerGif/stand_up_left.gif");
        break;
    case Down:
    case Right:
        movie=new QMovie(":/playerGif/stand_down_right.gif");
        break;
    default:
        break;
    }


    playerlabel->setMovie(movie);
    movie->start();
    player_1.gifChange=false;
    playerAttackTimer.stop();
}

//重置敌人的gif（置为站立状态）
void MainWindow::resetEnemyGif()
{
    delete enemyMovie;
    enemyMovie=nullptr;
    enemyMovie=new QMovie(":/enemy/enemy1_move_left.gif");
    enemyLabel[enemyIndex].setMovie(enemyMovie);
    enemyMovie->start();
    enemyAttackTimer.stop();
}

//主角label移动
inline void MainWindow::movePlayerLabel(){
    playerlabel->move((player_1.X()-100)*this->width()/1920.0 ,  (player_1.Y()-211+20)*this->height()/986.0);
}

//产生新的敌人
void MainWindow::newEnemy()
{
    enemyIndex++;


    ui->life_e->setValue(enemyList[enemyIndex].e_life);
    ui->life_e_label->setText(QString("敌人剩余生命：")+QString::number(enemyList[enemyIndex].e_life));
    resetEnemyGif();



    enemyList[enemyIndex].e_Free=false;

    enemyLabel[enemyIndex].move((enemyList[enemyIndex].e_x-ENEMY_1_LABEL_X)*RESIZE_FACTOR_WIDTH,
                           (enemyList[enemyIndex].e_y-ENEMY_1_LABEL_Y)*RESIZE_FACTOR_HEIGHT);
    enemyLabel[enemyIndex].show();


}

//主角移动（移动的边界： 上26+30，下986+30， 左30，右1920-30）
void MainWindow::move(Direction dir){
    int x=0,y=0;
    switch (dir) {
    case Up:
        if(is_accessable(player_1.X(),player_1.Y(),Up))//通过坐标和map判断下一步是否可走
        {
            y = player_1.Y() - 10;
            if(y >= 26+30)
            {
                //向上移动就是x不变y减小
                player_1.SetY(y);
                movePlayerLabel();
            }
        }
        //if(playerlabel->pos().x()==0&&playerlabel->pos().y()==0)storydialog->show();//到达左上角，触发剧情框
        break;


    case Down:
        if(is_accessable(player_1.X(),player_1.Y(),Down))//通过坐标和map判断下一步是否可走
        {
            y = player_1.Y() + 10;
            if(y <= 986-30)
            {
                //向下移动是x不变y 变大
                player_1.SetY(y);
                movePlayerLabel();
            }
        }
        break;


     case Right:
        if(is_accessable(player_1.X(),player_1.Y(),Right))//通过坐标和map判断下一步是否可走
        {
            x = player_1.X() + 10;
            if(x <= 1920-30)
            {
                player_1.SetX(x);
                movePlayerLabel();
            }
        }
        break;


     case Left:
        if(is_accessable(player_1.X(),player_1.Y(),Left))//通过坐标和map判断下一步是否可走
        {
            x = player_1.X() - 10;
            if(x >= 30)
            {
                player_1.SetX(x);
                movePlayerLabel();
            }
        }
        break;


     default:
         break;
     }



    //qDebug()<<player_1.X()<<player_1.Y();
}

//主角的行动
void MainWindow::playerAction()
{

    if(playerAttackTimer.isActive())
        if(movie->currentFrameNumber()>=movie->frameCount()-1)
            emit attackGifTimeOut();


    if(keypress.length()==0)    //不按键
    {
        if(!player_1.gifChange) return;

        if(!playerAttackTimer.isActive())
            resetGif();
        else
        {
            int nowframe=movie->currentFrameNumber();
            //qDebug()<<nowframe;
            delete movie;
            movie=nullptr;

            switch (player_1.playerDirection) {
            case Up:
                movie=new QMovie(":/playerGif/attack_up.gif");
                break;
            case Left:
                movie=new QMovie(":/playerGif/attack_left.gif");
                break;
            case Down:
            case Right:
                movie=new QMovie(":/playerGif/attack_down_right.gif");
                break;
            default:
                break;
            }

            playerlabel->setMovie(movie);
            movie->start();

            for(int i=0;i<nowframe;i++)
                movie->jumpToNextFrame();
            //qDebug()<<movie->currentFrameNumber();
            movie->setPaused(false);


            player_1.gifChange=false;
        }

    }
    else if(keypress.length()==1)   //只按一个键
    {
        switch (keypress[0]){
        case KeyUp:
            move(Up);

            /*
            if(playerlabel->pos().x()==0&&playerlabel->pos().y()==0)
            {
                keypress.clear();
                delete movie;

                switch (player_1.playerDirection) {
                case Up:
                case Left:
                    movie=new QMovie(":/playerGif/stand_up_left.gif");
                    break;
                case Down:
                case Right:
                    movie=new QMovie(":/playerGif/stand_down_right.gif");
                    break;
                default:
                    break;
                }

                ui->player->setMovie(movie);
                movie->start();
                player_1.gifChange=false;



                storydialog->show();
            }
            */


            if(!player_1.gifChange) break;

            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walk_up_left.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                player_1.playerDirection=Up;
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_up.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;
                player_1.playerDirection=Up;
            }
            break;


        case KeyDown:
            move(Down);


            if(!player_1.gifChange) break;


            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walk_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                player_1.playerDirection=Down;
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;
                player_1.playerDirection=Down;
            }
            break;


        case KeyRight:
            move(Right);

            if(!player_1.gifChange) break;

            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walk_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                player_1.playerDirection=Right;
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;
                player_1.playerDirection=Right;
            }
            break;


        case KeyLeft:
            move(Left);

            if(!player_1.gifChange) break;

            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walk_up_left.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                player_1.playerDirection=Left;
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_left.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;
                player_1.playerDirection=Left;
            }
            break;


        case KeySpace:

            if(!player_1.gifChange) break;

            if(playerAttackTimer.isActive()) break;


            delete movie;
            movie=nullptr;

            switch (player_1.playerDirection) {
            case Up:
                movie=new QMovie(":/playerGif/attack_up.gif");
                break;
            case Left:
                movie=new QMovie(":/playerGif/attack_left.gif");
                break;
            case Down:
            case Right:
                movie=new QMovie(":/playerGif/attack_down_right.gif");
                break;
            default:
                break;
            }

            playerlabel->setMovie(movie);
            movie->start();
            player_1.gifChange=false;
            playerAttackTimer.start();


            attack(player_1.playerDirection);


        default:
            break;
        }
    }
    else if(keypress.length()==2)  //按两个键
    {
        if(keypress.contains(KeySpace)&&keypress.contains(KeyUp))
        {
            move(Up);

            if(!player_1.gifChange) return;

            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_up.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                playerAttackTimer.start();
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_up.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;

            }


            attack(Up);

        }
        else if(keypress.contains(KeySpace)&&keypress.contains(KeyDown))
        {
            move(Down);

            if(!player_1.gifChange) return;

            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                playerAttackTimer.start();
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;

            }


            attack(Down);
        }
        else if(keypress.contains(KeySpace)&&keypress.contains(KeyRight))
        {
            move(Right);

            if(!player_1.gifChange) return;

            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                playerAttackTimer.start();
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_down_right.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;

            }

            attack(Right);
        }
        else if(keypress.contains(KeySpace)&&keypress.contains(KeyLeft))
        {
            move(Left);

            if(!player_1.gifChange) return;

            if(!playerAttackTimer.isActive())
            {
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_left.gif");
                playerlabel->setMovie(movie);
                movie->start();
                player_1.gifChange=false;
                playerAttackTimer.start();
            }
            else
            {
                int nowframe=movie->currentFrameNumber();
                delete movie;
                movie=new QMovie(":/playerGif/walkAttack_left.gif");
                playerlabel->setMovie(movie);
                movie->start();
                for(int i=0;i<nowframe;i++)
                    movie->jumpToNextFrame();
                movie->setPaused(false);
                player_1.gifChange=false;

            }

            attack(Left);
        }
\
        //else if(keypress.contains(KeyUp)&&keypress.contains(KeyLeft)){}
        //else if(keypress.contains(KeyUp)&&keypress.contains(KeyRight)){}
        //else if(keypress.contains(KeyDown)&&keypress.contains(KeyLeft)){}
        //else if(keypress.contains(KeyDown)&&keypress.contains(KeyRight)){}
    }

}

//敌人的行动
void MainWindow::enemyAction()
{

    enemyList[enemyIndex].updatePosition();
    enemyLabel[enemyIndex].move((enemyList[enemyIndex].e_x-ENEMY_1_LABEL_X)*RESIZE_FACTOR_WIDTH,
                     (enemyList[enemyIndex].e_y-ENEMY_1_LABEL_Y)*RESIZE_FACTOR_HEIGHT);

    //qDebug()<<"enemy"<<enemyList[index].e_x<<enemyList[index].e_y;
    //qDebug()<<"player"<<player_1.X()<<player_1.Y();
    //qDebug()<<"label"<<enemyLabel[index].x()<<enemyLabel[index].y();

    enemyHurt();

    if(isOverlap(player_1.m_Rect,enemyList[enemyIndex].e_Rect))
    {
        if(enemyAttackTimer.isActive())   return;

        enemyAttackTimer.start(800);
        delete enemyMovie;
        enemyMovie=nullptr;

        enemyMovie=new QMovie(":/enemy/enemy1_attack_left.gif");
        enemyLabel[enemyIndex].setMovie(enemyMovie);
        enemyMovie->start();

    }
    else if(enemyAttackTimer.isActive())
    {
        resetEnemyGif();
        resetGif();
    }


}

//子弹的行动（更新子弹数据和位置）
void MainWindow::bulletAction()
{
    for (int i=0;i<player_1.m_bullet.length();i++) {
        if(!(player_1.m_bullet[i].canReachable(player_1.m_bullet[i].m_x,player_1.m_bullet[i].m_y,player_1.m_bullet[i].m_dir,map)))
        {
            player_1.m_bullet.removeAt(i);
            bulletlabel[i]->close();
            bulletlabel.removeAt(i);
        }
        else
        {
            player_1.m_bullet[i].updatePosition();
            bulletlabel[i]->move(player_1.m_bullet[i].m_x*RESIZE_FACTOR_WIDTH,
                                 player_1.m_bullet[i].m_y*RESIZE_FACTOR_HEIGHT);

        }



      }
}

//主角攻击
void MainWindow::attack(Direction dir){

    if(!bullet::m_free) return;


    bulllabel=new QLabel(ui->stackedWidget->currentWidget());
    bulletlabel.append(bulllabel);


    if(dir==Up||dir==Down)
        bulletlabel[bulletlabel.length()-1]->resize(50*RESIZE_FACTOR_WIDTH,74*RESIZE_FACTOR_HEIGHT);
    else
        bulletlabel[bulletlabel.length()-1]->resize(74*RESIZE_FACTOR_WIDTH,50*RESIZE_FACTOR_HEIGHT);

    player_1.shoot();

    bulletlabel[bulletlabel.length()-1]->setScaledContents(true);
    bulletlabel[bulletlabel.length()-1]->setPixmap(player_1.m_bullet[player_1.m_bullet.length()-1].bulletPic);




    bulletlabel[bulletlabel.length()-1]->show();
    bulletlabel[bulletlabel.length()-1]->stackUnder(ui->framePause);


}

//敌人攻击
void MainWindow::enemyAttack()
{
    player_1.hurt();
    ui->life->setValue(player_1.life);
    ui->life_label->setText(QString("阿米娅剩余生命：")+QString::number(player_1.life));

    delete movie;
    movie=nullptr;
    if(player_1.playerDirection==Up||player_1.playerDirection==Right)
        movie=new QMovie(":/playerGif/hurt_right.gif");
    else
        movie=new QMovie(":/playerGif/hurt_left.gif");

    playerlabel->setMovie(movie);
    movie->start();


    if(player_1.life==0)
    {
        ui->winOrLose_label->setPixmap(QString(":/Icon/LOSE.png"));
        loseTimer.start(2000);
        winOrLoseTimer.start(20);
        End();
    }
}

//障碍物检测
bool MainWindow::is_accessable(int x,int y,Direction dir){//判断是否可走


    int x1(0);
    int y1(0);
    int x2(0);
    int y2(0);
    if(dir==Up)
    {
        x1 = x - 29;
        x2 = x + 29;
        y1 = y - 31;
        y2 = y1;
    }
    else if (dir==Down)
    {
        x1 = x - 29;
        x2 = x + 29;
        y1 = y + 30;
        y2 = y1;
    }
    else if(dir==Left)
    {
        y1 = y - 29;
        y2 = y + 29;
        x1 = x - 31;
        x2 = x1;
    }
    else if(dir==Right)
    {
        y1 = y - 29;
        y2 = y + 29;
        x1 = x + 30;
        x2 = x1;
    }

    x1 /= 120;
    y1 =(y1-26)/120;
    x2 /= 120;
    y2 =(y2-26)/120;

//    判断是否可行动，即无障碍物
    if(map.mymap[y1][x1]==0&&map.mymap[y2][x2]==0)
    {
        return true;
    }
    else
    {
//        qDebug()<<"障碍";
        return false;
    }
}

//判断敌人受击（调用了isOverlap）
void MainWindow::enemyHurt()
{
    for(int i=0;i<player_1.m_bullet.length();i++)
    {
        if(isOverlap(player_1.m_bullet[i].m_Rect,enemyList[enemyIndex].e_Rect))
        {
            player_1.m_bullet.removeAt(i);
            bulletlabel[i]->close();
            bulletlabel.removeAt(i);

            enemyList[enemyIndex].hurt();
            ui->life_e->setValue(enemyList[enemyIndex].e_life);
            ui->life_e_label->setText(QString("敌人剩余生命：")+QString::number(enemyList[enemyIndex].e_life));

            if(enemyList[enemyIndex].e_dead)
            {
                delete enemyMovie;
                enemyMovie=nullptr;
                if(enemyIndex+1>=ENEMY_NUM)
                {
                    ui->winOrLose_label->setPixmap(QString(":/Icon/WIN.png"));
                    winTimer.start(2000);
                    winOrLoseTimer.start(20);
                    End();


                }
                else
                {
                    update();
                    newEnemy();
                }
            }
        }
    }

}

//矩形是否重叠（用于判断人物与子弹的方框是否重合，即是否受击）
bool MainWindow::isOverlap(const QRect &rc1, const QRect &rc2)
{
    if (rc1.x() + rc1.width()  > rc2.x() &&
        rc2.x() + rc2.width()  > rc1.x() &&
        rc1.y() + rc1.height() > rc2.y() &&
        rc2.y() + rc2.height() > rc1.y()
       )
        return true;
    else
        return false;
}

//一关战斗获得胜利
void MainWindow::gameWin()
{

    ui->winOrLose_label->hide();
    save(map.m_level);
    if(map.m_level==5)
    {
        ui->lb_TitleBg->setPixmap(QString(":/CG/title_2.jpg"));

        ui->stackedWidget->setCurrentIndex(2);
        story(4);

    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
        updateMapLevel();
        playlist->setCurrentIndex(0);
        music->play();
    }

}

//一关战斗失败
void MainWindow::gameOver()
{
    ui->winOrLose_label->hide();

    ui->stackedWidget->setCurrentIndex(1);
    updateMapLevel();
    playlist->setCurrentIndex(0);
    music->play();


}

//存档
void MainWindow::save(int level)
{

    QFile f("../OurGame/file/mapchoose.txt");


    //写入已通过的最大关卡
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
       qDebug()<< "Open failed." << endl;
    QTextStream txtOutput(&f);
    if(lastWinLevel<level)
        lastWinLevel=level;

    txtOutput <<lastWinLevel<< endl;


    f.close();
}

//读档
int MainWindow::load()
{
    QFile f("../OurGame/file/mapchoose.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
       qDebug()<< "Open failed." << endl;
    QTextStream txtInput(&f);
    int mapLevel=0;
    mapLevel = txtInput.readLine().toInt();  //地图等级，可以设置传出
    f.close();
    return mapLevel;
}

//更新关卡的解锁状态
void MainWindow::updateMapLevel()
{
    if(ui->stackedWidget->currentIndex()!=1) return;

    int maxWinLevel=load();

    for(int i=1;i<=maxWinLevel;i++)
    {
        if(i==1 && !pb_2->isEnabled())
        {
            pb_2->SetType(1);
            QPixmap *pixb_2_1=new QPixmap(QString(":/button/2.png"));
            QPixmap *pixb_2_2=new QPixmap(QString(":/button/2_hover.png"));
            QPixmap *pixb_2_3=new QPixmap(QString(":/button/2_lighted.png"));
            pb_2->SetPixmap(pixb_2_1,pixb_2_2,pixb_2_3);
            pb_2->setGeometry(0,0,pixb_2_1->width(),pixb_2_1->height());
            pb_2->setEnabled(true);
        }
        if(i==2 && !pb_3->isEnabled())
        {
            pb_3->SetType(1);
            QPixmap *pixb_3_1=new QPixmap(QString(":/button/3.png"));
            QPixmap *pixb_3_2=new QPixmap(QString(":/button/3_hover.png"));
            QPixmap *pixb_3_3=new QPixmap(QString(":/button/3_lighted.png"));
            pb_3->SetPixmap(pixb_3_1,pixb_3_2,pixb_3_3);
            pb_3->setGeometry(0,0,pixb_3_1->width(),pixb_3_1->height());
            pb_3->setEnabled(true);
        }
        if(i==3 && !pb_4->isEnabled())
        {
            pb_4->SetType(1);
            QPixmap *pixb_4_1=new QPixmap(QString(":/button/4.png"));
            QPixmap *pixb_4_2=new QPixmap(QString(":/button/4_hover.png"));
            QPixmap *pixb_4_3=new QPixmap(QString(":/button/4_lighted.png"));
            pb_4->SetPixmap(pixb_4_1,pixb_4_2,pixb_4_3);
            pb_4->setGeometry(0,0,pixb_4_1->width(),pixb_4_1->height());
            pb_4->setEnabled(true);
        }
        if(i==4 && !pb_5->isEnabled())
        {
            pb_5->SetType(1);
            QPixmap *pixb_5_1=new QPixmap(QString(":/button/5.png"));
            QPixmap *pixb_5_2=new QPixmap(QString(":/button/5_hover.png"));
            QPixmap *pixb_5_3=new QPixmap(QString(":/button/5_lighted.png"));
            pb_5->SetPixmap(pixb_5_1,pixb_5_2,pixb_5_3);
            pb_5->setGeometry(0,0,pixb_5_1->width(),pixb_5_1->height());
            pb_5->setEnabled(true);
        }

    }

    this->resize(this->size() - QSize(1,1));
    this->resize(this->size() + QSize(1,1));


}

//渐入
void MainWindow::fade_in(QTimer &qtimer,QLabel *label,float &opacity,QGraphicsOpacityEffect *effect)
{
    if(opacity>=1.0)
    {
        qtimer.stop();//定时器停止
    }
    else
    {
        opacity+=0.05;//透明度累加
        effect->setOpacity(opacity);
        label->setGraphicsEffect(effect);
        label->show();
    }


}

//渐出
void MainWindow::fade_out(QTimer &qtimer,QLabel *label,float &opacity,QGraphicsOpacityEffect *effect)
{
    if(opacity<=0.0)
    {
        qtimer.stop();//定时器停止
    }
    else
    {
        opacity-=0.05;//透明度累减
        effect->setOpacity(opacity);
        label->setGraphicsEffect(effect);
        label->show();
    }


}




//剧情--------------------------------------------------------


void MainWindow::story(int i){

    storyTimer.start(20);


    storyIndex=i;
    switch (storyIndex) {
    case 1:
        wordindex=0;
        break;
    case 2:
        wordindex=4;
        playlist->setCurrentIndex(1);
        music->play();
        break;
    case 3:
        wordindex=46;
        playlist->setCurrentIndex(1);
        music->play();
        break;
    case 4:
        wordindex=62;
        playlist->setCurrentIndex(3);
        music->play();
        break;
    default:break;
    }

    text.setText(textlist[wordindex]);


}

void MainWindow::storyChange()
{
    switch (storyIndex) {
    case 1:
        if(wordindex>=0&&wordindex<=3)
        {
            text.changed();

            if(wordindex==3 && !CGTimer.isActive() &&opacity<0.05 )
            {
                opacity_state=1;
                ui->lb_storyBg->setPixmap(QString(":/CG/CG_1.jpg"));
                CGTimer.start(20);
            }
        }
        else if(wordindex>3)
        {
            storyTimer.stop();
            text.setText(" ");
            text.showAll();
            opacity=0.0;
            ui->lb_storyBg->hide();

            ui->stackedWidget->setCurrentIndex(0);
        }
        break;
    case 2:
        if(wordindex>=4&&wordindex<=45)
        {
            text.changed();

            if(wordindex==14 && !CGTimer.isActive() && opacity<0.05)
            {
                opacity_state=1;
                ui->lb_storyBg->setPixmap(QString(":/CG/bg_bridge.png"));
                CGTimer.start(20);
            }
            if(wordindex==15 && !CGPeopleTimer.isActive() && opacity_2<0.05)
            {
                opacity_2_state=1;
                ui->lb_people->setPixmap(QString(":/CG/riop.png"));
                CGPeopleTimer.start(20);
            }
            if(wordindex==16 && !CGPeopleTimer_2.isActive() && opacity_3<0.05)
            {
                opacity_3_state=1;
                ui->lb_people_2->setPixmap(QString(":/CG/amiya.png"));
                CGPeopleTimer_2.start(20);
            }
            if(wordindex==24 && !CGPeopleTimer_3.isActive() && opacity_4<0.05)
            {
                opacity_2_state=0;
                opacity_3_state=0;
                CGPeopleTimer.start(20);
                CGPeopleTimer_2.start(20);

                opacity_4_state=1;
                ui->lb_people_3->setPixmap(QString(":/CG/SCOUT.png"));
                CGPeopleTimer_3.start(20);
            }
            if(wordindex==28 && !CGPeopleTimer_2.isActive() && opacity_3<0.05)
            {
                opacity_4_state=0;
                CGPeopleTimer_3.start(20);

                opacity_3_state=1;
                CGPeopleTimer_2.start(20);
            }
            if(wordindex==34 && !CGPeopleTimer.isActive() && opacity_2<0.05)
            {
                opacity_2_state=1;
                ui->lb_people->setPixmap(QString(":/CG/ACE.png"));
                CGPeopleTimer.start(20);
            }
            if(wordindex==38 && !CGPeopleTimer_3.isActive() && opacity_4<0.05)
            {
                opacity_2_state=0;
                opacity_3_state=0;
                CGPeopleTimer.start(20);
                CGPeopleTimer_2.start(20);

                opacity_4_state=1;
                ui->lb_people_3->setPixmap(QString(":/CG/kalts.png"));
                CGPeopleTimer_3.start(20);
            }
            if(wordindex==41 && !CGPeopleTimer_2.isActive() && opacity_3<0.05)
            {
                opacity_4_state=0;
                CGPeopleTimer_3.start(20);

                opacity_3_state=1;
                CGPeopleTimer_2.start(20);
            }
            if(wordindex==42 && !CGPeopleTimer.isActive() && opacity_2<0.05)
            {
                opacity_2_state=1;
                ui->lb_people->setPixmap(QString(":/CG/kalts.png"));
                CGPeopleTimer.start(20);
            }

        }
        else if(wordindex>45)
        {
            storyTimer.stop();
            text.setText(" ");
            text.showAll();
            opacity=0.0;
            opacity_2=0.0;
            opacity_3=0.0;
            opacity_4=0.0;
            ui->lb_storyBg->hide();
            ui->lb_people->hide();
            ui->lb_people_2->hide();
            ui->lb_people_3->hide();

            ui->stackedWidget->setCurrentIndex(1);
            updateMapLevel();

            playlist->setCurrentIndex(0);
            music->play();

        }
        break;

    case 3:
        if(wordindex>=46&&wordindex<=61)
        {
            text.changed();

            if(wordindex==46 && !CGTimer.isActive() && opacity<0.05)
            {
                opacity_state=1;
                opacity_2_state=1;
                ui->lb_storyBg->setPixmap(QString(":/CG/bg_wild_a.png"));
                ui->lb_people->setPixmap(QString(":/CG/doberm.png"));
                CGTimer.start(20);
                CGPeopleTimer.start(20);
            }
            if(wordindex==47 && !CGPeopleTimer_2.isActive() && opacity_3<0.05)
            {
                opacity_3_state=1;
                ui->lb_people_2->setPixmap(QString(":/CG/amiya.png"));
                CGPeopleTimer_2.start(20);
            }

        }
        else if(wordindex>61)
        {
            storyTimer.stop();
            text.setText(" ");
            text.showAll();
            opacity=0.0;
            opacity_2=0.0;
            opacity_3=0.0;
            opacity_4=0.0;
            ui->lb_storyBg->hide();
            ui->lb_people->hide();
            ui->lb_people_2->hide();
            ui->lb_people_3->hide();

            ui->stackedWidget->setCurrentIndex(3);
            Start(1);

        }
        break;

    case 4:
        if(wordindex>=62&&wordindex<=83)
        {
            text.changed();

            if(wordindex==72 && !CGTimer.isActive() &&opacity<0.05 )
            {
                opacity_state=1;
                ui->lb_storyBg->setPixmap(QString(":/CG/CG_last_1.jpg"));
                ui->lb_storyBg->show();
                CGTimer.start(20);
            }
            if(wordindex==79 && !CGTimer.isActive() &&opacity>0.95 )
            {
                opacity_state=0;
                CGTimer.start(20);
            }
            if(wordindex==80 && !CGTimer.isActive() &&opacity<0.05 )
            {
                opacity_state=1;
                ui->lb_storyBg->setPixmap(QString(":/CG/CG_last_2.jpg"));
                CGTimer.start(20);
            }
        }
        else if(wordindex>83)
        {
            storyTimer.stop();
            text.setText(" ");
            text.showAll();
            opacity=0.0;
            ui->lb_storyBg->hide();

            ui->stackedWidget->setCurrentIndex(0);

            playlist->setCurrentIndex(0);
            music->play();
        }
        break;


    default:break;
    }



}



