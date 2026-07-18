#include "DBOperationsScreen.h"
#include <QHBoxLayout>
#include <QFont>
#include <QHeaderView>
#include <QMessageBox>

DBOperationsScreen::DBOperationsScreen(QWidget *parent) : QWidget(parent) {
    tree = nullptr;
    updateCurrentKey = -1;
    setupUI();
}

void DBOperationsScreen::setupUI() {
    setStyleSheet("background-color: #0d1117; color: white;");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- LEFT SIDEBAR ---
    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet("background-color: #161b22;");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(15, 25, 15, 25);
    sideLayout->setSpacing(10);

    titleLabel = new QLabel("Database");
    titleLabel->setStyleSheet("color: #4d9fff; font-size: 16px; font-weight: bold; background: transparent;");
    titleLabel->setWordWrap(true);
    titleLabel->setAlignment(Qt::AlignCenter);

    QString sideBtn =
        "QPushButton { background-color: transparent; color: #c9d1d9; "
        "border: 1px solid #30363d; border-radius: 6px; padding: 10px; "
        "text-align: left; font-size: 13px; }"
        "QPushButton:hover { background-color: #1f2937; color: #4d9fff; border-color: #4d9fff; }";

    viewAllBtn  = new QPushButton("📋  View All Records");
    searchBtn   = new QPushButton("🔍  Search a Record");
    updateBtn   = new QPushButton("✏️  Update a Record");
    deleteBtn   = new QPushButton("🗑  Delete a Record");

    viewAllBtn->setStyleSheet(sideBtn);
    searchBtn->setStyleSheet(sideBtn);
    updateBtn->setStyleSheet(sideBtn);
    deleteBtn->setStyleSheet(sideBtn);

    backBtn = new QPushButton("💾  Save & Back to Home");
    backBtn->setFixedHeight(42);
    backBtn->setStyleSheet(
        "QPushButton { background-color: #4d9fff; color: white; "
        "border-radius: 8px; font-size: 13px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #3a8eef; }"
        );

    sideLayout->addWidget(titleLabel);
    sideLayout->addSpacing(15);
    sideLayout->addWidget(viewAllBtn);
    sideLayout->addWidget(searchBtn);
    sideLayout->addWidget(updateBtn);
    sideLayout->addWidget(deleteBtn);
    sideLayout->addStretch();
    sideLayout->addWidget(backBtn);

    // --- RIGHT CONTENT STACK ---
    contentStack = new QStackedWidget();
    contentStack->setStyleSheet("background-color: #0d1117;");

    viewAllPanel  = buildViewAllPanel();
    searchPanel   = buildSearchPanel();
    updatePanel   = buildUpdatePanel();
    deletePanel   = buildDeletePanel();

    contentStack->addWidget(viewAllPanel);
    contentStack->addWidget(searchPanel);
    contentStack->addWidget(updatePanel);
    contentStack->addWidget(deletePanel);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentStack, 1);

    connect(viewAllBtn, &QPushButton::clicked, this, &DBOperationsScreen::onViewAllClicked);
    connect(searchBtn,  &QPushButton::clicked, this, &DBOperationsScreen::onSearchModeClicked);
    connect(updateBtn,  &QPushButton::clicked, this, &DBOperationsScreen::onUpdateModeClicked);
    connect(deleteBtn,  &QPushButton::clicked, this, &DBOperationsScreen::onDeleteModeClicked);
    connect(backBtn,    &QPushButton::clicked, this, &DBOperationsScreen::onBackClicked);
}

QString DBOperationsScreen::inputStyle() {
    return "QLineEdit { background-color: #161b22; color: white; "
           "border: 1px solid #30363d; border-radius: 6px; "
           "padding: 0 10px; font-size: 13px; }"
           "QLineEdit:focus { border: 1px solid #4d9fff; }";
}

QString DBOperationsScreen::tableStyle() {
    return "QTableWidget { background-color: #161b22; color: white; "
           "gridline-color: #30363d; border: 1px solid #30363d; border-radius: 6px; }"
           "QHeaderView::section { background-color: #21262d; color: #4d9fff; "
           "font-weight: bold; border: none; padding: 6px; }"
           "QTableWidget::item { padding: 6px; }"
           "QTableWidget::item:selected { background-color: #1f3a5f; }";
}

