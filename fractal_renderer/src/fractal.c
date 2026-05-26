#include <stdio.h>
#include "fractal.h"
#include "mandelbrot_asm.h"

void generate_mandelbrot(
    const char* filename,
    int width,
    int height,
    int max_iter
) {

    FILE* fp = fopen(filename, "w");

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            double real = (x - width / 2.0) * 4.0 / width;
            double imag = (y - height / 2.0) * 4.0 / height;

            double zr = 0.0;
            int iter = mandelbrot_iter_asm(real, imag, max_iter);

            int color = (iter * 255) / max_iter;

            fprintf(fp, "%d %d %d ",
                color,
                color / 2,
                255 - color
            );
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}