#include "helppanel.h"
#include "qevent.h"
#include <QBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QFile>
#include <QTextStream>
namespace GUI{
helpPanel::helpPanel(QWidget *parent): QWidget{parent}{
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    QLabel* Titolo = new QLabel("Aiuto",this);
    QFont fontTitolo = Titolo->font();
    fontTitolo.setBold(1);
    fontTitolo.setPointSize(24);
    Titolo->setFont(fontTitolo);
    Titolo->setAlignment(Qt::AlignCenter);
    layoutPrincipale->addWidget(Titolo);
    // QLabel* infoShift = new QLabel("Funzionamento tabella e intestazione turni", this);
    // QFont fontSottotitolo = infoShift->font();
    // fontSottotitolo.setBold(1);
    // fontSottotitolo.setPointSize(16);
    // infoShift->setFont(fontSottotitolo);
    // layoutPrincipale->addWidget(infoShift);
    testo = new QTextBrowser(this);
    testo->setReadOnly(true);
    testo->setOpenExternalLinks(true);
    testo->setFrameShape(QAbstractScrollArea::NoFrame);
    testo->setStyleSheet({"background-color: transparent;"});
    loadText();
    layoutPrincipale->addWidget(testo);
    QHBoxLayout* buttonRow = new QHBoxLayout();
    buttonRow->addStretch();
    backButton = new QPushButton("Indietro",this);
    buttonRow->addWidget(backButton);
    layoutPrincipale->addLayout(buttonRow);
    //connect
    connect(backButton, &QPushButton::clicked, this, &helpPanel::onBackPressed);
}

void helpPanel::loadText(){
    QFile file(":/resources/html/helpPage.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        stream.setCodec("UTF-8");
#endif
        testo->setHtml(stream.readAll());
        file.close();
    }
}

void helpPanel::keyPressEvent(QKeyEvent* event){
    if(event->key() ==  Qt::Key_Escape){
        emit onBackPressed();
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}
}