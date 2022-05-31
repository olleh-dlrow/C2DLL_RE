#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<Windows.h>
#include<assert.h>
#include<limits.h>
#define EXP31 0x80000000
//#define INT_MIN (1<<31)
//#define INT_MAX ~(1<<31)

struct MagicInfo
{
	int magicNumber;
	int expInc;
};

struct MagicInfo g_result;

struct MagicInfo g_MagicInfoArray[] = {
  {1, 1},           // 0 
  {1, 1},           // 1
  {1, 1},           // 2
  {0x55555556, 0},
  {0, 0},           // 4
  {0x66666667, 1},
  {0x2AAAAAAB, 0},
  {0x92492493, 2},
  {0, 0},           // 8
  {0x38E38E39, 1},
  {0x66666667, 2},
  {0x2E8BA2E9, 1},
  {0x2AAAAAAB, 1}
};

// 保留逆向代码的备份
struct MagicInfo* CalculateMagicInfoBackup(int divisor) {
	int p = 31;
	int var_4 = divisor;
	// ecx: divisor
	// edx: pExpInc
	if (divisor >= 3 && divisor < 13) {
		return &g_MagicInfoArray[divisor];
	}
	// loc_1075EAEA
	// eax <- divisor
	// cdq: edx = eax >> 31(SIGN)
	// edi <- eax
	// xor edi, edx
	// sub edi, edx
	// edi: absDivisor
	unsigned int eax1;
	unsigned int esi1;
	unsigned int absDivisor;
	unsigned int ebp1, ebx1, ecx1, edx1;
	unsigned int q1, r1, q2, r2, nc, delta;

	absDivisor = abs(divisor);
	//esi1 = ((unsigned)divisor >> 31) - EXP31;	// nLargestMultiple
	nc = ((unsigned)divisor >> 31) - EXP31;

	// eax1 =  esi1 / absDivisor;		// div edi 
	//esi1 = esi1 - esi1 % absDivisor;	// sub esi, edx
	//esi1--;
	nc = nc - nc % absDivisor;
	nc--;


	//eax1 =  EXP31 / esi1;				// div esi		// q1
	q1 = EXP31 / nc;


	//ebp1 = eax1;					// mov ebp, eax	//q1
	//eax1 = (int)eax1 * (int)esi1;					// imul eax, esi
	//ebx1 = EXP31;
	//ebx1 -= eax1;						// r1
	r1 = EXP31 - (unsigned)((int)q1 * (int)nc);

	//eax1 = EXP31 / absDivisor;					// div edi	// nMagicNumber
	q2 = EXP31 / absDivisor;

