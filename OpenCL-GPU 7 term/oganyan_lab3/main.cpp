#include <CL/cl.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <ctime>

void CalculateCLImage(float *data_X, float *data_Y, float *result, int count,
                               int n, int m, cl_device_id device, cl_platform_id &platform) {

    const size_t size = count;
    cl_int error_code;

    cl_context_properties properties[3] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties) (platform), 0
    };

    cl_context context = clCreateContextFromType(properties, CL_DEVICE_TYPE_ALL,
                                                 NULL, NULL, &error_code);


    if (error_code != CL_SUCCESS) {
        std::cerr << "Context error";
    }

    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error_code);

    if (error_code != CL_SUCCESS) {
        std::cerr << "Queue error";
    }

    std::ifstream source_kernel("../../oganyan_lab3/task3.cl", std::ios_base::in);

    std::string text;

    char current_letter;
    if (source_kernel.is_open()) {
        while (source_kernel.get(current_letter)) {
            text += current_letter;
        }
    }

    const size_t length = text.size();
    const char *source = text.c_str();

    source_kernel.close();


    cl_program program = clCreateProgramWithSource(context, 1, &source, &length, &error_code);
    if (error_code != CL_SUCCESS) {
        std::cerr << "Create Programm error";
    }
    error_code = clBuildProgram(program, 1, &device, nullptr, nullptr, NULL);
    if (error_code != CL_SUCCESS) {
        std::cerr << "Build Program error";
    }
    cl_kernel kernel = clCreateKernel(program, "matrix_multiply_image", &error_code);
    if (error_code != CL_SUCCESS) {
        std::cerr << "Kernel error";
    }

    cl_image_format imageFormat;
    imageFormat.image_channel_order = CL_R;
    imageFormat.image_channel_data_type = CL_FLOAT;

    auto aImage = clCreateImage2D(context, CL_MEM_READ_ONLY, &imageFormat, static_cast<size_t>(n),
                                  static_cast<size_t>(n), 0, nullptr, nullptr);
    auto bImage = clCreateImage2D(context, CL_MEM_READ_ONLY, &imageFormat, static_cast<size_t>(n),
                                  static_cast<size_t>(n), 0, nullptr, nullptr);
    auto cImage = clCreateImage2D(context, CL_MEM_WRITE_ONLY, &imageFormat, static_cast<size_t>(n),
                                  static_cast<size_t>(n), 0, nullptr, nullptr);
    size_t origin[] = {0, 0, 0};
    {
        size_t region[] = {static_cast<size_t>(n), static_cast<size_t>(n), 1};
        clEnqueueWriteImage(queue, aImage, CL_TRUE, origin, region, 0, 0, data_X, 0, nullptr, nullptr);
    }
    {
        size_t region[] = {static_cast<size_t>(n), static_cast<size_t>(n), 1};
        clEnqueueWriteImage(queue, bImage, CL_TRUE, origin, region, 0, 0, data_Y, 0, nullptr, nullptr);
    }

    // Setup arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &aImage);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bImage);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &cImage);
    clSetKernelArg(kernel, 3, sizeof(int), &n);

    size_t group;
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &group, nullptr);

    const size_t globalWorkSize[] = {static_cast<size_t>(n), static_cast<size_t>(n)};
    const size_t localWorkSize[] = {16, 16};
    double start = omp_get_wtime();
    clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, globalWorkSize, localWorkSize, 0, nullptr, nullptr);
    clFinish(queue);
    double end = omp_get_wtime() - start;
    std::cout<<"Image: " <<end<<std::endl;

    {
        size_t region[] = {static_cast<size_t>(n), static_cast<size_t>(n), 1};
        clEnqueueReadImage(queue, cImage, CL_TRUE, origin, region, 0, 0, result, 0, nullptr, nullptr);
    }

    clReleaseMemObject(aImage);
    clReleaseMemObject(bImage);
    clReleaseMemObject(cImage);
    clReleaseKernel(kernel);
    clReleaseProgram(program);

    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}


