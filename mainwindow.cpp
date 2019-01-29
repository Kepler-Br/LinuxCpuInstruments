#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h> // sysconf, getuid for checking for root
#include <QLabel>
#include <QProgressBar>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    coresTotal(uint(sysconf( _SC_NPROCESSORS_CONF )))
{
    ui->setupUi(this);
    initLogicCores();
    initCoreUsageTab();
    initDetailedTab();
    initParametersTab();
    updateParametersTab();
    interfaceUpdateTimer = new QTimer(this);
    connect(interfaceUpdateTimer, SIGNAL(timeout()), this, SLOT(updateInterface()));
    interfaceUpdateTimer->start(1000);
    if(getuid() != 0)
    {
        QMessageBox::warning(this, "Not root.", "You should be root to change logic core status.");
        ui->button_ApplyAll->setEnabled(false);
        ui->button_applyCurrent->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    for(auto &logicCore:this->logicCores)
    {
        delete logicCore;
    }
    for(auto &widgetPair:this->usageTabWidgets)
    {
        delete widgetPair.first;
        delete widgetPair.second;
    }
    delete interfaceUpdateTimer;
    delete ui;
}

void MainWindow::initLogicCores()
{
    for(uint i = 0; i < this->coresTotal; i++)
    {
        this->logicCores.push_back(new LogicCore(i));
    }
}

void MainWindow::initCoreUsageTab()
{
    QFormLayout *formLayout = static_cast<QFormLayout*>(ui->scrollAreaWidgetContents->layout());
    for(uint i = 0; i < this->coresTotal; i++)
    {
        QLabel *coreLabel = new QLabel(QString("Core " + QString::number(i)));
        QProgressBar *coreLoad = new QProgressBar();
        formLayout->addRow(coreLabel, coreLoad);
        QPair<QWidget*, QWidget*> widgetPair(coreLabel, coreLoad);
        this->usageTabWidgets.push_back(widgetPair);
    }
}

void MainWindow::initDetailedTab()
{
    for(int i = 0; i < this->logicCores.size(); i++)
        ui->listWidget_detailedTab->addItem("Logic core " + QString::number(i));
    ui->listWidget_detailedTab->setCurrentRow(0);
}

void MainWindow::initParametersTab()
{
    for(int i = 0; i < this->logicCores.size(); i++)
        ui->listWidget_parameterTab->addItem("Logic core " + QString::number(i));
    ui->listWidget_parameterTab->setCurrentRow(0);
}



void MainWindow::updateUsageTab()
{
    for(auto& logicCore: this->logicCores)
    {
        int maxFreq = int(logicCore->getMaxCoreFrequence());
        int minFreq = int(logicCore->getMinCoreFrequence());
        int curFreq = int(logicCore->getCurrentCoreFrequence());
        int coreNumber = int(logicCore->getNumber());
        QProgressBar* progressBar = static_cast<QProgressBar*>(usageTabWidgets[coreNumber].second);
        progressBar->setMaximum(maxFreq);
        progressBar->setMinimum(minFreq);
        progressBar->setValue(curFreq);
    }
}

void MainWindow::updateDetailedTab()
{
    int currentRow = ui->listWidget_detailedTab->currentRow();
    LogicCore *logicCore = logicCores[currentRow];
    int maxFreq = int(logicCore->getMaxCoreFrequence());
    int minFreq = int(logicCore->getMinCoreFrequence());
    int maxScalFreq = int(logicCore->getScalingMaxFrequence());
    int minScalFreq = int(logicCore->getScalingMinFrequence());
    int curFreq = int(logicCore->getCurrentCoreFrequence());
    QString currentGovernor = logicCore->getGovernor();
    bool coreOnline = logicCore->getOnline();
    int coreNumber = int(logicCore->getNumber());
    ui->coreNumValueLabel->setText(QString::number(coreNumber));
    ui->onlineValueLabel->setText(coreOnline?"True":"False");
    ui->governorValueLabel->setText(currentGovernor);

    const int HZ_TO_MHZ = 1000;
    ui->maxFreqValueLabel->setText(QString::number(maxFreq/HZ_TO_MHZ) + " MHz");
    ui->minFreqValueLabel->setText(QString::number(minFreq/HZ_TO_MHZ) + " MHz");
    ui->curFreqValueLabel->setText(QString::number(curFreq/HZ_TO_MHZ) + " MHz");
    ui->maxScalFreqValueLabel->setText(QString::number(maxScalFreq/HZ_TO_MHZ) + " MHz");
    ui->minScalFreqValueLabel->setText(QString::number(minScalFreq/HZ_TO_MHZ) + " MHz");

}

void MainWindow::updateParametersTab()
{
    int currentRow = ui->listWidget_parameterTab->currentRow();
    LogicCore *logicCore = logicCores[currentRow];
    QStringList list = logicCore->getAvailableGovernors();
    ui->comboBox_governors->clear();
    ui->comboBox_governors->addItems(list);
    QString currentGovernor = logicCore->getGovernor();
    ui->comboBox_governors->setCurrentText(currentGovernor);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::updateInterface()
{
    for(auto& logicCore: this->logicCores)
        logicCore->update();
    updateUsageTab();
    updateDetailedTab();
}

void MainWindow::on_listWidget_detailedTab_itemClicked(QListWidgetItem *item)
{
    updateDetailedTab();
}

void MainWindow::on_sliderMaxFreq_valueChanged(int value)
{
    int minValue = ui->sliderMinFreq->value();
    if(value < minValue)
    {
        ui->sliderMaxFreq->setValue(minValue);
        ui->maxFreqSliderValue->setText(QString::number(minValue) + "%");
        return;
    }
    ui->maxFreqSliderValue->setText(QString::number(value) + "%");
}

void MainWindow::on_sliderMinFreq_valueChanged(int value)
{
    int maxValue = ui->sliderMaxFreq->value();
    if(value > maxValue)
    {
        ui->sliderMinFreq->setValue(maxValue);
        ui->minFreqSliderValue->setText(QString::number(maxValue) + "%");
        return;
    }
    ui->minFreqSliderValue->setText(QString::number(value) + "%");
}

void MainWindow::on_listWidget_parameterTab_itemClicked(QListWidgetItem *item)
{
    updateParametersTab();
}

void MainWindow::on_button_applyCurrent_clicked()
{
    int selectedCore = ui->listWidget_parameterTab->currentRow();
    LogicCore *core = this->logicCores[selectedCore];
    int maxSliderValue = ui->sliderMaxFreq->maximum();
    int maxValue = ui->sliderMaxFreq->value()/maxSliderValue;
    int minValue = ui->sliderMinFreq->value()/maxSliderValue;
    bool isOnline = ui->checkBox_coreOnline->isChecked();

    core->setOnline(isOnline);
    int maxHardFreq = int(core->getMaxCoreFrequence());
    int minHardFreq = int(core->getMinCoreFrequence());
    uint maxScalingFreq = uint((maxHardFreq-minHardFreq)*maxValue+minHardFreq);
    uint minScalingFreq = uint((minHardFreq-minHardFreq)*minValue+minHardFreq);
    core->setScalingMaxFrequence(maxScalingFreq);
    core->setScalingMinFrequence(minScalingFreq);
}

void MainWindow::on_button_ApplyAll_clicked()
{
    int maxSliderValue = ui->sliderMaxFreq->maximum();
    int maxValue = ui->sliderMaxFreq->value()/maxSliderValue;
    int minValue = ui->sliderMinFreq->value()/maxSliderValue;
    bool isOnline = ui->checkBox_coreOnline->isChecked();

    for(auto* core: logicCores)
    {
        int maxHardFreq = int(core->getMaxCoreFrequence());
        int minHardFreq = int(core->getMinCoreFrequence());
        uint maxScalingFreq = uint(maxHardFreq*maxValue);
        uint minScalingFreq = uint(minHardFreq*minValue);
        core->setOnline(isOnline);
        core->setScalingMaxFrequence(maxScalingFreq);
        core->setScalingMinFrequence(minScalingFreq);
    }
}
