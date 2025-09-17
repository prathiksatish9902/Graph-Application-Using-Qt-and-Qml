#include "DataPoint.h"

DataPoint::DataPoint()
{
    qDebug() << Q_FUNC_INFO;
}

DataPoint::DataPoint(double soc, double power)
    : m_socPercentage(soc), m_power(power)
{
    qDebug() << Q_FUNC_INFO;
}

DataPoint::~DataPoint()
{
    qDebug() << Q_FUNC_INFO;
}

double DataPoint::getSocPercentage() const
{
    qDebug() << Q_FUNC_INFO;
    return m_socPercentage;
}

void DataPoint::setSocPercentage(double newSocPercentage)
{
    qDebug() << Q_FUNC_INFO;
    m_socPercentage = newSocPercentage;
}

double DataPoint::getPower() const
{
    qDebug() << Q_FUNC_INFO;
    return m_power;
}

void DataPoint::setPower(double newPower)
{
    qDebug() << Q_FUNC_INFO;
    m_power = newPower;
}

bool DataPoint::operator==(const DataPoint &other) const
{
    return qFuzzyCompare(m_socPercentage, other.m_socPercentage) &&
           qFuzzyCompare(m_power, other.m_power);
}

bool DataPoint::operator!=(const DataPoint &other) const
{
    return !(*this == other);
}
