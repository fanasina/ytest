#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

//#include "/home/fanasina/progr_/ptens0neD/src/permutation/permutation.h"

#include "src/permutation/permutation.h"

int sign(int a) {
	if (a < 0) return -1;
	return 1;
}

bool isPermutation(int* perm, setInit se, int sz) {
	std::vector<int> tmp;
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (perm[i] == se.setinit[j]) {
				if (find(tmp.begin(), tmp.end(), j) == tmp.end()) {
					tmp.push_back(j);
					break;
				}
			}
		}
	}
	return tmp.size() == sz;

}

int permutation::signature() {
	int ss = 1;
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			ss *= sign(perm[j] - perm[i]);// * sign(j - i);
		}
	}
	return ss;
}

int signature(int* tab, int sz) {
	int ss = 1;
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			ss *= sign(tab[j] - tab[i]) * sign(j - i);
		}
	}
	return ss;
}

template<typename T>
void permutation::permute(T* dst, T* src) {
	for (int i = 0; i < size;i++) {
		dst[i] = src[perm[i]];
	}
}

template
void permutation::permute(int* dst, int* src);
template
void permutation::permute(float* dst, float* src);

// complexité sz*(sz+1)/2
size_t TabToPlaceAlgo(int* tb, int sz) {
	int cnt = 0;
	int pl;

	int* tPlace = new int[sz];

	for (int i = sz - 1; i >= 0; i--) {
		cnt++;
		pl = 0;
		for (int j = i + 1; j < sz; j++) {
			cnt++;
			if (tb[j] < tb[i]) {
				pl++;
			}
			if (pl == tb[i]) break;
		}
		tPlace[tb[i]] = pl;

	}
	size_t q = 0;
	for (int i = 0; i < sz;i++) {
		cnt++;
		//printf("tPlace[%d] == %d et tb[%d] == %d\n", i, tPlace[i], i, tb[i]);
		q = (i + 1) * q + tPlace[i];
	}
	//printf("algo cnt = %d ", cnt);
	return q;
}

// complexité sz*(sz+1)/2
size_t TabToPlaceOpt1(int* tb, int sz) {
	int cnt = 0;
	int mx;
	int* tPlace = new int[sz];
	for (int i = sz - 1; i >= 0; i--) {
		cnt++;
		if (i == sz - 1) {
			mx = tb[i];
			tPlace[mx] = 0;
		}
		else if (mx > tb[i]) {
			int pli = 0; // si c est le plus à droite 0 si pas de superieur à lui, on incremente si on trouve plus petit
			for (int j = sz - 1; j > i; j--) {
				cnt++;
				if (tb[i] > tb[j]) {
					pli++;
				}
				else if (tb[i] == tb[j]) {
					//return -1; // something wrong
					throw "something wrong here, tb[i]==tb[j]";
				}
			}
			tPlace[tb[i]] = pli;
		}
		else if (mx < tb[i]) {
			mx = tb[i];
			tPlace[mx] = sz - 1 - i;
		}
		else {
			//return -1; // something wrong
			throw "something wrong here, tb[i]==mx";

		}
	}
	size_t q = 0;
	for (int i = 0; i < sz; i++) {
		cnt++;
		//printf("tab tPlace[%d] == %d et tb[%d] == %d [ q=%d, cnt = %d\n", i, tPlace[i], i, tb[i], q, cnt);
		q = (i + 1) * q + tPlace[i];
	}
	//printf("Opt cnt = %d ", cnt);

	return q;
}

// complexité sz*(sz+1)
size_t TabToPlaceNotab(int* tb, int sz) {
	int cnt = 0;
	int mx = sz - 1;
	size_t q = 0;
	int pl;
	for (int i = 0; i < sz; i++) {
		cnt++;
		int j;
		for (j = 0; j < sz;j++) {
			cnt++;
			if (tb[j] == i) break;
		}
		pl = 0;
		j++;
		for (;j < sz;j++) {
			cnt++;
			if (tb[j] < i) {
				pl++;
			}
			if (pl == i) break;
		}
		q = (i + 1) * q + pl;
		//q = (sz - tb[i]) * q + pl;
		//printf("notab tPlace[tb[%d]] == %d et tb[%d] == %d [ q=%d, cnt = %d\n", i, pl, i, tb[i], q, cnt);

	}
	//printf(" notab cnt = %d ", cnt);

	return q;
}





