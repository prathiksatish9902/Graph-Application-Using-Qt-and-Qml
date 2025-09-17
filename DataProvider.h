#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include "DataPoint.h"

class DataProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<DataPoint> dataPoints READ getDataPoints WRITE setDataPoints NOTIFY dataPointsChanged FINAL)
    Q_PROPERTY(double peakPower READ getPeakPower WRITE setPeakPower NOTIFY peakPowerChanged FINAL)

public:
    explicit DataProvider(QObject *parent = nullptr);
    DataProvider();
    ~DataProvider();

    QVector<DataPoint> getDataPoints() const;
    void setDataPoints(const QVector<DataPoint> &newDataPoints);

    double getPeakPower() const;
    void setPeakPower(double newPeakPower);

    void addPoint(const DataPoint &point);
    void clearData();

    Q_INVOKABLE void startRandomGeneration();
    Q_INVOKABLE void stopRandomGeneration();
    Q_INVOKABLE void generateRandomData();
    Q_INVOKABLE bool isAutoGenerating() const { return m_autoGenerationTimer->isActive(); }

signals:
    void dataPointsChanged();
    void peakPowerChanged();

private:
    QVector<DataPoint> m_dataPoints;
    double m_peakPower;
    QTimer *m_autoGenerationTimer;
};

#endif // DATAPROVIDER_H
