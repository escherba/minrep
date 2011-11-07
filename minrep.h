int COMPARE2(unsigned int nA, unsigned int nB, const char * const a, const char * const b, unsigned int i, unsigned int j);
unsigned int MSP(unsigned int n, const char * const a, unsigned int *d);
void minrep_inplace(char* a, unsigned int n);
void minrep_inplace_msp(char* a, unsigned int n,  unsigned int msp);
void minrep(char* dest, const char * const src, unsigned int n);
void cycle_shift(char* dest, const char * const src, unsigned int n, unsigned int msp);
