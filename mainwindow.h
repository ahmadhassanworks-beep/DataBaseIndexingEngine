#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "HomeScreen.h"
#include "CreateDBScreen.h"
#include "InsertScreen.h"
#include "ViewDBScreen.h"
#include "DBOperationsScreen.h"
#include "BPlusTree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showCreateDatabase();
    void showViewDatabases();
    void handleTableCreated(QString tableName, QVector<QString> columnNames);
    void handleBackToHome();
    void handleFinishedInserting();
    void handleDatabaseSelected(QString tableName);
    void handleOperationsBackToHome();

private:
    QStackedWidget *stack;
    HomeScreen *homeScreen;
    CreateDBScreen *createDBScreen;
    InsertScreen *insertScreen;
    ViewDBScreen *viewDBScreen;
    DBOperationsScreen *dbOperationsScreen;

    QString currentDBPath;
    QVector<QString> currentColumns;
    BPlusTree *currentTree;

    void setupScreens();
    bool databaseExists(const QString &tableName);
};

#endif