#ifndef FENPRINC_H
#define FENPRINC_H

#include "qformlayout.h"
#include "qlineedit.h"
#include "qtextstream.h"
#include "qfile.h"
#include "qstring.h"
#include "qwidget.h"
#include "qbytearray.h"
#include "qdir.h"
#include "qformlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qobject.h"
#include "qmessagebox.h"
#include "qfile.h"
#include "qiodevice.h"
#include "qcryptographichash.h"
#include "qtextstream.h"
#include "createuser.h"
#include "shower.h"
#include "qlayoutitem.h"

class fenPrinc : public QWidget
{
Q_OBJECT

public:
    fenPrinc();
private:
    QLineEdit *u1 = new QLineEdit;
    QLineEdit *p1=new QLineEdit;
    void verif(QString username, QString pw);
    QByteArray *newUserArray = new QByteArray;
    const int width =230;
    const int height=176;

public slots:
    void verificationnn();
    void createNewUser();
    void gotTheMessage(QByteArray arrayedUsername);
    void showPa(int a);
signals:
    void enterIsPressed();

};

#endif // FENPRINC_H
