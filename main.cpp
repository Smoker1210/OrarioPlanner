#include "view/mainwindow/MainWindow.h"
#include <qapplication.h>
#include <QMainWindow>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QTranslator translator;
    QLocale locale = QLocale::system();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    const QString trPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#else
    const QString trPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    app.setWindowIcon(QIcon(":/resources/icon/calendar.png"));
#endif
    if (translator.load(locale, "qtbase", "_", trPath)) {
        app.installTranslator(&translator);
    }
    app.setWindowIcon(QIcon(":/resources/icon/calendar.png"));
    QCoreApplication::setApplicationName("OrarioPlanner");
    GUI::MainWindow mainwindow;
    mainwindow.showMaximized();
    return app.exec();
}