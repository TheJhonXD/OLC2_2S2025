# Fases de un compilador

## Fase de análisis (Frontend)

### Análisis Léxico

Su objetivo es identificar que los caracteres pertenezcan al lenguaje.

El análisis léxico es el paso que:

1. Lee el texto carácter por carácter
2. Agrupa esos caracteres en `palabras` significativas, llamadas `tokens`.
3. Etiqueta cada token con su tipo (palabra reservada, nombre de variable, número, símbolo, etc.).
4. Descarta lo que no afecta al código (espacios, saltos de línea, comentarios).
5. Reporta errores muy básicos, por ejemplo: un carácter que no existe en el lenguaje.

---

### Ejemplo

Código de entrada (lenguaje `c`):

```c
int x = 5; // Es léxicamente correcto

; 10 = x int // Es léxicamente correcto

int $x = "hola mndo"; // Error léxico
```

### Análisis Sintáctico

Analiza el orden de los tokens generados en el análisis léxico.

Después de que el lexer convierte la entrada en tokens, el análisis sintáctico es el paso que:

1. Lee los tokens en orden
2. Comprueba que la secuencia siga las reglas gramaticales del lenguaje (la “sintaxis”).
3. Construye un árbol de sintaxis abstracta (AST – _Abstract Syntax Tree_) que representa la forma en que se relacionan los elementos.
4. Detecta y reporta errores, como paréntesis faltantes, llaves desbalanceadas, operador fuera de lugar, etc.

---

### Ejemplo

Código de entrada (lenguaje `c`):

```c
int x = 5; // Es sintácticamente correcto

; 10 = x int // Error de sintáxis

int x = 255 // Error de sintáxis
```

### Análisis semántico

Una vez que el parser construye el árbol de sintaxis (AST), el análisis semántico es la fase que:

1. Comprueba el “significado” correcto del programa
2. Verifica reglas de tipos y compatibilidad, que no se asigne un texto a una variable numérica, por ejemplo.
3. Revisa el ámbito (scope): ninguna variable sale de su alcance, no hay redefiniciones conflictivas, etc.
4. Detecta otros errores lógicos tempranos: número incorrecto de argumentos en una llamada, etc.

---

### Ejemplo

Código sintácticamente correcto:

Código de entrada (lenguaje `c`):

```c
int x = 5; // Es semanticamente correcto

int y = "hola"; // Error semántico (incompatibilidad de tipos)

```

## Fase de síntesis (Backend)
