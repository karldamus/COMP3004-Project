#include "session.h"

using namespace std;


Session::Session() {
	// this constructor is called when the device is turned on
	this->sessionType = NULL_SESSION_TYPE;
	this->sessionGroup = NULL_SESSION_GROUP;
	this->sessionIntensity = 1;
	this->groupSet = false;
	this->typeSet = false;
	this->userDesignatedSessionTime = 1;
}


Session::Session(SessionType sessionType, SessionGroup sessionGroup, int sessionIntensity) {
    this->sessionType = sessionType;
    this->sessionGroup = sessionGroup;
    this->sessionIntensity = sessionIntensity;
	this->userDesignatedSessionTime = 1;

	this->typeSet = (sessionType != Session::NULL_SESSION_TYPE);
    this->groupSet = (sessionGroup != Session::NULL_SESSION_GROUP);
}

Session::Session(SessionType sessionType, SessionGroup sessionGroup, int sessionIntensity, int userDesignatedSessionTime) {
    this->sessionType = sessionType;
    this->sessionGroup = sessionGroup;
    this->sessionIntensity = sessionIntensity;
    this->userDesignatedSessionTime = userDesignatedSessionTime;

    this->typeSet = (sessionType != Session::NULL_SESSION_TYPE);
    this->groupSet = (sessionGroup != Session::NULL_SESSION_GROUP);
}

Session::Session(QJsonObject sessionJson) {
    sessionType = strToType(sessionJson.value("sessionType").toString());
    sessionGroup = strToGroup(sessionJson.value("sessionGroup").toString());
    sessionIntensity = sessionJson["sessionIntensity"].toInt();
    userDesignatedSessionTime = sessionJson["userDesignatedSessionTime"].toInt();
}

Session::~Session() {

}

// override cout << for Session
std::ostream& operator<<(std::ostream& os, const Session& session) {
    os << "Session Type: " << session.sessionType << endl;
    os << "Session Group: " << session.sessionGroup << endl;
    os << "Session Intensity: " << session.sessionIntensity << endl;
    return os;
}

QString Session::groupToStr(SessionGroup sessionGroup) {
    switch (sessionGroup) {
        case Session::SessionGroup::TWENTY_MINUTES:
            return QString::fromStdString("20min");
        case Session::SessionGroup::FORTY_FIVE_MINUTES:
            return QString::fromStdString("45min");
        case Session::SessionGroup::USER_DESIGNATED:
            return QString::fromStdString("userDesignated");
        default:
            return "";
    }
}

QString Session::typeToStr(SessionType sessionType) {
    switch (sessionType) {
        case Session::SessionType::DELTA:
            return QString::fromStdString("delta");
        case Session::SessionType::ALPHA:
            return QString::fromStdString("alpha");
        case Session::SessionType::BETA1:
            return QString::fromStdString("beta1");
        case Session::SessionType::BETA2:
            return QString::fromStdString("beta2");
        default:
            return "";
    }
}

Session::SessionGroup Session::strToGroup(const QString& groupStr) {
    if (groupStr == "20min") {
        return Session::SessionGroup::TWENTY_MINUTES;
    } else if (groupStr == "45min") {
        return Session::SessionGroup::FORTY_FIVE_MINUTES;
    } else if (groupStr == "userDesignated") {
        return Session::SessionGroup::USER_DESIGNATED;
    } else {
        cout << "Received sessionGroupStr: " << groupStr.toStdString() << endl;
        qFatal("Invalid sessionGroupStr");
        return Session::SessionGroup::NULL_SESSION_GROUP;
    }
}

Session::SessionType Session::strToType(const QString& typeStr) {
    if (typeStr == "delta") {
        return Session::SessionType::DELTA;
    } else if (typeStr == "alpha") {
        return Session::SessionType::ALPHA;
    } else if (typeStr == "beta1") {
        return Session::SessionType::BETA1;
    } else if (typeStr == "beta2") {
        return Session::SessionType::BETA2;
    } else {
        cout << "Received sessionTypeStr: " << typeStr.toStdString() << endl;
        qFatal("Invalid sessionTypeStr");
        return Session::SessionType::NULL_SESSION_TYPE;
    }
}

QJsonObject Session::toJson() {
    // create json object
    QJsonObject sessionJson;

    sessionJson["sessionType"] = typeToStr(sessionType);
    sessionJson["sessionGroup"] = groupToStr(sessionGroup);
    sessionJson["sessionIntensity"] = sessionIntensity;
    sessionJson["userDesignatedSessionTime"] = userDesignatedSessionTime;

    return sessionJson;
}

int Session::getUserDesignatedSessionTime() {
    return userDesignatedSessionTime;
}

QString Session::toString() {
    return QString("%1, %2, INT: %3").arg(typeToStr(sessionType), groupToStr(sessionGroup), QString::number(sessionIntensity));
}

// getters
//

Session::SessionType Session::getSessionType() const {
    return this->sessionType;
}

Session::SessionGroup Session::getSessionGroup() const {
    return this->sessionGroup;
}

int Session::getSessionIntensity() const {
    return this->sessionIntensity;
}

bool Session::isGroupSet() {
	return this->groupSet;
}

bool Session::isTypeSet() {
	return this->typeSet;
}

void Session::setIsTypeSet(bool isSet) {
    this->typeSet = isSet;
}

void Session::setIsGroupSet(bool isSet) {
    this->groupSet = isSet;
}

// setters
//

void Session::setSessionType(SessionType sessionType) {
    this->sessionType = sessionType;
	if (sessionType != Session::NULL_SESSION_TYPE){
		this->typeSet = true;
	} else {
		this->typeSet = false;
	}
}

void Session::setSessionGroup(SessionGroup sessionGroup) {
    this->sessionGroup = sessionGroup;
	if (sessionGroup != Session::NULL_SESSION_GROUP){
		this->groupSet = true;
	} else {
		this->groupSet = false;
	}
}

void Session::setSessionIntensity(int sessionIntensity) {
    this->sessionIntensity = sessionIntensity;
}

