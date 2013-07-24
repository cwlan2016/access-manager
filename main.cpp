#include <stdafx.h>

#include "config.h"
#include "constant.h"
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle("fusion");

    //Bug in Qt. Not exist properties for disabled Actions
    QPalette palette = QStyleFactory::create("Fusion")->standardPalette();
    QColor disabledText = QColor(190, 190, 190);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledText);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledText);
    QApplication::setPalette(palette);

    a.setApplicationName("Access Manager");
    a.setApplicationVersion(QString::fromUtf8("0.2.2"));
    a.setOrganizationName(QString::fromUtf8("rt.ru"));
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
