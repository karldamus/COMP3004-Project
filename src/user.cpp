#include "user.h"

using namespace std;

User::User() {

}

User::User(int userId) {
    this->userId = userId;
}

User::~User() {
    unloadSession();
    unloadSavedSessions(); 
}

int User::getUserId() const {
    return this->userId;
}

void User::loadSavedSessions() {
    // convert userId to string
    string userIdStr = to_string(userId);

    
}

void User::unloadSavedSessions() {
    for (int i = 0; i < savedSessions.size(); i++) {
        delete savedSessions[i];
    }

    savedSessions.clear();
}

void User::saveSession() {
    // if activeSession is not NULL, save it
    if (activeSession != NULL) {
        // get session data from activeSession
        Session::SessionType sessionType = activeSession->getSessionType();
        Session::SessionGroup sessionGroup = activeSession->getSessionGroup();
        int sessionIntensity = activeSession->getSessionIntensity();

        // convert sessionType to string
        string sessionTypeStr;

        switch (sessionType) {
            case Session::SessionType::DELTA:
                sessionTypeStr = "delta"; break;
            case Session::SessionType::ALPHA:
                sessionTypeStr = "alpha"; break;
            case Session::SessionType::BETA1:
                sessionTypeStr = "beta1"; break;
            case Session::SessionType::BETA2:
                sessionTypeStr = "beta2"; break;
        }

        // convert sessionGroup to string
        string sessionGroupStr;

        switch (sessionGroup) {
            case Session::SessionGroup::TWENTY_MINUTES:
                sessionGroupStr = "20min"; break;
            case Session::SessionGroup::FORTY_FIVE_MINUTES:
                sessionGroupStr = "45min"; break;
            case Session::SessionGroup::USER_DESIGNATED:
                sessionGroupStr = "userDesignated"; break;
        }

        // create json object
        QJsonObject sessionJson;

        sessionJson["sessionType"] = QString::fromStdString(sessionTypeStr);
        sessionJson["sessionGroup"] = QString::fromStdString(sessionGroupStr);
        sessionJson["sessionIntensity"] = sessionIntensity;

        // write json object to file
        write(sessionJson);

        // add session to savedSessions
        savedSessions.push_back(activeSession);
    } else {
        // activeSession is NULL
        // TODO: throw Q warning -- no active session to save
    }
}

void User::loadSession(Session *session) {
    // if activeSession is not NULL, delete it
    if (activeSession != NULL) {
        delete activeSession;
    }

    // set activeSession to session
    activeSession = session;
}

void User::unloadSession() {
    if (activeSession != NULL) {
        delete activeSession;
        activeSession = NULL;
    }
}

void User::read(const QJsonObject &json) {

}

void User::write(QJsonObject &json) const {
    // read the user.json file to get the existing data
    QFile userFile("Resources/user.json");

    if (!userFile.exists()) {
        qWarning("user.json does not exist");
    }

    if (!userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open user.json");
        return;
    }

    QJsonDocument userDoc = QJsonDocument::fromJson(userFile.readAll());

    cout << userDoc.toJson().toStdString() << endl;

    userFile.close();

    // // read the user.json file to get the existing data
    // QFile userFile("user.json");

    // if (!userFile.open(QIODevice::ReadOnly)) {
    //     qWarning("Couldn't open user.json");
    //     return;
    // }

    // QJsonDocument userDoc(userFile.readAll());

    // userFile.close();

    // // convert userId to string
    // string userIdStr = to_string(userId);

    // // add json object to userJson["userId"]["savedSessions"]
    // QJsonObject userJson = userDoc.object();
    // QJsonObject userIdJson = userJson[QString::fromStdString(userIdStr)].toObject();
    // QJsonArray savedSessionsJson = userIdJson["savedSessions"].toArray();



    // // write json object to user.json
    // if (!userFile.open(QIODevice::WriteOnly)) {
    //     qWarning("Couldn't open user.json");
    //     return;
    // }

    // userFile.write(QJsonDocument(userJson).toJson());

    // userFile.close();
}

void User::test() {
    // create new QJsonObject with filler data
    QJsonObject sessionJson;

    sessionJson["sessionType"] = "alpha";
    sessionJson["sessionGroup"] = "20min";
    sessionJson["sessionIntensity"] = 5;

    // write json object to file
    write(sessionJson);
}