#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <fstream>

#define BLOCK 16
#define number long long int

using namespace std;

__global__ void parallelTranspose(number* A, number* B, number m, number n) {
    number i = blockIdx.y * blockDim.y + threadIdx.y;
    number j = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < m && j < n) {
        B[j * m + i] = A[i * n + j];
    }
}

number strToNumber(char* char_) {
    char* pEnd;
    return strtoll(char_, &pEnd, 0);
}

int main(int argc, char *argv[]) {
    
    if (argc == 0) {
        printf("Error: no arguments specified.\n");
        return EXIT_FAILURE;
    }

    ofstream output;
    output.open ("results.txt");
    number *A, *B, n, m, i, max, size, *d_A, *d_B;
    max = strToNumber(argv[1]);
    size = sizeof(number);
    if (max < 0) {
        printf("Error: negative number.\n");
        return EXIT_FAILURE;
    }
    srand ((unsigned)time(NULL));
    m = 1000;
    while (true) {
        if (m  > max) break;
        n = 1000;
        while (true) {
            if (n > max) break;
            std::cout << "m: " << m << " n: " << n << endl;
            output << "m: " << m << " n: " << n << endl;
            clock_t begin, end;
            double elapsed_secs;

            A = (number*) malloc(m * n * size);
            B = (number*) malloc(m * n * size);

            for (i = 0; i < m * n; ++i)
                A[i] = rand() % 10;

            cudaMalloc(&d_A, n * m * size);
            cudaMalloc(&d_B, n * m * size);

            //dimensions
            dim3 threadblock(BLOCK, BLOCK);
            dim3 grid(1 + n / threadblock.x, 1 + m / threadblock.y);

            //copying A to the GPU
            cudaMemcpy(d_A, A, n * m * size, cudaMemcpyHostToDevice);

            //START PARALLEL
            elapsed_secs = 0;
            for (int i = 0; i < 10; i++) {
                begin = clock();

                //calling function
                parallelTranspose<<<grid, threadblock>>>(d_A, d_B, m, n);
                cudaDeviceSynchronize();

                //once the function has been called I copy the result in matrix
                cudaMemcpy(B, d_B, n * m * size, cudaMemcpyDeviceToHost);

                end = clock();
                elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;
                std::cout << ".";
            }
            std::cout << elapsed_secs/10 << std::endl;
            output << elapsed_secs/10 << std::endl;
            //END PARALLEL

            cudaFree(d_A);
            cudaFree(d_B);

            free(A);
            free(B);
            output << endl;
            n += 1000;
        } 
        m += 1000;
    }
    return 0;
}
