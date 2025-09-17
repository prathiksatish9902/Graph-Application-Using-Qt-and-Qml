#include "GraphItem.h"
#include <QPainterPath>
#include <QLinearGradient>
#include <QPolygonF>
#include <QtMath>

GraphItem::GraphItem() : m_graphPointsProvider(nullptr)
{
    initializeDefaults();
}

void GraphItem::paint(QPainter *painter)
{
    if (!m_graphPointsProvider) {
        qDebug() << "No data provider set";
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing, true);

    drawBackground(painter);
    drawTitle(painter);
    drawAxes(painter);
    drawAxisLabels(painter);
    drawAxisValues(painter);
    drawGraph(painter);
    drawEndPoints(painter);
    drawArrows(painter);
}

void GraphItem::onDataChanged()
{
    update();
}

DataProvider *GraphItem::getGraphPointsProvider() const
{
    return m_graphPointsProvider;
}

void GraphItem::setGraphPointsProvider(DataProvider *newGraphPointsProvider)
{
    qDebug() << Q_FUNC_INFO;
    if (m_graphPointsProvider == newGraphPointsProvider)
        return;

    if (m_graphPointsProvider) {
        disconnect(m_graphPointsProvider, nullptr, this, nullptr);
    }

    m_graphPointsProvider = newGraphPointsProvider;

    if (m_graphPointsProvider) {
        connect(m_graphPointsProvider, &DataProvider::dataPointsChanged,
                this, &GraphItem::onDataChanged);
        connect(m_graphPointsProvider, &DataProvider::peakPowerChanged,
                this, &GraphItem::onDataChanged);
    }

    emit graphPointsProviderChanged();
    update();
}

QColor GraphItem::getBackgroundColor() const
{
    return m_backgroundColor;
}

void GraphItem::setBackgroundColor(const QColor &newBackgroundColor)
{
    qDebug() << Q_FUNC_INFO;
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_backgroundColor = newBackgroundColor;
    emit backgroundColorChanged();
    update();
}

QColor GraphItem::getTextColor() const
{
    return m_textColor;
}

void GraphItem::setTextColor(const QColor &newTextColor)
{
    qDebug() << Q_FUNC_INFO;
    if (m_textColor == newTextColor)
        return;
    m_textColor = newTextColor;
    emit textColorChanged();
    update();
}

QColor GraphItem::getLineColor() const
{
    return m_lineColor;
}

void GraphItem::setLineColor(const QColor &newLineColor)
{
    qDebug() << Q_FUNC_INFO;
    if (m_lineColor == newLineColor)
        return;
    m_lineColor = newLineColor;
    emit lineColorChanged();
    update();
}

QString GraphItem::getTitle() const
{
    return m_title;
}

void GraphItem::setTitle(const QString &newTitle)
{
    qDebug() << Q_FUNC_INFO;
    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged();
    update();
}

QString GraphItem::getXAxisLabel() const
{
    return m_xAxisLabel;
}

void GraphItem::setXAxisLabel(const QString &newXAxisLabel)
{
    qDebug() << Q_FUNC_INFO;
    if (m_xAxisLabel == newXAxisLabel)
        return;
    m_xAxisLabel = newXAxisLabel;
    emit xAxisLabelChanged();
    update();
}

QString GraphItem::getYAxisLabel() const
{
    return m_yAxisLabel;
}

void GraphItem::setYAxisLabel(const QString &newYAxisLabel)
{
    qDebug() << Q_FUNC_INFO;
    if (m_yAxisLabel == newYAxisLabel)
        return;
    m_yAxisLabel = newYAxisLabel;
    emit yAxisLabelChanged();
    update();
}

void GraphItem::initializeDefaults()
{
    m_backgroundColor = QColor("#1e1e1e");
    m_textColor = QColor("#ffffff");
    m_lineColor = QColor("#00AEEF");

    m_title = "Peak Power Delivered";
    m_xAxisLabel = "SOC (%)";
    m_yAxisLabel = "Power (kW)";

    m_titleFont = QFont("Arial", 24, QFont::Bold);
    m_axisFont = QFont("Arial", 12);
    m_labelFont = QFont("Arial", 14);
}

