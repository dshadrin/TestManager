#pragma once

#include <QMainWindow>
#include "ui_TestManager.h"

class QTimer;

class TestManager : public QMainWindow
{
    Q_OBJECT

public:
    TestManager( QWidget *parent = 0 );
    ~TestManager( );

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void writeSettings();
    void readSettings();

private slots:
    void dbConnection();
    void tickDbTimer();

private:
    Ui::TestManager ui;
    QTimer* checkDbTimer;
};
