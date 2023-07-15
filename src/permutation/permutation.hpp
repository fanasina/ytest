
#ifndef __PERMUTATION_H__
#define __PERMUTATION_H__

#include <stdlib.h>


struct setInit {
	int size;
	int* setinit;
	setInit(int sz = 1, int beg = 0) {
		size = sz;
		setinit = new int[sz];
		for (int i = 0; i < sz; i++) setinit[i] = beg + i;
	}
};

struct permutation {
	int size;// type
	int rang; //place;//rang;
	int* perm;
	permutation(int sz, bool b) {
		size = sz;
		perm = new int[size];
	}
	permutation(int sz = 1, int* tb = { 0 }) {
		size = sz;
		perm = new int[size];
		for (int i = 0; i < size; i++) perm[i] = tb[i];
	}
	//int TabToPlace(int* tb , int sz );
	//void PlaceToTab(int* tb , int pl , int sz);
	int signature();
	template<typename T>
	void permute(T* dst, T* src);
};

bool isPermutation(int* perm, setInit se, int sz);

int signature(int* tab, int sz);

// complexité sz*(sz+1)/2
size_t TabToPlaceAlgo(int* tb, int sz);

// complexité sz*(sz+1)/2
size_t TabToPlaceOpt1(int* tb, int sz);

// complexité sz*(sz+1)
size_t TabToPlaceNotab(int* tb, int sz);

//complexité sz*sz/2
void PlaceToTab(int* tb, size_t pl, int sz);

size_t factorial(int n);

// src1 o src0 = dst; dst(i) =  src1(src0(i))
void compose(int* dst, int* src0, int* src1, int sz);
// src1 o src0 = dst; dst(i) =  src1(src0(i))
void compose(size_t& rdst, size_t psrc0, size_t psrc1, int sz);

/*template<typename T>
void transform(T* dst, T* src, int* perm, int sz);
template<typename T>
void transform(T* dst, T* src, size_t pl, int sz);

template<typename T>
void transform(T* dst, T* src, int* perm, int sz) {
	for (int i = 0; i < sz; i++) {
		dst[i] = src[perm[i]];
	}
}
template<typename T>
void transform(T* dst, T* src, size_t pl, int sz) {
	int perm[sz];
	PlaceToTab(perm, pl, sz);
	for (int i = 0; i < sz; i++) {
		dst[i] = src[perm[i]];
	}
}*/

void permuteArray(int* dst, int* src, int* perm, int sz);
void inverseArray(int* dst, int* src, int sz);

// seek?/ seek o src = seek(src)=dst => seek = dst o inv(src)
void permCorrespondance(int* sk, int* dst, int* src, int sz);

// SRC o transf = DST,   SRC:{a,b,c,d,g,f} o transf = DST:{g,b,d,f,c,a}
// SRC[0]=a, SRC[1]=b, SRC[2]=c SRC[3]=d SRC[4]=g SRC[5]=f 
// DST[0]=g=SRC[4] DST[1]=b=SRC[1] DST[2]=d=SRC[3] DST[3]=f=SRC[5] DST[4]=c=SRC[2] DST[5]=a=SRC[0]
// trans[0]=4 trans[1]=1 trans[2]=3 trans[3]=5 trans[4]=2 trans[5]=0  
// (*cmp) (a,b) = 0 if a==b, -1 if a<b , 1 if a>b
template<typename T>
void CorrespondacePerm(T* src, T* dst, int* transf, int sz, int (*cmp)(T, T));



#endif


