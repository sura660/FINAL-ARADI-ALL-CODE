#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "toyaradi_function.h"


// Main Function
int main() {
	srand(time(NULL));
	uint8_t PDelta[4] = {0x00, 0x00, 0x00, 0x00};
	uint8_t Delta[4] = {0x10, 0x00, 0x00, 0x00};
	uint8_t Delta1[4] = {0xef, 0xef, 0xef, 0xef};
	uint8_t diff_8[6] = {0x5,0x7,0x8,0xa,0xd,0xf};

        uint8_t key[8] = {0x03, 0x07, 0x0b, 0x0f, 0x13, 0x17, 0x1b, 0x1f};

	uint8_t key_d[4] = {0x12,0x98,0x17,0x19};

	uint32_t count=0,cntt,temp,tot_cnt=0 ;
        uint8_t p1[4], deltap[4] ;
	uint8_t p2[4] ;
        uint8_t c1[4],c2[4],c11[4],c22[4],c111[4],c222[4], deciphered[4], delta_c[4];
	uint8_t out[4] = {0x00, 0x00, 0x00, 0x00};

	int col3[3] = {2,3,7};
	int col1 = 3;
	int col7[7] = {0,1,2,3,4,6,7};



	for(int abc = 0; abc < 200; abc++){
	count = 0;
	for( int loop = 0; loop < (1<<12); loop++){
		for(int i=0; i < 4;i++){	
			c1[i] = rand() & 0xff;
			c2[i] = c1[i];
			for(int j = 0; j < 3; j++)
				c2[i] = c2[i] ^ ((rand()&1) << (7-col3[j]) );

			c11[i] = c1[i];
			c22[i] = c2[i];

			}
		if(loop == 0){
			for(int i=0; i < 4;i++){	
				c1[i] = rand() & 0xff;
				c2[i] = c1[i] ^ Delta[i];
				c111[i] = c1[i];
				c222[i] = c2[i];
				}

			encryption_ARADI(c1, key, c11);
			encryption_ARADI(c2, key, c22);
			}



		for(uint64_t gkey = 0 ; gkey < ((uint64_t)(1)<<12) ; gkey++){

			for(int i=0; i < 4;i++) {
				c1[i] = c11[i];
				c2[i] = c22[i];
				}



			c1[0] = linear(2, c1[0]);
		        c1[1] = linear(2, c1[1]);
		        c1[2] = linear(2, c1[2]);
		        c1[3] = linear(2, c1[3]);

			c2[0] = linear(2, c2[0]);
		        c2[1] = linear(2, c2[1]);
		        c2[2] = linear(2, c2[2]);
		        c2[3] = linear(2, c2[3]);



			for(int i=0; i < 4;i++) {
				for(int j = 0; j < 3; j++){
					c1[i] = c1[i] ^ (((gkey >> 3*i+j)&1) << (7-col3[j])); 
					c2[i] = c2[i] ^ (((gkey >> 3*i+j)&1) << (7-col3[j]));

					}
				
				}



			sbox_inverse(&c1[0], &c1[1], &c1[2], &c1[3]);
			sbox_inverse(&c2[0], &c2[1], &c2[2], &c2[3]);



			c1[0] = linear(1, c1[0]);
		        c1[1] = linear(1, c1[1]);
		        c1[2] = linear(1, c1[2]);
		        c1[3] = linear(1, c1[3]);

			c2[0] = linear(1, c2[0]);
		        c2[1] = linear(1, c2[1]);
		        c2[2] = linear(1, c2[2]);
		        c2[3] = linear(1, c2[3]);

			for(int i=0; i < 4;i++) delta_c[i] = c1[i]^c2[i];



			int cnt = 0;
			for(int i=0; i < 4;i++){ 
				delta_c[i] = delta_c[i] & Delta1[i];
				if(delta_c[i] == 0)
					cnt++;
				}
			if(cnt == 4){
				for(int i=0; i < 4;i++) delta_c[i] = c1[i]^c2[i];
				temp = (((delta_c[0] >> 4)&1)<<3) ^ (((delta_c[1] >> 4)&1)<<2) ^ (((delta_c[2] >> 4)&1)<<1) ^ ((delta_c[3] >> 4)&1) ;
				cntt = 0;
				for(int i = 0; i < 6; i++){ 
					if(temp == diff_8[i]){
						cntt++;
						break;
						} 
					}
				if(cntt == 1){
					count++;
					if(loop> 0 )
						printf("WWWWRRRong Pair");
					}
				}

			}
		}

	printf("\n\t\tTotal count = %d ",count);

	tot_cnt = tot_cnt + count;




	}    

	printf("\n\tAverage  count = %f ",(float)tot_cnt/200);


    return 0;
}