QWidget* DBOperationsScreen::buildViewAllPanel() {
    QWidget *panel = new QWidget();
    panel->setStyleSheet("background-color: #0d1117;");
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(12);

    QLabel *title = new QLabel("All Records");
    title->setStyleSheet("color: #4d9fff; font-size: 16px; font-weight: bold;");

    viewAllTable = new QTableWidget(0, 0);
    viewAllTable->setStyleSheet(tableStyle());
    viewAllTable->horizontalHeader()->setStretchLastSection(true);
    viewAllTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    viewAllTable->verticalHeader()->setVisible(false);

    layout->addWidget(title);
    layout->addWidget(viewAllTable);
    return panel;
}

QWidget* DBOperationsScreen::buildSearchPanel() {
    QWidget *panel = new QWidget();
    panel->setStyleSheet("background-color: #0d1117;");
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(12);

    QLabel *title = new QLabel("Search a Record");
    title->setStyleSheet("color: #4d9fff; font-size: 16px; font-weight: bold;");

    QHBoxLayout *row = new QHBoxLayout();
    searchKeyInput = new QLineEdit();
    searchKeyInput->setPlaceholderText("Enter key to search...");
    searchKeyInput->setFixedHeight(38);
    searchKeyInput->setStyleSheet(inputStyle());

    QPushButton *goBtn = new QPushButton("Search");
    goBtn->setFixedHeight(38);
    goBtn->setFixedWidth(90);
    goBtn->setStyleSheet(
        "QPushButton { background-color: #4d9fff; color: white; "
        "border-radius: 6px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #3a8eef; }"
        );
    row->addWidget(searchKeyInput);
    row->addWidget(goBtn);

    searchResultLabel = new QLabel("");
    searchResultLabel->setStyleSheet("color: #8b949e; font-size: 12px;");

    searchResultTable = new QTableWidget(0, 0);
    searchResultTable->setStyleSheet(tableStyle());
    searchResultTable->horizontalHeader()->setStretchLastSection(true);
    searchResultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    searchResultTable->verticalHeader()->setVisible(false);

    layout->addWidget(title);
    layout->addLayout(row);
    layout->addWidget(searchResultLabel);
    layout->addWidget(searchResultTable);

    connect(goBtn, &QPushButton::clicked, this, &DBOperationsScreen::onSearchGoClicked);
    return panel;
}

QWidget* DBOperationsScreen::buildUpdatePanel() {
    QWidget *panel = new QWidget();
    panel->setStyleSheet("background-color: #0d1117;");
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(12);

    QLabel *title = new QLabel("Update a Record");
    title->setStyleSheet("color: #4d9fff; font-size: 16px; font-weight: bold;");

    QHBoxLayout *row = new QHBoxLayout();
    updateKeyInput = new QLineEdit();
    updateKeyInput->setPlaceholderText("Enter key to find record...");
    updateKeyInput->setFixedHeight(38);
    updateKeyInput->setStyleSheet(inputStyle());

    QPushButton *findBtn = new QPushButton("Find");
    findBtn->setFixedHeight(38);
    findBtn->setFixedWidth(90);
    findBtn->setStyleSheet(
        "QPushButton { background-color: #4d9fff; color: white; "
        "border-radius: 6px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #3a8eef; }"
        );
    row->addWidget(updateKeyInput);
    row->addWidget(findBtn);

    updateStatusLabel = new QLabel("");
    updateStatusLabel->setStyleSheet("color: #8b949e; font-size: 12px;");
    updateStatusLabel->setWordWrap(true);

    updateFieldsLayout = new QVBoxLayout();
    updateFieldsLayout->setSpacing(8);

    updateSaveBtn = new QPushButton("✔  Update Record");
    updateSaveBtn->setFixedHeight(42);
    updateSaveBtn->setVisible(false);
    updateSaveBtn->setStyleSheet(
        "QPushButton { background-color: #238636; color: white; "
        "border-radius: 8px; font-weight: bold; font-size: 13px; border: none; }"
        "QPushButton:hover { background-color: #2ea043; }"
        );

    layout->addWidget(title);
    layout->addLayout(row);
    layout->addWidget(updateStatusLabel);
    layout->addLayout(updateFieldsLayout);
    layout->addWidget(updateSaveBtn);
    layout->addStretch();

    connect(findBtn, &QPushButton::clicked, this, &DBOperationsScreen::onUpdateSearchClicked);
    connect(updateSaveBtn, &QPushButton::clicked, this, &DBOperationsScreen::onUpdateSaveClicked);
    return panel;
}

