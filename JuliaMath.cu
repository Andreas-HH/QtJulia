#include <JuliaMath.h>

__device__ void multiplyMyC_gpu(myComplex x, myComplex a) {
  double xReal = x.real;
  double xImg  = x.img;
  double aReal = a.real;
  double aImg  = a.img;
  
  x.real = xReal*aReal - xImg*aImg;
  x.img  = xImg*aReal + xReal*aImg;
}

__device__ void addMyC_gpu(myComplex x, myComplex a) {
  x.real = x.real + a.real;
  x.img  = x.img + a.img;
}

__device__ double norm2_gpu(myComplex x) { 
  return x.real * x.real + x.img * x.img; 
}

__global__ void juliaKernel(myComplex *coords, double *data, myComplex *c, int threshold, int width, int height) {
  int i;
  int idx, cacheIdx;
  int x, y;
  __shared__ myComplex xArray[256];
  myComplex lC = *c;
  
  x = 16*blockIdx.x + threadIdx.x;
  y = 16*blockIdx.y + threadIdx.y;
//   idx = 16*gridDim.x*y + x;
  idx = y*width + x;
  cacheIdx = 16*threadIdx.y + threadIdx.x;
  
  xArray[cacheIdx] = coords[idx];
  
  data[idx] = 1.;
  if (x < width && y < height) {// && idx < width*height) {
    for (i = 0; i < threshold; i++) {
      multiplyMyC_gpu(xArray[cacheIdx], lC);
      addMyC_gpu(xArray[cacheIdx], lC);
    
      if (norm2_gpu(xArray[cacheIdx]) > 1000.) {
        break;
      }
    }
//     data[idx] = ((double) i)/((double) threshold);
    data[idx] = 1.;
  }
}

__global__ void initCoords(myComplex *coords, double topLr, double topLi, double pixelW, int width, int height) {
  int idx;
  int x, y;
  
  x = 16*blockIdx.x + threadIdx.x;
  y = 16*blockIdx.y + threadIdx.y;
  idx = y*width + x;
//   idx = 16*gridDim.x*y + x;
//   idx = 16*blockIdx.x + 16*gridDim.x*blockIdx.y + threadIdx.x + 16*threadIdx.y;
//   x = idx % gridDim.x*blockDim.x;
//   y = idx / gridDim.x*blockDim.x;
  if (x < width && y < height) {// && idx < width*height) {
    coords[idx].real = topLr + x*pixelW;
    coords[idx].img = topLi - y*pixelW;
  }
}

