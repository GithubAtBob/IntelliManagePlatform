#ifndef NODEINFOBOX_H
#define NODEINFOBOX_H

#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QWidget>
#include "treeItemInfo.h"
#include <QPainter>

class VideoWidget;
class VideoWindow;
class nodeInfoBox:public QWidget
{
    Q_OBJECT
public:
    explicit nodeInfoBox(QWidget *parent = nullptr);
    void init();
    void setInfo(TREE_ITEM_INFO* item);
    VideoWidget* GetRealTimeVideoWidget();

signals:

private:
    QLabel* itemNameLab;
    QLabel*  typeLab;
    QLabel*  statLab;
    QPushButton* collectionBtn;
    QPushButton* ModifyLatLong;
    QPushButton* delLatLong;
    QPushButton* closeBoxBtn;

    QPushButton* PreviewBtn;
    QPushButton* ReplayBtn;

    QLabel* backgroundLab;

    QString m_stPuid = "";
    QString m_stChanid = "";
    int m_nFactoryCode = -1;
    bool m_isOnline = false;

    VideoWidget* m_realTimeWidget;
    VideoWidget* m_replayWidget;
    VideoWindow* m_realTimeWindow;
    VideoWindow* m_replayWindow;
};

#endif // NODEINFOBOX_H
