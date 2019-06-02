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
	u32 line;	//����
	u32 list;	//����
}matrixStr;

// ����һ������
matrixStr* matMalloc(u32 line,u32 list)
{
	//��̬����һƬ�ڴ棬��������������������������
	matrixStr* mat = (matrixStr*)malloc((line*list+2)*(sizeof(matDAT)/sizeof(u8)));
	mat->line = line;
	mat->list = list;
	return mat;

}

//�������
void matApendDat(matrixStr* mat , matDAT* dat)
{
	u32 L = 0;	//��
	u32 H = 0;	//��

	for(L = 0; L < mat->line; L ++)
	{
		for(H = 0; H < mat->list; H ++)
		{
			*(matDAT*)(&mat->list + 1 + L*mat->list + H) = *dat;
			dat ++;
		}
		
	}
}



//��ӡһ������
void PrintMat(matrixStr* mat)
{
	u32 L = 0;	//��
	u32 H = 0;	//��

	for(L = 0; L < mat->line; L ++)
	{
		for(H = 0; H < mat->list; H ++)
		{
			MAT_LOG("%f	",*((matDAT*)(&mat->list+ 1 + L*mat->list + H)));
		}
		MAT_LOG("\r\n");
	}
}

//��ȡһ�������Ԫ��
matDAT Get_Mat(matrixStr* mat,u32 line,u32 list)
{
	if(line < mat->line && list < mat->list )
		return *(matDAT*)(&mat->list + 1 + line*mat->list + list);
	else return -1;
}

//��ȡ����Ԫ�ص�Ԫ�ĵ�ַ
matDAT* Get_MatAddr(matrixStr* mat,u32 line,u32 list)
{
	if(line < mat->line && list < mat->list )
		return (matDAT*)(&mat->list + 1 + line*mat->list + list);
	else return (matDAT*)-1;
}

//�������
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
	}else ERROR_MAT_LOG("�����������������������!\r\n");

	return (matrixStr*)-1;
}

//��ˣ���� ,a�ĵڼ��и�b�ĵڼ���������
// ������˵Ľ��
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

//�������
//����һ���˻��������
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
	}else ERROR_MAT_LOG("����A��������B����������ȣ��������˷�\r\n");
	return (matrixStr*)-1;
}

