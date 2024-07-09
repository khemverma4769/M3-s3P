
#include <iostream>
#include <vector>
#include <CL/cl.hpp>

using namespace std;

void addVectors(vector<int>& a, vector<int>& b, vector<int>& c) {
    // Get available platforms
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    // Get available devices
    vector<cl::Device> devices;
    platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

    // Create a context and command queue for the first device
    cl::Context context(devices);
    cl::CommandQueue queue(context, devices[0]);

    // Create buffers for the input and output vectors
    cl::Buffer bufferA(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * a.size(), a.data());
    cl::Buffer bufferB(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * b.size(), b.data());
    cl::Buffer bufferC(context, CL_MEM_WRITE_ONLY, sizeof(int) * c.size());

    // Read the kernel source code into a string
    string kernelSourceCode =
        "__kernel void vectorAdd(__global const int* a, __global const int* b, __global int* c) {\n"
        "   int i = get_global_id(0);\n"
        "   c[i] = a[i] + b[i];\n"
        "}\n";

    // Create a program from the kernel source code
    cl::Program::Sources kernelSource(kernelSourceCode);
    cl::Program program(context, kernelSource);

    // Build the program for the available devices
    program.build(devices);

    // Create a kernel object from the program
    cl::Kernel kernel(program, "vectorAdd");

    // Set the kernel arguments
    kernel.setArg(0, bufferA);
    kernel.setArg(1, bufferB);
    kernel.setArg(2, bufferC);

    // Execute the kernel on the device
    cl::NDRange globalSize(a.size());
    cl::NDRange localSize(256);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, localSize);

    // Read the result back into the output vector
    queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, sizeof(int) * c.size(), c.data());
}

int main() {
    // Create two input vectors
    int n = 1000000;
    vector<int> a(n);
    vector<int> b(n);
    vector<int> c(n);
    for(int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Perform the vector addition
    addVectors(a, b, c);

    // Print the result
    for(int i = 0; i < n; i++) {
        cout << c[i] << " ";
    }
    cout << endl;

    return 0;
}
