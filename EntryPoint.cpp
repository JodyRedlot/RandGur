#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <windows.h> 
#include <WinInet.h>
#include <iostream>
#include <ctime>
#include <direct.h>

#pragma comment(lib, "wininet.lib")

using std::vector;
using std::string;

int count, id = 1;
bool newfile;

int GetRandInt(int min, int max)
{
	return min + rand() % (max - (min - 1));
}

string RandString(int len)
{
	string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXTZabcdefghiklmnopqrstuvwxyz";
	string output;
	int index;

	for (int i = 0; i < len; i++)
		output += chars[GetRandInt(0, chars.size() - 1)];

	return output;
}

bool GetPic(char* url, char* path, char* name)
{
	DWORD size;
	BYTE* tmp[1024];
	HINTERNET internet, open;
	FILE* file = NULL;
	char header[32] = "\0", out[255];
	
	internet = InternetOpen("randgur", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	sprintf(out, "%s%s", path, name);

	if (!(open = InternetOpenUrl(internet, url, header, strlen(header), INTERNET_FLAG_DONT_CACHE, 0)))
	{
		printf("Error while InternetOpenUrl");
		InternetCloseHandle(internet);
		return false;
	}

	newfile = true;
	do
	{
		if (!InternetReadFile(open, tmp, 1024, &size))
		{
			printf("Error while InternetReadFile\n");
			InternetCloseHandle(internet);
			return false;
		}
		if ((!size && newfile)
			|| ((BYTE)tmp[19] == 0xAC && (BYTE)tmp[23] == 0x81)  //file was removed
			|| ((BYTE)tmp[19] == 0x54 && (BYTE)tmp[23] == 0x74)  //file isn't exist
			|| ((BYTE)tmp[19] == 0xCC && (BYTE)tmp[23] == 0xCC)) //there is no file
		{
			InternetCloseHandle(internet);
			return false;
		}
		else if (!size)
			break;
		else
		{
			if (newfile)
			{
				if (!(file = fopen(out, "wb")))
				{
					printf("Error while fopen\n");
					InternetCloseHandle(internet);
					return false;
				}
				printf("%i. %s\n", id, url);
			}

			fwrite(tmp, sizeof(BYTE), size, file);
		}
		newfile = false;
	}
	while (true);

	if (file)
	{
		fflush(file);
		fclose(file);
	}

	InternetCloseHandle(internet);
	return true;
}

void Download()
{
	char dir[255];
	GetCurrentDirectory(strlen(dir), dir);
	sprintf(dir, "%s\\RandGur\\", dir);
	if (_mkdir(dir) && errno != EEXIST)
	{
		printf("Error during creating folder!\n");
		return;
	}

	printf("Starting download...\n");
	for (;id <= count;)
	{
		string name = RandString(GetRandInt(5, 7));\
		string url = "http://i.imgur.com/" + name + ".jpg";
		name += ".jpg";
		
		if(GetPic, (char*)url.c_str(), dir, (char*)name.c_str())
			id++;
	}
	printf("Finishing download...\n");
}

void main()
{
	srand(time(NULL));
	SetConsoleTitle("RandGur");
	printf("Enter pics count: ");
	scanf_s("%i", &count);
	system("cls");
	
	Download();

	system("pause");
}