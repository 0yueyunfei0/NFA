#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <algorithm>
#include <sstream>
using namespace std;


#define P_Amount 20
#define RMAX 20
#define CMAX 20
#define NT_AMOUNT 20
#define T_AMOUNT 20
//�ķ��ṹ��
class G
{
public:
	string T;  //�ս���ļ���
	string NT;  //���ս���ļ���
	string begin;  //�ķ���ʼ����
	string P[P_Amount];  //����ʽ�ļ���
	int P_Length;  //����ʽ�ĸ���
	bool FIRSTVT[RMAX][CMAX];  //���ս����FIRSTVT����
	bool LASTVT[RMAX][CMAX];  //���ս����FIRSTVT����
	int PRIORITY_TABLE[NT_AMOUNT][NT_AMOUNT];  //���ȱ�����Ԫ��ֻ��4�����͵�ֵ��-1������<�У�1������>�У�0������=�У�2�������
	int FR;  //��¼F����Ч����
	int FC;  //��¼F����Ч����
	int f[T_AMOUNT];  //����ջ�ڵ����ȼ�,f���Ⱥ���
	int g[T_AMOUNT];  //����ջ������ȼ���g���Ⱥ���
	G()
	{
		//��ʼ��
		T = "";
		NT = "";
		begin = "";
		P_Length = 0;
		for (int i = 0; i < P_Amount; i++)
			P[i] = "";
		for (int i = 0; i < RMAX; i++)
		{
			for (int j = 0; j < CMAX; j++)
			{
				FIRSTVT[i][j] = false;
			}
		}
		for (int i = 0; i < RMAX; i++)
		{
			for (int j = 0; j < CMAX; j++)
			{
				LASTVT[i][j] = false;
			}
		}
		for (int i = 0; i < NT_AMOUNT; i++)
		{
			for (int j = 0; j < NT_AMOUNT; j++)
			{
				PRIORITY_TABLE[i][j] = 2;  //Ĭ��Ϊ����״̬
			}
		}
		//��ʼ״̬��f��g�ڵ�Ԫ�ض���Ϊ1
		for (int i = 0; i < T_AMOUNT; i++)
		{
			f[i] = 1; 
			g[i] = 1;
		}
	}
	~G()
	{
		
	}
};

class Temp
{
public:
	char NT;
	char T;
};

void findor(string &str, char c, int *pos,int cap,int &len)  //��str��Ѱ�ҳ�����|��λ��,pos��¼��λ��,capΪλ�������������lenΪpos����Ч����
{
	for (int i = 0; i < cap; i++)
	{
		//��ʼ��
		pos[i] = 0;
	}
	int temp=0;
	for (int i = 0; i < str.length(); i++)
	{
		int j=str.find(c, temp);
		if (j == -1)
		{
			break;
		}
		pos[i] = j;
		len++;
		temp = j+1;
	}
}
//���ݲ���ʽ�õ��ս����������ս������
void getSyms(string *strs, int len, string &T,string &NT) //strsΪ����ʽ���ϣ�lenΪ����ʽ����
{
	string strTemp="";  
	for (int i = 0; i < len; i++)
	{
		if (strTemp.find(strs[i][0]) != -1)  //�����ҵ��Ѵ��ڵķ��ţ�����
			continue;
		strTemp += strs[i][0];
	}
	NT = strTemp;
	for (int i = 0; i < len; i++)
	{
		for (int j = 3; j < strs[i].length(); j++)
		{
			if (strs[i][j] >= 'A' && strs[i][j] <= 'Z')  //���ս��
			{
				if (NT.find(strs[i][j]) != 0)  //�÷����Ѵ���,����
					continue;
				NT += strs[i][j];
			}
			else  //�ս��,�����������i
			{
				if (T.find(strs[i][j]) != -1)  //�÷����Ѵ��ڣ�����
					continue;
				T += strs[i][j];
			}
		}
	}
}

