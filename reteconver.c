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
//汇率转换 C 2018年11月8日22点30分
//input ：currency1 to currency2, 最大9999的货币1的值，使用按键输入
/* #define 硬件地址定义
*/
//阶段一：汇率双向显示
#define func (volatile unsigned char *) 0x00011030 //SW[1:0]
#define type_low (volatile unsigned char *)  0x00011040 //SW[9:2]
#define type_high (volatile unsigned char *) 0x00011050//SW[17:10]
//

#define key1 (volatile unsigned char *) 0x00011000
#define key2 (volatile unsigned char *) 0x00011020
#define key3 (volatile unsigned char *) 0x00011010

#define seg8 (volatile char *) 0x00011060
#define seg7 (volatile char *) 0x00011070
#define seg6 (volatile char *) 0x00011080
#define seg5 (volatile char *) 0x00011090

#define seg4 (volatile char *) 0x000110a0
#define seg3 (volatile char *) 0x000110b0
#define seg2 (volatile char *) 0x000110c0
#define seg1 (volatile char *) 0x000110d0
////案件
//
//
////output


//基准货币 RMB
//所有货币与人民币的换算

/* 1RMB兑换外币个数
0.1442 美元  0
161.06 South Korean won 韩元 1
0.1261 欧元 2
0.11英镑  3
16.365 日元 4
0.193 澳大利亚元 5
2.0159南非兰特 6
1.0 人民币 7

*/

//转化成人民币函数
int decoder(int num8){

	switch(num8){
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	case 4:
		return 3;
		break;
	case 8:
		return 4;
		break;

	case 16:
		return 5;
		break;
	case 32:
		return 6;
		break;
	case 64:
		return 7;
		break;
	case 128:
		return 8;
		break;
	default:
		return 0;
	}

}
double cur_table[] = {0.1442,161.06,0.1261,0.11,16.365,0.193,2.0159,1.0};

/***********************************************/
unsigned char seg_tab[]={
			  0xc0,0xf9,0xa4,0xb0,
			  0x99,0x92,0x82,0xf8,
			  0x80,0x90,0x88,0x83,
			  0xc6,0xa1,0x86,0x8e
			 };
unsigned char bit_tab[]={
			  0x80,0x40,0x20,0x10,
			  0x08,0x04,0x02,0x01
			 };

//{
//	unsigned int x,y;
//	for(x=num; x>0; x--)
//		for(y=5500; y>0; y--)
//		{
//			;//延时1ms
//		}
//}

void delay(unsigned int y)
{
	unsigned int x,z;
	for(x=y; x>0; x--);
		//for(z=110; z>0; z--);
}


void display_led(unsigned char which_bit, unsigned char which_number)
{  		//根据硬件手册修改这里
	switch(which_bit){
	case	1:
			*seg1 = seg_tab[which_number];
						break;
	case	2:
			*seg2 = seg_tab[which_number];
						break;
	case	3:
			*seg3 = seg_tab[which_number];
						break;
	case	4:
			*seg4 = seg_tab[which_number];
						break;
	case	5:
			*seg5 = seg_tab[which_number];
						break;
	case	6:
			*seg6 = seg_tab[which_number];
						break;
	case	7:
			*seg7 = seg_tab[which_number];
						break;
	case	8:
			*seg8 = seg_tab[which_number];
						break;
	}
}
/***********************************************************************/
//show_num函数：给进一个xxxx.xxxx的小数，在数码管中显示
void show_num(double num){
	//double num = 0.45;
	int numh = (int)num;
	char v1 = numh/1000;
	char v2 = numh%1000/100;
	char v3 = numh%100/10;
	char v4 = numh%10;
	int numl = (num-numh)*10000;
	char v5 = numl/1000;
	char v6 = numl%1000/100;
	char v7 = numl%100/10;
	char v8 = numl%10;
	char segval[8] = {v1,v2,v3,v4,v5,v6,v7,v8};
	//以上。完成了小数的转化，↓显示
	int i;
	for(i=0;i<8;i++){
		display_led(i+1,segval[i]);
	}
}