void CalculateCL(void *data_X, void *data_Y, void *result, int count, int n, int m, int type,
                 cl_device_id &device, cl_platform_id &platform) {
    const size_t size = count;
    cl_int error_code;

    cl_context_properties properties[3] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties) (platform), 0
    };

    cl_context context = clCreateContextFromType(properties, CL_DEVICE_TYPE_ALL,
                                                 NULL, NULL, &error_code);


    if (error_code != CL_SUCCESS) {
        std::cerr << "Context error";
    }

    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error_code);

    if (error_code != CL_SUCCESS) {
        std::cerr << "Queue error";
    }

    std::ifstream source_kernel("../../oganyan_lab3/task3.cl", std::ios_base::in);

    std::string text;

    char current_letter;
    if (source_kernel.is_open()) {
        while (source_kernel.get(current_letter)) {
            text += current_letter;
        }
    }

    const size_t length = text.size();
    const char *source = text.c_str();

    source_kernel.close();


    cl_program program = clCreateProgramWithSource(context, 1, &source, &length,
                                                   &error_code);


    if (error_code != CL_SUCCESS) {
        std::cerr << "Create error: " << error_code;
    }

    error_code = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    if (error_code != CL_SUCCESS) {
        std::cerr << "Build error:" << error_code;
    }
    std::string kernel_name;
    if (type == 0) {
        kernel_name = "matrix_multiply";
    } else if (type == 1) {
        kernel_name = "matrix_multiply_block";
    }

    cl_kernel kernel = clCreateKernel(program, kernel_name.c_str(), &error_code);
    if (error_code != CL_SUCCESS) {
        std::cerr << "Kernel creating fails" << error_code;
    }

    cl_mem input_X = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * size, NULL, NULL);
    cl_mem input_Y = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * size, NULL, NULL);
    cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * size, NULL, NULL);

    clEnqueueWriteBuffer(queue, input_X, CL_TRUE, 0, sizeof(float) * size, data_X, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, input_Y, CL_TRUE, 0, sizeof(float) * size, data_Y, 0, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_X);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &input_Y);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);
    clSetKernelArg(kernel, 3, sizeof(const unsigned int), &n);

    const size_t BLOCK_SIZE = 16;
    size_t *grid_size = new size_t[2]{static_cast<size_t>(n), static_cast<size_t>(n)};
    size_t *group = new size_t[2]{BLOCK_SIZE, BLOCK_SIZE};

    auto start = omp_get_wtime();
    error_code = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, grid_size, group, 0, NULL, NULL);
    if (error_code != CL_SUCCESS) {
        throw (" ");
    }
    clFinish(queue);
    auto end = omp_get_wtime() - start;
    std::cout << (type == 1 ? "Block optimized: " : "Naive: ") << end << std::endl;

    clEnqueueReadBuffer(queue, output, CL_TRUE, 0, sizeof(float) * n * n, result, 0, NULL, NULL);

    delete[] group;
    delete[] grid_size;
    clReleaseMemObject(input_X);
    clReleaseMemObject(input_Y);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

}

template<class Type>
void FillArrayWithRdm(Type *array, int size) {
    std::random_device device;
    std::mt19937 gen_{std::mt19937(device())};
    std::uniform_real_distribution<Type> dist_(-1e1, 1e1);
    for (size_t i = 0; i < size; ++i) {
        array[i] = dist_(gen_);
    }
}

template<class T>
double CalculateSeq(int n, int m, T *x, T *y, T *c) {
    auto start = omp_get_wtime();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float sum = 0;
            for (int k = 0; k < m; ++k)
                sum += x[i * m + k] * y[j + k * n];
            c[i * n + j] = sum;
        }
    }
    return omp_get_wtime() - start;
}

