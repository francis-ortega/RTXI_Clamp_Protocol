PLUGIN_NAME = clamp_protocol

RTXI_INCLUDES = /usr/local/lib/rtxi_includes

HEADERS = clamp-protocol.h\
          protocol.h\
          clamp-protocol-editor.h\
          clamp-protocol-window.h\
          $(RTXI_INCLUDES)/basicplot.h\
          $(RTXI_INCLUDES)/scrollzoomer.h\
          $(RTXI_INCLUDES)/scrollbar.h\

SOURCES = clamp-protocol.cpp \
          moc_clamp-protocol.cpp\
          protocol.cpp\
          clamp-protocol-editor.cpp\
          moc_clamp-protocol-editor.cpp\
          clamp-protocol-window.cpp\
          moc_clamp-protocol-window.cpp\
          $(RTXI_INCLUDES)/basicplot.cpp\
          $(RTXI_INCLUDES)/moc_basicplot.cpp\
          $(RTXI_INCLUDES)/scrollzoomer.cpp\
          $(RTXI_INCLUDES)/moc_scrollzoomer.cpp\
          $(RTXI_INCLUDES)/scrollbar.cpp\
          $(RTXI_INCLUDES)/moc_scrollbar.cpp\

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
