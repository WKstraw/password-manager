#include "handler.h"

//TODO: find where mdps.txt is created and remove that
//      Add COPY btn
//      troubleshoot the fact that QACtion (QMenu) is also beige color (stylesheet appended on qApp)

Handler* Handler::instance = 0;

Handler* Handler::getInstance()
{
    {
        if(instance == 0)
        {
            instance = new Handler;
        }
        return instance;
    }
}
Handler* Handler::getInstance(QString currentUsername, QString currentPassword, QWidget *parent)
{
    {
        if(instance == 0)
        {
            instance = new Handler(currentUsername,currentPassword, parent);
        }
        return instance;
    }
}

int Handler::getNumber()
{
    return *number;
}


Handler::Handler(QString user, QString pass, QWidget *parent) : QWidget(parent)
{
    qDebug("Handler with username+ created");

    *username = user;
    *password = pass;
    fileToSave = new QFile("C:/Users/Victor/Documents/Victor/passwordKeeper/Users/" +*username+"/passwords.psw"); //Change the path to PATH defined before release.
    QDir dir("C:/Users/Victor/Documents/Victor/passwordKeeper/Users/" +*username);
    if(!dir.exists())
    {
        qWarning() << "dir does not exist";
        dir.mkpath(dir.path());
    }
    if (!fileToSave->exists())
    {
        if(!fileToSave->open(QFile::WriteOnly)){
        qWarning() << "couldn't create file.";}

    else{fileToSave->close();}}

    externalList->clear();
    SecondList->clear();          //Just in case (to lazy to test if actually needed)


}
Handler::Handler()
{
  qDebug("Handler Created");
  *number = 0;
}

void Handler::giveInfo(QByteArray mdp, QByteArray site)
{
    QString tempM = QString::fromLocal8Bit(mdp);
    QString tempS = QString::fromLocal8Bit(site);
    createPasswordWidget(tempM,tempS);
}

    //pass mdp + site to MyWidg constructor, then add the created widg to list.

    /*

    Idea: the position in the list of the widget will be what lets us identify the widget (and further delete it if needed
    (Second list with link between password,site + wdget (that is returned to mainWin(shower.cpp/h)?) that HAS to be saved on a file
    So when user re-opens it,MyWidg are reloaded according to this file (with the passwords); (the save procedure can be done in handler?,
    therefore link with shower?))

    Idea Follow-up: connect() between shower and Handler: when Handler will emit a signal, shower will "update", which will load MyWidgets from list.

    */
QByteArray Handler::encryptMoreLikeDecryptHex(QByteArray Hex_toEnDe)
{
    QByteArray toED = QByteArray::fromHex(Hex_toEnDe);  //this will transform the hex saved in file to a decrypted byte array

    QByteArray k; //copyPaste from other E/D function
    k=password->toLocal8Bit();

    QByteArray output;
    for(int i=0; i<toED.size();i++)
    {
        int a = i % (sizeof(k) / sizeof(char));
        output[i] = toED[i] ^ k[a];
    }

    qWarning() << output;

    return output;
}

QByteArray Handler::encryptDecrypt(QString ToEncryptDecrypt)
{
    QByteArray toED;
    QByteArray k;

    toED = ToEncryptDecrypt.toLocal8Bit();
    k=password->toLocal8Bit();

    QByteArray output;
    for(int i=0; i<toED.size();i++)
    {
        int a =i % (sizeof(k) / sizeof(char));
        output[i] = toED[i] ^ k[a]; //?? TODO
                            //when adding a letter/number to passW, encrypted hex does not change...
    }

    //qWarning() << output;

    return output;

}

void Handler::addNewPassword(QString pass, QString site)
{
    qDebug("addNewPass called");
    //this slot will be connected to QAction of shower when the user creates a new password
    createPasswordWidget(pass,site);
}

