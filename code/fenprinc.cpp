#include "fenprinc.h"

fenPrinc::fenPrinc()
{

    QFormLayout *layout = new QFormLayout;
    QLabel *u = new QLabel;
    QLabel *p = new QLabel;
    QPushButton *access = new QPushButton;
    QPushButton *newUser =new QPushButton;
    QSpacerItem *spacer =new QSpacerItem(1,3);
    QCheckBox *showP = new QCheckBox("Show Password");
    p1->setEchoMode(QLineEdit::Password);

    access->setObjectName("ac");
    newUser->setObjectName("nU");
    this->setFixedSize(width,height);

    u->setText("Username:");
    p->setText("Password:");
    access->setText("Access!");
    newUser->setText("Create a new user");
    access->setFocusPolicy(Qt::ClickFocus);
    newUser->setFocusPolicy(Qt::ClickFocus);
    showP->setFocusPolicy(Qt::ClickFocus);

    layout->addRow(u,u1);
    layout->addRow(p,p1);
    layout->addRow(showP);
    layout->addItem(spacer);
    layout->addRow(access);
    layout->addRow(newUser);
    this->setLayout(layout);

    connect(access,SIGNAL(clicked(bool)),this,SLOT(verificationnn()));
    connect(u1,SIGNAL(returnPressed()),this,SLOT(verificationnn()));
    connect(p1,SIGNAL(returnPressed()),this,SLOT(verificationnn()));
    connect(newUser,SIGNAL(clicked(bool)),this,SLOT(createNewUser()));
    connect(showP,SIGNAL(stateChanged(int)),this,SLOT(showPa(int)));
}

void fenPrinc::verif(QString username, QString pw)
{
    QCryptographicHash sha3_512(QCryptographicHash::Sha3_512);
    QByteArray *passwordArray = new QByteArray;
    *passwordArray = pw.toLocal8Bit();
    sha3_512.addData(passwordArray->constData());
    QString *path = new QString("C:/Users/Victor/Documents/Victor/passwordKeeper/Users/"+username);
    QDir dir = QDir(*path);
    QFile *file2 =new QFile(*path+"/hashedPassword.txt");
    QTextStream *stream = new QTextStream(file2);
    if (!dir.exists() || u1->text().isEmpty())
    {qWarning("wrong username");}
    else if(!file2->open(QIODevice::ReadOnly))
    {qWarning("file not oppened");}
    else
    {
        if (sha3_512.result().toHex() !=stream->readLine())
        {qWarning("wrong password");}
        else
        {qWarning("nice");
        shower *kjqdf = new shower(pw,username);
        kjqdf->show();
        this->close();}
    }
    }

void fenPrinc::verificationnn()
{
        verif(u1->text(),p1->text());
}
void fenPrinc::createNewUser()
{
    createUser *cUdsfqkjb =new createUser();
    cUdsfqkjb->show();
    QObject::connect(cUdsfqkjb,SIGNAL(closssseeee(QByteArray)),this,SLOT(gotTheMessage(QByteArray)));
}
void fenPrinc::gotTheMessage(QByteArray arrayedUsername)
{
    *newUserArray = arrayedUsername;
    QString newUser = QString::fromLatin1(*newUserArray);
   qWarning(newUserArray->data());

    u1->setText(newUser);
}

void fenPrinc::showPa(int a)
{
    if (a==0)
    {p1->setEchoMode(QLineEdit::Password);}
    else if(a==2)
    {p1->setEchoMode(QLineEdit::Normal);}
}
