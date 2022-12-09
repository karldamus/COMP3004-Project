#ifndef SESSION_H
#define SESSION_H

#include <string>

class Session {
    Q_GADGET;

    public:
        enum SessionType {
            DELTA, ALPHA, BETA1, BETA2
        };

        enum SessionGroup {
            TWENTY_MINUTES, FORTY_FIVE_MINUTES, USER_DESIGNATED
        };

        Session();
        Session(SessionType sessionType, SessionGroup sessionGroup);


        // getters
        SessionType getSessionType() const;
        SessionGroup getSessionGroup() const;

        // setters
        void setSessionType(SessionType sessionType);
        void setSessionGroup(SessionGroup sessionGroup);

    private:
        SessionType sessionType;
        SessionGroup groupType; // 20min, 45min, userDesignated
}


#endif // SESSION_H