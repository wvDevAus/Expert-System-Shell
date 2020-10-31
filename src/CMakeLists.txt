# Source files for the executable
target_sources(${ESS_TARGET_NAME}
	PRIVATE
		# Entry point
		main.cpp

		# Utility
		utility/DynamicEnum.cpp

		# Main Window
		gui/mainwindow.h
		gui/mainwindow.cpp
		gui/mainwindow.ui

		# Fact Database
		databases/facts/FactTypes.cpp
		databases/facts/FactDatabase.cpp
)
