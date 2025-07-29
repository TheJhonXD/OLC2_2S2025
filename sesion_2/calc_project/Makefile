SRC     := src
BUILD   := build

CC      := gcc
BISON   := bison
FLEX    := flex
CFLAGS  := -I$(SRC) -Wall -Wextra

# Usa siempre rm -rf y mkdir -p
RM      := rm -rf $(BUILD)
MKDIR   := mkdir -p $(BUILD)

.DEFAULT_GOAL := all

all: $(BUILD)/calc

# 1) Crear carpeta build/
$(BUILD):
	$(MKDIR)

# 2) Generar parser
$(BUILD)/parser.tab.c $(BUILD)/parser.tab.h $(BUILD)/parser.output: \
    $(SRC)/parser.y | $(BUILD)
	$(BISON) -d -v --locations -o $@ $<

# 3) Generar lexer
$(BUILD)/lex.yy.c: $(SRC)/lexer.l $(BUILD)/parser.tab.h | $(BUILD)
	$(FLEX) -o $@ $<

# 4) Compilar ejecutable
$(BUILD)/calc: $(BUILD)/parser.tab.c $(BUILD)/lex.yy.c $(SRC)/main.c | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM)

.PHONY: all clean
