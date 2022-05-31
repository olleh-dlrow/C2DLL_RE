#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#define EXP31 0x80000000

struct MagicInfo
{
	int nMagic;
	int nExpInc;
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

	g_result.nMagic = dQuotient + 1;
	if (divisor < 0) {
		g_result.nMagic = -g_result.nMagic;
	}

	g_result.nExpInc = shift - 32;

	return &g_result;
}

struct MagicInfo* CalculateMagicInfo(int divisor) {
	int var_8 = 31;
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
	unsigned int absDivisor = abs(divisor);
	unsigned int esi1 = ((unsigned int)divisor >> 31) - EXP31;	// nLargestMultiple
	unsigned int eax1 = absDivisor / esi1;		// div edi
	// 
	esi1 = esi1 - esi1 % absDivisor;	// sub esi, edx
	esi1--;
	eax1 = EXP31 / esi1;				// div esi		// q1
	unsigned int ebp1 = eax1;					// mov ebp, eax	
	eax1 = (int)eax1 * (int)esi1;					// imul eax, esi
	unsigned int ebx1 = EXP31;
	ebx1 -= eax1;						// r1
	eax1 = EXP31 / absDivisor;					// div edi	// nMagicNumber
	unsigned int ecx1 = eax1;
	eax1 = (int)eax1 * (int)absDivisor;			// imul eax, edi
	unsigned int edx1 = EXP31 - eax1;				// sub edx, eax
	// loc_1075EB47
	do {
		// mov eax, var_8
		// inc eax
		eax1 = var_8 + 1;
		ebx1 = ebx1 + ebx1;			// add ebx, ebx
		ebp1 = ebp1 + ebp1;
		var_8 = eax1;
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
	g_result.nMagic = eax1;	// mov     ds:dword_1079F090, eax
	if ((int)ecx1 < 0) {
		// loc_1078F41C
		eax1 = -(int)eax1;
		g_result.nMagic = eax1; // mov     ds : dword_1079F090, eax
	}
	// loc_1075EB87
	ecx1 = var_8;
	eax1 = (int)&g_result; //mov eax, offset dword_1079F090;
	ecx1 += -32; //0x0FFFFFFE0
	g_result.nExpInc = ecx1; // mov     ds : dword_1079F094, ecx
	return &g_result;
}

// 以下代码还原修改自VC++6.0 bin目录下c2.dll(版本12.0.9782.0)，文件偏移5EACE，
// 原程序的返回值定义为结构体，这里修改为参数返回
int GetMagic(int nDivC, int* nOutExpInc)
{
	if ((int)nDivC >= 3 && nDivC < 13)
	{
		*nOutExpInc = g_MagicInfoArray[nDivC].nExpInc;
		return g_MagicInfoArray[nDivC].nMagic;
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

int main(int argc, char* argv[]) {
	int divisor = 4;
	int out = 0;

	DWORD BaseAddress = (DWORD)LoadLibraryA("C2.DLL");
	if (BaseAddress == 0) {
		printf("LoadLibraryA Failed!, %d", GetLastError());
		system("pause");
		return -1;
	}

	MagicInfo* (__fastcall * OrigFunc)(int) = (MagicInfo * (__fastcall*)(int))(BaseAddress + 0x5EACE);

	int wrongRes = 0;
	for (int i = -2147483648; i <= 2147483647; i++) {
		if (i != 0) {
			if (i % 1000000 == 0)printf("[ok] for divisor < %d\n", i);
			divisor = i;
			if ((OrigFunc(divisor)->nMagic != CalculateMagicInfo(divisor)->nMagic) ||
				(OrigFunc(divisor)->nExpInc != CalculateMagicInfo(divisor)->nExpInc)) {
				printf("error at: %d\n", i);
				printf("correct magic, expInc: 0x%08x 0x%08x\n", OrigFunc(divisor)->nMagic, OrigFunc(divisor)->nExpInc);
				printf("my magic, expInc: 0x%08x 0x%08x\n", CalculateMagicInfo(divisor)->nMagic, CalculateMagicInfo(divisor)->nExpInc);
				wrongRes++;
				//system("pause");
			}
		}
		if (i == 2147483647) {
			printf("[ok] for divisor <= 2147483647\n");
			break;
		}
	}
	if (wrongRes == 0)
		printf("Verified all divisors from INT_MIN to INT_MAX,\nYour result is absolutely right!!!");

	getchar();
	return 0;
}

