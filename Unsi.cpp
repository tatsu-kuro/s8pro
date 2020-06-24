//unsi.cpp
#include "stdafx.h"
#include "Syaku8.h"
#include "Syaku8Doc.h"
#include "Syaku8View.h"
#include "math.h"
#include "tuner.h"
int lastinst;//last instrment
int lasttn;//last tone num.
extern int inst_para[30][10];
extern int ct[15][2];
extern DATADP DP;
#define mer 23
#define chu 24
#define kou 25
short ichi[16][2]={
	600,346,//�\���g�̍���
	0,0,	//
	50,44,	//2:����
	-25,44,	//3:���b
	20,20,	//4:���S
	20,4,	//5:
	40,15,	//6:
	40,40,	//7:
	70,-10,	//8:��
	70,20,	//9:�b
	-5,40,
     2,30,
	-13,13,
	18,18,//13:���̂�������
};

char nanakou[40][16]={
	13,4,mer,10, 0,0,  0,0,4,4,4,4,4,4,0,0,//ro
	13,4,  0,0,  0,0,  0,0,2,4,4,4,4,4,0,0,//ro
	0,1,   0,0,  0,0,  0,0,0,4,4,4,4,4,0,0,//ro
	14,4,  0,0,  0,0,  0,0,0,4,4,4,4,4,0,1,//
	1,1,mer,11,  0,0,  0,0,1,4,4,4,4,1,0,1,
	1,1,   0,0,  0,0,  0,0,0,4,4,4,4,0,0,1,
	15,4,  0,0,  0,0,  0,0,1,4,4,4,2,0,0,1,
	2,1,   0,0,  0,0,  0,0,0,4,4,4,0,0,0,1,
	16,4,  0,0,  0,0,  0,0,1,4,4,2,0,0,0,1,//�`
	4,1,   0,0,  0,0,  0,0,0,4,4,0,0,0,0,1,//�`
	17,4,  0,0,  0,0,  0,0,0,4,4,0,0,0,1,1,
	10,1,mer,12, 0,0,  0,0,1,4,1,0,4,4,0,0,
	10,1,  0,0,  0,0,  0,0,0,4,0,0,4,4,0,0,

	18,4,0,0,    0,0,  0,0,1,2,0,0,4,4,0,0,
	0,1, kou,3,  0,0,  0,0,0,4,4,4,4,4,0,0,
	14,4,kou,3,  0,0,  0,0,0,4,4,4,4,4,0,1,
	1,1,mer,11,kou,3,  0,0,1,4,4,4,4,1,0,1,
	1,1, kou,3,  0,0,  0,0,0,4,4,4,4,0,0,1,
	15,4,kou,3,  0,0,  0,0,1,4,4,4,2,0,0,1,
	2,1, kou,3,  0,0,  0,0,0,4,4,4,0,0,0,1,
	16,4,kou,3,  0,0,  0,0,1,4,4,2,0,0,0,1,//�`
	4,1, kou,3,  0,0,  0,0,0,4,4,0,0,0,0,1,//�`
	17,4,kou,3,  0,0,  0,0,0,4,4,0,0,0,1,1,
	10,1,kou,3,mer,12, 0,0,1,4,1,0,4,4,0,0,
	10,1,kou,3,  0,0,  0,0,0,4,0,0,4,4,0,0,

	18,4,kou,3,  0,0,  0,0,1,2,0,0,4,4,0,0,
	11,1,  0,0,  0,0,  0,0,1,0,0,4,4,4,0,0,
	8,1,   0,0,  0,0,  0,0,3,0,0,4,0,4,0,0,
	9,1,   0,0,  0,0,  0,0,0,0,4,0,0,0,0,1,
	1,1,  19,8,kou,9,  0,0,3,2,4,0,0,4,1,1,//tsu
//	1,1,  19,8,kou,9,  0,0,2,4,4,4,4,0,0,1,//tsu
	15,4, 19,8,kou,9,  0,0,1,4,4,4,2,0,0,1,//re
	2,1,  19,8,kou,9,  0,0,0,4,4,4,0,0,0,1,
	16,4, 19,8,kou,9,  0,0,0,4,0,4,0,0,0,1,//chi
	4,1,  19,8,kou,9,  0,0,0,4,0,4,4,4,0,0,
	17,4, 19,8,kou,9,  0,0,0,0,0,4,4,4,0,0,//hito
//	26,4,  0,0,  0,0,  0,0,0,11,0,0,0,0,1,1,
	10,1, 19,8,kou,9,mer,12,0,0,4,0,0,0,1,1,
	10,1, 19,8,kou,9,  0, 0,0,4,4,4,0,4,0,0,
	26,4,  0,0,  0,0,  0,0,0,11,0,0,0,0,1,1,
	11,1, 19,8,kou,9,  0,0,0,4,0,0,4,4,0,0
};
char tozan[40][14]={
	13,4,mer,10, 0,0,  0,0,4,4,4,4,8,4,
	13,4,  0,0,  0,0,  0,0,2,4,4,4,8,4,
	0,1,   0,0,  0,0,  0,0,0,4,4,4,8,4,
	14,4,  0,0,  0,0,  0,0,2,4,4,4,8,3,
	1,1,mer,11,  0,0,  0,0,1,4,4,4,8,1,
	1,1,   0,0,  0,0,  0,0,0,4,4,4,8,0,
	15,4,  0,0,  0,0,  0,0,1,4,4,8,2,0,
	2,1,   0,0,  0,0,  0,0,0,4,4,8,0,0,
	16,4,  0,0,  0,0,  0,0,1,4,8,2,0,0,//�`
	4,1,   0,0,  0,0,  0,0,0,4,8,0,0,0,//�`
	17,4,  0,0,  0,0,  0,0,2,8,2,0,4,4,
	10,1,mer,12, 0,0,  0,0,1,8,1,0,4,4,
	10,1,  0,0,  0,0,  0,0,0,8,0,0,4,4,

	18,4,0,0,    0,0,  0,0,1,2,7,0,4,4,
	0,1, kou,3,  0,0,  0,0,0,4,4,4,8,4,
	14,4,kou,3,  0,0,  0,0,2,4,4,4,8,3,
	1,1,mer,11,kou,3,  0,0,1,4,4,4,8,1,
	1,1, kou,3,  0,0,  0,0,0,4,4,4,8,0,
	15,4,kou,3,  0,0,  0,0,1,4,4,8,2,0,
	2,1, kou,3,  0,0,  0,0,0,4,4,8,0,0,
	16,4,kou,3,  0,0,  0,0,1,4,8,2,0,0,//�`
	4,1, kou,3,  0,0,  0,0,0,4,8,0,0,0,//�`
	17,4,kou,3,  0,0,  0,0,2,8,2,0,4,4,
	10,1,kou,3,mer,12, 0,0,1,8,1,0,4,4,
	10,1,kou,3,  0,0,  0,0,0,8,0,0,4,4,

	18,4,kou,3,  0,0,  0,0,1,2,7,0,4,4,
	11,1,  0,0,  0,0,  0,0,1,0,0,4,8,4,
	8,1,   0,0,  0,0,  0,0,3,0,0,8,0,4,
	9,1,   0,0,  0,0,  0,0,0,0,8,0,0,0,
	1,1,  19,8,kou,9,  0,0,2,4,4,4,4,0,
	15,4, 19,8,kou,9,  0,0,1,4,4,4,2,0,//re
	2,1,  19,8,kou,9,  0,0,0,4,4,4,0,0,
	16,4, 19,8,kou,9,  0,0,0,4,0,4,0,0,//chi
	4,1,  19,8,kou,9,  0,0,0,4,0,4,4,4,
	17,4, 19,8,kou,9,  0,0,0,0,0,4,4,4,//hito
//    26,4,  0,0,  0,0,  0,0,0,11,0,0,0,0,
	10,1, 19,8,kou,9,  mer,12,0,0,4,0,0,0,
	10,1, 19,8,kou,9,  0,0,0,4,4,4,0,4,
    26,4,  0,0,  0,0,  0,0,0,11,0,0,0,0,
	11,1, 19,8,kou,9,  0,0,0,4,0,0,4,4
};
char kinko[40][14]={
	0,1, 19,13, 0,0,  0, 0,4,4,4,4,8,4,//ro
	0,1,mer,13, 0,0,  0, 0,2,4,4,4,8,4,
	0,1,  0,0,  0,0,  0, 0,0,4,4,4,8,4,
	1,1,mer,2,  0,0,  0, 0,2,4,4,4,8,3,
	1,1,chu,2,  0,0,  0, 0,1,4,4,4,8,1,
	1,1,  0,0,  0,0,  0, 0,0,4,4,4,8,0,//tsu
	2,1,chu,5,  0,0,  0, 0,1,4,8,4,2,0,
	2,1,  0,0,  0,0,  0, 0,0,4,8,4,0,0,//re
	3,1,  0,0,  0,0,  0, 0,1,4,8,0,4,0,
	4,1,  0,0,  0,0,  0, 0,0,4,8,0,0,0,//chi
	5,1,mer,2,  0,0,  0, 0,2,8,3,0,4,4,
	5,1,chu,2,  0,0,  0, 0,1,8,2,0,4,4,
	5,1,  0,0,  0,0,  0, 0,0,8,0,0,4,4,//ri

	0,1,mer,13,kou,3, 0, 0,2,4,4,4,8,4,
	0,1,kou,3,  0,0,  0, 0,0,4,4,4,8,4,//ro
	1,1,mer,2,kou,3,  0, 0,2,4,4,4,8,3,
	1,1,chu,2,kou,3,  0, 0,1,4,4,4,8,1,
	1,1,kou,3,  0,0,  0, 0,0,4,4,4,8,0,
	2,1,kou,3,chu,5,  0, 0,1,4,8,4,2,0,
	2,1,kou,3,  0,0,  0, 0,0,4,8,4,0,0,
	4,1,kou,3,mer,2,  0, 0,1,4,8,1,0,0,
	4,1,kou,3,  0,0,  0, 0,0,4,8,0,0,0,
	6,1,mer,4,  0,0,  0, 0,2,8,3,0,0,4,
	6,1,chu,4,  0,0,  0, 0,1,8,2,0,0,4,
	6,1,  0,0,  0,0,  0, 0,0,8,0,0,0,4,//hi

	6,1,chu,6, 22,4,  0, 0,1,0,9,0,0,4,
	7,1, 22,4,  0,0,  0, 0,4,3,4,8,4,4,//ha
	7,1, 20,4,  0,0,  0, 0,1,3,4,2,8,4,
	7,1, 21,4,  0,0,  0, 0,3,0,8,0,0,4,
	1,1, 19,8,kou,9,  0, 0,1,4,4,4,4,3,//tsu
	2,1, 19,8,kou,9,chu, 5,0,4,4,4,4,0,
	2,1, 20,7, 22,5,  0, 0,1,4,2,4,0,0,
	4,1, 19,8,kou,9,mer, 2,0,4,0,4,0,0,
	4,1, 19,8,kou,9,  0, 0,3,4,0,4,4,4,
	6,1, 19,8,kou,9,mer, 4,0,0,4,0,4,0,
//    26,4, 0,0,  0,0,  0, 0,0,11,0,0,0,0,
	6,1, 19,8,kou,9,chu,4, 0,0,4,0,0,0,
	6,1, 19,8,kou,9,0,   0,3,4,4,4,0,4,
    26,4, 0,0,  0,0, 0, 0,0,11,0,0,0,0,
	7,1,22,4,19,8,kou,9,   0,4,0,0,4,4//ha
};
char okarina[22][12]={
	1,1,1,1,1, 1,1,1,1,1, 1,1,
	1,1,1,1,1, 1,1,1,1,1, 1,0,
	1,1,1,1,1, 1,1,1,1,1, 0,1,

	1,1,1,1,1, 1,1,1,1,1, 0,0,//c
	1,1,1,1,1, 1,1,1,0,1, 0,1,
	1,1,1,1,1, 1,1,1,0,1, 0,0,//d
	1,1,1,1,1, 1,1,0,0,1, 0,1,
	1,1,1,1,1, 1,1,0,0,1, 0,0,//e
	1,1,1,1,1, 1,0,0,0,1, 0,0,//f
	1,1,1,1,1, 0,0,1,0,1, 0,0,
	1,1,1,1,1, 0,0,0,0,1, 0,0,//g
	1,1,0,1,1, 0,0,1,0,1, 0,0,
	1,1,0,1,1, 0,0,0,0,1, 0,0,//a
	1,0,0,1,1, 0,0,1,0,1, 0,0,
	1,0,0,1,1, 0,0,0,0,1, 0,0,

	0,0,0,1,1, 0,0,0,0,1, 0,0,//c
	0,0,0,0,1, 0,0,0,0,1, 0,0,//
	0,0,0,1,0, 0,0,0,0,1, 0,0,//d
	0,0,0,0,0, 0,0,0,0,1, 0,0,//
	0,0,0,1,0, 0,0,0,0,0, 0,0,//e
	0,0,0,0,0, 0,0,0,0,0, 0,0,//
};
//void Disp12abs(CDC* MemDC,CDC* pDC,int a,int b,int da,int db,int n,int x,int y,int x1,int y1,int sx,int sy,int sx1,int sy1,int sx2,int sy2)
//{//                               dest_xy     dx,dy         0-1   sorce_xy    offset1     offset2
//	int xx,yy;
//	if(n==0){xx=x+x1;yy=y+y1;}
//	else if(n==1) {xx=x+sx;yy=y+sy;}
//	else if(n==2){xx=x+sx1;yy=y+sy1;}
//	else if(n==3){xx=x+sx2;yy=y+sy2;}
//	pDC->BitBlt(a+x1,b+y1,da,db,MemDC,xx,yy,SRCCOPY);
//}
//�y�J�^�w ������]�ӏ��z
//���̉����͔��{���q�i�b�j�ł̎����ł��B
//���͔��J�`�J�U�V�w�̈Ӗ��ł��B
//�� �ʒu
//
//A4 [������|��������]���� ������҂ɂ�鍷���傫��	//0,1,	1,1,1,1,1,1,1,1,//1:meri
//Bb4[������|��������]�J�� ������҂ɂ�鍷���傫��	//1,1,	1,1,1,1,1,1,1,2,//2:kari
//B4 [������|��������]����							//2,1,	1,1,1,1,1,1,2,1,
//�i�u�����v�u�J���v�͉�ʍ��A�̌����ɋL�ڂ��肢���܂��j
//F5 [������|��������]								//8,2,	1,1,1,0,0,0,1,0,
//F5 [������|��������]���ւ��w						//8,3,  1,1,1,0,1,0,0,0,
//�i�ւ��w�́A�\���������ł�����Ȃ��Ă��������B�ȉ����l�j
//Gb5,Ab5,Bb5�F�����R�̎w�E�i����j�̔��J��
//��ʂł̉������J����`�ɕύX���肢���܂��B
//Bb5[������|��������]			//10,2, 2,0,0,0,0,0,1,0,
//Bb5[������|��������]���ւ��w	//10,3, 1,0,1,1,1,1,1,0,
//B5 [������|��������]			//11,2, 0,0,0,0,0,0,1,0,
//B5 [������|��������]���ւ��w	//11,3, 0,1,1,1,1,1,1,0,
//F6 [������|��������]			//17,2, 0,1,1,1,0,0,0,1,
//F6 [������|��������]���ւ��w	//17,3, 1,1,1,0,1,0,0,0,
//Gb6,Ab6,Bb6�F�����R�̎w�E�i����j�̔��J��
//��ʂł̉������J����`�ɕύX���肢���܂�
//Bb6[������|��������]			//23,4, 2,0,0,0,0,0,1,0,
//Bb6[������|��������]���ւ��w1	//23,5, 1,0,1,1,1,1,1.0,
//Bb6[������|��������]���ւ��w2	//23,6, 0,1,0,0,0,0,1,0,
//B6 [������|��������]			//24,1, 0,0,0,0,0,0,1,0,
//B6 [������|��������]���ւ��w  //24,2, 0,1,1,1,0,0,1,0,
//Db7[������|��������]			//27,3, 1,1,2,1,1,1,0,0,
//Db7[������|��������]���ւ��w1	//24,4, 1,1,0,0,0,0,1,0,
//Db7[������|��������]���ւ��w2	//24,5, 1,1,2,0,0,0,1,0,
//D7 [������|��������]			//25,2, 1,1,0,1,1,0,0,0,
//D7 [������|��������]���ւ��w	//25,2, 1,1,2,1,1,0,0,1,
//Eb7[������|��������]			//26,0, 1,2,0,1,0,0,1,0,
//E7 [������|��������]			//27,3, 1,0,1,1,0,0,1,1,
//E7 [������|��������]���ւ��w1	//27,4, 1,0,0,1,1,0,0,0,
//E7 [������|��������]���ւ��w2	//27,5, 1,0,1,1,0,0,0,0,
//F7 [������|��������]			//28,3, 1,0,1,1,1,1,1,0,
//F7 [������|��������]���ւ��w1	//28,4, 1,0,1,1,1,1,0,0,
//F7 [������|��������]���ւ��w2	//28,5, 1,0,1,1,0,1,0,0,
//Gb7[������|��������]			//29,0, 0,0,1,0,0,1,0,0,
//G7 [������|��������]			//30,1, 0,1,0,0,1,0,0,0,
//G7 [������|��������]���ւ��w	//30,2, 0,1,0,0,0,1,0,0,
//Ab7[������|��������]			//31,0, 0,1,0,1,1,1,0,0,
//A7 [������|��������]			//32,1, 1,0,0,1,0,1,0,0,
//A7 [������|��������]���ւ��w	//32,2, 1,2,0,1,0,1,0,0,
//Bb7[������|��������]			//33,0, 1,0,1,0,1,0,1,0,
//B7 [������|��������]			//34,0, 2,0,2,0,1,0,0,0,
//C8 [������|��������]			//35,0, 0,1,2,1,1,0,0,0,
//�i�ȏ�ł��j
char sino[56][10]={
0,1,	1,1,1,1,1,1,1,1,//1:meri//A4 [������|��������]���� ������҂ɂ�鍷���傫��	//
1,1,	1,1,1,1,1,1,1,2,//Bb4[������|��������]�J�� ������҂ɂ�鍷���傫��	////2:kari	1,1,1,1,1,1,1,//Bb2
2,1,	1,1,1,1,1,1,2,1,//B4 [������|��������]����							//	3,3,3,3,3,3,3,
3,1,	1,1,1,1,1,1,0,0,//c
4,1,	1,1,1,1,1,2,0,0,
5,1,	1,1,1,1,1,0,0,0,//d
6,1,	1,1,1,1,2,0,0,0,
7,1,	1,1,1,1,0,0,0,0,//e
//8,4,	1,1,1,0,1,0,0,0,
8,2,	1,1,1,0,0,0,1,0,//F5 [������|��������]								//
8,3,	1,1,1,0,1,0,0,0,//F5 [������|��������]���ւ��w						//
9,1,	1,1,2,0,0,0,0,0,
10,1,	1,1,0,0,0,0,1,0,//g
11,1,	1,2,0,0,0,0,1,0,
12,1,	1,0,0,0,0,0,1,0,//a
13,2,   2,0,0,0,0,0,1,0,//Bb5[������|��������]			//
13,3,	1,0,1,1,1,1,1,0,//Bb5[������|��������]���ւ��w	//
14,2,	0,0,0,0,0,0,1,0,//B5 [������|��������]			//
14,3,	0,1,1,1,1,1,1,0,//B5 [������|��������]���ւ��w	//
15,1,	1,1,1,1,1,1,0,0,//c
16,1,	1,1,1,1,1,2,0,0,
17,1,	1,1,1,1,1,0,0,0,//d
18,1,	1,1,1,1,2,0,0,0,
19,1,	1,1,1,1,0,0,0,0,//e
20,2,	1,1,1,0,0,0,1,0,//F6 [������|��������]			//
20,3,	1,1,1,0,1,0,0,0,//F6 [������|��������]���ւ��w	//	1,1,1,0,1,0,0,
21,1,	1,1,2,0,0,0,0,0,
22,1,	1,1,0,0,0,0,1,0,//g
23,1,	1,2,0,0,0,0,1,0,
24,1,	1,0,0,0,0,0,1,0,//a
25,4,	2,0,0,0,0,0,1,0,//Bb6[������|��������]		2,0,0,0,0,0,1,
25,5,	1,0,1,1,1,1,1,0,//Bb6[������|��������]���ւ��w1	//
25,6,	0,1,0,0,0,0,1,0,//Bb6[������|��������]���ւ��w2	//
26,2,	0,0,0,0,0,0,1,0,//B6 [������|��������]		0,0,0,0,0,0,1,
26,3,	0,1,1,1,0,0,1,0,//B6 [������|��������]���ւ��w  //	
27,1,	0,1,1,0,0,0,1,0,//c
28,4,	1,1,2,1,1,1,0,0,//Db7[������|��������]			
28,5,	1,1,0,0,0,0,1,0,//Db7[������|��������]���ւ��w1	//
28,6,	1,1,2,0,0,0,1,0,//Db7[������|��������]���ւ��w2	//
29,2,	1,1,0,1,1,0,0,0,//D7 [������|��������]			//	1,1,0,1,1,0,0,
29,3,	1,1,2,1,1,0,0,0,//D7 [������|��������]���ւ��w	//
30,1,	1,2,0,1,1,0,0,0,//Eb7[������|��������]			//
31,4,	1,0,1,1,0,0,1,0,//E7 [������|��������]			//
31,5,	1,0,0,1,1,0,0,0,//E7 [������|��������]���ւ��w1	//
31,6,	1,0,1,1,0,0,0,0,//E7 [������|��������]���ւ��w2	//
32,4,	1,0,1,1,1,1,1,0,//F7 [������|��������]			//
32,5,	1,0,1,1,1,1,0,0,//F7 [������|��������]���ւ��w1	//
32,6,	1,0,1,1,0,1,0,0,//F7 [������|��������]���ւ��w2	//
33,1,	0,0,1,0,0,1,0,0,//Gb7[������|��������]			//
34,2,	0,1,0,0,1,0,0,0,//G7 [������|��������]			//
34,3,	0,1,0,0,0,1,0,0,//G7 [������|��������]���ւ��w	//
35,1,	0,1,0,1,1,1,0,0,//Ab7[������|��������]			//
36,2,	1,0,0,1,0,1,0,0,//A7 [������|��������]			//
36,3,	1,2,0,1,0,1,0,0,//A7 [������|��������]���ւ��w	//
37,1,	1,0,1,0,1,0,1,0,//Bb7[������|��������]			//
38,1,	2,0,2,0,1,0,0,0,//B7 [������|��������]			//
0,0,	0,0,0,0,0,0,0,0
};
char ocarina4[15][5]={//LeftUp,RightUp,LeftDown,RightDown 0:open 1:half 2:close
0, 3,3,3,3,
1, 3,2,3,3,
2, 3,1,3,3,//2,0,2,2,
3, 3,3,3,2,//2,2,2,1,
4, 3,3,3,1,//2,2,2,0,
5, 1,3,3,3,//0,2,2,2,
6, 3,1,3,1,//2,0,2,0,
7, 1,3,3,1,//0,2,2,0,
8, 1,1,3,3,//0,0,2,2,
9, 1,1,1,3,//0,0,0,2,
10,1,1,3,1,//0,0,2,0,
11,1,3,1,1,//0,2,0,0,
12,1,1,1,1,//0,0,0,0,
};
char bansuri[15][7]={
0, 3,3,3,3,3,3,//C
1, 3,3,3,3,3,2,
2, 3,3,3,3,3,1,//D
3, 3,3,3,3,2,1,
4, 3,3,3,3,1,1,//E
5, 3,3,3,1,1,1,//F
6, 3,3,2,1,1,1,
7, 3,3,1,1,1,1,//G
8, 3,2,1,1,1,1,
9, 3,1,1,1,1,1,//A
10,2,1,1,1,1,1,
11,1,1,1,1,1,1,//B
};

