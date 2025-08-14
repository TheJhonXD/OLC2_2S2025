# ==== Config ====
BUILD   := build
CC      := gcc
BISON   := bison           # en Windows con win_flex_bison usa: win_bison
FLEX    := flex            # en Windows con win_flex_bison usa: win_flex

# Incluye donde se generan headers del parser y todo src/
CFLAGS  := -I$(BUILD) -Isrc -Isrc/headers -Wall -Wextra -g
LDFLAGS :=

# Rutas reales según tu árbol
PARSER := src/parser/parser.y
LEXER  := src/parser/lexer.l

# Todos los .c propios (no incluye los generados por bison/flex)
SRC_DIRS := src src/entorno src/ast src/ast/expresiones src/ast/sentencias
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
SRCS += main.c   

.PHONY: all run clean

all: $(BUILD)/calc

# 1) Carpeta build
$(BUILD):
	@mkdir -p $(BUILD)

# 2) Bison -> parser.tab.c/.h/.output (a build/)
$(BUILD)/parser.tab.c $(BUILD)/parser.tab.h $(BUILD)/parser.output: $(PARSER) | $(BUILD)
	$(BISON) -d -v -o $(BUILD)/parser.tab.c $<

# 3) Flex -> lex.yy.c (depende del .h del parser)
$(BUILD)/lex.yy.c: $(LEXER) $(BUILD)/parser.tab.h | $(BUILD)
	$(FLEX) -o $@ $<

# 4) Compilar y enlazar todo
$(BUILD)/calc: $(BUILD)/parser.tab.c $(BUILD)/lex.yy.c $(SRCS) | $(BUILD)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Ejecutar: make run FILE=archivo.txt
run: $(BUILD)/calc
	@./$(BUILD)/calc $(FILE)

clean:
	@rm -rf $(BUILD)
