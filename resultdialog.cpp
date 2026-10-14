
#include "resultdialog.h"
#include "chartwidgets.h"
#include "theme.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace {

QLabel *makeLabel(const QString &text,
                  const QString &style = QString(),
                  bool wrap = false)
{
    auto *label = new QLabel(text);
    label->setWordWrap(wrap);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);

    if (!style.isEmpty())
        label->setStyleSheet(style);

    return label;
}

QFrame *makeCard()
{
    auto *frame = new QFrame;
    frame->setObjectName("card");
    return frame;
}

} // namespace

ResultDialog::ResultDialog(const ResultData &d, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Carbon Emission Results");

    // Keep the normal Windows title bar and close (X) button.
    setWindowFlags(
        Qt::Dialog |
        Qt::WindowTitleHint |
        Qt::WindowSystemMenuHint |
        Qt::WindowCloseButtonHint
        );

    setModal(true);
    setMinimumSize(560, 500);
    resize(650, 760);
    setStyleSheet(Theme::styleSheet());

    // Root layout
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);
    root->setSpacing(0);

    // Scrollable report content
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto *content = new QWidget;
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(10, 8, 10, 16);
    contentLayout->setSpacing(14);

    const QString color = Theme::categoryColor(d.category);

    // ---- Header ----
    auto *header = new QHBoxLayout;
    auto *titleBox = new QVBoxLayout;
    titleBox->setSpacing(3);

    titleBox->addWidget(makeLabel(
        "Calculation complete",
        "font-size:11pt; color:#5c6b63;"
        ));

    titleBox->addWidget(makeLabel(
        QString("%1 kg CO2e").arg(d.total, 0, 'f', 2),
        "font-size:26pt; font-weight:700; color:#14402d;"
        ));

    titleBox->addWidget(makeLabel(
        QString("Report for %1").arg(d.userName),
        "color:#5c6b63;"
        ));

    header->addLayout(titleBox);
    header->addStretch();

    auto *badge = makeLabel(
        d.category + " footprint",
        QString(
            "background:%1; color:white; font-weight:700;"
            "border-radius:14px; padding:6px 16px;"
            ).arg(color)
        );

    badge->setAlignment(Qt::AlignCenter);
    header->addWidget(badge, 0, Qt::AlignTop);
    contentLayout->addLayout(header);

    // ---- Emission breakdown card ----
    auto *breakdown = makeCard();
    auto *breakdownLayout = new QVBoxLayout(breakdown);
    breakdownLayout->setContentsMargins(16, 12, 16, 12);
    breakdownLayout->setSpacing(8);

    breakdownLayout->addWidget(makeLabel(
        "Emission breakdown",
        "font-weight:700; font-size:11pt; border:none;"
        ));

    auto *donut = new DonutChart;
    donut->setData(
        d.transportEmissions,
        d.electricityEmissions,
        d.foodEmissions
        );
    donut->setMinimumHeight(180);
    donut->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Minimum
        );

    breakdownLayout->addWidget(donut);
    contentLayout->addWidget(breakdown);

    // ---- Activity details card ----
    auto *inputs = makeCard();
    auto *grid = new QGridLayout(inputs);
    grid->setContentsMargins(16, 12, 16, 12);
    grid->setHorizontalSpacing(24);
    grid->setVerticalSpacing(8);

    const QString keyStyle = "color:#5c6b63; border:none;";
    const QString valueStyle = "font-weight:600; border:none;";

    auto addRow = [&](int row, const QString &key,
                      const QString &value) {
        grid->addWidget(makeLabel(key, keyStyle), row, 0);
        grid->addWidget(makeLabel(value, valueStyle, true), row, 1);
    };

    addRow(0, "Transport",
           QString("%1, %2 km")
               .arg(d.transportType)
               .arg(d.distance, 0, 'f', 2));

    addRow(1, "Electricity",
           QString("%1 kWh").arg(d.electricity, 0, 'f', 2));

    addRow(2, "Food",
           QString("%1, %2 servings")
               .arg(d.dietCategory)
               .arg(d.servings));

    addRow(3, "Activities recorded",
           QString::number(d.activityCount));

    grid->setColumnStretch(1, 1);
    contentLayout->addWidget(inputs);

    // ---- Advice card ----
    auto *advice = makeCard();

    advice->setStyleSheet(QString(
                              "QFrame#card { border-left: 5px solid %1; }"
                              ).arg(color));

    auto *adviceLayout = new QVBoxLayout(advice);
    adviceLayout->setContentsMargins(16, 12, 16, 12);
    adviceLayout->setSpacing(10);

    adviceLayout->addWidget(makeLabel(
        "What this means",
        "font-weight:700; border:none;"
        ));

    adviceLayout->addWidget(makeLabel(
        d.analysis,
        "border:none;",
        true
        ));

    // Each recommendation is shown as its own wrapped text block.
    for (const QString &recommendation : d.recommendations) {
        auto *recommendationLabel = makeLabel(
            recommendation,
            "border:none;",
            true
            );

        recommendationLabel->setTextFormat(Qt::PlainText);
        recommendationLabel->setSizePolicy(
            QSizePolicy::Preferred,
            QSizePolicy::Minimum
            );

        adviceLayout->addWidget(recommendationLabel);
    }

    contentLayout->addWidget(advice);

    // ---- Threshold note ----
    contentLayout->addWidget(makeLabel(
        "Thresholds: Low < 3, Moderate 3 to < 7, High >= 7 kg CO2e/day. "
        "Thresholds and emission factors are illustrative project values, "
        "not universal scientific standards.",
        "color:#7a8a82; font-size:8.5pt;",
        true
        ));

    contentLayout->addStretch();

    scrollArea->setWidget(content);
    root->addWidget(scrollArea, 1);

    // ---- Fixed footer ----
    auto *footer = new QHBoxLayout;
    footer->setContentsMargins(8, 12, 8, 4);

    footer->addWidget(makeLabel(
        d.saved ? "Saved to My Reports" : "",
        "color:#23775a; font-weight:600;"
        ));

    footer->addStretch();

    auto *doneButton = new QPushButton("Done");
    doneButton->setObjectName("primaryButton");
    doneButton->setDefault(true);
    doneButton->setMinimumSize(110, 40);

    connect(doneButton, &QPushButton::clicked,
            this, &QDialog::accept);

    footer->addWidget(doneButton);
    root->addLayout(footer);
}
