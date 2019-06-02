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

// 申请一个矩阵
matrixStr* matMalloc(u32 line,u32 list)
{
	//动态申请一片内存，并多申请两个用来保存行列数
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

//生成一个矩阵，并全部赋值成num
matrixStr* matrix_Num(u32 line,u32 list,matDAT num)
{
	u32 L = 0;	//行
	u32 H = 0;	//列
	matrixStr* mat = matMalloc(line,list);
	for(L = 0; L < mat->line; L ++)
	{
		for(H = 0; H < mat->list; H ++)
		{
			*Get_MatAddr(mat,L,H) = num;
		}
	}
	return mat;
}


//生成一个矩阵，并全部赋值成1
matrixStr* matrix_One(u32 line,u32 list)
{
	return matrix_Num(line, list,1);
}

//生成一个矩阵，并全部赋值成0
matrixStr* matrix_Zero(u32 line,u32 list)
{
	return matrix_Num(line, list,0);
}

// 将mat矩阵,添加到source矩阵中，axis = 1添加到右边，axis = 0 添加到下边
matrixStr* matrix_Append(matrixStr* source,matrixStr* bmat,u8 axis)
{
	matrixStr* mat = 0 ;
	u32 L = 0;	//行
	u32 H = 0;	//列
	if(axis == 0)	//添加到下边
	{
		if(source->list != bmat->list)
			ERROR_MAT_LOG("错误：axis=0，列数必须相同\r\n");
		else
		{
			mat = matMalloc(source->line + bmat->line,source->list);
			for(L = 0; L < source->line; L ++)			//拷贝源矩阵
			{
				for(H = 0; H < mat->list; H ++)
				{
					*Get_MatAddr(mat,L,H) = Get_Mat(source,L,H);
				}
			}

			for(L = source->line; L < mat->line; L ++)	//拷贝添加的矩阵
			{
				for(H = 0; H < mat->list; H ++)
				{
					*Get_MatAddr(mat,L,H) = Get_Mat(bmat,L - source->line ,H);
				}
			}
		}
	}
	else
	if(axis == 1)
	{
		if(source->line != bmat->line)
			ERROR_MAT_LOG("错误：axis=1，行数必须相同\r\n");
		else
		{
			mat = matMalloc(source->line,source->list + bmat->list);
			for(H = 0; H < source->list; H ++)			//拷贝源矩阵
			{
				for(L = 0; L < mat->line; L ++)
				{
					*Get_MatAddr(mat,L,H) = Get_Mat(source,L,H);
				}
			}

			for(H = source->list; H < mat->list; H ++)	//拷贝添加的矩阵
			{
				for(L = 0; L < mat->line; L ++)
				{
					*Get_MatAddr(mat,L,H) = Get_Mat(bmat,L,H - source->list);
				}
			}		
		}

	}
	else ERROR_MAT_LOG("错误：axis 必须是0或者1。0添加到下边，1添加到右边\r\n");
	return mat;
}

//对矩阵操作的方法都在这个结构体中
typedef struct
{
	matrixStr* (*array)(u32 line,u32 list);			// 申请一个矩阵，注：不用需要释放内存
	void (*apendData)(matrixStr* mat , matDAT* dat);// 给矩阵添加数据	
	void (*printMat)(matrixStr* );					// 打印函数
	matrixStr* (*T)(matrixStr* );					// 转置，注：不用需要释放内存
	matrixStr* (*ones)(u32 line,u32 list);			// 生成一个矩阵，并全部赋值成1
	matrixStr* (*zeros)(u32 line,u32 list);			// 生成一个矩阵，并全部赋值成0
	matrixStr* (*dot)(matrixStr* a,matrixStr* b);	// 矩阵乘法
	matrixStr* (*append)(matrixStr* source,matrixStr* bmat,u8 axis); // 将mat矩阵,添加到source矩阵中，axis = 1添加到右边，axis = 0 添加到下边
}numpy;	//仿Python的numpy


//初始化numpy对象
void numpy_Init(numpy* np)
{
	np->array = matMalloc;
	np->apendData = matApendDat;
	np->printMat = PrintMat;
	np->T = matrix_T;
	np->ones = matrix_One;
	np->zeros = matrix_Zero;
	np->dot = matDot;
	np->append = matrix_Append;
	
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

	numpy np;
	numpy_Init(&np);	//初始化np
	
	printf("申请一个矩阵:\r\n");
	matrixStr* mat1 = np.array(2,3);		// 申请一个矩阵
	np.apendData(mat1,a1);					// 给这个矩阵赋值
	np.printMat(mat1);						// 打印矩阵
	 
	printf("打印矩阵的转置:\r\n");
	np.printMat(np.T(mat1));				// 打印矩阵的转置

	printf("申请一个矩阵:\r\n");
	matrixStr* mat2 = np.array(3,2);
	np.apendData(mat2,a2);
	np.printMat(mat2);

	printf("打印矩阵的转置:\r\n");
	np.printMat(np.T(mat2));

	printf("将两个矩阵相乘:\r\n");
	matrixStr* mat3 = np.dot(mat1,mat2);	// 将两个矩阵相乘
	np.printMat(mat3);

	printf("申请一个矩阵mat4，并全部赋值为1:\r\n");
	matrixStr* mat4 = np.ones(3,1);	// 申请一个矩阵，并全部赋值为1
	np.printMat(mat4);

	printf("申请一个矩阵mat5，并全部赋值为0:\r\n");
	matrixStr* mat5 = np.zeros(1,3);	// 申请一个矩阵，并全部赋值为1
	np.printMat(mat5);

	printf("申请一个矩阵mat6，并全部赋值为1:\r\n");
	matrixStr* mat6 = np.ones(1,3);	// 申请一个矩阵，并全部赋值为1
	np.printMat(mat6);

	printf("申请一个矩阵mat7，并全部赋值为0:\r\n");
	matrixStr* mat7 = np.zeros(3,1);	// 申请一个矩阵，并全部赋值为1
	np.printMat(mat7);

	printf("将mat6添加到mat5下边:\r\n");
	matrixStr* mat8 = np.append(mat5,mat6,0);	// 申请一个矩阵，并全部赋值为1
	np.printMat(mat8);

	printf("将mat7添加到mat4右边:\r\n");
	matrixStr* mat9 = np.append(mat4,mat7,1);	// 申请一个矩阵，并全部赋值为1
	np.printMat(mat9);

	return 0;
}