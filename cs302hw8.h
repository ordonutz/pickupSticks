/*
 * Name: Leslie Ordonez
 * Description: This program simulates the playing of pick up sticks. The game
				requires players to avoid picking up sticks that are underneath
				other sticks. Through the help of a graph to store the order
				of the sticks and using top sort we are able to simulate the
				playing of this game. If the graph of a game has a cycle than
				it is impossible to win the game so we first check to see if a
				cycle is detected. The program modies depth first search to
				help detect cycles as well as pushes the sticks onto a myStack
				object in reverse top sort order.
 * Input: The input for this program is a text file. The text file contains the
		  number of vertices, edges, and the dependencies.
 * Output: The results for each test case are printed with n lines of integers.
		   Prints error message if there is no such correct order to p/u sticks.
*/

#include <iostream>

/*
* myStack: Dynamic array implementation of a stack. 
* constructors: myStack ();
				myStack ( const myStack < type >&);
* public functions: const myStack & operator =( const myStack < type >&);
					void push ( const type &);
					type pop ();
					type peek () const ;
					bool isEmpty () const ;
* static members:
*/
template <class type>
class myStack
{
public:
	myStack();
	myStack(const myStack <type>&);
	const myStack& operator=(const myStack <type>&);
	~myStack();
	void push(const type&);
	type pop();
	type peek() const;
	bool isEmpty() const;
private:
	type* stack;
	size_t size;
	size_t stackTop;
};

/*
* myStack: Default constructor, sets size to 10, allocates the array to stack
*          pointer, and sets stackTop to 0;
* parameters: what to pass into the function
* return value: what the function returns, if any
*/
template <class type>
myStack<type>::myStack()
{
	size = 10;
	stack = new type[size](); // initializes array with default values
	stackTop = 0;
}

/*
* myStack(const myStack& copy): Copy constructor
* parameters: const myStack& copy object to make a copy of
* return value:  none
*/
template <class type>
myStack<type>::myStack(const myStack& copy)
{
	// clear out j.i.c
	type* temp = stack;
	delete[] temp;
	stack = new type[copy.size]();
	size = copy.size;
	stackTop = 0;

	if (copy.stackTop != 0)
	{
		for (int i = 1; i < (static_cast<int>(copy.stackTop) + 1); i++)
		{
			stack[i] = copy.stack[i];
			stackTop++;
		}
	}	
}

/*
* operator=(const myStack<type>& rhs): Assignment operator
* parameters: const myStack<type>& rhs to make lhs equal to
* return value: lhs object
*/
template <class type>
const myStack<type>& myStack<type>::operator=(const myStack<type>& rhs)
{
	// clear out j.i.c
	type* temp = stack;
	delete[] temp;
	stack = new type[rhs.size]();
	size = rhs.size;
	stackTop = 0;

	if (rhs.stackTop != 0)
	{
		for (int i = 1; i < (static_cast<int>(rhs.stackTop) + 1); i++)
		{
			stack[i] = rhs.stack[i];
			stackTop++;
		}
	}

	return *this;
}

/*
* ~myStack(): Destructor
* parameters: none
* return value: none
*/
template <class type>
myStack<type>::~myStack()
{
	size = 0;
	stackTop = 0;
	delete[] stack;
	stack = NULL;
}

/*
* push(const type& item): Inserts item to the top of the stack and updates
						  the stackTop index, resizes if needed, if a resize
						  does occur, resize the stack to 10 more elements.
* parameters: const type& item to push to the top of the stack
* return value: none
*/
template <class type>
void myStack<type>::push(const type& item)
{
	if (stackTop == (size - 1)) // resize
	{
		int tempSize = stackTop + 1;
		size += 10;
		stackTop = 0;
		type* temp = stack;
		stack = new type[size]();

		for (int i = 1; i < tempSize; i++)
		{
			stack[i] = temp[i];
			stackTop++;
		}
		delete[] temp;
	}

	stack[(stackTop + 1)] = item; // insert element at end
	stackTop++;
}

