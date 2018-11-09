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
	bool isempty()const { return pos == 0; }
	bool isfull()const { return pos == max; }
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

class QUEUE:public STACK {
	STACK  s2;
public:
	QUEUE(int m); //每个栈最多m个元素，要求实现的队列最多能入2m个元素
	QUEUE(const QUEUE&s); 			//用队列s拷贝初始化队列
	virtual operator int() const;			//返回队列的实际元素个数
	virtual int full() const;		       //返回队列是否已满，满返回1，否则返回0
	virtual int operator[ ](int x)const;   //取下标为x的元素，第1个元素下标为0
	virtual QUEUE& operator<<(int e);  //将e入队列,并返回队列
	virtual QUEUE& operator>>(int &e);	//出队列到e,并返回队列
	virtual QUEUE& operator=(const QUEUE&s); //赋s给队列,并返回被赋值的队列
	virtual void print() const;			//打印队列
	virtual ~QUEUE();					//销毁队列
};

QUEUE::QUEUE(int m) :STACK(m), s2(m) {}

QUEUE::QUEUE(const QUEUE&s):STACK(s),s2(s.s2) {}

QUEUE::operator int() const {
	return (int)s2 +STACK::operator int();
}

int QUEUE::full() const {
	//if (s2.isfull() && this->isfull())
	//	return 1;
	if (!s2.isempty() && !s2.isfull()&&STACK::isfull())
		return 1;
	else return 0;
}

int QUEUE::operator[ ](int x)const {
	if (x < (int)s2)
		return s2[int(s2) - 1 - x];
	else return STACK::operator[](x - int(s2));
}

QUEUE& QUEUE::operator<<(int e) {
	if (full()) return *this;
	if(STACK::isfull()){
		while (!STACK::isempty()) { //换栈
			int swp;
			STACK::operator>>(swp);
			s2 << swp;
		}
	}
	STACK::operator<<(e);
	return *this;
}

QUEUE& QUEUE::operator>>(int &e) {
	if (STACK::isempty() && s2.isempty())
		return *this;
	if (s2.isempty()) {
		while (!STACK::isempty()) {
			int swp;
			STACK::operator>>(swp);
			s2 << swp;
		}
	}
	s2 >> e;
	return *this;
}

QUEUE& QUEUE:: operator=(const QUEUE&s) {
	STACK::operator=(s);
	s2 = s.s2;
	return *this;
}

void QUEUE::print() const {
	for (int i = (int)s2 - 1; i >= 0; i--)
		cout << s2[i] << "  ";
	for (int i = 0; i < STACK::operator int(); i++)
		cout << STACK::operator[](i) << "  ";
}

QUEUE:: ~QUEUE() {}

int main(int argc, char *argv[]) {
	int i = 1; QUEUE* ps = nullptr;
		for (i = 1; i < argc; ++i)
		{
			string s = argv[i];
			if (s == "-S") {
				int num = stoi(argv[i + 1]);
				if (num <= 0) {
					cout << "S  E  "; break;
				}
				else {
					ps = new QUEUE(num);
				}
				cout << "S  " << ps->size() << "  ";
				i++;
			}
			else if (s == "-I") {
				while (i + 1 < argc && argv[i + 1][0] != '-') {
					if (ps->full()) {
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
					if (int(*ps)==0) {
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
				QUEUE *pps;
				pps = new QUEUE(*ps);
				delete ps;
				ps = pps;
				cout << "C  ";
				ps->print();
			}
			else if (s == "-G") {
				int num = stoi(argv[i + 1]);
				if (num + 1 > int(*ps))
				{
					cout << "G  E  ";
					return 0;
				}
				cout << "G  " << (*ps)[num] << "  ";
				i++;
			}
			else if (s == "-A") {
				int num = stoi(argv[i + 1]);
				QUEUE *ptemp = new QUEUE(num);
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
