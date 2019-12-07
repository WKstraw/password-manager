#include "createuser.h"

createUser::createUser()
{
    QFormLayout *layout = new QFormLayout;
    QLabel *nU1 = new QLabel;
    QLabel *nP3 = new QLabel;
    QLabel *nP4 = new QLabel;
    QPushButton *create = new QPushButton;
    QCheckBox *showP = new QCheckBox("Show Password");
    nP1->setEchoMode(QLineEdit::Password);
    nP2->setEchoMode(QLineEdit::Password);

    nU1->setText("Create yourself a username:");
    nP3->setText("New Password:");
    nP4->setText("Confirm password:");
    create->setText("Create User");
    showP->setFocusPolicy(Qt::ClickFocus);
    create->setFocusPolicy(Qt::ClickFocus);

    layout->addRow(nU1,nU);
    layout->addRow(nP3,nP1);
    layout->addRow(nP4,nP2);
    layout->addRow(showP);
    layout->addRow(create);

    setLayout(layout);

    QObject::connect(create,SIGNAL(clicked(bool)),this,SLOT(actuallyCreateTheUser()));
    connect(showP,SIGNAL(stateChanged(int)),this,SLOT(showPa(int)));
}
void createUser::actuallyCreateTheUser()
{
    if (nU->text().trimmed().isEmpty())
    {QMessageBox::warning(this,"Error","Please enter a valid username");
    nU->setText("");}
    else if(nP1->text() != nP2->text() || nP1->text().trimmed().isEmpty())
    {QMessageBox::warning(this,"Error","The passwords do not match");
    nP1->setText("");
    nP2->setText("");}
    else if (nP1->text().length() < 8)
    {
        QMessageBox::warning(this,"Error","The password is too weak. Please make it at least 8 charachters long");
        nP2->setText("");
    }
    else
    {
        yeahJustDoIt(nU->text(),nP1->text());
    }
}
void createUser::yeahJustDoIt(QString userame, QString password)
{
    if (!dir->mkdir(userame))
        {QMessageBox::warning(this,"Error","Username already in use or could not create a folder");}
    else{dir->mkdir(userame);
    QFile *file = new QFile ("C:/Users/Victor/Documents/Victor/passwordKeeper/Users/"+userame+"/hashedPassword.txt");
    file->open(QIODevice::WriteOnly);
    file->close();
    QString usernameOfNewUser;
    usernameOfNewUser = userame;
    *byt = usernameOfNewUser.toLocal8Bit();
    changeFile(userame, password);
    QFile *file2 = new QFile("C:/Users/Victor/Documents/Victor/passwordKeeper/Users/"+userame+"/passwords.txt");
    if (!file2->open(QIODevice::WriteOnly))
    {qWarning("errrrrror");}
    emit closssseeee(*byt);
    this->close();}
}
void createUser::showPa(int a)
{
    if (a==0)
    {nP1->setEchoMode(QLineEdit::Password);
     nP2->setEchoMode(QLineEdit::Password);}
    else if(a==2)
    {nP1->setEchoMode(QLineEdit::Normal);
     nP2->setEchoMode(QLineEdit::Normal);}
}
void createUser::changeFile(QString u, QString p)
{
    QFile *fffff = new QFile ("C:/Users/Victor/Documents/Victor/passwordKeeper/Users/"+u+"/hashedPassword.txt");
    if(!fffff->open(QIODevice::WriteOnly|QIODevice::Text))
    {qWarning("can't open the file");}
            else
    {qWarning("file opened");
     QCryptographicHash sha3_512(QCryptographicHash::Sha3_512);
     QByteArray *passwordArray = new QByteArray;
     *passwordArray = p.toLocal8Bit();
     sha3_512.addData(passwordArray->constData());
     QByteArray *hashedPassword = new QByteArray;
     *hashedPassword = sha3_512.result().toHex();
     QTextStream stream(fffff);
     QString str = QString::fromLatin1(*hashedPassword);
     stream << str;
    }
}

