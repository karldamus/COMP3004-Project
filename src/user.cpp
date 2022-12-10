#include "user.h"

using namespace std;

User::User() : activeSession(NULL) {

}

User::User(int userId) : activeSession(NULL) {
    this->userId = userId;

    createNewUserFile();
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

QJsonObject User::read() {
    // cout << "Attempting to read user.json..." << endl;

    // read the user.json file from the app data location
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (path.isEmpty()) {
        qFatal("Couldn't find writable location for user.json");
    }

    QDir dir(path);

    if (dir.mkpath(dir.absolutePath()) && QDir::setCurrent(dir.absolutePath())) {
        // cout << "user.json  in " << QDir::currentPath().toStdString() << endl;

        QFile userFile("user.json");

        // if file exists, read it
        if (userFile.exists()) {
            if (userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QJsonParseError parseError;
                QJsonDocument userDoc = QJsonDocument::fromJson(userFile.readAll(), &parseError);

                if (parseError.error == QJsonParseError::NoError) {
                    if (userDoc.isObject()) {
                        QJsonObject userJson = userDoc.object();

                        return userJson;
                    } else {
                        qWarning("user.json is not an object");
                        return QJsonObject();
                    }
                } else {
                    qWarning("user.json parse error");
                    return QJsonObject();
                }
            } else {
                qWarning("Couldn't open user.json");
                return QJsonObject();
            }
        } else {
            // file doesn't exist, create it
            createNewUserFile();
            // now read it
            return read();
        }
    }
}

void User::createNewUserFile() {
    // cout << "Attempting to create new user.json file..." << endl;

    // if user.json exists, return without doing anything
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (path.isEmpty()) {
        qFatal("Couldn't find writable location for user.json");
    }

    // create new userJson object
    QJsonObject userJson;

    for (int i = 1; i <= 3; i++) {
        QJsonObject userJsonSingleUser;
        userJsonSingleUser["userId"] = i;
        userJsonSingleUser["savedSessions"] = QJsonArray();

        userJson[QString::number(i)] = userJsonSingleUser;
    }

    QDir dir(path);

    if (dir.mkpath(dir.absolutePath()) && QDir::setCurrent(dir.absolutePath())) {
        QFile userFile("user.json");

        if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonDocument userDoc(userJson);
            userFile.write(userDoc.toJson());
            userFile.close();
        }
    }  
}

void User::write(QJsonObject &json) {
    // cout << "Attempting to write to user.json..." << endl;

    if (!isValidData(json)) {
        qWarning("Invalid data passed to User::write");
        return;
    }

    // get existing json data
    QJsonObject existingJson = read();

    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    
    if (path.isEmpty()) {
        qFatal("Couldn't find writable location for user.json");
    }



    QDir dir(path);

    if (dir.mkpath(dir.absolutePath()) && QDir::setCurrent(dir.absolutePath())) {
        cout << "user.json  in " << QDir::currentPath().toStdString() << endl;

        QFile userFile("user.json");

        if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // get the user id as a string
            QString userIdStr = QString::number(userId);

            // get the user json object
            QJsonObject userJson = existingJson[userIdStr].toObject();

            // get the saved sessions array
            QJsonArray savedSessionsArray = userJson["savedSessions"].toArray();

            // create new object from json
            QJsonObject newSessionJson;
            newSessionJson["sessionType"] = json["sessionType"];
            newSessionJson["sessionGroup"] = json["sessionGroup"];
            newSessionJson["sessionIntensity"] = json["sessionIntensity"];

            // add new session to saved sessions array
            savedSessionsArray.push_back(newSessionJson);

            // add saved sessions array to user json object
            userJson["savedSessions"] = savedSessionsArray;

            // add user json object to existing json
            existingJson[userIdStr] = userJson;

            // write the json object to user.json
            userFile.write(QJsonDocument(existingJson).toJson());
            userFile.close();
        } else {
            qWarning("Couldn't open user.json. Creating new user.json file and writing data to it");
            createNewUserFile();
            write(json);
        }
    }
}

bool User::isValidData(QJsonObject json) const {
    // check if json is empty
    if (json.isEmpty()) {
        return false;
    }

    // check if json has the correct keys
    if (!json.contains("sessionType") || !json.contains("sessionGroup") || !json.contains("sessionIntensity")) {
        return false;
    }

    // check if json values are valid
    if (json["sessionType"].toString().isEmpty() || json["sessionGroup"].toString().isEmpty() || json["sessionIntensity"].toInt() == 0) {
        return false;
    }

    return true;
}

/**
void User::write(QJsonObject &json) const {
    // // read the user.json file to get the existing data
    // QFile userFile(":/user.json");

    // if (!userFile.exists()) {
    //     qWarning("user.json does not exist");
    // }

    // if (!userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     qWarning("Couldn't open user.json :: read");
    //     return;
    // }

    // QJsonDocument userDoc = QJsonDocument::fromJson(userFile.readAll());

    // cout << userDoc.toJson().toStdString() << endl;

    // userFile.close();

    // // convert userId to string
    // string userIdStr = to_string(userId);

    // // add json object to userJson["userId"]["savedSessions"]
    // QJsonObject userJson = userDoc.object();
    // QJsonObject userIdJson = userJson[QString::fromStdString(userIdStr)].toObject();
    // QJsonArray savedSessionsJson = userIdJson["savedSessions"].toArray();

    // savedSessionsJson.append(json);

    // userIdJson["savedSessions"] = savedSessionsJson;

    // userJson[QString::fromStdString(userIdStr)] = userIdJson;

    // // create new user.json file
    // QFile newUserFile(":/user.json");

    // if (!newUserFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //     qWarning("Couldn't open user.json :: write");
    //     return;
    // }

    // newUserFile.close();
}
*/

void User::test() {
    // create new QJsonObject with filler data
    QJsonObject sessionJson;

    sessionJson["sessionType"] = "alpha";
    sessionJson["sessionGroup"] = "20min";
    sessionJson["sessionIntensity"] = 5;

    // write json object to file
    write(sessionJson);
    QJsonObject userJson = read();

    cout << "userJson (test()): " << QJsonDocument(userJson).toJson().toStdString() << endl;
}