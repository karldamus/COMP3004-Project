#ifndef USER_H
#define USER_H

#include "session.h"
#include <string>

#include <iostream>

#include <QFile>
#include <QString>
#include <QVector>

// QJson
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QStandardPaths>
#include <QDir>


class User {
    Q_GADGET;

    public:
        User(int userId=1);
        ~User();

        int getUserId() const;

        // file io
        void createNewUserFile();

        // sessions
        void loadSavedSessions();
        void unloadSavedSessions();
        void saveSession(); // activeSession is saved
        void loadSession(Session* session);
        void unloadSession();
        bool isValidData(QJsonObject json) const;

        // QJson
        QJsonObject read();
        void write(QJsonObject &json);

        // helpers
        QJsonObject validateUserDoc(QJsonDocument &userDoc);
        QJsonObject createEmptyUserJson();

        // dev
        void test();

    private:
        int userId; // 1, 2, or 3
        QVector<Session*> savedSessions;
        Session* activeSession;
};


#endif // USER_H
