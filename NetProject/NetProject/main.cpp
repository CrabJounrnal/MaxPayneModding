#include <windows.h>
#include <stdio.h>
//#include <math.h>

#pragma comment(linker,"/merge:.rdata=.text")
//#pragma comment(linker,"/merge:.data=.text")

struct vector3 {
	float x, y, z;
};
union Address {
	void* ptr;
	int i;
};

void *HookedPtr = nullptr;
/*

HookedPtr - 0x4E = X_Character ??

float matrix[] = HookedPtr;
arcsin(matrix[0]) = ���� �������� �� ��������� (������ ���������)
���������: �������-������ vector2 v
matrix[0] = v.x
matrix[8] = v.x
matrix[2] = v.y
matrix[6] = -v.y

X_Character + 0x72 (HookedPtr + 0x24) = coords (vector3)
X_Character + 0x22E = X_CharacterProperties
X_CharacterProperties + 0x85 (X_Character + 0x2B3) = health (float)

�������� ����� (_thiscall, �� ����� ������������ ��� ������� _fastcall (this, �����, ...) [�����]) 
�� ������ 0x00729ED0([this = (void)*(X_Character + 0x398)], (int??)*(X_Character + 0x728))
���������� ��������� �� ������ X_GlobalPainkillerSettings??

*(int*)X_GlobalPainkillerSettings = painkillers_amount

*/

extern "C" void _cdecl IGiveUCharacter();

void _stdcall FromAsm(void *character) { 
	vector3 *pos = (vector3*)(((float*)character)+9);
	if (HookedPtr == nullptr) {
		char buf[64];
		sprintf(buf, "x = %f, y = %f, z = %f\n", pos->x, pos->y, pos->z);
		MessageBoxA(nullptr, buf, "Hooked!", MB_OK);
	}
	HookedPtr = character;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpReserved ) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		// setting hooks
		Address HookPlace;
		HookPlace.i = 0x0504403;
		DWORD old;
		VirtualProtect(HookPlace.ptr, 5, PAGE_EXECUTE_READWRITE, &old);
		*(void**)HookPlace.ptr = (void*)((char*)IGiveUCharacter - ((char*)HookPlace.ptr + 4));
	}
    return TRUE;
}