QWidget* DBOperationsScreen::buildDeletePanel() {
    QWidget *panel = new QWidget();
    panel->setStyleSheet("background-color: #0d1117;");
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(12);

    QLabel *title = new QLabel("Delete a Record");
    title->setStyleSheet("color: #e05c5c; font-size: 16px; font-weight: bold;");

    QHBoxLayout *row = new QHBoxLayout();
    deleteKeyInput = new QLineEdit();
    deleteKeyInput->setPlaceholderText("Enter key to delete...");
    deleteKeyInput->setFixedHeight(38);
    deleteKeyInput->setStyleSheet(inputStyle());

    QPushButton *delBtn = new QPushButton("Delete");
    delBtn->setFixedHeight(38);
    delBtn->setFixedWidth(90);
    delBtn->setStyleSheet(
        "QPushButton { background-color: #da3633; color: white; "
        "border-radius: 6px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #b91c1c; }"
        );
    row->addWidget(deleteKeyInput);
    row->addWidget(delBtn);

    deleteStatusLabel = new QLabel("");
    deleteStatusLabel->setStyleSheet("color: #8b949e; font-size: 12px;");

    layout->addWidget(title);
    layout->addLayout(row);
    layout->addWidget(deleteStatusLabel);
    layout->addStretch();

    connect(delBtn, &QPushButton::clicked, this, &DBOperationsScreen::onDeleteGoClicked);
    return panel;
}

void DBOperationsScreen::setupForDatabase(QString tableName, QVector<QString> columns, BPlusTree *treePtr, QString filePath) {
    currentTableName = tableName;
    currentColumns   = columns;
    tree             = treePtr;
    dbFilePath       = filePath;

    titleLabel->setText(tableName);

    QStringList headers;
    for (auto &c : columns) headers << c;

    viewAllTable->setColumnCount(columns.size());
    viewAllTable->setHorizontalHeaderLabels(headers);

    searchResultTable->setColumnCount(columns.size());
    searchResultTable->setHorizontalHeaderLabels(headers);

    searchKeyInput->clear();
    searchResultLabel->setText("");
    searchResultTable->setRowCount(0);

    updateKeyInput->clear();
    updateStatusLabel->setText("");
    updateSaveBtn->setVisible(false);
    clearUpdateFields();

    deleteKeyInput->clear();
    deleteStatusLabel->setText("");

    contentStack->setCurrentWidget(viewAllPanel);
    refreshViewAllTable();
}

void DBOperationsScreen::refreshViewAllTable() {
    viewAllTable->setRowCount(0);
    if (!tree) return;
    QVector<Record> all;
    tree->getAllRecords(all);
    for (auto &r : all) {
        int row = viewAllTable->rowCount();
        viewAllTable->insertRow(row);
        viewAllTable->setItem(row, 0, new QTableWidgetItem(QString::number(r.key)));
        for (int i = 0; i < r.values.size(); i++)
            viewAllTable->setItem(row, i + 1, new QTableWidgetItem(r.values[i]));
    }
}

void DBOperationsScreen::onViewAllClicked() {
    refreshViewAllTable();
    contentStack->setCurrentWidget(viewAllPanel);
}

void DBOperationsScreen::onSearchModeClicked() {
    contentStack->setCurrentWidget(searchPanel);
}

void DBOperationsScreen::onUpdateModeClicked() {
    contentStack->setCurrentWidget(updatePanel);
}

void DBOperationsScreen::onDeleteModeClicked() {
    contentStack->setCurrentWidget(deletePanel);
}

