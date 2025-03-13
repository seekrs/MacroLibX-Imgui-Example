NAME = ./mlx_imgui

SRCS = main.c

OBJ_DIR	= objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC = clang
CFLAGS = -I MacroLibX/includes -g3 -D IMGUI_IMPL_VULKAN_NO_PROTOTYPES -D CIMGUI_USE_VULKAN -D CIMGUI_USE_SDL2
CLIBS = cimgui/cimgui.so MacroLibX/libmlx.so -lSDL2

RM = rm -rf

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: cimgui mlx $(NAME)

cimgui:
	@make -f cimgui.mk -j12

mlx:
	@make -C MacroLibX -j12

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) -o $(NAME) $(CLIBS) $(OBJS)

$(OBJ_DIR):
	mkdir -p $(sort $(addprefix $(OBJ_DIR)/, $(dir $(SRCS))))

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re cimgui mlx
