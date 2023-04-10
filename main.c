#include <stdio.h>
#include <string.h>
#include <stdlib.h>


    FILE *fp, *fp2 , *fp3;
    int i=0, x, tempa[30], *val;
    char *ptr;
    char *buffer;
    char  stbuffer[FILENAME_MAX];
    unsigned int lSize, numbers=0;
    char MagicWord[5];
    char filename[FILENAME_MAX];


struct HTEXHeader{
   unsigned int  file_size;
   unsigned int  header_size;
   unsigned int  flag;     //???
    };

struct HTSFHeader{

   unsigned int  HTSF_size;
   unsigned int  HTSF_header_size;
   unsigned int  flag;     //???
   unsigned int  flag2;
   unsigned int  HTFS_size_again;
    };

/*struct DDSHeader {
   unsigned int  DDSHeader_size;
   unsigned int  DwHeight;
   unsigned int  DwWidth;
   unsigned int  DDS_size;
    };*/

char checkfile (char checkheader[])
    {
    fgets(MagicWord, 5, fp);
    puts (MagicWord);
    return 1;
    }


char readfilename (char readfilename[])
    {
    if ((filename[strlen(filename) - 1] == '\n') && (filename[strlen(filename) - 2] == '\"'))
        {
                ptr = strchr (filename, '\\');   //initiate pointer
                //printf ("%d \n" , *ptr);
                //printf ("%d \n" , *filename);
                while (ptr != NULL)
                        {
                        //printf ("%d ", ptr-filename+1);
                        ptr = strchr (ptr+1,'\\');
                        x++;
                        }
                        printf ("\n x = %d \n", x);

                ptr = strchr (filename, '\\');      //reset pointer to the start of string
                    for(int i=1;i<=x;i++)
                        {
                        tempa[i] = ptr-filename+1;
                        ptr = strchr (ptr+1,'\\');
                        //printf ("location of \\ is %d \n", tempa[i]);
                        }

                    for(int i=x;i>0;i--)
                        {
                        //printf ("i= %d location of \\ is %d \n", i, tempa[i]);
                        //printf("check stbuffer name %s\n\n",stbuffer);
                        strncpy(stbuffer, filename, tempa[i]);
                        //printf("%s \n",stbuffer);
                        //printf("filename+tempa[%d] %s\n", i, tempa[i]+filename);
                        stbuffer[tempa[i]] = '\0';
                        strcat(stbuffer,"\\");
                        //puts(stbuffer);
                        strcat(stbuffer,filename+tempa[i]);
                        //printf("check stbuffer name 1 %s\n\n",stbuffer);
                            //for(int i=0; i<strlen(filename)+1; i++)
                        strcpy(filename, stbuffer);
                        //printf("check file name %s\n\n", filename);
                        memset(stbuffer, 0, sizeof(stbuffer));
                        //printf("check stbuffer name 2 %s\n\n",stbuffer);
                        }
                        memmove(filename, filename+1, strlen(filename));
                        filename[strlen (filename) - 2] = '\0';
                        //puts(filename);
                        //printf("filename long : %d characters\n", strlen(filename));

        }
    else if ((strlen(filename) > 0) && (filename[strlen (filename) - 1] == '\n'))
                        filename[strlen (filename) - 1] = '\0';
                        fp=fopen(filename, "rb");  //rb = readbinary
                        checkfile(MagicWord);
    if  (strcmp (MagicWord,"HTEX")!=0)
        {
        printf("File type Incorrect \n");
		fclose(fp);
		return -1;
        }
    else
        {
        printf("File type Correct\n");
        return 0;
    }
}


