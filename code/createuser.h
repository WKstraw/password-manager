#ifndef CREATEUSER_H
#define CREATEUSER_H

#include "QtWidgets"
#include "qwidget.h"
#include <QCryptographicHash>

class createUser : public QWidget
{
Q_OBJECT
public:
    createUser();
    void yeahJustDoIt(QString userame, QString password);
    QByteArray *byt = new QByteArray;
    void static changeFile(QString u, QString p);
private:
    QLineEdit *nU = new QLineEdit;
    QLineEdit *nP1 = new QLineEdit;
    QLineEdit *nP2 = new QLineEdit;
    QDir *dir = new QDir("C:/Users/Victor/Documents/Victor/passwordKeeper/Users");
    QByteArray *arayedPassword = new QByteArray;
public slots:
    void actuallyCreateTheUser();
    void showPa(int a);
signals:
    void closssseeee(QByteArray arrayedUsername);
};




#endif // CREATEUSER_H
