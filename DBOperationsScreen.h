#ifndef DBOPERATIONSSCREEN_H
#define DBOPERATIONSSCREEN_H

#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "BPlusTree.h"

class DBOperationsScreen : public QWidget {
    Q_OBJECT

public:
    explicit DBOperationsScreen(QWidget *parent = nullptr);
    void setupForDatabase(QString tableName, QVector<QString> columns, BPlusTree *tree, QString filePath);

signals:
    void backToHomeClicked();

private slots:
    void onViewAllClicked();
    void onSearchModeClicked();
    void onUpdateModeClicked();
    void onDeleteModeClicked();
    void onSearchGoClicked();
    void onUpdateSearchClicked();
    void onUpdateSaveClicked();
    void onDeleteGoClicked();
    void onBackClicked();

private:
    QString currentTableName;
    QVector<QString> currentColumns;
    BPlusTree *tree;
    QString dbFilePath;

    // Left sidebar
    QLabel *titleLabel;
    QPushButton *viewAllBtn;
    QPushButton *searchBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *backBtn;

    // Right content stack
    QStackedWidget *contentStack;

    // View All panel
    QWidget *viewAllPanel;
    QTableWidget *viewAllTable;

    // Search panel
    QWidget *searchPanel;
    QLineEdit *searchKeyInput;
    QLabel *searchResultLabel;
    QTableWidget *searchResultTable;

    // Update panel
    QWidget *updatePanel;
    QLineEdit *updateKeyInput;
    QLabel *updateStatusLabel;
    QVBoxLayout *updateFieldsLayout;
    QVector<QLineEdit*> updateFieldInputs;
    QPushButton *updateSaveBtn;
    int updateCurrentKey;

    // Delete panel
    QWidget *deletePanel;
    QLineEdit *deleteKeyInput;
    QLabel *deleteStatusLabel;

    void setupUI();
    QWidget* buildViewAllPanel();
    QWidget* buildSearchPanel();
    QWidget* buildUpdatePanel();
    QWidget* buildDeletePanel();
    void refreshViewAllTable();
    void clearUpdateFields();
    QString inputStyle();
    QString tableStyle();
};

#endif