void DBOperationsScreen::onSearchGoClicked() {
    searchResultTable->setRowCount(0);
    bool ok;
    int key = searchKeyInput->text().trimmed().toInt(&ok);
    if (!ok) {
        searchResultLabel->setText("Please enter a valid numeric key.");
        searchResultLabel->setStyleSheet("color: #e05c5c; font-size: 12px;");
        return;
    }
    Record *r = tree->search(key);
    if (!r) {
        searchResultLabel->setText("No record found with key " + QString::number(key));
        searchResultLabel->setStyleSheet("color: #e05c5c; font-size: 12px;");
        return;
    }
    searchResultLabel->setText("✔ Record found:");
    searchResultLabel->setStyleSheet("color: #3fb950; font-size: 12px;");
    int row = searchResultTable->rowCount();
    searchResultTable->insertRow(row);
    searchResultTable->setItem(row, 0, new QTableWidgetItem(QString::number(r->key)));
    for (int i = 0; i < r->values.size(); i++)
        searchResultTable->setItem(row, i + 1, new QTableWidgetItem(r->values[i]));
}

void DBOperationsScreen::clearUpdateFields() {
    for (auto *field : updateFieldInputs) {
        updateFieldsLayout->removeWidget(field);
        delete field;
    }
    updateFieldInputs.clear();
}

void DBOperationsScreen::onUpdateSearchClicked() {
    clearUpdateFields();
    updateSaveBtn->setVisible(false);
    bool ok;
    int key = updateKeyInput->text().trimmed().toInt(&ok);
    if (!ok) {
        updateStatusLabel->setText("Please enter a valid numeric key.");
        updateStatusLabel->setStyleSheet("color: #e05c5c; font-size: 12px;");
        return;
    }
    Record *r = tree->search(key);
    if (!r) {
        updateStatusLabel->setText("No record found with key " + QString::number(key));
        updateStatusLabel->setStyleSheet("color: #e05c5c; font-size: 12px;");
        return;
    }
    updateCurrentKey = key;
    updateStatusLabel->setText("✔ Record found — edit fields below:");
    updateStatusLabel->setStyleSheet("color: #3fb950; font-size: 12px;");
    for (int i = 0; i < r->values.size(); i++) {
        QLineEdit *field = new QLineEdit(r->values[i]);
        field->setPlaceholderText(currentColumns[i + 1]);
        field->setFixedHeight(36);
        field->setStyleSheet(inputStyle());
        updateFieldsLayout->addWidget(field);
        updateFieldInputs.append(field);
    }
    updateSaveBtn->setVisible(true);
}

void DBOperationsScreen::onUpdateSaveClicked() {
    if (updateCurrentKey == -1) return;
    QVector<QString> newValues;
    for (auto *field : updateFieldInputs) {
        QString val = field->text().trimmed();
        if (val.isEmpty()) {
            updateStatusLabel->setText("All fields must be filled.");
            updateStatusLabel->setStyleSheet("color: #e05c5c; font-size: 12px;");
            return;
        }
        newValues.append(val);
    }
    tree->update(updateCurrentKey, newValues);
    tree->saveToFile(dbFilePath, currentColumns);
    updateStatusLabel->setText("✔ Record updated successfully.");
    updateStatusLabel->setStyleSheet("color: #3fb950; font-size: 12px;");
}

void DBOperationsScreen::onDeleteGoClicked() {
    bool ok;
    int key = deleteKeyInput->text().trimmed().toInt(&ok);
    if (!ok) {
        deleteStatusLabel->setText("Please enter a valid numeric key.");
        deleteStatusLabel->setStyleSheet("color: #e05c5c; font-size: 12px;");
        return;
    }
    Record *r = tree->search(key);
    if (!r) {
        deleteStatusLabel->setText("No record found with key " + QString::number(key));
        deleteStatusLabel->setStyleSheet("color: #e05c5c; font-size: 12px;");
        return;
    }
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Delete",
        "Delete record with key " + QString::number(key) + "?",
        QMessageBox::Yes | QMessageBox::No
        );
    if (confirm != QMessageBox::Yes) return;
    tree->remove(key);
    tree->saveToFile(dbFilePath, currentColumns);
    deleteStatusLabel->setText("✔ Record deleted successfully.");
    deleteStatusLabel->setStyleSheet("color: #3fb950; font-size: 12px;");
    deleteKeyInput->clear();
}

void DBOperationsScreen::onBackClicked() {
    if (tree)
        tree->saveToFile(dbFilePath, currentColumns);
    emit backToHomeClicked();
}