//complexité sz*sz/2
void PlaceToTab(int* tb, size_t pl, int sz) {
	int cnt = 0;
	size_t a = pl;
	int pltbi;
	int size = 1;
	// s'assurer que tb soit null
	for (int i = 0;i < sz;i++) tb[i] = 0;

	for (int i = sz - 1; i >= 0; i--) {
		cnt++;
		pltbi = a % (i + 1);
		a /= (i + 1);
		if (i == sz - 1) {
			tb[sz - 1 - pltbi] = i;
		}
		else {
			int lt = 0, j = sz - 1;
			while (lt < pltbi && j >= 0) {
				cnt++;
				if (tb[j--] < i) {
					lt++;
				}
			}
			while (tb[j] > i) {
				cnt++;
				j--;
			}
			tb[j] = i;

		}
	}
	//printf("cnt PlaceToTab :%d  ", cnt);
}

size_t factorial(int n) {
	size_t ret = 1;
	for (size_t i = 2; i <= n; i++) {
		ret *= i;
	}
	return ret;
}

// src1 o src0 = dst; dst(i) =  src1(src0(i))
void compose(int* dst, int* src0, int* src1, int sz) {
	for (int i = 0; i < sz; i++) {
		dst[i] = src1[src0[i]];
	}
}
// src1 o src0 = dst; dst(i) =  src1(src0(i))
void compose(size_t& rdst, size_t psrc0, size_t psrc1, int sz) {
	int dst[sz], src0[sz], src1[sz];
	PlaceToTab(src0, psrc0, sz);
	PlaceToTab(src1, psrc1, sz);
	for (int i = 0; i < sz; i++) {
		dst[i] = src1[src0[i]];
	}
	rdst = TabToPlaceOpt1(dst, sz);
}
/*
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
}
*/
void permuteArray(int* dst, int* src, int* perm, int sz) {
	for (int i = 0; i < sz; i++) {
		dst[i] = src[perm[i]];
	}
}

void inverseArray(int* dst, int* src, int sz) {
	for (int i = 0; i < sz; i++) {
		dst[src[i]] = i;
	}
}

// seek?/ seek o src = seek(src)=dst => seek = dst o inv(src)
void permCorrespondance(int* sk, int* dst, int* src, int sz) {
	int inv[sz];
	inverseArray(inv, src, sz);
	compose(sk, dst, inv, sz);
}


// SRC o transf = DST,   SRC:{a,b,c,d,g,f} o transf = DST:{g,b,d,f,c,a}
// SRC[0]=a, SRC[1]=b, SRC[2]=c SRC[3]=d SRC[4]=g SRC[5]=f 
// DST[0]=g=SRC[4] DST[1]=b=SRC[1] DST[2]=d=SRC[3] DST[3]=f=SRC[5] DST[4]=c=SRC[2] DST[5]=a=SRC[1]
// trans[0]=4 trans[1]=1 trans[2]=3 trans[3]=5 trans[4]=2 trans[5]=0  
// (*cmp) (a,b) = 0 if a==b, -1 if a<b , 1 if a>b
template<typename T>
void CorrespondacePerm(T* src, T* dst, int* transf, int sz, int (*cmp)(T, T)) {
	int tmp[sz];
	std::vector<int> tmpV;
	int curt = 0;
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (cmp(dst[i], src[j]) == 0) {
				if (std::find(tmpV.begin(), tmpV.end(), j) == tmpV.end()) {// not found
					transf[i] = j;
					tmpV.push_back(j);
					break;
				}
			}
		}
	}
}
template void CorrespondacePerm<char>(char* src, char* dst, int* transf, int sz, int (*cmp)(char, char));