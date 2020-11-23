#include "Dota2MemReader.h"


std::vector<unsigned int> offsets = { 0x0, 0x28, 0x38, 0x70, 0x170, 0x0, 0x1E4 };
const char* basePtr1 = "\xE0\x0A\x32\xBE\xA6\x02\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x60\x06\x30\x37\xA7\x02\x00\x00";
const char* mask1 = "?????xxxxxxxxxxxxxxxxxxx?????xxx";
const char* basePtr2 = "\xE0\x0A\x32\xBE\xA6\x01\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x60\x06\x30\x37\xA7\x01\x00\x00";
const char* mask2 = "?????xxxxxxxxxxxxxxxxxxx?????xxx";

uintptr_t vbEAddr;
void* vbeBaseAddr;
HANDLE hProcess;
int initHack()
{

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



//Not Visible = 06(radiant team) , 10(dire team) ; Visible = 14 
//Update address
//AOB = 06 00 00 00 ?? ?? ?? ?? F? 7F 00 00 ?? ?? ?? 0? 00 00 00 00 0? 00 00 00 00 00 00 00 ?0 0? ?? ?? = 32bytes (50results)
//AOB sig  = 06 00 00 00 ?0 ?? ?? ?? F? 7F 00 00 ?? ?? ?? 0? 00 00 00 00 0? 00 00 00 00 00 00 00 ?0 0? ?? ?? ?? 0? 00 00 0? 00 00 00 00 00 00 00 0? 00 00 00 00 00 00 00 ?0 ?? ?? ?? ?? 0? 00 00 ?? 00 00 00 00 00 00 00 0? FF FF FF = 72bytes (1result)
//BaseAddr = engine2.dll + 00??????
//Offset usually ends with  = 170 0 1E4
