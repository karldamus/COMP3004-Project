#ifndef USER_H
#define USER_H

#include "session.h"
#include <vector>
#include <string>

#include <iostream>

#include <QFile>
#include <QString>

// QJson
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>


class User {
    Q_GADGET;

    public:
        User();
        User(int userId);
        ~User();

        int getUserId() const;

        // sessions
        void loadSavedSessions();
        void unloadSavedSessions();
        void saveSession(); // activeSession is saved
        void loadSession(Session* session);
        void unloadSession();

        // QJson
        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

        // dev
        void test();

    private:
        int userId; // 1, 2, or 3
        std::vector<Session*> savedSessions;
        Session* activeSession;
};


#endif // USER_H