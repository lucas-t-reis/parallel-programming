# Parallel tinkering

Experiments on Parallelism for C/C++. In each program in this repo I'll try to explore some of the omp.h functionalities. 
Aiming to build a future "quick reference", there is a structure in every .cpp file you see here. Currently, it consists of:

* Each .cpp file header will contain my observations on the program's behavior
* They will be verbose, not only for future reference but to assure I understand what I'm doing on every step
* So far, everything will be done using the omp.h library, meaning you have to compile with the flag "-fopenmp"
* No -O3 will be used for testing, since the purpose is to study speedups from parallelism and not other optimization techniques



## Functionalities covered

### Functions
- [X] omp_get_num_threads()
- [X] omp_get_thread_num()
- [ ] omp_set_num_threads()

### Clauses
- [X] shared() & private()
- [X] firstprivate() & lastprivate()
- [ ] num_threads()
