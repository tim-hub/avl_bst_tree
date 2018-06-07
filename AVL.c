/*

Platform:
- GCC Version: (Apple LLVM version 9.0.0 (clang-900.0.39.2) Target: x86_64-apple-darwin16.7.0)
- Online Complier (https://www.onlinegdb.com/online_c_compiler)
Author: Tim
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
  int     data;
  struct node* left;
  struct node* right;
}Node;

typedef struct{
  Node*    root;
}BSTHead;

/*
  Create The Binary Search Tree
*/
BSTHead* CreateBST(){
  BSTHead* myTree =  (BSTHead *) malloc (sizeof(BSTHead));
  myTree -> root = NULL;
  return myTree;
}

void AddNodeRecursively(Node* root, int data){
  if (data>= root->data){
    if (root -> right == NULL){
      Node* pNew = (Node *) malloc(sizeof(Node));
      pNew -> data = data;
      pNew -> left = NULL;
      pNew -> right = NULL;

      root -> right = pNew;

    }else {
      AddNodeRecursively(root -> right, data);
    }
  }else{
    if (root -> left == NULL){
      Node* pNew = (Node *) malloc(sizeof(Node));
      pNew -> data = data;
      pNew -> left = NULL;
      pNew -> right = NULL;

      root -> left = pNew;
    }else {
      AddNodeRecursively(root -> left, data);
    }
  }
}

void AddNode(BSTHead* myBST, int  data){
  if(myBST-> root ==NULL){

    Node* pNew = (Node *) malloc(sizeof(Node));
    pNew -> left = NULL;
    pNew -> right = NULL;
    pNew -> data = data;

    myBST ->root = pNew;

  }else{
    // Node* root =(Node *) malloc(sizeof(Node));
    // root =myBST -> root;
    AddNodeRecursively(myBST -> root, data);
  }
}

void TraversalInOrder(Node* root){
  if (root != NULL){
    TraversalInOrder(root -> left);
    printf ("%d, ", root->data);
    TraversalInOrder(root -> right);
  }
  // if we print it inorder way
  // then the output should be in order
  // if the output is in order, then the program works well
}

void CountNodes(Node* root, signed int *p_count){

  if (root !=NULL){

    CountNodes(root -> left, p_count);

    CountNodes(root -> right, p_count);
    ++ *p_count;
    // printf("%lu ", *p_count);
  }
}

int GetHeight(Node* root)
{
   if (root==NULL)
       return 0;
   else
   {

       int l = GetHeight(root->left);
       int r = GetHeight(root->right);


       if (l > r) {
         return(l+1);
       }else{
         return(r+1);
       }
   }
  // int height_left = 0;
	// int height_right = 0;
  // if( root->left ) height_left = GetHeight( root->left );
  // if( root->right ) height_right = GetHeight( root->right );

  // return height_right > height_left ? ++height_right : ++height_left;
}

void PopulateTree(BSTHead* myBST, int length){
  for (int i=0; i<length; i++){
    AddNode(myBST, (rand()%(100+1-1)) +1);
  }
}

/*
the above is the work from lab 4

the below is the new work for AVL
*/


// ASCII printing of BST
// https://pastebin.com/kmxFA5ax
typedef struct asciinode_struct asciinode;

struct asciinode_struct
{
    asciinode * left, * right;

    //length of the edge from this node to its children
    int edge_length;

    int height;

    int lablen;

    //-1=I am left, 0=I am root, 1=right
    int parent_dir;

    //max supported unit32 in dec, 10 digits max
    char label[11];
};

#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

//adjust gap between left and right nodes
int gap = 3;

//used for printing next node in the same level,
//this is the x coordinate of the next char printed
int print_next;


asciinode * build_ascii_tree_recursive(Node * t);
asciinode * build_ascii_tree(Node * t);
void free_ascii_tree(asciinode *anode);
void compute_lprofile(asciinode *anode, int x, int y);
void compute_rprofile(asciinode *anode, int x, int y);
void compute_edge_lengths(asciinode *anode);
void print_level(asciinode *anode, int x, int level);
void print_ascii_tree(Node * t);

//printing tree in ascii

//used for printing next node in the same level,
//this is the x coordinate of the next char printed


int MIN (int X, int Y)
{
    return ((X) < (Y)) ? (X) : (Y);
}

int MAX (int X, int Y)
{
    return ((X) > (Y)) ? (X) : (Y);
}

asciinode * build_ascii_tree_recursive(Node * t)
{
    asciinode * anode;

    if (t == NULL)
        return NULL;

    anode = ( struct asciinode_struct* )malloc(sizeof(asciinode));
    anode->left = build_ascii_tree_recursive(t->left);
    anode->right = build_ascii_tree_recursive(t->right);

    if (anode->left != NULL)
    {
        anode->left->parent_dir = -1;
    }

    if (anode->right != NULL)
    {
        anode->right->parent_dir = 1;
    }

    sprintf(anode->label, "%d", t->data);
    anode->lablen = strlen(anode->label);

    return anode;
}

