#include "Tree.h"
#include <limits>	// for std::numeric_limits
#include <iostream>

using namespace std;

void kill_extra_char()
{
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void tree_operations()
{
	Tree<char> tree;
	// input via console, say
	// A(B(E), C, D(F,G))#
	// A(B(E(H,I,J)), C, D(F,G))#
	// A(B(E,P,Q(M,N)), C(X,Y,Z), D(F,G))#
	cout << "Please input your tree in generalized list, e.g. A(B(E), C, D(F,G))# \n--> ";
	cin >> tree;
	auto root{ tree.get_root() };

	cout << "\nThe tree inputted is: "; tree.print_tree(root); cout << '\n';
	cout << "All paths from root to leaves are as follows:\n";
	tree.print_path(root);
	cout << "number of tree nodes: " << tree.count_node(root) << '\n';
	cout << "number of tree leaves: " << tree.count_leaves(root) << '\n';
	cout << "depth:  " << tree.depth(root) << '\n';
	cout << "preorder: "; tree.preorder(root); cout << '\n';
	cout << "postorder: "; tree.postorder(root); cout << '\n';

}

int main()
{
	tree_operations();

#if defined _WIN32 || _WIN64
	cout << "\nPress any key to leave...\n";
	char wait;
	kill_extra_char();
	cin >> noskipws >> wait;
#endif // defined _WIN32 || _WIN64

	return 0;
}
