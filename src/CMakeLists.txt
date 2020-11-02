# Source files for the executable
target_sources(${ESS_TARGET_NAME}
	PRIVATE
		# Entry point
		main.cpp

		# Utility
		utility/Confidence.cpp
		utility/DynamicEnum.cpp

		# Fact Database
		knowledge/facts/Facts.cpp
		knowledge/facts/FactDatabase.cpp

		# Rule Database
		knowledge/rules/Antecedent.cpp
		knowledge/rules/Assignments.cpp
		knowledge/rules/Conditions.cpp
		knowledge/rules/Consequent.cpp
		knowledge/rules/Rule.cpp
		knowledge/rules/RuleDatabase.cpp

		# Main Menu
		gui/menu/MainMenu.h
		gui/menu/MainMenu.cpp
		gui/menu/MainMenu.ui

		# Fact Editor
		gui/editor/facts/FactEditor.h
		gui/editor/facts/FactEditor.cpp
		gui/editor/facts/FactEditor.ui

		# New Fact
		gui/editor/facts/NewFact.h
		gui/editor/facts/NewFact.cpp
		gui/editor/facts/NewFact.ui

		# New Enum
		gui/editor/facts/NewEnum.h
		gui/editor/facts/NewEnum.cpp
		gui/editor/facts/NewEnum.ui

		# Rule Editor
		gui/editor/rules/RuleEditor.h
		gui/editor/rules/RuleEditor.cpp
		gui/editor/rules/RuleEditor.ui

		# New Rule
		gui/editor/rules/NewRule.h
		gui/editor/rules/NewRule.cpp
		gui/editor/rules/NewRule.ui
)
