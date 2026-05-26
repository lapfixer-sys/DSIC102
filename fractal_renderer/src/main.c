#include <stdio.h>
#include <time.h>

#include "fractal.h"
#include "fractal_parallel.h"

int main() {
    int width = 1920;
    int height = 1080;
    int max_iter = 1000;
    int num_threads = 4;

    clock_t start_seq = clock();

    generate_mandelbrot(
        "output/mandelbrot_asm_secuencial.ppm",
        width,
        height,
        max_iter
    );

    clock_t end_seq = clock();

    double time_seq =
        (double)(end_seq - start_seq) / CLOCKS_PER_SEC;

    printf("Render secuencial C/ASM completado.\n");
    printf("Tiempo secuencial: %.2f segundos\n\n", time_seq);

    clock_t start_par = clock();

    generate_mandelbrot_parallel(
        "output/mandelbrot_asm_parallel.ppm",
        width,
        height,
        max_iter,
        num_threads
    );

    clock_t end_par = clock();

    double time_par =
        (double)(end_par - start_par) / CLOCKS_PER_SEC;

    printf("Render paralelo C/ASM completado.\n");
    printf("Hilos utilizados: %d\n", num_threads);
    printf("Tiempo paralelo: %.2f segundos\n", time_par);

    if (time_par > 0) {
        printf("Speedup preliminar: %.2fx\n", time_seq / time_par);
    }

    return 0;
}