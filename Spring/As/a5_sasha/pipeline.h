#ifndef __PIPELINE_H__
#define __PIPELINE_H__

/**
 * Adds array[0], array[1], ..., array[size - 1] as
 * a program arguments to the end of the sequence of programs arguments.
 * Initially the sequence is empty.
 * Terminating null argument is not passed here.
 * Returns 0 if success, non 0 otherwise.
 */
int add_to_pipeline(const char *array[], int size);

/**
 * Runs the pipeline of the sequence of program arguments.
 * Waits for ending all children processes.
 * Returns 0 if success, non 0 otherwise.
 */
int run_pipeline(void);

/**
 * Frees any resources which is created by pipeline.
 * Must be called after the completion of all operations.
 */
void free_pipeline(void);

#endif
