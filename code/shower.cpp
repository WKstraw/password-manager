#include <shower.h>

shower::shower(QString motDePasse, QString username)
{
    setMinimumWidth(508); //ratio (according to 1920): 0.264583
    setMinimumHeight(564); //ratio: 0.5222
    confirmationLabel = new QLabel(scrollArea);
    QString text("Password has been copied!");
    confirmationLabel->setText(text);
    confirmationLabel->adjustSize();
    confirmationLabel->hide();
    showMaximized();
    confirmationLabel->setGeometry(QRect((this->geometry().topLeft().x()/2),0,confirmationLabel->width(),confirmationLabel->height()));
    setAttribute(Qt::WA_QuitOnClose);
    setAttribute(Qt::WA_DeleteOnClose);

    *password = motDePasse;
    *username1 = username; //redone?


    confirmationLabel->setObjectName("CONL");

    QWidget *mainWid = new QWidget;


    //setting up the handler
    handler = Handler::getInstance(username,motDePasse,mainWid);
    connect(handler,SIGNAL(uploadView(MyWidget*)),this,SLOT(updateL(MyWidget*)));
    connect(this,SIGNAL(nP(QString,QString)),handler,SLOT(addNewPassword(QString,QString)));

    connect(handler,SIGNAL(resetL()),this,SLOT(resetLayout())); //for btn del,up,dwn
    connect(handler,SIGNAL(nPass()),this,SLOT(newPassword())); //for btn new
    connect(handler,SIGNAL(animCopy()),this,SLOT(aCop())); //for btn copy

    if(!handler->initialiseData())
    {
        qWarning() << "fatality";
    }

    scrollArea->setWidgetResizable(true);
    QFrame *frame = new QFrame;
    frame->setLayout(innerSALayout);
    scrollArea->setWidget(frame);

    layout->addWidget(scrollArea);
    mainWid->setLayout(layout);

    QMenu *menuFichier = menuBar()->addMenu("&File");
    QAction *nouveauMdp = new QAction("&New Password",this);
    nouveauMdp->setShortcut(QKeySequence("Ctrl+N"));
    nouveauMdp->setToolTip("Add a Password to the Password Manager");
    menuFichier->addAction(nouveauMdp);
    QAction *changeMasterPassword = new QAction("Change &Master Password",this);
    changeMasterPassword->setShortcut(QKeySequence("Ctrl + M"));
    changeMasterPassword->setToolTip("Change your master password (password that is used to connect into the Password Manager App");
    menuFichier->addAction(changeMasterPassword);


    this->setCentralWidget(mainWid);

    connect(nouveauMdp,SIGNAL(triggered(bool)),this,SLOT(newPassword()));
    connect(changeMasterPassword,SIGNAL(triggered(bool)),this,SLOT(changeMPass()),Qt::UniqueConnection);


}
QByteArray shower::encryptDecrypt(QString ToEncryptDecrypt, QString key)
{
    QByteArray toED;
    QByteArray k;

    toED = ToEncryptDecrypt.toLocal8Bit();
    k=key.toLocal8Bit();

    QByteArray output;
    for(int i=0; i<toED.size();i++)
    {
        int a = i % (sizeof(k) / sizeof(char));
        output[i] = toED[i] ^ k[a];
    }

    qWarning() << output;
    return output;

}
void shower::newPassword()
{QDialog *d = new QDialog;

    QFormLayout *layout1 = new QFormLayout;
    QPushButton *ok =new QPushButton("Add");
    QPushButton *cancel =new QPushButton("Cancel");
    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(ok,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancel,QDialogButtonBox::RejectRole);
    QLineEdit *website = new QLineEdit;
    QLabel *mdp = new QLabel("Mot de Passe: ");
    QLabel *site = new QLabel("Site web: ");
    QLabel *nI =new QLabel("No inspiration?");
    QPushButton *createRandom = new QPushButton("Create a random Password!");

    connect(buttonBox,SIGNAL(accepted()),d,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),d,SLOT(reject()));

    layout1->addRow(mdp,password1);
    layout1->addRow(site,website);
    layout1->addRow(nI,createRandom);
    layout1->addRow(buttonBox);
    ok->setFocusPolicy(Qt::ClickFocus);
    cancel->setFocusPolicy(Qt::ClickFocus);


    connect(createRandom,SIGNAL(clicked()),this,SLOT(dbox()));

    d->setLayout(layout1);
    int result = d->exec();

    if (result==QDialog::Accepted)
    {emit nP(password1->text(),website->text());
    password1->setText("");
    }

}

