/*

Platform:
- GCC Version: (Apple LLVM version 9.0.0 (clang-900.0.39.2) Target: x86_64-apple-darwin16.7.0)
- Online Complier (https://www.onlinegdb.com/online_c_compiler)
Author: Tim
*/

#include <stdio.h>
#include <stdlib.h>

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

signed int GetHeight(Node* root)
{
   if (root==NULL)
       return 0;
   else
   {

       signed int l = GetHeight(root->left);
       signed int r = GetHeight(root->right);


       if (l > r) {
         return(l+1);
       }else{
         return(r+1);
       }
   }
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

/*
* max in C
* https://stackoverflow.com/questions/3437404/min-and-max-in-c?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
*/

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

/*
* print tree
* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/
*/

char depth[ 2056 ];
int di;
/*
* print tree
* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/
*/
void Push( char c )
{
    depth[ di++ ] = ' ';
    depth[ di++ ] = c;
    depth[ di++ ] = ' ';
    depth[ di++ ] = ' ';
    depth[ di ] = 0;
}
/*
* print tree
* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/
*/

void Pop( )
{
    depth[ di -= 4 ] = 0;
}
/*
* print tree
* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/
*/
void PrintTree( Node* root )
{
    printf( "(%d)\n", root->data );

    if ( root->left )
    {
        printf( "%s `--", depth );
        Push( '|' );
        PrintTree( root->left );
        Pop( );

        printf( "%s `--", depth );
        Push( ' ' );
        PrintTree( root->right );
        Pop( );
    }
}

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
    return max(leftChildHeight, rightChildHeight) + 1;
}

int isBalanced(BSTHead* myTree){

  if (CheckTreeHeight(myTree -> root) != -1){
    // it is balanced
    return 0;

  }else{
    int l = GetHeight( myTree->root->left);
    int r = GetHeight( myTree->root->right);
    if (l >r){
      return -1;
    }else{
      return 1;
    }
  }
}




int main(){
  BSTHead* myBST = CreateBST();

// populate the tree automatically
  // PopulateTree(myBST, 2);

// populate the tree manually
  AddNode(myBST, 17);
  AddNode(myBST, 20);
  AddNode(myBST, 11);
  AddNode(myBST, 17);
  AddNode(myBST, 127);
  AddNode(myBST, 1);
  AddNode(myBST, 11);
  AddNode(myBST, 9);
  AddNode(myBST, 11111);
  AddNode(myBST, 91);
    AddNode(myBST, 1);
  AddNode(myBST, 19);

  printf("show the tree: \n");
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

  PrintTree(myBST->root);

  return 1;

}