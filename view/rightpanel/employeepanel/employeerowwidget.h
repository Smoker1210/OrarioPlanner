#ifndef EMPLOYEEROWWIDGET_H
#define EMPLOYEEROWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
namespace GUI{
class employeeRowWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* nome;
    QLabel* cognome;
    QLabel* oreContratto;
    QLabel* ruolo;
    QCheckBox* attivo;
    QCheckBox* categoriaProtetta;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public:
    explicit employeeRowWidget(const QString& name, const QString& surname, int ore, const QString& role, bool active, bool listaSpeciale, QWidget *parent = nullptr);

signals:
    void onAttivoChanged(const GUI::employeeRowWidget* self, bool state);
};
}
#endif // EMPLOYEEROWWIDGET_H
