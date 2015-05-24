#ifndef IMGIO_H
#define IMGIO_H

#include "header_list.h"
#define HEADER_SIZE		54
#define WIDTH_OFFSET	18 
#define HEIGHT_OFFSET	22
#define BITCOUNT_OFFSET 28	


typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGB;

typedef struct
{
	int width;
	int height;
	int pad;
	char *file_name;
} BMP_FILE_HEADER;

/*
void select_menu(void)
{
	printf("select filter\n");
	printf("1.identity	2.blur	3.quick\n");
	scanf("%d", &mode);
	fflush(stdin);
	switch(mode)
	{
		case 1 : img = filter_image(img, header, identity); break;
		case 2 : img = filter_image(img, header, blur);	break;
		case 3 : img = filter_image(img, header, quick); break;
	}
}
*/

void print_continue_msg(void)
{
	printf("Press Any Key to Continue ... ");
	getchar();
}

void clean_stdin(void)
{
	int c;
	do
	{
		c = getchar();
	}while(c != '\n' && c != EOF);
}

void print_menu(void)
{
	system("clear");
	printf("********************************************************************************\n");
	printf("********************************************************************************\n");
	printf(" /$$$$$$ /$$      /$$  /$$$$$$        /$$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$	\n");
	printf("|_  $$_/| $$$    /$$$ /$$__  $$      | $$__  $$| $$__  $$ /$$__  $$ /$$__  $$	\n");
	printf("  | $$  | $$$$  /$$$$| $$  \\__/      | $$  \\ $$| $$  \\ $$| $$  \\ $$| $$  \\__/	\n");
	printf("  | $$  | $$ $$/$$ $$| $$ /$$$$      | $$$$$$$/| $$$$$$$/| $$  | $$| $$			\n");
	printf("  | $$  | $$  $$$| $$| $$|_  $$      | $$____/ | $$__  $$| $$  | $$| $$			\n");
	printf("  | $$  | $$\\  $ | $$| $$  \\ $$      | $$      | $$  \\ $$| $$  | $$| $$    $$	\n");
	printf(" /$$$$$$| $$ \\/  | $$|  $$$$$$/      | $$      | $$  | $$|  $$$$$$/|  $$$$$$/	\n");
	printf("|______/|__/     |__/ \\______/       |__/      |__/  |__/ \\______/  \\______/	\n");
	printf("********************************************************************************\n");
	printf("********************************************************************************\n");       
}                                                                     
                                                                        
char select_menu(void)
{
	char mode;

	printf("\nSELECT MENU\n");
	printf("[1]Mount Image\n");
	printf("[2]Export Image\n");
	printf("[3]Apply Filters\n");

	printf("\n");
	printf("[E]xit\n");

	while(1)
	{	
		printf("\n[1, 2, 3, E] ");
		scanf("%c", &mode);
		clean_stdin();
		if(mode!='1' && mode!='2' && mode!='3' && mode!='E' && mode!='e')
		{
			continue;
		}
		return mode;
	}
}


/**************************************************
 *
 * is_a_bmp(...
 *
 * This function looks at a file to see if it
 * is a bmp file. First look ar the file 
 * extension. Next look at the first two 
 * characters of the header to check if it reads
 * "BM"
 *
 * ************************************************/
bool is_a_bmp(unsigned char *raw_header)
{	
	if(raw_header[0] != 'B' || raw_header[1] != 'M')
	{
		printf("ERROR : FILE MUST BE OF TYPE BMP\n");
		return false;
	}

	if(*(short*)&raw_header[BITCOUNT_OFFSET] != 24)
	{
		printf("ERROR : BITCOUNT MUST BE 24\n");
		return false;
	}
	return true;
}

/**************************************************
 *
 * read_file_name(...
 *
 * This function reads in th at a file to see if it
 * is a bmp file. First look ar the file 
 * extension. Next look at the first two 
 * characters of the header to check if it reads
 * "BM"
 *
 * ************************************************/
char* read_file_name(void)
{
	int cin, idx = 0;
	char *file_extension;
	char *file_name;

	file_name = (char*)malloc(sizeof(char));
	do
	{
		cin = getchar();
		file_name = (char*)realloc(file_name, sizeof(char)*(idx+1));
		if(cin == '\n')
			cin = '\0';
		file_name[idx++] = cin;
	}while(cin != '\0');
	
	file_extension = strstr(file_name, ".bmp");
	if(file_extension == NULL)
		return NULL;

	return file_name;
}

/**************************************************
 *
 * calculate_pad(...
 *
 * This function calculates the pad needed at the 
 * end of each row of pixels in a bmp image
 *
 * ************************************************/
int calculate_pad(int width)
{
	int pad; 

	for(pad=0;(width*3+pad)%4;++pad);
	return pad;
}

/**************************************************
 *
 * construct_header(...
 *
 * This function extracts basic information of the
 * given BMP image from the raw header string
 *
 * ************************************************/
