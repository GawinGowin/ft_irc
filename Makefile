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
TESTS += $(shell find tests/gtests -name '*.h' -o -name '*.hpp' -o -name '*.c' -o -name '*.cpp')

CXX := c++
COMMON_FLAGS := -Wall -Wextra -Werror -std=c++98 -MMD -MP -I$(BASE_PKG_DIR)
CFLAGS := -O3
LFALGS := 
DFLAGS := -fdiagnostics-color=always -g3 -fsanitize=address

OBJS := $(SOURCE:.cpp=.o)
DOBJS := $(SOURCE:.cpp=_d.o)
DEP = $(OBJS:.o=.d)
DDEP = $(DOBJS:.o=.d)

COV_INFO = coverage.info
TEST_LOG = build/tests/gtests/Testing/Temporary/LastTest.log

ORGANIZATION := 42tokyo-b9
PROJECT := ft_irc

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(COMMON_FLAGS) $(CFLAGS) $^ $(LFALGS) -o $@

.PHONY: debug
debug: $(DNAME)

$(DNAME): $(DOBJS)
	$(CXX) $(COMMON_FLAGS) $(DFLAGS) $^ $(LFALGS) -o $@

-include $(DEP)
%.o: %.cpp
	$(CXX) $(COMMON_FLAGS) $(CFLAGS) -c $< -o $@

-include $(DDEP)
%_d.o: %.cpp
	$(CXX) $(COMMON_FLAGS) $(DFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(DOBJS) $(DEP) $(DDEP) $(COV_INFO)

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(DNAME) $(DNAME).debug
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
	cd $(BUILD_DIR)/tests/gtests && make test

.PHONY: dif
dif: debug
	rm -f $(DNAME).debug
	objcopy --only-keep-debug $(DNAME) $(DNAME).debug
	objcopy --strip-debug --strip-unneeded $(DNAME)
	objcopy --add-gnu-debuglink=$(DNAME).debug $(DNAME)
	sentry-cli dif upload -o $(ORGANIZATION) -p $(PROJECT) --wait $(DNAME).debug --include-sources $(DOBJS) 

.PHONY: login
login: 
	env `cat .env.sentry | xargs` sh -c 'sentry-cli login --auth-token $$SENTRY_CLI_AUTH_TOKEN'

.PHONY: locust
locust: debug
	locust -f tools/locustfile.py

$(COV_INFO): $(TEST_LOG)
	lcov --capture --directory . --output-file $(COV_INFO)
	lcov --remove $(COV_INFO) '/usr/*' --output-file $(COV_INFO) # filter system-files

.PHONY: cov
cov: $(COV_INFO)
	lcov --list $(COV_INFO) # debug info
