PLUGIN_NAME = clamp_protocol

HEADERS = clamp-protocol.cpp

LIBS = -lqwt

SOURCES = \
		clamp_protocol.cpp\
		CP_protocol.cpp\
		CP_protocol_editor.cpp\
		ui/CP_main_windowUI.cpp\
		ui/CP_protocol_editorUI.cpp\
		CP_plot_window.cpp\
		ui/CP_plot_windowUI.cpp\
		plot/basicplot.cpp\
		plot/scrollzoomer.cpp\
		plot/scrollbar.cpp\
		moc_clamp_protocol.cpp\
		moc_CP_protocol_editor.cpp\
		ui/moc_CP_main_windowUI.cpp\
		ui/moc_CP_protocol_editorUI.cpp\
		moc_CP_plot_window.cpp\
		ui/moc_CP_plot_windowUI.cpp\
		plot/moc_basicplot.cpp\
		plot/moc_scrollzoomer.cpp\
		plot/moc_scrollbar.cpp

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
