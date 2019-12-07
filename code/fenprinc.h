#ifndef FENPRINC_H
#define FENPRINC_H

#include <QFormLayout>
#include <QtWidgets>
#include <QTextStream>
#include <QFile>
#include <createuser.h>
#include "shower.h"

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
    const int height=186;

public slots:
    void verificationnn();
    void createNewUser();
    void gotTheMessage(QByteArray arrayedUsername);
    void showPa(int a);
signals:
    void enterIsPressed();

};

#endif // FENPRINC_H
