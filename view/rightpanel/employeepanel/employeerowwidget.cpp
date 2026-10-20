#include "employeerowwidget.h"
#include <QBoxLayout>
#include <QEvent>
namespace GUI{
employeeRowWidget::employeeRowWidget(const QString& name, const QString& surname, int ore, const QString& role, bool active, bool listaSpeciale, QWidget *parent): QWidget{parent}{
    QHBoxLayout* layoutPrincipale = new QHBoxLayout();
    layoutPrincipale->setContentsMargins(8,4,8,4);
    nome = new QLabel(name,this);
    cognome = new QLabel(surname,this);
    oreContratto = new QLabel(QString::number(ore),this);
    ruolo = new QLabel(role,this);
    attivo = new QCheckBox(this);
    attivo->setChecked(active);
    categoriaProtetta = new QCheckBox(this);
    categoriaProtetta->setChecked(listaSpeciale);
    categoriaProtetta->installEventFilter(this);
    layoutPrincipale->addWidget(attivo, 1);
    layoutPrincipale->addWidget(nome, 1);
    layoutPrincipale->addWidget(cognome, 1);
    layoutPrincipale->addWidget(oreContratto, 1);
    layoutPrincipale->addWidget(ruolo, 1);
    layoutPrincipale->addWidget(categoriaProtetta,1);
    setLayout(layoutPrincipale);
    connect(attivo, &QCheckBox::toggled, this, [this](bool state){
        emit onAttivoChanged(this, state);
    });
}

bool employeeRowWidget::eventFilter(QObject *watched, QEvent *event){
    if(watched == categoriaProtetta){
        if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseButtonDblClick ||
            event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
            return true;
    }
    return QWidget::eventFilter(watched, event);
}

}