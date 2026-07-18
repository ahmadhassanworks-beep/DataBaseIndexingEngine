#ifndef CREATEDBSCREEN_H
#define CREATEDBSCREEN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QVector>
#include <QTableWidget>
#include <QHeaderView>

class CreateDBScreen : public QWidget {
    Q_OBJECT

public:
    explicit CreateDBScreen(QWidget *parent = nullptr);
    void resetScreen();

signals:
    void backToHomeClicked();
    void tableCreated(QString tableName, QVector<QString> columnNames);

private slots:
    void onSetColumnsClicked();
    void onSaveClicked();
    void onBackClicked();

private:
    QLineEdit *tableNameInput;
    QSpinBox *columnCountInput;
    QPushButton *setColumnsBtn;
    QPushButton *saveBtn;
    QPushButton *backBtn;
    QLabel *statusLabel;
    QLabel *keyHintLabel;
    QTableWidget *previewTable;

    QVBoxLayout *mainLayout;
    QVBoxLayout *columnFieldsLayout;
    QVector<QLineEdit*> columnInputs;

    void setupUI();
    void generateColumnFields(int count);
    void clearColumnFields();
};

#endif