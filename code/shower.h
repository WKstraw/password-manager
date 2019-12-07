#ifndef SHOWER_H
#define SHOWER_H
#pragma once
#include <QtWidgets>
#include <QMainWindow>
#include <QLayout>
#include <qtextstream.h>
#include <QGlobal.h>
#include <QTime>
#include "QCryptographicHash"
#include "createuser.h"
#include "handler.h"
#include "perswidget.h"

class shower : public QMainWindow
{Q_OBJECT
public :
    shower(QString motDePasse,QString username);
private:
    QByteArray encryptDecrypt(QString ToEncryptDecrypt,QString key);
    QString *password = new QString;
    QFile *mdps;
    QLineEdit *password1 = new QLineEdit;
    int randInt(int high,int low = 0);
    int numberOfRows;
    QString *username1 = new QString;
    QString path = QString("C:/Users/Victor/Documents/Victor/passwordKeeper/Users");
    Handler *handler;
    QVBoxLayout *layout = new QVBoxLayout;
    QScrollArea *scrollArea = new QScrollArea;
    QVBoxLayout *innerSALayout = new QVBoxLayout;
    void clearLayout(QLayout *layout);
    QLabel *confirmationLabel;
    bool anFin = true;
public slots:
    void newPassword();
    void dbox();
    void changeMPass();
    void updateL(MyWidget *widget);
    void resetLayout();
    void aCop();
    void contA();
signals:
    void nP(QString newP,QString nwS);

};
#endif // SHOWER_H
