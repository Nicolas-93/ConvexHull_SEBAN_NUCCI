DEBUG_CVH_CLEANING=no

.PHONY: clean

# Définition des variables
CC=clang
CFLAGS=-fdiagnostics-color=always -g -Wall -pedantic -std=c17
INCLUDE=-Iinclude
LIBS=-lMLV -lm
SRC_DIR=src
BUILD_DIR=build
EXEC=demo

SOURCES=$(wildcard $(SRC_DIR)/*.c)
# "Substitution reference" / patsubst
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
$(info $(OBJECTS))

ifeq ($(DEBUG_CVH_CLEANING), yes)
	CFLAGS:=$(CFLAGS) -DDEBUG_CVH_CLEANING
endif

all: $(EXEC)

# Cible par défaut : l'exécutable demo
demo: $(OBJECTS)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $(BUILD_DIR)/$@ $(LIBS)

# Création des fichiers objets à partir des fichiers sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/demo