BMP_FILE_HEADER* construct_header(BMP_FILE_HEADER *header, unsigned char *raw_header) 
{
	header->width = *(int*)&raw_header[WIDTH_OFFSET];
	header->height = *(int*)&raw_header[HEIGHT_OFFSET];
	header->pad = calculate_pad(header->width);

	return header;
}

/**************************************************
 *
 * read_raw_header(...
 *
 * This function calculates the pad needed at the 
 * end of each row of pixels in a bmp image
 *
 * ************************************************/
void read_raw_header(FILE *fp, unsigned char *raw_header)
{
	fread(raw_header, sizeof(unsigned char), HEADER_SIZE, fp);
	//return raw_header;
}

/**************************************************
 *
 * read_bmp_image(...
 *
 * This function calculates the pad needed at the 
 * end of each row of pixels in a bmp image
 *
 * ************************************************/
RGB** read_bmp_image(BMP_FILE_HEADER *header)
{
	int i, j;
	RGB **img;
	FILE *fp; 
	unsigned char raw_header[HEADER_SIZE];
	unsigned char *raw_data;

	print_menu();
	printf("\n[1] Mount Image\n\n");
	printf("Input File Name : ");
	header->file_name = read_file_name();
	if(header->file_name == NULL)
	{
		printf("ERROR : FILE MUST BE OF EXTENSION .BMP\n");
		return NULL;
	}	

	fp = fopen(header->file_name, "rb");
	if(fp == NULL)
	{
		printf("ERROR : FILE DOES NOT EXIST\n");
		return NULL;
	}

	read_raw_header(fp, raw_header);
	if(!is_a_bmp(raw_header))
		return NULL;
	
	construct_header(header, raw_header);
		
	raw_data = (unsigned char*)malloc(sizeof(unsigned char)*((header->width*3)+header->pad));
	if(raw_data == NULL)
	{
		printf("ERROR : COULD NOT ALLOCATE MEMORY FOR RAW_DATA\n");
		return NULL;
	}
	
	img = (RGB**)malloc(sizeof(RGB*)*header->width);
	if(img == NULL)
	{
		printf("ERROR : COULD NOT ALLOCATE MEMORY FOR IMG\n");
		return NULL;
	}	
	for(i=0;i<header->width;++i)
		img[i] = (RGB*)malloc(sizeof(RGB)*header->height);
	/* ======== end of initialization ======== */

//	printf("READING IMAGE . . .\n");
	for(i=0;i<header->height;++i)
	{
		fread(raw_data,sizeof(unsigned char),header->width*3+header->pad,fp);
		for(j=0;j<header->width*3;j+=3)
		{
			img[j/3][header->height-i-1].b = raw_data[j];
			img[j/3][header->height-i-1].g = raw_data[j+1];
			img[j/3][header->height-i-1].r = raw_data[j+2];
		}
	//	if(i % (header->height/25) == 0)
	//		printf("#");
	}
	fclose(fp);

	printf("Image Has Been Successfully Mounted\n");

	return img;
}

/**************************************************
 *
 * write_bmp_image(...
 *
 * This function calculates the pad needed at the 
 * end of each row of pixels in a bmp image
 *
 * ************************************************/
bool write_bmp_image(RGB **img, BMP_FILE_HEADER *header)
{
	int i, j;
	char *output_name;
	unsigned char raw_header[HEADER_SIZE];
	unsigned char *raw_data;
	FILE *fp;

	if(img == NULL)
	{
		printf("Error : Mount Image First\n");
		return false;
	}

	print_menu();
	printf("\n[2] Export Image\n");
	printf("\nOutput File Name : ");
	output_name = read_file_name();
	if(output_name == NULL)
	{
		printf("ERROR : FILE MUST BE OF EXTENSION .BMP\n");
		return false;
	}	
	raw_data = (unsigned char*)malloc(sizeof(unsigned char)*((header->width*3)+header->pad));
	
	fp = fopen(header->file_name, "rb");
	fread(raw_header, sizeof(unsigned char), HEADER_SIZE, fp);
	fclose(fp);

	fp = fopen(output_name, "wb");
	fwrite(raw_header, sizeof(unsigned char), HEADER_SIZE, fp);
	free(output_name);

	for(i=0;i<header->height;++i)
	{
		memset(raw_data, 0, ((header->width*3)+header->pad)*sizeof(unsigned char));
		for(j=0;j<header->width*3;j+=3)
		{
			raw_data[j]   = img[j/3][header->height-i-1].b;		
			raw_data[j+1] = img[j/3][header->height-i-1].g;
			raw_data[j+2] = img[j/3][header->height-i-1].r;
		}
		fwrite(raw_data, sizeof(unsigned char), header->width*3+header->pad, fp);
	}

	free(raw_data);
	fclose(fp);

	printf("Image Has Been Successfully Exported\n");
	return true;
}

RGB** free_image(RGB** img, BMP_FILE_HEADER header)
{
	int i;

	if(!img)
	{
		for(i=0;i<header.width;++i)
			free(img[i]);
		free(img);
	}
	return NULL;
}




#endif /* IMGIO_H */
