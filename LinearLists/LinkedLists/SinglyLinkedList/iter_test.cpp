#include<iostream>
#include<string>
#include"Node.h"
using namespace std;

int main()
{
	Node<string> ns("Iron Man"),*pns=&ns;
	Nodeptr<string> p = &ns, p2 = p;
	ns.next = new Node<string>("Tony Stark");
	p->next->next = new Node<string>("Marvelous");
	if (p == pns) cout << "p and pns point to the same object" << endl;
	else cout << "p and pns point to different objects" << endl;
	p2 += 2;
	cout << p->data << endl;
	cout << (++p)->data << endl;
	cout << p2->data << endl;	
	return 0;
}
