#include "HomeScreen.h"
#include <QVBoxLayout>
#include <QFont>

HomeScreen::HomeScreen(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void HomeScreen::setupUI() {
    setStyleSheet("background-color: #0d1117;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel *icon = new QLabel("🗄");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size: 48px;");

    titleLabel = new QLabel("Database Indexing Engine");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #4d9fff;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *subtitle = new QLabel("Powered by B+ Tree");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #8b949e; font-size: 13px;");

    createBtn = new QPushButton("＋  Create Database");
    createBtn->setFixedSize(260, 52);
    createBtn->setStyleSheet(
        "QPushButton { background-color: #4d9fff; color: white; "
        "font-size: 15px; font-weight: bold; border-radius: 10px; border: none; }"
        "QPushButton:hover { background-color: #3a8eef; }"
        "QPushButton:pressed { background-color: #2a7edf; }"
        );

    viewBtn = new QPushButton("📂  View Existing Databases");
    viewBtn->setFixedSize(260, 52);
    viewBtn->setStyleSheet(
        "QPushButton { background-color: #161b22; color: #4d9fff; "
        "font-size: 15px; font-weight: bold; border: 2px solid #4d9fff; border-radius: 10px; }"
        "QPushButton:hover { background-color: #1f2937; }"
        "QPushButton:pressed { background-color: #2a7edf; color: white; }"
        );

    layout->addWidget(icon);
    layout->addWidget(titleLabel);
    layout->addWidget(subtitle);
    layout->addSpacing(20);
    layout->addWidget(createBtn, 0, Qt::AlignCenter);
    layout->addWidget(viewBtn, 0, Qt::AlignCenter);

    connect(createBtn, &QPushButton::clicked, this, &HomeScreen::createDatabaseClicked);
    connect(viewBtn, &QPushButton::clicked, this, &HomeScreen::viewDatabasesClicked);
}