void extractfile()
 {
        ptr = (char*)malloc(4);
        int *val = (int*)ptr;

        //printf("fp position %X \n", (unsigned int)ftell(fp));
        //rewind(fp);

        fread(ptr, 4, 1, fp);
    struct HTEXHeader Header;

            Header.file_size = *val;
            printf("file size in hex: %X\n", Header.file_size);
            //printf("file size ib dec: %d\n", Header.file_size);

        fread(ptr, 4, 1, fp);
            Header.header_size= *val;
            printf("header size in hex: %X\n", Header.header_size);
            //printf("header size ib dec: %d\n", Header.header_size);


        fseek(fp, Header.header_size, SEEK_SET);
            printf("fp position in hex 1 %X \n", (unsigned int)ftell(fp));

            //printf("save position in hex %X \n", savepos);


    checkfile(MagicWord);

        while (strcmp (MagicWord,"HTSF")==0)
       {
            struct HTSFHeader HTSF;
            memset(MagicWord, 0, sizeof(MagicWord));    //Clear magicword to prevent infinite loop
                //printf("fp position in hex 2 %X \n", (unsigned int)ftell(fp));

            fread(ptr, 4, 1, fp);
                //printf("fp position in hex 3 %X \n", (unsigned int)ftell(fp));
                HTSF.HTSF_size = *val;
                printf("HTSF size in hex: %X\n", HTSF.HTSF_size);
                //printf("HTSF size ib dec: %d\n", HTSF.HTSF_size);

            fread(ptr, 4, 1, fp);
                //printf("fp position in hex 4 %X \n", (unsigned int)ftell(fp));

                HTSF.HTSF_header_size = *val;
                //printf("HTSFHeader size in hex: %X\n", HTSF.HTSF_header_size);
                //printf("HTSFHeader size ib dec: %d\n", HTSF.HTSF_header_size);

            lSize = HTSF.HTSF_header_size + HTSF.HTSF_size;
            printf("lSize in hex %X \n", lSize);
                //printf("fp position in hex %X \n", (unsigned int)ftell(fp));
            fseek(fp, 52, SEEK_CUR);
                //printf("fp position in hex %X \n", (unsigned int)ftell(fp));
                //free(buffer);
            buffer = ptr;
            buffer = (char*)malloc(lSize);
            fread(buffer, 1, lSize-64, fp);
                //printf("lSize-64 in hex %X \n", lSize-64);
                //printf("fp position in hex %X \n", (unsigned int)ftell(fp));
            sprintf(filename, "%s-%d.dds", filename, numbers);
                //strcat(filename, ".dds");
            fp2=fopen(filename, "w+b");
            filename[strlen (filename) - 6] = '\0';
            fwrite(buffer, 1, lSize-64, fp2);
            fclose(fp2);
            //fseek(fp, 0, SEEK_CUR);
            //printf("fp position in hex %X \n", (unsigned int)ftell(fp));
            checkfile(MagicWord);
            numbers++;
            free(buffer);
        }

            printf("numbers of files extracted: %d\n", numbers);
        fclose(fp);
   return;
}



