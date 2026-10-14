#include "chartwidgets.h"

#include <QPainter>
#include <QtMath>
#include <algorithm>

namespace {
const QColor kTransport("#2f80c0");
const QColor kElectricity("#e0a21b");
const QColor kFood("#2e9e5b");
const QColor kText("#1f3d2f");
const QColor kMuted("#7a8a82");
}

// ---------------- DonutChart ----------------

DonutChart::DonutChart(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(180);
}

void DonutChart::setData(double transport, double electricity, double food)
{
    m_transport = transport;
    m_electricity = electricity;
    m_food = food;
    update();
}

void DonutChart::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const double total = m_transport + m_electricity + m_food;
    if (total <= 0.0) {
        p.setPen(kMuted);
        p.drawText(rect(), Qt::AlignCenter, "No data yet");
        return;
    }

    const QRectF area = rect().adjusted(8, 8, -8, -8);
    const double side = qMin(area.width() * 0.5, area.height());
    const QRectF pie(area.left(), area.top() + (area.height() - side) / 2.0,
                     side, side);

    const double values[3] = {m_transport, m_electricity, m_food};
    const QColor colors[3] = {kTransport, kElectricity, kFood};
    const QString names[3] = {"Transport", "Electricity", "Food"};

    int start = 90 * 16;
    p.setPen(Qt::NoPen);
    for (int i = 0; i < 3; ++i) {
        const int span = -qRound(values[i] / total * 360.0 * 16.0);
        p.setBrush(colors[i]);
        p.drawPie(pie, start, span);
        start += span;
    }

    // Hole + total label
    p.setBrush(Qt::white);
    p.drawEllipse(pie.center(), side * 0.3, side * 0.3);
    p.setPen(kText);
    QFont f = font();
    f.setBold(true);
    f.setPointSize(f.pointSize() + 2);
    p.setFont(f);
    p.drawText(QRectF(pie.center().x() - side * 0.3, pie.center().y() - 14,
                      side * 0.6, 20),
               Qt::AlignCenter, QString::number(total, 'f', 1));
    f.setBold(false);
    f.setPointSize(f.pointSize() - 4);
    p.setFont(f);
    p.setPen(kMuted);
    p.drawText(QRectF(pie.center().x() - side * 0.3, pie.center().y() + 6,
                      side * 0.6, 16),
               Qt::AlignCenter, "kg CO2e");

    // Legend
    f.setPointSize(font().pointSize());
    p.setFont(f);
    double y = pie.top() + side / 2.0 - 3 * 14;
    const double x = pie.right() + 24;
    for (int i = 0; i < 3; ++i) {
        p.setPen(Qt::NoPen);
        p.setBrush(colors[i]);
        p.drawRoundedRect(QRectF(x, y + 3, 12, 12), 3, 3);
        p.setPen(kText);
        const double pct = values[i] / total * 100.0;
        p.drawText(QRectF(x + 20, y, area.right() - x - 20, 20),
                   Qt::AlignVCenter | Qt::AlignLeft,
                   QString("%1  %2 kg (%3%)")
                       .arg(names[i])
                       .arg(values[i], 0, 'f', 2)
                       .arg(pct, 0, 'f', 0));
        y += 28;
    }
}

// ---------------- TrendChart ----------------

TrendChart::TrendChart(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(200);
}

void TrendChart::setData(const QVector<double> &values, const QStringList &labels)
{
    m_values = values;
    m_labels = labels;
    update();
}

void TrendChart::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (m_values.isEmpty()) {
        p.setPen(kMuted);
        p.drawText(rect(), Qt::AlignCenter, "No saved reports yet");
        return;
    }

    const QRectF plot(46, 14, width() - 62, height() - 54);
    const double maxV = qMax(1.0, *std::max_element(m_values.begin(), m_values.end()));
    const double top = qCeil(maxV * 1.15);

    // Grid + y labels
    p.setPen(QPen(QColor("#e3e9e5"), 1));
    for (int i = 0; i <= 4; ++i) {
        const double y = plot.bottom() - plot.height() * i / 4.0;
        p.drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));
        p.setPen(kMuted);
        p.drawText(QRectF(0, y - 9, plot.left() - 6, 18),
                   Qt::AlignRight | Qt::AlignVCenter,
                   QString::number(top * i / 4.0, 'f', 0));
        p.setPen(QPen(QColor("#e3e9e5"), 1));
    }

    // Bars
    const int n = m_values.size();
    const double slot = plot.width() / n;
    const double barW = qMin(48.0, slot * 0.6);
    for (int i = 0; i < n; ++i) {
        const double h = plot.height() * m_values[i] / top;
        const QRectF bar(plot.left() + slot * i + (slot - barW) / 2.0,
                         plot.bottom() - h, barW, h);
        QString cat = m_values[i] < 3.0 ? "Low" : (m_values[i] < 7.0 ? "Moderate" : "High");
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(cat == "Low" ? "#2e9e5b" : cat == "Moderate" ? "#e0a21b" : "#d64545"));
        p.drawRoundedRect(bar, 4, 4);

        p.setPen(kText);
        p.drawText(QRectF(bar.left() - 10, bar.top() - 18, barW + 20, 16),
                   Qt::AlignCenter, QString::number(m_values[i], 'f', 1));
        if (i < m_labels.size()) {
            p.setPen(kMuted);
            p.drawText(QRectF(plot.left() + slot * i, plot.bottom() + 4, slot, 30),
                       Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap,
                       m_labels[i]);
        }
    }
}