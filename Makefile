NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCDIR = coders
OBJDIR = obj

SRCS = $(SRCDIR)/main.c \
	$(SRCDIR)/parser.c \
	$(SRCDIR)/init.c \
	$(SRCDIR)/dongle.c \
	$(SRCDIR)/threads.c \
	$(SRCDIR)/utils.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re