#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include <string>
#include "mem.h"

using namespace std;

uintptr_t moduleBase;
uintptr_t dynamicPtrBaseAddr;
HANDLE hProcess;
DWORD procId;
boolean godMode = false;
boolean infAmmo = false;
boolean recoilE = true;

void hookClient()
{
	// Get Proccess ID
	procId = GetProcId(L"ac_client.exe");

	// Get the Base Address 
	moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

	// Get Handle to Process
	hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

}



void updateHealth(int newHealth)
{
	// Resolve the base address 
	dynamicPtrBaseAddr = moduleBase + 0x00109B74; // Base Pointer Address

	// Offsets
	std::vector<unsigned int> healthOffsets = { 0xF8 };
	// ammoAddress
	uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);

	cout << healthAddr << endl;


	//Read Ammo value
	int currentHealth = 0;

	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &currentHealth, sizeof(currentHealth), nullptr);

	// Write Value to Address
	WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

	// Re-read the Address Value
	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &currentHealth, sizeof(currentHealth), nullptr);

}

void updateAmmo(int newBullets)
{
	// Resolve the base address 
	dynamicPtrBaseAddr = moduleBase + 0x00109B74; // Base Pointer Address

	// Offsets
	std::vector<unsigned int> ammoOffsets = { 0x378, 0x14, 0x0 };
	// ammoAddress
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

	cout << ammoAddr << endl;

	//Read Ammo value
	int ammoValue = 0;

	cout << ammoValue << endl;

	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

	// Write Value to Address
	int newAmmo = newBullets;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

	// Re-read the Address Value
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

	cout << ammoValue << endl;

}

void toggleGod()
{
	godMode = !godMode;
	if (godMode == true)
	{ // \x29\x7B\x04 - Original Opcode | sub [ebx+04],edi
		mem::NopEx((BYTE*)(moduleBase + 0x29D1F), 3, hProcess);
		cout << "ON" << endl;
	}
	else
	{ // NOP
		mem::PatchEx((BYTE*)(moduleBase + 0x29D1F), (BYTE*)"\x29\x7B\x04", 3, hProcess);
		cout << "OFF" << endl;
	}
}

void toggleAmmo()
{
	infAmmo = !infAmmo;
	if (infAmmo == true)
	{
		
		mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x06", 2, hProcess);
	} 
	else
	{ 
		mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2, hProcess);
	}

}

void updateArmor(int newArmor)
{
	// Resolve the base address 
	dynamicPtrBaseAddr = moduleBase + 0x109B74; // Base Pointer Address

	// Offsets
	std::vector<unsigned int> ammoOffsets = { 0xFC };
	// ammoAddress
	uintptr_t armorAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

	cout << armorAddr << endl;

	//Read Ammo value
	int armorValue = 0;

	cout << armorValue << endl;

	ReadProcessMemory(hProcess, (BYTE*)armorAddr, &armorValue, sizeof(armorValue), nullptr);

	// Write Value to Address
	WriteProcessMemory(hProcess, (BYTE*)armorAddr, &newArmor, sizeof(newArmor), nullptr);

	// Re-read the Address Value
	ReadProcessMemory(hProcess, (BYTE*)armorAddr, &armorValue, sizeof(armorValue), nullptr);

	cout << armorValue << endl;
}

void toggleRecoil()
{
	recoilE = !recoilE;
	if (recoilE == false)
	{
		mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
	}
	else
	{ // \n50\n8D\n4C\n24\n1C\n51\n8B\nCE\nFF\nD2
		mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\n50\n8D\n4C\n24\n1C\n51\n8B\nCE\nFF\nD2", 10, hProcess);
	}

}





HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main()
{	
	//////////////////////////////////////////////////////////
	/////////////////      Loading         ///////////////////
	//////////////////////////////////////////////////////////
	SetConsoleTextAttribute(hConsole, 12);
	cout << "Loading Assult Cube Trainer." << endl;
	Sleep(250);
	cout << "Loading Assult Cube Trainer. ." << endl;
	Sleep(500);
	cout << "Loading Assult Cube Trainer. . ." << endl;
	Sleep(750);
	cout << "Loading Assult Cube Trainer. . . ." << endl;
	Sleep(1000);
	SetConsoleTextAttribute(hConsole, 10);
	cout << "Loaded!";
	Sleep(1000);
	cout << "\x1B[2J\x1B[H";
	// Main Hook
	hookClient();
	//////////////////////////////////////////////////////////
	/////////////////      Loading         ///////////////////
	//////////////////////////////////////////////////////////


	// Main "Loop"
	while (true == true)
	{
		// Input Option
		string option = "nil";

		SetConsoleTextAttribute(hConsole, 12);
		cout << "\n" <<
			+"           _____ _____        _    _ _   _______    _____ _    _ ____  ______  \n" <<
			+"    /\    / ____/ ____|  /\  | |  | | | |__   __|  / ____| |  | |  _ \|  ____| \n" <<
			+"   /  \  | (___| (___   /  \ | |  | | |    | |    | |    | |  | | |_) | |__    \n" <<
			+"  / /\ \  \___ \\___ \ / /\ \| |  | | |    | |    | |    | |  | |  _ <|  __|   \n" <<
			+" / ____ \ ____) |___) / ____ \ |__| | |____| |    | |____| |__| | |_) | |____  \n" <<
			+"/_/    \_\_____/_____/_/    \_\____/|______|_|     \_____|\____/|____/|______| \n" <<
			+"  _______ _____            _____ _   _ ______ _____   \n" <<
			+" |__   __|  __ \     /\   |_   _| \ | |  ____|  __ \  \n" <<
			+"    | |  | |__) |   /  \    | | |  \| | |__  | |__) | \n" <<
			+"    | |  |  _  /   / /\ \   | | | . ` |  __| |  _  /  \n" <<
			+"    | |  | | \ \  / ____ \ _| |_| |\  | |____| | \ \  \n" << 
			+"    |_|  |_|  \_\/_/    \_\_____|_| \_|______|_|  \_\ \n" << endl;

		SetConsoleTextAttribute(hConsole, 15);
		cout << "Made by - ssh#8423" << endl;
		cout << endl;
		cout << "[-------------------------------------]" << endl;
		cout << "[--------Please Pick an Option--------]" << endl;
		cout << "[-------------------------------------]" << endl;
		cout << "[1] Health Cheat" << endl;
		cout << "[2] Ammo Cheat" << endl;
		cout << "[3] God Mode" << endl;
		cout << "[4] Infinite Ammo" << endl;
		cout << "[5] Armor Cheat" << endl;
		cout << "[6] No Recoil Cheat" << endl;
		cout << "[10] Exit" << endl;
		cout << "[-------------------------------------]" << endl;
		getline(std::cin, option);
		if (option == "1")
		{
			string enteredHealth;
			cout << "Enter new health: ";
			getline(std::cin, enteredHealth);
			updateHealth(stoi(enteredHealth));
		}
		else if (option == "2")
		{
			string enteredBullets;
			cout << "Enter new ammo value: ";
			getline(std::cin, enteredBullets);
			updateAmmo(stoi(enteredBullets));
		}
		else if (option == "3")
		{
			toggleGod();
		}
		else if (option == "4")
		{
			toggleAmmo();
		}
		else if (option == "5")
		{
			string enteredArmor;
			cout << "Enter new armor value: ";
			getline(std::cin, enteredArmor);
			updateArmor(stoi(enteredArmor));
		}
		else if (option == "6")
		{
			toggleRecoil();
		}
		else if (option == "10")
		{
			exit(-1);
		}








		cout << "\x1B[2J\x1B[H";
	}

	return 0;
}