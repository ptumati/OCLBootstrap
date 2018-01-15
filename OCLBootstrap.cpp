#include "OCLBootstrap.h"

int main( int argc, char *argv[] ) {
    cl_int status; // Used for error checking 
    // Retrieve the number of platforms 
    cl_uint numPlatforms = 0; 
    status = clGetPlatformIDs(0, NULL, &numPlatforms); // Allocate enough space for each platform
    if( status != CL_SUCCESS ) {
        std::cout << "clPGetPlatformIDs unsuccessful: " << status << std::endl;
        return -1;
    }
    
    cl_platform_id *platforms = NULL; 
    platforms = (cl_platform_id*)malloc(numPlatforms*sizeof (cl_platform_id)); // Fill in the platforms 
    status = clGetPlatformIDs(numPlatforms, platforms, NULL); // Retrieve the number of devices 
    cl_uint numDevices = 0; 
    status = clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices); // Allocate enough space for each device 
    cl_device_id *devices; 
    devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id)); // Fill in the devices 
    status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

    return 0;
};