char tin[100][8]={//1:open 2:han kai 3:close
0,5,	3,3,3,3,3,3,//
1,5,	3,3,3,3,3,2,//
2,5,	3,3,3,3,3,1,//
3,4,	3,3,3,3,2,3,//
3,6,	3,3,3,3,2,1,//
4,5,	3,3,3,3,1,1,//
5,4,	3,3,3,1,1,1,//
5,6,	3,3,3,1,1,3,//
6,3,	3,3,2,1,1,1,//c
6,5,	3,3,1,3,3,3,
6,7,	3,3,1,3,3,1,//d
7,4,	3,3,1,1,1,1,
7,6,	3,3,1,1,3,3,//e
8,4,	3,1,3,3,3,3,//F5						//
8,6,	3,2,1,1,1,1,//F5
9,3,	3,1,1,1,1,1,
9,5,	3,1,1,3,3,3,//g
9,7,	2,3,3,1,3,3,
10,0,	1,3,3,1,1,1,//a
10,2,   1,3,3,3,1,1,//Bb5
10,4,	2,1,1,1,1,1,//Bb5
10,6,	1,3,1,3,3,3,//a
10,8,   1,3,3,1,3,3,//Bb5
10,10,	1,3,3,1,1,3,//Bb5
11,3,	1,1,1,1,1,1,//B5
11,5,	1,1,1,1,1,3,//B5
11,7,	1,1,1,3,3,3,//B5

12,3,	1,3,3,3,3,3,//
12,5,	3,3,3,3,3,3,//
12,7,	2,3,3,3,3,3,//
13,5,	3,3,3,3,3,2,//
14,5,	3,3,3,3,3,1,//
15,4,	3,3,3,3,2,3,//
15,6,	3,3,3,3,2,1,//
16,5,	3,3,3,3,1,1,//
17,4,	3,3,3,1,1,1,//
17,6,	3,3,3,1,1,3,//
18,2,	3,3,1,3,3,1,//c
18,4,	3,3,2,1,1,1,//c
18,6,	3,3,1,3,3,3,
18,8,	3,3,1,3,3,1,//d
19,3,	3,3,1,1,1,1,
19,5,	3,3,1,1,3,3,//e
19,7,	3,3,3,3,3,3,//e
20,4,	3,2,1,1,1,1,//F5						//
20,6,	3,1,3,1,1,1,//F5
21,2,	3,1,1,1,1,1,
21,4,	3,1,3,3,3,3,
21,6,	3,1,1,3,3,3,//g
21,8,	3,3,3,3,3,1,
22,1,	1,3,3,1,1,1,//a
22,3,	1,3,1,3,3,3,//Bb5
22,5,	2,1,1,1,1,1,//a
22,7,   1,3,3,1,3,3,//Bb5
22,9,	1,3,3,1,1,3,//Bb5
23,1,	1,1,1,1,1,1,//B5
23,3,	1,1,1,1,1,3,//B5
23,5,	1,1,1,3,3,3,//B5
23,7,	1,3,3,3,1,1,//B7
23,9,	3,3,3,3,1,1,//B5

24,2,	1,3,3,3,3,3,//
24,4,	1,3,3,1,1,1,//
24,6,	3,3,3,3,3,3,//
24,8,	3,3,3,3,1,1,//
25,3,	3,3,3,3,3,2,//
25,5,	1,3,2,1,1,1,//
25,7,	1,3,3,3,1,3,//
26,3,	3,3,3,3,3,1,//
26,5,	3,1,1,3,3,1,//
26,7,	3,3,1,1,1,3,//
27,4,	3,3,3,3,2,3,//
27,6,	3,3,3,3,2,1,//
28,3,	3,3,3,3,1,1,//
28,5,	3,3,3,3,1,3,//
28,7,	3,1,3,3,1,1,//
29,3,	3,3,3,1,1,3,//
29,5,	3,3,3,1,1,1,//
29,7,	3,1,3,1,1,1,//
30,11,	1,1,1,1,1,1,
31,5,	1,3,3,3,3,1,
32,11,	1,1,1,1,1,1,
33,4,	3,1,3,1,3,1,
33,6,	1,3,1,3,1,1,//g
34,11,	1,1,1,1,1,1,
35,4,	1,1,1,1,1,1,//B5
35,6,	1,3,1,1,1,1,//B5
-1,0,	0,0,0,0,0,0,
};


