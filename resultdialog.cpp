#include "resultdialog.h"
#include "chartwidgets.h"
#include "theme.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace {

QLabel *makeLabel(const QString &text, const QString &style = QString(),
                  bool wrap = false)
{
    auto *l = new QLabel(text);
    l->setWordWrap(wrap);
    if (!style.isEmpty())
        l->setStyleSheet(style);
    return l;
}

QFrame *makeCard()
{
    auto *f = new QFrame;
    f->setObjectName("card");
    return f;
}

} // namespace

ResultDialog::ResultDialog(const ResultData &d, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Carbon Emission Results");
    setModal(true);
    setMinimumWidth(560);
    setStyleSheet(Theme::styleSheet());

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(22, 20, 22, 20);
    root->setSpacing(14);

    // ---- Header: total + badge ----
    const QString color = Theme::categoryColor(d.category);

    auto *header = new QHBoxLayout;
    auto *titleBox = new QVBoxLayout;
    titleBox->setSpacing(2);
    titleBox->addWidget(makeLabel("Calculation complete",
                                  "font-size:11pt; color:#5c6b63;"));
    titleBox->addWidget(makeLabel(
        QString("%1 kg CO2e").arg(d.total, 0, 'f', 2),
        "font-size:26pt; font-weight:700; color:#14402d;"));
    titleBox->addWidget(makeLabel(QString("Report for %1").arg(d.userName),
                                  "color:#5c6b63;"));
    header->addLayout(titleBox);
    header->addStretch();

    auto *badge = makeLabel(
        d.category + " footprint",
        QString("background:%1; color:white; font-weight:700; "
                "border-radius:14px; padding:6px 16px;").arg(color));
    badge->setAlignment(Qt::AlignCenter);
    header->addWidget(badge, 0, Qt::AlignTop);
    root->addLayout(header);

    // ---- Breakdown card: donut + details ----
    auto *breakdown = makeCard();
    auto *bl = new QVBoxLayout(breakdown);
    bl->setContentsMargins(16, 12, 16, 12);
    bl->addWidget(makeLabel("Emission breakdown",
                            "font-weight:700; font-size:11pt; border:none;"));
    auto *donut = new DonutChart;
    donut->setData(d.transportEmissions, d.electricityEmissions, d.foodEmissions);
    donut->setMinimumHeight(170);
    bl->addWidget(donut);
    root->addWidget(breakdown);

    // ---- Inputs card ----
    auto *inputs = makeCard();
    auto *grid = new QGridLayout(inputs);
    grid->setContentsMargins(16, 12, 16, 12);
    grid->setHorizontalSpacing(24);
    grid->setVerticalSpacing(6);

    const QString key = "color:#5c6b63; border:none;";
    const QString val = "font-weight:600; border:none;";
    auto addRow = [&](int r, const QString &k, const QString &v) {
        grid->addWidget(makeLabel(k, key), r, 0);
        grid->addWidget(makeLabel(v, val), r, 1);
    };
    addRow(0, "Transport", QString("%1, %2 km").arg(d.transportType).arg(d.distance, 0, 'f', 2));
    addRow(1, "Electricity", QString("%1 kWh").arg(d.electricity, 0, 'f', 2));
    addRow(2, "Food", QString("%1, %2 servings").arg(d.dietCategory).arg(d.servings));
    addRow(3, "Activities recorded", QString::number(d.activityCount));
    grid->setColumnStretch(1, 1);
    root->addWidget(inputs);

    // ---- Advice card ----
    auto *advice = makeCard();
    advice->setStyleSheet(QString("QFrame#card { border-left: 5px solid %1; }").arg(color));
    auto *al = new QVBoxLayout(advice);
    al->setContentsMargins(16, 12, 16, 12);
    al->addWidget(makeLabel("What this means", "font-weight:700; border:none;"));
    al->addWidget(makeLabel(d.analysis, "border:none;", true));
    for (const QString &rec : d.recommendations)
        al->addWidget(makeLabel(QString::fromUtf8("\u2022 ") + rec, "border:none;", true));
    root->addWidget(advice);

    root->addWidget(makeLabel(
        "Thresholds: Low < 3, Moderate 3 to < 7, High >= 7 kg CO2e/day. "
        "Thresholds and emission factors are illustrative project values, "
        "not universal scientific standards.",
        "color:#7a8a82; font-size:8.5pt;", true));

    // ---- Footer ----
    auto *footer = new QHBoxLayout;
    footer->addWidget(makeLabel(d.saved ? "Saved to My Reports" : "",
                                "color:#23775a; font-weight:600;"));
    footer->addStretch();
    auto *ok = new QPushButton("Done");
    ok->setObjectName("primaryButton");
    ok->setDefault(true);
    ok->setMinimumWidth(110);
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);
    footer->addWidget(ok);
    root->addLayout(footer);
}