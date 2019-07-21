#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <windows.h>
#include <dos.h>

using namespace std;

void ShowInfo()
{
    printf("     ############");
    printf("(ERICK194)");
    printf("#############\n");   
    printf("     #           BAR EXTRACTOR         #\n");
    printf("     # CREADO POR ERICK VASQUEZ GARCIA #\n");
    printf("     #            VERSION 1.0          #\n");
    printf("     # Use:BAR_EXTARCTOR <filename.bar>#\n");
    printf("     ###################################\n");
    printf("\n");
    system("PAUSE");
    exit(0);
}

typedef unsigned char byte;

int main(int argc, char *argv[])
{
    FILE *f1, *f2;
    int tableoffset;
    int filesize;
    int datasize;
    int tablecout;
    int zipsize;
    int i, j;
    int *table_data;
    char filename[256];
    char dirname[256];
    char zipname[256];
    byte data_;
    
    if(argc==1) 
    {
        ShowInfo();
    }
    
    printf("Open File %s\n",argv[1]);
    
    f1=fopen (argv[1],"rb");
    if(f1 == NULL){printf("No Found %s\n",argv[1]);}
    
    strcpy(filename, argv[1]);

    //remove .bar
    *(filename+strlen(filename)-1) = '\0';
    *(filename+strlen(filename)-1) = '\0';
    *(filename+strlen(filename)-1) = '\0';
    *(filename+strlen(filename)-1) = '\0';
    
    //printf("filename %s\n", filename);
    sprintf(dirname, "%s_barunpacked", filename);
    mkdir(dirname);
    
    
    fseek(f1,0x10,SEEK_SET);
    fread (&tableoffset,sizeof(int),1,f1);
    fseek(f1,0x1C,SEEK_SET);
    fread (&datasize,sizeof(int),1,f1);
    fseek(f1,0,SEEK_END);
    filesize = ftell(f1);
    fseek(f1,tableoffset,SEEK_SET);
    
    tablecout = ((filesize-datasize)-tableoffset)/sizeof(int);
    
    table_data = (int *)malloc(sizeof(int) * tablecout+1);
    for(i = 0; i < tablecout; i++)
    {
          fread (&table_data[i],sizeof(int),1,f1);
    }
    table_data[i] = table_data[i-1];
    
    //f2 = fopen (argv[1],"rb");
    for(i = 0; i < tablecout; i++)
    {
         memset(filename, 0, 256);
         zipsize = (table_data[i+1] - table_data[i]);
         //printf("zipsize %d\n",zipsize);
         if(zipsize <= 32)
           continue;
         
         //copy name
         fseek(f1,table_data[i]+42,SEEK_SET);
         for(j = 0; j < zipsize-32; j++)
         {
               fread (&data_,sizeof(byte),1,f1);
               if(data_ == 0) break;
               filename[j] = data_;
         }
         
         sprintf(zipname, "%s/%s.zip", dirname, filename);
         printf("Extract %s\n",zipname);
         f2 = fopen (zipname,"wb");
         
          //copy data
         fseek(f1,table_data[i]+32,SEEK_SET);
         for(j = 0; j < zipsize-32; j++)
         {
               fread (&data_,sizeof(byte),1,f1);
               fputc(data_,f2);
         }
         fclose(f2);
    }
    
    
    fclose(f1);
    free(table_data);
    system("PAUSE");
    return EXIT_SUCCESS;
}
