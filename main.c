#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <windows.h>

int CheckExe (FILE **CheckFile, int *p);
int CheckLenght (FILE **CheckFile, long int *shift, int *p);
int SearchOfSignature(FILE **CheckFile, unsigned char *sign, long int *shift);

int main()
{
    int Result, len, p;
    unsigned char sign[8];
    long int shift;
    char* Res;
    char waysign[MAX_PATH], waycheck[MAX_PATH];
    Res=setlocale(LC_ALL, "Rus");
    if (Res==NULL)
        {
            printf ("Ошибка функции setlocale");
            return 1;
        }

    FILE *SignatureFile;
    Result=printf("Сигнатурный сканер\n\nВведите путь к базе сигнатур:\n");
    if (Result<0)
    {
        printf ("Ошибка вывода");
        return 2;
    }

    Res=fgets(waysign, MAX_PATH, stdin);
    if (Res==NULL)
        {
            printf("Ошибка ввода");
            return 3;
        }
    else
        {
            len=strlen(waysign)-1;
            if (waysign[len]=='\n')
                {
                    waysign[len]='\0';
                }
        }

    if (waysign[0]==' ')
    {
        printf("Введён неверный путь к базе сигнатур");
        return 4;
    }

    SignatureFile=fopen(waysign, "r");
    if (SignatureFile==NULL)
    {
        printf("Ошибка открытия базы сигнатур");
        return 5;
    }

    Result=fscanf(SignatureFile, "%hhd%hhd%hhd%hhd%hhd%hhd%hhd%hhd%li",
               &sign[0], &sign[1], &sign[2], &sign[3],
               &sign[4], &sign[5], &sign[6], &sign[7], &shift);
    if (Result!=9)
    {
        printf("Ошибка считывания из файла");
        fclose(SignatureFile);
        return 6;
    }
    Result=fclose(SignatureFile);
    if (Result!=0)
    {
        printf("Ошибка закрытия базы сигнатур");
        return 7;
    }

    FILE *CheckFile;
    Result=printf("Введите путь к проверяемому файлу:\n");
    if (Result<0)
    {
        printf ("Ошибка вывода");
        return 8;
    }
    Res=fgets(waycheck, MAX_PATH, stdin);
    if (Res==NULL)
        {
            printf("Ошибка ввода");
            return 9;
        }
    else
        {
            len=strlen(waycheck)-1;
            if (waycheck[len]=='\n')
                {
                    waycheck[len]='\0';
                }
        }

    if (waycheck[0]==' ')
        {
            printf("Введён неверный путь к проверяемому файлу");
            return 10;
        }

    CheckFile=fopen(waycheck, "rb");
    if (CheckFile==NULL)
    {
        printf("Ошибка открытия проверяемого файла");
        return 11;
    }

    Result=CheckExe(&CheckFile, &p);
    if ((Result==0) && (p==1)) return 0;
    if (Result==1) return 12;
    if (Result==2) return 13;
    if (Result==3) return 14;

    Result=CheckLenght(&CheckFile, &shift, &p);
    if ((Result==0) && (p==1)) return 0;
    if (Result==1) return 15;
    if (Result==2) return 16;
    if (Result==3) return 17;
    if (Result==4) return 18;
    if (Result==5) return 19;

    Result=SearchOfSignature(&CheckFile, sign, &shift);
    if (Result==0) return 0;
    if (Result==1) return 20;
    if (Result==2) return 21;
    if (Result==3) return 22;
    if (Result==4) return 23;
    if (Result==5) return 24;
    if (Result==6) return 25;
    if (Result==7) return 26;

    Result=fclose(CheckFile);
    if (Result!=0)
    {
        printf("Ошибка закрытия проверяемого файла");
        return 27;
    }

return 0;
}

int CheckExe (FILE **CheckFile, int *p)
{
    char Check_Exe[3];
    int Result;
    if (CheckFile==NULL)
    {
        printf("Ошибка первого аргумента");
        fclose(*CheckFile);
        return 1;
    }
    if (p==NULL)
    {
        printf("Ошибка второго аргумента");
        fclose(*CheckFile);
        return 2;
    }

    Result=fread(Check_Exe, 1, 2, *CheckFile);
    Check_Exe[2]='\0';
    if (Result!=2)
    {
        printf("Ошибка чтения");
        fclose(*CheckFile);
        return 3;
    }
    if ((Check_Exe[0]!='M') || (Check_Exe[1]!='Z'))
    {
        printf("Файл чистый");
        *p=1;
        fclose(*CheckFile);
        return 0;
    }
    return 0;
}

int CheckLenght (FILE **CheckFile, long int *shift, int *p)
{
    long int len1, len2;
    int Result;
    if (CheckFile==NULL)
    {
        printf("Ошибка первого аргумента");
        fclose(*CheckFile);
        return 1;
    }
    if (shift==NULL)
    {
        printf("Ошибка второго аргумента");
        fclose(*CheckFile);
        return 2;
    }
    if (p==NULL)
    {
        printf("Ошибка третьего аргумента");
        fclose(*CheckFile);
        return 3;
    }

    Result=fseek(*CheckFile, 0, SEEK_END);
    if (Result!=0)
    {
        printf("Ошибка перехода в конец файла");
        fclose(*CheckFile);
        return 4;
    }
    len1=ftell(*CheckFile);
    if (Result==-1)
    {
        printf("Ошибка определения размера файла");
        fclose(*CheckFile);
        return 5;
    }
    len2=(*shift)+8;
    if (len2>len1)
    {
        printf("Файл чистый");
        *p=1;
        fclose(*CheckFile);
        return 0;
    }
    return 0;
}

int SearchOfSignature(FILE **CheckFile, unsigned char *sign, long int *shift)
{
    unsigned char checkfile[8];
    int Result;
    if (CheckFile==NULL)
    {
        printf("Ошибка первого аргумента");
        fclose(*CheckFile);
        return 1;
    }
    if (sign==NULL)
    {
        printf("Ошибка второго аргумента");
        fclose(*CheckFile);
        return 2;
    }
    if (shift==NULL)
    {
        printf("Ошибка третьего аргумента");
        fclose(*CheckFile);
        return 3;
    }

    Result=fseek(*CheckFile, *shift, SEEK_SET);
    if (Result!=0)
    {
        printf("Ошибка смещения");
        fclose(*CheckFile);
        return 4;
    }

    Result=fread(checkfile, sizeof(unsigned char), 8, *CheckFile);
    if (Result!=8)
    {
        printf("Ошибка чтения");
        fclose(*CheckFile);
        return 5;
    }

    Result=memcmp(sign, checkfile, 8);
    if (Result==0)
    {
        Result=printf("Файл заражённый");
        if (Result<0)
            {
            printf ("\nОшибка вывода");
            fclose(*CheckFile);
            return 6;
            }
    }
    else
    {
        Result=printf("Файл чистый");
        if (Result<0)
            {
            printf ("\nОшибка вывода");
            fclose(*CheckFile);
            return 7;
            }
    }

return 0;
}
