#include <QApplication>

#include "gui/menu/MainMenu.h"

int main(int argc, char *argv[]) {
    // Create a new QApplication, launching the MainMenu window
    QApplication expert_system_shell_app(argc, argv);
    MainMenu expert_system_shell_main_menu;
    expert_system_shell_main_menu.show();
    return expert_system_shell_app.exec();
}
