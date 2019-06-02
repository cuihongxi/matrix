#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define	ERROR_MAT_LOG	printf
#define	MAT_LOG	printf
typedef	 unsigned char u8;
typedef	 unsigned  int u32;
typedef	 float  matDAT;

typedef struct
{
	u32 line;	//行数
	u32 list;	//列数
}matrixStr;

typedef struct
{

}numpy;	//仿Python的numpy


// 申请一个矩阵
matrixStr* matMalloc(u32 line,u32 list)
{
	matrixStr* mat = (matrixStr*)malloc((line*list+2)*(sizeof(matDAT)/sizeof(u8)));
	mat->line = line;
	mat->list = list;
	return mat;

}

//添加数据
void matApendDat(matrixStr* mat , matDAT* dat)
{
	u32 L = 0;	//行
	u32 H = 0;	//列

	for(L = 0; L < mat->line; L ++)
	{
		for(H = 0; H < mat->list; H ++)
		{
			*(matDAT*)(&mat->list + 1 + L*mat->list + H) = *dat;
			dat ++;
		}
		
	}
}



//打印一个矩阵
void PrintMat(matrixStr* mat)
{
	u32 L = 0;	//行
	u32 H = 0;	//列

	for(L = 0; L < mat->line; L ++)
	{
		for(H = 0; H < mat->list; H ++)
		{
			MAT_LOG("%f	",*((matDAT*)(&mat->list+ 1 + L*mat->list + H)));
		}
		MAT_LOG("\r\n");
	}
}

//获取一个矩阵的元素
matDAT Get_Mat(matrixStr* mat,u32 line,u32 list)
{
	if(line < mat->line && list < mat->list )
		return *(matDAT*)(&mat->list + 1 + line*mat->list + list);
	else return -1;
}

//获取矩阵元素单元的地址
matDAT* Get_MatAddr(matrixStr* mat,u32 line,u32 list)
{
	if(line < mat->line && list < mat->list )
		return (matDAT*)(&mat->list + 1 + line*mat->list + list);
	else return (matDAT*)-1;
}

//矩阵相加
matrixStr* matAdd(matrixStr* a,matrixStr* b)
{

	if(a->line == b->line && a->list == b->list)
	{
		u32 L = 0;
		u32 H = 0;
		matrixStr* mat = matMalloc(a->line,a->list);
		mat->line = a->line;
		mat->list = a->list;
		for(L = 0; L < a->line; L ++)
		{
			for(H = 0; H < a->list; H ++)
			{
				*(matDAT*)(&mat->list + 1 + L*mat->list + H) = Get_Mat(a,L,H) +  Get_Mat(b,L,H);
			}		
		}
		return mat;
	}else ERROR_MAT_LOG("错误：两个矩阵行列数不相等!\r\n");

	return (matrixStr*)-1;
}

//相乘，相加 ,a的第几行跟b的第几列相乘相加
// 返回相乘的结果
matDAT GetMat_MultAdd(matrixStr* a,matrixStr* b,u32 aline,u32 blist)
{
	u32 i = 0;
	matDAT dat = 0;
	for(i=0;i<a->list;i++)
	{	
		dat += (matDAT)Get_Mat(a,aline,i) *  (matDAT)Get_Mat(b,i,blist);
	//	printf("dat = %f\r\n",dat);
	}
	//printf("->dat = %f\r\n",dat);
	return dat;
}

//矩阵相乘
//返回一个乘积结果矩阵
matrixStr* matDot(matrixStr* a,matrixStr* b)
{

	if( a->list == b->line)
	{
		u32 L = 0;
		u32 H = 0;
		matrixStr* mat = matMalloc(a->line,b->list);
		for(L = 0; L < a->line; L ++)
		{
			for(H = 0; H < b->list; H ++)
			{
				*Get_MatAddr(mat,L,H) = GetMat_MultAdd(a,b,L,H);
			}		
		}
		return mat;
	}else ERROR_MAT_LOG("错误：A的列数与B的行数不相等，不能做乘法\r\n");
	return (matrixStr*)-1;
}

//返回矩阵的转置
//注意，如果不再需要需要释放内存
matrixStr* matrix_T(matrixStr* mat)
{
	u32 L = 0;	//行
	u32 H = 0;	//列
	matrixStr* mat_t = matMalloc(mat->list,mat->line);

	for(L = 0; L < mat_t->line; L ++)
	{
		for(H = 0; H < mat_t->list; H ++)
		{
			*Get_MatAddr(mat_t,L,H) = Get_Mat(mat,H,L);
		}
	}
	return mat_t;

}

//生成一个矩阵，并全部赋值成1
matrixStr* matrix_One(u32 line,u32 list)
{
	u32 L = 0;	//行
	u32 H = 0;	//列
	matrixStr* mat = matMalloc(line,list);
	for(L = 0; L < mat->line; L ++)
	{
		for(H = 0; H < mat->list; H ++)
		{
			*Get_MatAddr(mat,L,H) = 1;
		}
	}
	return mat;
}

// 测试代码

int main()
{
	matDAT a1[] = {
			3,1,2,
			-5,4,1,
	};
	matDAT a2[] = {
			0,5,
			3,2,
			10,0.5f
	};
	printf("申请一个矩阵:\r\n");
	matrixStr* mat1 = matMalloc(2,3);	// 申请一个矩阵
	matApendDat(mat1,a1);				// 给这个矩阵赋值
	PrintMat(mat1);						// 打印矩阵
	 
	printf("打印矩阵的转置:\r\n");
	PrintMat(matrix_T(mat1));			// 打印矩阵的转置

	printf("申请一个矩阵:\r\n");
	matrixStr* mat2 = matMalloc(3,2);
	matApendDat(mat2,a2);
	PrintMat(mat2);

	printf("打印矩阵的转置:\r\n");
	PrintMat(matrix_T(mat2));

	printf("将两个矩阵相乘:\r\n");
	matrixStr* mat3 = matDot(mat1,mat2);// 将两个矩阵相乘
	PrintMat(mat3);

	printf("申请一个矩阵，并全部赋值为1:\r\n");
	matrixStr* mat4 = matrix_One(6,1);	// 申请一个矩阵，并全部赋值为1
	PrintMat(mat4);


	return 0;
}