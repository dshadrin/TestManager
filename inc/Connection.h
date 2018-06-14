#pragma once

//////////////////////////////////////////////////////////////////////////
#include <QDialog>
#include "ui_Connection.h"
#include <QSettings>
//////////////////////////////////////////////////////////////////////////

extern const QString CONF_NAME;

class VersionModel;
class QSqlQuery;

class Connection : public QDialog
{
    Q_OBJECT

public:
    Connection(QWidget *parent = Q_NULLPTR);
    ~Connection() = default;

    static void SetRevision(int rev);
    static QString RevisionDescription();
    static void SetRevisionDescription(const QString& desc);
    static int SuiteDefaultTimeout();

protected slots:
    void accept() override;
    void urlEdit(const QString& str);
    void dbNameEdit(const QString& str);
    void loginEdit(const QString& str);
    void passEdit(const QString& str);
    void sqliteNameEdit(const QString& str);
    void testConnection();

private:
    void readSettings();

private:
    Ui::Connection ui;
    QString url;
    QString dbName;
    QString login;
    QString pass;
    QString sqliteName;
    QSettings settings;
};

#define CHECK_DB(db) if (!db.isOpen()) { db.open(); }

//////////////////////////////////////////////////////////////////////////
