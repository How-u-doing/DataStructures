#include "Tree.h"
#include <iostream>

using namespace std;

int main()
{
	Tree<char> tree;
	auto root = tree.get_root();
	//tree.create_tree(root);	// to be added
	//          A
	//        / | \	
	//       B  C  D
	//       |    / \
	//       E    F  G
	cout <<
		"            A\n"
		"          / | \\     \n"
		"         B  C  D     \n"
		"         |    / \\   \n"
		"         E   F   G   \n";


	root = new TreeNode<char>('A');
	root->_first_child = new TreeNode<char>('B');
	root->_first_child->_first_child=new TreeNode<char>('E');

	root->_first_child->_next_sibling = new TreeNode<char>('C');
	root->_first_child->_next_sibling->_next_sibling = new TreeNode<char>('D');

	root->_first_child->_next_sibling->_next_sibling
		->_first_child = new TreeNode<char>('F');

	root->_first_child->_next_sibling->_next_sibling
		->_first_child->_next_sibling = new TreeNode<char>('G');

	cout << "All paths from root to leaves are as follows:\n";
	tree.print_path(root);
	cout << "number of tree nodes: " << tree.count_node(root) << '\n';
	cout << "number of tree leaves: " << tree.count_leaves(root) << '\n';
	cout << "depth:  " << tree.depth(root) << '\n';
	cout << "preorder: "; tree.preorder(root); cout << '\n';
	cout << "postorder: "; tree.postorder(root);

	cout << "\n\nPress any key to leave...\n";
	char wait;
	cin >> noskipws >> wait;

	return 0;
}