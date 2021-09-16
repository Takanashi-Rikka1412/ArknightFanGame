#ifndef CIMAGEBUTTON_H
#define CIMAGEBUTTON_H


#include <QPushButton>

class CImageButton : public QPushButton
{
    Q_OBJECT

public:
    CImageButton(QWidget *parent,int t=1);
    ~CImageButton();
    void SetPixmap(QPixmap* pNormal,QPixmap* pHover,QPixmap* pLighted);
    void SetLighted(bool value);
    void SetType(int t);

    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

    void resizeFactor(float w,float h);

    bool m_bLighted;   //是否点击
    bool m_bMouseIN;   //是否鼠标移入
    bool m_bLeftDown;  //是否点击左键
    QPixmap* m_pHover;  //鼠标移入效果图
    QPixmap* m_pNormal; //正常效果图
    QPixmap* m_pLighted; //鼠标点击效果图

    QPixmap m_pHoverTemp;
    QPixmap m_pNormalTemp;
    QPixmap m_pLightedTemp;

    QPixmap* m_pHover_full;
    QPixmap* m_pNormal_full;
    QPixmap* m_pLighted_full;


    //用于窗口大小重绘
    float refactor_Width;
    float refactor_Height;

    int type;   //类型

};


#endif // CIMAGEBUTTON_H
