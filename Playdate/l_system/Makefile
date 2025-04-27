##############################################################################
# Basic Makefile for compiling Lua for Playdate.
#
# The game will be compiled using the Playdate SDK compiler.
# The source code is expected to be in a folder called "source".
# The compiled game will be placed in a folder called "builds".
# The compiled game will have the same name as the folder where the Makefile
#
# Usage:
#  make        - Compiles the game.
#  make run    - Compiles the game and runs it in the Playdate Simulator.
#  make clean  - Deletes the compiled game in /builds/game_name.pdx
###############################################################################

GAME_NAME := $(notdir $(CURDIR))

# This assumes that the source code is in a folder called "source" and the
# builds will be placed in a folder called "builds".
SOURCE_FOLDER := source
BUILDS_FOLDER := builds

# Playdate SDK compiler
COMPILER := pdc

OUTPUT_FILE := $(BUILDS_FOLDER)/$(GAME_NAME).pdx

# Default target
all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(SOURCE_FOLDER)/*
	$(COMPILER) $(SOURCE_FOLDER) $(OUTPUT_FILE)

run: $(OUTPUT_FILE)
	PlaydateSimulator $(OUTPUT_FILE)

clean claen clena:
	rm -rf $(OUTPUT_FILE)
