#include "Dota2MemReader.h"


std::vector<unsigned int> offsets;
const char* basePtr1 = "\xE0\x0D\x42\xCD\xF4\x01\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x40\x07\x20\xE1\xF3\x01\x00\x00";
const char* mask1 = "?????xxxxxxxxxxxxxxxxxxx?????xxx";
const char* basePtr2 = "\x40\x04\x13\x87\x4E\x02\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x60\x06\xAF\x6F\x4D\x02\x00\x00";
const char* mask2 = "?????xxxxxxxxxxxxxxxxxxx?????xxx";
uintptr_t vbEAddr;
void* vbeBaseAddr;
HANDLE hProcess;


std::vector<unsigned int> getOffsetFromText() {

    std::fstream file;
    std::string word;
    std::vector<std::string> offsets;
    std::vector<unsigned int> offsetsInt;

    file.open("offsets.ini", std::ios::out | std::ios::in);
    if (file.fail()) {
        OutputDebugString(L"Failed to load config file...");
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

int initHack()
{
    offsets = getOffsetFromText();
    //Get ProcId of the target process
    DWORD procId = GetProcId(L"dota2.exe");
    if (!procId == 0)
    {
        _RPT1(0, "Process id found = %d\n", procId);
        hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, NULL, procId);
        //Get Handle to Process
        if (hProcess == 0)
        {
            MessageBox(NULL, L"Failed to attach process. Try running with administrative previlages.", L"Error", MB_ICONERROR);
            return -1;
        }
        OutputDebugString(L"Handle successfully created...\n");
        return 0;
    }
    else
    {
        MessageBox(NULL, L"Process not found.", L"Error", MB_ICONERROR);
        return -1;
    }
}

int getVbe() {

    // First Scan engine2.dll for baseaddress.
    if (vbeBaseAddr == NULL)
    {//                                                                 
        vbeBaseAddr = PatternScanExModule(hProcess, L"dota2.exe", L"engine2.dll", basePtr1, mask1);
        if (vbeBaseAddr == NULL) // Retry with second offset
        {
            OutputDebugString(L"Retrying AOB with offset 2...\n");
            vbeBaseAddr = PatternScanExModule(hProcess, L"dota2.exe", L"engine2.dll", basePtr2, mask2);
        }
        if (vbeBaseAddr == NULL) { OutputDebugString(L"BaseAddress AOB Pattern not found...\n"); return NULL; } // try to scan again later.
    }

    // Scan for vbe address using our offset and base address
    if (vbEAddr == 0)
    {
        vbEAddr = FindDMAAddy(hProcess, (uintptr_t)vbeBaseAddr, offsets);
        if (vbEAddr == 0) {
            vbEAddr = NULL;
            return -1;
        }
    }

    int vbEVal = 0;
    ReadProcessMemory(hProcess, (BYTE*)vbEAddr, &vbEVal, sizeof(vbEVal), nullptr);
    //Test Value
    if (vbEVal == 14) // Visible by enemy
    {
        return 14;
    }else if(vbEVal >= 6 && vbEVal <= 10) // Not visible by enemy
    {
        return 6;
    }
    else
    {
        vbEAddr = NULL;
        return 0;
    }
}


void close()
{
    CloseHandle(hProcess);
}

