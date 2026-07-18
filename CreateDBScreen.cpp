#include "CreateDBScreen.h"
#include <QHBoxLayout>
#include <QFont>
#include <QSet>

CreateDBScreen::CreateDBScreen(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void CreateDBScreen::setupUI() {
    setStyleSheet("background-color: #0d1117; color: white;");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // --- LEFT PANEL ---
    QWidget *leftPanel = new QWidget();
    leftPanel->setStyleSheet(
        "background-color: #161b22; border-radius: 12px;"
        );
    leftPanel->setFixedWidth(380);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(25, 25, 25, 25);
    leftLayout->setSpacing(14);

    QLabel *titleLabel = new QLabel("Define Form Columns");
    titleLabel->setStyleSheet("color: #4d9fff; font-size: 18px; font-weight: bold; background: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *nameLabel = new QLabel("Table Name:");
    nameLabel->setStyleSheet("color: #8b949e; font-size: 12px; background: transparent;");

    tableNameInput = new QLineEdit();
    tableNameInput->setPlaceholderText("e.g. Students, Appointments");
    tableNameInput->setFixedHeight(38);
    tableNameInput->setStyleSheet(
        "QLineEdit { background-color: #0d1117; color: white; "
        "border: 1px solid #30363d; border-radius: 6px; padding: 0 10px; font-size: 13px; }"
        "QLineEdit:focus { border: 1px solid #4d9fff; }"
        );

    QLabel *colLabel = new QLabel("Number of Columns (2-6):");
    colLabel->setStyleSheet("color: #8b949e; font-size: 12px; background: transparent;");

    QHBoxLayout *colRow = new QHBoxLayout();
    columnCountInput = new QSpinBox();
    columnCountInput->setRange(2, 6);
    columnCountInput->setValue(2);
    columnCountInput->setFixedHeight(36);
    columnCountInput->setFixedWidth(70);
    columnCountInput->setStyleSheet(
        "QSpinBox { background-color: #0d1117; color: white; "
        "border: 1px solid #30363d; border-radius: 6px; padding: 0 8px; }"
        );

    setColumnsBtn = new QPushButton("Set Columns");
    setColumnsBtn->setFixedHeight(36);
    setColumnsBtn->setStyleSheet(
        "QPushButton { background-color: #4d9fff; color: white; "
        "border-radius: 6px; font-size: 13px; padding: 0 16px; border: none; }"
        "QPushButton:hover { background-color: #3a8eef; }"
        );
    colRow->addWidget(columnCountInput);
    colRow->addWidget(setColumnsBtn);
    colRow->addStretch();

    keyHintLabel = new QLabel("★ First column = numeric key (index)");
    keyHintLabel->setStyleSheet("color: #f0883e; font-size: 11px; background: transparent;");

    columnFieldsLayout = new QVBoxLayout();
    columnFieldsLayout->setSpacing(8);

    saveBtn = new QPushButton("Save Columns");
    saveBtn->setFixedHeight(44);
    saveBtn->setEnabled(false);
    saveBtn->setStyleSheet(
        "QPushButton:enabled { background-color: #4d9fff; color: white; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; border: none; }"
        "QPushButton:disabled { background-color: #21262d; color: #8b949e; border-radius: 8px; }"
        "QPushButton:hover:enabled { background-color: #3a8eef; }"
        );

    statusLabel = new QLabel("");
    statusLabel->setStyleSheet("color: #8b949e; font-size: 12px; background: transparent;");
    statusLabel->setWordWrap(true);

    backBtn = new QPushButton("← Back to Home");
    backBtn->setFixedHeight(36);
    backBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: #e05c5c; "
        "border: 1px solid #e05c5c; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background-color: #2a1a1a; }"
        );

    leftLayout->addWidget(titleLabel);
    leftLayout->addSpacing(5);
    leftLayout->addWidget(nameLabel);
    leftLayout->addWidget(tableNameInput);
    leftLayout->addWidget(colLabel);
    leftLayout->addLayout(colRow);
    leftLayout->addWidget(keyHintLabel);
    leftLayout->addLayout(columnFieldsLayout);
    leftLayout->addSpacing(5);
    leftLayout->addWidget(saveBtn);
    leftLayout->addWidget(statusLabel);
    leftLayout->addStretch();
    leftLayout->addWidget(backBtn);

    // --- RIGHT PANEL ---
    QWidget *rightPanel = new QWidget();
    rightPanel->setStyleSheet(
        "background-color: #161b22; border-radius: 12px;"
        );
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(25, 25, 25, 25);
    rightLayout->setSpacing(12);

    QLabel *rightTitle = new QLabel("Column Preview");
    rightTitle->setStyleSheet("color: #4d9fff; font-size: 18px; font-weight: bold; background: transparent;");
    rightTitle->setAlignment(Qt::AlignCenter);

    QLabel *previewHint = new QLabel("Your column names will appear here after clicking Set Columns.");
    previewHint->setStyleSheet("color: #8b949e; font-size: 12px; background: transparent;");
    previewHint->setWordWrap(true);
    previewHint->setAlignment(Qt::AlignCenter);

    previewTable = new QTableWidget(0, 0);
    previewTable->setStyleSheet(
        "QTableWidget { background-color: #0d1117; color: white; "
        "gridline-color: #30363d; border: 1px solid #30363d; border-radius: 6px; }"
        "QHeaderView::section { background-color: #21262d; color: #4d9fff; "
        "font-weight: bold; border: none; padding: 6px; }"
        "QTableWidget::item { padding: 6px; }"
        );
    previewTable->horizontalHeader()->setStretchLastSection(true);
    previewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    previewTable->verticalHeader()->setVisible(false);

    rightLayout->addWidget(rightTitle);
    rightLayout->addWidget(previewHint);
    rightLayout->addWidget(previewTable);

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);

    connect(setColumnsBtn, &QPushButton::clicked, this, &CreateDBScreen::onSetColumnsClicked);
    connect(saveBtn, &QPushButton::clicked, this, &CreateDBScreen::onSaveClicked);
    connect(backBtn, &QPushButton::clicked, this, &CreateDBScreen::onBackClicked);
}

