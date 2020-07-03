#ifndef LOGIN_H
#define LOGIN_H

#include "qv3videoplayinterface.h"
#include <QtWidgets/QMainWindow>
#include <QPoint>

class QLineEdit;
class index;
class login : public QMainWindow
{
    Q_OBJECT

public:
    login(QWidget *parent = Q_NULLPTR);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *e);

private:
    //初始化界面
    void InitUi();

    //窗口移动用
    bool m_bPressed = false;

    QPoint m_point = QPoint(0,0);

    //绘制边框图层
    QWidget* m_unWidget;

    QWidget* m_pwdWidget;

    //用户名编辑框
    QLineEdit* m_unEdit;

    //密码编辑框
    QLineEdit* m_pwdEdit;

    //用户名
    QString m_un = "";

    //密码
    QString m_pwd = "";

    //是否保存密码
    bool m_savePwd = false;

    //ip
    QString m_ip;

    //端口
    int m_port;

    //主页面
    index* m_index;

    QV3VideoPlayInterface m_v3VideoPlayInterface;
private slots:
    //登录按钮槽函数
    void LoginSlot();
};

#endif // LOGIN_H
