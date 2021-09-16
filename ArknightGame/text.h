#ifndef TEXT_H
#define TEXT_H


#include <QtGui>
#include <QtCore>
#include <QLabel>
#include <QString>

class Text: public QLabel
{
    Q_OBJECT
public:

    Text()
        :finish(false),m_inc(0),m_next(0)
    {
        QLabel::setWordWrap(true);
        setWordWrap(true);

    }

    void setText ( const QString & text )
    {
        m_inc=0;
        finish=false;
        m_text = text;
    }


    void changed()
    {
        if(!finish) m_next=0;

        if (!finish && m_inc <= m_text.size())
        {

            QLabel::setText(m_text.left(m_inc));
            m_inc ++;
        }
        else
        {
            finish=true;
            m_next+=1;
            if(m_next/25%2==0)
                showAll();
            else
                showAll_2();
        }
    }

    void showAll()
    {
        QLabel::setText(m_text);
        finish=true;

    }

    void showAll_2()
    {
        QLabel::setText(m_text+"   ▼");


    }

    bool finish;

private:
    int m_inc;
    int m_next;

    QString m_text;
};

#endif // TEXT_H
