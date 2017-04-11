#include "CL_utils.h"

int CL_isSupport()
{
	cl_platform_id *platforms;
	cl_uint num_platforms;
	cl_int i, err, platform_index = -1;

	char* ext_data;
	size_t ext_size;
	const char icd_ext[] = "cl_khr_icd";

	err = clGetPlatformIDs(1, NULL, &num_platforms);
	if (err < 0) {
		perror("Couldn't find any platforms.");
		exit(1);
	}

	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*num_platforms);
	clGetPlatformIDs(num_platforms, platforms, NULL);

	for (i = 0; i < num_platforms; i++)
	{
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &ext_size);
		if (err < 0) {
			perror("Couldn't read extension data.");
			exit(1);
		}
		ext_data = (char*)malloc(ext_size);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, ext_size, ext_data, NULL);
		printf("Platform %d supports extensions: %s\n",
			i, ext_data);

		if (strstr(ext_data, icd_ext) != NULL) {
			free(ext_data);
			platform_index = i;
			break;
		}
		free(ext_data);
	}

	if (platform_index > -1)
		printf("Platform %d supports the %s extension.\n",
			platform_index, icd_ext);
	else
		printf("No platforms support the %s extension.\n", icd_ext);
	return platform_index;
}

void CL_deviceInfo()
{
	cl_platform_id platform;
	
	cl_device_id *devices;
	cl_uint num_devices, addr_data;
	cl_int i, err;
	char name_data[48], ext_data[4096];
	err = clGetPlatformIDs(1, &platform, NULL);
	if (err < 0) {
		perror("Couldn't find any platforms");
		exit(1);
	}
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
		1, NULL, &num_devices);
	if (err < 0) {
		perror("Couldn't find any devices");
		exit(1);
	}
	devices = (cl_device_id*)
		malloc(sizeof(cl_device_id) * num_devices);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
		num_devices, devices, NULL);
	for (i = 0; i<num_devices; i++) {
		err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME,
			sizeof(name_data), name_data, NULL);
		if (err < 0) {
			perror("Couldn't read extension data");
			exit(1);
		}
		clGetDeviceInfo(devices[i], CL_DEVICE_ADDRESS_BITS,
			sizeof(ext_data), &addr_data, NULL);
		clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS,
			sizeof(ext_data), ext_data, NULL);
		printf("NAME: %s\nADDRESS_WIDTH: %u\nEXTENSIONS: %s",
			name_data, addr_data, ext_data);
	}
	free(devices);
	return 0;
}
