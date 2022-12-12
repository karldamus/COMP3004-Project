#include "user.h"

using namespace std;

User::User(int userId) : activeSession(NULL) {
    this->userId = userId;

    QFileInfo userFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation), "user.json");
    if (!userFile.exists()) {
        createNewUserFile();
    }
    loadSavedSessions();
}

User::~User() {
    unloadSession();
    unloadSavedSessions(); 
}

int User::getUserId() const { return this->userId; }
const QVector<Session*>* User::getSavedSessions() const { return &savedSessions; }

void User::loadSavedSessions() {
    // convert userId to a QString
    QString userIdStr = QString::number(userId);

    // get the savesSessionsArray for the current user
    QJsonObject existingJson = read();
    QJsonObject userJson = existingJson[userIdStr].toObject();
    QJsonArray savedSessionsArray = userJson["savedSessions"].toArray();

    // iterate through savedSessionsArray and create sessions
    for (int i = 0; i < savedSessionsArray.size(); i++) {
        QJsonObject sessionJson = savedSessionsArray[i].toObject();

        // create session
        Session* session = new Session(sessionJson);

        // add session to savedSessions
        savedSessions.append(session);
    }
}

void User::unloadSavedSessions() {
    for (int i = 0; i < (int)savedSessions.size(); i++) {
        delete savedSessions[i];
    }

    savedSessions.clear();
}

void User::saveSession() {
    // if activeSession is not NULL, save it
    if (activeSession != NULL) {
        // create json object
        QJsonObject sessionJson = activeSession->toJson();

        // valid data check
        if (!isValidData(sessionJson)) {
            qFatal("Invalid data in sessionJson");
            return;
        }

        // write session to file
        write(sessionJson);

        // add session to savedSessions
        savedSessions.append(activeSession);
        if (savedSessions.size() > MAX_SESSIONS) {
            savedSessions.removeFirst();
        }
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
    // locate the app data location
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (path.isEmpty()) {
        qFatal("Couldn't find writable location for user.json");
    }

    QDir dir(path); // init path
    if (dir.mkpath(dir.absolutePath()) && QDir::setCurrent(dir.absolutePath())) {
        cout << "user.json  in " << QDir::currentPath().toStdString() << endl;

        QFile userFile("user.json");

        // if file exists, read it
        if (userFile.exists()) {
            if (userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QJsonParseError parseError;
                QJsonDocument userDoc = QJsonDocument::fromJson(userFile.readAll(), &parseError);

                if (parseError.error == QJsonParseError::NoError) {
                    return validateUserDoc(userDoc);
                } else {
                    // parse error with file
                    qWarning("user.json parse error");
                    return QJsonObject();
                }
            } else {
                // couldn't open file
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

QJsonObject User::validateUserDoc(QJsonDocument &userDoc) {
    if (userDoc.isObject()) {
        QJsonObject userJson = userDoc.object();
        
        // conver userJson to a string
        QString userJsonStr = QString::fromUtf8(QJsonDocument(userJson).toJson(QJsonDocument::Compact));

        // output userJsonStr
        cout << "user.json: " << userJsonStr.toStdString() << endl;

        return userJson;
    } else {
        qWarning("user.json is not an object");
        return QJsonObject();
    }
}


void User::write(QJsonObject &json) {
    if (!isValidData(json)) {
        qWarning("Invalid data passed to User::write");
        return;
    }

    // get existing json data
    QJsonObject existingJson = read();

    // locate the app data location
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (path.isEmpty()) {
        qFatal("Couldn't find writable location for user.json");
        return;
    }

    QDir dir(path); // init path
    if (dir.mkpath(dir.absolutePath()) && QDir::setCurrent(dir.absolutePath())) {
        QFile userFile("user.json");

        if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QString userIdStr = QString::number(userId);

            // getters
            // current user data
            QJsonObject userJson = existingJson[userIdStr].toObject();
            // current users savesSessions array
            QJsonArray savedSessionsArray = userJson["savedSessions"].toArray();

            // setters/adders
            // new session -> savedSessions array
            savedSessionsArray.push_back(json);
            if (savedSessionsArray.size() > MAX_SESSIONS) {
                savedSessionsArray.removeFirst();
            }
            // replace savedSessions array with modified array
            userJson["savedSessions"] = savedSessionsArray;
            // replace userJson with modified userJson
            existingJson[userIdStr] = userJson;

            // write the modified json to the file
            userFile.write(QJsonDocument(existingJson).toJson());
            userFile.close();
        } else {
            qWarning("Couldn't open user.json. Creating new user.json file and writing data to it");
            createNewUserFile();
            write(json);
        }
    }
}

void User::createNewUserFile() {
    // locate the app data location
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (path.isEmpty()) {
        qFatal("Couldn't find writable location for user.json");
        return;
    }

    // create new userJson object
    QJsonObject userJson = createEmptyUserJson();

    QDir dir(path);

    if (dir.mkpath(dir.absolutePath()) && QDir::setCurrent(dir.absolutePath())) {
        QFile userFile("user.json");

        if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonDocument userDoc(userJson);
            userFile.write(userDoc.toJson());
            userFile.close();
        } else {
            qFatal("Couldn't create user.json");
        }
    }  
}

QJsonObject User::createEmptyUserJson() {
    QJsonObject userJson;

    for (int i = 1; i <= 3; i++) {
        QJsonObject userJsonSingleUser;
        userJsonSingleUser["userId"] = i;
        userJsonSingleUser["savedSessions"] = QJsonArray();

        userJson[QString::number(i)] = userJsonSingleUser;
    }

    return userJson;
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

void User::test() {
    QJsonObject userJson = read();
    cout << "userJson (test()): " << QJsonDocument(userJson).toJson().toStdString() << endl;

    cout << "loadSavedSessions(): " << endl;
    loadSavedSessions();

    cout << "Iterating over savedSessions: " << endl;

    for (auto session : savedSessions) {
        cout << *session << endl;
    }
}
