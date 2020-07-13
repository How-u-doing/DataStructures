#define preorder_create_tree 1
#define nonrecursive_visit 1
#include "BinTree.h"
#include <limits>	// for std::numeric_limits
#include <iostream>

using namespace std;

void kill_extra_char()
{
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
	BinTree<char> tree, tree_L, tree_R;
	// say input: A(B(D, E(, G)), C(F, ))#
	// or ABD##E#G##CF###  when defined 
	// preorder_create_tree
	cin >> tree;

	cout << "\ntree input via console is: " << tree << "\n\n"
		<< "tree.size()=" << tree.size() << '\n'
		<< "tree.leaves_count()=" << tree.leaves_count() << '\n'
		<< "tree.depth()=" << tree.depth() << "\n\n"
		<< "preorder: "; tree.preorder(); cout << '\n'
		<< "inorder: "; tree.inorder(); cout << '\n'
		<< "postorder: "; tree.postorder(); cout << '\n'
		<< "level_order: "; tree.level_order(); cout << "\n\n";

	tree_L.set_root(tree.get_root()->_lchild);
	tree_R.set_root(tree.get_root()->_rchild);
	cout << "constructing left subtree: " << tree_L << "\n\n";
	cout << "constructing right subtree: " << tree_R << "\n\n";
	
	BinTree<char> tree_2;
	tree_2.set_root(tree_2.create_tree("ABDEGCF", "DBEGAFC", 7));
	cout << "\ntree_2 constructed with \"ABDEGCF\" as preorder & \"DBEGAFC\" "
		<< "as inorder is: " << tree_2 << "\n\n";

#if defined _WIN32 || _WIN64
	cout << "Press any key to leave...\n";
	char wait;
	kill_extra_char();
	cin >> noskipws >> wait;
#endif // defined _WIN32 || _WIN64
	return 0;
}
