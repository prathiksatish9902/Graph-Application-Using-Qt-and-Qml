#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QString>
#include <QFont>
#include <QPainter>
#include "DataProvider.h"

class GraphItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(DataProvider *graphPointsProvider READ getGraphPointsProvider WRITE setGraphPointsProvider NOTIFY graphPointsProviderChanged FINAL)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged FINAL)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor NOTIFY textColorChanged FINAL)
    Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor NOTIFY lineColorChanged FINAL)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString xAxisLabel READ getXAxisLabel WRITE setXAxisLabel NOTIFY xAxisLabelChanged FINAL)
    Q_PROPERTY(QString yAxisLabel READ getYAxisLabel WRITE setYAxisLabel NOTIFY yAxisLabelChanged FINAL)

public:
    GraphItem();
    ~GraphItem() = default;

    void paint(QPainter *painter) override;

    DataProvider *getGraphPointsProvider() const;
    void setGraphPointsProvider(DataProvider *newGraphPointsProvider);

    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    QColor getTextColor() const;
    void setTextColor(const QColor &newTextColor);

    QColor getLineColor() const;
    void setLineColor(const QColor &newLineColor);

    QString getTitle() const;
    void setTitle(const QString &newTitle);

    QString getXAxisLabel() const;
    void setXAxisLabel(const QString &newXAxisLabel);

    QString getYAxisLabel() const;
    void setYAxisLabel(const QString &newYAxisLabel);

signals:
    void graphPointsProviderChanged();
    void backgroundColorChanged();
    void textColorChanged();
    void lineColorChanged();
    void titleChanged();
    void xAxisLabelChanged();
    void yAxisLabelChanged();

private slots:
    void onDataChanged();

private:
    void initializeDefaults();
    QRectF getPlotArea() const;
    QPointF mapDataToPixel(double soc, double power) const;
    void drawBackground(QPainter *painter);
    void drawTitle(QPainter *painter);
    void drawAxes(QPainter *painter);
    void drawAxisLabels(QPainter *painter);
    void drawAxisValues(QPainter *painter);
    void drawGraph(QPainter *painter);
    void drawEndPoints(QPainter *painter);
    void drawArrows(QPainter *painter);
    QString formatPowerValue(double power) const;

private:
    DataProvider *m_graphPointsProvider;
    QColor m_backgroundColor;
    QColor m_textColor;
    QColor m_lineColor;
    QString m_title;
    QString m_xAxisLabel;
    QString m_yAxisLabel;

    QFont m_titleFont;
    QFont m_axisFont;
    QFont m_labelFont;
    static constexpr qreal TOP_MARGIN = 120;
    static constexpr qreal BOTTOM_MARGIN = 80;
    static constexpr qreal LEFT_MARGIN = 80;
    static constexpr qreal RIGHT_MARGIN = 60;
    static constexpr qreal LINE_WIDTH = 3.0;
    static constexpr qreal POINT_RADIUS = 6.0;
};

#endif // GRAPHITEM_H