void InsPict(CDC* MemDC,CDC* pDC,int a1,int b1,int a2,int b2,int a3,int b3,int x,int y,int dx,int dy)
{
	pDC->FillSolidRect(a1,b1,a2,b3-b1,RGB(255,255,255));
	pDC->FillSolidRect(a1,b3+dy,a2,b1+b2-b3-dy,RGB(255,255,255));
	pDC->FillSolidRect(a1,b3,a3-a1,dy,RGB(255,255,255));
	pDC->FillSolidRect(a3+dx,b3,a1+a2-a3-dx,dy,RGB(255,255,255));
	pDC->BitBlt(a3,b3,dx,dy,MemDC,x,y,SRCCOPY);
}
void unsi(CDC* MemDC,CDC* pDC,int tone,int flag)
{//	char *abc[15]={"a","b$","b","c","d$","d","e$","e","f","g$","g","a$","a"};
	//sinobue ocarina ocarina4 syakuhachi ���������ł��B
	int key;
	if(flag)lastinst=0;
	int tn,maxtn,mintn;
	tn=tone-inst_para[DP.inst][2];
	mintn=inst_para[DP.inst][2];
	maxtn=inst_para[DP.inst][3];
	if(DP.inst==SINOBUE&&tone-DP.sino>=mintn&&tone-DP.sino<=maxtn){//sinobue
		tn=tn-DP.sino;
		int i,j,a=unX+9,b,x=560,y=145;//x,y��a,b��
		//b=unY+53+10-30;
		pDC->FillSolidRect(unX,unY,195,122,RGB(255,255,255));
		for(i=tn;i<58;i++)if(sino[i][0]==tn)break;
		for(j=0;j<3;j++){
			if(sino[i+j][0]==tn){
				if(sino[i+j][1]==1)b=unY+50+10;
				else if(sino[i+j][1]==2)b=unY+50+10-15;
				else if(sino[i+j][1]==3)b=unY+50+10+15;
				else if(sino[i+j][1]==4)b=unY+50-15;
				else if(sino[i+j][1]==5)b=unY+50+10;
				else b=unY+50+35;
				pDC->BitBlt(a,b,178,18,MemDC,x,y,SRCCOPY);
				Disp12abs(MemDC,pDC,a,b,13,10,sino[i+j][2],x,y,16,    4,7,-10,32+25,-10,57,-10);
				Disp12abs(MemDC,pDC,a,b,13,10,sino[i+j][3],x,y,16+22, 4,7,-10,32+25,-10,57,-10);
				Disp12abs(MemDC,pDC,a,b,13,10,sino[i+j][4],x,y,16+44, 4,7,-10,32+25,-10,57,-10);
				Disp12abs(MemDC,pDC,a,b,13,10,sino[i+j][5],x,y,16+66, 4,7,-10,32,-10,57,-10);
				Disp12abs(MemDC,pDC,a,b,13,10,sino[i+j][6],x,y,16+88, 4,7,-10,32,-10,57,-10);
				Disp12abs(MemDC,pDC,a,b,13,10,sino[i+j][7],x,y,16+110,4,7,-10,32,-10,57,-10);
				Disp12abs(MemDC,pDC,a,b,10,8,sino[i+j][8],x,y, 149,   5,21,-10,46,-10,71,-10);
//				if(sino[i+j][9]==2)pDC->BitBlt(a+2,b+4,4,11,MemDC,743,139,SRCCOPY);
//				else if(sino[i+j][9]==1)pDC->BitBlt(a+2,b+4,4,11,MemDC,743,151,SRCCOPY);
//				else pDC->BitBlt(a+2,b+4,4,11,pDC,a+2+4,b+4,SRCCOPY);		
				if(sino[i+j][9]==2){
					pDC->BitBlt(a+2,b-2,10,10,MemDC,682,135,SRCAND);
					pDC->BitBlt(a+2,b-2+11,10,10,MemDC,694,135,SRCAND);
				}
				else if(sino[i+j][9]==1){
					pDC->BitBlt(a+2,b-2,10,10,MemDC,670,135,SRCAND);
					pDC->BitBlt(a+2,b-2+11,10,10,MemDC,694,135,SRCAND);
				}
				else pDC->BitBlt(a+2,b+4,4,11,pDC,a+2+4,b+4,SRCCOPY);		
			}
		}
		printabc(MemDC,pDC,"By Iwa!5take",unX+120,unY+101,SRCAND);
		pDC->FillSolidRect(unX+120+39,unY+101+15,4,1,RGB(0,0,0));
		lastinst=SINOBUE;
	}
	else if(DP.inst==TIN&&tone-DP.tin>=mintn&&tone-DP.tin<=maxtn){//sinobue
		tn=tn-DP.tin;
		int i,a=unY+10,x=760,y=0;//x,y��a,b��
		int b,tb=unX+35;
		pDC->FillSolidRect(unX,unY,195,122,RGB(255,255,255));
		for(i=0;tin[i][0]!=-1;i++){
			if(tin[i][0]==tn){
				if     (tin[i][1]==0)b=tb;
				else if(tin[i][1]==1)b=tb+13*1;
				else if(tin[i][1]==2)b=tb+13*2;
				else if(tin[i][1]==3)b=tb+13*3;
				else if(tin[i][1]==4)b=tb+13*4;
				else if(tin[i][1]==5)b=tb+13*5;
				else if(tin[i][1]==6)b=tb+13*6;
				else if(tin[i][1]==7)b=tb+13*7;
				else if(tin[i][1]==8)b=tb+13*8;
				else if(tin[i][1]==9)b=tb+13*9;
				else if(tin[i][1]==10)b=tb+13*10;
				if(tin[i][1]==11){
					printabc(MemDC,pDC,"Tin Whistle fingering charts",unX+25,unY+43);
					printabc(MemDC,pDC,"By Coach John S. Nagy",unX+35,unY+63);
				}else{
					pDC->BitBlt(b,a,20,105,MemDC,x,y,SRCCOPY);
					Disp12abs(MemDC,pDC,b,a,9,9,tin[i][2],x,y,5,29,5,29,5,41,5,53);
					Disp12abs(MemDC,pDC,b,a,9,9,tin[i][3],x,y,5,41,5,29,5,41,5,53);
					Disp12abs(MemDC,pDC,b,a,9,9,tin[i][4],x,y,5,53,5,29,5,41,5,53);
					Disp12abs(MemDC,pDC,b,a,9,9,tin[i][5],x,y,5,65,5,29,5,41,5,53);
					Disp12abs(MemDC,pDC,b,a,9,9,tin[i][6],x,y,5,77,5,29,5,41,5,53);
					Disp12abs(MemDC,pDC,b,a,9,9,tin[i][7],x,y,5,89,5,29,5,41,5,53);
				}
			}
		}
		lastinst=TIN;
	}
	else if(DP.inst==BANSURI&&tone-DP.bansuri>=mintn&&tone-DP.bansuri<=maxtn){
		int a=unX+13,b=unY+50,x=586,y=763;
		tn=tn-DP.bansuri;tn=tn%12;
		if(lastinst!=BANSURI){
			InsPict(MemDC,pDC,unX,unY,195,121,a,b,x,y,171,30);
//			pDC->BitBlt(a+15,b+35,33,33,MemDC,x+15,y+35,SRCCOPY);
			lastinst=BANSURI;
		}
		Disp12abs(MemDC,pDC,a,b,11,11,bansuri[tn][1],x,y, 72, 5,72, 5, 87,5,102,5);//780,121
		Disp12abs(MemDC,pDC,a,b,11,11,bansuri[tn][2],x,y, 87, 5,72, 5, 87,5,102,5);//814,121,
		Disp12abs(MemDC,pDC,a,b,11,11,bansuri[tn][3],x,y,102, 5,72, 5, 87,5,102,5);//780,157
		Disp12abs(MemDC,pDC,a,b,11,11,bansuri[tn][4],x,y,117, 5,72, 5, 87,5,102,5);//813,157
		Disp12abs(MemDC,pDC,a,b,11,11,bansuri[tn][5],x,y,132, 5,72, 5, 87,5,102,5);//780,157
		Disp12abs(MemDC,pDC,a,b,11,11,bansuri[tn][6],x,y,147, 5,72, 5, 87,5,102,5);//813,157
		printabc(MemDC,pDC,"By Harshavardhan A Agashe",unX+22,unY+90,SRCAND);
	}
	else if(DP.inst==OCARINA4&&tone-DP.oca4>=mintn&&tone-DP.oca4<=maxtn){//sinobue
		int a=unX+50,b=unY+10-5,x=760,y=105;
		tn=tn-DP.oca4;
		if(lastinst!=OCARINA4){
			InsPict(MemDC,pDC,unX,unY,195,121,a,b,x,y,92,101);
			pDC->BitBlt(a+15,b+35,33,33,MemDC,x+15,y+35,SRCCOPY);
			lastinst=OCARINA4;
		}
		Disp12abs(MemDC,pDC,a,b,18,18,ocarina4[tn][1],x,y,20,16,20,16,20,52,54,16);//780,121
		Disp12abs(MemDC,pDC,a,b,18,18,ocarina4[tn][2],x,y,54,16,20,16,20,52,54,16);//814,121,
		Disp12abs(MemDC,pDC,a,b,18,18,ocarina4[tn][3],x,y,20,52,20,16,20,52,54,16);//780,157
		Disp12abs(MemDC,pDC,a,b,18,18,ocarina4[tn][4],x,y,54,52,20,16,20,52,54,16);//813,157
		printabc(MemDC,pDC,"By Coach John S. Nagy",unX+35,unY+100,SRCAND);
	}
	else if(DP.inst==OCARINA&&tone-DP.oca>=mintn&&tone-DP.oca<=maxtn){//sinobue
		int a=unX+51,b=unY+31,x=417,y=107;
		tn=tn-DP.oca;
		if(lastinst!=OCARINA){
			InsPict(MemDC,pDC,unX,unY,196,121,a,b,x,y,102,36);
			pDC->BitBlt(a+15,b+35,33,33,MemDC,x+15,y+35,SRCCOPY);
			lastinst=OCARINA;
		}
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][0],x,y,4,9,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][1],x,y,14,10,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][2],x,y,24,8,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][3],x,y,33,4,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][4],x,y,8,26,0,37);

		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][5],x,y,42,19,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][6],x,y,51,14,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][7],x,y,63,10,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][8],x,y,75,10,0,37);
		Disp12abs(MemDC,pDC,a,b,7,7,okarina[tn][9],x,y,50,28,0,37);

		Disp12abs(MemDC,pDC,a,b,4,4,okarina[tn][10],x,y,41,14,7,37);
		Disp12abs(MemDC,pDC,a,b,4,4,okarina[tn][11],x,y,50,9,7,37);
	}
	else if(DP.inst==FULLRANGE&&tone-DP.voca>=mintn&&tone-DP.voca<=maxtn){//full range
		int a,b,d,aa=abcX,bb=abcY;
		if(DP.cdedore==1){a=206;b=56;d=21;}
		else {a=abcX;b=abcY;d=23;}
		tn=tn-DP.voca+3;//abcX lasidoX ��A,La����n�܂�B�̂�+3
		pDC->FillSolidRect(unX,unY,196,121,RGB(255,255,255));//MemDC,whiteX,whiteY,1,1,SRCCOPY);
		pDC->StretchBlt(unX+61,unY+28,24*3,23*3,MemDC,a+25*(ct[tn%12][0]),b,24,d,SRCCOPY);
		if(ct[tn%12][1])pDC->StretchBlt(unX+59+24*3,unY+38-10*3,13*3,23*3,MemDC,aa+25*7,bb,13,23,SRCAND);
		lastinst=FULLRANGE;
	}
	else if((DP.inst==TOZAN||DP.inst==KINKO||DP.inst==NANAKOU)&&
			(tone+DP.sn>=mintn&&tone+DP.sn<=maxtn)){
		static int lastsun;
		if(DP.inst==lastinst&&tone==lasttn&&DP.sn==lastsun)return;
		key=tn+DP.sn;

		pDC->FillSolidRect(unX,unY,196,121,RGB(255,255,255));//MemDC,whiteX,whiteY,1,1,SRCCOPY);
		int i,x1,y1,x,y,xd,yd,rot[100][18];
		if(DP.inst==KINKO)for(i=0;i<99;i++)for(x=0;x<14;x++)rot[i][x]=kinko[i][x];
		else if(DP.inst==TOZAN)for(i=0;i<99;i++)for(x=0;x<14;x++)rot[i][x]=tozan[i][x];
		else for(i=0;i<99;i++)for(x=0;x<16;x++)rot[i][x]=nanakou[i][x];
		for(i=0;i<4;i++){//���c���`�@�����@�b�@��b
			if(rot[key][i*2+1]!=0){
				if(rot[key][i*2]>=13){
					xd=33;yd=33;
					x=273+34*((rot[key][i*2]-13+8)%8);
					y=593+34*((rot[key][i*2]-13)/8);
				}
				else {
					xd=67;yd=67;
					x=1+68*((rot[key][i*2]+8)%8);
					y=525+68*(rot[key][i*2]/8);
				}
				x1=unX-554+ichi[0][0]+ichi[rot[key][i*2+1]][0];
				y1=unY-322+ichi[0][1]+ichi[rot[key][i*2+1]][1];
				pDC->BitBlt(x1,y1,xd,yd,MemDC,x,y,SRCAND);
			}
		}
//unX 554 unY 322
		x=unX+150;y=unY+10;
		x1=635;y1=526;
		pDC->BitBlt(x+6,y+24,34,80,MemDC,x1+93,y1,SRCCOPY);
		if(DP.inst==KINKO)pDC->BitBlt(x-2,y,40,24,MemDC,x1+39,rot[key][8]*25+y1,SRCCOPY);
		else pDC->BitBlt(x-2,y,40,24,MemDC,x1-2,rot[key][8]*25+y1,SRCCOPY);
		pDC->BitBlt(x+13,y+26,9,10,MemDC,x1+81,rot[key][ 9]*9+y1-2,SRCCOPY);
		pDC->BitBlt(x+20,y+38,9,10,MemDC,x1+81,rot[key][10]*9+y1-2,SRCCOPY);
		pDC->BitBlt(x+20,y+50,9,10,MemDC,x1+81,rot[key][11]*9+y1-2,SRCCOPY);
		pDC->BitBlt(x+20,y+62,9,10,MemDC,x1+81,rot[key][12]*9+y1-2,SRCCOPY);
		pDC->BitBlt(x+20,y+74,9,10,MemDC,x1+81,rot[key][13]*9+y1-2,SRCCOPY);
		if(DP.inst==NANAKOU){
			if(rot[key][14])pDC->BitBlt(x+15,y+47,6,5,MemDC,x1+81,y1+113,SRCCOPY);//7kou
			else pDC->BitBlt(x+15,y+47,6,5,MemDC,x1+81,y1+118,SRCCOPY);//7kou
			if(rot[key][15])pDC->BitBlt(x+15,y+82,6,5,MemDC,x1+81,y1+113,SRCCOPY);//7kou
			else pDC->BitBlt(x+15,y+82,6,5,MemDC,x1+81,y1+118,SRCCOPY);//7kou
		}
		lastinst=DP.inst;//TOZAN;
		lastsun=DP.sn;
	}else{
		if(lastinst!=OUTKEY)pDC->FillSolidRect(unX,unY,196,121,RGB(255,255,255));
		lastinst=OUTKEY;
	}
	printinkey(MemDC,pDC,tone);//,lastinst);
	lasttn=tone;
}

