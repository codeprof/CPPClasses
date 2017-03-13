#include <Windows.h>
#include <iostream>

const float K = 0.125; // use factor 1.0/8 instead of  1.0 /sqrt(8) so we do not need to multiply it when doing IDCT

static const float a1 = 1.3870398453221475f;
static const float a2 = 1.3065629648763766f;
static const float b1 = 1.1758756024193588f;
static const float b2 = 0.54119610014619712f;
static const float c1 = 0.78569495838710224f;
static const float d1 = 0.27589937928294311f;

//Sadly these constants are needed for optimization, because VC++ 2010 is to silly to do the optimization by itself... 
static const float _a1 = 1.3870398453221475f  * K;
static const float _a2 = 1.3065629648763766f  * K;
static const float _b1 = 1.1758756024193588f  * K;
static const float _b2 = 0.54119610014619712f * K;
static const float _c1 = 0.78569495838710224f * K;
static const float _d1 = 0.27589937928294311f * K;

//DCT transformation
//  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00  
//    a1,    b1,    c1,    d1,   -d1,   -c1,   -b1,   -a1  
//    a2,    b2,   -b2,   -a2,   -a2,   -b2,    b2,    a2  
//    b1,   -d1,   -a1,   -c1,    c1,    a1,    d1,   -b1  
//  1.00, -1.00, -1.00,  1.00,  1.00, -1.00, -1.00,  1.00  
//    c1,   -a1,    d1,    b1,   -b1,   -d1,    a1,   -c1  
//    b2,   -a2,    a2,   -b2,   -b2,    a2,   -a2,    b2  
//    d1,   -c1,    b1,   -a1,    a1,   -b1,    c1,   -d1  

//DCT inverse transformation
//  1.00,    a1,    a2,    b1,  1.00,    c1,    b2,    d1  
//  1.00,    b1,    b2,   -d1, -1.00,   -a1,   -a2,   -c1  
//  1.00,    c1,   -b2,   -a1, -1.00,    d1,    a2,    b1  
//  1.00,    d1,   -a2,   -c1,  1.00,    b1,   -b2,   -a1  
//  1.00,   -d1,   -a2,    c1,  1.00,   -b1,   -b2,    a1  
//  1.00,   -c1,   -b2,    a1, -1.00,   -d1,    a2,   -b1  
//  1.00,   -b1,    b2,    d1, -1.00,    a1,   -a2,    c1  
//  1.00,   -a1,    a2,   -b1,  1.00,   -c1,    b2,   -d1 


// Performs a horizontal one dimensional DCT transformation of block size 8 
void TransformRow(float** arr, int y)
{
	float s1 = arr[0][y] + arr[7][y];
	float m1 = arr[0][y] - arr[7][y];
	float s2 = arr[1][y] + arr[6][y];
	float m2 = arr[1][y] - arr[6][y];
	float s3 = arr[2][y] + arr[5][y];
	float m3 = arr[2][y] - arr[5][y];
	float s4 = arr[3][y] + arr[4][y];
	float m4 = arr[3][y] - arr[4][y];

	float ss1 = s2 + s3;
	float mm1 = s2 - s3;
	float ss2 = s1 + s4;
	float mm2 = s1 - s4;

	arr[0][y] = ( ss2 + ss1									) * K;
	arr[1][y] = ( _a1 * m1 + _b1 * m2 + _c1 * m3 + _d1 * m4	);
	arr[2][y] = ( _a2 * mm2 + b2 * mm1						);
	arr[3][y] = ( _b1 * m1 - _d1 * m2 - _a1 * m3 - _c1 * m4	);
	arr[4][y] = ( ss2 - ss1 								) * K;
	arr[5][y] = ( _c1 * m1 - _a1 * m2 + _d1 * m3 + _b1 * m4	);
	arr[6][y] = ( _b2 * mm2 - _a2 * mm1						);
	arr[7][y] = ( _d1 * m1 - _c1 * m2 + _b1 * m3 - _a1 * m4 );
}
void TransformColumn(float** arr, int x)
{
	float s1 = arr[x][0] + arr[x][7];
	float m1 = arr[x][0] - arr[x][7];
	float s2 = arr[x][1] + arr[x][6];
	float m2 = arr[x][1] - arr[x][6];
	float s3 = arr[x][2] + arr[x][5];
	float m3 = arr[x][2] - arr[x][5];
	float s4 = arr[x][3] + arr[x][4];
	float m4 = arr[x][3] - arr[x][4];

	float ss1 = s2 + s3;
	float mm1 = s2 - s3;
	float ss2 = s1 + s4;
	float mm2 = s1 - s4;

	arr[x][0] = ( ss2 + ss1									) * K;
	arr[x][1] = ( _a1 * m1 + _b1 * m2 + _c1 * m3 + _d1 * m4	);
	arr[x][2] = ( _a2 * mm2 + _b2 * mm1						);
	arr[x][3] = ( _b1 * m1 - _d1 * m2 - _a1 * m3 - _c1 * m4	);
	arr[x][4] = ( ss2 - ss1 								) * K;
	arr[x][5] = ( _c1 * m1 - _a1 * m2 + _d1 * m3 + _b1 * m4	);
	arr[x][6] = ( _b2 * mm2 - _a2 * mm1						);
	arr[x][7] = ( _d1 * m1 - _c1 * m2 + _b1 * m3 - _a1 * m4 );
}

