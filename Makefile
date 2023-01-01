DEBUG?=no
DEBUG_CVH_CLEANING?=no

# Définition des variables
CC=clang
BUILD_DIR=build
SRC_DIR=src
INCLUDE=-Iinclude
LIBS=-lMLV -lm
CFLAGS=-fdiagnostics-color=always -Wall -pedantic -std=c17
SOURCES=$(wildcard $(SRC_DIR)/*.c)
# "Substitution reference" / patsubst
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# $(info $(OBJECTS))

ifeq ($(DEBUG), yes)
	CFLAGS += -g
endif
ifeq ($(DEBUG_CVH_CLEANING), yes)
	CFLAGS += -DDEBUG_CVH_CLEANING
endif

all: demo

# Cible par défaut : l'exécutable demo
demo: $(OBJECTS)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $(BUILD_DIR)/$@ $(LIBS)

# Création des fichiers objets à partir des fichiers sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/demo

.PHONY: clean all
