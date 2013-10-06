#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <customtypes.h>
#include <improvedwidgets/improvedmessagewidget.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createDeviceListPage();
    void loadDeviceList();
    void loadProgramSettings();

private:    
    void showMessage(const QString &msg,
                     ImprovedMessageWidget::MessageType messageType = ImprovedMessageWidget::Information,
                     const QString &detailedText = "");

    void getServiceDataFromDevice();

    void showAboutProgramPage();
    void showSettingsPage();

    void tabCurrentChanged(int index);
    void tabCloseRequested(int index);

    void closeEvent(QCloseEvent *event);

    Ui::MainWindow              *ui;
    QVector<PageType::Enum>     *mTypePageList;
    QHash<QString, QWidget *>   *mPageList;
};

#endif // MAINWINDOW_H
