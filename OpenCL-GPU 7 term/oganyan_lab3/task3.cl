__kernel void matrix_multiply(__global float *a, __global float *b, __global float *c, int n)
{
    int row = get_global_id(1);
    int column = get_global_id(0);
    float localResult = 0;
    for (int i = 0; i < n; ++i) {
        localResult += a[row * n + i] * b[column + n * i];
    }
    c[n * row + column] = localResult;
}

__kernel void matrix_multiply_block(__global float *a, __global float *b, __global float *c, int n)
{

    __local float a_block[16][16];
    __local float b_block[16][16];
    int row = get_global_id(1);
    int column = get_global_id(0);
    int local_row = get_local_id(1);
    int local_column = get_local_id(0);
    int blocks_count = n / 16;
    float localResult = 0;
    for (int i = 0; i < blocks_count; ++i)
    {
        a_block[local_row][local_column] = a[row * n + 16 * i + local_column];
        b_block[local_row][local_column] = b[(16 * i + local_row) * n + column];
        barrier(CLK_LOCAL_MEM_FENCE);
        for (int j = 0; j < 16; ++j)
        {
            localResult += a_block[local_row][j] * b_block[j][local_column];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    c[n * row + column] = localResult;
}


__kernel void matrix_multiply_image(__read_only image2d_t a, __read_only image2d_t b, __write_only image2d_t c, int n)
{
    __local float a_block[16][16];
    __local float b_block[16][16];
    int row = get_global_id(1);
    int column = get_global_id(0);
    int local_row = get_local_id(1);
    int local_column = get_local_id(0);
    int blocks_count = n / 16;
    float localResult = 0;
    for (int i = 0; i < blocks_count; ++i)
    {
        a_block[local_row][local_column] = read_imagef(a, (int2)(16 * i + local_column, row)).x;
        b_block[local_row][local_column] = read_imagef(b, (int2)(column, 16 * i + local_row)).x;
        barrier(CLK_LOCAL_MEM_FENCE);
        for (int j = 0; j < 16; ++j)
        {
            localResult += a_block[local_row][j] * b_block[j][local_column];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    write_imagef(c, (int2)(column, row), localResult);
}
