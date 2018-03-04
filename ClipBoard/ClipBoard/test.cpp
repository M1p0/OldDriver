#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )  //隐藏dos窗口
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

int Program_Mutex()
{
    HANDLE hMutex = NULL;
    LPCTSTR lpszName = "APPMutex";
    int nRet = 0;

    hMutex = ::CreateMutex(NULL, FALSE, lpszName);
    DWORD dwRet = ::GetLastError();

    switch (dwRet)
    {
        case 0:
        {
            break;
        }
        case ERROR_ALREADY_EXISTS:
        {
            cout << "program is already running" << endl;
            nRet = 1;
            break;
        }
        default:
        {
            cout << "create mutex failed" << endl;
            nRet = -1;
            break;
        }
    }
    return nRet;
}

void deleteAllMark(string &s, const string &mark)
{
    unsigned int nSize = mark.size();
    while (true)
    {
        unsigned int pos = 0;
        pos = s.find(mark, pos);
        if (pos == string::npos)
        {
            return;
        }
        s.erase(pos, nSize);
    }
}

string GetTextFromClipboard()
{
    string Str;
    if (!IsClipboardFormatAvailable(CF_TEXT))
        return "Error";  //内容不是CF_TEXT
    if (OpenClipboard(NULL))
    {
        HANDLE hClip;
        hClip = GetClipboardData(CF_TEXT);  //获得剪贴板的内存块首地址
        Str = (char*)GlobalLock(hClip);     //将地址放入Str并锁定内存
        GlobalUnlock(hClip);                //解锁内存
        CloseClipboard();
        return Str;
    }
    else
        return "Error";
}


bool CopyToClipboard(string szData)
{
    if (!IsClipboardFormatAvailable(CF_TEXT))
        return false;  //内容不是CF_TEXT

    if (OpenClipboard(NULL))
    {
        EmptyClipboard();  //清空剪切板
        HANDLE hClip;
        char* buffer;
        hClip = GlobalAlloc(GMEM_MOVEABLE, szData.size() + 1);//分配全局内存
        buffer = (char *)GlobalLock(hClip);                   //将分配到的内存地址保存在buffer并锁定内存  
        strcpy(buffer, szData.c_str());                       //把数据拷贝到buffer指向的内存
        GlobalUnlock(hClip);                                  //解锁内存
        SetClipboardData(CF_TEXT, hClip);                     //向剪切板插入内容
        CloseClipboard();
        return true;
    }
    return false;
}

bool Check(string str)    //判断是否完全是英文
{
    for (unsigned int i = 0; i < str.size(); i++)
    {
        if (str[i] <= 'Z'&&str[i] >= 'A' || str[i] <= 'z'&&str[i] >= 'a')
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

int main()
{
    if (Program_Mutex() != 0)   //确保只有一个实例
        return 0;
    string Str;
    string Str_Last;
    const string Head = "magnet:?xt=urn:btih:";
    while (true)
    {
        Str = GetTextFromClipboard();
        deleteAllMark(Str, " ");                           //删除空格
        if (Str.size() == 40 || Str.size() == 32 && Check(Str))
        {
            CopyToClipboard(Head + Str);
        }
        Sleep(1);
    }
    return 0;
}