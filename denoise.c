/***g++ -o denoise.so -std=c++11 -shared -fPIC denoise.c*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


//全部变量
int* src;
int* dst;
int r;
int c;
int d;
int v = 1;//把连通的区域改为这个值，这个值不断增加，一个连通区域对应一个值
std::vector<int> ps;//保存连通区域点集，供必要时操作

//根据一点向四周遍历连通区域（八连通），属于深度遍历；
//该函数执行一次代表计算出了一个连通区域，并把dst矩阵中对应的区域值改为v；
//执行过程中src和dst都会被修改，执行完，该连通区域对应的src全部为0，对应的dst为v；
//参数含义：
//src,原始矩阵；
//dst，保存的结果，会修改这个矩阵；
//r,c,d，矩阵shape；
//i,j,k，当前坐标；
//v,使用该值去更改dst中目标值
void pot_erg(int  i, int j, int k)
{
	int ind = k + j*d + i*c*d;
	if (src[ind] != 0)
	{
		ps.push_back(ind);
		src[ind] = 0;
		for (int ii = i - 1; ii <= i + 1; ii++)
			for (int jj = j - 1; jj <= j + 1; jj++)
				for (int kk = k - 1; kk <= k + 1; kk++)
					if (ii >= 0 && ii < r &&
						jj >= 0 && jj < c &&
						kk >= 0 && kk < d)
						pot_erg(ii, jj, kk);
	}
}

extern "C"
{
	//输入一张二值图像，和另外一张相同shape的零值矩阵作为输出；
	//输入的src不一定非得是二值图像，不一定非得是0和255，本算法判断的依据是0和非零；
	//输入的dst一定要跟src相同shape，一定要是个全为0的矩阵。
	//pixel_nb_th:去噪阈值，把面积小于该值的连通区域去掉，若不想去噪可把它设为0；
	void connectedComponents3D(int* src_, int* dst_, int r_, int c_, int d_, int pixel_nb_th = 0)
	{
		src = src_;
		dst = dst_;
		r = r_;
		c = c_;
		d = d_;

		int i, j, k, ind;
		for (i = 0; i < r; i++)
			for (j = 0; j < c; j++)
				for (k = 0; k < d; k++)
				{
					ind = k + j*d + i*c*d;
					if (src[ind] != 0)
					{
						ps.clear();
						pot_erg(i, j, k);
						if (ps.size() > pixel_nb_th)
						{
							for (auto it : ps)
								dst[it] = v;
							v++;
						}
					}
				}
		return;
	}


}