void zero(string *strs, int &len) //strsΪ������ķ����ʽ��lenΪ���ʽ�ĸ���
{
	string StrTemp[P_Amount];
	int lens,count=0;  //count��¼�����ɱ��ʽ�ĸ���,lens��¼ÿ��
	int pos[10];  
	for (int i = 0; i < len; i++)  //�����в���ʽ��|���������ɶ������ʽ���洢��StrTemp��
	{
		lens = 0;
		findor(strs[i], '|', pos, 10, lens);  //Ѱ���������ʽ�����С�|����λ�ã���ŵ�pos�У�10Ϊ���������,lensΪ��|������

		for (int z = 0; z < lens+1; z++)  //��һ������ʽ��|���������ɶ������ʽ���洢��StrTemp�У�(len+1)����Ҫ���ɵı��ʽ����
		{
			if (z == 0)
			{
				StrTemp[z + count] = strs[i].substr(0, 3) + strs[i].substr(3, (pos[z]-2 - 1));
				continue;
			}

			StrTemp[z + count] = strs[i].substr(0, 3) + strs[i].substr(pos[z - 1]+1, (pos[z] - pos[z - 1] - 1));

			if (z == lens)
			{
				StrTemp[count + lens] = strs[i].substr(0, 3) + strs[i].substr(pos[lens - 1]+1, (strs[i].length() - pos[lens - 1] - 1));
			}
		}

		count += lens + 1;
	}

	//��StrTemp����strs
	len = count;
	for (int i = 0; i < count; i++)
	{
		strs[i] = StrTemp[i];
	}
}
void test1(G &gm);  //����
void first(G &grammer,string strs[],int len)  //grammerΪ��Ҫ������ķ�����strsΪ����zero����ķ����ʽ��lenΪ���ʽ�ĸ���
{
	grammer.begin = strs[0][0];  //��ʼ����
	grammer.P_Length = len;  //����ʽ�ĸ���
	for (int i = 0; i < len; i++)  //�õ�����ʽ
	{
		grammer.P[i] = strs[i];
	}
	getSyms(strs, len, grammer.T, grammer.NT);  //�Ӳ���ʽ�еõ����ս���������ս������

}

void test1(G &gm)  //��⺯��,�������ķ����ݽṹ����ȷ��
{
	cout << endl << "��⣺" << endl;
	cout << "�ķ��Ŀ�ʼ���ţ�" << gm.begin << endl;
	cout << "�ķ��ķ��ս�����ϣ�" << gm.NT << endl;
	cout << "�ķ����ս�����ϣ�" << gm.T << endl;
	cout << "�ķ��Ĳ���ʽ��" << endl;
	for (int i = 0; i < gm.P_Length; i++)
	{
		cout << gm.P[i] << endl;
	}
	cout << endl;
}

void insert(G &gm, stack<Temp> &stck, char A, char a)  //
{
	int iA = gm.NT.find(A);
	int ia = gm.T.find(a);
	Temp tmp;
	tmp.NT = A;
	tmp.T = a;
	
	if (!gm.FIRSTVT[iA][ia])
	{
		gm.FIRSTVT[iA][ia] = true;
		stck.push(tmp);
	}
}

void insert1(G &gm, stack<Temp> &stck, char A, char a)  //
{
	int iA = gm.NT.find(A);
	int ia = gm.T.find(a);
	Temp tmp;
	tmp.NT = A;
	tmp.T = a;

	if (!gm.LASTVT[iA][ia])
	{
		gm.LASTVT[iA][ia] = true;
		stck.push(tmp);
	}
}

void test2(G &gm);  //����gm��FIRSTVT��LASTVT����

