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

typedef struct
{

}numpy;	//��Python��numpy


// ����һ������
matrixStr* matMalloc(u32 line,u32 list)
{
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

//����һ�����󣬲�ȫ����ֵ��1
matrixStr* matrix_One(u32 line,u32 list)
{
	u32 L = 0;	//��
	u32 H = 0;	//��
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
	printf("����һ������:\r\n");
	matrixStr* mat1 = matMalloc(2,3);	// ����һ������
	matApendDat(mat1,a1);				// ���������ֵ
	PrintMat(mat1);						// ��ӡ����
	 
	printf("��ӡ�����ת��:\r\n");
	PrintMat(matrix_T(mat1));			// ��ӡ�����ת��

	printf("����һ������:\r\n");
	matrixStr* mat2 = matMalloc(3,2);
	matApendDat(mat2,a2);
	PrintMat(mat2);

	printf("��ӡ�����ת��:\r\n");
	PrintMat(matrix_T(mat2));

	printf("�������������:\r\n");
	matrixStr* mat3 = matDot(mat1,mat2);// �������������
	PrintMat(mat3);

	printf("����һ�����󣬲�ȫ����ֵΪ1:\r\n");
	matrixStr* mat4 = matrix_One(6,1);	// ����һ�����󣬲�ȫ����ֵΪ1
	PrintMat(mat4);


	return 0;
}