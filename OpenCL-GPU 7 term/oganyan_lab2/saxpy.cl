__kernel void saxpy(int n, float a, __global float *x, int incx, __global float *y, int incy) {
    int i = get_global_id(0);
    if ((i * incy < n) && (i * incx < n)) {
        y[i * incy] += a * x[i * incx];
    }
}