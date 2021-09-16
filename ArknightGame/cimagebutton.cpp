#include "cimagebutton.h"
#include "config.h"

#include <QtGui>
#include <cmath>

CImageButton::CImageButton(QWidget* pParent,int t):QPushButton(pParent),
    m_bMouseIN(false),
    m_bLeftDown(false),
    m_bLighted(false),
    type(t)
{
}
CImageButton::~CImageButton()
{

}
void CImageButton::SetPixmap(QPixmap* pNormal,QPixmap* pHover,QPixmap* pLighted)
{
    m_pNormal=pNormal;
    m_pNormal_full=pNormal;

    m_pHover=pHover;
    m_pHover_full=pHover;

    m_pLighted=pLighted;
    m_pLighted_full=pLighted;
}

void CImageButton::SetLighted(bool value)
{
    if(value != m_bLighted)
    {
        m_bLighted=value;
        repaint();
    }
}

void CImageButton::SetType(int t)
{
    type=t;
}

void CImageButton::paintEvent(QPaintEvent* pEvent)
{

    QPixmap* pPixmap;
    if(type==1)
    {
        if(m_bLighted || m_bLeftDown)
       {
           pPixmap=m_pLighted;
           move(this->x()+0.5*(width()-m_pLighted->width()),
                this->y()+0.5*(height()-m_pLighted->height()));
           resize(m_pLighted->width(),
                  m_pLighted->height());
       }
       else
       {
           if(m_bMouseIN)
           {
               pPixmap=m_pHover;
               move(this->x()+0.5*(width()-m_pHover->width()),
                    this->y()+0.5*(height()-m_pHover->height()));
               resize(m_pHover->width(),
                      m_pHover->height());
           }
           else
           {
               pPixmap=m_pNormal;
               move(this->x()+0.5*(width()-m_pNormal->width()),
                    this->y()+0.5*(height()-m_pNormal->height()));
               resize(m_pNormal->width(),
                      m_pNormal->height());

               //qDebug()<<this->x()<<width()<<m_pNormal->width()
               //       <<this->y()<<height()<<m_pNormal->height();

           }

       }

    }
    else if(type==2)
    {
        if(m_bLighted || m_bLeftDown)
            pPixmap=m_pLighted;
        else if(m_bMouseIN)
            pPixmap=m_pHover;
        else
            pPixmap=m_pNormal;

    }


    QPainter painter;
    painter.begin(this);
    if(pPixmap)
        painter.drawPixmap(rect(),*pPixmap);
    if(text().count() > 0)
        painter.drawText(rect(),Qt::AlignCenter,text());
    painter.end();

}

void CImageButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        m_bLeftDown=true;
        repaint();
    }
    QPushButton::mousePressEvent(e);
}

void CImageButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        m_bLeftDown=false;
        repaint();
    }
    QPushButton::mouseReleaseEvent(e);
}

void CImageButton::enterEvent(QEvent *e)
{
    m_bMouseIN=true;
    repaint();
}

void CImageButton::leaveEvent(QEvent *e)
{
    m_bMouseIN=false;
    repaint();
}

void CImageButton::resizeFactor(float w, float h)
{
    refactor_Width=w;
    refactor_Height=h;

    m_pNormalTemp=m_pNormal_full->scaled(m_pNormal_full->width()*w,m_pNormal_full->height()*h);
    m_pHoverTemp=m_pHover_full->scaled(m_pHover_full->width()*w,m_pHover_full->height()*h);
    m_pLightedTemp=m_pLighted_full->scaled(m_pLighted_full->width()*w,m_pLighted_full->height()*h);

    m_pNormal=&m_pNormalTemp;
    m_pHover=&m_pHoverTemp;
    m_pLighted=&m_pLightedTemp;

    if(abs(w-1)<=1e-6&&abs(h-1)<=1e-6)
    {
        m_pNormal=m_pNormal_full;
        m_pHover=m_pHover_full;
        m_pLighted=m_pLighted_full;
    }
}

