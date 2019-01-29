#include "logiccore.h"

#include <QTextStream>

#include <QFile>

const QString LogicCore::defaultPath = "/sys/devices/system/cpu/cpu";

LogicCore::LogicCore(const uint &coreNumber):
    coreNumber(coreNumber)
{
    this->maxCoreFrequence = readMaxCoreFrequence();
    this->minCoreFrequence = readMinCoreFrequence();
    this->availableGovernors = readAvalibleGovernors();

    update();
}

bool LogicCore::getOnline() const
{
    return this->isOnline;
}

uint LogicCore::readCurrentCoreFrequence() const
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_cur_freq");
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Scaling current frequence file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    uint frequence = uint(fileStream.readAll().toInt(&success));
    if(success != true)
    {
        QString errorMessage = "Cannot read scaling current core frequence on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return frequence;
}

uint LogicCore::readMaxCoreFrequence() const
{
    QString path = LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/cpuinfo_max_freq";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Max frequence file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    uint frequence = uint(fileStream.readAll().toInt(&success));
    if(success != true)
    {
        QString errorMessage = "Cannot read min core frequence on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return frequence;
}

uint LogicCore::readMinCoreFrequence() const
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/cpuinfo_min_freq");
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Min core frequence file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    uint frequence = uint(fileStream.readAll().toInt(&success));
    if(success != true)
    {
        QString errorMessage = "Cannot read min core frequence on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return frequence;
}

uint LogicCore::readScalingMaxFrequence() const
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_max_freq");
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Scaling max frequence file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    uint frequence = uint(fileStream.readAll().toInt(&success));
    if(success != true)
    {
        QString errorMessage = "Cannot read Scaling max frequence on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return frequence;
}

void LogicCore::setScalingMaxFrequence(const uint &value)
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_max_freq");
    if(value > this->maxCoreFrequence)
        throw std::logic_error("Cannot set scaling max frequence greater than max hardware frequence.");
    if(value < this->minCoreFrequence)
        throw std::logic_error("Cannot set scaling max frequence lesser than min hardware frequence.");
    if(value <= this->readScalingMinFrequence())
        throw std::logic_error("Cannot set scaling max frequence lesser than min scaling frequence.");
    if(!file.open(QIODevice::WriteOnly))
        throw std::logic_error("Scaling max frequence file is not existing or permission error.");
    QTextStream fileStream(&file);
    fileStream << value;
    file.close();
}

void LogicCore::setScalingMinFrequence(const uint &value)
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_min_freq");
    if(value > this->maxCoreFrequence)
        throw std::logic_error("Cannot set scaling min frequence greater than max hardware frequence.");
    if(value < this->minCoreFrequence)
        throw std::logic_error("Cannot set scaling min frequence lesser than min hardware frequence.");
    if(value >= this->readScalingMaxFrequence())
        throw std::logic_error("Cannot set scaling max frequence greater than max scaling frequence.");
    if(!file.open(QIODevice::WriteOnly))
        throw std::logic_error("Scaling min frequence file is not existing or permission error.");
    QTextStream fileStream(&file);
    fileStream << value;
    file.close();
}

uint LogicCore::getScalingMaxFrequence() const
{
    return this->maxScalingFrequence;
}

uint LogicCore::getCurrentCoreFrequence() const
{
    return this->currentCoreFrequence;
}

uint LogicCore::getMaxCoreFrequence() const
{
    return this->maxCoreFrequence;
}

uint LogicCore::getMinCoreFrequence() const
{
    return this->minCoreFrequence;
}

QStringList LogicCore::getAvailableGovernors() const
{
    return this->availableGovernors;
}

uint LogicCore::readScalingMinFrequence() const
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_min_freq");
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Scaling min frequence file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    uint frequence = uint(fileStream.readAll().toInt(&success));
    if(success != true)
    {
        QString errorMessage = "Cannot read Scaling min frequence on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return frequence;
}

uint LogicCore::getScalingMinFrequence() const
{
    return this->minScalingFrequence;
}

QStringList LogicCore::readAvalibleGovernors() const
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_available_governors");
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Governor file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    QStringList governors = fileStream.readAll().split(" ");
    for(auto& governor: governors)
    {
        governor = governor.trimmed();
    }
    if(success != true)
    {
        QString errorMessage = "Cannot read governor list on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return governors;
}

void LogicCore::setGovernor(const QString &governorName)
{
    QStringList avalibleGovernors = this->readAvalibleGovernors();
    if(!avalibleGovernors.contains(governorName))
        return;
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_available_governors");
    if(!file.open(QIODevice::WriteOnly))
        throw std::logic_error("Governor file is not existing or permission error.");
    QTextStream fileStream(&file);
    fileStream << governorName;
    file.close();
}

QString LogicCore::getGovernor() const
{
    return this->currentGovernor;
}

uint LogicCore::getNumber() const
{
    return this->coreNumber;
}

QString LogicCore::readCurrentGovernor()
{
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/cpufreq/scaling_governor");
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Governor file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    QString governor = fileStream.readAll();
    if(success != true)
    {
        QString errorMessage = "Cannot read governor on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return governor.trimmed();
}

void LogicCore::update()
{
    this->isOnline = readIsOnline();
    this->currentCoreFrequence = readCurrentCoreFrequence();

    this->maxScalingFrequence = readScalingMaxFrequence();
    this->minScalingFrequence = readScalingMinFrequence();
    this->currentGovernor = readCurrentGovernor();
}

bool LogicCore::readIsOnline() const
{
    // Core 0 has no online parameter,
    //so core is always online.
    if(this->coreNumber == 0)
        return true;
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/online");
    if(!file.open(QIODevice::ReadOnly))
        throw std::logic_error("Online file is not existing or permission error.");
    QTextStream fileStream(&file);
    bool success = true;
    bool isOnline = fileStream.readAll().toInt(&success);
    if(success != true)
    {
        QString errorMessage = "Cannot read online parameter on core " + QString::number(this->coreNumber);
        throw std::logic_error(errorMessage.toUtf8().constData());
    }
    file.close();
    return isOnline;
}

void LogicCore::setOnline(bool value)
{
    // Core 0 has no online parameter
    if(this->coreNumber == 0)
        return;
    QFile file(LogicCore::defaultPath + QString::number(this->coreNumber) + "/online");
    if(!file.open(QIODevice::WriteOnly))
        throw std::logic_error("Online file is not existing or permission error.");
    QTextStream fileStream(&file);
    fileStream << value;
    file.close();
}
