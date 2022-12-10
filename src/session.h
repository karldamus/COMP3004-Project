#ifndef SESSION_H
#define SESSION_H

#include <string>

class Session {
    public:
        enum SessionType {
            DELTA, ALPHA, BETA1, BETA2
        };

        enum SessionGroup {
            TWENTY_MINUTES, FORTY_FIVE_MINUTES, USER_DESIGNATED
        };

        Session();
        Session(SessionType sessionType, SessionGroup sessionGroup, int sessionIntensity);
        ~Session();

        // getters
        SessionType getSessionType() const;
        SessionGroup getSessionGroup() const;
        int getSessionIntensity() const;

        // setters
        void setSessionType(SessionType sessionType);
        void setSessionGroup(SessionGroup sessionGroup);
        void setSessionIntensity(int sessionIntensity);

    private:
        SessionType sessionType;
        SessionGroup sessionGroup; // 20min, 45min, userDesignated
        int sessionIntensity;
};


#endif // SESSION_H