	//ecx1 = eax1;
	//eax1 = (int)eax1 * (int)absDivisor;			// imul eax, edi
	//edx1 = EXP31 - eax1;				// sub edx, eax
	r2 = EXP31 - unsigned((int)q2 * (int)absDivisor);
	// loc_1075EB47
	// LOOP_BEGIN
	do {
		// mov eax, var_8
		// inc eax
		//eax1 = var_8 + 1;
		p += 1;
		//ebx1 = ebx1 + ebx1;			// add ebx, ebx
		r1 += r1;
		//ebp1 = ebp1 + ebp1;
		q1 += q1;

		//var_8 = eax1;
		if (r1 >= nc) { // 
		//if (ebx1 >= esi1) {
			// loc_1078F407
			//ebp1++;
			q1 += 1;
			//ebx1 -= esi1;
			r1 -= nc;
		}
		// loc_1075EB5C
		//edx1 += edx1;
		r2 += r2;
		//ecx1 += ecx1;
		q2 += q2;
		if (r2 >= absDivisor) { // 
		//if (edx1 >= absDivisor) {	//	cmp edx, edi
			//ecx1++;
			q2 += 1;
			//edx1 -= absDivisor;
			r2 -= absDivisor;
		}
		// loc_1075EB3F
		//eax1 = absDivisor;
		//eax1 -= edx1;
		delta = absDivisor - r2;
		//if (ebp1 < eax1)goto LOOP_BEGIN;
		//if (ebp1 == eax1) {
		//	// loc_1078F40F
		//	if (ebx1 == 0)goto LOOP_BEGIN;
		//	break;
		//}
	} while ((q1 < delta) || (q1 == delta && r1 == 0));//   
	//} while ((ebp1 < eax1) || (ebp1 == eax1 && ebx1 == 0));
	// loc_1075EB6F
	//eax1 = ecx1 + 1;
	//ecx1 = var_4;
	//g_result.magicNumber = eax1;	// mov ds:dword_1079F090, eax
	g_result.magicNumber = q2 + 1;
	if (divisor < 0) { // 
	//if((int)ecx1 < 0) {
		// loc_1078F41C
		//eax1 = -(int)eax1;
		//g_result.magicNumber = eax1; // mov ds : dword_1079F090, eax
		g_result.magicNumber = ~(q2 + 1) + 1;
		//g_result.magicNumber *= -1;
	}
	// loc_1075EB87
	//ecx1 = p;
	// eax1 = (int) & g_result; //mov eax, offset dword_1079F090;
	//ecx1 += -32; //0x0FFFFFFE0
	//g_result.expInc = ecx1; // mov ds : dword_1079F094, ecx
	g_result.expInc = p - 32;
	return &g_result;
}

struct MagicInfo* CalculateMagicInfoOrigin(int divisor) {
	int p = 31;
	int var_4 = divisor;
	// ecx: divisor
	// edx: pExpInc
	if (divisor >= 3 && divisor < 13) {
		return &g_MagicInfoArray[divisor];
	}
	// loc_1075EAEA
	// eax <- divisor
	// cdq: edx = eax >> 31(SIGN)
	// edi <- eax
	// xor edi, edx
	// sub edi, edx
	// edi: absDivisor
	unsigned int eax1;
	unsigned int esi1;
	unsigned int absDivisor;
	unsigned int ebp1, ebx1, ecx1, edx1;

	absDivisor = abs(divisor);
	esi1 = ((unsigned)divisor >> 31) - EXP31;	// nLargestMultiple

	eax1 =  esi1 / absDivisor;		// div edi 
	esi1 = esi1 - esi1 % absDivisor;	// sub esi, edx
	esi1--;

	eax1 =  EXP31 / esi1;				// div esi		// q1


	ebp1 = eax1;					// mov ebp, eax	//q1
	eax1 = (int)eax1 * (int)esi1;					// imul eax, esi
	ebx1 = EXP31;
	ebx1 -= eax1;						// r1

	eax1 = EXP31 / absDivisor;					// div edi	// nMagicNumber

	ecx1 = eax1;
	eax1 = (int)eax1 * (int)absDivisor;			// imul eax, edi
	edx1 = EXP31 - eax1;				// sub edx, eax

	// loc_1075EB47
	// LOOP_BEGIN
	do {
		// mov eax, var_8
		// inc eax
		//eax1 = var_8 + 1;
		p += 1;
		ebx1 = ebx1 + ebx1;			// add ebx, ebx
		ebp1 = ebp1 + ebp1;

		//var_8 = eax1;
		if (ebx1 >= esi1) {
			// loc_1078F407
			ebp1++;
			ebx1 -= esi1;
		}
		// loc_1075EB5C
		edx1 += edx1;
		ecx1 += ecx1;
		if (edx1 >= absDivisor) {	//	cmp edx, edi
			ecx1++;
			edx1 -= absDivisor;
		}
		// loc_1075EB3F
		eax1 = absDivisor;
		eax1 -= edx1;
		//if (ebp1 < eax1)goto LOOP_BEGIN;
		//if (ebp1 == eax1) {
		//	// loc_1078F40F
		//	if (ebx1 == 0)goto LOOP_BEGIN;
		//	break;
		//}
	} while ((ebp1 < eax1) || (ebp1 == eax1 && ebx1 == 0));
	// loc_1075EB6F
	eax1 = ecx1 + 1;
	ecx1 = var_4;
	g_result.magicNumber = eax1;	// mov ds:dword_1079F090, eax
	if((int)ecx1 < 0) {
		// loc_1078F41C
		eax1 = -(int)eax1;
		g_result.magicNumber = eax1; // mov ds : dword_1079F090, eax
	}
	// loc_1075EB87
	ecx1 = p;
	// eax1 = (int) & g_result; //mov eax, offset dword_1079F090;
	ecx1 += -32; //0x0FFFFFFE0
	g_result.expInc = ecx1; // mov ds : dword_1079F094, ecx
	return &g_result;
}

