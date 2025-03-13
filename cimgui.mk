OBJS = cimgui/cimgui.o
OBJS += cimgui/imgui/imgui.o
OBJS += cimgui/imgui/imgui_draw.o
OBJS += cimgui/imgui/imgui_demo.o
OBJS += cimgui/imgui/imgui_tables.o
OBJS += cimgui/imgui/imgui_widgets.o
OBJS += cimgui/imgui/backends/imgui_impl_sdl2.o
OBJS += cimgui/imgui/backends/imgui_impl_vulkan.o

CXXFLAGS = -O3 -fno-exceptions -fno-rtti -I /usr/include/SDL2 -D CIMGUI_USE_VULKAN -D CIMGUI_USE_SDL2 -D IMGUI_IMPL_VULKAN_NO_PROTOTYPES -D IMGUI_IMPL_API="extern \"C\""

OUTPUTNAME = cimgui/cimgui.so
CXXFLAGS += -I cimgui/imgui/
CXXFLAGS += -Wall
CXXFLAGS += -shared -fPIC
CFLAGS = $(CXXFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OUTPUTNAME)

$(OUTPUTNAME): $(OBJS)
	$(CXX) -o $(OUTPUTNAME) $(OBJS) $(CXXFLAGS) $(LINKFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(OUTPUTNAME)

re: fclean all

.PHONY: all clean fclean re