extern "C" {
  void juliaValueGPU(myComplex *coords, double *data, myComplex *c, int width, int height, int threshold) {
    cudaPointerAttributes attr;
    cudaPointerGetAttributes(&attr, data);
    printf("data on device: %i\n", attr.device);
    
    dim3 grid((width+15)/16, (height+15)/16);
    dim3 block(16, 16);
    juliaKernel<<<grid,block>>>(coords, data, c, threshold, width, height);
    printf("Der kernel sollte eig fertig sein...\n");
  }
  
  void downloadData(double *data_gpu, double *data_cpu, int N) {
    printf("downloading data...\n");
    cudaPointerAttributes attr;
    cudaPointerGetAttributes(&attr, data_gpu);
    if (attr.memoryType == cudaMemoryTypeHost) {
      printf("error: gpu\n");
    }
    cudaPointerGetAttributes(&attr, data_cpu);
    if (attr.memoryType == cudaMemoryTypeDevice) {
      printf("error: cpu\n");
    }
    
    cudaMemcpy(data_cpu, data_gpu, N*sizeof(double), cudaMemcpyDefault);
  }
  
  void uploadC(myComplex *c_gpu, myComplex *c_cpu) {
    printf("uploadC()\n");
    cudaPointerAttributes attr;
    cudaPointerGetAttributes(&attr, c_cpu);
    if (attr.memoryType == cudaMemoryTypeDevice) {
      printf("error: cpu\n");
    }
    cudaPointerGetAttributes(&attr, c_gpu);
    if (attr.memoryType == cudaMemoryTypeHost) {
      printf("error: gpu\n");
    }

//     myComplex *test = createMyComplex(0., 0.);
    cudaMemcpy(c_gpu, c_cpu, sizeof(myComplex), cudaMemcpyDefault);
//     cudaMemcpy(test, c_gpu, sizeof(myComplex), cudaMemcpyDefault);
//     printf("c_gpu = (%f. %f)\n", test->real, test->img);
  }
  
  /*myComplex* createEmptyMyComplexOnGPU() {
    myComplex *a_gpu;
    
    cudaMalloc(&a_gpu, sizeof(myComplex));
  
    return a_gpu;
  }*/
  
  myComplex* createMyComplexOnGPU(myComplex *value_host) {
    printf("createMyComplexOnGPU()\n");
//     cudaSetDevice(1);
    myComplex *a_gpu;
    cudaError_t error;
    cudaPointerAttributes attr;
    
    cudaHostRegister(value_host, sizeof(myComplex), cudaHostRegisterMapped);
    
    error = cudaMalloc(&a_gpu, sizeof(myComplex));
    
    
    cudaPointerGetAttributes(&attr, value_host);
    if (attr.memoryType == cudaMemoryTypeDevice) {
      printf("error: cpu\n");
    }
    cudaPointerGetAttributes(&attr, a_gpu);
    if (attr.memoryType == cudaMemoryTypeHost) {
      printf("error: gpu\n");
    }
    
    cudaMemcpy(a_gpu, value_host, sizeof(myComplex), cudaMemcpyHostToDevice);
    
    cudaPointerGetAttributes(&attr, value_host);
    if (attr.memoryType == cudaMemoryTypeDevice) {
      printf("now error: cpu\n");
    }
    cudaPointerGetAttributes(&attr, a_gpu);
    if (attr.memoryType == cudaMemoryTypeHost) {
      printf("now error: gpu\n");
    }
    
    
    if (error == cudaSuccess) {
      printf("successfully uploaded a myComplex\n");
    }
    
//     cudaPointerAttributes attr;
    cudaPointerGetAttributes(&attr, a_gpu);
    printf("a_gpu on device: %i\n", attr.device);
    
    cudaHostUnregister(value_host);
  
    return a_gpu;
  }
  
  double* initDataGPU(int width, int height) {
    double *data_gpu;
    cudaError_t error;
    
    error = cudaMalloc(&data_gpu, width*height*sizeof(double));
    if (error == cudaSuccess) {
      printf("GPU data allocated!\n");
    }
    
    cudaPointerAttributes attr;
    cudaPointerGetAttributes(&attr, data_gpu);
    printf("data_gpu on device: %i\n", attr.device);
    
    return data_gpu;
  }
  
  // fill using a kernel !! (Erlaubt auch die alte Struktur mit myComplex**)
  myComplex* initCoordsGPU(int width, int height, myComplex* screenMid_host, double pixelW) {
//     int x, y;
    myComplex *coords;
    cudaError_t error;
    double topLeftX = screenMid_host->real - (width/2.)*pixelW;
    double topLeftY = screenMid_host->img + (height/2.)*pixelW;
//     myComplex *current_point = createMyComplex(topLeftX, topLeftY);
//     myComplex *tmp_point = createMyComplex(1., 1.);
    
    error = cudaMalloc(&coords, width*height*sizeof(myComplex));
    if (error == cudaSuccess) {
      printf("cudaMalloc ausgefuehrt\n");
    }
    dim3 grid((width+15)/16, (height+15)/16);
    dim3 block(16, 16);
    initCoords<<<grid, block>>>(coords, topLeftX, topLeftY, pixelW, width, height);
    
    myComplex *test = createMyComplex(topLeftX, topLeftY);
    cudaMemcpy(test, coords+1610, sizeof(myComplex), cudaMemcpyDefault);
    printf("von GPU coords gelesen: (%f, %f), pW = %f\n", test->real, test->img, pixelW);
//     printf("kernel feddüsch\n");
    /*for (x = 0; x < width; x++) {
      for (y = 0; y < height; y++) {
// 	cudaMalloc(coords + y*width + x, sizeof(myComplex));
	cudaMemcpy(coords + y*width + x, current_point, sizeof(myComplex), cudaMemcpyDefault);
        current_point->img -= pixelW;
      }
      current_point->img = topLeftY;
      current_point->real += pixelW;
    }
    free(current_point);*/
    
    return coords;
  }
  
  void clearData(double *data) {
    cudaFree(data);
  }
  
  void clearCoords(myComplex *coords) {
//     int i;
    
    /*for (i = 0; i < width*height; i++) {
      cudaFree(coords + i);
    }*/
    cudaFree(coords);
  }
}