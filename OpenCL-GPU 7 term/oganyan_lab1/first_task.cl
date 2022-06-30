 __kernel void TaskOneOganyan(__global const int *input, __global int *output, const unsigned int count_of_buffer) {
        int global_index = get_global_id(0);
        int block = get_group_id(0);
        int thread = get_local_id(0);
        if (count_of_buffer > global_index) {
            printf("I am from %d block, %d thread, (global index: %d) \n", block, thread, global_index);
            output[global_index] = input[global_index] + global_index;
        }
    }