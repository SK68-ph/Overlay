#include "Dota2MemReader.h"



std::vector<unsigned int> offsets;
const char* baseAddrPtr1 = "\xE0\x0D\x42\xCD\xF4\x01\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x40\x07\x20\xE1\xF3\x01\x00\x00";
const char* baseAddrPtr2 = "\x40\x04\x13\x87\x4E\x02\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x60\x06\xAF\x6F\x4D\x02\x00\x00";
const char* baseAddrMask = "?????xxxxxxxxxxxxxxxxxxx?????xxx";

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
}


int getVbe() {

    // First Scan engine2.dll for baseaddress.
    if (vbeBaseAddr == NULL)
    {
        vbeBaseAddr = PatternScanExModule(hProcess, procId, L"engine2.dll", baseAddrPtr1, baseAddrMask);
        if (vbeBaseAddr == NULL) // Retry with second offset
        {
            OutputDebugString(L"Retrying AOB with offset 2...\n");
            vbeBaseAddr = PatternScanExModule(hProcess, procId, L"engine2.dll", baseAddrPtr2, baseAddrMask);
        }
        if (vbeBaseAddr == NULL) { OutputDebugString(L"BaseAddress AOB Pattern not found...\n"); return Waiting; } // try to scan again later.
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
            return Waiting;
        }
    }

    int vbeVal = 0;
    ReadProcessMemory(hProcess, (BYTE*)vbEAddr, &vbeVal, sizeof(vbeVal), nullptr);

    if (vbeVal == 0 && vbeVal != NULL) // Visible by enemy value = 0
    {
        return Visible;
    }
    else if (vbeVal == NULL)
    {
        vbEAddr = NULL;
        return NULL;
    }
}


void close()
{
    CloseHandle(hProcess);
}

