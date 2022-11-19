#pragma once

#include "Task.h"
using namespace yazi::thread;

#include "Socket.h"
using namespace yazi::socket;
#define FRAME_LENGTH 13
#define DATA_LENGTH 10

namespace yazi
{
    namespace task
    {

        struct MsgHead
        {
            char flag[8]; //标志位，相当于起始位
            uint32_t cmd; //插件的选择
            uint32_t len;
        };
        struct RadarProtol
        {
            char HEAD[1];
            char DATA[DATA_LENGTH];
            char END[2];
            /* data */
        };

        struct Dataprotol
        {
            char HEAD = 0x5a;

            char END1 = 0x0d;

            char END2 = 0x0a;
            /* data */
        };

        const uint32_t recv_buff_size = 1024;

        class WorkTask : public Task
        {
        public:
            WorkTask(Socket *socket);
            virtual ~WorkTask();

            virtual void run();
            virtual void destroy();
        };

    }
}
