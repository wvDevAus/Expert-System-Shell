#pragma once

#include <QDialog>

#include "ui_NewFact.h"

    /**
     * @brief Expert System Shell Fact creation GUI.
     * Implemented through the Member Variable method.
     */
class NewFact : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit NewFact(QWidget *parent = nullptr);

private slots:
        /// Creates a new Fact using the data it is currently storing.
    void CreateNewFact();

private:
        /// Pointer member to auto-generated UI element.
    Ui::NewFact ui;
};