void Handler::createPasswordWidget(QString p, QString s, int onCreationId)
{
    //function called when MyWidg must be created
    if(onCreationId < 0)
    {qDebug("widget created without input ID");
    MyWidget *widg = new MyWidget(p,s,*number);
    externalList->append(widg);
    QVector<QString> vect;
    vect << p << s << QString::number(*number);
    *number+=1;
    SecondList->append(vect);
    emit uploadView(widg);
    qDebug("uploadView emitted");
    }
    else  //this can happen when handler is loaded (to load already saved pass)
    {qDebug("widget created with input ID");
        MyWidget *widg = new MyWidget(p,s,onCreationId);

        externalList->append(widg);
        emit uploadView(widg);
        qDebug("uploadView emitted");
    }
}
void Handler::changePassword(QString newPassword, int id)
{
    /*if the for loop takes too much time, research for a way in
     * which id = index in the list (and when delete
     * i.e. index 2, index 3,4,5,... stay at the same place */
    QString nP = newPassword;

    int i = 0;
    bool loopBreaker = false;

    while (!loopBreaker)    //for (int i = 0; i<SecondList->size();i++)
    {
        if (SecondList->at(i).at(2) == QString::number(id))
        {
            (*SecondList)[i][1] = nP; //Changes the value for password in the secondlist
            loopBreaker = true;
            saveList();
        }
        i++;
    }
}

void Handler::saveList() //TODO:as every password is on a new line, can reduce save time by just changing the lines on which widgets have changed
{   qWarning() << "Slllsls";

    QTextStream *ts;

    //must append the value of the list in the file defined in constructor after encrypting them
    if(fileToSave->open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text)) //truncate erases all previous data in file
    {
       ts = new QTextStream(fileToSave);
    }
    else
    {
        qWarning() << "error";
        qWarning() << fileToSave->error();
    }


    for (int i = 0; i<SecondList->size();i++)
    {
        for (int j=0 ; j <= 2; j++) //j <= 2 is the same as j <= secondList[i][j] (as the size of the inlisted list is always 3)
        {
            //TODO: BIG PROBLEM : xor encryption sometime returns "unacceptable" string characters --> must find a key that is valid for every character that is encrypted.
            //Ideas: maybe have the key to be a constant (its hexadecimal value would be higher than any "unacceptable" string character, therefore  when a word is encrypted, the adding-up proccess will not be able to return a "banned" value (ex.: "\n")
            //Second (simpler) idea: file input will be to hexa or binary or byte(?) values of the encrypted objects themselves.
            QByteArray hexData = encryptDecrypt((*SecondList)[i][j]);
            hexData = hexData.toHex();
            qWarning() << hexData;
            if(j<2){
            *ts << hexData << ",";  //comma-separated
        }
            else{
                *ts << hexData;
            }
            }
        *ts << endl;
    }
    fileToSave->close();
}
void Handler::readList()
{ qDebug() << "started reading";
    qDebug() << "-----------------------------------------------------";
    SecondList->clear(); //erases all items as they will be loaded from the file.
    if (!fileToSave->open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << "error read list";
        qWarning() << fileToSave->error();
    }
    if(fileToSave->size() == 0)
    {
        fileEmpty = true;
    }
    if(!fileEmpty || !(SecondList->isEmpty()))
    {
        fileEmpty = false;
        QTextStream ts(fileToSave);
        QString tempS;

        while(!ts.atEnd())
        {
            tempS = ts.readLine(); //gather the data

            QStringList sL = tempS.split(",");
            QVector<QString> vectS;
            for (int i=0;i<sL.size();i++)
            {
                //every string is a hex value of the real value
                //creating a byteArr translating hex value of string :
                QString s = sL[i];

                QByteArray bytA = encryptMoreLikeDecryptHex(s.toLocal8Bit());

                vectS << QString::fromLocal8Bit(bytA);
            }
            SecondList->append(vectS); //appending the newly created valueLists to "secondList"
        qWarning() << "-----------------------------------------------------";
        }

    }
    if(externalList->isEmpty() || processOfWidgetChange)
    {
        externalList->clear();
        for(int i=0; i<SecondList->size();i++)
        {
            QString s =(*SecondList)[i][0];
            QString p = (*SecondList)[i][1];
            QString id = (*SecondList)[i][2];
            int id1 = id.toInt();
            createPasswordWidget(p,s,id1);
        }
        if(processOfWidgetChange)
        {
            processOfWidgetChange = false;
        }
    }
    fileToSave->close();
    if(!fileEmpty || !(SecondList->isEmpty()))
    {
        setFirstLast();
    }
    changerNumber();
    qDebug() << "stopped reading";
}
Handler::~Handler()
{
    qWarning() << "destructor called";
    saveList();
}
void Handler::changerNumber()
{
    if(externalList->isEmpty())
    {
    *number = 0;}
    else{
        int highestNumber = (*SecondList)[0][2].toInt();
        for(int i = 0;i<SecondList->size(); i++)
        {
            if((*SecondList)[i][2].toInt() > highestNumber)
            {highestNumber = (*SecondList)[i][2].toInt();}
        }
        *number = highestNumber;
        *number += 1;
    }
}
bool Handler::initialiseData()
{
    if (permissionToInit)
    {readList();
    permissionToInit = false;
    return true;
    }
    else{
        return false;
    }
    }
