#pragma once
#include <QString>

namespace Theme {

inline QString categoryColor(const QString &category)
{
    if (category == "Low")      return "#2e9e5b";
    if (category == "Moderate") return "#e0a21b";
    if (category == "High")     return "#d64545";
    return "#7a8a82";
}

inline QString styleSheet()
{
    return R"(
QWidget { font-family: "Segoe UI"; font-size: 10pt; color: #1f3d2f; }
QMainWindow, QStackedWidget, QScrollArea { background: #f4f6f4; }
QLabel#pageTitle { font-size: 24pt; font-weight: 700; color: #14402d; }
QLabel#pageSubtitle { color: #5c6b63; }
QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
    background: white; border: 1px solid #cfd8d2; border-radius: 6px;
    padding: 6px 8px; min-height: 22px;
}
QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
    border: 1px solid #23775a;
}
QPushButton {
    background: white; color: #23775a; border: 1px solid #23775a;
    border-radius: 6px; padding: 9px 16px; font-weight: 600;
}
QPushButton:hover { background: #e8f3ee; }
QPushButton#primaryButton { background: #23775a; color: white; border: none; }
QPushButton#primaryButton:hover { background: #1c6149; }
QPushButton#dangerButton { color: #c0392b; border: 1px solid #c0392b; }
QPushButton#dangerButton:hover { background: #fbeceb; }
QGroupBox, QFrame#card {
    background: white; border: 1px solid #dfe6e1; border-radius: 10px;
}
QGroupBox { margin-top: 14px; padding: 14px 12px 12px 12px; font-weight: 700; }
QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 4px; color: #14402d; }
QTableWidget, QTableView {
    background: white; alternate-background-color: #f4f8f5;
    border: 1px solid #dfe6e1; border-radius: 8px;
    gridline-color: transparent; selection-background-color: #cfe8db;
    selection-color: #14402d;
}
QHeaderView::section {
    background: #e4efe8; color: #14402d; font-weight: 700;
    padding: 8px; border: none; border-bottom: 1px solid #cfd8d2;
}
QProgressBar {
    background: #e8eeea; border: none; border-radius: 5px;
    height: 10px; text-align: center; color: transparent;
}
)";
}

} // namespace Theme