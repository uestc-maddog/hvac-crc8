#include <stdio.h>
#include <stdlib.h>
/*
CRC-8

Yiran Tian
Credit goes to https://chromium.googlesource.com/chromiumos/platform/vboot_reference/+/master/firmware/lib/crc8.c

Two implements

Implement A:
After receiving all the data, call function
Crc8(const void *vptr, int len);

Example:
crcResult = Crc8(testbuf,sizeof(testbuf));


Implement B:
Initialize before start CRC-8 calculation by call Crc8Start(),
then feed in byte by byte. The final result is CRC8RESULT.

Example:
(before start)
Crc8Start();

(when receiving one byte)
Crc8Creat(&pIn);

(When receiving finish)
crcResult = CRC8RESULT;
*/

typedef unsigned char uint8_t;

// Test Data
uint8_t testbuf[100] = {0x4c ,0x75 ,0x63 ,0x75 ,0x73 ,0x49 ,0x61 ,0x6d ,0x79 ,
0x6f ,0x75 ,0x72 ,0x66 ,0x61 ,0xD4 ,0x68 ,0x65 ,0x72 ,0x74 ,0x68 ,0x69 ,0x73 ,
0x69 ,0x73 ,0x74 ,0x68 ,0x65 ,0x62 ,0x69 ,0x67 ,0x65 ,0x73 ,0x74 ,0x61 ,0x6e ,
0x64 ,0x73 ,0x74 ,0x72 ,0x6f ,0x6e ,0x71 ,0x64 ,0x6b ,0x61 ,0x66 ,0x6f ,0x6e ,
0x4f ,0x4e ,0x4f ,0x69 ,0x6a ,0x20 ,0x70 ,0x39 ,0x30};


/*
 *  Group for Implement A
 */
uint8_t Crc8(const void *vptr, int len)
{
	const uint8_t *data = vptr;
	unsigned crc = 0;
	int i, j;
	for (j = len; j; j--, data++) {
		crc ^= (*data << 8);
		for(i = 8; i; i--) {
			if (crc & 0x8000)
				crc ^= (0x1070 << 3);
			crc <<= 1;
		}
	}
	return (uint8_t)(crc >> 8);
}


/*
 *  Group for Implement B
 */

// Macro for Implement B
#define CRC8RESULT  (crc8Result >> 8)

// Global variable for Implement B
unsigned crc8Result;

/*********************************************************************
 * @fn      Crc8Start
 *
 * @brief   Initialize global variable
 *
 * @param   none
 *
 * @return  none
 */
uint8_t Crc8Start( void )
{
    crc8Result = 0;
}

/*********************************************************************
 * @fn      Crc8Creat
 *
 * @brief   Calculate data when receiving one byte data
 *
 * @param   uint8_t *datainput - input data pointer
 *
 * @return  uint8_t - current crc8 result
 */
uint8_t Crc8Creat(uint8_t *datainput)
{
    uint8_t i;

    crc8Result ^= (*datainput << 8);

    for(i = 8; i; i--)
    {
        if(crc8Result & 0x8000)
            crc8Result ^= (0x1070 << 3);
        crc8Result <<= 1;
    }
    return (uint8_t)(crc8Result >> 8);
}



int main( void )
{
    uint8_t crcResult;
    int i;

    // Implement A
    crcResult = Crc8(testbuf,sizeof(testbuf));

    printf("Implement A\n");
    printf("The CRC result is %d\n",crcResult);
    printf("The size of CRC buffer is %d\n",sizeof(testbuf));


    // Implement B
    // Start with Crc8Start() function
    Crc8Start();

    for(i = 0;i<sizeof(testbuf) ;i++)
    {
        // feed byte while receiving
        Crc8Creat(&testbuf[i]);
    }
    // obtain result while finish
    crcResult = CRC8RESULT;

    printf("Implement B\n");
    printf("The CRC result is %d\n",CRC8RESULT);
    printf("The size of CRC buffer is %d\n",sizeof(testbuf));

    while(1);
}