void Handler::changeMasterP(QString newMP)
{
    *password = newMP;
    saveList();
}
void Handler::buttonClicked(int button, int id)
{
    switch(button){

    case MyWidget::Button_delete:{
        processOfWidgetChange = true;
        //Must: delete from second list, save list, reset layout, recreate layout

        //Delete from list:
        for(int i = 0; i<SecondList->size(); i++)
        {
            QString num = (*SecondList)[i][2];
            if (num.toInt() == id)
            {
                qWarning() << "deleted at id: " + QString::number(id);
                SecondList->remove(i); //TODO: Undo option --> save deleted element in another file before deletion
            }

        }
        saveList();
        emit resetL();

        break;
    }
    case MyWidget::Button_New:{
        emit nPass();
        setFirstLast();
        break;
    }
    case MyWidget::Button_up:{
        //Must change order in SecondL, saveL, readL;
        processOfWidgetChange = true;

        //test if id is not first in list (as cannot be moved up)
        if((*SecondList)[0][2].toInt() != id)
        {
            for(int i = 0; i< SecondList->size();i++)
            {
                QString num = (*SecondList)[i][2];
                if (num.toInt() == id)
                {
                    SecondList->move(i,(i-1));
                    externalList->move(i,(i-1));
                    break;
                }
            }
            saveList();
            emit resetL();
        }

        break;
    }
    case MyWidget::Button_Down:{
        processOfWidgetChange = true;

        //test if id is not last in list (as cannot be moved down)
        if((*SecondList)[(SecondList->size()-1)][2].toInt() != id)
        {
            for(int i = 0; i< SecondList->size();i++)
            {
                QString num = (*SecondList)[i][2];
                if (num.toInt() == id)
                {
                    SecondList->move(i,(i+1));
                    externalList->move(i,(i+1));
                    break;
                }
            }
            saveList();
            emit resetL();
        }

        break;
    }
    case MyWidget::Button_Copy:
    {
        emit animCopy();}
    }
}
void Handler::rL()
{
    readList();
}
void Handler::setFirstLast()
{
    int firstId = (*SecondList)[0][2].toInt();
    int lastId = (*SecondList)[(SecondList->size() - 1)][2].toInt();

    for(int i=0; i< externalList->size();i++)
    {
        int currentWidId = (*externalList)[i]->getPosInList();
        if(currentWidId == firstId)
        {(*externalList)[i]->setFirst();}
        else if(currentWidId == lastId)
        {(*externalList)[i]->setLast();}
        else{(*externalList)[i]->setDefault();}
    }

}
