/*
	本项目需要在能够加载C2.dll的操作系统下运行
	推荐运行环境：win7
*/

#include<MagicInfo.h>

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
			if ((OrigFunc(divisor)->magicNumber != CalculateMagicInfo(divisor)->magicNumber) ||
				(OrigFunc(divisor)->expInc != CalculateMagicInfo(divisor)->expInc)) {
				printf("error at: %d\n", i);
				printf("correct magic, expInc: 0x%08x 0x%08x\n", OrigFunc(divisor)->magicNumber, OrigFunc(divisor)->expInc);
				printf("my magic, expInc: 0x%08x 0x%08x\n", CalculateMagicInfo(divisor)->magicNumber, CalculateMagicInfo(divisor)->expInc);
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

