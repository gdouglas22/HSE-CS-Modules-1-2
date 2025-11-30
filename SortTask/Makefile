CXX ?= g++
CPPFLAGS ?= -MMD -MP
CXXFLAGS ?= -std=c++17 -Wall -Wextra
LDFLAGS ?=
LDLIBS ?=

SRCDIR := src
TESTDIR := tests
INCDIR := include
TESTINCDIR := tests
BUILDDIR := build
APP_OBJDIR := $(BUILDDIR)/app
TEST_OBJDIR := $(BUILDDIR)/tests
BINDIR := bin

CPPFLAGS += -I$(INCDIR) -I$(TESTINCDIR)

APP := app
TEST_BIN := tests

COMMON_SOURCES := io.cpp index.cpp search.cpp edit.cpp data.cpp messages.cpp validation.cpp types.cpp
APP_SOURCES := $(addprefix $(SRCDIR)/,main.cpp $(COMMON_SOURCES))
TEST_SOURCES := $(addprefix $(SRCDIR)/,$(COMMON_SOURCES)) $(TESTDIR)/tests.cpp

APP_OBJECTS := $(addprefix $(APP_OBJDIR)/,$(notdir $(APP_SOURCES:.cpp=.o)))
TEST_OBJECTS := $(addprefix $(TEST_OBJDIR)/,$(notdir $(TEST_SOURCES:.cpp=.o)))

APP_TARGET := $(BINDIR)/$(APP)
TEST_TARGET := $(BINDIR)/$(TEST_BIN)

DEPS := $(APP_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

vpath %.cpp $(SRCDIR) $(TESTDIR)

.PHONY: all clean app tests
.PHONY: coverage

all: $(APP_TARGET) $(TEST_TARGET)

app: $(APP_TARGET)
tests: $(TEST_TARGET)

$(APP_TARGET): $(APP_OBJECTS) | $(BINDIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(TEST_TARGET): $(TEST_OBJECTS) | $(BINDIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(APP_OBJDIR)/%.o: %.cpp | $(APP_OBJDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJDIR)/%.o: %.cpp | $(TEST_OBJDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BINDIR) $(APP_OBJDIR) $(TEST_OBJDIR):
	@mkdir -p $@

clean:
	$(RM) -r $(BINDIR) $(BUILDDIR) *.gcda *.gcno *.gcov *.html

coverage:
	$(MAKE) clean
	$(MAKE) tests CXXFLAGS+="--coverage" LDFLAGS+="--coverage"
	cd $(BINDIR) && ./tests
	if command -v gcovr >/dev/null 2>&1; then \
		gcovr -r . --object-directory $(TEST_OBJDIR) --html --html-details -o coverage.html && open coverage.html; \
		echo "Coverage report: coverage.html"; \
	else \
		echo "gcovr not found; install gcovr to generate HTML coverage"; \
	fi

-include $(DEPS)
