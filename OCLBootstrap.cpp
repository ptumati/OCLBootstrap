#include "OCLBootstrap.h"

int main( int argc, char *argv[] ) {
    OpenCLPlatformList allPlatforms;
    cl::Platform::get( &allPlatforms );

    if( allPlatforms.empty() ) {
        std::cout << "No OpenCL platforms detected." << std::endl;
        return 0;
    }

    cl::Platform defaultPlatform = allPlatforms[0];
    std::cout << "OpenCL Platform Selected: " << defaultPlatform.getInfo< CL_PLATFORM_NAME >() << std::endl;

    OpenCLDeviceList allDevices;
    defaultPlatform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);
    if( allDevices.empty() ) {
        std::cout<<" No devices found. Check OpenCL installation!\n";
        return 0;
    }

    int index = 0;
    for( auto& device : allDevices ) {
        std::cout << "Device Index [" << index++ << "] " << device.getInfo< CL_DEVICE_NAME >() << std::endl;
    }

    cl::Context deviceOneContext( allDevices[0] );
    cl::Context deviceTwoContext( allDevices[1] );
    cl::Program::Sources sources;

    // calculates for each element; C = A + B
    std::string kernel_code=
        "   void kernel square(global const int* A, global const int* N, global int *result ) {"
        "       int ID, Nthreads, n, ratio, start, stop;"
        ""
        "       ID = get_global_id(0);"
        "       Nthreads = get_global_size(0);"
        "       n = N[0];"
        ""
        "       ratio = (n / Nthreads);"  // number of elements for each thread
        "       start = ratio * ID;"
        "       stop  = ratio * (ID + 1);"
        ""
        "       for (int i=start; i<stop; i++)"
        "           result[i] = A[i] * A[i];"
        "   }";
    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program deviceOneProgram( deviceOneContext, sources );
    cl::Program deviceTwoProgram( deviceTwoContext, sources );

    auto buildProgram = []( cl::Program& program, cl::Device& device ) -> bool {
        return (program.build( {device} ) == CL_SUCCESS );
    };

    if( !buildProgram( deviceOneProgram, allDevices[0] ) ) {
        std::cout << "Error building: " << deviceOneProgram.getBuildInfo< CL_PROGRAM_BUILD_LOG >( allDevices[0] ) << std::endl;
        return 1;
    }
    if( !buildProgram( deviceTwoProgram, allDevices[1] ) ) {
        std::cout << "Error building: " << deviceTwoProgram.getBuildInfo< CL_PROGRAM_BUILD_LOG >( allDevices[1] ) << std::endl;
        return 1;
    }

    int testArrayASize[] = {100};
    int testArrayA[100] = {0};
    int resultArrayA[100] = {0};
    
    auto openCLTestArrayABuffer = MakeBufferWrapper( testArrayA, deviceOneContext );
    auto openCLTestArrayASizeBuffer = MakeBufferWrapper( testArrayASize, deviceOneContext );
    auto openCLResultArrayBuffer = MakeBufferWrapper( resultArrayA, deviceOneContext );
    
    for( int i = 0; i < 100; i++ ) testArrayA[i] = i;

    cl::CommandQueue queue( deviceOneContext, allDevices[0] );
    queue.enqueueWriteBuffer( openCLTestArrayABuffer, CL_TRUE, 0, openCLTestArrayABuffer.bufferSize(), testArrayA );
    queue.enqueueWriteBuffer( openCLTestArrayASizeBuffer, CL_TRUE, 0, openCLTestArrayASizeBuffer.bufferSize(), testArrayASize );
    
    auto deviceOneKernel = cl::Kernel( deviceOneProgram, "square" );
    auto squareFunction =
      cl::KernelFunctor< cl::Buffer&, cl::Buffer&, cl::Buffer& >( deviceOneKernel );

    cl_int error(0);
    squareFunction(cl::EnqueueArgs(queue,cl::NDRange(1)), openCLTestArrayABuffer, openCLTestArrayASizeBuffer, openCLResultArrayBuffer, error );

    queue.enqueueReadBuffer( openCLResultArrayBuffer, CL_TRUE, 0, openCLResultArrayBuffer.bufferSize(), resultArrayA );

    return 0;
};
