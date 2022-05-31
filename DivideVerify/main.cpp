#include<MagicInfo.h>
#include<assert.h>

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
	printf("Start to verify (INT_MIN / d) for d in range(INT_MIN, INT_MAX)...\n");
	DivideVerify(argc - 1 + INT_MIN);
	return 0;
}