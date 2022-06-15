#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <fstream>

#define number long long int

using namespace std;

number strToNumber(char* char_) {
    char* pEnd;
    return strtoll(char_, &pEnd, 0);
}

int main(int argc, char** argv) {

    if (argc == 0) {
        printf("Error: no arguments specified.\n");
        return EXIT_FAILURE;
    }

    ofstream output;
    output.open ("results.txt");
    number **A, **B, n, m, iw, ik, max;
    max = strToNumber(argv[1]);
    if (n < 0) {
        printf("Error: negative number.\n");
        return EXIT_FAILURE;
    }
    srand (( unsigned )time(NULL));
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
            
            A = new number * [ m ];
            B = new number * [ n ];
            for( iw = 0; iw < m; iw++ ) A [ iw ] = new number [ n ];
            for( iw = 0; iw < n; iw++ ) B [ iw ] = new number [ m ];

            for( iw = 0; iw < m; iw++ )
                for( ik = 0; ik < n; ik++ ) A [ iw ][ ik ] = rand( ) % 199 - 99;

            //START SEQUENTIAL 
            elapsed_secs = 0;
            for (int i = 0; i < 10; i++) {
                begin = clock();
                for( iw = 0; iw < m; iw++ )
                    for( ik = 0; ik < n; ik++ )
                        B [ ik ][ iw ] = A [ iw ][ ik ];
                end = clock();
                elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;
                std::cout << ".";
            }
            std::cout << "\nSequential: " << elapsed_secs << std::endl;
            output << "Sequential: " << elapsed_secs << std::endl;
            //END SEQUENTIAL

            //START PARALLEL 1
            elapsed_secs = 0;
            for (int i = 0; i < 10; i++) {
  
                begin = clock();
                #pragma omp parallel for
                for( iw = 0; iw < m; iw++ )
                    for( ik = 0; ik < n; ik++ )
                        B [ ik ][ iw ] = A [ iw ][ ik ];
                end = clock();
                elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                std::cout << ".";
            }
            std::cout << "\nParallel 1: " << elapsed_secs << std::endl;
            output<< "Parallel 1: " << elapsed_secs << std::endl;
            //END PARALLEL 1

            //START PARALLEL 2
            elapsed_secs = 0;
            for (int i = 0; i < 10; i++) {
  
                begin = clock();
                #pragma omp parallel for
                for( iw = 0; iw < m; iw++ )
                    #pragma omp parallel for
                    for( ik = 0; ik < n; ik++ )
                        B [ ik ][ iw ] = A [ iw ][ ik ];
                end = clock();
                elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                std::cout << ".";
            }
            std::cout << "\nParallel 2: " << elapsed_secs << std::endl;
            output<< "Parallel 2: " << elapsed_secs << std::endl;
            //END PARALLEL 2

            for( iw = 0; iw < m; iw++ ) delete [ ] A [ iw ];
            for( iw = 0; iw < n; iw++ ) delete [ ] B [ iw ];
            delete [ ] A;
            delete [ ] B;
            output << endl;
            n += 1000;
        } 
        m += 1000;
    }
    return EXIT_SUCCESS;
} 