void second(G &gm)  //gmΪ�ķ�����Ԫ��
{
	gm.FR = gm.NT.length();
	gm.FC = gm.T.length();
	stack<Temp> stck;  
	Temp tmpBa;

	//�õ�G��FIRSTVT����
	for (int i = 0; i < gm.P_Length; i++)
	{
		if (gm.P[i].length() - 4 == 0)  //����ʽ�ұ�ֻ��һ������
		{
			if (gm.P[i][3] < 'A' || gm.P[i][3]>'Z')  //����������ս��
			{
				insert(gm, stck, gm.P[i][0], gm.P[i][3]);
			}
		}
		else if (gm.P[i][3] >= 'A' && gm.P[i][3] <= 'Z')  //�Ҳ��ĵ�һ������Ϊ���ս������ô�����Ҳ��ĵڶ�������
		{
			insert(gm, stck, gm.P[i][0], gm.P[i][4]);
		}
		else  //�Ҳ��ĵ�һ������Ϊ�ս��
		{
			insert(gm, stck, gm.P[i][0], gm.P[i][3]);
		}
	}

	while (!stck.empty())
	{
		tmpBa = stck.top();
		stck.pop();
		for (int i = 0; i < gm.P_Length; i++)
		{
			if (gm.P[i][0] != tmpBa.NT&&gm.P[i][3] == tmpBa.NT)  //�ҵ�����A->B�������Ĳ���ʽ��FIRSTVT(B)��ȻFIRSTVT(A)
			{
				insert(gm, stck, gm.P[i][0], tmpBa.T);
			}
		}
	}

	//�õ�G��LASTVT����
	for (int i = 0; i < gm.P_Length; i++)
	{
		if (gm.P[i].length() - 4 == 0)  //����ʽ�ұ�ֻ��һ������
		{
			if (gm.P[i][3] < 'A' || gm.P[i][3]>'Z')  //����������ս��
			{
				insert1(gm, stck, gm.P[i][0], gm.P[i][3]);
			}
		}
		else if (gm.P[i][3] >= 'A' && gm.P[i][3] <= 'Z')  //�Ҳ������һ������Ϊ���ս������ô�����Ҳ��ĵ����ڶ�������
		{
			insert1(gm, stck, gm.P[i][0], gm.P[i][gm.P[i].length()-2]);
		}
		else  //�Ҳ������һ������Ϊ�ս��
		{
			insert1(gm, stck, gm.P[i][0], gm.P[i][gm.P[i].length()-1]);
		}
	}

	while (!stck.empty())
	{
		tmpBa = stck.top();
		stck.pop();
		for (int i = 0; i < gm.P_Length; i++)
		{
			if (gm.P[i][0] != tmpBa.NT&&gm.P[i][gm.P[i].length()-1] == tmpBa.NT)  //�ҵ�����A->������B�Ĳ���ʽ��LASTVT(B)��ȻLASTVT(A)
			{
				insert1(gm, stck, gm.P[i][0], tmpBa.T);
			}
		}
	}

}

void test2(G &gm)
{
	cout << "G��FIRSTVT���ϣ�" << endl;
	cout << "\t";
	for (int i = 0; i < gm.T.length(); i++)
	{
		if (i == gm.T.length() - 1)
		{
			cout << gm.T[i] << endl;
			break;
		}
		cout << gm.T[i] << "\t";
	}

	for (int i = 0; i < gm.FR; i++)
	{
		cout << gm.NT[i] << "\t";
		for (int j = 0; j < gm.FC; j++)
		{
			if (j == gm.FC - 1)
			{
				cout << gm.FIRSTVT[i][j] << endl;
				break;
			}
			cout << gm.FIRSTVT[i][j] << "\t";
		}
	}

	cout << "G��LASTVT���ϣ�" << endl;
	cout << "\t";
	for (int i = 0; i < gm.T.length(); i++)
	{
		if (i == gm.T.length() - 1)
		{
			cout << gm.T[i] << endl;
			break;
		}
		cout << gm.T[i] << "\t";
	}
	for (int i = 0; i < gm.FR; i++)
	{
		cout << gm.NT[i] << "\t";
		for (int j = 0; j < gm.FC; j++)
		{
			if (j == gm.FC - 1)
			{
				cout << gm.LASTVT[i][j] << endl;
				break;
			}
			cout << gm.LASTVT[i][j] << "\t";
		}
	}
	cout << endl;
}

void test3(G &gm);  //����third����

