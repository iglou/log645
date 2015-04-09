/*
* Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
*
* Please refer to the NVIDIA end user license agreement (EULA) associated
* with this source code for terms and conditions that govern your use of
* this software. Any use, reproduction, disclosure, or distribution of
* this software and related documentation outside the terms of the EULA
* is strictly prohibited.
*
*/

// *********************************************************************
// oclVectorAdd Notes:  
//
// A simple OpenCL API demo application that implements 
// element by element vector addition between 2 float arrays. 
//
// Runs computations with OpenCL on the GPU device and then checks results 
// against basic host CPU/C++ computation.
//
// Uses some 'shr' and 'ocl' functions from oclUtils and shrUtils libraries for 
// compactness, but these are NOT required libs for OpenCL developement in general.
// *********************************************************************

// common SDK header for standard utilities and system libs 
#include "oclUtils.h"
#include "shrQATest.h"

// Name of the file with the source code for the computation kernel
// *********************************************************************
const char* cSourceFile = "VectorAdd.cl";

// Host buffers for demo
// *********************************************************************
void **matrix, *dst;	        // Host buffers for OpenCL test
float *matrix_line;
float *matrix_sortie;
void* Golden;                   // Host buffer for host golden processing cross check

// OpenCL Vars
cl_context cxGPUContext;        // OpenCL context
cl_command_queue cqCommandQueue;// OpenCL command que
cl_platform_id cpPlatform;      // OpenCL platform
cl_device_id cdDevice;          // OpenCL device
cl_program cpProgram;           // OpenCL program
cl_kernel ckKernel;             // OpenCL kernel
cl_mem cmDevA;					// OpenCL device source buffer A
cl_mem cmDevB;					// OpenCL device source buffer B 
cl_mem cmDevC;					// OpenCL device source buffer C
cl_mem cmDevH;					// OpenCL device source buffer H 
cl_mem cmDevTD;					// OpenCL device source buffer TD 
cl_mem cmDevDst;                // OpenCL device destination buffer 
size_t szGlobalWorkSize;        // 1D var for Total # of work items
size_t szLocalWorkSize;		    // 1D var for # of work items in the work group	
size_t szParmDataBytes;			// Byte size of context information
size_t szKernelLength;			// Byte size of kernel code
cl_int ciErr1, ciErr2;			// Error code var
char* cPathAndName = NULL;      // var for full paths to data, src, etc.
char* cSourceCL = NULL;         // Buffer to hold source for compilation
const char* cExecutableName = NULL;

// demo config vars
//int iNumElements = 11444777;	// Length of float arrays to process (odd # for illustration)
shrBOOL bNoPrompt = shrFALSE;

// Forward Declarations
// *********************************************************************
void VectorAddHost(const float* pfData1, const float* pfData2, float* pfResult, int iNumElements);
void Cleanup(int argc, char **argv, int iExitCode);

