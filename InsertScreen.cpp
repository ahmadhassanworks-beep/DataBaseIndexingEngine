#include "InsertScreen.h"
#include <QHBoxLayout>
#include <QFont>
#include <QHeaderView>

InsertScreen::InsertScreen(QWidget *parent) : QWidget(parent) {
    tree = nullptr;
    setupUI();
}

void InsertScreen::setupUI() {
    setStyleSheet("background-color: #0d1117; color: white;");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // --- LEFT PANEL ---
    QWidget *leftPanel = new QWidget();
    leftPanel->setStyleSheet("background-color: #161b22; border-radius: 12px;");
    leftPanel->setFixedWidth(320);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(25, 25, 25, 25);
    leftLayout->setSpacing(12);

    titleLabel = new QLabel("Insert Records");
    titleLabel->setStyleSheet("color: #4d9fff; font-size: 18px; font-weight: bold; background: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);

    fieldsLayout = new QVBoxLayout();
    fieldsLayout->setSpacing(8);

    insertBtn = new QPushButton("＋  Insert Record");
    insertBtn->setFixedHeight(44);
    insertBtn->setStyleSheet(
        "QPushButton { background-color: #4d9fff; color: white; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; border: none; }"
        "QPushButton:hover { background-color: #3a8eef; }"
        "QPushButton:pressed { background-color: #2a7edf; }"
        );

    saveBtn = new QPushButton("✔  Save && Finish");
    saveBtn->setFixedHeight(44);
    saveBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: #4d9fff; "
        "font-size: 14px; font-weight: bold; border: 2px solid #4d9fff; border-radius: 8px; }"
        "QPushButton:hover { background-color: #1f2937; }"
        );

    countLabel = new QLabel("0 / 150 records");
    countLabel->setStyleSheet("color: #8b949e; font-size: 11px; background: transparent;");
    countLabel->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel("");
    statusLabel->setStyleSheet("color: #8b949e; font-size: 12px; background: transparent;");
    statusLabel->setWordWrap(true);
    statusLabel->setAlignment(Qt::AlignCenter);

    leftLayout->addWidget(titleLabel);
    leftLayout->addSpacing(8);
    leftLayout->addLayout(fieldsLayout);
    leftLayout->addSpacing(8);
    leftLayout->addWidget(insertBtn);
    leftLayout->addWidget(saveBtn);
    leftLayout->addWidget(countLabel);
    leftLayout->addWidget(statusLabel);
    leftLayout->addStretch();

    // --- RIGHT PANEL ---
    QWidget *rightPanel = new QWidget();
    rightPanel->setStyleSheet("background-color: #161b22; border-radius: 12px;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(25, 25, 25, 25);
    rightLayout->setSpacing(12);

    QLabel *rightTitle = new QLabel("Database Records");
    rightTitle->setStyleSheet("color: #4d9fff; font-size: 18px; font-weight: bold; background: transparent;");
    rightTitle->setAlignment(Qt::AlignCenter);

    previewTable = new QTableWidget(0, 0);
    previewTable->setStyleSheet(
        "QTableWidget { background-color: #0d1117; color: white; "
        "gridline-color: #30363d; border: 1px solid #30363d; border-radius: 6px; }"
        "QHeaderView::section { background-color: #21262d; color: #4d9fff; "
        "font-weight: bold; border: none; padding: 6px; }"
        "QTableWidget::item { padding: 6px; }"
        "QTableWidget::item:selected { background-color: #1f3a5f; }"
        );
    previewTable->horizontalHeader()->setStretchLastSection(true);
    previewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    previewTable->verticalHeader()->setVisible(false);

    rightLayout->addWidget(rightTitle);
    rightLayout->addWidget(previewTable);

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);

    connect(insertBtn, &QPushButton::clicked, this, &InsertScreen::onInsertClicked);
    connect(saveBtn, &QPushButton::clicked, this, &InsertScreen::onSaveClicked);
}

void InsertScreen::setupForTable(QString tableName, QVector<QString> columns, BPlusTree *treePtr, QString filePath) {
    currentTableName = tableName;
    currentColumns = columns;
    tree = treePtr;
    dbFilePath = filePath;

    titleLabel->setText("Insert — " + tableName);

    buildInputFields();

    previewTable->setColumnCount(columns.size());
    QStringList headers;
    for (auto &c : columns) headers << c;
    previewTable->setHorizontalHeaderLabels(headers);
    previewTable->setRowCount(0);

    statusLabel->setText("");
    updateCountLabel();
}

void InsertScreen::buildInputFields() {
    for (auto *field : fieldInputs) {
        fieldsLayout->removeWidget(field);
        delete field;
    }
    fieldInputs.clear();

    for (int i = 0; i < currentColumns.size(); i++) {
        QLineEdit *field = new QLineEdit();
        if (i == 0)
            field->setPlaceholderText(currentColumns[i] + " (key — number)");
        else
            field->setPlaceholderText(currentColumns[i]);
        field->setFixedHeight(36);
        field->setStyleSheet(
            "QLineEdit { background-color: #0d1117; color: white; "
            "border: 1px solid #30363d; border-radius: 6px; padding: 0 10px; font-size: 13px; }"
            "QLineEdit:focus { border: 1px solid #4d9fff; }"
            );
        fieldsLayout->addWidget(field);
        fieldInputs.append(field);
    }
}

void InsertScreen::onInsertClicked() {
    if (!tree) return;

    if (tree->isFull()) {
        statusLabel->setText("Table full — max 150 records reached.");
        statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
        return;
    }

    QString keyText = fieldInputs[0]->text().trimmed();
    bool ok;
    int key = keyText.toInt(&ok);
    if (!ok || keyText.isEmpty()) {
        statusLabel->setText(currentColumns[0] + " must be a number.");
        statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
        return;
    }

    QVector<QString> values;
    for (int i = 1; i < fieldInputs.size(); i++) {
        QString val = fieldInputs[i]->text().trimmed();
        if (val.isEmpty()) {
            statusLabel->setText("Please fill all fields.");
            statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
            return;
        }
        values.append(val);
    }

    bool success = tree->insert(key, values);
    if (!success) {
        statusLabel->setText("Duplicate key — record already exists.");
        statusLabel->setStyleSheet("color: #e05c5c; font-size: 12px; background: transparent;");
        return;
    }

    for (int i = 0; i < fieldInputs.size(); i++) fieldInputs[i]->clear();
    statusLabel->setText("✔ Record inserted successfully.");
    statusLabel->setStyleSheet("color: #3fb950; font-size: 12px; background: transparent;");

    refreshPreview();
    updateCountLabel();
}

void InsertScreen::refreshPreview() {
    previewTable->setRowCount(0);
    QVector<Record> all;
    tree->getAllRecords(all);
    for (auto &r : all) {
        int row = previewTable->rowCount();
        previewTable->insertRow(row);
        previewTable->setItem(row, 0, new QTableWidgetItem(QString::number(r.key)));
        for (int i = 0; i < r.values.size(); i++)
            previewTable->setItem(row, i + 1, new QTableWidgetItem(r.values[i]));
    }
}

void InsertScreen::updateCountLabel() {
    int count = tree ? tree->totalRecords : 0;
    countLabel->setText(QString::number(count) + " / 150 records");
}

void InsertScreen::onSaveClicked() {
    if (tree)
        tree->saveToFile(dbFilePath, currentColumns);
    emit finishedInserting();
}