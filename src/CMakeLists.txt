# Source files for the executable
target_sources(${ESS_TARGET_NAME}
	PRIVATE
		# Entry point
		main.cpp

		# Utility
		utility/Confidence.cpp
		utility/DynamicEnum.cpp

		# Main Window
		gui/mainwindow.h
		gui/mainwindow.cpp
		gui/mainwindow.ui

		# Fact Database
		knowledge/facts/Facts.cpp
		knowledge/facts/Database.cpp

		# Rule Database
		knowledge/rules/Conditions.cpp
		knowledge/rules/Antecedent.cpp
)
