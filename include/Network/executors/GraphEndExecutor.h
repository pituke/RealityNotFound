#ifndef __3DVisualGraphEndExecutor_H__
#define __3DVisualGraphEndExecutor_H__

#include "Network/executors/AbstractExecutor.h"

namespace Network {

    class GraphEndExecutor : public AbstractExecutor {

    public:
        void execute();
        static const quint8 INSTRUCTION_NUMBER = 7;

    };

}

#endif
