#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QVector>
#include <QPair>
#include <QTimer>

#include "logiccore.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void updateInterface();

    void on_listWidget_detailedTab_itemClicked(QListWidgetItem *item);

    void on_sliderMaxFreq_valueChanged(int value);

    void on_sliderMinFreq_valueChanged(int value);

    void on_listWidget_parameterTab_itemClicked(QListWidgetItem *item);

    void on_button_applyCurrent_clicked();

    void on_button_ApplyAll_clicked();

private:
    QTimer *interfaceUpdateTimer;
    Ui::MainWindow *ui;
    const uint coresTotal;
    QVector<LogicCore*> logicCores;
    QVector<QPair<QWidget*, QWidget*>> usageTabWidgets;

    void initLogicCores();
    void initCoreUsageTab();
    void initDetailedTab();

    void updateUsageTab();
    void updateDetailedTab();
    void updateParametersTab();
    void initParametersTab();
};

#endif // MAINWINDOW_H
