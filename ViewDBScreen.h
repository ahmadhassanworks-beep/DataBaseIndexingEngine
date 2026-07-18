#ifndef VIEWDBSCREEN_H
#define VIEWDBSCREEN_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ViewDBScreen : public QWidget {
    Q_OBJECT

public:
    explicit ViewDBScreen(QWidget *parent = nullptr);
    void refreshList();

signals:
    void backToHomeClicked();
    void databaseSelected(QString tableName);

private slots:
    void onItemDoubleClicked(QListWidgetItem *item);
    void onOpenClicked();
    void onBackClicked();

private:
    QListWidget *dbListWidget;
    QPushButton *openBtn;
    QPushButton *backBtn;
    QLabel *titleLabel;
    QLabel *emptyLabel;

    void setupUI();
};

#endif