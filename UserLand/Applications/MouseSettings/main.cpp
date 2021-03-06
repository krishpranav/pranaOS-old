// includes
#include "MouseSettingsWindow.h"
#include <LibGUI/Action.h>
#include <LibGUI/Application.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Menu.h>
#include <LibGUI/MenuBar.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (pledge("stdio cpath rpath recvfd sendfd unix fattr", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    auto app = GUI::Application::construct(argc, argv);

    if (pledge("stdio cpath rpath recvfd sendfd", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    auto app_icon = GUI::Icon::default_icon("app-mouse");

    auto window = MouseSettingsWindow::construct();
    window->set_title("Mouse Settings");
    window->resize(300, 220);
    window->set_resizable(false);
    window->set_minimizable(false);
    window->set_icon(app_icon.bitmap_for_size(16));

    auto menubar = GUI::MenuBar::construct();
    auto& app_menu = menubar->add_menu("File");
    app_menu.add_action(GUI::CommonActions::make_quit_action([&](auto&) {
        app->quit();
    }));

    auto& help_menu = menubar->add_menu("Help");
    help_menu.add_action(GUI::CommonActions::make_about_action("Mouse Settings", app_icon, window));
    window->set_menubar(move(menubar));

    window->show();
    return app->exec();
}
