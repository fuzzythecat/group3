#ifndef FILTER_H
#define FILTER_H

#include "imgio.h"

enum filter_type {identity, blur, quick};

short quick_mask[3][3] =
{
	{-1, 0, -1},
	{ 0, 4, 0},
	{-1, 0, -1} 
};

short identity_mask[3][3] =
{
	{0, 0, 0},
	{0, 9, 0},
	{0, 0, 0} 
};

short blur_mask[3][3] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1} 
};



/***************************
*
* Directions for the masks
* 3 2 1
* 4 x 0
* 5 6 7
*
****************************/









/*******************************************
*
* setup_filter(...
*
* This function filters an image by using
* a single 3x3 mask.
*
*******************************************/

short** setup_filter(enum filter_type type)
{
	short **filter = (short**)malloc(sizeof(short*)*3);
	int i, j;

	for(i=0;i<3;++i)
		filter[i] = (short*)malloc(sizeof(short)*3);

	switch(type)
	{
		case identity :	for(i=0;i<3;++i){for(j=0;j<3;++j) filter[i][j] = identity_mask[i][j];} break;
		case blur	  :	for(i=0;i<3;++i){for(j=0;j<3;++j) filter[i][j] = blur_mask[i][j];}	  break;
		case quick	  : for(i=0;i<3;++i){for(j=0;j<3;++j) filter[i][j] = quick_mask[i][j];}	  break;
	}

	return filter;
}

/*******************************************
*
* filter_image(...
*
* This function filters an image by using
* a single 3x3 mask.
*
*******************************************/
RGB** filter_image(RGB** img, BMP_FILE_HEADER header, enum filter_type type)
{
	int i, j, r, g, b;
	short **filter;

	filter = setup_filter(type);

	for(i=0;i<header.height;++i)
		for(j=0;j<header.width;++j)
		{
			r = 0;
			g = 0;
			b = 0;
			for(int m=-1;m<2;++m)
				for(int n=-1;n<2;++n)
					if((i+m)>=0&&(i+m)<header.height&&(j+n)>=0&&(j+n)<header.width)
					{
						r+=img[j+n][i+m].r * filter[n+1][m+1];
						g+=img[j+n][i+m].g * filter[n+1][m+1];
						b+=img[j+n][i+m].b * filter[n+1][m+1];
					}
			img[j][i].r = r/9;
			img[j][i].g = g/9;
			img[j][i].b = b/9;
		}

		printf("Filter successfully applied\n");
		//print_continue_msg();

	return img;
}

RGB** reverse_filter(RGB** img, BMP_FILE_HEADER header)
{
	int i, j;
	for(i=0;i<header.height;++i)
		for(j=0;j<header.width;++j)
		{
			img[j][i].r = 255 - img[j][i].r;
			img[j][i].g = 255 - img[j][i].g;
			img[j][i].b = 255 - img[j][i].b;
		}
	printf("Filter Successfully Applied\n");
//	print_continue_msg();
	return img;
}

RGB** select_filter(RGB** img, BMP_FILE_HEADER header)
{
	char mode;
	
	print_menu();

	if(img == NULL)
	{
		printf("Error : Mount Image First\n");
		return NULL;
	}

	printf("\nSELECT FILTER\n");
	printf("[1] Identity Filter\n");
	printf("[2] Blur FIlter\n");
	printf("[3] Median Filter\n");
	printf("[4] Reverse Filter\n");
	printf("\n");
	printf("[R]eturn to Menu\n");

	printf("[1, 2, 3, 4, R] ");
	scanf("%c", &mode);
	clean_stdin();	

	switch(mode)
	{
		case '1': img = filter_image(img, header, identity); break;
		case '2': img = filter_image(img, header, blur); break;
		//case '3': img = median_filter(img, header) break;
		case '4': img = reverse_filter(img, header); break;
		case 'R':break;
	}

	return img;
}

#endif /* FILTER_H */
