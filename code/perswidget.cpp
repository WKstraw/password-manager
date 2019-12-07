#include "perswidget.h"

MyWidget::MyWidget(QString mdp, QString site, QWidget *parent,QSize size) :QFrame(parent)
{
    qDebug("widget created without ID");
    *id = UNDEF;
    widgSize = size;
    constructor(mdp,site);

}
MyWidget::MyWidget(QString mdp, QString site, int numberInList, QSize size, QWidget *parent) : QFrame(parent)
{

    if (numberInList < 0)
    {
        *id=UNDEF;
    }
    else{*id = numberInList;}
    widgSize = size;
    constructor(mdp,site);

}
int MyWidget::getPosInList()
{
    return *id;
}
void MyWidget::constructor(QString mdp, QString site)
{
    handler = Handler::getInstance();//connection with handler established;
    QObject::connect(this,SIGNAL(signalPasswordChange(QString,int)),handler,SLOT(changePassword(QString,int)));
    this->setFixedSize(widgSize);
    this->setLineWidth(1);
    this->setMidLineWidth(1);
    this->setFrameStyle(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Sunken);
    this->setLayout(mainLayout);
    mainLayout->addSpacing(10);
    QVBoxLayout *layoutSpace = new QVBoxLayout;
    layoutSpace->addSpacing(24);
    layoutSpace->addLayout(siteL);
    mainLayout->addLayout(layoutSpace);
    siteL->addRow(siteText,siteLab);
    siteLab->setText(site);
    mainLayout->addSpacing(30);
    QVBoxLayout *layoutSpaceP = new QVBoxLayout;
    layoutSpaceP->addSpacing(24);
    layoutSpaceP->addLayout(passL);
    mainLayout->addLayout(layoutSpaceP);
    passL->addRow(passT,passEdit);
    passEdit->setStyleSheet("background-color : rgba(0,0,0,0)");
    *editing = false;
    passEdit->setObjectName("Password");
    passEdit->setReadOnly(true);
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->installEventFilter(this);
    passEdit->setText(mdp);
    passEdit->setFrame(false);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonsLayout);  //ICONS of btn set in qss file
    buttonsLayout->addWidget(button_up,0,1);
    button_up->setIcon(QIcon(":/images/arrows-up.png"));
    button_up->setObjectName("UP");
    buttonsLayout->addWidget(button_down,1,1);
    button_down->setIcon(QIcon(":/images/arrows-down.png"));
    button_down->setObjectName("DOWN");
    buttonsLayout->addWidget(button_plus,0,2);
    button_plus->setIcon(QIcon(":/images/plus-green.png"));
    button_plus->setObjectName("NEW");
    buttonsLayout->addWidget(button_delete,1,2);
    button_delete->setIcon(QIcon(":/images/minus-red.png"));
    button_delete->installEventFilter(this);
    button_delete->setObjectName("DEL");
    buttonsLayout->addWidget(button_copy,0,0,2,1,Qt::AlignCenter);
    button_copy->setObjectName("COPY");
    button_copy->setIcon(QIcon(":/images/copy-128.png"));
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    QFile style(":/stylesheets/mywidget.qss");
    QString tempS ="";
    if (style.open(QFile::ReadOnly))
    {
        tempS=style.readAll();
        this->setStyleSheet(tempS);
    }
    QObject::connect(button_delete,SIGNAL(released()),this,SLOT(delClick()));
    QObject::connect(button_down,SIGNAL(released()),this,SLOT(downClick()));
    QObject::connect(button_up,SIGNAL(released()),this,SLOT(upClick()));
    QObject::connect(button_plus,SIGNAL(released()),this,SLOT(newClick()));
    QObject::connect(button_copy,SIGNAL(released()),this,SLOT(copyClick()));
    QObject::connect(this,SIGNAL(butClicked(int,int)),handler,SLOT(buttonClicked(int,int)));
}

