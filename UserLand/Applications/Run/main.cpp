// includes
#include "RunWindow.h"
#include <AK/StringBuilder.h>
#include <LibGUI/Application.h>
#include <LibGUI/Desktop.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (pledge("stdio recvfd sendfd thread accept cpath rpath wpath unix fattr proc exec", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    auto app = GUI::Application::construct(argc, argv);

    if (pledge("stdio recvfd sendfd thread accept cpath rpath wpath unix proc exec", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    auto window = RunWindow::construct();

    window->move_to(12, GUI::Desktop::the().rect().bottom() - GUI::Desktop::the().taskbar_height() - 12 - window->height());
    window->show();

    return app->exec();
}
