#include "imgio.h"
#include "filter.h"
#pragma warning(disable:4996)

int main(void)
{
	BMP_FILE_HEADER header = {0,0,0};
	RGB **img = NULL;
	char mode;
	int exit_flag = 0;
		
	while(exit_flag == 0)
	{
		print_menu();
		mode = select_menu();
		switch(mode)
		{
			case '1': img = free_image(img, header); img = read_bmp_image(&header); break;
			case '2': write_bmp_image(img, &header); break;
			case '3': img = select_filter(img, header); break;
			case 'E': exit_flag = 1;
		}
		if(exit_flag == 0)
			print_continue_msg();
	}


	return 0;
}
