# Directories
# -----------
SRC_DIR = src
OBJ_DIR = build

# Files
# -----
OUT = a.out
src = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp)
_temp = $(src:$(SRC_DIR)/%=$(OBJ_DIR)/%)
obj = $(_temp:%.cpp=%.o)
dep = $(obj:.o=.d)

# Compiler
# --------
CC = g++
LDFLAGS = -lGL -lglfw -lGLEW
CFLAGS = -Wall -Wextra -g -std=c++14

# Rules
# -----
all: program

program: $(obj)
	mkdir -p $(@D)
	$(CC) -o $(OUT) $^ $(LDFLAGS) $(CFLAGS)
include $(dep) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: cleanobj
cleanobj:
	rm -f $(obj)

.PHONY: cleandep
cleandep:
	rm -f $(dep)

.PHONY: clean
clean: cleanobj cleandep
	rm -f $(OUT)

.PHONY: run
run: all
	./$(OUT)