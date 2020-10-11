#include <iostream>
#include <string>

#include "QApplication"
#include "nlohmann/json.hpp"

#include "mainwindow.h"

int main(int argc, char *argv[]) {
	// Testing JSON
    nlohmann::json JSONTest;
    JSONTest["example"] = "Hello World!";
	std::cout << JSONTest["example"];

    // Testing Qt
    QApplication QtAppTest(argc, argv);
    MainWindow QtWindowTest;
    QtWindowTest.show();
    return QtAppTest.exec();
}
