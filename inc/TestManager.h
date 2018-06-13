#pragma once

#include <QMainWindow>
#include "ui_TestManager.h"

class TestManager : public QMainWindow
{
    Q_OBJECT

public:
    TestManager( QWidget *parent = 0 );
    ~TestManager( );


private:
    Ui::TestManager ui;
};
