PLUGIN_NAME = new_clamp_protocol

HEADERS = clamp_protocol.h\
          CP_protocol.h\
          CP_protocol_editor.h\
          ui/CP_main_windowUI.h\
          ui/CP_protocol_editorUI.h\
          CP_plot_window.h\
          ui/CP_plot_windowUI.h\
          plot/basicplot.h\
          plot/scrollzoomer.h\
          plot/scrollbar.h\

SOURCES = clamp_protocol.cpp\
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

LIBS = -lqwt

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
