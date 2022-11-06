#include <string>
#include <iostream>
using namespace std;
class Protocol
{
private:
    enum State
    {
        is_long_enough,
        is_head,
        is_end,
        is_data
    };                   // 列举数据包解析存在的状态
    string buf_remained; // 存储之前没有处理完的数据
    union MyUnion
    {
        unsigned char buf[4];
        float f;
    } FLOAT_DATA;

    union MyUnion1
    {
        unsigned char buf[2];
        uint16_t f;
    } FLOAT_body_dis;

public:
    Protocol(/* args */);
    ~Protocol();
    void DATA_ANALYS(string DATA_BUFFER);
};

Protocol::Protocol(/* args */)
{
}

Protocol::~Protocol()
{
}
