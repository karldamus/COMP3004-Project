#ifndef USER_H
#define USER_H

#include "session.h"
#include <vector>


class User {
    Q_GADGET;

    public:
        User();
        User(int userId);
        ~User();

        int getUserId();

        // sessions
        void loadSavedSessions();
        void unloadSavedSessions();
        void loadSession(Session* session);
        void unloadSession();

    private:
        int userId; // 1, 2, or 3
        std::vector<Session*> savedSessions;
        Session* activeSession;

}


#endif // USER_H