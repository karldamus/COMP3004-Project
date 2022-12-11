#ifndef SESSION_H
#define SESSION_H

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
        // std::string convertSessionGroupToString(SessionGroup sessionGroup);
        // std::string convertSessionTypeToString(SessionType sessionType);

        // override cout <<
        friend std::ostream& operator<<(std::ostream& os, const Session& session);

    private:
        SessionType sessionType;
        SessionGroup sessionGroup; // 20min, 45min, userDesignated
        int sessionIntensity;
		bool typeSet;
		bool groupSet;
};


#endif // SESSION_H
