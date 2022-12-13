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
    unloadSavedSessions(); 
}

int User::getUserId() const { return this->userId; }
Session* User::getActiveSession() const { return this->activeSession; }
const QVector<Session*>* User::getSavedSessions() const { return &savedSessions; }

void User::setActiveSession(Session* s) { this->activeSession = s; }

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
            qWarning("Invalid data in sessionJson");
            return;
        }

        // add session to savedSessions
        savedSessions.append(activeSession);
        if (savedSessions.size() > MAX_SESSIONS) {
            delete savedSessions[0];
            savedSessions.removeFirst();
        }

        // write session to file
        write();
    } else {
        // activeSession is NULL
        // TODO: throw Q warning -- no active session to save
    }
}

void User::loadSession(Session *session) {
    // set activeSession to session
    // make sure saveSession is called after calling loadSession
    activeSession = new Session(session);
}

void User::unloadSession() {
    activeSession = NULL;
}

QJsonObject User::read() {
    // locate the app data location
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (path.isEmpty()) {
        qFatal("Couldn't find writable location for user.json");
    }

    QDir dir(path); // init path
    if (dir.mkpath(dir.absolutePath()) && QDir::setCurrent(dir.absolutePath())) {
        // cout << "user.json  in " << QDir::currentPath().toStdString() << endl;

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
        return userJson;
    } else {
        qWarning("user.json is not an object");
        return QJsonObject();
    }
}


void User::write() {
    // all data in the savedSessions vector should be valid, no need to double check

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

            // get current user data
            QJsonObject userJson = existingJson[userIdStr].toObject();

            // replace savedSessions array with local savedSessions array
            userJson["savedSessions"] = convertToJsonArray();

            // replace userJson with modified userJson
            existingJson[userIdStr] = userJson;

            // write the modified json to the file
            userFile.write(QJsonDocument(existingJson).toJson());
            userFile.close();
        } else {
            qWarning("Couldn't open user.json. Creating new user.json file and writing data to it");
            createNewUserFile();
            write();
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

QJsonArray User::convertToJsonArray() {
    QJsonArray jsonArray;
    for (int i = 0; i < savedSessions.size(); ++i) {
        jsonArray.append(savedSessions[i]->toJson());
    }
    return jsonArray;
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
