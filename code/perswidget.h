#pragma once

#ifndef PERSWIDGET_H
#define PERSWIDGET_H


#include "qstring.h"
#include "qwidget.h"
#include "qbytearray.h"
#include "qlineedit.h"
#include "qdir.h"
#include "qformlayout.h"
#include "qlabel.h"
#include "QPushButton"
#include "qcheckbox.h"
#include "qobject.h"
#include "qmessagebox.h"
#include "qfile.h"
#include "qiodevice.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qevent.h"
#include "qguiapplication.h"
#include "qclipboard.h"
#define UNDEF -1

#include "handler.h"
class Handler;

class MyWidget : public QFrame
{
    Q_OBJECT
public:
    MyWidget(QString mdp, QString site,QWidget *parent = nullptr,QSize size = QSize(400,400));
    MyWidget(QString mdp, QString site,int numberInList,QSize size = QSize(400,400),QWidget *parent = nullptr);
    int getPosInList();
    bool changePosInList(int *newNumber);
    Handler *handler;
    enum button_enum{
        Button_up = 1,
        Button_Down = 2,
        Button_delete = 3,
        Button_New = 4,
        Button_Copy = 5
    };
    void setFirst();
    void setLast();
    void setDefault();
private:
    QSize widgSize;
    void constructor(QString mdp,QString site); //Function used to avoid copy/paste of the same code in the two different constructors
    int *id = new int;
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QFormLayout *siteL = new QFormLayout();
    QLabel *siteText = new QLabel("Site: ");
    QLabel *siteLab = new QLabel();
    QFormLayout *passL = new QFormLayout();
    QLabel *passT = new QLabel("Password: ");
    QLineEdit *passEdit = new QLineEdit;
    QGridLayout *buttonsLayout = new QGridLayout;
    QPushButton *button_up = new QPushButton;
    QPushButton *button_down = new QPushButton;
    QPushButton *button_plus = new QPushButton;
    QPushButton *button_delete = new QPushButton;
    QPushButton *button_copy = new QPushButton;
    bool *editing = new bool;
    void editChange();
    QString currentPassword;
    bool isFirst = false;
    bool isLast = false;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void signalPasswordChange(QString newPassword,int widgetId);
    void notifyDelete(int id);
    void butClicked(int button,int id);
public slots:
    void upClick();
    void downClick();
    void delClick();
    void newClick();
    void copyClick();

    /*
     * (connect must be in the mywidget, and not handler??, as handler
     * may not know which widget itcomees from, EXCEPT if number is
     * also passed!!!!), therefore must be double conncetion
     * (handler-->mywidg, mywidg-->handler)
     * (only needed if number is not passed, like said before)
     */
};

#endif // PERSWIDGET_H
