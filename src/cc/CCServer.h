#ifndef __CC_SERVER_H__
#define __CC_SERVER_H__


#include <uv.h>


#include "interfaces/IConsoleListener.h"


class Console;
class Httpd;
class Options;

class CCServer : public IConsoleListener
{
public:
  CCServer(int argc, char **argv);
  ~CCServer();

  int start();

protected:
  void onConsoleCommand(char command) override;

private:
  void stop();
  void moveToBackground();

  static void onSignal(uv_signal_t* handle, int signum);

  static CCServer* m_self;

  Console* m_console;
  Httpd* m_httpd;
  Options* m_options;
  uv_signal_t m_signal;
};


#endif /* __CC_SERVER_H__ */
