#include "session.h"

using namespace std;

Session::Session() {

}

Session::Session(SessionType sessionType, SessionGroup sessionGroup, int sessionIntensity) {
    this->sessionType = sessionType;
    this->sessionGroup = sessionGroup;
    this->sessionIntensity = sessionIntensity;
}

Session::~Session() {

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