Traversal is done recursively: 
	if the original file is a directory, traversal will call itself on that directory. if it is a file, it will check for tokens, and move on to look for more files
	(unless it is in the root directory)

	Complexity:if there are n directories and y files, the time complexity is O(n+y).
		(ONLY For the traversal)

Checking for tokens:
Program uses fscanf to alternately scan tokens and delimiters (delimiter scans are thrown away).
When a token is obtained, the program checks a hash table to see if it has been found before, if not, it is added to the hash table (with a count of 1 and filename it appeared in)
	if the token already exists in the hash table, it will check to see if it was in the same file
		-if it was from the same file, the count is increased
		-if it was not, a new "filetoken" is created with a count of 1 and current filename
	
	Complexity: since the hash lookup and adding is near constant time, it is ignored
		    The slowest part will be scanning the tokens from the files, fscanf is scanning in 256 byte chunks, so if x is the amount of characters in the document all of the documents,
			the time complexity to scan all documents will be ~O(x/256), which is still O(x).

Writing to file:
The program calls print_tokens to write all of the output to a file, which is O(n*5) time in the worst case if there are n tokens and each token was found in 5 files. Which is simply O(n).

///SEARCH ///////---------------1!@$!@%!@#

The search algorithm makes use of the sorted-list program, modified to sort in alphabetical/ascending order instead of reverse.

For "sa", the input is parsed via strtok and tokenized. all tokens are then added to a sorted-list. The sorted list is traversed one term at a time, and the program finds that term in the 
	inverted index file. Then, it goes to the next line, where the filenames are written. It tokenizes that line and throws away every other token (the number of appearances in the file).
	Using the function, compareSL, which takes 2 sortedlistptrs as arguments, the two sorted-lists are compared AND modified, keeping ONLY the file tokens that had all of the terms in the term SL.
	It continues to do this on every term, updating the file token SL with every term, until eventually only the files that contained all terms remain.

For "so", I just modified the sa command slightly. Instead of comparing/removing, it simply removes duplicate file tokens from the file list.

"q" was pretty straight forward... just a printf and a return.