void BackTransformRow(float** arr, int y)
{
	float s1 = a2 * arr[2][y];
	float m1 = a2 * arr[6][y] - arr[0][y] - arr[4][y];
	float s2 = b2 * arr[2][y];
	float m2 = b2 * arr[6][y] + arr[0][y] + arr[4][y];

	float ss1 = m2 + s1;
	float mm1 = m2 - s1;
	float ss2 =- s2 - m1;
	float mm2 =  s2 - m1;

	float k1 = a1 * arr[1][y] + b1 * arr[3][y] + c1 * arr[5][y] + d1 * arr[7][y];
	float k2 = b1 * arr[1][y] - d1 * arr[3][y] - a1 * arr[5][y] - c1 * arr[7][y];
	float k3 = c1 * arr[1][y] - a1 * arr[3][y] + d1 * arr[5][y] + b1 * arr[7][y];
	float k4 = d1 * arr[1][y] - c1 * arr[3][y] + b1 * arr[5][y] - a1 * arr[7][y];

	arr[0][y] = ss1 + k1; 
	arr[1][y] = mm2 + k2;
	arr[2][y] = ss2 + k3;
	arr[3][y] = mm1 + k4;  
	arr[4][y] = mm1 - k4;  
	arr[5][y] = ss2 - k3;  
	arr[6][y] = mm2 - k2; 
	arr[7][y] = ss1 - k1;  
}

void BackTransformColumn(float** arr, int x)
{
	float s1 = a2 * arr[x][2];
	float m1 = a2 * arr[x][6] - arr[x][0] - arr[x][4];
	float s2 = b2 * arr[x][2];
	float m2 = b2 * arr[x][6] + arr[x][0] + arr[x][4];

	float ss1 = m2 + s1;
	float mm1 = m2 - s1;
	float ss2 =- s2 - m1;
	float mm2 =  s2 - m1;

	float k1 = a1 * arr[x][1] + b1 * arr[x][3] + c1 * arr[x][5] + d1 * arr[x][7];
	float k2 = b1 * arr[x][1] - d1 * arr[x][3] - a1 * arr[x][5] - c1 * arr[x][7];
	float k3 = c1 * arr[x][1] - a1 * arr[x][3] + d1 * arr[x][5] + b1 * arr[x][7];
	float k4 = d1 * arr[x][1] - c1 * arr[x][3] + b1 * arr[x][5] - a1 * arr[x][7];

	arr[x][0] = ss1 + k1; 
	arr[x][1] = mm2 + k2;
	arr[x][2] = ss2 + k3;
	arr[x][3] = mm1 + k4;  
	arr[x][4] = mm1 - k4;  
	arr[x][5] = ss2 - k3;  
	arr[x][6] = mm2 - k2; 
	arr[x][7] = ss1 - k1;

	/* unoptimized version
	float t0 = arr[x][0] + a1 * arr[x][1] + a2 * arr[x][2] + b1 * arr[x][3] + arr[x][4] + c1 * arr[x][5] + b2 * arr[x][6] + d1 * arr[x][7];  
	float t1 = arr[x][0] + b1 * arr[x][1] + b2 * arr[x][2] - d1 * arr[x][3] - arr[x][4] - a1 * arr[x][5] - a2 * arr[x][6] - c1 * arr[x][7];
	float t2 = arr[x][0] + c1 * arr[x][1] - b2 * arr[x][2] - a1 * arr[x][3] - arr[x][4] + d1 * arr[x][5] + a2 * arr[x][6] + b1 * arr[x][7];
	float t3 = arr[x][0] + d1 * arr[x][1] - a2 * arr[x][2] - c1 * arr[x][3] + arr[x][4] + b1 * arr[x][5] - b2 * arr[x][6] - a1 * arr[x][7];  
	float t4 = arr[x][0] - d1 * arr[x][1] - a2 * arr[x][2] + c1 * arr[x][3] + arr[x][4] - b1 * arr[x][5] - b2 * arr[x][6] + a1 * arr[x][7];  
	float t5 = arr[x][0] - c1 * arr[x][1] - b2 * arr[x][2] + a1 * arr[x][3] - arr[x][4] - d1 * arr[x][5] + a2 * arr[x][6] - b1 * arr[x][7];  
	float t6 = arr[x][0] - b1 * arr[x][1] + b2 * arr[x][2] + d1 * arr[x][3] - arr[x][4] + a1 * arr[x][5] - a2 * arr[x][6] + c1 * arr[x][7]; 
	float t7 = arr[x][0] - a1 * arr[x][1] + a2 * arr[x][2] - b1 * arr[x][3] + arr[x][4] - c1 * arr[x][5] + b2 * arr[x][6] - d1 * arr[x][7];  

	arr[x][0] = t0;
	arr[x][1] = t1;
	arr[x][2] = t2;
	arr[x][3] = t3;
	arr[x][4] = t4;
	arr[x][5] = t5;
	arr[x][6] = t6;
	arr[x][7] = t7;
	*/
}


void dct(float** arr)
{
	for (int y = 0; y < 8; y++)
	{
		TransformRow(arr, y);
	}
	for (int x = 0; x < 8; x++)
	{
		TransformColumn(arr, x);
	}
}

void idct(float** arr)
{
	for (int x = 0; x < 8; x++)
	{
		BackTransformColumn(arr, x);
	}
	for (int y = 0; y < 8; y++)
	{
		BackTransformRow(arr, y);
	}

}





/*
void main()
{
	float xxx_d[64];
	float* xxx[8];
	for (int x=0; x < 8;x++)
	{
		xxx[x] = &xxx_d[x*8];
		for(int y=0; y< 8;y++)
		{
			xxx[x][y]=x*y;
		}
	}
	//std::cout << (GetTickCount()-start); 

	dct((float**) xxx);
	idct((float**) xxx);

};
*/
