#include "TestManager.h"
#include "DatabaseDef.h"
#include <QtWidgets>
#include <QSettings>

extern QString confName;
extern DBPtr DB;

TestManager::TestManager( QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

TestManager::~TestManager( )
{

}