template<class T>
double CalculateOmp(int n, int m, T *x, T *y, T *c) {
    auto start = omp_get_wtime();
#pragma omp parallel for schedule(dynamic) num_threads(4)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float sum = 0;
            for (int k = 0; k < m; ++k)
                sum += x[i * m + k] * y[j + k * n];
            c[i * n + j] = sum;
        }
    }
    double end = omp_get_wtime() - start;
    return end;
}

void FastOutPut() {
    std::cout.setf(std::ios::fixed);
    std::cout.setf(std::ios::showpoint);
    std::cout.precision(10);
}

template<class Type>
bool check(Type *true_arr, Type *test_arr, int n, int m) {

    for (size_t i = 0; i < n * m; ++i) {

        if (std::fabs(true_arr[i] - test_arr[i]) > 1e-1) {
            std::cout << true_arr[i] << " " << test_arr[i] << std::endl;
            std::cout << i << std::endl;
            return false;
        }
    }
    return true;
}

int main() {
    FastOutPut();
    int n = 64 * 16;
    int m = 64 * 16;
    int count_of_elements = n * m;
    float *x = new float[count_of_elements];
    float *y = new float[count_of_elements];
    float *true_res = new float[count_of_elements];
    float *omp_c = new float[count_of_elements];


    FillArrayWithRdm(x, count_of_elements);
    FillArrayWithRdm(y, count_of_elements);

    auto time_ = CalculateSeq(n, m, x, y, true_res);
    std::cout << "CPU sequential time: " << time_
               << std::endl;
//    delete[] true_res;

    auto time = CalculateOmp(n, m, x, y, omp_c);

    std::cout << "CPU omp time: " << time << std::endl;
    if (!check(true_res, omp_c, n, m)) {
        std::cerr << "Failed" << std::endl;
    }
    delete[] omp_c;

    cl_uint numPlatforms{0};
    clGetPlatformIDs(0, nullptr, &numPlatforms);
    if (numPlatforms > 0) {
        cl_platform_id *platforms{nullptr};
        platforms = new cl_platform_id[numPlatforms];
        clGetPlatformIDs(numPlatforms, platforms, nullptr);
        cl_platform_id platform{};
        for (size_t i_plt = 0; i_plt < numPlatforms; ++i_plt) {
            char platformName[128];
            clGetPlatformInfo(platforms[i_plt], CL_PLATFORM_NAME,
                              128, platformName, nullptr);
            platform = platforms[i_plt];
            if (std::string(platformName) == "OpenCLOn12") {
                continue;
            }
            cl_uint deviceCount = 0;

            clGetDeviceIDs(platforms[i_plt], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);

            cl_device_id *devices = new cl_device_id[deviceCount];

            clGetDeviceIDs(platforms[i_plt], CL_DEVICE_TYPE_ALL, 1, devices, &deviceCount);

            for (size_t j_dev = 0; j_dev < deviceCount; ++j_dev) {
                char deviceName[128];
                clGetDeviceInfo(devices[j_dev], CL_DEVICE_NAME, 128, deviceName, nullptr);
                std::cout << "OpenCL. " << platformName << ", " << deviceName << "." << std::endl;
                for (int i = 0; i < 2; ++i) {
                    float *cl_c = new float[count_of_elements];
                    CalculateCL(x, y, cl_c, count_of_elements, n, m, i, devices[j_dev], platform);
                    if (!check(true_res, cl_c, n, m)) {
                        std::cerr << "Failed" << std::endl;
                    }
                    delete[] cl_c;
                }
                float *cl_c = new float[count_of_elements];
                CalculateCLImage(x, y, cl_c, count_of_elements, n, m, devices[j_dev], platform);
                if (!check(true_res, cl_c, n, m)) {
                    std::cerr << "Failed" << std::endl;
                }
                delete[] cl_c;
            }
            delete[] devices;

        }
        delete[] platforms;
    } else {
        std::cerr << "Error: Platform has not been found" << std::endl;
        return 1;
    }

    delete[] x;
    delete[] y;
    delete[] true_res;
    return 0;
}