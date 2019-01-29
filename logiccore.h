#ifndef LOGICCORE_H
#define LOGICCORE_H

#include <vector>
#include <QString>
#include <QStringList>

class LogicCore
{
private:
    static const QString defaultPath;
    const uint coreNumber;

    QStringList availableGovernors;
    bool isOnline;
    uint currentCoreFrequence;
    uint maxCoreFrequence;
    uint minCoreFrequence;
    uint maxScalingFrequence;
    uint minScalingFrequence;
    QString currentGovernor;

    QString readCurrentGovernor();
    QStringList readAvalibleGovernors() const;
    uint readScalingMinFrequence() const;
    uint readScalingMaxFrequence() const;
    uint readCurrentCoreFrequence() const;
    uint readMaxCoreFrequence() const;
    uint readMinCoreFrequence() const;
    bool readIsOnline() const;

public:
    LogicCore(const uint &coreNumber);

    bool getOnline() const;
    void setOnline(bool value);

    void setScalingMaxFrequence(const uint &value);
    uint getScalingMaxFrequence() const;

    uint getCurrentCoreFrequence() const;
    uint getMaxCoreFrequence() const;
    uint getMinCoreFrequence() const;
    QStringList getAvailableGovernors() const;

    void setScalingMinFrequence(const uint &value);
    uint getScalingMinFrequence() const;

    void setGovernor(const QString &governorName);
    QString getGovernor() const;

    void setCurrentGovernor(const QString &governorName);

    uint getNumber() const;

    void update();
};

#endif // LOGICCORE_H
