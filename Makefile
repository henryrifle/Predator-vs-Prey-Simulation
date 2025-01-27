CXX := g++
STD := -std=c++17
CFLAGS := -Wall -Werror -Wextra -pedantic
DEFINITIONS := -DFMT_HEADER_ONLY
OPTIMIZATION_FLAGS := -O3

PYTHON := python3.10
PYTHON_CONFIG := python3.10-config

SRC := $(wildcard src/*.cpp)
INC := $(wildcard include/*.h)
OBJ := $(SRC:src/%.cpp=obj/%.o)

INCLUDES := -I include -I/opt/homebrew/include \
           $(shell $(PYTHON_CONFIG) --includes) \
           -I/opt/homebrew/include/pybind11

LDFLAGS := -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lfmt

PYTHON_INCLUDES := $(shell $(PYTHON_CONFIG) --includes)
PYTHON_LIB_DIR := $(shell $(PYTHON_CONFIG) --prefix)/lib
PYTHON_LIBS := $(shell $(PYTHON_CONFIG) --ldflags) -lpython3.10
PYTHON_LD_FLAGS := -Wl,-rpath,$(PYTHON_LIB_DIR) -L$(PYTHON_LIB_DIR) $(PYTHON_LIBS)

TARGET := predator_prey_simulation
DASHBOARD_TARGET := dashboard/model$(shell $(PYTHON_CONFIG) --extension-suffix)

all: $(TARGET)

$(TARGET): setup $(OBJ) obj/main.o
	$(CXX) $(OBJ) obj/main.o $(LDFLAGS) -o $@

dashboard: setup $(OBJ) obj/dashboard_model.o
	$(CXX) -shared -undefined dynamic_lookup \
		$(OBJ) obj/dashboard_model.o \
		$(PYTHON_LD_FLAGS) \
		-o $(DASHBOARD_TARGET)
	PYTHONPATH=$(PWD)/dashboard $(PYTHON) -m panel serve dashboard/dashboard.py

setup:
	@mkdir -p obj
	@mkdir -p bin
	@mkdir -p dashboard

obj/%.o: src/%.cpp $(INC)
	$(CXX) $(STD) $(OPTIMIZATION_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) -fPIC -c $< -o $@

obj/main.o: main.cpp $(INC)
	$(CXX) $(STD) $(OPTIMIZATION_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) -c $< -o $@

obj/dashboard_model.o: dashboard/dashboard_model.cpp $(INC)
	$(CXX) $(STD) $(OPTIMIZATION_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) -fPIC -c $< -o $@

clean:
	rm -rf obj bin $(TARGET) $(DASHBOARD_TARGET)

.PHONY: clean setup dashboard