bool MyWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == button_delete && (event->type() == QEvent::Enter || event->type() == QEvent::HoverEnter))
    {
        button_delete->setIcon(QIcon(":/images/delete-128.png"));
    }
    else if(watched == button_delete && event->type() == QEvent::Leave)
    {
        button_delete->setIcon(QIcon(":/images/minus-red.png"));
    }
    else if (watched == passEdit && event->type() == QEvent::Enter && *editing == false)
    {
        qWarning() <<"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCc";
            passEdit->setEchoMode(QLineEdit::Normal);
            return true;

    }
    else if (watched == passEdit && event->type() == QEvent::Leave && *editing == false)
        {
            passEdit->setEchoMode(QLineEdit::Password);
            return true;

        }
    else if (watched == passEdit && event->type() == QEvent::MouseButtonRelease && *editing == false)
    {
        QMessageBox *mB = new QMessageBox(QMessageBox::Warning,"Warning!","By clicking you will enable the editing of your password. Once ths is done, you will have to press Enter to submit/save your new password. Press escape at any moment to discard any changes."
                                          ,QMessageBox::Ok|QMessageBox::Cancel);
        int res = mB->exec();
        switch (res){
        case QMessageBox::Ok:
        {
            this->editChange();
            break;
        }
        case QMessageBox::Cancel:
            break;
        }
    }
    else if(watched == passEdit && event->type()==QEvent::KeyPress && *editing == true)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {
            qDebug("enter key");
            QString newPass = passEdit->text();
            qDebug(QString(newPass.at(newPass.size()-1)).toLatin1());
            QString newPassSecret = QString(newPass.size()-2,'*');
            newPassSecret.append(QString(newPass.at(newPass.size()-1)).toLatin1());
            QMessageBox *msgB = new QMessageBox(QMessageBox::Information,
                                            "Confirm","Are you sure? By pressing ok, you will change your recorded password for " + siteLab->text() + " to "+ newPassSecret +" . This is a point of no return.",
                                            QMessageBox::Yes|QMessageBox::Cancel);
            int result = msgB->exec();
            switch (result)
            {
                case QMessageBox::Yes:
                {
                    //change Password
                    passEdit->setText(newPass);
                    emit signalPasswordChange(newPass,*id);
                    editChange();
                    break;
                }
            case QMessageBox::Cancel:
            {
                //go back;
                break;
            }

            }
        }
        else if(keyEvent->key() == Qt::Key_Escape)
        {
            passEdit->setText(currentPassword);
            editChange();
        }
    }
    else{
        return false;
    }
    return false;
}

void MyWidget::editChange()
{
    if (*editing == false)
    {
        currentPassword = passEdit->text();
        *editing = true;
        passEdit->setFrame(true);
        passEdit->setStyleSheet("background-color: rgb(255,255,255)");
        passEdit->setEchoMode(QLineEdit::Normal);
        passEdit->setReadOnly(false);
        button_copy->setEnabled(false);
    }
    else
    {
        *editing = false;
        currentPassword ="";
        passEdit->setFrame(false);
        passEdit->setStyleSheet("background-color: rgba(0,0,0,0)");
        passEdit->setEchoMode(QLineEdit::Password);
        passEdit->setReadOnly(true);
        button_copy->setEnabled(true);
    }
}
void MyWidget::upClick()
{
    emit butClicked(Button_up,*id);
}
void MyWidget::downClick()
{
    emit butClicked(Button_Down,*id);
}
void MyWidget::delClick()
{
    emit butClicked(Button_delete,*id);
}
void MyWidget::newClick()
{
    emit butClicked(Button_New,*id);
}
void MyWidget::copyClick()
{
    QClipboard *c = QGuiApplication::clipboard();
    c->setText(passEdit->text());

    emit butClicked(Button_Copy,*id);

}
void MyWidget::setFirst()
{
    //grey out up-btn

    if(button_up->isEnabled()){
    button_up->setEnabled(false);}
}
void MyWidget::setLast()
{
    //grey out down-btn
    if(button_down->isEnabled()){
    button_down->setEnabled(false);}
}
void MyWidget::setDefault()
{
    button_down->setEnabled(true);
    button_up->setEnabled(true);
}