//void show_1rmb(char *type_low){
//	double cur_table[] = {0.1442,161.06,0.1261,0.11,16.365,0.193,2.0159,1.0};
//	int type = *type_low;
//	show_num(cur_table[type]);
//}






double toRmb(int i_cur,int type){
	double rate,result;
	//double cur_table[] = {0.1442,161.06,0.1261,0.11,0.193,2.0159,1.0};
	rate = cur_table[type-1];
//	printf("rate = %lf\n",rate);
	result = (double)(1.0/rate)*i_cur;
	return  result; //返回i_cur个外币可以兑换人民币的数目
}

double toCur(double rmb,int cur_type){
	//double cur_table[] = {0.1442,161.06,0.1261,0.11,0.193,2.0159,1.0};
	double rate = cur_table[cur_type-1];
	return (double)rate*rmb;  //返回rmb元可以兑换特定的外币的数目
}

double cal(int type1,int type2,int type1_num){

	double rmb_mid,result_in_type2;
//	scanf("%d %d %d",&type1,&type2,&type1_num);
	rmb_mid = toRmb(type1_num,type1);
	result_in_type2 = toCur(rmb_mid,type2);
	return result_in_type2;
//	printf("%lf",rmb_mid);
//	printf("类型%d的货币可以转化为类型%d的货币%lf元\n",type1,type2,result_in_type2);
//	return 0;
}

/******************************************************************************************************************************************************/
//扩展功能1：两个货币互相转化：左边的是type1

int main(){
	int ge = 0;
	int shi = 0;
	int bai = 0;
	int type1,type1_3bit,fun,gewei,shiwei,baiwei,cur_i,rmb_num;
	double num_o,rmb_o;

	while(1){
		type1 = *type_low;
		type1_3bit = decoder(type1);
		fun = *func;
		gewei = *key1;
		shiwei = *key2;
		baiwei = *key3;

		switch(fun){
			case 0:
				show_num(cur_table[type1_3bit-1]);
				break;
			case 1:
				//按键显示
					if (gewei == 0)
					{
						delay(1000000);

						if (gewei == 0)
							ge++;
					}
					//while(!gewei);
					if (shiwei == 0)
					{
						delay(1000000);
						if (shiwei == 0)
							shi++;
					}
					//while(!shiwei);
					if (baiwei == 0)
					{
						delay(1000000);
						if (baiwei == 0)
							bai++;
					}
					//while(!baiwei);
					double num_i = ge*0.0001 + shi*0.001 +bai*0.01;
					show_num(num_i);
					break;
			case 2:	//转化为rmb

				cur_i= ge + shi*10 + bai*100;
				num_o= toRmb(cur_i,type1_3bit);
				show_num(num_o);
				break;
			case 3: //转化为外币
			 	rmb_num =  ge + shi*10 + bai*100;
			 	rmb_o= toCur(rmb_num,type1_3bit);
			 	show_num(rmb_o);
			 	break;
			default:
				//show_num(8888.8888);
				break;
		}
	}
	}


		//double result = cal(type1_3bit,type2_3bit,1);
		//show_num(cur_table[type1_3bit-1])




 ////get_segvalue(double)函数 将xxxx。xxxx转化为每一位的值，返回数组
//char *get_segvalue(double num){
//	int numh = (int)num;
//	char v1 = numh/1000;
//	char v2 = numh%1000/100;
//	char v3 = numh%100/10;
//	char v4 = numh%10;
//	int numl = (num-numh)*10000;
//	char v5 = numl/1000;
//	char v6 = numl%1000/100;
//	char v7 = numl%100/10;
//	char v8 = numl%10;
//	char segval[8] = {v1,v2,v3,v4,v5,v6,v7,v8};
//	return segval;
//}
