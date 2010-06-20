#ifndef SQL_H
#define SQL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "server.h"
/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/

class SQLCreator {
public:
    static void createSQLConnection(const QString &name = QString())
    {
        mutex.lock();
        QSettings s;
        databaseType = s.value("sql_driver").toInt();

        QString driver = databaseType == SQLite ? "QSQLITE":"QPSQL";

        QSqlDatabase db;
        if (name == "")
            db = QSqlDatabase::addDatabase(driver);
        else
            db = QSqlDatabase::addDatabase(driver, name);

        db.setDatabaseName(s.value("sql_db_name").toString());

        if (databaseType == PostGreSQL) {
            db.setHostName(s.value("sql_db_host").toString());
            db.setPort(s.value("sql_db_port").toInt());
            db.setUserName(s.value("sql_db_user").toString());
            db.setPassword(s.value("sql_db_pass").toString());
        }

        if (!db.open() && name=="") {
            throw (QString("Unable to establish a database connection.") + db.lastError().text());
        } else if (name == "") {
            throw (QString("Connection to the database established!"));
        }

        /* Cleans the database on restart */
        if (name == "")
            db.exec("vacuum");
        mutex.unlock();
    }

    enum DataBaseType  {
        SQLite,
        PostGreSQL
    };

    static int databaseType;
    static QMutex mutex;
};

#endif // SQL_H