NAME := ft_irc
DNAME := $(NAME)_debug
BUILD_DIR = ./build
BASE_PKG_DIR = ./pkg

SOURCE =
SOURCE += cmd/ft_irc/main.cpp
SOURCE += $(shell find $(BASE_PKG_DIR)/application $(BASE_PKG_DIR)/domain $(BASE_PKG_DIR)/infra $(BASE_PKG_DIR)/presentation -name '*.cpp')

HEADER =
HEADER += $(shell find $(BASE_PKG_DIR)/application $(BASE_PKG_DIR)/domain $(BASE_PKG_DIR)/infra $(BASE_PKG_DIR)/presentation -name '*.h' -o -name '*.hpp')

TESTS =
TESTS += $(shell find $(BASE_PKG_DIR)/tests -name '*.h' -o -name '*.hpp' -o -name '*.c' -o -name '*.cpp')

CXX := c++
CFLAGS := -Wall -Wextra -Werror -std=c++98 -MMD -MP -I$(BASE_PKG_DIR)
LFALGS := -lsentry -Wl,-rpath,/usr/local/lib
DFLAGS := -fdiagnostics-color=always -g3 -fsanitize=address

OBJS := $(SOURCE:.cpp=.o)
DOBJS := $(SOURCE:.cpp=_d.o)
DEP = $(OBJS:.o=.d)
DDEP = $(DOBJS:.o=.d)

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LFALGS) -o $@

.PHONY: debug
debug: $(DNAME)

$(DNAME): $(DOBJS)
	$(CXX) $(CFLAGS) $(DFLAGS) $^ $(LFALGS) -o $@

-include $(DEP)
%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

-include $(DDEP)
%_d.o: %.cpp
	$(CXX) $(CFLAGS) $(DFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(DOBJS) $(DEP) $(DDEP)

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(DNAME)
	rm -rf $(BUILD_DIR)

.PHONY: re
re: fclean all

.PHONY: fmt
fmt:
	clang-format -i --style=file $(SOURCE) $(HEADER) $(TESTS)

.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..  && make

.PHONY: test
test: build
	cd $(BUILD_DIR)/pkg/tests && make test