void CreateDBScreen::onSetColumnsClicked() {
    int count = columnCountInput->value();
    generateColumnFields(count);

    previewTable->setColumnCount(count);
    QStringList headers;
    for (int i = 0; i < count; i++)
        headers << QString("Column %1").arg(i + 1);
    previewTable->setHorizontalHeaderLabels(headers);
    previewTable->setRowCount(1);
    for (int i = 0; i < count; i++)
        previewTable->setItem(0, i, new QTableWidgetItem(i == 0 ? "🔑 Key" : "—"));

    saveBtn->setEnabled(true);
    statusLabel->setText("Enter column names below.");
    statusLabel->setStyleSheet("color: #8b949e; font-size: 12px; background: transparent;");
}

void CreateDBScreen::generateColumnFields(int count) {
    clearColumnFields();
    for (int i = 0; i < count; i++) {
        QLineEdit *field = new QLineEdit();
        if (i == 0)
            field->setPlaceholderText("Column 1 — Key (e.g. ID)");
        else
            field->setPlaceholderText(QString("Column %1 name").arg(i + 1));
        field->setFixedHeight(36);
        field->setStyleSheet(
            "QLineEdit { background-color: #0d1117; color: white; "
            "border: 1px solid #30363d; border-radius: 6px; padding: 0 10px; font-size: 13px; }"
            "QLineEdit:focus { border: 1px solid #4d9fff; }"
            );
        connect(field, &QLineEdit::textChanged, this, [this, i](const QString &text) {
            if (previewTable->columnCount() > i)
                previewTable->setHorizontalHeaderItem(i, new QTableWidgetItem(
                                                             i == 0 ? "🔑 " + text : text));
        });
        columnFieldsLayout->addWidget(field);
        columnInputs.append(field);
    }
}

void CreateDBScreen::clearColumnFields() {
    for (auto *field : columnInputs) {
        columnFieldsLayout->removeWidget(field);
        delete field;
    }
    columnInputs.clear();
}

void CreateDBScreen::onSaveClicked() {
    QString tableName = tableNameInput->text().trimmed();
    if (tableName.isEmpty()) {
        statusLabel->setText("Please enter a table name.");
        statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
        return;
    }
    if (columnInputs.isEmpty()) {
        statusLabel->setText("Please set columns first.");
        statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
        return;
    }
    QVector<QString> columnNames;
    QSet<QString> seen;
    for (auto *field : columnInputs) {
        QString name = field->text().trimmed();
        if (name.isEmpty()) {
            statusLabel->setText("All column names must be filled.");
            statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
            return;
        }
        if (seen.contains(name.toLower())) {
            statusLabel->setText("Column names must be unique.");
            statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
            return;
        }
        seen.insert(name.toLower());
        columnNames.append(name);
    }
    emit tableCreated(tableName, columnNames);
}

void CreateDBScreen::onBackClicked() {
    emit backToHomeClicked();
}

void CreateDBScreen::resetScreen() {
    tableNameInput->clear();
    columnCountInput->setValue(2);
    clearColumnFields();
    previewTable->setColumnCount(0);
    previewTable->setRowCount(0);
    saveBtn->setEnabled(false);
    statusLabel->setText("");
}