/*
* pop(): Returns the top element as well as remove it if the stack is not empty.
* parameters: none
* return value: Top element of the stack
*/
template <class type>
type myStack<type>::pop()
{
	if (!isEmpty())
	{
		type temp; // store top element
		temp = stack[stackTop];
		stackTop--; // "remove" top element
		return temp;
	}
	return type();
}

/*
* peek(): Returns the top element without removing it from the stack.
* parameters: none
* return value: Top element of the stack or default value if empty.
*/
template <class type>
type myStack<type>::peek() const
{
	if (!isEmpty())
	{
		return stack[stackTop];
	}
	return type();
}

/*
* isEmpty(): returns true if the stack is empty and false otherwise.
* parameters: none
* return value: True for empty or False for not 
*/
template <class type>
bool myStack<type>::isEmpty() const
{
	return (stackTop == 0);
}

/*
* vertex: Custom class used for vertices in a graph. With helper class 
*         edgeIterator provides vertices and an interator for to point to a 
*		  vertex and iterate through its neighbors.
* constructors: vertex();
*       	    vertex(const vertex&);
* public functions: const vertex& operator =(const vertex&);
*					edgeIterator begin();
*					edgeIterator end();
*					void addEdge(const Type&);
* static members:
*/
template <class Type>
class vertex
{
	struct node
	{
		Type item;
		node* link;
	};
public:
	/*
* edgeIterator: Custom friend class of vertex used for an interator to point to 
*		        a vertex and iterate through its neighbors.
* constructors: edgeIterator();
*		        edgeIterator(node*);
*       	    
* public functions: edgeIterator operator ++(int);
*		            Type operator *();
*		            bool operator ==(const edgeIterator&);
*		            bool operator !=(const edgeIterator&);
* static members:
*/
	class edgeIterator
	{
	public:
		friend class vertex;
		edgeIterator();
		edgeIterator(node*);
		edgeIterator operator ++(int);
		Type operator *();
		bool operator ==(const edgeIterator&);
		bool operator !=(const edgeIterator&);
	private:
		node* current;
	};

	vertex();
	vertex(const vertex&);
	const vertex& operator =(const vertex&);
	~vertex();
	edgeIterator begin();
	edgeIterator end();
	void addEdge(const Type&);
private:
	node* neighbors;
};

/*
* edgeIterator(): Default constructor that sets current to NULL;
* parameters: none
* return value: 
*/
template <class Type>
vertex<Type>::edgeIterator::edgeIterator()
{
	current = NULL;
}

/*
* edgeIterator(vertexType>::node* edge: a constructor that takes in a node 
*									    object which gets assigned to current
* parameters: vertex<Type>::node* node object 
* return value: none
*/
template <class Type>
vertex<Type>::edgeIterator::edgeIterator(vertex<Type>::node* edge)
{
	current = edge;
}

/*
* edgeIterator::operator++(int): An operator function that sets the iterator to
*								 point to the next node object, you will need 
*								 to set current to point to the next node
*
* parameters: what to pass into the function
* return value: what the function returns, if any
*/
template <class Type>
typename vertex<Type>::edgeIterator vertex<Type>::edgeIterator::operator++(int)
{
	current = current->link;
	return *this;
}

/*
* operator*(): an operator that dereferences the iterator, returns the item
*			   field of the node that current points to
* parameters: none
* return value: item field of the node or default value if empty
*/
template <class Type>
Type vertex<Type>::edgeIterator::operator*()
{
	if (current != NULL)
	{
		return current->item;
	}
	return Type();
}
/*
* operator==: - compares the address of the iterator on the left side with the
*				iterator on the right side, returns true if they both point to
*               the same node, and returns false otherwise
* parameters: rhs vertex edgeiterator 
* return value: True if lhs and rhs point to same node false otherwise
*/
template <class Type>
bool vertex<Type>::edgeIterator::operator==(const vertex<Type>::edgeIterator& rhs)
{
	if (this->current == rhs.current) 
	{
		return true;
	}
	return false;
}

