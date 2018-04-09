#include <string>
#include "App.h"
#include "Cpu.h"
#include "log/Log.h"
#include "Options.h"
#include "version.h"

void App::background()
{
    if (m_options->affinity() != -1L) {
        Cpu::setAffinity(-1, m_options->affinity());
    }

    if (m_options->background()) {
        Log::i()->text(Options::i()->colors()
                       ? "\x1B[01;31m\nBackground mode is not supported by %s on *nix Systems. Please use screen/tmux or systemd service instead.\n"
                       : "\nBackground mode is not supported by %s on *nix Systems. Please use screen/tmux or systemd service instead.\n",
                       APP_NAME);
    }
}