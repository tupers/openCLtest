#include "CL_utils.h"

int CL_isSupport()
{
	cl_platform_id *platforms;
	cl_uint num_platforms;
	cl_int i, err, platform_index = -1;

	char* ext_data;
	size_t ext_size;
	const char icd_ext[] = "cl_khr_icd";

	char* name_data;
	size_t name_size;
	

	err = clGetPlatformIDs(1, NULL, &num_platforms);
	if (err < 0) {
		perror("Couldn't find any platforms.");
		exit(1);
	}

	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*num_platforms);
	clGetPlatformIDs(num_platforms, platforms, NULL);

	for (i = 0; i < num_platforms; i++)
	{
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, NULL, &name_size);
		if (err < 0) {
			perror("Couldn't read name data.");
			exit(1);
		}
		name_data = (char*)malloc(name_size);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, name_size, name_data, NULL);
		printf("Platform %d supports platform name: %s\n", i, name_data);

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
			//free(ext_data);
			platform_index = i;
			//break;
		}
		free(ext_data);
		free(name_data);
	}

	//if (platform_index > -1)
	//	printf("Platform %d supports the %s extension.\n",
	//		platform_index, icd_ext);
	//else
	//	printf("No platforms support the %s extension.\n", icd_ext);
	return platform_index;
}

void CL_deviceInfo(char* platform_name)
{
	cl_platform_id *platforms;
	cl_uint num_platforms;
	cl_device_id *devices;
	cl_uint num_devices, addr_data;
	cl_int i, err;
	char name_data[48], ext_data[20];
	char* platformname_data;
	size_t platformname_size;
	cl_int findPlatform = -1;

	err = clGetPlatformIDs(1, NULL, &num_platforms);
	if (err < 0) {
		perror("Couldn't find any platforms.");
		exit(1);
	}

	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*num_platforms);
	clGetPlatformIDs(num_platforms, platforms, NULL);
	for (i = 0; i < num_platforms; i++)
	{
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, NULL, &platformname_size);
		if (err < 0) {
			perror("Couldn't read name data.");
			exit(1);
		}
		platformname_data = (char*)malloc(platformname_size);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, platformname_size, platformname_data, NULL);
		if (strstr(platformname_data, platform_name) != NULL){
			findPlatform = i;
			free(platformname_data);
			break;
		}
		free(platformname_data);
	}

	if (findPlatform == -1)
	{
		perror("Couldn't find specified platform");
		free(platforms);
		exit(1);
	}
	err = clGetDeviceIDs(platforms[findPlatform], CL_DEVICE_TYPE_ALL,
		1, NULL, &num_devices);
	if (err < 0) {
		perror("Couldn't find any devices");
		free(platforms);
		exit(1);
	}
	devices = (cl_device_id*)
		malloc(sizeof(cl_device_id) * num_devices);
	clGetDeviceIDs(platforms[findPlatform], CL_DEVICE_TYPE_ALL,
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
		//clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS,
		//	sizeof(ext_data), ext_data, NULL);
		printf("NAME: %s\nADDRESS_WIDTH: %u\n",
			name_data, addr_data);
		
	}
	free(platforms);
	free(devices);
	return 0;
}
