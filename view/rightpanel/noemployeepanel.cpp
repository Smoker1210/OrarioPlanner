#include "noemployeepanel.h"
#include <QVBoxLayout>
#include <QLabel>
namespace GUI{
noEmployeePanel::noEmployeePanel(QWidget* parent): QWidget(parent) {
    QVBoxLayout* layoutPrincipale = new QVBoxLayout(this);
    QLabel* mainText = new QLabel(this);
    QString html = R"(
        <h2>Benvenuto in OrarioPlanner!</h2>
        <p>Per iniziare a fare uso del programma abilita dei dipendenti dalla schermata dipendenti</p>
    )";
    mainText->setStyleSheet("background-color:transparent;");
    mainText->setAlignment(Qt::AlignCenter);
    mainText->setText(html);
    layoutPrincipale->addWidget(mainText, Qt::AlignCenter);
}
}