#pragma once

#include <QDialog>

#include "ui_FactEditor.h"

    /**
     * @brief Expert System Shell Fact editor GUI.
     * Implemented through the Member Variable method.
     */
class FactEditor : public QDialog {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit FactEditor(QWidget *parent = nullptr);

private slots:

private:
        /// Pointer member to auto-generated MainMenu UI class
    Ui::FactEditor ui;
};
