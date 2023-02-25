#include"myTime.h"

string myTime::getCurrentFormatTimeStr()
{
    time_t timep;
    time(&timep);
    char tmp[64] = {'\0'};
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return string(tmp);
}

string myTime::getTimeStampStr()
{
    timeval stamp{};
    gettimeofday(&stamp, NULL);
    return to_string(stamp.tv_sec) + to_string(stamp.tv_usec);
}