struct MagicInfo* CalculateMagicInfo(int divisor) {
	int p = 31;
	if (divisor >= 3 && divisor < 13) {
		return &g_MagicInfoArray[divisor];
	}

	unsigned int q1, r1, q2, r2, nc, delta, absDivisor;

	absDivisor = abs(divisor);
	nc = ((unsigned)divisor >> 31) - EXP31;
	nc = nc - nc % absDivisor - 1;
	q1 = EXP31 / nc;
	r1 = EXP31 % nc;
	q2 = EXP31 / absDivisor;
	r2 = EXP31 % absDivisor;
	do {
		p += 1;
		r1 *= 2;
		q1 *= 2;
		if (r1 >= nc){
			q1 += 1;
			r1 -= nc;
		}
		r2 *= 2;
		q2 *= 2;
		if (r2 >= absDivisor) {
			q2 += 1;
			r2 -= absDivisor;
		}
		delta = absDivisor - r2;
	} while ((q1 < delta) || (q1 == delta && r1 == 0));

	g_result.magicNumber = q2 + 1;
	if (divisor < 0) {
		g_result.magicNumber = -g_result.magicNumber;//~(q2 + 1) + 1;
	}

	g_result.expInc = p - 32;
	return &g_result;
}

// 以下代码还原修改自VC++6.0 bin目录下c2.dll(版本12.0.9782.0)，文件偏移5EACE，
// 原程序的返回值定义为结构体，这里修改为参数返回
int GetMagic(int nDivC, int* nOutExpInc)
{
	   if ((int)nDivC >= 3 && nDivC < 13)
	   {
	     *nOutExpInc = g_MagicInfoArray[nDivC].expInc;
	     return g_MagicInfoArray[nDivC].magicNumber;
	   }

	unsigned int nAbsDivC = abs(nDivC);
	int nExcBase = 31;

	// t = 2^31 if nDivC > 0
	// or t = 2^31 + 1 if nDivC < 0
	unsigned int t = (nDivC >> 31) + EXP31;

	// |nc| = t - 1 - rem(t, |nDivC|)
	unsigned int nLargestMultiple = t - t % nAbsDivC - 1;
	unsigned int q1 = EXP31 / nLargestMultiple;
	unsigned int r1 = EXP31 - nLargestMultiple * q1;
	unsigned int nMagicNumber = EXP31 / nAbsDivC;
	unsigned int r2 = EXP31 - nAbsDivC * nMagicNumber;

	do
	{
		r1 *= 2;
		q1 *= 2;
		++nExcBase;
		if (r1 >= nLargestMultiple)
		{
			++q1;
			r1 -= nLargestMultiple;
		}
		r2 *= 2;
		nMagicNumber *= 2;
		if (r2 >= nAbsDivC)
		{
			++nMagicNumber;
			r2 -= nAbsDivC;
		}
	} while (q1 < nAbsDivC - r2 || q1 == nAbsDivC - r2 && !r1);

	nMagicNumber++;

	if ((int)nDivC < 0)
		nMagicNumber = -(int)nMagicNumber;

	*nOutExpInc = nExcBase - 32;

	return nMagicNumber;
}

