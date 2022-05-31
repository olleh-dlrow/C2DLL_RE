#include <iostream>
#include <malloc.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>   

#define for if(0){}else for

// ���ڳ�����3��13֮�䣬ֱ�Ӳ����ṹ����
struct MagicNumber
{
	int nMagic;
	int nExpInc;
};

// ���ڳ���Ϊ2���ݣ������������ʱ����޶�Ӧֵ
struct MagicNumber MagicTable[] = {
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


#define EXP31 0x80000000

// ���´��뻹ԭ�޸���VC++6.0 binĿ¼��c2.dll(�汾12.0.9782.0)���ļ�ƫ��5EACE��
// ԭ����ķ���ֵ����Ϊ�ṹ�壬�����޸�Ϊ��������
int GetMagic(int nDivC, int* nOutExpInc)
{
	//   if ((int)nDivC >= 3 && nDivC < 13)
	//   {
	//     *nOutExpInc = MagicTable[nDivC].nExpInc;
	//     return MagicTable[nDivC].nMagic;
	//   }

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

int main(int argc)
{
	int nExpInc;
	int nMagicNumber;

	nMagicNumber = GetMagic(7, &nExpInc);
	printf("nMagicNumber = 0x%08x, ExpInc = %d\r\n", nMagicNumber, nExpInc);

	int nDividend = argc - 201; // ���Ǳ�����
	int nDivisor = -100;      // ���ǳ���
	int nQuotient;            // ��������

	nDividend = 2147483647;
	nDivisor = -2147483648;
	// GetMagic��������magic number��
	// ��һ������ָ���������ڶ�������OUTָ�����32������
	// �����������ģ�����70 / -7�Ľ��
	do
	{
		if (nDivisor % 1 == 0)printf("%d\n", nDivisor);
		nMagicNumber = GetMagic(nDivisor, &nExpInc);
		//printf("nMagicNumber = 0x%08x, ExpInc = %d\r\n", nMagicNumber, nExpInc);

		if (nDivisor >= 0)
		{
			__asm
			{
				mov eax, nMagicNumber // ������������immѰַ��nMagicNumber�����ڱ����ڼ����
				mov esi, nDividend
				imul esi

				// ��������������������ת��
				// ��Ϊ����׶ξͼ����nMagicNumber��ȡֵ�ˣ�
				// ���Ա����ڼ�Ϳ��Ծ����Ƿ��������addָ��,
				// nMagicNumberС��0x80000000(����)��������add
				test nMagicNumber, 80000000h
				jz NEXT1
				add edx, esi
				NEXT1 :
				mov ecx, nExpInc
					sar edx, cl
					shr esi, 31
					add edx, esi
					mov nQuotient, edx
			}
		}
		else
		{
			__asm
			{
				mov eax, nMagicNumber
				mov esi, nDividend
				imul esi

				test nMagicNumber, 80000000h
				jnz NEXT2
				sub edx, esi
				NEXT2 :
				mov ecx, nExpInc
					sar edx, cl
					mov ecx, edx
					shr ecx, 31
					add edx, ecx
					mov nQuotient, edx
			}
		}

		//printf("%d / %d = %d\r\n", nDividend, nDivisor, nQuotient);
		//printf("%d / %d = %d\r\n", nDividend, nDivisor, nDividend / nDivisor);
		if (nQuotient != nDividend / nDivisor)
		{
			puts("Error");
			printf("%d / %d = %d\r\n", nDividend, nDivisor, nQuotient);
			printf("%d / %d = %d\r\n", nDividend, nDivisor, nDividend / nDivisor);
			getchar();
			//break;
		}

		nDivisor++;
		if (nDivisor == 0 || nDivisor == -1 || nDivisor == 1)
		{
			nDivisor = 2;
		}
		//nDividend += 10;
	} while (nDivisor <= 2147483647);

	return 0;
}