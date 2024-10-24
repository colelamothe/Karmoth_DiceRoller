/*
 * matrixLED.c
 *
 *  Created on: Aug 17, 2024
 *      Author: Mothm
 */

#include <3x5Font.h>
#include "main.h"
const int rowCount = 5;
const int columnCount = 6;
char inputArray[2];

int displayArray[5][6];
uint8_t currentRow = 0;

uint16_t rowGPIO[] = {LDR1_Pin,LDR2_Pin,LDR3_Pin,LDR4_Pin,LDR5_Pin};
uint16_t columnGPIO[] = {LDC1_Pin,LDC2_Pin,LDC3_Pin,LDC4_Pin,LDC5_Pin,LDC6_Pin};

void updateNumber(uint8_t inputNumber){
	if(inputNumber < 10){
		inputArray[0] = 0 + '0';
		inputArray[1] = inputNumber + '0';
	}
	else{
		inputArray[0] = inputNumber/10 + '0';
		inputArray[1] = inputNumber%10 + '0';
	}

	for(int i = 0; i < 2; i++){
		switch(inputArray[i]){
			case 48: // ASCII 48
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = zero[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = zero[j][k-3];
						}
					}
				}
				break;
			case 49: // ONE
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = one[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = one[j][k-3];
						}
					}
				}
				break;
			case 50:
				// Two
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = two[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = two[j][k-3];
						}
					}
				}
				break;
			case 51:
				// Three
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = three[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = three[j][k-3];
						}
					}
				}
				break;
			case 52:
				// Four
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = four[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = four[j][k-3];
						}
					}
				}
				break;
			case 53:
				// Five
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = five[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = five[j][k-3];
						}
					}
				}
				break;
			case 54:
				// Six
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = six[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = six[j][k-3];
						}
					}
				}
				break;
			case 55:
				// Seven
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = seven[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = seven[j][k-3];
						}
					}
				}
				break;
			case 56:
				// Eight
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = eight[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = eight[j][k-3];
						}
					}
				}
				break;
			case 57:
				// Nine
				if(i == 0){
					for(int j = 0; j < rowCount; j++){
						for(int k = 0; k < (columnCount / 2); k++){
							displayArray[j][k] = nine[j][k];
						}
					}
				}
				else{
					for(int j = 0; j < rowCount; j++){
						for(int k = 3; k < (columnCount); k++){
							displayArray[j][k] = nine[j][k-3];
						}
					}
				}
				break;
		}
	}

}


void refreshDisplay(){
	/*
	displayArray[5][6];
	rowGPIO[]
	columnGPIO[] */
	if(currentRow == 0){
		HAL_GPIO_WritePin(GPIOA,rowGPIO[rowCount-1],SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOA,rowGPIO[currentRow-1],SET);
	}
	for(int i = 0; i < columnCount; i++){
		HAL_GPIO_WritePin(GPIOA,columnGPIO[i],RESET);
	}
	for(int i = 0; i < columnCount; i++){
		if(displayArray[currentRow][i]){
			HAL_GPIO_WritePin(GPIOA,rowGPIO[currentRow],RESET);
			HAL_GPIO_WritePin(GPIOA,columnGPIO[i],SET);
		}
	}
	if(currentRow >= rowCount-1){
		currentRow = 0;
	}
	else{
		currentRow++;
	}
}

void scrollDisplay(){
	uint8_t ledScrollSpeed = 10;
	for(int i = 0; i < 5; i++){
		  for(int j = 0; j<6;j++){
			  HAL_GPIO_WritePin(GPIOA,rowGPIO[i],SET);
			  HAL_GPIO_WritePin(GPIOA,columnGPIO[j],RESET);
		  }
	  }
	  for(int i = 0; i < 5; i++){
		  HAL_GPIO_WritePin(GPIOA,rowGPIO[i],RESET);
		  if(i%2>0){
			  for(int j = 5; j>=0;j--){
				  HAL_Delay(ledScrollSpeed);
				  HAL_GPIO_WritePin(GPIOA,columnGPIO[j],SET);
				  HAL_Delay(ledScrollSpeed);
				  HAL_GPIO_WritePin(GPIOA,columnGPIO[j],RESET);
			  }
		  }
		  else{
			  for(int j = 0; j<6;j++){
				  HAL_Delay(ledScrollSpeed);
				  HAL_GPIO_WritePin(GPIOA,columnGPIO[j],SET);
				  HAL_Delay(ledScrollSpeed);
				  HAL_GPIO_WritePin(GPIOA,columnGPIO[j],RESET);
			  }
		  }

		  HAL_GPIO_WritePin(GPIOA,rowGPIO[i],SET);
		  HAL_Delay(ledScrollSpeed);
	  }
	  HAL_Delay(ledScrollSpeed);
}


