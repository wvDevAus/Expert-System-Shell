# Source files for the executable
target_sources(${ESS_TARGET_NAME}
	PRIVATE
		# Entry point
		main.cpp

		# Main Window
		gui/mainwindow.h
		gui/mainwindow.cpp
		gui/mainwindow.ui
)
