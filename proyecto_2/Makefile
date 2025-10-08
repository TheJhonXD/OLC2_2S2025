# ==== Config ====
BUILD   := build
CC      := gcc
BISON   := bison
FLEX    := flex

# ARM64 tools
AS_ARM  := aarch64-linux-gnu-as
LD_ARM  := aarch64-linux-gnu-ld
QEMU    := qemu-aarch64

CFLAGS  := -I$(BUILD) -Isrc -Isrc/headers -Wall -Wextra -g
LDFLAGS :=

PARSER := src/parser/parser.y
LEXER  := src/parser/lexer.l

# Todos los .c propios (no incluye los generados por bison/flex)
SRC_DIRS := src src/entorno src/ast src/ast/expresiones src/ast/sentencias src/generator
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
SRCS += main.c   

.PHONY: all run clean asm exec

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

# Compilar ARM64: make asm FILE=program.txt
asm: $(BUILD)/calc
	@./$(BUILD)/calc $(FILE)
	@$(AS_ARM) -o $(basename $(FILE)).o $(basename $(FILE)).s
	@$(LD_ARM) -o $(basename $(FILE)) $(basename $(FILE)).o

# Ejecutar ARM64: make exec FILE=program.txt
exec: asm
	@$(QEMU) ./$(basename $(FILE))

clean:
	@rm -rf $(BUILD) *.o *.s program output test_if test_completo