// Main function 
// *********************************************************************
int main(int argc, char **argv)
{
	shrQAStart(argc, argv);
	int nb_col = atoi(argv[1]);
	int nb_ligne = atoi(argv[2]);
	int np = atoi(argv[3]);
	float td = atof(argv[4]);
	float h = atof(argv[5]);

	int iNumElements = nb_ligne * nb_ligne;//nb_ligne*nb_col - 2 * nb_ligne - 2 * nb_col + 4;

	//float **matrix;
	//float *matrix_line;

	// get command line arg for quick test, if provided
	bNoPrompt = shrCheckCmdLineFlag(argc, (const char**)argv, "noprompt");

	// start logs 
	cExecutableName = argv[0];
	shrSetLogFileName("oclVectorAdd.txt");
	shrLog("%s Starting...\n\n# of float elements per Array \t= %i\n", argv[0], iNumElements);

	// set and log Global and Local work size dimensions
	szLocalWorkSize = 512;//CL_KERNEL_WORK_GROUP_SIZE;
	szGlobalWorkSize = shrRoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize
	shrLog("Global Work Size \t\t= %u\nLocal Work Size \t\t= %u\n# of Work Groups \t\t= %u\n\n", szGlobalWorkSize, szLocalWorkSize, (szGlobalWorkSize % szLocalWorkSize + szGlobalWorkSize / szLocalWorkSize));

	// Allocate and initialize host arrays 
	shrLog("Allocate and Init Host Mem...\n");
	//matrix = (void *)malloc(sizeof(cl_float)* (nb_ligne*nb_col));
	matrix_line = (float *)malloc(sizeof(cl_float)* (nb_ligne*nb_col * 2));
	matrix_sortie = (float *)malloc(sizeof(cl_float)* (nb_ligne*nb_col * 2));

	int i, j;
	for (i = 0; i < nb_ligne; i++) {
		for (j = 0; j < nb_col; j++){
			//matrix[i][j] = i*(nb_ligne - i - 1)*j*(nb_col - j - 1);
			matrix_line[i + nb_col*j] = i*(nb_ligne - i - 1)*j*(nb_col - j - 1);
			matrix_sortie[i + nb_col*j] = 0;
		}
	}
	printf("Matrix initial \n");
	for (i = 0; i < nb_ligne; i++) {
		for (j = 0; j < nb_col; j++){
				printf("%0.2f \t", matrix_line[i + nb_col*j]);
		}
		printf("\n");
	}

	printf("Matrix initial \n");
	for (i = 0; i < nb_ligne; i++) {
		for (j = 0; j < nb_col; j++){
			printf("%0.2f \t", matrix_sortie[i + nb_col*j]);
		}
		printf("\n");
	}

	//Get an OpenCL platform : clGetPlatformIDs
	ciErr1 = clGetPlatformIDs(1, &cpPlatform, NULL);

	shrLog("clGetPlatformID...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clGetPlatformID, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	//Get the devices : clGetDeviceIDs
	ciErr1 = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &cdDevice, NULL);
	shrLog("clGetDeviceIDs...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clGetDeviceIDs, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	//Create the context : clCreateContext
	cxGPUContext = clCreateContext(0, 1, &cdDevice, NULL, NULL, &ciErr1);
	shrLog("clCreateContext...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clCreateContext, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	// Create a command-queue : clCreateCommandQueue
	cqCommandQueue = clCreateCommandQueue(cxGPUContext, cdDevice, 0, &ciErr1);
	shrLog("clCreateCommandQueue...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clCreateCommandQueue, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	// Allocate the OpenCL buffer memory objects for source and result on the device GMEM : clCreateBuffer
	cmDevA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float)* szGlobalWorkSize, NULL, &ciErr1);
	ciErr1 |= ciErr2;
	cmDevDst = clCreateBuffer(cxGPUContext, CL_MEM_WRITE_ONLY, sizeof(cl_float)* szGlobalWorkSize, NULL, &ciErr2);
	ciErr1 |= ciErr2;
	shrLog("clCreateBuffer...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clCreateBuffer, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	// Read the OpenCL kernel in from source file
	shrLog("oclLoadProgSource (%s)...\n", cSourceFile);
	cPathAndName = shrFindFilePath(cSourceFile, argv[0]);
	cSourceCL = oclLoadProgSource(cPathAndName, "", &szKernelLength);

	// Create the program : clCreateProgramWithSource
	cpProgram = clCreateProgramWithSource(cxGPUContext, 1, (const char **)&cSourceCL, &szKernelLength, &ciErr1);
	shrLog("clCreateProgramWithSource...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clCreateProgramWithSource, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	// Build the program with 'mad' Optimization option : clBuildProgram
#ifdef MAC
	char* flags = "-cl-fast-relaxed-math -DMAC";
#else
	char* flags = "-cl-fast-relaxed-math";
#endif
	ciErr1 = clBuildProgram(cpProgram, 0, NULL, NULL, NULL, NULL);
	shrLog("clBuildProgram...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		if (ciErr1 == CL_BUILD_PROGRAM_FAILURE) {
			// Determine the size of the log
			size_t log_size;
			clGetProgramBuildInfo(cpProgram, cdDevice, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			// Allocate memory for the log
			char *log = (char *)malloc(log_size);

			// Get the log
			clGetProgramBuildInfo(cpProgram, cdDevice, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

			// Print the log
			printf("%s\n", log);
		}


		shrLog("Error in clBuildProgram, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	// Create the kernel : clCreateKernel
	ckKernel = clCreateKernel(cpProgram, "VectorAdd", &ciErr1);
	shrLog("clCreateKernel (VectorAdd)...\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clCreateKernel, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	// Set the Argument values : clSetKernelArg
	ciErr1 = clSetKernelArg(ckKernel, 0, sizeof(cl_mem), (void*)&cmDevA);
	ciErr1 |= clSetKernelArg(ckKernel, 1, sizeof(cl_int), (void*)&nb_col);
	ciErr1 |= clSetKernelArg(ckKernel, 2, sizeof(cl_int), (void*)&nb_ligne);
	ciErr1 |= clSetKernelArg(ckKernel, 3, sizeof(cl_float), (void*)&h);
	ciErr1 |= clSetKernelArg(ckKernel, 4, sizeof(cl_float), (void*)&td);
	ciErr1 |= clSetKernelArg(ckKernel, 5, sizeof(cl_mem), (void*)&cmDevDst);
	ciErr1 |= clSetKernelArg(ckKernel, 6, sizeof(cl_int), (void*)&iNumElements);
	shrLog("clSetKernelArg 0 - 8...\n\n");
	if (ciErr1 != CL_SUCCESS)
	{
		shrLog("Error in clSetKernelArg, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		Cleanup(argc, argv, EXIT_FAILURE);
	}

	// --------------------------------------------------------
	// Start Core sequence... copy input data to GPU, compute, copy results back

	// Asynchronous write of data to GPU device : clEnqueueWriteBuffer

	clock_t start_t, end_t;
	double total_t;

	start_t = clock();
	printf("Starting of the program, start_t = %ld\n", start_t);


	for (int x = 1; x < np; x++){

		if (x % 2 == 1)
		{
			ciErr1 = clEnqueueWriteBuffer(cqCommandQueue, cmDevA, CL_FALSE, 0, sizeof(cl_float)* iNumElements, matrix_line, 0, NULL, NULL);
			ciErr1 = clEnqueueWriteBuffer(cqCommandQueue, cmDevDst, CL_FALSE, 0, sizeof(cl_float)* iNumElements, matrix_sortie, 0, NULL, NULL);
		}
		else
		{
			ciErr1 = clEnqueueWriteBuffer(cqCommandQueue, cmDevA, CL_FALSE, 0, sizeof(cl_float)* iNumElements, matrix_sortie, 0, NULL, NULL);
			ciErr1 = clEnqueueWriteBuffer(cqCommandQueue, cmDevDst, CL_FALSE, 0, sizeof(cl_float)* iNumElements, matrix_line, 0, NULL, NULL);
		}

		// Launch kernel : clEnqueueNDRangeKernel
		ciErr1 = clEnqueueNDRangeKernel(cqCommandQueue, ckKernel, 1, NULL, &szGlobalWorkSize, &szLocalWorkSize, 0, NULL, NULL);

		// Synchronous/blocking read of results, and check accumulated errors : clEnqueueReadBuffer
		if (x % 2 == 1)
		{
			ciErr1 = clEnqueueReadBuffer(cqCommandQueue, cmDevDst, CL_TRUE, 0, sizeof(cl_float)* iNumElements, matrix_sortie, 0, NULL, NULL);
		}
		else
		{
			ciErr1 = clEnqueueReadBuffer(cqCommandQueue, cmDevDst, CL_TRUE, 0, sizeof(cl_float)* iNumElements, matrix_line, 0, NULL, NULL);
		}
	}

	printf("Going to scan a big loop, start_t = %ld\n", start_t);
	end_t = clock();
	printf("End of the big loop, end_t = %ld\n", end_t);

	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("Total time taken by CPU: %f\n", total_t);
	printf("Exiting of the program...\n");
	//--------------------------------------------------------

	// Compute and compare results for golden-host and report errors and pass/fail
	//shrLog("Comparing against Host/C++ computation...\n\n");
	//VectorAddHost((const float)matrix_line, (const float)B, (float*)Golden, iNumElements);
	shrBOOL bMatch = shrComparefet((const float*)Golden, (const float*)dst, (unsigned int)iNumElements, 0.0f, 0);

	// Cleanup and leave
	Cleanup(argc, argv, (bMatch == shrTRUE) ? EXIT_SUCCESS : EXIT_FAILURE);
}

void Cleanup(int argc, char **argv, int iExitCode)
{
	// Cleanup allocated objects
	shrLog("Starting Cleanup...\n\n");
	if (cPathAndName)free(cPathAndName);
	if (cSourceCL)free(cSourceCL);
	if (ckKernel)clReleaseKernel(ckKernel);
	if (cpProgram)clReleaseProgram(cpProgram);
	if (cqCommandQueue)clReleaseCommandQueue(cqCommandQueue);
	if (cxGPUContext)clReleaseContext(cxGPUContext);
	if (cmDevA)clReleaseMemObject(cmDevA);
	if (cmDevB)clReleaseMemObject(cmDevB);
	if (cmDevC)clReleaseMemObject(cmDevC);
	if (cmDevH)clReleaseMemObject(cmDevH);
	if (cmDevTD)clReleaseMemObject(cmDevTD);
	if (cmDevDst)clReleaseMemObject(cmDevDst);

	// Free host memory
	//free(matrix_line);
	free(dst);
	free(Golden);

	// finalize logs and leave
	shrQAFinishExit(argc, (const char **)argv, (iExitCode == EXIT_SUCCESS) ? QA_PASSED : QA_FAILED);
}

// "Golden" Host processing vector addition function for comparison purposes
// *********************************************************************
void VectorAddHost(const float* pfData1, const float* pfData2, float* pfResult, int iNumElements)
{
	int i;
	for (i = 0; i < iNumElements; i++)
	{
		pfResult[i] = pfData1[i] + pfData2[i];
	}
}
