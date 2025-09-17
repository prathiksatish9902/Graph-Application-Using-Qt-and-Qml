#ifndef DATAPOINT_H
#define DATAPOINT_H
#include <QDebug>

class DataPoint
{
public:
    DataPoint();
    DataPoint(double soc, double power);
    ~DataPoint();

    double getSocPercentage() const;
    void setSocPercentage(double newSocPercentage);

    double getPower() const;
    void setPower(double newPower);

    bool operator==(const DataPoint &other) const;
    bool operator!=(const DataPoint &other) const;

private:
    double m_socPercentage;
    double m_power;
};

#endif // DATAPOINT_H
