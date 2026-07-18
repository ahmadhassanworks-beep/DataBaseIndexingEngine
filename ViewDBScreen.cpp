#include "ViewDBScreen.h"
#include <QFont>
#include <QDir>
#include <QMessageBox>

ViewDBScreen::ViewDBScreen(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void ViewDBScreen::setupUI() {
    setStyleSheet("background-color: #0d1117; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("Existing Databases");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #4d9fff;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *subtitle = new QLabel("Select a database to open");
    subtitle->setStyleSheet("color: #8b949e; font-size: 12px;");
    subtitle->setAlignment(Qt::AlignCenter);

    dbListWidget = new QListWidget();
    dbListWidget->setStyleSheet(
        "QListWidget { background-color: #161b22; border: 1px solid #30363d; "
        "border-radius: 8px; font-size: 14px; color: white; }"
        "QListWidget::item { padding: 14px 16px; border-bottom: 1px solid #21262d; }"
        "QListWidget::item:selected { background-color: #1f3a5f; color: #4d9fff; }"
        "QListWidget::item:hover { background-color: #1f2937; }"
        );
    dbListWidget->setMinimumHeight(300);

    emptyLabel = new QLabel("No databases exist yet.\nCreate one first from the Home screen.");
    emptyLabel->setStyleSheet("color: #8b949e; font-size: 13px;");
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setVisible(false);

    openBtn = new QPushButton("📂  Open Database");
    openBtn->setFixedHeight(48);
    openBtn->setStyleSheet(
        "QPushButton { background-color: #4d9fff; color: white; "
        "font-size: 14px; font-weight: bold; border-radius: 10px; border: none; }"
        "QPushButton:hover { background-color: #3a8eef; }"
        "QPushButton:disabled { background-color: #21262d; color: #8b949e; }"
        );

    backBtn = new QPushButton("← Back to Home");
    backBtn->setFixedHeight(38);
    backBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: #e05c5c; "
        "border: 1px solid #e05c5c; border-radius: 8px; font-size: 13px; }"
        "QPushButton:hover { background-color: #2a1a1a; }"
        );

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitle);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(dbListWidget);
    mainLayout->addWidget(emptyLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(openBtn);
    mainLayout->addStretch();
    mainLayout->addWidget(backBtn);

    connect(dbListWidget, &QListWidget::itemDoubleClicked,
            this, &ViewDBScreen::onItemDoubleClicked);
    connect(openBtn, &QPushButton::clicked, this, &ViewDBScreen::onOpenClicked);
    connect(backBtn, &QPushButton::clicked, this, &ViewDBScreen::onBackClicked);
}

void ViewDBScreen::refreshList() {
    dbListWidget->clear();

    QDir dir("databases");
    if (!dir.exists()) {
        emptyLabel->setVisible(true);
        dbListWidget->setVisible(false);
        openBtn->setEnabled(false);
        return;
    }

    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if (folders.isEmpty()) {
        emptyLabel->setVisible(true);
        dbListWidget->setVisible(false);
        openBtn->setEnabled(false);
    } else {
        emptyLabel->setVisible(false);
        dbListWidget->setVisible(true);
        openBtn->setEnabled(true);
        for (auto &name : folders) {
            QListWidgetItem *item = new QListWidgetItem("🗄  " + name);
            item->setData(Qt::UserRole, name);
            dbListWidget->addItem(item);
        }
    }
}

void ViewDBScreen::onItemDoubleClicked(QListWidgetItem *item) {
    emit databaseSelected(item->data(Qt::UserRole).toString());
}

void ViewDBScreen::onOpenClicked() {
    QListWidgetItem *item = dbListWidget->currentItem();
    if (!item) {
        QMessageBox::information(this, "Select a Database",
                                 "Please select a database from the list first.");
        return;
    }
    emit databaseSelected(item->data(Qt::UserRole).toString());
}

void ViewDBScreen::onBackClicked() {
    emit backToHomeClicked();
}