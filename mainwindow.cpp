#include "mainwindow.h"
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    currentTree = nullptr;
    setupScreens();
}

MainWindow::~MainWindow() {}

void MainWindow::setupScreens() {
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    homeScreen = new HomeScreen();
    createDBScreen = new CreateDBScreen();
    insertScreen = new InsertScreen();
    viewDBScreen = new ViewDBScreen();
    dbOperationsScreen = new DBOperationsScreen();

    stack->addWidget(homeScreen);
    stack->addWidget(createDBScreen);
    stack->addWidget(insertScreen);
    stack->addWidget(viewDBScreen);
    stack->addWidget(dbOperationsScreen);

    connect(homeScreen, &HomeScreen::createDatabaseClicked, this, &MainWindow::showCreateDatabase);
    connect(homeScreen, &HomeScreen::viewDatabasesClicked, this, &MainWindow::showViewDatabases);

    connect(createDBScreen, &CreateDBScreen::tableCreated, this, &MainWindow::handleTableCreated);
    connect(createDBScreen, &CreateDBScreen::backToHomeClicked, this, &MainWindow::handleBackToHome);

    connect(insertScreen, &InsertScreen::finishedInserting, this, &MainWindow::handleFinishedInserting);

    connect(viewDBScreen, &ViewDBScreen::backToHomeClicked, this, &MainWindow::handleBackToHome);
    connect(viewDBScreen, &ViewDBScreen::databaseSelected, this, &MainWindow::handleDatabaseSelected);

    connect(dbOperationsScreen, &DBOperationsScreen::backToHomeClicked, this, &MainWindow::handleOperationsBackToHome);

    stack->setCurrentWidget(homeScreen);

    setWindowTitle("Database Indexing Engine");
    resize(1000, 650);
}

void MainWindow::showCreateDatabase() {
    createDBScreen->resetScreen();
    stack->setCurrentWidget(createDBScreen);
}

void MainWindow::showViewDatabases() {
    viewDBScreen->refreshList();
    stack->setCurrentWidget(viewDBScreen);
}

bool MainWindow::databaseExists(const QString &tableName) {
    QDir dir("databases/" + tableName);
    return dir.exists();
}

void MainWindow::handleTableCreated(QString tableName, QVector<QString> columnNames) {
    if (databaseExists(tableName)) {
        QMessageBox::warning(this, "Name Taken",
                             "A database named \"" + tableName + "\" already exists. Please choose a different name.");
        return;
    }

    QDir dir;
    dir.mkpath("databases/" + tableName);

    currentDBPath = "databases/" + tableName + "/records.dat";
    currentColumns = columnNames;

    if (currentTree) {
        delete currentTree;
    }
    currentTree = new BPlusTree();
    currentTree->saveToFile(currentDBPath, currentColumns);

    insertScreen->setupForTable(tableName, currentColumns, currentTree, currentDBPath);
    stack->setCurrentWidget(insertScreen);
}

void MainWindow::handleBackToHome() {
    stack->setCurrentWidget(homeScreen);
}

void MainWindow::handleFinishedInserting() {
    stack->setCurrentWidget(homeScreen);
}

void MainWindow::handleDatabaseSelected(QString tableName) {
    QString filePath = "databases/" + tableName + "/records.dat";

    if (currentTree) {
        delete currentTree;
    }
    currentTree = new BPlusTree();

    QVector<QString> columns;
    bool loaded = currentTree->loadFromFile(filePath, columns);

    if (!loaded) {
        QMessageBox::warning(this, "Load Failed",
                             "Could not load database \"" + tableName + "\". The file may be missing or corrupted.");
        return;
    }

    currentColumns = columns;
    currentDBPath = filePath;

    dbOperationsScreen->setupForDatabase(tableName, currentColumns, currentTree, currentDBPath);
    stack->setCurrentWidget(dbOperationsScreen);
}

void MainWindow::handleOperationsBackToHome() {
    stack->setCurrentWidget(homeScreen);
}