#ifndef SESSION_H
#define SESSION_H

#include <QJsonObject>
#include <string>
#include <iostream>

class Session {
    public:
        enum SessionType {
            DELTA, ALPHA, BETA1, BETA2, NULL_SESSION_TYPE
        };

        enum SessionGroup {
            TWENTY_MINUTES, FORTY_FIVE_MINUTES, USER_DESIGNATED, NULL_SESSION_GROUP
        };

        Session();
        Session(SessionType sessionType, SessionGroup sessionGroup, int sessionIntensity);
        Session(QJsonObject sessionJson);
        ~Session();

        // getters
        SessionType getSessionType() const;
        SessionGroup getSessionGroup() const;
        int getSessionIntensity() const;
		bool isTypeSet();
		bool isGroupSet();

        // setters
        void setSessionType(SessionType sessionType);
        void setSessionGroup(SessionGroup sessionGroup);
        void setSessionIntensity(int sessionIntensity);


        // helpers
        static QString groupToStr(SessionGroup sessionGroup);
        static QString typeToStr(SessionType sessionType);
        static SessionGroup strToGroup(const QString& groupStr);
        static SessionType strToType(const QString& typeStr);

        // Json helpers
        QJsonObject toJson();

        // converts to a one line string
        QString toString();

        // override cout <<
        friend std::ostream& operator<<(std::ostream& os, const Session& session);

    private:
        SessionType sessionType;
        SessionGroup sessionGroup; // 20min, 45min, userDesignated
        int sessionIntensity;
		bool typeSet;
		bool groupSet;
		int userDesignatedSessionTime;
};


#endif // SESSION_H
