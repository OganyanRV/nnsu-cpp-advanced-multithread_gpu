#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <omp.h>

template<class Type>
std::vector<Type> gen_array(size_t size) {
    std::vector<Type> array(size);
    std::random_device device;
    std::mt19937 gen_{std::mt19937(device())};
    std::uniform_real_distribution<Type> dist_(-1e2, 1e2);
    for (size_t i = 0; i < size; ++i) {
        array[i] = dist_(gen_);
    }
    return array;
}

template<class Type>
Type gen_acoef() {
    std::random_device device;
    std::mt19937 gen_{std::mt19937(device())};
    std::uniform_real_distribution<Type> dist_(-1, 1);
    return dist_(gen_);
}

template<class Type>
bool check2(const std::vector<Type> &true_arr, const std::vector<Type> &test_arr) {
//    for (size_t i = 0; i < true_arr.size(); ++i) {
//        std::cout<<true_arr[i] << " "<< test_arr[i]<<std::endl;
//    }
    if (true_arr.size() != test_arr.size()) {
        return false;
    }
    for (size_t i = 0; i < true_arr.size(); ++i) {
        if (std::fabs(true_arr[i] - test_arr[i]) > 1e-3) {
            std::cout << "Mistake here: " << i << std::endl;
            std::cout << true_arr[i] << " " << test_arr[i] << std::endl;
            return false;
        }
    }
    return true;
}

template<class Type>
bool check(const std::vector<Type> &true_arr, const std::vector<Type> &test_arr) {

    if (true_arr.size() != test_arr.size()) {
        return false;
    }
    for (size_t i = 0; i < true_arr.size(); ++i) {
        if (std::fabs(true_arr[i] - test_arr[i]) > 1e-6) {
            return false;
        }
    }
    return true;
}

template<class Type>
double AxpySeq(size_t size, Type a_coef, const std::vector<Type> &x,
               size_t incx, std::vector<Type> *y, size_t incy) {
    double start = omp_get_wtime();
    for (int i = 0; i < size; ++i) {
        (*y)[i * incy] += a_coef * x[i * incx];
    }
    return omp_get_wtime() - start;
}

template<class Type>
double AxpyOmp(size_t size, Type a_coef, const std::vector<Type> &x,
               size_t incx, std::vector<Type> *y, size_t incy) {
    double start = omp_get_wtime();
    omp_set_num_threads(4);
#pragma omp parallel
    {
#pragma omp for schedule(static)
        for (int i = 0; i < size; ++i) {
            (*y)[i * incy] += a_coef * x[i * incx];
        }
    }
    return omp_get_wtime() - start;
}

