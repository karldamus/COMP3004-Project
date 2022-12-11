#include "user.h"

using namespace std;

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
        Session* session = createSessionFromJson(sessionJson);

        // add session to savedSessions
        savedSessions.push_back(session);
    }
}

Session* User::createSessionFromJson(QJsonObject sessionJson) {
    Session::SessionType sessionType = convertStringToSessionType(sessionJson.value("sessionType").toString().toStdString());
    Session::SessionGroup sessionGroup = convertStringToSessionGroup(sessionJson.value("sessionGroup").toString().toStdString());
    int sessionIntensity = sessionJson["sessionIntensity"].toInt();

    // create session
    Session* session = new Session(sessionType, sessionGroup, sessionIntensity);

    return session;
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
        // get session data from activeSession
        Session::SessionType sessionType = activeSession->getSessionType();
        Session::SessionGroup sessionGroup = activeSession->getSessionGroup();
        int sessionIntensity = activeSession->getSessionIntensity();

        // create json object
        QJsonObject sessionJson = createNewSessionJson(sessionType, sessionGroup, sessionIntensity);

        // TODO: add valid data check
        if (!isValidData(sessionJson)) {
            qFatal("Invalid data in sessionJson");
            return;
        }

        // write session to file
        write(sessionJson);

        // add session to savedSessions
        savedSessions.push_back(activeSession);
    } else {
        // activeSession is NULL
        // TODO: throw Q warning -- no active session to save
    }
}

QJsonObject User::createNewSessionJson(Session::SessionType sessionType, Session::SessionGroup sessionGroup, int sessionIntensity) {
    // convert session type and group to strings
    QString sessionTypeStr = convertSessionTypeToQString(sessionType);
    QString sessionGroupStr = convertSessionGroupToQString(sessionGroup);

    // create json object
    QJsonObject sessionJson;

    sessionJson["sessionType"] = sessionTypeStr;
    sessionJson["sessionGroup"] = sessionGroupStr;
    sessionJson["sessionIntensity"] = sessionIntensity;

    return sessionJson;
}

QString User::convertSessionGroupToQString(Session::SessionGroup sessionGroup) {
    switch (sessionGroup) {
        case Session::SessionGroup::TWENTY_MINUTES:
            return QString::fromStdString("20min");
        case Session::SessionGroup::FORTY_FIVE_MINUTES:
            return QString::fromStdString("45min");
        case Session::SessionGroup::USER_DESIGNATED:
            return QString::fromStdString("userDesignated");
    }
}

QString User::convertSessionTypeToQString(Session::SessionType sessionType) {
    switch (sessionType) {
        case Session::SessionType::DELTA:
            return QString::fromStdString("delta");
        case Session::SessionType::ALPHA:
            return QString::fromStdString("alpha");
        case Session::SessionType::BETA1:
            return QString::fromStdString("beta1");
        case Session::SessionType::BETA2:
            return QString::fromStdString("beta2");
    }
}

Session::SessionGroup User::convertStringToSessionGroup(string sessionGroupStr) {
    if (sessionGroupStr == "20min") {
        return Session::SessionGroup::TWENTY_MINUTES;
    } else if (sessionGroupStr == "45min") {
        return Session::SessionGroup::FORTY_FIVE_MINUTES;
    } else if (sessionGroupStr == "userDesignated") {
        return Session::SessionGroup::USER_DESIGNATED;
    } else {
        cout << "Received sessionGroupStr: " << sessionGroupStr << endl;
        qFatal("Invalid sessionGroupStr");
        return Session::SessionGroup::NULL_SESSION_GROUP;
    }
}

Session::SessionType User::convertStringToSessionType(string sessionTypeStr) {
    if (sessionTypeStr == "delta") {
        return Session::SessionType::DELTA;
    } else if (sessionTypeStr == "alpha") {
        return Session::SessionType::ALPHA;
    } else if (sessionTypeStr == "beta1") {
        return Session::SessionType::BETA1;
    } else if (sessionTypeStr == "beta2") {
        return Session::SessionType::BETA2;
    } else {
        cout << "Received sessionTypeStr: " << sessionTypeStr << endl;
        qFatal("Invalid sessionTypeStr");
        return Session::SessionType::NULL_SESSION_TYPE;
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
    // create new QJsonObject with filler data
    QJsonObject sessionJson;

    sessionJson["sessionType"] = "alpha";
    sessionJson["sessionGroup"] = "45min";
    sessionJson["sessionIntensity"] = 5;

    // write json object to file
    write(sessionJson);

    QJsonObject userJson = read();
    cout << "userJson (test()): " << QJsonDocument(userJson).toJson().toStdString() << endl;

    sessionJson["sessionType"] = "beta1";
    write(sessionJson);

    userJson = read();

    cout << "userJson (test()): " << QJsonDocument(userJson).toJson().toStdString() << endl;

    cout << "loadSavedSessions(): " << endl;
    loadSavedSessions();

    cout << "Iterating over savedSessions: " << endl;

    for (auto session : savedSessions) {
        cout << *session << endl;
    }
}