void third(G &gm)  //gmΪ�ķ������ݽṹ
{
	for (int i = 0; i < gm.P_Length; i++)
	{
		int lens = gm.P[i].length();
		for (int j = 3; j <= lens-2; j++)
		{
			if ((gm.P[i][j]<'A'||gm.P[i][j]>'Z')&&(gm.P[i][j + 1]<'A'||gm.P[i][j + 1]>'Z'))  //Xi��X(i+1)��Ϊ�ս��
			{
				int temp = gm.T.find(gm.P[i][j]);
				int temp1 = gm.T.find(gm.P[i][j + 1]);
				gm.PRIORITY_TABLE[temp][temp1] = 0;  //��Ϊͬ�����ȼ�
			}
			if (j <= (lens - 3) && (gm.P[i][j]<'A' || gm.P[i][j]>'Z') && (gm.P[i][j + 2]<'A' || gm.P[i][j + 2]>'Z') && (gm.P[i][j + 1] >= 'A'&&gm.P[i][j + 1] <= 'Z'))  //Xi��X(i+2)Ϊ�ս������X(i+1)Ϊ���ս��
			{
				int temp = gm.T.find(gm.P[i][j]);
				int temp1 = gm.T.find(gm.P[i][j + 2]);
				gm.PRIORITY_TABLE[temp][temp1] = 0;  //��Ϊͬ�����ȼ�
			}
			if ((gm.P[i][j]<'A' || gm.P[i][j]>'Z') && (gm.P[i][j + 1] >= 'A' && gm.P[i][j + 1] <= 'Z'))  //XiΪ�ս����X(i+1)Ϊ���ս��
			{
				for (int z = 0; z < gm.T.length(); z++)
				{
					if (gm.FIRSTVT[gm.NT.find(gm.P[i][j + 1])][z])
					{
						int temp = gm.T.find(gm.P[i][j]);
						int temp1 = z;
						gm.PRIORITY_TABLE[temp][temp1] = -1;  //��������������
					}
				}
			}
			if ((gm.P[i][j+1]<'A' || gm.P[i][j+1]>'Z') && (gm.P[i][j] >= 'A' && gm.P[i][j] <= 'Z'))  //X(i+1)Ϊ�ս����XiΪ���ս��
			{
				for (int z = 0; z < gm.T.length(); z++)
				{
					if (gm.LASTVT[gm.NT.find(gm.P[i][j])][z])
					{
						int temp = z;
						int temp1 = gm.T.find(gm.P[i][j+1]);
						gm.PRIORITY_TABLE[temp][temp1] = 1;  //��������������
					}
				}
			}
		}
	}
}

void test3(G &gm)
{
	cout << "G�����ȷ�����" << endl;
	cout << "\t";
	for (int i = 0; i < gm.T.length(); i++)
	{
		if (i == gm.T.length() - 1)
		{
			cout << gm.T[i] << endl;
			break;
		}
		cout << gm.T[i] << "\t";
	}
	int lens = gm.T.length();
	for (int i = 0; i < lens; i++)
	{
		cout << gm.T[i] << "\t";
		for (int j = 0; j < lens; j++)
		{
			if (j == gm.T.length() - 1)
			{
				cout << gm.PRIORITY_TABLE[i][j] << endl;
				break;
			}
			cout << gm.PRIORITY_TABLE[i][j] << "\t";
		}
	}
}

void test4(G &gm);  //������Ⱥ���f��g

void four(G &gm)  //�������Ⱥ���f��g
{
	bool isChanged = true;  //isChanged������һ�ε���f��g������û�з����仯����Ϊtrue
	int lens = gm.T.length();
	while (isChanged)
	{
		isChanged = false;
		for (int i = 0; i < lens; i++)
		{
			for (int j = 0; j < lens; j++)
			{
				if (gm.PRIORITY_TABLE[i][j] == 2)
					continue;
				if (gm.PRIORITY_TABLE[i][j] == 1 && (gm.f[i] <= gm.g[j]))
				{
					gm.f[i] = gm.g[j] + 1;
					isChanged = true;
					continue;
				}
				if (gm.PRIORITY_TABLE[i][j] == -1 && (gm.f[i] >= gm.g[j]))
				{
					gm.g[j] = gm.f[i] + 1;
					isChanged = true;
					continue;
				}
				if (gm.PRIORITY_TABLE[i][j] == 0 && (gm.f[i] != gm.g[j]))
				{
					if (gm.f[i] > gm.g[j])
					{
						gm.g[j] = gm.f[i];
					}
					else
					{
						gm.f[i] = gm.g[j];
					}
					isChanged = true;
					continue;
				}
			}
		}
	}
}

