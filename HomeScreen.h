#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class HomeScreen : public QWidget {
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = nullptr);

signals:
    void createDatabaseClicked();
    void viewDatabasesClicked();

private:
    QPushButton *createBtn;
    QPushButton *viewBtn;
    QLabel *titleLabel;

    void setupUI();
};

#endif