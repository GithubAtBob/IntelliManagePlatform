#ifndef INDEX_H
#define INDEX_H

#include "btnIdxEnum.h"
#include <QMainWindow>
#include <QPoint>
#include <QWidget>
#include <windows.h>
#include <windowsx.h>

class QLabel;
class QPushButton;
class QButtonGroup;
class QVBoxLayout;
class VideoSurveil;
class index : public QMainWindow
{
    Q_OBJECT

public:
    index(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent* size);

private:
    //初始化界面
    void InitUi();

    //移动按钮
    void MoveBtn();

    //添加页面
    void AddWidget(btnIdx idx, QWidget* wdgt);

    //设置按钮属性
    void SetTitleBtnAttr(QPushButton* btn, QFont font);

    void SetCloseBtnAttr(QPushButton* btn);

    void SetIdxBtnAttr(QPushButton* btn, std::pair<int, int>& mvDist, const QString& iconPath, btnIdx idx,const QString& name);

    //显示当前页面
    void ShowCurrWdgt();

    //隐藏或显示标题按钮
    void ShowHideTitleBtn();

    //加载数据单例
    void LoadDataInstance();
/***窗体拖拽用***/
    int m_boundaryWidth = 4;

    QPoint m_clickPos;

    int m_titleHeight = 54;

    bool m_double = false;
/**************/

    //页面集合
    std::vector<QWidget*> m_widgetVec;

    //标题栏
    QWidget* m_titleBar;

    //当前界面下标
    int m_currIdx = 0;

    //前一次选中下标
    int m_lastIdx = -1;

    //头像
    QLabel* m_headLbl;

    //用户名
    QLabel* m_unLbl;

    //下拉框
    QPushButton* m_dropdownBtn;

    //放大按钮
    QPushButton* m_maxBtn;

    //缩小按钮
    QPushButton* m_minBtn;

    //关闭按钮
    QPushButton* m_closeBtn;

    //标题栏按钮
    QButtonGroup* m_titleGrp;

    //退出按钮
    QButtonGroup* m_backGrp;

    //未完全显示的按钮
    QPushButton* m_remainBtn;

    //当前标题栏按钮位置
    int m_currPos = 563;

    //标题栏按钮最大位置
    int m_maxPos = 1400;

    //Logo 宽度
    int m_LogoWeight = 350;

    //中心窗体
    QWidget* m_mainWdgt;

    //总布局
    QVBoxLayout* m_mainLyt;

    //视频预览
    VideoSurveil* m_videoSurveil;
 private slots:
    //标题栏按钮点击
    void TitleButtonJudge(int idx);

    //退出按钮点击
    void BackButtonJudge(int idx);
};
#endif // INDEX_H