void test4(G &gm)  //���four����
{
	cout << "\t";
	for (int i = 0; i < gm.T.length(); i++)
	{
		if (i == gm.T.length() - 1)
		{
			cout << gm.T[i] << endl;
			break;
		}
		cout << gm.T[i] << "\t";
	}
	int lens = gm.T.length();
	cout << "f������ ";
	for (int i = 0; i < lens; i++)
	{
		if (i == lens - 1)
		{
			cout << gm.f[i] << endl;
			break;
		}
		cout << gm.f[i] << "\t";
	}
	cout << "g������ ";
	for (int i = 0; i < lens; i++)
	{
		if (i == lens - 1)
		{
			cout << gm.g[i] << endl;
			break;
		}
		cout << gm.g[i] << "\t";
	}
}

void test5(bool b);  //���GuiYueQi�Ƿ�ʶ�������Լ��,bΪtrue����˵����ʶ��

bool GuiYueQi(G &gm, string str)  //gmΪ�ķ������ݽṹ��strΪ����Լ��,��ʶ��Ϊtrue������Ϊfalse
{
	string strStck="";  //��string����������ջ������ȡֵ
	strStck.append("#");  //��ʼ��
	int k = 0,num=0;
	int j;  //j��ʾ�����ض���Ŀ�ͷλ��
	string a="";  //���뵥Ԫ
	string Q = "";  //��Ϊ�����������ض���Ŀ�ͷ����
	while (a!="#")  //�������봮�����һ���ַ�������
	{
		a = str[num++];  //����һ��������Ŷ���a��
		if (gm.T.find(strStck[k]) != gm.T.npos)  //���ջ���ķ���Ϊ�ս��
		{
			j = k;
		}
		else
		{
			j = k - 1;
		}
		while (gm.f[gm.T.find(strStck[j])] > gm.g[gm.T.find(a)])  //�����ң�ֱ���ҵ�ջ�ڷ������ȼ�С�ڻ���ڵ�ǰ������ŵ����ȼ�
		{
			do
			{
				Q = strStck[j];
				if (gm.T.find(strStck[j - 1]) != gm.T.npos)  //
				{
					j = j - 1;
				}
				else
				{
					j = j - 2;
				}
			} while (gm.f[gm.T.find(strStck[j])] >= gm.g[gm.T.find(Q)]);

			//��strStck[j+1]~strStck[k]��ԼΪĳ��N
			strStck.erase(strStck.begin() + j + 1, strStck.begin() + k);  //�ѹ�Լ�ķ�����ջ
			strStck.append("N");  //ѹ��ĳ�����ս��N
			k = j + 1;
			break;
		}

		if (gm.f[gm.T.find(strStck[j])] <= gm.g[gm.T.find(a)])  
		{
			k = k + 1;
			strStck.append(a);
		}
		else
		{
			return false;
		}
	}
	

	return true;  //
}

void test5(bool b)
{
	if (b)
	{
		cout << "acc!" << endl;
	}
	else
	{
		cout << "fail!" << endl;
	}
}

int main()
{
	G gm;
	string str[10];
	int len;  //����ı��ʽ����
	int i = 0;
	//���ļ������ķ�
	ifstream ifile("wenfa1.txt");  //��wenfa.txt�ļ�
	while (ifile)
	{
		ifile >> str[i++];  //�����ķ�
	}
	ifile.close();  //�ر��ļ�
	len = i - 1;
	zero(str, len);  //���ݡ�|������ֱ��ʽ
	first(gm, str, len);  //�õ��ķ����ݽṹ
	//test1(gm);  //���zero()��first()����

	second(gm);  //�õ�ÿ�����ս����FIRSTVT��LASTVT����
	//test2(gm);  //���second����

	third(gm);  //�õ����ȷ�����
	//test3(gm);  //���third����

	four(gm);  //�õ����Ⱥ���f��g
	//test4(gm);  //���four����

	string guiyuestr;  //��Լ��
	ifile.open("guiyuechuan3.txt");
	stringstream io;
	io <<ifile.rdbuf();
	io >> guiyuestr;
	cout << "����Լ��Ϊ��"<< guiyuestr <<endl;
	bool b = GuiYueQi(gm, guiyuestr);  //��Լ������Լstr
	test5(b);  //������Լ������Լ�������Ľ��

	return 0;
}