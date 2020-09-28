#include "Dota2MemReader.h"


std::vector<unsigned int> offsets = { 0x40, 0x98, 0x170, 0x0, 0x418, 0x20, 0xE04 };
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
            OutputDebugString(L"Failed to attach process. Try running with administrative previlages\n");
            return -1;
        }
        OutputDebugString(L"Handle successfully created...\n");
        return 0;
    }
    else
    {
        OutputDebugString(L"Process not found.\n");
        return -1;
    }
}

int getVbe() {

    if (vbeBaseAddr == NULL)
    {
        vbeBaseAddr = PatternScanExModule(hProcess, L"dota2.exe", L"engine2.dll", "\x00\x18\x23\x2E\x16\x02\x00\x00\x10\x00\x00\x00\x00\x00\x00\x00\x25\x00\x00\x00\x00\x00\x00\x00\x00\x08\x58\x12\x16\x02\x00\x00", "??????xxxxxxxxxxxxxxxxxxx?????xx");
        if (vbeBaseAddr == NULL) { OutputDebugString(L"Pattern not found...\n"); return NULL; }
        _RPT1(0, "Vbe BaseAddress = %p\n", vbeBaseAddr);
    }

    if (vbEAddr == 0)
    {
        vbEAddr = FindDMAAddy(hProcess, (uintptr_t)vbeBaseAddr, offsets);
        if (vbEAddr == 0) {
            _RPT1(0, "VBE ADDR NULL = %p\n", vbEAddr);
            vbEAddr = NULL;
            return -1;
        }
        OutputDebugString(L"Overlay success...\n");
        _RPT1(0, "Vbe Address = %p\n", vbEAddr);
    }

    int vbEVal = 0;
    ReadProcessMemory(hProcess, (BYTE*)vbEAddr, &vbEVal, sizeof(vbEVal), nullptr);
    //Test Value
    if (vbEVal == 14) // Visible by enemy
    {
        return 14;
    }
    else if(vbEVal >= 6 && vbEVal <= 10) // Not visible by enemy
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
    offsets.empty();
    vbeBaseAddr = NULL;
    vbEAddr = NULL;
    CloseHandle(hProcess);
}


//Not Visible = 06(radiant team) , 10(dire team) ; Visible = 14 
//Update address
//AOB = 06 00 00 00 ?? ?? ?? ?? F? 7F 00 00 ?? ?? ?? 0? 00 00 00 00 0? 00 00 00 00 00 00 00 ?0 0? ?? ?? = 32bytes (50results)
//AOB sig  = 06 00 00 00 ?0 ?? ?? ?? F? 7F 00 00 ?? ?? ?? 0? 00 00 00 00 0? 00 00 00 00 00 00 00 ?0 0? ?? ?? ?? 0? 00 00 0? 00 00 00 00 00 00 00 0? 00 00 00 00 00 00 00 ?0 ?? ?? ?? ?? 0? 00 00 ?? 00 00 00 00 00 00 00 0? FF FF FF = 72bytes (1result)
//BaseAddr = engine2.dll + 00??????
//Offset usually ends with  = 170 0 1E4