QRectF GraphItem::getPlotArea() const
{
    return QRectF(LEFT_MARGIN, TOP_MARGIN,
                  width() - LEFT_MARGIN - RIGHT_MARGIN,
                  height() - TOP_MARGIN - BOTTOM_MARGIN);
}

QPointF GraphItem::mapDataToPixel(double soc, double power) const
{
    QRectF plotArea = getPlotArea();

    if (!m_graphPointsProvider) {
        return QPointF(0, 0);
    }

    double minSoc = 0.0;
    double maxSoc = 100.0;
    double minPower = 0.0;
    double maxPower = qMax(300.0, m_graphPointsProvider->getPeakPower() * 1.2);

    double x = plotArea.left() + ((soc - minSoc) / (maxSoc - minSoc)) * plotArea.width();
    double y = plotArea.bottom() - ((power - minPower) / (maxPower - minPower)) * plotArea.height();

    return QPointF(x, y);
}

void GraphItem::drawBackground(QPainter *painter)
{
    painter->fillRect(0, 0, width(), height(), m_backgroundColor);
}

void GraphItem::drawTitle(QPainter *painter)
{
    painter->setPen(m_textColor);
    painter->setFont(m_titleFont);

    QFontMetrics fm(m_titleFont);
    QRect titleRect = fm.boundingRect(m_title);
    QPointF titlePos((width() - titleRect.width()) / 2, TOP_MARGIN / 2);
    painter->drawText(titlePos, m_title);
}

void GraphItem::drawAxes(QPainter *painter)
{
    QRectF plotArea = getPlotArea();
    painter->setPen(QPen(m_textColor, 1));

    painter->drawLine(plotArea.bottomLeft(), plotArea.bottomRight());
    painter->drawLine(plotArea.topLeft(), plotArea.bottomLeft());
}

void GraphItem::drawAxisLabels(QPainter *painter)
{
    QRectF plotArea = getPlotArea();
    painter->setPen(m_textColor);
    painter->setFont(m_labelFont);

    painter->save();
    painter->translate(LEFT_MARGIN / 2, plotArea.center().y());
    painter->rotate(-90);
    painter->drawText(0, 0, m_yAxisLabel);
    painter->restore();

    QFontMetrics fm(m_labelFont);
    QRect xLabelRect = fm.boundingRect(m_xAxisLabel);
    QPointF xLabelPos(plotArea.center().x() - xLabelRect.width() / 2, height() - BOTTOM_MARGIN / 2);
    painter->drawText(xLabelPos, m_xAxisLabel);
}

void GraphItem::drawAxisValues(QPainter *painter)
{
    painter->setFont(m_axisFont);
    painter->setPen(m_textColor);
    QRectF plotArea = getPlotArea();

    if (!m_graphPointsProvider) return;

    QVector<DataPoint> dataPoints = m_graphPointsProvider->getDataPoints();
    if (dataPoints.isEmpty()) return;

    DataPoint firstPoint = dataPoints.first();
    DataPoint lastPoint = dataPoints.last();

    QPointF firstPixel = mapDataToPixel(firstPoint.getSocPercentage(), 0);
    QPointF lastPixel = mapDataToPixel(lastPoint.getSocPercentage(), 0);

    QString firstText = QString::number(qRound(firstPoint.getSocPercentage())) + "%";
    QString lastText = QString::number(qRound(lastPoint.getSocPercentage())) + "%";

    QFontMetrics fm(m_axisFont);
    QRect firstTextRect = fm.boundingRect(firstText);
    QRect lastTextRect = fm.boundingRect(lastText);

    painter->drawText(QPointF(firstPixel.x() - firstTextRect.width() / 2, plotArea.bottom() + 20), firstText);
    painter->drawText(QPointF(lastPixel.x() - lastTextRect.width() / 2, plotArea.bottom() + 20), lastText);

    double maxPower = m_graphPointsProvider->getPeakPower();
    QPointF maxPowerPixel = mapDataToPixel(0, maxPower);
    QString maxPowerText = QString::number(qRound(maxPower)) + "kW";
    QRect maxPowerTextRect = fm.boundingRect(maxPowerText);
    painter->drawText(QPointF(plotArea.left() - maxPowerTextRect.width() - 10, maxPowerPixel.y() + 5), maxPowerText);
}

