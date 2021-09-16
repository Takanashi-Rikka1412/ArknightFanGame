#ifndef NEWMAP_H
#define NEWMAP_H


class newMap
{

public:

    newMap();

    //选择关卡
    void choose(int);

    int m_level;

    //地图数据加载
    int mymap[8][16];

    //是否绘制
    bool ifpaint=false;

    //QPixmap Qmap;
};

#endif // NEWMAP_H
