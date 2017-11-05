#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )  //����dos����
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;



void deleteAllMark(string &s, const string &mark)
{
    unsigned int nSize = mark.size();
    while (1)
    {
        unsigned int pos=0;
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
        return "Error";  //���ݲ���CF_TEXT
    if (OpenClipboard(NULL))
    {
        HANDLE hClip;
        hClip = GetClipboardData(CF_TEXT);  //��ü�������ڴ���׵�ַ
        Str = (char*)GlobalLock(hClip);     //����ַ����Str�������ڴ�
        GlobalUnlock(hClip);                //�����ڴ�
        CloseClipboard();
        return Str;
    }
    else
        return "Error";
}


bool CopyToClipboard(string szData)
{
    if (!IsClipboardFormatAvailable(CF_TEXT))
        return false;  //���ݲ���CF_TEXT

    if (OpenClipboard(NULL))
    {
        EmptyClipboard();  //��ռ��а�
        HANDLE hClip;
        char* buffer;
        hClip = GlobalAlloc(GMEM_MOVEABLE, szData.size() + 1);//����ȫ���ڴ�
        buffer = (char *)GlobalLock(hClip);                   //�����䵽���ڴ��ַ������buffer�������ڴ�  
        strcpy(buffer, szData.c_str());                       //�����ݿ�����bufferָ����ڴ�
        GlobalUnlock(hClip);                                  //�����ڴ�
        SetClipboardData(CF_TEXT, hClip);                     //����а��������
        CloseClipboard();
        return true;
    }
    return false;
}



int main()
{
    string Str;
    string Str_Last;
    const string Head = "magnet:?xt=urn:btih:";
    while (true)
    {
        Str = GetTextFromClipboard();
        deleteAllMark(Str, " ");                           //ɾ���ո�
        if (Str.size() == 40 || Str.size() == 32)
        {
            CopyToClipboard(Head + Str);
        }
        Sleep(1);
    }


    return 0;
}