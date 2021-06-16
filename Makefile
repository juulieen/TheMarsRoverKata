
# PROJECT

release.name		=	TheMarsRover

release.srcs		=	src/flow.c	\
						src/parsing.c	\
						src/rover.c	\

release.main 		=	src/main.c

release.objs		=	$(addprefix $(dir $(BUILD_DIR)$(release.name)/), $(release.srcs:.c=.o))	\
						$(addprefix $(dir $(BUILD_DIR)$(release.name)/), $(release.main:.c=.o))

release.cflags		=	-Werror -pedantic-errors

release.ldflags		=

# TESTS

unit_tests.name		=	unit_tests_$(release.name)

unit_tests.srcs		=	$(release.srcs)									\
						tests/rover.c									\
						tests/parsing.c									\
						tests/flow.c									\

unit_tests.main 	=	

unit_tests.objs		=	$(addprefix $(dir $(BUILD_DIR)$(unit_tests.name)/), $(unit_tests.srcs:.c=.o))	\
						$(addprefix $(dir $(BUILD_DIR)$(unit_tests.name)/), $(unit_tests.main:.c=.o))

unit_tests.cflags	=	-fprofile-arcs -ftest-coverage

unit_tests.ldflags	=	-lcriterion -lgcov

# DEBUG

debug.name			=	debug_$(release.name)

debug.srcs			=	$(release.srcs)

debug.objs			=	$(addprefix $(dir $(BUILD_DIR)$(debug.name)/), $(debug.srcs:.c=.o))	\
						$(addprefix $(dir $(BUILD_DIR)$(debug.name)/), $(debug.main:.c=.o))

debug.main 			=	$(release.main)

debug.cflags		=	-g3

debug.ldflags		=

# COMPILATION

BUILD_DIR			=	build/

HEADERS				=	-I ./include

CFLAGS				=	-Wall -Wextra $(HEADERS) -std=gnu11

LDFLAGS				=	-lcrypto

CC					=	gcc

all: $(release.name)

#	MANDATORY

$(release.name): $(LIBS)	$(release.objs)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(release.cflags) $(release.ldflags)

$(unit_tests.name): $(LIBS) $(unit_tests.objs)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(unit_tests.cflags) $(unit_tests.ldflags)

tests_compile: $(LIB_TEST_COMPILE) $(unit_tests.name)

tests_run: tests_compile $(LIB_TESTS_RUN)
	./$(unit_tests.name) -j1 --verbose --full-stats

clean:
	rm -rf $(BUILD_DIR)

fclean:	clean $(LIB_FCLEAN)
	rm -rf $(release.name) $(debug.name) $(unit_tests.name) $(DOCS_DIR)

re: fclean all

# Create .o

$(BUILD_DIR)$(release.name)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(release.cflags) $< -c -o $@

$(BUILD_DIR)$(debug.name)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(debug.cflags) $< -c -o $@

$(BUILD_DIR)$(unit_tests.name)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(unit_tests.cflags) $< -c -o $@

# Develloper tools
$(debug.name): $(LIBS) $(debug.objs)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(debug.cflags) $(debug.ldflags)

debug: $(debug.name)

