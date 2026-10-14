#pragma once

#include <QWidget>
#include <QSize>
#include <QVector>
#include <QStringList>

// Donut chart: transport / electricity / food split.
class DonutChart : public QWidget
{
    Q_OBJECT
public:
    explicit DonutChart(QWidget *parent = nullptr);
    void setData(double transport, double electricity, double food);
    QSize sizeHint() const override { return QSize(360, 200); }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    double m_transport = 0, m_electricity = 0, m_food = 0;
};

// Bar chart: total emissions over time (oldest on the left).
class TrendChart : public QWidget
{
    Q_OBJECT
public:
    explicit TrendChart(QWidget *parent = nullptr);
    void setData(const QVector<double> &values, const QStringList &labels);
    QSize sizeHint() const override { return QSize(480, 220); }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QVector<double> m_values;
    QStringList m_labels;
};