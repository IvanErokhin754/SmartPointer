CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -g -O0
SANITIZE_FLAGS = -fsanitize=address -g

SRC = src/main.cpp #src/UI.cpp
APP = main


TEST_SRC = tests/test_subtyping.cpp \
		   tests/test_SmrtPtr_array.cpp \
		   tests/test_SmrtPtr_single.cpp
	

TEST_BIN = tests_run

GTEST_FLAGS = $(shell pkg-config --cflags --libs gtest_main)

all: $(APP)

$(APP): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(APP)

$(TEST_BIN): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(GTEST_FLAGS) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

asan: CXXFLAGS += $(SANITIZE_FLAGS)
asan: clean $(APP)

asan-test: CXXFLAGS += $(SANITIZE_FLAGS)
asan-test: clean $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(APP) $(TEST_BIN)

.PHONY: all test asan asan-test clean