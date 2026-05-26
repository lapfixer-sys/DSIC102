#ifndef FRACTAL_PARALLEL_H
#define FRACTAL_PARALLEL_H

void generate_mandelbrot_parallel(
    const char* filename,
    int width,
    int height,
    int max_iter,
    int num_threads
);

#endif