short nt[7][6]={//number table
//	�J�n    rect  ����  �\���Ԋu��
	570,85,16,26,18,20,//on ryou
	374,178,6,8,6,6,//330,124,7, 12,7,7,//very small
	20,125,30,50,31,31,//big black
	559,474-8,8,9,8,8,//notes no komoji
	272,113,8,11,8,8,//notes no oomoji
	330,124,7, 12,7,7,//very small
};
void Num(CDC* MemDC,CDC* pDC,int n,int x,int y,int keta,int numb,int f)
{
	int x1=nt[n][0],y1=nt[n][1],xd=nt[n][2],yd=nt[n][3],d=nt[n][4],dd=nt[n][5];
	x=x+(keta-5)*dd;
	if(keta>=5)pDC->BitBlt(x+0*dd,y,xd,yd,MemDC,x1+d*((numb/10000)%10),y1,f);
	if(keta>=4)pDC->BitBlt(x+1*dd,y,xd,yd,MemDC,x1+d*((numb/1000)%10),y1,f);
	if(keta>=3)pDC->BitBlt(x+2*dd,y,xd,yd,MemDC,x1+d*((numb/100)%10),y1,f);
	if(keta>=2)pDC->BitBlt(x+3*dd,y,xd,yd,MemDC,x1+d*((numb/10)%10),y1,f);
			   pDC->BitBlt(x+4*dd,y,xd,yd,MemDC,x1+d*(numb%10),y1,f);
}
void DispDB(CDC* MemDC,CDC* pDC,double volt)
{
	double tmp;
	volt=volt-DP.dB;//waveNow��volt
	if(volt>0)tmp=volt;
	else tmp=-volt;
	Num(MemDC,pDC,2,606,21,3,int(tmp));
	Num(MemDC,pDC,2,606+102,21,1,int(tmp*10));
	if(volt>0)
		pDC->FillSolidRect(586,50-7,16,7,RGB(255,255,255));//MemDC,whiteX,whiteY,1,1,SRCCOPY);
	else pDC->BitBlt(586,50-7,16,7,MemDC,638,29,SRCCOPY);
}