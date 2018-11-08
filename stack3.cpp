#include <iostream>
#include <string>
using namespace std;
class STACK {
	int  *const  elems;	//申请内存用于存放栈的元素
	const  int   max;	//栈能存放的最大元素个数
	int   pos;			//栈实际已有元素个数，栈空时pos=0;
public:
	STACK(int m);		//初始化栈：最多m个元素
	STACK(const STACK&s); 			//用栈s拷贝初始化栈
	virtual int  size() const;			//返回栈的最大元素个数max
	virtual operator int() const;			//返回栈的实际元素个数pos
	virtual int operator[ ] (int x) const;	//取下标x处的栈元素
	virtual STACK& operator<<(int e); 	//将e入栈,并返回栈
	virtual STACK& operator>>(int &e);	//出栈到e,并返回栈
	virtual STACK& operator=(const STACK&s); //赋s给栈,并返回被赋值的栈
	virtual void print() const;			//打印栈
	virtual ~STACK();					//销毁栈
	bool isempty() { return pos == 0; }
	bool isfull() { return pos == max; }
};

STACK::STACK(int m) :elems(new int[m]), max(m), pos(0) {}
STACK::STACK(const STACK&s) : elems(new int[s.max]), max(s.max), pos(s.pos) {
	for (int i = 0; i < s.pos; i++)
		elems[i] = s.elems[i];
}
int STACK::size() const
{
	return max;
}
STACK::operator int() const {
	return pos;
}
int  STACK::operator[](int x) const
{
	return elems[x];
}
STACK& STACK::operator<<(int e)
{
	elems[pos++] = e;
	return *this;
}
STACK& STACK::operator>>(int &e)
{
	e = elems[--pos];
	return *this;
}
STACK& STACK::operator=(const STACK&s)
{
	int** temp = (int**)&elems;//bruce conversion
	delete[] * temp; //delete the old elemts
	*temp = new int[s.max]; //point to new elemts
	int *mtemp = (int*)&max; //bruce conversion
	*mtemp = s.max; pos = s.pos;
	for (int i = 0; i < s.pos; i++)
		elems[i] = s.elems[i];
	return *this;
}
void STACK::print() const
{
	for (int i = 0; i < pos; i++)
		cout << elems[i] << "  ";
}
STACK::~STACK() {
	delete[] elems;
}
int main(int argc, char *argv[])
{
	int i = 1; STACK* ps=nullptr;
	for (i = 1; i < argc; ++i)
	{
		string s = argv[i];
		if (s == "-S") {
			int num = stoi(argv[i + 1]);
			if (num <= 0) {
				cout << "S  E  "; break;
			}
			else {
				ps = new STACK(num);
			}
			cout << "S  " << ps->size() << "  ";
			i++;
		}
		else if (s == "-I") {
			while (i + 1 < argc && argv[i + 1][0] != '-') {
				if (ps->isfull()) {
					cout << "I  E  "; return 0;
				}
				else (*ps)<<(stoi(argv[i + 1]));
				i++;
			}
			cout << "I  "; ps->print();
		}
		else if (s == "-O") {
			int num, t = stoi(argv[i + 1]);
			while (t--) {
				if (ps->isempty()) {
					cout << "O  E  "; return 0;
				}
				else {
					(*ps)>>(num);
				}
			}
			i++;
			cout << "O  "; ps->print();
		}
		else if (s == "-C") {
			STACK *pps;
			pps = new STACK(*ps);
			delete ps;
			ps = pps;
			cout << "C  ";
			ps->print();
		}
		else if (s == "-G") {
			int num = stoi(argv[i + 1]);
			if (num + 1 > int(*ps))
			{
				cout << "G E  ";
				return 0;
			}
			cout << "G  " << (*ps)[num] << "  ";
			i++;
		}
		else if (s == "-A") {
			int num = stoi(argv[i + 1]);
			STACK *ptemp = new STACK(num);
			(*ptemp)=(*ps);
			delete ps;
			ps = ptemp;
			cout << "A  "; ps->print();
			i++;
		}
		else { //-N
			cout << "N  " << (int)(*ps);
		}
	}
}
