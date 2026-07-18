#ifndef INSERTSCREEN_H
#define INSERTSCREEN_H

#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include "BPlusTree.h"

class InsertScreen : public QWidget {
    Q_OBJECT

public:
    explicit InsertScreen(QWidget *parent = nullptr);
    void setupForTable(QString tableName, QVector<QString> columns, BPlusTree *tree, QString filePath);

signals:
    void finishedInserting();

private slots:
    void onInsertClicked();
    void onSaveClicked();

private:
    QString currentTableName;
    QVector<QString> currentColumns;
    BPlusTree *tree;
    QString dbFilePath;

    QLabel *titleLabel;
    QVector<QLineEdit*> fieldInputs;
    QVBoxLayout *fieldsLayout;
    QPushButton *insertBtn;
    QPushButton *saveBtn;
    QLabel *statusLabel;
    QLabel *countLabel;
    QTableWidget *previewTable;

    void setupUI();
    void buildInputFields();
    void refreshPreview();
    void updateCountLabel();
};

#endif