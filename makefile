quineMcCluskey: linkedlist petrick
	gcc -o quineMcCluskey main.c linkedlist.c petrick.c

linkedlist: 
	gcc -c linkedlist.c

petrick: linkedlist
	gcc -c petrick.c