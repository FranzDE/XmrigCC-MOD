#ifndef __APP_H__
#define __APP_H__


#include <uv.h>


#include "interfaces/IConsoleListener.h"


class Console;
class Httpd;
class Network;
class Options;
class CCClient;

class App : public IConsoleListener
{
public:
  App(int argc, char **argv);
  ~App();

  int start();

  static void restart();
  static void shutdown();

protected:
  void onConsoleCommand(char command) override;

private:
  void background();
  void release();
  void stop(bool restart);

  static void onSignal(uv_signal_t* handle, int signum);
  static void onCommandReceived(uv_async_t* handle);

  static App *m_self;

  bool m_restart;

  Console *m_console;
  Httpd *m_httpd;
  Network *m_network;
  Options *m_options;
  uv_signal_t m_signal;
  uv_signal_t m_sigHUP;
  uv_signal_t m_sigINT;
  uv_signal_t m_sigTERM;
  CCClient *m_ccclient;
  uv_async_t m_async;
};


#endif /* __APP_H__ */
