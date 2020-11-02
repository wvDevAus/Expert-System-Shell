#include "RuleEditor.h"

#include "gui/editor/rules/NewRule.h"

RuleEditor::RuleEditor(QWidget *parent) : QDialog(parent) {
    // Ensure that the UI is configured
    ui.setupUi(this);
}

void RuleEditor::RuleSelected() {

}

void RuleEditor::NewRule() {
    // Open the NewRule dialog
    class NewRule modal_dialog(this);
    modal_dialog.exec();

    // Update the Fact list
    UpdateRuleList();
}

void RuleEditor::DeleteRule() {

}

void RuleEditor::ConditionSelected() {

}

void RuleEditor::NewCondition() {

}

void RuleEditor::DeleteCondition() {

}

void RuleEditor::SaveCondition() {

}

void RuleEditor::ConditionFactSelected() {

}

void RuleEditor::AssignmentSelected() {

}

void RuleEditor::NewAssignment() {

}

void RuleEditor::DeleteAssignment() {

}

void RuleEditor::SaveAssignment() {

}

void RuleEditor::AssignmentFactSelected() {

}

void RuleEditor::UpdateRuleList() {

}

void RuleEditor::UpdateRuleEditors() {

}

void RuleEditor::ClearRuleEditors() {

}
