
/* 

Trabalho de programação Programação Paralela 

Nome: Gabriel Cortez Mazzardo
Matrícula: 201612820

*/



#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <omp.h>
#include <string>
#include <list>

class SharedArray {

private:
    char* array;
    int index;
    int size;
    bool exclusaomutua;
    omp_lock_t lock;

public:

    //vetor de ints apenas para armazenar qual thread (0,1,2) executou cada iteracao (0 a 59) e poder listar depois lá na função printStats
    int iteracoes[60];            

    //lista apenas para armazenar a ordem que as iteracoes foram executadas (threads foram preenchendo o array) e poder printar depois na função printStats
    std::list<int> lista_ordem_iteracoes;


    SharedArray(int n, bool use) : size(n), index(0), exclusaomutua(use) {
        array = new char[size];
        std::fill(array, array+size, '-');
        omp_init_lock(&this->lock);    
    }

    ~SharedArray() {
        omp_destroy_lock(&this->lock);
        delete[] array;                
    }

    void addChar(char c, int itnum) {
        
        this->iteracoes[itnum] = omp_get_thread_num();        

        if (exclusaomutua) omp_set_lock(&this->lock);
        
        array[index] = c;
        spendSomeTime();
        index++;

        if(exclusaomutua) omp_unset_lock(&this->lock);
    
        this->lista_ordem_iteracoes.push_back(itnum);
    }

    int countOccurrences(char c) {
        return std::count(array, array+size, c);
    }

    std::string toString() {
        return std::string(array, size);
    }

private:

    void spendSomeTime() {
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 100; j++) {
                // These loops shouldn't be removed by the compiler
            }
        }
    }
};


class OMPArrayFiller {

private:
    static const int nThreads = 3;
    static const int nTimes = 20;
    SharedArray* array;

public:

    OMPArrayFiller(bool exclusaomutua) {

        omp_set_num_threads(nThreads);
        array = new SharedArray(nThreads * nTimes, exclusaomutua);
    }


    ~OMPArrayFiller() {
        delete array;
    }

    void printStats() {
        
        //listas auxiliares para printar iteracoes de cada thread
        std::list<int> iteracoes_thread0, iteracoes_thread1, iteracoes_thread2;
                
        for(int i=0; i<60; i++){

            if (array->iteracoes[i] == 0)
                iteracoes_thread0.push_back(i);
            else if (array->iteracoes[i] == 1)
                iteracoes_thread1.push_back(i);
            else
                iteracoes_thread2.push_back(i);
        }
        std::cout << "\nIteracoes atribuidas para a Thread 0:\n";
        for (auto const& i: iteracoes_thread0){
            std::cout << i << " ";
        }
        
        std::cout << "\n\nIteracoes atribuidas para a Thread 1:\n";
        for (auto const& i: iteracoes_thread1){
            std::cout << i << " "; 
        }        
        
        std::cout << "\n\nIteracoes atribuidas para a Thread 2:\n";
        for (auto const& i: iteracoes_thread2){
            std::cout << i << " ";
        }

        std::cout << "\n\nOrdem em que as iteracoes foram executadas pelas Threads:\n";
        for (auto const& i: array->lista_ordem_iteracoes){
            std::cout << i << " ";
        }

        std::cout << "\n\nArray preenchido apos as execucoes:\n" << array->toString() << std::endl; 
        std::cout << "\nNumero de iteracoes executadas por cada thread:\n";
        for (int i = 0; i < nThreads; ++i)
            std::cout << (char) ('A'+i) << "=" << array->countOccurrences('A'+i) << " ";
            std::cout << std::endl;
    }

    void static_sem_chunk() {
                
        int i;
        
        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(static)

            for (int i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

    void static_com_chunk(int tamanhoChunk) {
                
        int i, chunk;
        chunk = tamanhoChunk;
        
        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(static, chunk)

            for (i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

    void dynamic_sem_chunk() {
              
        int i;
        
        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(dynamic)

            for (i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

    void dynamic_com_chunk(int tamanhoChunk) {
                
        int i, chunk;
        chunk = tamanhoChunk;
        
        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(dynamic, chunk)

            for (i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

    void guided_sem_chunk() {
              
        int i;
  
        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(guided)

            for (i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

    void guided_com_chunk(int tamanhoChunk) {
                
        int i, chunk;
        chunk = tamanhoChunk;
        
        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(guided, chunk)

            for (i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

    void runtime() {
              
        int i;
  
        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(runtime)

            for (i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

    void Auto() {
                
        int i;

        #pragma omp parallel private(i)
        {
            #pragma omp for schedule(auto)

            for (i = 0; i < nTimes*nThreads; i++) {

                if(omp_get_thread_num() == 0)
                    array->addChar('A',i);
                else if (omp_get_thread_num() == 1)
                    array->addChar('B',i);
                else
                    array->addChar('C',i);
            }
        }
    }

};


int main() {
  
        /* 

        OBS.: As explicacoes de cada um dos 9 casos de execucao abaixo estao no arquivo Entrega.md
        Para executar cada caso, descomentar cada um deles individualmente abaixo:

        */
        

        //Caso 1: static SEM especificacao do chunk (com exclusão mútua)       
        OMPArrayFiller caso1(true);
        caso1.static_sem_chunk();
        caso1.printStats();        
                

        /*
        //Caso 2: static COM especificacao do chunk==10 (com exclusão mútua) 
        OMPArrayFiller caso2(true);
        caso2.static_com_chunk(10);
        caso2.printStats();
        */

        /*
        //Caso 3: dynamic SEM especificacao do chunk (com exclusão mútua)         
        OMPArrayFiller caso3(true);
        caso3.dynamic_sem_chunk();
        caso3.printStats();
        */          
              
        /*
        //Caso 4: dynamic COM especificacao do chunk==10 (com exclusão mútua) 
        OMPArrayFiller caso4(true);
        caso4.dynamic_com_chunk(10);
        caso4.printStats();
        */          
     
        /*
        //Caso 5: guided SEM especificacao do chunk (com exclusão mútua) 
        OMPArrayFiller caso5(true);
        caso5.guided_sem_chunk();
        caso5.printStats();
        */          
       
        /*
        //Caso 6: guided COM especificacao do chunk (com exclusão mútua) 
        OMPArrayFiller caso6(true);
        caso6.guided_com_chunk(5);
        caso6.printStats();
        */      

        /*
        //Caso 7: runtime (com exclusão mútua) 
        OMPArrayFiller caso7(true);
        caso7.runtime();
        caso7.printStats();
        */
        
        /*
        //Caso 8: auto (com exclusão mútua) 
        OMPArrayFiller caso8(true);
        caso8.Auto();
        caso8.printStats();
        */  
      
        /*
        //Caso 9: static sem chunk e SEM EXCLUSÃO MUTUA 
        OMPArrayFiller caso9(false);
        caso9.static_sem_chunk();
        caso9.printStats();
        */
}
