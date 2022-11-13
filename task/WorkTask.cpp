#include "WorkTask.h"

#include "../mysql/Connection_Pool.h"
using namespace yazi::Conpool;
#include "Logger.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "SocketHandler.h"
using namespace yazi::socket;

#include "Workflow.h"
using namespace yazi::engine;

using namespace yazi::task;
using namespace std;

#include <sstream>

#include <iostream>
#include <iomanip>
#include <chrono>
using std::ostringstream;

WorkTask::WorkTask(Socket *socket) : Task(socket)
{
}

WorkTask::~WorkTask()
{
}

void WorkTask::run()
{
    int device_id;
    int on_bed;
    int body_move;
    int heart_rate;
    int breath_rate;
    string log_time;
    char QUERY_CMD[1024] = {0}; //
    debug("work task run");
    SocketHandler *handler = Singleton<SocketHandler>::instance();

    Socket *socket = static_cast<Socket *>(m_data);

    MsgHead msg_head;
    RadarProtol data_struct;
    Dataprotol data_format;

    // int len;
    //  memset(&msg_head, 0, sizeof(msg_head));
    int temp = sizeof(data_struct);
    printf("%d", temp);
    int len = socket->recv((char *)(&data_struct), sizeof(data_struct)); //把帧头先读出来
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //转为字符串
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d-%H-%M-%S");
    log_time = ss.str();
    device_id = data_struct.DATA[0];
    on_bed = data_struct.DATA[1];
    body_move = data_struct.DATA[2];
    heart_rate = data_struct.DATA[3];
    breath_rate = data_struct.DATA[4];
    if (len == 0)
    {
        error("socket closed by peer");
        handler->remove(socket);
        return;
    }
    if (len == -1 && errno == EAGAIN)
    {
        error("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len == -1 && errno == EWOULDBLOCK)
    {
        error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len == -1 && errno == EINTR)
    {
        error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len != sizeof(data_struct))
    {
        error("recv msg head error length: %d, errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    // info("recv msg head len: %d, flag: %s, cmd: %d, body len: %d", len, msg_head.flag, msg_head.cmd, msg_head.len);
    if (strncmp(data_struct.HEAD, &data_format.HEAD, 1) != 0) //比较帧头
    {
        error("msg head flag error");
        handler->remove(socket);
    }
    if (data_struct.END[0] != data_format.END1) //比较帧头
    {
        error("msg end1 error");
        handler->remove(socket);
    }
    if (data_struct.END[1] != data_format.END2) //比较帧头
    {
        error("msg end2 error");
        handler->remove(socket);
    }
    // if (len >= uint32_t(recv_buff_size)) //比较数据长度
    // {
    //     // error("recv msg body len: %d, large than recv_buff_size: %d", msg_head.len, recv_buff_size);
    //     handler->remove(socket);
    //     return;
    // }

    // char buf[recv_buff_size];
    // memset(buf, 0, recv_buff_size);
    // //接收到的数据
    // len = socket->recv(buf, msg_head.len); //继续接收接下来的数据
    sprintf(QUERY_CMD, "INSERT INTO Sleep(id,off_bed,body_move,heart_rate,breath_rate,log_time) VALUES('%d','%d','%d','%d','%d','%s')", device_id, on_bed, body_move, breath_rate, heart_rate, log_time.c_str());

    // if (len == -1 && errno == EAGAIN)
    // {
    //     error("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
    //     handler->remove(socket);
    //     return;
    // }
    // if (len == -1 && errno == EWOULDBLOCK)
    // {
    //     error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
    //     handler->remove(socket);
    //     return;
    // }
    // if (len == -1 && errno == EINTR)
    // {
    //     error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
    //     handler->remove(socket);
    //     return;
    // }
    // if (len != (int)(msg_head.len))
    // {
    //     error("recv msg body error length: %d, body: %s, errno: %d", len, buf, errno);
    //     handler->remove(socket);
    //     return;
    // }

    // info("recv msg body len: %d, msg data: %s", len, buf);

    //建立工作流对象
    // Workflow *workflow = Singleton<Workflow>::instance();
    // DATA解析出的数据
    Connection_Pool *Con_Pool = Singleton<Connection_Pool>::instance();
    ostringstream os;
    os << (int)(msg_head.cmd);
    const string work = os.str();
    // const string input = buf;
    string output;

    //获取连接
    shared_ptr<Mysql> Mysql_CON = Con_Pool->getConect();
    //插入数据
    Mysql_CON->Mysql_query(QUERY_CMD);
    // workflow->run(work, input, output);
    //发送函数
    // socket->send(output.c_str(), output.length());
    handler->attach(socket);
}

void WorkTask::destroy()
{
    debug("work job destory");
    delete this;
}
