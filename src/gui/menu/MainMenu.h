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
         * @brief Called when pressing the "SystemSave" button.
         * Saves the current domain knowledge databases to file.
         */
    void SaveExpertSystem();

        /**
         * @brief Called when pressing the "SystemLoad" button.
         * Loads the current domain knowledge databases from file.
         */
    void LoadExpertSystem();

        /**
         * @brief Called when pressing the "DomainKnowledgeRules" button.
         * Opens an interface to view and edit the current domain knowledge Rule database.
         */
    void RulesDomainKnowledge();

        /**
         * @brief Called when pressing the "DomainKnowledgeFacts" button.
         * Opens an interface to view and edit the current domain knowledge Fact database.
         */
    void FactsDomainKnowledge();

        /**
         * @brief Called when pressing the "ConsultationForward" button.
         * Opens the consultation interface with forwards chaining inference.
         */
    void ForwardInferrencingConsult();

        /**
         * @brief Called when pressing the "ConsultationBackward" button.
         * Opens the consultation interface with backwards chaining inference.
         */
    void BackwardInferrencingConsult();

private:
        /// Pointer member to auto-generated MainMenu UI class
    Ui::MainMenu ui;
};
