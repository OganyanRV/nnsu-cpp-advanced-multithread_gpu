#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <random>
#include <string>


int main(int argc, char *argv[]) {
    size_t size = 128;
    size_t group = 32;

    cl_uint numPlatforms{0};
    clGetPlatformIDs(0, nullptr, &numPlatforms);

    size_t is_cpu = 0;

    cl_platform_id *platforms{nullptr};
    cl_platform_id platform{};
    if (numPlatforms > 0) {
        platforms = new cl_platform_id[numPlatforms];
        clGetPlatformIDs(numPlatforms, platforms, nullptr);
        std::cout << "Platforms available: " << std::endl;
        for (size_t i = 0; i < numPlatforms; ++i) {
            char platformName[128];
            clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME,
                              128, platformName, nullptr);
            std::cout << platformName << std::endl;
        }
        platform = platforms[is_cpu];
        delete[] platforms;
    } else {
        std::cerr << "Error: Platform has not been found" << std::endl;
        return 1;
    }
    auto type = CL_DEVICE_TYPE_GPU;
    if (is_cpu) {
        type = CL_DEVICE_TYPE_CPU;
    }

    char platformName[128];
    clGetPlatformInfo(platform, CL_PLATFORM_NAME,
                      128, platformName, nullptr);
    std::cout << "Chosen platform: " << platformName << std::endl;


    cl_context_properties properties[3] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties) platform, 0
    };

    cl_context context = clCreateContextFromType(properties,
                                                 type, 0, 0, 0);


    cl_uint dev_count;
    clGetDeviceIDs(platform, type, 256, NULL, &dev_count);
    if (dev_count == 0) {
        std::cerr << "Error: Developers have not been found" << std::endl;
        return 2;
    } else {
        std::cout << "Devices available: " << std::endl;
        cl_device_id *devices = new cl_device_id[dev_count];
        clGetDeviceIDs(platform, type, 256, devices, &dev_count);
        for (size_t i = 0; i < dev_count; ++i) {
            char deviceName[128];
            clGetDeviceInfo( devices[i], type,
                             128, deviceName, nullptr);
            std::cout << deviceName << std::endl;
        }
    }



    cl_device_id *devices = new cl_device_id[dev_count];
    clGetDeviceIDs(platform, type, 256, devices, &dev_count);
    char deviceName[128];
    clGetDeviceInfo( devices[0], CL_DEVICE_NAME,
                     128, deviceName, nullptr);
    std::cout<<"Chosen device: " << deviceName << std::endl;
    cl_device_id device = devices[0];

    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    //  Parsing
    std::ifstream source_kernel("../../oganyan_lab1/first_task.cl");
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

    cl_program program = clCreateProgramWithSource(context, 1, &source, &length, NULL);


    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "TaskOneOganyan", NULL);

    int *data = new int[size];
    int *result = new int[size];
    std::mt19937 generator(0);
    std::uniform_int_distribution<int> distribution(0, 1e5);
    for (size_t i = 0; i < size; ++i) {
        data[i] = distribution(generator) % 100;
    }

    cl_mem input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * size, NULL, NULL);
    cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * size, NULL, NULL);

    clEnqueueWriteBuffer(queue, input, CL_TRUE, 0, sizeof(int) * size, data, 0, NULL, NULL);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    clSetKernelArg(kernel, 2, sizeof(unsigned int), &size);


    size_t count = size / group;
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &count, NULL);
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &size, &group, 0, NULL, NULL);


    clEnqueueReadBuffer(queue, output, CL_TRUE, 0, sizeof(int) * size, result, 0, NULL, NULL);


    for (size_t i = 0; i < size; ++i)
        std::cout << data[i] << "  " << result[i] << "\n";

    delete[] data;
    delete[] result;
    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
