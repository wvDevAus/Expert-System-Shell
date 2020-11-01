#pragma once

#include <QMainWindow>

#include "ui_MainMenu.h"

    /**
     * @brief Expert System Shell main menu GUI.
     * Implemented through the Member Variable method.
     */
class MainMenu : public QMainWindow {
    Q_OBJECT

public:
        /// Inherited Qt5 constructor.
    explicit MainMenu(QWidget *parent = nullptr);

private slots:
        /**
         *
         */
    void SaveExpertSystem();

        /**
         *
         */
    void LoadExpertSystem();

        /**
         *
         */
    void RulesDomainKnowledge();

        /**
         *
         */
    void FactsDomainKnowledge();

        /**
         *
         */
    void ForwardInferrencingConsult();

        /**
         *
         */
    void BackwardInferrencingConsult();

private:
        /// Pointer member to auto-generated MainMenu UI class
    Ui::MainMenu ui;
};