void importfile()
{
    ptr = (char*)malloc(4);
    int *val = (int*)ptr;

        //printf("fp position %X \n", (unsigned int)ftell(fp));
        //rewind(fp);

        fread(ptr, 4, 1, fp);

    struct HTEXHeader Header;

            Header.file_size = *val;
            printf("file size in hex: %X\n", Header.file_size);
            //printf("file size ib dec: %d\n", Header.file_size);

        fread(ptr, 4, 1, fp);
            Header.header_size= *val;
            printf("header size in hex: %X\n", Header.header_size);
            //printf("header size ib dec: %d\n", Header.header_size);
        rewind(fp);
        buffer = ptr;
        buffer = (char*)malloc(Header.header_size);
        fread(buffer, 1, Header.header_size, fp);
        //strcat("new_",filename);
        memset(stbuffer, 0, sizeof(stbuffer));
        strcpy(stbuffer, filename);
        stbuffer[strlen (stbuffer) - 4] = '\0';
        strcat(stbuffer,"_new.htx");
        fp3=fopen(stbuffer, "a+b");

        fwrite(buffer, 1, Header.header_size, fp3);
        free(buffer);

        fseek(fp, Header.header_size, SEEK_SET);
            printf("fp position in hex 1 %X \n", (unsigned int)ftell(fp));



        checkfile(MagicWord);
        //for (ptr=Header.header_size; ptr<Header.file_size; ptr++)

            while (strcmp (MagicWord,"HTSF")==0)
            {
                FILE *fp4;
                unsigned int DDSsize;
                struct HTSFHeader HTSF;
                memset(MagicWord, 0, sizeof(MagicWord));   //Clear magicword to prevent infinite loop


                fread(ptr, 4, 1, fp);
                    //printf("fp position in hex 3 %X \n", (unsigned int)ftell(fp));
                HTSF.HTSF_size = *val;
                    printf("HTSF size in hex: %X\n", HTSF.HTSF_size);
                    //printf("HTSF size ib dec: %d\n", HTSF.HTSF_size);

                fread(ptr, 4, 1, fp);
                    //printf("fp position in hex 4 %X \n", (unsigned int)ftell(fp));

                HTSF.HTSF_header_size = *val;
                    //printf("HTSFHeader size in hex: %X\n", HTSF.HTSF_header_size);
                    //printf("HTSFHeader size ib dec: %d\n", HTSF.HTSF_header_size);

                lSize = HTSF.HTSF_header_size + HTSF.HTSF_size;
                    printf("lSize in hex %X \n", lSize);

                fseek(fp, -12, SEEK_CUR);
                buffer = (char*)malloc(HTSF.HTSF_header_size);
                fread(buffer, 1, HTSF.HTSF_header_size, fp);
                fp4 = tmpfile();
                fwrite(buffer, 1, HTSF.HTSF_header_size, fp4);
                memset(tempa, 0, sizeof(tempa));
                fwrite(tempa, 1, 32, fp4);                   //write 32 bytes zero dummy after HTSF header
                free(buffer);

                //fseek(fp4, -28, SEEK_CUR);



                sprintf(filename, "%s-%d.dds", filename, numbers);
                fp2 = fopen(filename,"rb");
                if (fp2==NULL) perror ("Error opening file");
                filename[strlen (filename) - 6] = '\0';
                fseek(fp2, 0, SEEK_END);
                DDSsize = ftell(fp2);
                printf ("Size of DDS file: %x bytes.\n",DDSsize);
                rewind(fp2);
                buffer = (char*)malloc(DDSsize);
                fread(buffer, 1, DDSsize, fp2);
                fclose(fp2);
                fwrite(buffer, 1, DDSsize, fp4);            //Copy DDS to temp file
                free(buffer);

                fseek(fp4, 4, SEEK_SET);
                DDSsize = DDSsize + 32;                     //Add 32 bytes zero dummy for DDS size
                fwrite(&DDSsize, 4, 1, fp4);                //Write value of DDS file size to the HTSF
                fseek(fp4, 12, SEEK_CUR);
                fwrite(&DDSsize, 4, 1, fp4);
                rewind(fp4);
                buffer = (char*)malloc(DDSsize+32);
                fread(buffer, 1, 32+DDSsize, fp4);          //read all temp file (fp4)
                fclose(fp4);
                fwrite(buffer, 1, 32+DDSsize, fp3);         //Copy all temp file to file new (fp3)
                free(buffer);


                fseek(fp, HTSF.HTSF_size, SEEK_CUR);            //continue on original file (fp)
                    //printf("fp position in hex 3 %X \n", (unsigned int)ftell(fp));
                checkfile(MagicWord);
                numbers++;
            }
        printf ("number of files = %d \n", numbers);
        fseek(fp, -4, SEEK_CUR);
        //printf ("where the fuck is my pointer = %X \n", (unsigned int)ftell(fp));
        i = ftell(fp);
        fseek(fp, 0, SEEK_END);
        x = ftell(fp);
        i = x-i;
        fseek(fp, -i, SEEK_END);
        //printf ("end of file long = %d \n", i);
        buffer = (char*)malloc(i);
        fread(buffer, 1, i, fp);
        fwrite(buffer, 1, i, fp3);
        free(buffer);
        fclose(fp);
        fseek(fp3, -i, SEEK_END);
        Header.file_size = ftell(fp3);
        //printf ("where the fuck is my pointer = %X \n", (unsigned int)ftell(fp3));
        freopen (stbuffer, "rb+", fp3);
        fseek(fp3, 4, SEEK_SET);
        //printf ("where the fuck is my pointer = %X \n", (unsigned int)ftell(fp3));
        fwrite(&Header.file_size, 1, 4, fp3);
        fclose(fp3);
}



int main(int argC, char *argV[])
{
    //printf("check number of arguments is: %d \n", argC);
    switch(argC)
    {
    case 1
    :
        printf("------------Enter 1 for extract, 2 for import------------\n");
        printf("Wot r u gonna du? ");
        scanf("%d%*c", &argV[1]);
        argC++;
    case 2
    :
        if (argV[1] == 1)
        {
        printf("Drag your files here: ");
        fgets(filename, FILENAME_MAX, stdin);
        readfilename(filename);
        extractfile();
        }
        else if (argV[1] == 2)
        {
        printf("Drag your files here: ");
        fgets(filename, FILENAME_MAX, stdin);
        readfilename(filename);
        importfile();
        }
    break;
    case 3
    :
        //for(int i = 0; i < argC; i++)
        //printf("argument[%d] = %s \n", i, argV[i]);
        //printf("check argV[1]: %d\n\n", *argV[1]);
        //printf("check argument is: %s \n ", argV);
        if (*argV[1] == '1')
        {
        strcpy(filename, argV[2]);
        printf("check file name: %s\n\n", filename);
        readfilename(filename);
        extractfile();
        }
        else if (*argV[1] == '2')
        {
        strcpy(filename, argV[2]);
        printf("check file name: %s\n\n", filename);
        readfilename(filename);
        importfile();
        }
    break;
    default
    :
        printf("Invalid argument\n" );
    break;
    }
return 0;
}
