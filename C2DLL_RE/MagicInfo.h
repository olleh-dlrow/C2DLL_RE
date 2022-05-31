#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#define EXP31 0x80000000

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
		if (r1 >= nc) {
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

	eax1 = esi1 / absDivisor;		// div edi 
	esi1 = esi1 - esi1 % absDivisor;	// sub esi, edx
	esi1--;

	eax1 = EXP31 / esi1;				// div esi		// q1


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
	if ((int)ecx1 < 0) {
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
