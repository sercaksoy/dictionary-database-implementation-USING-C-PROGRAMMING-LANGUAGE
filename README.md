# dictionary-database-implementation-USING-C-PROGRAMMING-LANGUAGE

## Programs GOAL 

Just to find in which documents a word occurs may take long time, if the way is searching sequally. Therefor this program keeps track of it ; our database is a .txt file which located in the same file directory with our .exe file.
Lets give a name to our .txt file, data.txt;
data.txt keeps THE LOAD FACTOR which is calculated with dividing loaded hashTable indexes to hashTable capacity.
The size of our table is limited to 997.
Lets say we have x different words and documents they occurs.
data.txt file starts with a line "LOAD FACTOR:(a float value comes from x/997)"
This line helps us to avoid from exceed the hash size.
Below that line, data.txt keeps the information in this format:
"word1,doc1.txt,doc2.txt,
 word2,doc1.txt,
 word3,doc2.txt,
 word4,doc4.txt,doc1.txt,"
 The method to avoid collusion in the hash table settled 'double hashing' by the instructors.
 
 ### Double Hashing
 
Double hashing is a method to avoid collusion in the hashtable, for this purpose every key(I mean pre-key) has to undergo another hashing, return from the second hashing needs to be checked so it still does not cause another collusion problem. The second hashing part loops until an eye is found or exceed of size.

### Converting a String to a Key

To insert any information into our hash table, information needs an index. Horner's method was settled for this purpose by the instructors.
Horner's method decompose the words by their character values and their order in the word. We can use ASCII table to get character values for each.
For this method we need to decide which prime number we are going to use. Several prime numbers work just fine for this algoritm but in this context, I decided to go with '31' because below 31, may not be enough to create the big numbers that I need and above 31, is may create meaninglessly large numbers.

To calculate this numbers I am writing about. This pseudo code is used:
double get_key(word,word_length):
  R_HORNER_NUMBER = 31
  i = words_length - 1
  while i>-1 :
    key = key + (R_HORNER_NUMBER ^ i) * word[word_length-i-1] - 'a' +1
    --i
  end
 return key

### From an(almost)unique and large number to index

We get our word and turned it into a key. But still it is not usable for our hash table. At least it is not efficient to use it before ' to hash ' process.
In the -double hashing- section I already said that the program uses the double hashing algoritm to insert and search operations. Pseudo code of this process like this:

int hash(key,i,hashSize):
  h1 = key mod(hashSize)
  h2 = 1 + key mod(hashSize-1) // In this line we can use any number below hashSize 
 return (h1 + i*h2) mod (hashSize)
 
 Hardly(it gets easier as the table fills) but this function may generate non unique indexes, i value is given in the parameter to avoid this problem as I told in the -double hashing section-
