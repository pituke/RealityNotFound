#ifndef __3DVisualUsersExecutor_H__
#define __3DVisualUsersExecutor_H__

#include "Network/executors/AbstractExecutor.h"

namespace Network {

    class UsersExecutor : public AbstractExecutor {

    private:
        QDataStream *stream;

    public:
        void execute();
        void setDataStream(QDataStream *ds) {stream = ds;}
        static const quint8 INSTRUCTION_NUMBER = 3;
    };

}

#endif
