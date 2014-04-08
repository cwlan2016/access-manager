#include "stdafx.h"

#include "config.h"
#include "constant.h"
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle("fusion");

    //Bug in Qt. Not exist properties for disabled Actions
    QPalette palette = QApplication::style()->standardPalette();
    QColor disabledText = QColor(190, 190, 190);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledText);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledText);
    QApplication::setPalette(palette);

    a.setApplicationName("Access Manager");
    a.setApplicationVersion(QString::fromUtf8("0.2.2"));
    a.setOrganizationName(QString::fromUtf8("rt.ru"));
    Config::init();

    QSplashScreen splash(QPixmap(":/images/splash.png"));
    splash.show();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::LoadLocale,
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white);
    QTranslator translator;

    if (translator.load(":tr/qtbase_ru.qm")) {
        a.installTranslator(&translator);
    }

    a.processEvents();

    splash.showMessage(LoadProgramStrings::CreateWindow,
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white);
    MainWindow *w = new MainWindow();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::CreateDeviceListPage,
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white);
    w->createDeviceListPage();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::LoadList,
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white);
    w->loadDeviceList();
    a.processEvents();

    splash.showMessage(LoadProgramStrings::LoadConfig,
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white);
    w->loadProgramSettings();
    a.processEvents();

    w->show();
    splash.finish(w);

    qRegisterMetaType<QVector<int> >("QVector<int>");

    return a.exec();
}