// absolutely right!!!
struct MagicInfo* __fastcall CalcMagicNumber(int divisor) {
	if (divisor >= 3 && divisor < 13) {
		return &g_MagicInfoArray[divisor];
	}

	unsigned int AdRemain, dQuotient, dRemain, Ad, d, AdQuotient;
	unsigned int shift = 31;
	if (divisor >= 0) {
		d = divisor;
		Ad = 0x7fffffff - 0x80000000 % d;
	}
	else {
		d = ~divisor + 1;
		Ad = 0x80000000 - 0x80000001 % d;
	}
	AdQuotient = 0x80000000 / Ad;
	AdRemain = 0x80000000 % Ad;
	dQuotient = 0x80000000 / d;
	dRemain = 0x80000000 % d;

	do {
		shift++;
		AdRemain = AdRemain * 2;
		AdQuotient = AdQuotient * 2;
		if (AdRemain >= Ad) {
			AdQuotient++;
			AdRemain = AdRemain - Ad;
		}
		dRemain = dRemain * 2;
		dQuotient = dQuotient * 2;
		if (dRemain >= d) {
			dQuotient++;
			dRemain = dRemain - d;
		}
	} while (AdQuotient < d - dRemain || (AdQuotient == d - dRemain && AdRemain == 0));

	g_result.magicNumber = dQuotient + 1;
	if (divisor < 0) {
		g_result.magicNumber = -g_result.magicNumber;
	}

	g_result.expInc = shift - 32;

	return &g_result;
}

void test1() {
	int divisor = 1;
	int out = 0;

	//printf("divisor:%d, 0x%08x\n", INT_MAX, CalculateMagicInfo(INT_MAX)->magicNumber);

	int lower = INT_MIN + 1;
	int upper = INT_MAX;
	for (int i = lower; i <= upper; i++) {
		if (i != 0) {
			divisor = i;
			if (divisor % 100000 == 0)printf("[ok] %d\n", divisor);
			// printf("divisor:%d, 0x%08x\n", divisor, CalcMagicNumber(divisor)->magicNumber);
			if (GetMagic(divisor, &out) != CalculateMagicInfo(divisor)->magicNumber) {
				printf("error: %d\n", i);
				printf("book's magic: 0x%08x\n", GetMagic(divisor, &out));
				printf("my: 0x%08x\n", CalculateMagicInfo(divisor)->magicNumber);
				printf("\n");
				system("pause");
			}
		}
	}
}

void test2() {
	DWORD quotient = 0;
	DWORD remain = 0;
	__asm {
		push eax
		push ebx
		push edx

		mov ebx, 0
		mov eax, 0x80000000
		div ebx

		mov quotient, eax
		mov remain, edx

		pop edx
		pop ebx
		pop eax
	}
	printf("q...r: 0x%08x, 0x%08x\n", quotient, remain);
}

void _verify(int i) {
	if (CalcMagicNumber(i)->magicNumber != CalculateMagicInfoOrigin(i)->magicNumber ||
		CalcMagicNumber(i)->expInc != CalculateMagicInfoOrigin(i)->expInc) {
		printf("error: %d\n", i);
		printf("correct magic: 0x%08x\n", CalcMagicNumber(i)->magicNumber);
		printf("my magic: 0x%08x\n", CalculateMagicInfoOrigin(i)->magicNumber);
		system("pause");
	}
}

void verify() {
	int lower = INT_MIN;
	int upper = INT_MAX;
	for (int i = lower; i <= upper; i++) {
		if (i == 0)continue;
		if (i % 10000 == 0)printf("%d\n", i);
		_verify(i);
		if (i == INT_MAX)break;
	}
	return;
}

bool IsPowerOfTwo(int divisor, int* pExp) {
	unsigned int absDivisor = abs(divisor);
	bool oneVisited = false;
	for (int i = 0; i < 32; i++) {
		int bit = absDivisor & 0x1;
		if (bit == 1) {
			if (oneVisited)return false;
			else {
				oneVisited = true;
				*pExp = i;
			}
		}
		absDivisor >>= 1;
	}
	return oneVisited == true;
}

