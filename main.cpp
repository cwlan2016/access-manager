#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include "config.h"
#include "constant.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("AccessManager");
    a.setApplicationVersion(QString::fromUtf8("0.2 unstable"));
    a.setOrganizationName(QObject::trUtf8("Rostelecom"));
    Config::init();

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);

    splash.show();
    a.processEvents();

    splash.showMessage(LoadProgramString::LoadLocale);
    QTranslator *qt_translator = new QTranslator;

    if (qt_translator->load(":tr/qtbase_ru.qm")) {
        a.installTranslator(qt_translator);
    }

    a.processEvents();

    splash.showMessage(LoadProgramString::CreateWindow);
    MainWindow *w = new MainWindow();
    a.processEvents();

    splash.showMessage(LoadProgramString::CreateDeviceListPage);
    w->createDeviceListPage();
    a.processEvents();

    splash.showMessage(LoadProgramString::LoadList);
    w->loadDeviceList();
    a.processEvents();

    splash.showMessage(LoadProgramString::LoadConfig);
    w->loadProgramSettings();
    a.processEvents();

    w->show();
    splash.finish(w);

    return a.exec();
}
