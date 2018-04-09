#ifndef __HTTPD_H__
#define __HTTPD_H__

#include <string>
#include <sstream>
#include <uv.h>

#include "Options.h"


struct MHD_Connection;
struct MHD_Daemon;
struct MHD_Response;

class Httpd
{
public:
    Httpd(const Options *options);
    bool start();

private:

    typedef struct PostContext
	{
		std::stringstream data;
    } ConnectionContext;

    static int sendResponse(MHD_Connection* connection, unsigned status, MHD_Response* rsp, const char* contentType);

    unsigned basicAuth(MHD_Connection* connection, std::string &resp);
    unsigned tokenAuth(MHD_Connection* connection);

    static int handler(void* httpd, MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void**con_cls);
    static int handleGET(const Httpd* httpd, MHD_Connection* connection, const char* url);
    static int handlePOST(const Httpd* httpd, MHD_Connection* connection, const char* url, const char* upload_data, size_t* upload_data_size, void** con_cls);

	static std::string readFile(const std::string &fileName);

	const Options* m_options;
    MHD_Daemon* m_daemon;

	std::string m_keyPem;
	std::string m_certPem;
};

#endif /* __HTTPD_H__ */
