#include "DataProvider.h"
#include <QRandomGenerator>
#include <QTimer>

DataProvider::DataProvider(QObject *parent)
    : QObject{parent}
    , m_peakPower(0.0)
    , m_autoGenerationTimer(new QTimer(this))
{
    qDebug() << Q_FUNC_INFO;
    connect(m_autoGenerationTimer, &QTimer::timeout, this, &DataProvider::generateRandomData);
}

DataProvider::DataProvider()
    : m_peakPower(0.0)
    , m_autoGenerationTimer(new QTimer(this))
{
    qDebug() << Q_FUNC_INFO;
    connect(m_autoGenerationTimer, &QTimer::timeout, this, &DataProvider::generateRandomData);
}

DataProvider::~DataProvider()
{
    qDebug() << Q_FUNC_INFO;
    m_autoGenerationTimer->stop();
    delete m_autoGenerationTimer;
}

QVector<DataPoint> DataProvider::getDataPoints() const
{
    qDebug() << Q_FUNC_INFO;
    return m_dataPoints;
}

void DataProvider::setDataPoints(const QVector<DataPoint> &newDataPoints)
{
    qDebug() << Q_FUNC_INFO;
    if (m_dataPoints == newDataPoints)
        return;
    m_dataPoints = newDataPoints;
    emit dataPointsChanged();
}

double DataProvider::getPeakPower() const
{
    qDebug() << Q_FUNC_INFO;
    return m_peakPower;
}

void DataProvider::setPeakPower(double newPeakPower)
{
    qDebug() << Q_FUNC_INFO;
    if (qFuzzyCompare(m_peakPower, newPeakPower))
        return;
    m_peakPower = newPeakPower;
    emit peakPowerChanged();
}

void DataProvider::addPoint(const DataPoint &point)
{
    qDebug() << Q_FUNC_INFO;
    m_dataPoints.append(point);

    if (point.getPower() > m_peakPower)
    {
        m_peakPower = point.getPower();
        emit peakPowerChanged();
    }
    emit dataPointsChanged();
}

void DataProvider::clearData()
{
    qDebug() << Q_FUNC_INFO;
    m_dataPoints.clear();
    m_peakPower = 0.0;
    emit dataPointsChanged();
    emit peakPowerChanged();
}

void DataProvider::startRandomGeneration()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_autoGenerationTimer->isActive()) {
        m_autoGenerationTimer->start(5000);
    }
}

void DataProvider::stopRandomGeneration()
{
    qDebug() << Q_FUNC_INFO;
    if (m_autoGenerationTimer->isActive()) {
        m_autoGenerationTimer->stop();
    }
}

void DataProvider::generateRandomData()
{
    qDebug() << Q_FUNC_INFO;
    clearData();
    int numPoints = QRandomGenerator::global()->bounded(5, 15);
    QVector<DataPoint> newData;

    for (int i = 0; i < numPoints; ++i) {
        double soc = QRandomGenerator::global()->bounded(25, 85);
        newData.append(DataPoint(soc, 0));
    }

    std::sort(newData.begin(), newData.end(), [](const DataPoint &a, const DataPoint &b) {
        return a.getSocPercentage() < b.getSocPercentage();
    });

    double minPower = 100;
    double maxPower = 300;
    double step = (maxPower - minPower) / (numPoints - 1);

    for (int i = 0; i < numPoints; ++i) {
        double power = minPower + i * step;
        power += QRandomGenerator::global()->bounded(10.0) - 5.0;
        newData[i] = DataPoint(newData[i].getSocPercentage(), power);
    }

    setDataPoints(newData);
}