//Copy the tree into the ascii node structre
asciinode * build_ascii_tree(Node * t)
{
    asciinode *anode;
    if (t == NULL)
        return NULL;
    anode = build_ascii_tree_recursive(t);
    anode->parent_dir = 0;
    return anode;
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *anode)
{
    if (anode == NULL)
        return;
    free_ascii_tree(anode->left);
    free_ascii_tree(anode->right);
    free(anode);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *anode, int x, int y)
{
    int i, isleft;
    if (anode == NULL)
        return;
    isleft = (anode->parent_dir == -1);
    lprofile[y] = MIN(lprofile[y], x-((anode->lablen-isleft)/2));
    if (anode->left != NULL)
    {
        for (i=1; i <= anode->edge_length && y+i < MAX_HEIGHT; i++)
        {
            lprofile[y+i] = MIN(lprofile[y+i], x-i);
        }
    }
    compute_lprofile(anode->left, x-anode->edge_length-1, y+anode->edge_length+1);
    compute_lprofile(anode->right, x+anode->edge_length+1, y+anode->edge_length+1);
}

void compute_rprofile(asciinode *anode, int x, int y)
{
    int i, notleft;
    if (anode == NULL)
        return;
    notleft = (anode->parent_dir != -1);
    rprofile[y] = MAX(rprofile[y], x+((anode->lablen-notleft)/2));
    if (anode->right != NULL)
    {
        for (i=1; i <= anode->edge_length && y+i < MAX_HEIGHT; i++)
        {
            rprofile[y+i] = MAX(rprofile[y+i], x+i);
        }
    }
    compute_rprofile(anode->left, x-anode->edge_length-1, y+anode->edge_length+1);
    compute_rprofile(anode->right, x+anode->edge_length+1, y+anode->edge_length+1);
}

//This function fills in the edge_length and
//height fields of the specified tree
void compute_edge_lengths(asciinode *anode)
{
    int h, hmin, i, delta;
    if (anode == NULL)
        return;
    compute_edge_lengths(anode->left);
    compute_edge_lengths(anode->right);

    /* first fill in the edge_length of node */
    if (anode->right == NULL && anode->left == NULL)
    {
        anode->edge_length = 0;
    }
    else
    {
        if (anode->left != NULL)
        {
            for (i=0; i<anode->left->height && i < MAX_HEIGHT; i++)
            {
                rprofile[i] = -INFINITY;
            }
            compute_rprofile(anode->left, 0, 0);
            hmin = anode->left->height;
        }
        else
        {
            hmin = 0;
        }
        if (anode->right != NULL)
        {
            for (i=0; i<anode->right->height && i < MAX_HEIGHT; i++)
            {
                lprofile[i] = INFINITY;
            }
            compute_lprofile(anode->right, 0, 0);
            hmin = MIN(anode->right->height, hmin);
        }
        else
        {
            hmin = 0;
        }
        delta = 4;
        for (i=0; i<hmin; i++)
        {
            delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
        }
        //If the node has two children of height 1, then we allow the
        //two leaves to be within 1, instead of 2
        if (((anode->left != NULL && anode->left->height == 1) ||
                (anode->right != NULL && anode->right->height == 1))&&delta>4)
        {
            delta--;
        }

        anode->edge_length = ((delta+1)/2) - 1;
    }

    //now fill in the height of node
    h = 1;
    if (anode->left != NULL)
    {
        h = MAX(anode->left->height + anode->edge_length + 1, h);
    }
    if (anode->right != NULL)
    {
        h = MAX(anode->right->height + anode->edge_length + 1, h);
    }
    anode->height = h;
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *anode, int x, int level)
{
    int i, isleft;
    if (anode == NULL)
        return;
    isleft = (anode->parent_dir == -1);
    if (level == 0)
    {
        for (i=0; i<(x-print_next-((anode->lablen-isleft)/2)); i++)
        {
            printf(" ");
        }
        print_next += i;
        printf("%s", anode->label);
        print_next += anode->lablen;
    }
    else
        if (anode->edge_length >= level)
        {
            if (anode->left != NULL)
            {
                for (i=0; i<(x-print_next-(level)); i++)
                {
                    printf(" ");
                }
                print_next += i;
                printf("/");
                print_next++;
            }
            if (anode->right != NULL)
            {
                for (i=0; i<(x-print_next+(level)); i++)
                {
                    printf(" ");
                }
                print_next += i;
                printf("\\");
                print_next++;
            }
        }
        else
        {
            print_level(anode->left,
                        x-anode->edge_length-1,
                        level-anode->edge_length-1);
            print_level(anode->right,
                        x+anode->edge_length+1,
                        level-anode->edge_length-1);
        }
}

