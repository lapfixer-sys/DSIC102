#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "fractal_parallel.h"
#include "mandelbrot_asm.h"

typedef struct {
    int start_y;
    int end_y;
    int width;
    int height;
    int max_iter;
    unsigned char* image;
} ThreadData;

static void* render_section(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (int y = data->start_y; y < data->end_y; y++) {
        for (int x = 0; x < data->width; x++) {

            double real = (x - data->width / 2.0) * 4.0 / data->width;
            double imag = (y - data->height / 2.0) * 4.0 / data->height;

            int iter = mandelbrot_iter_asm(real, imag, data->max_iter);

            int color = (iter * 255) / data->max_iter;
            int index = (y * data->width + x) * 3;

            data->image[index]     = (unsigned char)color;
            data->image[index + 1] = (unsigned char)(color / 2);
            data->image[index + 2] = (unsigned char)(255 - color);
        }
    }

    return NULL;
}

void generate_mandelbrot_parallel(
    const char* filename,
    int width,
    int height,
    int max_iter,
    int num_threads
) {
    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);
    ThreadData* thread_data = malloc(sizeof(ThreadData) * num_threads);
    unsigned char* image = malloc(width * height * 3);

    if (!threads || !thread_data || !image) {
        printf("Error: no se pudo reservar memoria.\n");
        free(threads);
        free(thread_data);
        free(image);
        return;
    }

    int rows_per_thread = height / num_threads;

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start_y = i * rows_per_thread;
        thread_data[i].end_y = (i == num_threads - 1)
            ? height
            : (i + 1) * rows_per_thread;

        thread_data[i].width = width;
        thread_data[i].height = height;
        thread_data[i].max_iter = max_iter;
        thread_data[i].image = image;

        pthread_create(&threads[i], NULL, render_section, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    FILE* fp = fopen(filename, "wb");

    if (!fp) {
        printf("Error: no se pudo crear el archivo %s\n", filename);
        free(threads);
        free(thread_data);
        free(image);
        return;
    }

    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    fwrite(image, 1, width * height * 3, fp);

    fclose(fp);

    free(threads);
    free(thread_data);
    free(image);
}