#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "log/FileLog.h"


FileLog::FileLog(const char *fileName)
{
    uv_fs_t req;
    m_file = uv_fs_open(uv_default_loop(), &req, fileName, O_CREAT | O_APPEND | O_WRONLY, 0644, nullptr);
    uv_fs_req_cleanup(&req);
}


void FileLog::message(int level, const char* fmt, va_list args)
{
    if (m_file < 0) {
        return;
    }

    time_t now = time(nullptr);
    tm stime;

#   ifdef _WIN32
    localtime_s(&stime, &now);
#   else
    localtime_r(&now, &stime);
#   endif

    char *buf = new char[512];
    int size = snprintf(buf, 23, "[%d-%02d-%02d %02d:%02d:%02d] ",
                        stime.tm_year + 1900,
                        stime.tm_mon + 1,
                        stime.tm_mday,
                        stime.tm_hour,
                        stime.tm_min,
                        stime.tm_sec);

    size = vsnprintf(buf + size, 512 - size - 1, fmt, args) + size;
    buf[size] = '\n';

    write(buf, size + 1);
}


void FileLog::text(const char* fmt, va_list args)
{
    message(0, fmt, args);
}



void FileLog::onWrite(uv_fs_t *req)
{
    delete [] static_cast<char *>(req->data);

    uv_fs_req_cleanup(req);
    delete req;
}


void FileLog::write(char *data, size_t size)
{
    uv_buf_t buf = uv_buf_init(data, (unsigned int) size);
    uv_fs_t *req = new uv_fs_t;
    req->data = buf.base;

    uv_fs_write(uv_default_loop(), req, m_file, &buf, 1, 0, FileLog::onWrite);
}
