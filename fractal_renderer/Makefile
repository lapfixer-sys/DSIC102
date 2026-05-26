CC = clang
ASM = nasm

CFLAGS = -O2
LDFLAGS = -pthread

SRC = src/main.c src/fractal.c src/fractal_parallel.c
ASM_SRC = asm/mandelbrot_asm.asm
ASM_OBJ = asm/mandelbrot_asm.o

OUT = fractal_renderer.exe

all: $(ASM_OBJ)
	$(CC) $(CFLAGS) $(SRC) $(ASM_OBJ) -o $(OUT) $(LDFLAGS)

$(ASM_OBJ): $(ASM_SRC)
	$(ASM) -f win64 $(ASM_SRC) -o $(ASM_OBJ)

clean:
	rm -f $(OUT) $(ASM_OBJ)