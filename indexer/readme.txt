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