int _DivideVerify(int dividend, int divisor) {
	assert(divisor != 0);
	if (divisor == 1)return dividend;
	if (divisor == -1)return -dividend;
	// power of 2
	int exp = 32;
	if (divisor != INT_MIN && IsPowerOfTwo(divisor, &exp)) {
		int eax = dividend;
		int edx = eax >= 0 ? 0 : -1;
		edx = edx & (abs(divisor) - 1);
		eax += edx;
		eax = eax >> exp;
		if (divisor < 0) {
			eax = -eax;
		}
		return eax;
	}
	// not power of 2
	else {
		struct MagicInfo* magicInfo = CalculateMagicInfo(divisor);
		int magicNumber = magicInfo->magicNumber;
		int shiftAdd = magicInfo->expInc;
		int quotient = 0;
		if (divisor >= 0) {
			__asm {
				mov eax, magicNumber
				mov esi, dividend
				imul esi

				cmp magicNumber, 0x7fffffff
				jna NEXT1
				add edx, esi
				NEXT1 :
				mov ecx, shiftAdd
				sar edx, cl
				shr esi, 31
				add edx, esi
				mov quotient, edx
			}
		}
		else {
			__asm {
				mov eax, magicNumber
				mov esi, dividend
				imul esi

				cmp magicNumber, 0x7fffffff
				ja NEXT2
				sub edx, esi
				NEXT2 :
				mov ecx, shiftAdd
				sar edx, cl
				mov ecx, edx
				shr ecx, 31
				add edx, ecx
				mov quotient, edx
			}
		}

		return quotient;
	}
}

void DivideVerify(int dividend) {
	FILE* fp;
	const char* filename = "divide_verify.txt";
	if (fopen_s(&fp, filename, "w") != 0) {
		printf("open error: %s\n", filename);
		return;
	}
	int wrongRes = 0;
	int lower = INT_MIN;
	int upper = INT_MAX;
	for (int i = lower; i <= upper; i++) {
		if (i == 0)continue;
		if (i % 1000000 == 0)printf("[ok] for divisor < %d\n", i);

		int correct = 0;
		if (dividend == INT_MIN && i == -1)correct = -dividend;
		else correct = dividend / i;
		int res = _DivideVerify(dividend, i);
		if (correct != res) {
			wrongRes++;
			printf("error: %d\n", i);
			printf("cor: %d\n", correct);
			printf("my:  %d\n", res);

			fprintf(fp, "error: %d / %d\n", dividend, i);
			fprintf(fp, "cor: %d\n", correct);
			fprintf(fp, "my:  %d\n", res);
			fprintf(fp, "===================\n");
		}

		if (i == INT_MAX) {
			printf("[ok] for divisor <= 2147483647\n");
			break;
		}
	}
	fclose(fp);
	if (wrongRes == 0) {
		printf("Verified (INT_MIN / d) for d in range(INT_MIN, INT_MAX)\n"
			   "Your result is absolutely right!!!");
	}
}

int main(int argc) {
	// test1();
	//for (int i = 4; i < 100; i *= 2) {
	//	printf("i: %d\n", i);
	//	printf("my magic: 0x%08x\n", CalculateMagicInfo(i)->magicNumber);
	//	printf("my expin: %d\n", CalculateMagicInfo(i)->expInc);
	//}
	//printf("my magic: 0x%08x\n", CalculateMagicInfo(2)->magicNumber);
	//printf("my expin: %d\n", CalculateMagicInfo(2)->expInc);

	//DivideVerify(argc - 1 + INT_MIN);
	verify();
	//test1();
	//const int a = 715827883;
	//printf("0x%08x\n", CalculateMagicInfo(a)->magicNumber);
	//printf("0x%08x\n", CalculateMagicInfo(-a)->magicNumber);
	//printf("%d\n", argc / a);
	//printf("%d\n", argc / -a);

	return 0;
}