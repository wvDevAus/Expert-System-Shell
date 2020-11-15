#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <QApplication>

#include "gui/menu/MainMenu.h"

int main(int argc, char *argv[]) {
    // Create and configure the QApplication
    QApplication expert_system_shell_app(argc, argv);

    // Generate and show the MainMenu
    MainMenu expert_system_shell_main_menu;
    expert_system_shell_main_menu.show();

    // Run the QApplication
    return expert_system_shell_app.exec();
}
