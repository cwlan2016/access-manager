#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include "config.h"
#include "constant.h"
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("fusion");
    a.setApplicationName("Access Manager");
    a.setApplicationVersion(QString::fromUtf8("0.2"));
    a.setOrganizationName(QObject::trUtf8("rt.ru"));
    Config::init();

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);

    splash.show();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::LoadLocale);
    QTranslator translator;

    if (translator.load(":tr/qtbase_ru.qm")) {
        a.installTranslator(&translator);
    }

    a.processEvents();

    splash.showMessage(LoadProgramStrings::CreateWindow);
    MainWindow *w = new MainWindow();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::CreateDeviceListPage);
    w->createDeviceListPage();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::LoadList);
    w->loadDeviceList();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::LoadConfig);
    w->loadProgramSettings();
    a.processEvents();

    w->show();
    splash.finish(w);

    return a.exec();
}
