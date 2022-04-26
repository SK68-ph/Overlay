#include "Dota2MemReader.h"



std::vector<unsigned int> offsets;
const char* baseAddrPtr1 = "\xE0\x0D\x42\xCD\xF4\x01\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x40\x07\x20\xE1\xF3\x01\x00\x00";
const char* maskPtr1 = "?????xxxxxxxxxxxxxxxxxxx?????xxx";
const char* baseAddrPtr2 = "\x40\x04\x13\x87\x4E\x02\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x60\x06\xAF\x6F\x4D\x02\x00\x00";
const char* maskPtr2 = "?????xxxxxxxxxxxxxxxxxxx?????xxx";
const char* patch = "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\x05\x17\xFA\x03\x03\x48\x8B\xFA\x41\x8B\x18\x8B\x88\xB8\x00\x00\x00\x83\xE9\x0F\xF7\xC1\xFB\xFF\xFF\xFF\x74\x17\x33\xC9\xE8\x41\x5C\xF2\xFF\x83\xF8\x01\x76\x0B\x8B\xC8\xB8\x01\x00\x00\x00\xD3\xE0\x21\xD8\x39\x9F\x04\x0E\x00\x00\x74\x06\x89\x9F\x04\x0E\x00\x00\x48\x8B\x5C\x24\x30\x48\x83\xC4\x20\x5F\xC3";
const char* patchMask = "xxxxxxxxxxxxx??xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx??xx??xxxxxxxxxxxxxxxxxxxxxxxxx";

uintptr_t vbEAddr;
void* vbeBaseAddr;
HANDLE hProcess;
DWORD procId;


std::vector<unsigned int> getOffsetFromText() {

    std::fstream file;
    std::string word;
    std::vector<std::string> offsets;
    std::vector<unsigned int> offsetsInt;

    file.open("offsets.ini", std::ios::out | std::ios::in);
    if (file.fail()) {
        std::cout << "Failed to load offsets.ini ..." << std::endl;
        system("pause");
        exit(-1);
    }

    int i = 0;
    while (file >> word)
    {
        if (i >= 8) break;

        offsets.push_back(word);
        i++;
    }
    for (size_t i = 0; i < offsets.size(); i++)
    {
        std::istringstream buffer(offsets[i]);
        unsigned long long value;
        buffer >> std::hex >> value;
        offsetsInt.push_back(value);
    }

    return offsetsInt;
}


void bytePatch() {
    uintptr_t baseAddr = (uintptr_t)PatternScanExModule(hProcess, procId, L"client.dll", patch, patchMask);
    if (baseAddr == NULL)
    {
        std::cout << "Failed to patch byte ..." << std::endl;
        system("pause");
        exit(-1);
    }
    baseAddr += 0x3D;
    byte toPatch = 0x09;
    WriteProcessMemory(hProcess, (BYTE*)baseAddr, &toPatch, sizeof(toPatch), nullptr);
}


void initHack()
{
    offsets = getOffsetFromText();
    //Get ProcId of the target process
    procId = GetProcId(L"dota2.exe");
    if (procId == 0)
    {
        std::cout << "Process not found ..." << std::endl;
        system("pause");
        exit(-1);
    }

    std::cout << "Process id found = " << procId << std::endl;
    hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, NULL, procId);

    //Get Handle to Process
    if (hProcess == 0)
    {
        std::cout << "Failed to attach process. Try running with administrative previlages." << std::endl;
        system("pause");
        exit(-1);
    }
    std::cout << "Handle successfully created ..." << std::endl;
    bytePatch();
}


int getVbe() {

    // First Scan engine2.dll for baseaddress.
    if (vbeBaseAddr == NULL)
    {
        vbeBaseAddr = PatternScanExModule(hProcess, procId, L"engine2.dll", baseAddrPtr1, maskPtr1);
        if (vbeBaseAddr == NULL) // Retry with second offset
        {
            OutputDebugString(L"Retrying AOB with offset 2...\n");
            vbeBaseAddr = PatternScanExModule(hProcess, procId, L"engine2.dll", baseAddrPtr2, maskPtr2);
        }
        if (vbeBaseAddr == NULL) { OutputDebugString(L"BaseAddress AOB Pattern not found...\n"); return NULL; } // try to scan again later.
    }

    // Scan for vbe address using our offset and base address
    if (vbEAddr == NULL)
    {
        uintptr_t baseAddr = (uintptr_t)vbeBaseAddr; // Fix bug duplicate pointers
        baseAddr += offsets[0];
        vbEAddr = FindDMAAddy(hProcess, (uintptr_t)vbeBaseAddr, offsets);
        if (vbEAddr == 0) {
            vbEAddr = NULL;
            OutputDebugString(L"vbeaddr not found");
            return NULL;
        }
    }

    int vbEVal = 0;
    ReadProcessMemory(hProcess, (BYTE*)vbEAddr, &vbEVal, sizeof(vbEVal), nullptr);
    //Test Value
    if (vbEVal == 14 || vbEVal == 30) // Visible by enemy (normal game 14, custom game 30)
    {
        return 14;
    }
    else if (vbEVal == NULL)
    {
        vbEAddr = NULL;
        return NULL;
    }
}


void close()
{
    CloseHandle(hProcess);
}

