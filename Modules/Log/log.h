/*=============================================================================
#     FileName: log.h
#         Desc: 日志记录，只支持单线程，支持网页格式
#       Author: hanxi
#        Email: hanxi.com@gmail.com
#     HomePage: http://hanxi.cnblogs.com
#      Version: 0.0.1
#   LastChange: 2013-03-07 18:24:21
#      History:
=============================================================================*/

#ifndef __LOG_H_
#define __LOG_H_

//STD
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>

const char HTML_LOG =  1;     //输出为html格式
const char TXT_LOG  =  2;     //输出为txt格式

#define __LOGARG__ __FILE__, __LINE__, __FUNCTION__

class Log
{
private:
    static std::ofstream     ms_file;          //log文件流
    static const char       *ms_path;          //log文件路径
    static int               ms_logLevel;      //小于logLevel的log将被打印出来
    static int               ms_logFileType;   //log文件类型

    std::string             *m_theFunctionName;//进入的函数名
    int                      m_nowLevel;

public:
    static void s_init(const char *i_path, int i_logLevel, int i_fileType);
    static void s_stop();

    Log(const char *fileName, int line, const char *funcName, int logLevel);
    ~Log();

    template<typename Type>
    void debug(Type msg);
    template<typename Type>
    Log& operator<<(Type msg);
    Log& operator<<(Log& (*__pf)(Log&));

    static Log& endl(Log& log);
};

inline
void Log::s_init(const char* i_path, int i_logLevel, int i_fileType)
{
    ms_path = i_path;
    ms_logLevel = i_logLevel;
    ms_logFileType = i_fileType;
    ms_file.open(ms_path, std::ios::out|std::ios::trunc);

    if(!ms_file)
    {
        ms_file.close();
        std::cout << "ERROR. Initializing Log::path.Path does not exist: "
            << ms_path <<std::endl;
        exit(1);
    }
    if (ms_logFileType==HTML_LOG)
    {
        const char* utf8header = "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />";
        ms_file << utf8header;
        const char* styleType = "<head><style type=\"text/css\">r{color: red}b{color: blue}</style></head>";
        ms_file << styleType;
    }
}

inline
void Log::s_stop()
{
    ms_file.close();
}

//系统当前时间
inline
std::string get_cur_datetime(const char* format)
{
    time_t ttime = time(NULL);
    struct tm* datetime = localtime(&ttime);
    char datetime_str[64];
    strftime(datetime_str, sizeof(datetime_str), format, datetime);
    return datetime_str;
}

inline
Log::Log(const char *fileName, int line, const char *funcName, int logLevel)
:m_theFunctionName(0),m_nowLevel(logLevel)
{
    if(m_nowLevel <= ms_logLevel) {
        if (ms_logFileType==TXT_LOG) {
            ms_file << "[" << get_cur_datetime("%d-%m-%Y][%H:%M:%S") << "] "
                << "[Enter function :" << funcName << "] "
                << "[" << fileName << "] [" << line << "] " << std::endl;
            m_theFunctionName = new std::string(funcName);
        }
        else if (ms_logFileType==HTML_LOG) {
            ms_file << "<r>[" << get_cur_datetime("%d-%m-%Y][%H:%M:%S") << "]</r>"
                << "<b>[Enter function :" << funcName << "]</b>"
                << "<r>[" << fileName << "][" << line << "]<r><br>";
            m_theFunctionName = new std::string(funcName);
        }
        else
        {
            std::cout << "ERROR. File type can't define." << std::endl;
        }
    }
}

inline
Log::~Log()
{
    if(m_nowLevel <= ms_logLevel) {
        if (ms_logFileType==TXT_LOG) {
            ms_file << "[" << get_cur_datetime("%d-%m-%Y][%H:%M:%S") << "] "
                << "[Exit  function :" << *m_theFunctionName << "] " << std::endl;
            delete m_theFunctionName;
        }
        else if (ms_logFileType==HTML_LOG) {
            ms_file << "<r>[" << get_cur_datetime("%d-%m-%Y][%H:%M:%S") << "]</r>"
                << "<b>[Exit  function :" << *m_theFunctionName << "]<b><br>";
            delete m_theFunctionName;
        }
        else
        {
            std::cout << "ERROR. File type can't define." << std::endl;
        }
    }
}

template<typename Type>
inline
void Log::debug(Type msg)
{
    if(m_nowLevel <= ms_logLevel)
    {
        if (ms_logFileType==TXT_LOG)
        {
            ms_file << msg << std::endl;
        }
        else if (ms_logFileType==HTML_LOG)
        {
            ms_file << "<b>" << msg << "</b><br>";
        }
        else
        {
            std::cout << "ERROR. File type can't define." << std::endl;
        }
    }
}

template<typename Type>
inline
Log& Log::operator<<(Type msg)
{
    if(m_nowLevel <= ms_logLevel)
    {
        if (ms_logFileType==TXT_LOG)
        {
            ms_file << msg;
        }
        else if (ms_logFileType==HTML_LOG)
        {
            ms_file << "<b>" << msg << "</b>";
        }
        else
        {
            std::cout << "ERROR. File type can't define." << std::endl;
        }
    }
    return *this;
}

inline
Log& Log::operator<<(Log& (*__pf)(Log&))
{
    return __pf(*this);
}

inline
Log& Log::endl(Log& log)
{
    if(log.m_nowLevel <= ms_logLevel) {
        if (ms_logFileType==TXT_LOG)
        {
            ms_file << '\n';
            flush(ms_file);
        }
        else if (ms_logFileType==HTML_LOG)
        {
            ms_file << "</br>";
            flush(ms_file);
        }
        else
        {
            std::cout << "ERROR. File type can't define." << std::endl;
        }
    }
    return log;
}

#endif /* defined(__LOG_H_) */

