CC	= gcc
CFLAGS	= -Wall -Wextra -Werror
VFLAGS 	= --leak-check=full --show-leak-kinds=all --track-origins=yes
SRCDIR	= srcs
INCDIR	= includes
BUILDDIR = build
EXEC = kv
DEBUG ?= 0
LIBNAME = libkv.a

ifneq ($(DEBUG), 0)
	CFLAGS += -g3
endif

SRCS	:= $(shell find $(SRCDIR) -type f -name "*.c")
LIB_SRCS	:= $(filter-out $(SRCDIR)/main.c, $(SRCS))

OBJS	:= $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))
LIB_OBJS	:= $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(LIB_SRCS))

all:	${EXEC}

${EXEC}: ${OBJS}
	${CC} ${OBJS} -o ${EXEC}

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -I ${INCDIR} -c $< -o $@

lib:	${LIBNAME}

${LIBNAME}: ${LIB_OBJS}
	ar rcs $@ $^

clean:
	rm -rf ${BUILDDIR}

fclean:	clean
	rm -f ${EXEC}

lclean:
	rm -rf ${LIBNAME}

debug: fclean ${EXEC} run

run:
	clear
	valgrind ${VFLAGS} ./${EXEC}

re:	fclean all

.PHONY:	all clean fclean re debug run