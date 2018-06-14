#include "TestManager.h"
#include "DatabaseDef.h"
#include "Connection.h"
#include <QtWidgets>
#include <QSettings>
#include <QTimer>

//////////////////////////////////////////////////////////////////////////
extern QString confName;
DBPtr DB;

//////////////////////////////////////////////////////////////////////////
TestManager::TestManager( QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    readSettings();
    connect(ui.actionConnection, &QAction::triggered, this, &TestManager::dbConnection);

    DB = Database::create(EDBType::PostgreSQL);
    if (DB->status() != Database::EDbConnStatus::ok)
    {
        DB.reset();
        checkDbTimer = new QTimer(this);
        checkDbTimer->setInterval(300);
        connect(checkDbTimer, SIGNAL(timeout()), this, SLOT(tickDbTimer()));
        checkDbTimer->start();
    }
    else
    {
        DB->checkDbStructure();
    }
}

//////////////////////////////////////////////////////////////////////////
TestManager::~TestManager( )
{

}

//////////////////////////////////////////////////////////////////////////
void TestManager::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

//////////////////////////////////////////////////////////////////////////
void TestManager::writeSettings()
{
    QSettings settings(confName, QSettings::IniFormat, Q_NULLPTR);
    settings.setValue("geometry", saveGeometry());
}

//////////////////////////////////////////////////////////////////////////
void TestManager::readSettings()
{
    QSettings settings(confName, QSettings::IniFormat, Q_NULLPTR);
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
            (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
}

//////////////////////////////////////////////////////////////////////////
void TestManager::dbConnection()
{
    QScopedPointer<Connection> opDialog(new Connection(Q_NULLPTR));
    if (opDialog)
    {
        opDialog->exec();
    }
}

//////////////////////////////////////////////////////////////////////////
void TestManager::tickDbTimer()
{
    checkDbTimer->stop();
    dbConnection();
    DB = Database::create(EDBType::PostgreSQL);
    if (DB->status() != Database::EDbConnStatus::ok)
    {
        this->close();
    }
    else
    {
        DB->checkDbStructure();
    }
}
