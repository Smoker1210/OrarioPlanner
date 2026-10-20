#ifndef NEWEMPLOYEEWIDGET_H
#define NEWEMPLOYEEWIDGET_H
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include "../model/employee/employee.h"
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
namespace GUI{
class newEmployeeWidget : public QWidget
{
    Q_OBJECT

private slots:
    //funzione che controlla se le condizioni minime per la save sono state raggiunte (nome e cognome non vuoti, ruolo non vuoto)
    void tryEnableSave();
protected:
    shiftplanner::employee* nuovoDipendente;
    QLabel* Titolo;
    QLineEdit* NomeField;
    QLineEdit* CognomeField;
    QSpinBox* OreField;
    QComboBox* RuoloField;
    QCheckBox* CategoriaProtField;
    QPushButton* saveButton;
    QPushButton* cancelButton;
protected slots:
    void assembleDipendente();
    //manda il nuovo dipendente assemblato dai field dati del widget
    virtual void packageDipendente();
public:
    explicit newEmployeeWidget(QWidget *parent = nullptr);
public slots:
    void clearView();

signals:
    void sendDipendente(shiftplanner::employee* nuovoDip);
    void sendCancel();
};
}
#endif // NEWEMPLOYEEWIDGET_H
