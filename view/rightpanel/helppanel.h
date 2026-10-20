#ifndef HELPPANEL_H
#define HELPPANEL_H
#include <QPushButton>
#include <QWidget>
#include <QTextBrowser>
namespace GUI{
class helpPanel : public QWidget
{
    Q_OBJECT
private:
    QPushButton* backButton;
    void loadText();
    QTextBrowser* testo;
protected:
    void keyPressEvent(QKeyEvent* event) override;
public:
    explicit helpPanel(QWidget *parent = nullptr);

signals:
    void onBackPressed();
};
}
#endif // HELPPANEL_H
