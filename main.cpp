#include <stdafx.h>

#include "config.h"
#include "constant.h"
//#include "gui/mainwindow.h"
#include "models/devicetablemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
//    QApplication apps(argc, argv);

    //QGuiApplication::setStyle("fusion");

    //Bug in Qt. Not exist properties for disabled Actions
    QPalette palette = QStyleFactory::create("Fusion")->standardPalette();
    QColor disabledText = QColor(190, 190, 190);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledText);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledText);
    //QGuiApplication::setPalette(palette);

    //app.setPalette(palette);
    //app.setApplicationName("Access Manager");
    //app.setApplicationVersion(QString::fromUtf8("0.2.1"));
    //app.setOrganizationName(QString::fromUtf8("rt.ru"));

    Config::init();

    //QPixmap pixmap(":/images/splash.png");
    //QSplashScreen splash(pixmap);

    //splash.show();
    app.processEvents();

    //splash.showMessage(LoadProgramStrings::LoadLocale);
   // QTranslator translator;

   // if (translator.load(":/tr/qtbase_ru.qm")) {
   //     app.installTranslator(&translator);
  //  }

    app.processEvents();

    DeviceTableModel *model = new DeviceTableModel();
    model->load();

    //splash.showMessage(LoadProgramStrings::CreateWindow);

    //MainWindow *w = new MainWindow();

    //qmlRegisterType<DeviceTableModel *>("DeviceTableModel", 1, 0, "DeviceTableModel");
    QQmlApplicationEngine engine(QUrl("qrc:/qml/main.qml"));
    QObject *topLevel = engine.rootObjects().value(0);
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("deviceListModel", model);

    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    if (!window) {
        qWarning("Error: Your root item has to be a Window.");
        return -1;
    }

    window->show();

    app.processEvents();

    //splash.showMessage(LoadProgramStrings::CreateDeviceListPage);
    //w->createDeviceListPage();
    app.processEvents();

    //splash.showMessage(LoadProgramStrings::LoadList);
    //w->loadDeviceList();
    app.processEvents();

    //splash.showMessage(LoadProgramStrings::LoadConfig);
    //w->loadProgramSettings();
    app.processEvents();

    //w->show();
    //splash.finish(w);

    return app.exec();
}
