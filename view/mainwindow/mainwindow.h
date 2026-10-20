#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "../model/shiftmanager/shiftmanager.h"
#include "../model/observer/observer.h"
#include "../view/leftpanel/leftpanel.h"
#include "../view/rightpanel/rightpanel.h"
#include <QMainWindow>
#include <QObject>
namespace GUI{
class MainWindow : public QMainWindow, public shiftplanner::ObserverInterface
{
    Q_OBJECT
private:
    //senza questo bool abbiamo a tempo di testing un totale di ben 80 emit di observerNotified (dovute dalla load del json), questo ci salva il problema
    bool blockUpdate;
    int contatore=0;
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* exportAction;
    QAction* exitAction;
    QAction* actionHelp;
    QWidget* CentralContainer;
    leftpanel* pannelloSinistro;
    rightpanel* pannelloDestro;
    bool dirtyBit;
    shiftplanner::shiftManager mainManager;
    std::vector<shiftplanner::anomaly> anomalie;
    QString currentFile;
    QMenu* fileMenu;
    //hex per gestione colore di sistema in base al tema di windows
    QString coloreHex;
    //funzione di utility che chiede all'utente di salvare nel caso di azione con possibile perdita di dati
    bool askToSave();
    void createActions();
    void setupMenu();
    void updateWindowTitle();
    void setupCentralWidget();
    void setupRightPanel();
    void setupLeftPanel();
    void closeEvent(QCloseEvent* event) override;
    void checkFirstAvvio();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void update() override;
private slots:
    void updateLeftPanel(shiftplanner::shopConfig& newConfig, int incasso, double maxOre, int settimana);
public slots:
    void New();
    void Open();
    void Save();
    void Export();
    void clearAllViews();
    void acceptNewDipendente(shiftplanner::employee* nuovoDip);
    void deleteDipendente(const shiftplanner::employee* target);
    void deleteFromEdit(const shiftplanner::employee* target);
    void updateActiveStateOfDipendente(const std::string& key, bool state);
    void acceptNewShift(const std::string& key, int giorno, const shiftplanner::shift& turno);
    void updateLeftInfo();
    void checkFailsafeRules();
    void onDipInfoRequest(const std::string key);
    //svuota il vettore anomalie e aggiorna il pannello sinistro
    void emptyAnomalies();

signals:
    void sendAnomalies(const std::vector<shiftplanner::anomaly>& anomalie);
    void onObserverNotified();
    void sendCriticalDay(int giorno);
    void cleanCriticalDays();
    void sendInfoDip(const shiftplanner::employee* dip);
    void requestFillEmployeeDetail();
};
}
#endif // MAINWINDOW_H
