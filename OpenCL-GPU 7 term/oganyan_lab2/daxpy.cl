__kernel void daxpy(int n, double a, __global double *x, int incx, __global double *y, int incy) {
    int i = get_global_id(0);
    if ((i * incy < n) && (i * incx < n)) {
        y[i * incy] += a * x[i * incx];
    }
}