//���ؾ����ת��
//ע�⣬���������Ҫ��Ҫ�ͷ��ڴ�
matrixStr* matrix_T(matrixStr* mat)
{
	u32 L = 0;	//��
	u32 H = 0;	//��
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

//����һ�����󣬲�ȫ����ֵ��num
matrixStr* matrix_Num(u32 line,u32 list,matDAT num)
{
	u32 L = 0;	//��
	u32 H = 0;	//��
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


//����һ�����󣬲�ȫ����ֵ��1
matrixStr* matrix_One(u32 line,u32 list)
{
	return matrix_Num(line, list,1);
}

//����һ�����󣬲�ȫ����ֵ��0
matrixStr* matrix_Zero(u32 line,u32 list)
{
	return matrix_Num(line, list,0);
}

// ��mat����,��ӵ�source�����У�axis = 1��ӵ��ұߣ�axis = 0 ��ӵ��±�
matrixStr* matrix_Append(matrixStr* source,matrixStr* bmat,u8 axis)
{
	matrixStr* mat = 0 ;
	u32 L = 0;	//��
	u32 H = 0;	//��
	if(axis == 0)	//��ӵ��±�
	{
		if(source->list != bmat->list)
			ERROR_MAT_LOG("����axis=0������������ͬ\r\n");
		else
		{
			mat = matMalloc(source->line + bmat->line,source->list);
			for(L = 0; L < source->line; L ++)			//����Դ����
			{
				for(H = 0; H < mat->list; H ++)
				{
					*Get_MatAddr(mat,L,H) = Get_Mat(source,L,H);
				}
			}

			for(L = source->line; L < mat->line; L ++)	//������ӵľ���
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
			ERROR_MAT_LOG("����axis=1������������ͬ\r\n");
		else
		{
			mat = matMalloc(source->line,source->list + bmat->list);
			for(H = 0; H < source->list; H ++)			//����Դ����
			{
				for(L = 0; L < mat->line; L ++)
				{
					*Get_MatAddr(mat,L,H) = Get_Mat(source,L,H);
				}
			}

			for(H = source->list; H < mat->list; H ++)	//������ӵľ���
			{
				for(L = 0; L < mat->line; L ++)
				{
					*Get_MatAddr(mat,L,H) = Get_Mat(bmat,L,H - source->list);
				}
			}		
		}

	}
	else ERROR_MAT_LOG("����axis ������0����1��0��ӵ��±ߣ�1��ӵ��ұ�\r\n");
	return mat;
}

//�Ծ�������ķ�����������ṹ����
typedef struct
{
	matrixStr* (*array)(u32 line,u32 list);			// ����һ������ע��������Ҫ�ͷ��ڴ�
	void (*apendData)(matrixStr* mat , matDAT* dat);// �������������	
	void (*printMat)(matrixStr* );					// ��ӡ����
	matrixStr* (*T)(matrixStr* );					// ת�ã�ע��������Ҫ�ͷ��ڴ�
	matrixStr* (*ones)(u32 line,u32 list);			// ����һ�����󣬲�ȫ����ֵ��1
	matrixStr* (*zeros)(u32 line,u32 list);			// ����һ�����󣬲�ȫ����ֵ��0
	matrixStr* (*dot)(matrixStr* a,matrixStr* b);	// ����˷�
	matrixStr* (*append)(matrixStr* source,matrixStr* bmat,u8 axis); // ��mat����,��ӵ�source�����У�axis = 1��ӵ��ұߣ�axis = 0 ��ӵ��±�
}numpy;	//��Python��numpy


//��ʼ��numpy����
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
// ���Դ���

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
	numpy_Init(&np);	//��ʼ��np
	
	printf("����һ������:\r\n");
	matrixStr* mat1 = np.array(2,3);		// ����һ������
	np.apendData(mat1,a1);					// ���������ֵ
	np.printMat(mat1);						// ��ӡ����
	 
	printf("��ӡ�����ת��:\r\n");
	np.printMat(np.T(mat1));				// ��ӡ�����ת��

	printf("����һ������:\r\n");
	matrixStr* mat2 = np.array(3,2);
	np.apendData(mat2,a2);
	np.printMat(mat2);

	printf("��ӡ�����ת��:\r\n");
	np.printMat(np.T(mat2));

	printf("�������������:\r\n");
	matrixStr* mat3 = np.dot(mat1,mat2);	// �������������
	np.printMat(mat3);

	printf("����һ������mat4����ȫ����ֵΪ1:\r\n");
	matrixStr* mat4 = np.ones(3,1);	// ����һ�����󣬲�ȫ����ֵΪ1
	np.printMat(mat4);

	printf("����һ������mat5����ȫ����ֵΪ0:\r\n");
	matrixStr* mat5 = np.zeros(1,3);	// ����һ�����󣬲�ȫ����ֵΪ1
	np.printMat(mat5);

	printf("����һ������mat6����ȫ����ֵΪ1:\r\n");
	matrixStr* mat6 = np.ones(1,3);	// ����һ�����󣬲�ȫ����ֵΪ1
	np.printMat(mat6);

	printf("����һ������mat7����ȫ����ֵΪ0:\r\n");
	matrixStr* mat7 = np.zeros(3,1);	// ����һ�����󣬲�ȫ����ֵΪ1
	np.printMat(mat7);

	printf("��mat6��ӵ�mat5�±�:\r\n");
	matrixStr* mat8 = np.append(mat5,mat6,0);	// ����һ�����󣬲�ȫ����ֵΪ1
	np.printMat(mat8);

	printf("��mat7��ӵ�mat4�ұ�:\r\n");
	matrixStr* mat9 = np.append(mat4,mat7,1);	// ����һ�����󣬲�ȫ����ֵΪ1
	np.printMat(mat9);

	return 0;
}