template<class Type>
std::pair<std::pair<double, double>, bool>
AxmpyCl(cl_device_id &device_id, int size, Type a_coef, const std::vector<Type> &x,
        int incx, const std::vector<Type> &y, int incy,
        int block_size, cl_platform_id &platform_id,
        const std::vector<Type> &true_y) {

    std::ifstream source_kernel;
    if (std::is_same_v<Type, float>) {
        source_kernel.open("../../oganyan_lab2/saxpy.cl");
    } else if (std::is_same_v<Type, double>) {
        source_kernel.open("../../oganyan_lab2/daxpy.cl");
    } else {
        std::string msg = "Wrong type failed";
        std::cerr << msg << std::endl;
        throw std::runtime_error(msg);
    }

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

    double start = omp_get_wtime();
    cl_int error_code;
    cl_context_properties properties[3] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties) platform_id, 0
    };

    cl_context context = clCreateContext(properties, 1, &device_id, nullptr, nullptr, &error_code);

    if (error_code != CL_SUCCESS) {
        std::string msg = "Context failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &error_code);

    if (error_code != CL_SUCCESS) {
        std::string msg = "Queue failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    cl_program program = clCreateProgramWithSource(context, 1, &source, &length, &error_code);
    if (error_code != CL_SUCCESS) {
        std::string msg = "Program failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    error_code = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (error_code != CL_SUCCESS) {
        std::string msg = "Build program failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    cl_kernel kernel;
    if (std::is_same_v<Type, float>) {
        kernel = clCreateKernel(program, "saxpy", &error_code);
    } else if (std::is_same_v<Type, double>) {
        kernel = clCreateKernel(program, "daxpy", &error_code);
    } else {
        std::string msg = "Wrong type failed";
        std::cerr << msg << std::endl;
        throw std::runtime_error(msg);
    }
    if (error_code != CL_SUCCESS) {
        std::string msg = "Kernel failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    cl_mem input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(Type) * size, NULL, &error_code);
    if (error_code != CL_SUCCESS) {
        std::string msg = "Buffer input failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }
    cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(Type) * size, NULL, &error_code);

    if (error_code != CL_SUCCESS) {
        std::string msg = "Buffer output failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    Type *data = new Type[size];
    Type *result = new Type[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = x[i];
        result[i] = y[i];
    }

    error_code = clEnqueueWriteBuffer(queue, input, CL_TRUE, 0, sizeof(Type) * size, data, 0, NULL, NULL);
    error_code = clEnqueueWriteBuffer(queue, output, CL_TRUE, 0, sizeof(Type) * size, result, 0, NULL, NULL);

    if (error_code != CL_SUCCESS) {
        std::string msg = "Buffer write failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    error_code = clSetKernelArg(kernel, 0, sizeof(int), &size);
    error_code = clSetKernelArg(kernel, 1, sizeof(Type), &a_coef);
    error_code = clSetKernelArg(kernel, 2, sizeof(cl_mem), &input);
    error_code = clSetKernelArg(kernel, 3, sizeof(int), &incx);
    error_code = clSetKernelArg(kernel, 4, sizeof(cl_mem), &output);
    error_code = clSetKernelArg(kernel, 5, sizeof(int), &incy);

    if (error_code != CL_SUCCESS) {
        std::string msg = "Setting Kernel args failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

//    size_t count_of_blocks = size / block_size;
    size_t sz = size;
    size_t block_sz = block_size;

    double start_kernel = omp_get_wtime();
    error_code = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &sz, &block_sz, 0, NULL, NULL);
    if (error_code != CL_SUCCESS) {
        throw (std::runtime_error("Program running failed"));
    }
    error_code = clFinish(queue);
    start_kernel = omp_get_wtime() - start_kernel;
    if (error_code != CL_SUCCESS) {
        std::string msg = "Finish failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    error_code = clEnqueueReadBuffer(queue, output, CL_TRUE, 0, sizeof(Type) * size, result, 0, NULL, NULL);
    if (error_code != CL_SUCCESS) {
        std::string msg = "Read Buffer failed";
        std::cerr << msg << std::endl;
        throw (std::runtime_error(msg));
    }

    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    start = omp_get_wtime() - start;
    std::vector<Type> test_y(size);
    for (size_t i = 0; i < size; ++i) {
        test_y[i] = result[i];
    }
    bool is_ok = check2(true_y, test_y);
    delete[] data;
    delete[] result;

    return {{start, start_kernel}, is_ok};
}

void FastOutPut() {
    std::cout.setf(std::ios::fixed);
    std::cout.setf(std::ios::showpoint);
    std::cout.precision(10);
}

int main() {
    FastOutPut();
    //    16777216; // 2^24
    //67108864; //2^26
    size_t count_of_elements = 33554432; //2^25
    //    size_t count_of_elements = 8388608; //2^23
    size_t incx = 1;
    size_t incy = 1;

    std::cout << "Count of elements: " << count_of_elements << std::endl;
    double d_a_coef = gen_acoef<double>();
    float fl_a_coef = gen_acoef<float>();
    auto fl_x = gen_array<float>(count_of_elements);
    auto fl_y = gen_array<float>(count_of_elements);
    auto d_x = gen_array<double>(count_of_elements);
    auto d_y = gen_array<double>(count_of_elements);

    std::vector<float> fl_was = fl_y;
    std::vector<double> d_was = d_y;

    std::cout << "CPU daxpy sequential time: " <<
              AxpySeq(count_of_elements, d_a_coef, d_x, incx, &d_y, incy) << std::endl;

    std::cout << "CPU saxpy sequential time: " <<
              AxpySeq(count_of_elements, fl_a_coef, fl_x, incx, &fl_y, incy) << std::endl;


    {
        auto tmp_res = d_was;
        std::cout << "CPU OMP daxpy parallel time: " <<
                  AxpyOmp(count_of_elements, d_a_coef, d_x, incx, &tmp_res, incy) << std::endl;
        if (!check(tmp_res, d_y)) {
            std::cerr << "Wrong CPU OMP DAXPY!" << std::endl;
        } else {
            std::cout << "PASSED!" << std::endl;
        }
    }


    {
        auto tmp_res = fl_was;
        std::cout << "CPU OMP saxpy parallel time: " <<
                  AxpyOmp(count_of_elements, fl_a_coef, fl_x, incx, &tmp_res, incy) << std::endl;
        if (!check(tmp_res, fl_y)) {
            std::cerr << "Wrong CPU OMP SAXPY!" << std::endl;
        } else {
            std::cout << "PASSED!" << std::endl;
        }
    }


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

                std::pair<size_t, std::pair<double, double>> benchmark = {8, {4.0, 4.0}};
                for (size_t block_size = 8; block_size <= 256; block_size *= 2) {
                    auto res = AxmpyCl(devices[j_dev], count_of_elements, fl_a_coef,
                                       fl_x, incx, fl_was, incy, block_size, platform, fl_y);
                    if (res.second) {
                        if (res.first < benchmark.second) {
                            benchmark = {block_size, res.first};
                        }
                    }
                }
                std::cout << platformName << " saxpy, best block size = " << benchmark.first <<
                          ". Best time: " << benchmark.second.first << ". Best time (kernel): "
                          << benchmark.second.second << std::endl;

                benchmark = {8, {4.0, 4.0}};
                for (size_t block_size = 8; block_size <= 256; block_size *= 2) {
                    auto res = AxmpyCl(devices[j_dev], count_of_elements, d_a_coef,
                                       d_x, incx, d_was, incy, block_size, platform, d_y);
                    if (res.second) {
                        if (res.first < benchmark.second) {
                            benchmark = {block_size, res.first};
                        }
                    }
                }
                std::cout << platformName << " daxpy, best block size = " << benchmark.first <<
                          ". Best time: " << benchmark.second.first << ". Best time (kernel): "
                          << benchmark.second.second << std::endl;

            }
            delete[] devices;

        }
        delete[] platforms;
    } else {
        std::cerr << "Error: Platform has not been found" << std::endl;
        return 1;
    }

    return 0;
}
