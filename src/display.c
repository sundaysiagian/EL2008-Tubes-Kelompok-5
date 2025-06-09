#include <stdio.h>
#include <string.h>
#include <ctype.h>

void make_lower(char *s);

int main()
{
    // Deklarasi
    char s1[50], s2[50];

    // Menerima input dr pengguna
    printf("Masukkan kata ke-1: ");
    fgets(s1, 50, stdin);

    printf("Masukkan kata ke-2: ");
    fgets(s2, 50, stdin);

    // Bikin semua string lowercase (fitur case-insensitive)
    make_lower(s1);
    make_lower(s2);

    // Filter 1: Length
    if(strlen(s1)!=strlen(s2))
    {
        printf("Kedua kata tidak anagram");
        return 0;
    }

    // Filter 2: Cek per kata
    int n=strlen(s1);
    char temp;
    for(int i=0; i<n-1; i++)
    {
        for(int j=i+1; j<n; j++)
        {
            if(s1[i]>s1[j])
            {
                temp=s1[i];
                s1[i]=s1[j];
                s1[j]=temp;
            }
            if(s2[i]>s2[j])
            {
                temp=s2[i];
                s2[i]=s2[j];
                s2[j]=temp;
            }
        }
    }

    for(int i=0; i<n-1; i++)
    {
        if(s1[i]!=s2[i])
        {
            printf("Kedua kata tidak anagram");
            return 0;
        }
    }

    printf("Kedua kata anagram");
    return 0;
}

void make_lower(char *s)
{
    int length=strlen(s);
    for(int i=0; i<length; i++)
    {
        s[i]=tolower(s[i]);
    }
}