//prints ascii tree for given Node structure
void print_ascii_tree(Node * t)
{
    asciinode *proot;
    int xmin, i;
    if (t == NULL)
        return;
    proot = build_ascii_tree(t);
    compute_edge_lengths(proot);
    for (i=0; i<proot->height && i < MAX_HEIGHT; i++)
    {
        lprofile[i] = INFINITY;
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
    {
        xmin = MIN(xmin, lprofile[i]);
    }
    for (i = 0; i < proot->height; i++)
    {
        print_next = 0;
        print_level(proot, -xmin, i);
        printf("\n");
    }
    if (proot->height >= MAX_HEIGHT)
    {
        printf("(This tree is higher than %d, so might not be drawn correctly.)\n", MAX_HEIGHT);
    }
    free_ascii_tree(proot);
}
// END ASCII Printing






int CheckTreeHeight(Node * root)
{
  // -1 means unbalanced
  if(root == NULL) return 0;

  int leftChildHeight = CheckTreeHeight(root->left);
  if(leftChildHeight == -1) return -1;


  int rightChildHeight = CheckTreeHeight(root->right);
  if(rightChildHeight == -1) return -1;


  int heightDifference = leftChildHeight - rightChildHeight;

  if(abs(heightDifference) > 1)
    return -1;
  else
    return MAX(leftChildHeight, rightChildHeight) + 1;
}

int isBalanced(BSTHead* myTree){

  if (CheckTreeHeight(myTree -> root) != -1){
    // it is balanced
    return 0;

  }else{
    int l = GetHeight( myTree->root->left);
    int r = GetHeight( myTree->root->right);

    if (l >r){
      // left larger
      return -1;
    }else{
      // right larger
      return 1;
    }
  }
}
// left left
Node *rotateLeft( Node *node ) {
 	Node *a = node;
	Node *b = a->left;

	a->left = b->right;
	b->right = a;

	return( b );
}

// left right
Node *rotateLeftRight( Node *node ) {
	Node *a = node;
	Node *b = a->left;
	Node *c = b->right;

	a->left = c->right;
	b->right = c->left;
	c->left = b;
	c->right = a;

	return( c );
}

// left right
Node *rotateRightLeft( Node *node ) {
	Node *a = node;
	Node *b = a->right;
	Node *c = b->left;

	a->right = c->left;
	b->left = c->right;
	c->right = b;
	c->left = a;

	return( c );
}

// right right
Node *rotateRight( Node *node ) {
	Node *a = node;
	Node *b = a->right;

	a->right = b->left;
	b->left = a;

	return( b );
}

int getBalanceCount( Node *node ) {
	int bc = 0;

	if( node->left  ) bc -= GetHeight( node->left );
	if( node->right ) bc += GetHeight( node->right );

	return bc ;
}


Node *balanceNode( Node *node ) {
	Node *newroot = NULL;

	/* Balance our children, if they exist. */
	if( node->left )
		node->left  = balanceNode( node->left  );
	if( node->right )
		node->right = balanceNode( node->right );

	int balanceCount = getBalanceCount(node);

	if( balanceCount <= -2 ) {
		// left larger

		if( getBalanceCount( node->left ) >= 1 )
			newroot = rotateLeftRight( node );
		else
			newroot = rotateLeft( node );

	} else if( balanceCount >= 2 ) {
		// right larger

		if( getBalanceCount( node->right ) <= -1 )
			newroot = rotateRightLeft( node );
		else
			newroot = rotateRight( node );

	} else {
		/* This node is balanced -- no change. */
		newroot = node;
	}

	return( newroot );
}

/* Balance a given tree */
void balanceTree( BSTHead *tree ) {

	Node *newroot = NULL;

	newroot = balanceNode( tree->root );

	if( newroot != tree->root )  {
		tree->root = newroot;
	}
}


int main(){
  BSTHead* myBST = CreateBST();

// populate the tree automatically
  // PopulateTree(myBST, 10);

// populate the tree manually
  AddNode(myBST, 17);
  AddNode(myBST, 20);
  AddNode(myBST, 11);
  AddNode(myBST, 17);
  AddNode(myBST, 127);
  AddNode(myBST, 1);
  AddNode(myBST, 11);
  AddNode(myBST, 9);
  AddNode(myBST, 1111);
  AddNode(myBST, 91);
  AddNode(myBST, 1);
  AddNode(myBST, 19);

  printf("show the tree: \n");

  print_ascii_tree(myBST->root);
  TraversalInOrder(myBST -> root);

// counting test
  printf("\ncount of tree: ");
  signed int count;
  CountNodes(myBST -> root, & count) ;
  printf("%d \n", count);

// get tree height
  int h=GetHeight(myBST -> root);
  printf("tree height: %d \n", h);




  int bal = isBalanced(myBST);
  printf("tree balancing: %d\n", bal);


  if (bal == 0) {
    printf("\ntree is balanced, not required to be balanced\n");
  }
  else if (bal >0)
  {
    printf("\norginal tree is right larger, tree below is balanced \n");
    balanceTree(myBST);
    print_ascii_tree(myBST->root);

  }else{
    printf("\norginal tree is left larger, tree below is balanced\n");
    balanceTree(myBST);
    print_ascii_tree(myBST->root);

  }

  // end of main function
  return 1;
}