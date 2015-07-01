void init_genrand(unsigned long s);
void init_by_array(unsigned long init_key[], int key_length);
unsigned long genrand_int32(void);
long genrand_int31(void);
double genrand_real1(void); // [0,1]
double genrand_real2(void); // [0,1)
double genrand_real3(void); // (0,1)
double genrand_res53(void); // [0,1) 53-bit resolution
