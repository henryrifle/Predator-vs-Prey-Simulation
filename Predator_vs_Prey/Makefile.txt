CXX:=g++
STD:=-std=c++20
CFLAGS:=-Wall -Werror -Wextra -Weffc++ -pedantic -fPIC
DEFINITIONS:= -DFMT_HEADER_ONLY
OPTIMIZATION_FLAGS:=-O3 -ffast-math

PYTHON:=python3

SRC:=$(wildcard src/*.cpp)
INC:=$(wildcard include/*.h)
OBJ:=$(SRC:src/%.cpp=obj/%.o)
LIBOBJ:=$(subst obj/main.o,,${OBJ})
OBJ:=$(subst obj/dashboard_model.o,,${OBJ})

DEP:=$(SRC:src/%.cpp=dep/%.d)
TRGT_EXE:=bin/main.exe
TGRT_DASHBOARD:=dashboard/model$(shell $(PYTHON)-config --extension-suffix)

ifeq ($(SRC),)
$(error No source files found, there is nothing to build!)
endif

INCLUDES:=-I../include -Iinclude

PYTHON_INCLUDES:=$(shell $(PYTHON)-config --includes)
PYTHON_LIB_DIR:=$(shell $(PYTHON)-config --prefix)/lib
PYTHON_LIBS:=$(shell $(PYTHON)-config --libs --embed)
PYTHON_LD_FLAGS:=-Wl,-rpath,$(PYTHON_LIB_DIR) -L$(PYTHON_LIB_DIR) $(PYTHON_LIBS)

$(TRGT_EXE): setup setup_bin $(OBJ)
	$(CXX) $(OBJ) $(PYTHON_LD_FLAGS) -o $@

dashboard: setup $(LIBOBJ) obj/dashboard_model.o
	$(CXX) $(LIBOBJ) obj/dashboard_model.o $(PYTHON_LD_FLAGS) -shared -o $(TGRT_DASHBOARD)
	panel serve dashboard

setup:
	@mkdir -p obj
	@mkdir -p dep

setup_bin:
	@mkdir -p bin

-include $(DEP)

obj/dashboard_model.o: dashboard/dashboard_model.cpp $(INC)
	$(CXX) $(STD) $(OPTIMIZATION_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) $(PYTHON_INCLUDES) -MF $(<:dashboard/%.cpp=dep/%.d) -MMD -c $< -o $@

obj/%.o: src/%.cpp $(INC)
	$(CXX) $(STD) $(OPTIMIZATION_FLAGS) $(CFLAGS) $(DEFINITIONS) $(INCLUDES) $(PYTHON_INCLUDES) -MF $(<:src/%.cpp=dep/%.d) -MMD -c $< -o $@

clean:
	rm -rf obj dep bin lib $(TRGT_EXE) $(TGRT_DASHBOARD) test_output __pycache__ $(HTML)

.PHONY:all dashboard setup clean test