/*
* :operator!=: Compares the address of the iterator on the left side with the 
 *              iterator on the right side, returns false if they both point to 
*			   the same node, and returns true otherwise
* parameters: rhs vertext edgeiterator
* return value: True if lhs and rhs do not point to same node false otherwise 
*/
template <class Type>
bool vertex<Type>::edgeIterator::operator!=(const vertex<Type>::edgeIterator& rhs)
{
	if (this->current == rhs.current)
	{
		return false;
	}
	return true;
}

/*
* vertext(): Default constructor that sets neighbors to NULL
* parameters: none
* return value: none
*/
template <class Type>
vertex<Type>::vertex()
{
	neighbors = NULL;
}

/*
* vertex(const vertex<Type>& copy): a copy constructor that deep copies the
									neighbor list of the object passed into the 
									constructor to the object that calls the 
									constructor
* parameters: vertext to make a deep copy of
* return value: none
*/
template <class Type>
vertex<Type>::vertex(const vertex<Type>& copy)
{
	node* temp;     // ptr to traverse copy list
	node* next;     // ptr to link list
	node* nextNode; // ptr for each new node that is allocated
	
	// clear out this object j.i.c
	while (this->neighbors != NULL)
	{
		temp = neighbors;
		neighbors = neighbors->link;
		delete temp;
	}
	
	neighbors = NULL;

	if (copy.neighbors == NULL)
	{
		return;
	}

	temp = copy.neighbors;          // pts to copy list
	neighbors = new node;           // this list has a starting node
	neighbors->item = temp->item;   // copy item of copy list
	neighbors->link = NULL;         // set link to NULL
	next = neighbors;               // use to link this list
	temp = temp->link;              // next node to be copied

	while(temp != NULL)             // until the end of copy list
	{
		nextNode = new node;         // create new node to copy into
		nextNode->item = temp->item; // copy item of next copy node 
		nextNode->link = NULL;       // end of stack at this point
		next->link = nextNode;       // previous node points to new next node 
		next = nextNode;             // next ptr moves to new node created  
		temp = temp->link;           // temp ptr moves to next element to copy
	}
}

/*
* operator=(const vertext<Type>& rhs: assignment operator, that performs a deep
									  copy of the right side object with the left
									  side object (the object that calls the 
									  operator function)
* parameters: rhs side vertex to make a deep copy of into lhs
* return value: *this return the pointer to the lhs
*/
template <class Type>
const vertex<Type>& vertex<Type>::operator=(const vertex<Type>& rhs)
{
	if (this != rhs)
	{
		vertex<Type>::vertex(rhs);
	}
	return *this;
}

/*
* ~vertex(): Destrutor deallocates all the nodes in its neighbor list
* parameters: none
* return value: none
*/
template <class Type>
vertex<Type>::~vertex()
{
	node* temp; // point to node to be deleted

	while (this->neighbors != NULL)
	{
		temp = neighbors;
		neighbors = neighbors->link;
		delete temp;
	}

	neighbors = NULL;
}

/*
* begin(): returns a edgeIterator object whose current will be the head of the
*          neighbor list for the vertex object
* parameters: none
* return value: edgeIterator whose current is the head of the neibor list
*/
template <class Type>
typename vertex<Type>::edgeIterator vertex<Type>::begin()
{
	edgeIterator lstBeg;
	lstBeg.current = neighbors;
	return lstBeg;
}

/*
* end(): returns a edgeIterator object whose current will be assigned to NULL
* parameters: none
* return value: edgeIterator object with current = NULL
*/
template <class Type>
typename vertex<Type>::edgeIterator vertex<Type>::end()
{
	edgeIterator lstEnd;
	lstEnd.current = NULL;
	return lstEnd;
}

/*
* addEdge(const Type& edge): adds a new node into the neighbor list (a head 
*                            insert would be the best way to implement this)
* parameters: const Type& edge to be inserted
* return value: none
*/
template <class Type>
void vertex<Type>::addEdge(const Type& edge)
{
	node* tmpNext = new node; // new node to be head inserted

	if (neighbors == NULL)    // neighbor list is empty
	{
		tmpNext->item = edge;
		tmpNext->link = NULL;
		neighbors = tmpNext;
	}
	
	else
	{
		tmpNext->item = edge;
		tmpNext->link = neighbors;
		neighbors = tmpNext;
	}
}