/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <system.h>
#include <altera_avalon_pio_regs.h>
#include <alt_types.h>
#include <altera_up_avalon_accelerometer_spi.h>
#include <unistd.h>

#define NUM_DISPLAYS 6
const alt_u8 CERC_SUS=0b10011100;
const alt_u8 CERC_JOS=0b10100011;

const alt_u32 HEX_BASES[NUM_DISPLAYS] =
{
		HEX_0_BASE,
		HEX_1_BASE,
		HEX_2_BASE,
		HEX_3_BASE,
		HEX_4_BASE,
		HEX_5_BASE
};

alt_u8 cerc(alt_32 x, alt_32 y) {
    if (y > 20) {
        return CERC_SUS;
    } else if (y < -20) {
        return CERC_JOS;
    } else {
        return 0xFF;
    }
}

void shift_pattern(alt_u8 cerc, alt_32 inclination)
{	static int current_position = 0;
	if( inclination >10){
		current_position++;
	}else if (inclination <-15)
		current_position--;


	if(current_position >= NUM_DISPLAYS) current_position =0;
	if(current_position <0) current_position =NUM_DISPLAYS -1;

	for (int i=0; i<NUM_DISPLAYS;i++)
	{
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_BASES[i], 0xFF);
	}
	 IOWR_ALTERA_AVALON_PIO_DATA(HEX_BASES[current_position], cerc);
}
int main()
{



	alt_up_accelerometer_spi_dev* acc_dev;
	alt_32 x=0, y=0 ,z=0;
	int sw_status;
	sw_status = IORD_ALTERA_AVALON_PIO_DATA(SV_INPUT_BASE);



	acc_dev= alt_up_accelerometer_spi_open_dev(ACCELEROMETER_SPI_0_NAME);
	if(acc_dev==NULL){
	printf("ERROR:Failed to open the accelerometer device.\n");}
	else{
		while(1){
			//usleep(2000);
			alt_up_accelerometer_spi_read_x_axis(acc_dev ,&x);
			alt_up_accelerometer_spi_read_y_axis(acc_dev ,&y);
			alt_up_accelerometer_spi_read_z_axis(acc_dev ,&z);
			printf("x=%d, y=%d, z=%d\n", x, y, z);
			if(sw_status>0)                  //easteregg
					printf("it works! sw=%d\n",sw_status);
			else if(sw_status==0)
			printf("error sw=%d\n",sw_status);



			alt_u8 pattern = (y > 0) ? CERC_SUS : CERC_JOS;
			shift_pattern(pattern, x);
			 usleep(300000);
		}
	}



  return 0;
}
