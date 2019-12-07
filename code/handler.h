#pragma once

#ifndef HANDLER_H
#define HANDLER_H

#include "QtWidgets"
#include "perswidget.h"

class MyWidget;

class Handler : public QWidget
{Q_OBJECT
    static Handler *instance;
public:
    static Handler *getInstance();
    static Handler *getInstance(QString currentUsername, QString currentPassword, QWidget *parent = nullptr);

    int getNumber(); //was used for test reason
    QByteArray encryptDecrypt(QString ToEncryptDecrypt);
    QByteArray encryptMoreLikeDecryptHex(QByteArray Hex_toEnDe);
    ~Handler();
    bool initialiseData();
    void changeMasterP(QString newMP);
    void rL();
private:
    Handler();
    Handler(QString user, QString pass, QWidget *parent = nullptr);
    QString *username = new QString;
    QString *password = new QString;
    QFile *fileToSave;
    QVector<MyWidget *> *externalList = new QVector<MyWidget *>;
    QVector<QVector<QString>> *SecondList = new QVector<QVector<QString>>; //this list contains password + site + widgetId
    int *number = new int;
    void giveInfo(QByteArray mdp, QByteArray site);
    void createPasswordWidget(QString p,QString s,int onCreationId = -1);
    void saveList();
    void readList();
    void changerNumber();
    bool permissionToInit = true;
    bool processOfWidgetChange = false;
    void setFirstLast();
    bool fileEmpty = false;
signals:
    void uploadView(MyWidget *widget);
    void resetL();
    void nPass();
    void animCopy();
public slots:
    void addNewPassword(QString pass,QString site);
    void changePassword(QString newPassword,int id);
    void buttonClicked(int button,int id);
};
/*TODO: handle password change signal emitted from perswidget  DONE
 *      save "function" DONE
 *      read "funcion"  DONE
 *      edit widget "function(s)" --> must also create new slots in MyWidg class
 *      |
 *       -----> linked with change place in layout (layout.delete?, see QLayout doc)
 *      handle external passWord change (shower action from "file->changeP")
 *
*/


#endif // HANDLER_H
