#include "session.h"

using namespace std;

Session::Session() {
	// this constructor is called when the device is turned on
	this->sessionType = NULL_SESSION_TYPE;
	this->sessionGroup = NULL_SESSION_GROUP;
	this->sessionIntensity = 1;
}

Session::Session(SessionType sessionType, SessionGroup sessionGroup, int sessionIntensity) {
    this->sessionType = sessionType;
    this->sessionGroup = sessionGroup;
    this->sessionIntensity = sessionIntensity;
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

// helpers
// std::string Session::convertSessionGroupToString(SessionGroup sessionGroup) {
//     switch (sessionGroup) {
//         case Session::TWENTY_MINUTES:
//             return "TWENTY_MINUTES";
//         case Session::FORTY_FIVE_MINUTES:
//             return "FORTY_FIVE_MINUTES";
//         case Session::USER_DESIGNATED:
//             return "USER_DESIGNATED";
//         default:
//             return "NULL_SESSION_GROUP";
//     }
// }

// std::string Session::convertSessionTypeToString(SessionType sessionType) {
//     switch (sessionType) {
//         case Session::DELTA:
//             return "DELTA";
//         case Session::ALPHA:
//             return "ALPHA";
//         case Session::BETA1:
//             return "BETA1";
//         case Session::BETA2:
//             return "BETA2";
//         default:
//             return "NULL_SESSION_TYPE";
//     }
// }

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

// setters
//

void Session::setSessionType(SessionType sessionType) {
    this->sessionType = sessionType;
}

void Session::setSessionGroup(SessionGroup sessionGroup) {
    this->sessionGroup = sessionGroup;
}

void Session::setSessionIntensity(int sessionIntensity) {
    this->sessionIntensity = sessionIntensity;
}