void shower::dbox()
{
    QDialog *d1= new QDialog();
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    QFormLayout *l3 =new QFormLayout;
    QSpinBox *sb = new QSpinBox;
    sb->setMinimum(1);
    sb->setValue(8);
    sb->setMaximum(128);
    QLabel *ch =new QLabel("Number of characters");
    l3->addRow(ch,sb);
    sb->setFocusPolicy(Qt::ClickFocus);
    QPushButton *create = new QPushButton("Create!");
    QPushButton *cancel =new QPushButton("Cancel");
    QDialogButtonBox *bb1 = new QDialogButtonBox;
    bb1->addButton(create,QDialogButtonBox::AcceptRole);
    bb1->addButton(cancel,QDialogButtonBox::RejectRole);
    layoutDialog->addLayout(l3);
    d1->setLayout(layoutDialog);
    connect(bb1,SIGNAL(accepted()),d1,SLOT(accept()));
    connect(bb1,SIGNAL(rejected()),d1,SLOT(reject()));
    QLabel *label = new QLabel("Include:");
    QCheckBox *letters =new QCheckBox("Letters");
    letters->setCheckState(Qt::Checked);
    QCheckBox *capitalLetters =new QCheckBox("Capital Letters");
    capitalLetters->setCheckState(Qt::Checked);
    QCheckBox *numbers =new QCheckBox("Numbers");
    numbers->setCheckState(Qt::Checked);
    QCheckBox *symbols =new QCheckBox("Symbols");
    QCheckBox *specialLetters =new QCheckBox("Accented Letters");
    QHBoxLayout *hLay = new QHBoxLayout;
    hLay->addWidget(label);
    hLay->addWidget(letters);
    hLay->addWidget(capitalLetters);
    hLay->addWidget(numbers);
    hLay->addWidget(symbols);
    hLay->addWidget(specialLetters);
    layoutDialog->addLayout(hLay);
    layoutDialog->addWidget(bb1);

    int result1=d1->exec();
    QStringList totalList;
    if(result1==QDialog::Accepted)
    {
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());

        QStringList listLetters = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
        QStringList listUpperLetters;
        QStringList listNumbers = {"0","1","2","3","4","5","6","7","8","9"};
        QStringList listSymbols = {"&","@","(",")","!","{","}","#","/","\\","\"","\'","$","%","*","-","+","=","€",",",";",".","?",":","<",">","[","]","_"};  //            {"@","(",")","!","{","}","#","/","\\","$","%","*","-","+","=",",",".","?",":","^","[","]","_"};
        QStringList listSpecialLetters= {"é","è","à","ç","ê","ô","ö","ï","î","ñ","â","ë"};
        for(int i=0; i<listLetters.size();i++)
        {QString tstr = listLetters.at(i);
         tstr = tstr.toUpper();
         listUpperLetters<< tstr;}
        if(letters->isChecked())
        {totalList << listLetters;}
        if(capitalLetters->isChecked())
        {totalList << listUpperLetters;}
        if(numbers->isChecked())
        {totalList << listNumbers;}
        if(symbols->isChecked())
        {totalList << listSymbols;}
        if(specialLetters->isChecked())
        {totalList << listSpecialLetters;}
        if (totalList.isEmpty())
        {}
        else{
        QString tempstr;
        for (int a; a<=sb->value();a++)
        {
            int randomValue = randInt((totalList.size()-1));
            tempstr = tempstr + totalList.at(randomValue);
            password1->setText(tempstr);
        }
        d1->close();}
        }

}
int shower::randInt(int high, int low)
{
    return qrand() % ((high + 1) - low) + low;
}
void shower::changeMPass()
{
    QDialog *d = new QDialog;
    QFormLayout *l = new QFormLayout;
    QLineEdit *oldPW = new QLineEdit;
    QLineEdit *newP = new QLineEdit;
    QLineEdit *newPC = new QLineEdit;
    QLabel *oldP = new QLabel("Old Password:");
    QLabel *nP = new QLabel("New password:");
    QLabel *nPc =new QLabel("Confirm new password:");
    l->addRow(oldP,oldPW);
    l->addRow(nP,newP);
    l->addRow(nPc,newPC);
    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    l->addWidget(bb);
    d->setLayout(l);
    connect(bb, SIGNAL(accepted()), d, SLOT(accept()));
    connect(bb, SIGNAL(rejected()), d, SLOT(reject()));

    int result = d->exec();
    if(result == QDialog::Accepted)

    {
        //Check if password confirmation matches
        if(newP->text() != newPC->text())
        {
            QMessageBox::warning(this,"Error","Error: The inputed password does not match the confirmattion password!");
        }
        else if(newP->text() == oldPW->text())
        {
            QMessageBox::warning(this,"Error","The Passwords are the same...");
        }
        else if(newP->text().length() < 8 || newP->text().trimmed() == "")
        {
            QMessageBox::warning(this,"Warning","The new Password is not safe enough...");
        }
        else{
        QString passw = newP->text();
        createUser::changeFile(*username1,passw);
        handler->changeMasterP(passw);
        }
}

}
void shower::updateL(MyWidget *widget)
{
    qDebug("slotFen");
    innerSALayout->addWidget(widget);
}
void shower::clearLayout(QLayout *layout)
{QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}
void shower::resetLayout()
{
    clearLayout(innerSALayout);
    handler->rL();
}
void shower::aCop()
{
    if(anFin){
    anFin = false;
    confirmationLabel->setGeometry(((this->width()/2)- (confirmationLabel->width()/2)),0,confirmationLabel->width(),confirmationLabel->height()+10);
    confirmationLabel->show();


    QPropertyAnimation *an = new QPropertyAnimation(confirmationLabel,"geometry",confirmationLabel);
    an->setDuration(2400);
    connect(an,SIGNAL(finished()),this,SLOT(contA()));

    an->setKeyValueAt(0,confirmationLabel->geometry());
    an->setKeyValueAt(0.86,QRect(confirmationLabel->geometry().topLeft().x(),(this->height()/2),confirmationLabel->width(),confirmationLabel->height()));
    an->setKeyValueAt(1,QRect(confirmationLabel->geometry().topLeft().x(),0,confirmationLabel->width(),confirmationLabel->height()));
    an->start();
    }

}
void shower::contA()
{
    anFin = true;
    confirmationLabel->hide();
    confirmationLabel->setGeometry(confirmationLabel->geometry().topLeft().x(),confirmationLabel->geometry().topLeft().y(),confirmationLabel->width(),confirmationLabel->height()-10);
}


