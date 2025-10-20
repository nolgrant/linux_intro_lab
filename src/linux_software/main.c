#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h> 
#include <fcntl.h> 
#include <unistd.h>
#define _BSD_SOURCE

#define GPIO_DIP_OFFSET 2
#define GPIO_LED_OFFSET 0 
#define DIPS_AND_LEDS_BASEADDR 0x41200000

#define GPIO_LED0_OFFSET 0
#define GPIO_LED1_OFFSET 2
#define RGB_LEDS_BASEADDR 0x41210000

// the below code uses a device called /dev/mem to get a pointer to a physical
// address.  We will use this pointer to read/write the custom peripheral
volatile unsigned int * get_a_pointer(unsigned int phys_addr)
{

	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC); 
	void *map_base = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr); 
	volatile unsigned int *radio_base = (volatile unsigned int *)map_base; 
	return (radio_base);
}

int main()
{

// first, get a pointer to the peripheral base address using /dev/mem and the function mmap

    volatile unsigned int *dipsandleds_ptr = get_a_pointer(DIPS_AND_LEDS_BASEADDR);	
	volatile unsigned int *rgbleds_ptr = get_a_pointer(RGB_LEDS_BASEADDR);	
	uint8_t counter = 0;
	uint8_t sw_bot3;
	uint8_t rgb_val; //red = LSB, blue - MSB

    printf("\r\n\r\n\r\nLab 9 - Nolan Andreassen\n\r");
    while (1)
    {
	
	    // *(dipsandleds_ptr+GPIO_LED_OFFSET) = 0xf;
	    // usleep(500000);
	    // *(dipsandleds_ptr+GPIO_LED_OFFSET) = 0x0;	
		// usleep(500000);
	    // // for demonstration purposes, print the value of the DIPS
	    // printf("Current Switches = %d\r\n",*(dipsandleds_ptr+GPIO_DIP_OFFSET));

		// count via 4 LEDs
		if (counter < 15) {
			counter += 1;
		} else {
			counter = 0;
		}
		// printf("Counter = %d\r\n",counter);
		
		// write to LEDs
		*(dipsandleds_ptr+GPIO_LED_OFFSET) = counter;
		
		// extract bottom 3 switches
		sw_bot3 = *(dipsandleds_ptr+GPIO_DIP_OFFSET) & 0x7; 

		// set bits according to assigment
		// if (sw_bot3 == 0b001) {
		// 	rgb_val = 0b001; //red
		// } else if (sw_bot3 == 0b010) {
		// 	rgb_val = 0b010; //green
		// } else if (sw_bot3 == 0b100) {
		// 	rgb_val = 0b100; //blue
		// } else if (sw_bot3 == 0b011) {
		// 	rgb_val = 0b011; //red+green
		// } else if (sw_bot3 == 0b101) {
		// 	rgb_val = 0b101; //red+blue
		// } else if (sw_bot3 == 0b110) {
		// 	rgb_val = 0b110; //blue+green
		// } else if (sw_bot3 == 0b111) {
		// 	rgb_val = 0b111; //red+green+blue
		// } else {
		// 	rgb_val = 0; //OFF
		// }
		rgb_val = sw_bot3;

		// write to FPGA
		*(rgbleds_ptr+GPIO_LED0_OFFSET) = rgb_val;
		*(rgbleds_ptr+GPIO_LED1_OFFSET) = rgb_val;	

		usleep(500000);
    }
    return 0;
}