void GraphItem::drawGraph(QPainter *painter)
{
    if (!m_graphPointsProvider || m_graphPointsProvider->getDataPoints().isEmpty()) {
        return;
    }

    QVector<DataPoint> dataPoints = m_graphPointsProvider->getDataPoints();
    QVector<QPointF> pixelPoints;

    for (const DataPoint &dp : dataPoints) {
        QPointF pixelPoint = mapDataToPixel(dp.getSocPercentage(), dp.getPower());
        pixelPoints.append(pixelPoint);
    }

    if (pixelPoints.size() < 2) {
        return;
    }

    QPainterPath path;
    path.moveTo(pixelPoints.first());

    for (int i = 1; i < pixelPoints.size(); ++i) {
        path.lineTo(pixelPoints[i]);
    }

    QRectF plotArea = getPlotArea();

    QPainterPath fillPath = path;
    fillPath.lineTo(pixelPoints.last().x(), plotArea.bottom());
    fillPath.lineTo(pixelPoints.first().x(), plotArea.bottom());
    fillPath.closeSubpath();

    QLinearGradient gradient(0, plotArea.top(), 0, plotArea.bottom());
    gradient.setColorAt(0.0, m_lineColor.lighter(35));
    gradient.setColorAt(0.5, m_lineColor.lighter(25));
    gradient.setColorAt(0.9, QColor(m_lineColor.red(),
                                    m_lineColor.green(),
                                    m_lineColor.blue(),
                                    0));
    painter->fillPath(fillPath, gradient);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(m_lineColor, LINE_WIDTH));
    painter->drawPath(path);
}



void GraphItem::drawEndPoints(QPainter *painter)
{
    if (!m_graphPointsProvider || m_graphPointsProvider->getDataPoints().isEmpty()) {
        return;
    }

    QVector<DataPoint> dataPoints = m_graphPointsProvider->getDataPoints();
    painter->setPen(QPen(m_lineColor, 2));
    painter->setBrush(m_lineColor);

    DataPoint firstPoint = dataPoints.first();
    DataPoint lastPoint = dataPoints.last();

    QPointF firstPixel = mapDataToPixel(firstPoint.getSocPercentage(), firstPoint.getPower());
    QPointF lastPixel = mapDataToPixel(lastPoint.getSocPercentage(), lastPoint.getPower());

    painter->drawEllipse(firstPixel, POINT_RADIUS, POINT_RADIUS);
    painter->drawEllipse(lastPixel, POINT_RADIUS, POINT_RADIUS);

    painter->setPen(m_lineColor);
    painter->setFont(m_labelFont);

    QString firstPowerText = formatPowerValue(firstPoint.getPower());
    QString lastPowerText = formatPowerValue(lastPoint.getPower());

    QFontMetrics fm(m_labelFont);
    QRect firstTextRect = fm.boundingRect(firstPowerText);
    QRect lastTextRect = fm.boundingRect(lastPowerText);

    QPointF firstTextPos(firstPixel.x() - firstTextRect.width() / 2, firstPixel.y() - 15);
    QPointF lastTextPos(lastPixel.x() - lastTextRect.width() / 2, lastPixel.y() - 15);

    painter->drawText(firstTextPos, firstPowerText);
    painter->drawText(lastTextPos, lastPowerText);
}

void GraphItem::drawArrows(QPainter *painter)
{
    QRectF plotArea = getPlotArea();
    painter->setPen(QPen(m_textColor, 1));

    QPointF xArrowEnd = plotArea.bottomRight();
    QPointF xArrow1(xArrowEnd.x() - 10, xArrowEnd.y() - 5);
    QPointF xArrow2(xArrowEnd.x() - 10, xArrowEnd.y() + 5);
    painter->drawLine(xArrowEnd, xArrow1);
    painter->drawLine(xArrowEnd, xArrow2);

    QPointF yArrowEnd = plotArea.topLeft();
    QPointF yArrow1(yArrowEnd.x() + 5, yArrowEnd.y() + 10);
    QPointF yArrow2(yArrowEnd.x() - 5, yArrowEnd.y() + 10);
    painter->drawLine(yArrowEnd, yArrow1);
    painter->drawLine(yArrowEnd, yArrow2);
}

QString GraphItem::formatPowerValue(double power) const
{
    return QString("%1kW").arg(power, 0, 'f', 0);
}
