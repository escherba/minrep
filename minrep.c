#include <math.h>
#include <assert.h>
//#include <stdio.h>

#define MIN2(a,b) ((a) > (b) ? (b) : (a))
#define MIN3(a,b,c) ((b)<(a)?((c)<(b)?(c):(b)):((c)<(a)?(c):(a)))

#define MAX2(a,b) ((a) < (b) ? (b) : (a))
#define MAX3(a,b,c) ((b)>(a)?((c)>(b)?(c):(b)):((c)>(a)?(c):(a)))

// Generic macro to swap two variables, supported by C89 and later
#define SWAP(x,y) { __typeof__ (x) _T = x; x = y; y = _T; }


static unsigned int gcd(unsigned int p, unsigned int y);
static int COMPARE(unsigned int n, const char * const a, unsigned int i, unsigned int j, unsigned int k, unsigned int s);

////////////////////////////////////////////////////////////////////////////////////////////////////
static unsigned int gcd(unsigned int p, unsigned int y)
{
	unsigned int a, b;
	if (p < y) { a = y; b = p; }
	else if (p > y) { a = p; b = y; }
	else { return p; }
	do {
		unsigned int r = a % b;
		a = b;
		b = r;
	} while (b != 0);
	return a;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static int COMPARE(unsigned int n, const char * const a, unsigned int i, unsigned int j, unsigned int k, unsigned int s)
{
	// returns 0 when s comparisons have been reached or the
	// number of comparisons (positive or negative, depending
	// on whether A or B won, respectively) until the first inequality

	assert(n > 0u);
	if (k >= s) return 0;
	for (;; k++) {
		int A = a[(i + k) % n];
		int B = a[(j + k) % n];
		if (A == B) {
			//if (k == s - 1)
			if (k + 2 > s) return 0;
		} else if (A < B) { // A wins
			// add a[j], ... a[bj] to EX
			return (int)(k + 1u);
		} else { // A > B -- B wins
			// add a[i], ... a[ai] to EX
			return -(int)(k + 1u);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int COMPARE2(unsigned int nA, unsigned int nB, const char * const a, const char * const b, unsigned int i, unsigned int j)
{
	// compares two strings, not necessarily the same length
	// returns 0 when s comparisons have been reached or the
	// number of comparisons (positive or negative, depending
	// on whether A or B won, respectively) until the first inequality

	unsigned int k, s = MIN2(nA, nB);
	if (s == 0) {
		return (nB < nA) ? -1 : 1;
	}
	for (k = 0;; k++) {
		int A = (int)a[(i + k) % nA];
		int B = (int)b[(j + k) % nB];
		if (A == B) {
			//if (k == s - 1)
			if (k + 2 > s) return 0;
		} else if (A < B) { // A wins
			// add a[j], ... a[bj] to EX
			return (int)(k + 1u);
		} else { // A > B -- B wins
			// add a[i], ... a[ai] to EX
			return -(int)(k + 1u);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int MSP(unsigned int n, const char * const a, unsigned int *d)
{
	if (n <= 1u) {
		*d = (n == 0u) ? 0u : 1u;
		return 0u;
	}
	double middle = 0.5 * n;
	unsigned int middle_floor = floor(middle);
	unsigned int middle_ceil = ceil(middle);
	unsigned int i_max = 0u;
	unsigned int j_max = middle_floor;
	unsigned int k_offset = 0u;
	unsigned int alpha, beta;
	int p = 0; // must be signed
	while (i_max < middle_floor && j_max < n) {
		p = COMPARE(n, a, i_max, j_max, 0u, middle_ceil);
		if (p == 0) {
			k_offset = middle_ceil;
			break;
		} else if (p > 0) {	// p > 0, A[i] wins over B[j]: exclude B
			j_max += p;
		} else {		// p < 0, B[j] wins over A[i]: exclude A
			i_max -= p;
		}
	}
	// special treatment
	if (i_max == 0u && j_max + j_max == n) {
		// q is even (this simultaneously tests whether n is even)
		if (middle_floor > 1u) {
			*d = n >> 1u;	// divide d in half
			return MSP(middle_floor, a, d);
		} else {
			*d = 1u;
			return 0u;
		}
	} else {
		// q is odd
		i_max = i_max % n;
		j_max = j_max % n;
		// ensure that (i_max <= j_max) is always true
		if (i_max > j_max) SWAP(i_max, j_max);
		assert(j_max >= i_max);
		unsigned int k = j_max - i_max;
		unsigned int d_prime = (k == 0u) ? n : gcd(n, k);
		//int j_max_prime = j_max - middle_floor; // j_max_prime can be negative
		//unsigned int s = n - MAX3((int)d_prime, (int)i_max, j_max_prime);
		unsigned int s;
		if (j_max < middle_floor) {
			s = n - MAX2(d_prime, i_max);
		} else {
			unsigned int j_max_prime = j_max - middle_floor;
			s = n - MAX3(d_prime, i_max, j_max_prime);
		}
		//// end block
		p = COMPARE(n, a, i_max, j_max, k_offset, s);
		if (p == 0) {
			if (s + d_prime == n) {
				// q > 1
				assert(d_prime > 0u);
				if (i_max + middle_floor < j_max) {
					// (int)i_max < j_max_prime
					// apply Phase 2 to a[j_max + 1] ... a[middle_floor + d]
					alpha = j_max;
					beta = (middle_floor + d_prime) - 1u;
				} else {
					// apply Phase 2 to a[i_max + 1] ... a[d]
					alpha = i_max;
					beta = d_prime - 1u;
				}
				*d = d_prime;
			} else if (s == n - i_max) {
				// q = 1
				alpha = MIN2(i_max, k) + 1u;
				assert(middle_floor > 0u);
				beta = middle_floor - 1u;
				*d = n;
			} else { // s == n - j_max_prime
				// q = 1
				assert(middle_floor + n >= k);
				unsigned int tmp = (middle_floor + n) - k;
				alpha = MIN2(j_max, tmp) + 1u;
				assert(n > 0u);
				beta = n - 1u;
				*d = n;
			}
		} else if (p > 0) {
			// last COMPARE encountered an inequality, a[i] won over a[j]
			if (i_max < middle_floor) {
				// all js have been excluded
				alpha = i_max;
				assert(middle_floor > 0u);
				beta = middle_floor - 1u;
			} else {
				// all is have been excluded
				alpha = j_max + p;
				assert(n > 0u);
				beta = n - 1u;
			}
			*d = n; // because if q > 1, last COMPARE in Phase 1 always terminates with s equalities
		} else { // p < 0
			// last COMPARE encountered an inequality, a[j] won over a[i]
			if (j_max < n) {
				// all is have been excluded
				alpha = j_max;
				assert(n > 0u);
				beta = n - 1u;
			} else {
				// all js have been excluded
				assert((int)i_max >= p);
				alpha = i_max - p;
				assert(middle_floor > 0u);
				beta = middle_floor - 1u;
			}
			*d = n; // because if q > 1, last COMPARE in Phase 1 always terminates with s equalities
		}
	}

	unsigned int gamma = alpha;
	unsigned int sigma = gamma + 1u;
	while (sigma <= beta) {
		assert(beta >= gamma);
		p = COMPARE(n, a, gamma, sigma, 0u, beta - gamma);
		if (p < 0) {
			// sigma wins
			gamma -= p;
			gamma = MAX2(sigma, gamma);
			sigma = gamma + 1u;
		} else if (p > 0) {
			// gamma wins or (p = beta - gamma) equalities are encountered
			sigma = sigma + p;
		} else {
			// interrupt after encountering (beta - gamma) equalities
			break;
		}
	}
	// Output gamma as a minimal starting point
	return gamma;
}

void minrep_inplace(char* a, unsigned int n)
{
	// create minimal representation of a string a of lenght n
	// in-place:
	
	// first call MSP
        unsigned int d,
	             msp = MSP(n, a, &d);
	if (msp == 0u) return;

	unsigned int i,
                     join = n - msp;
	if (join == msp) {
		// Simplest case: join == msp,
		// note also that strlen(a) is even in this case
		for (i = 0; i < msp; i++) {
			SWAP(a[i], a[msp + i]);
		}
	} else {
		unsigned int move_ops, source;
		// shift source point after full cycle, until
		// the number of move operations is equal to n:
		for (move_ops = 0u, source = msp; move_ops < n; source++) {
			char tmp = a[source % n];
			for (i = source + join;; i += join) {
				i %= n;
				SWAP(a[i], tmp);
				move_ops++;
				if (i == source) break;
			}
		}
	}
}
void minrep_inplace_msp(char* a, unsigned int n, unsigned int msp)
{
	// create minimal representation of a string a of lenght n
	// in-place:
	
	if (msp == 0u) return;

	unsigned int i,
                     join = n - msp;
	if (join == msp) {
		// Simplest case: join == msp,
		// note also that strlen(a) is even in this case
		for (i = 0; i < msp; i++) {
			SWAP(a[i], a[msp + i]);
		}
	} else {
		unsigned int move_ops, source;
		// shift source point after full cycle, until
		// the number of move operations is equal to n:
		for (move_ops = 0u, source = msp; move_ops < n; source++) {
			char tmp = a[source % n];
			for (i = source + join;; i += join) {
				i %= n;
				SWAP(a[i], tmp);
				move_ops++;
				if (i == source) break;
			}
		}
	}
}
void cycle_shift(char* dest, const char * const src, unsigned int n, unsigned int msp)
{
	// simply cyclically shifts the string to obtain a minimal 
	// representation
	// Note: the destination string is *not* zero-terminated
	unsigned int i;
	char *eos = dest + n;
	for (i = msp; dest < eos; i++, dest++) {
		i %= n;
		*dest = src[i];
	}
}

void minrep(char* dest, const char * const src, unsigned int n)
{
	// a wrapper around cycle_shift
	unsigned int d;
	cycle_shift(dest, src, n, MSP(n, src, &d));
}
