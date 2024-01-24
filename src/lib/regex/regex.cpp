// Includes
#include "regex.hpp"

/**
 * Converts a string to an integer.
 * 
 * This function reads a positive integer from a C-style string. It iterates through each character of the string,
 * converting it from an ASCII character to its corresponding integer value and building the integer result. 
 * The function assumes the string represents a non-negative integer and does not handle negative signs or non-numeric characters.
 *
 * @param s constant character pointer to the C-style string to be converted.
 * @return The integer value of the string. Returns 0 if the string does not start with a digit.
 */ 
static int ft_fast_atoi(const char* s){			// Read positive integer from string. Like "123" is converted to 123.
	int resI = 0;															
	for (;isDigit(*s); s++)
		resI = resI*10 + (s[0]-'0');
	return resI;
}


/****************************************************************************/
// Total there are 6 functions
// First parameter is the pattern. Second is the sample string and optional third is pointer to the end of the pattern
// Each one returns the number of consumed characters in sample string or NO_MATCH if inapplicable 
// (like 'a+' was applied on 'bbb'). Note 'a*' can be applied on 'bbb' and it consumes zero characters.
static inline int c_achar(	 const char* pat, const char* sam);					 // Handling single char comparison. Like 'b'
static inline int c_any(	 const char* pat, const char* sam);					 // Handlling 'any' comparison. '.'  Note: '.' may consume few bytes (1 char) when working with unicodes, and only 1 byte (= 1 char) for ascii.
static inline int c_extended(const char* pat, const char* sam);					 // Handaling special extended abreviations starting with '\\'
static inline int c_group(	 const char* pat, const char* sam);					 // Sub pattern. Grouping characters. Like (the) when searching inside 'I am the master of the realm'
static inline int c_option(	 const char* pat, const char* sam);					 // Selection of one option to match [aA]ce mathces both words: Ace and ace
static inline int c_multi(	 const char* pat, const char* sam, const char* endp);// Multiple occurance of the character. Like A+, A* A{4} A?

// Define pointers to functions that process the special commands. 2 Types
typedef int (*Stand_Func)(const char* pat, const char* sam                 );	// Standard functions and characters like () . [] \\ A 7 ....
typedef int (*SuffixFunc)(const char* pat, const char* sam,const char* endp);	// Suffix functions for multiple occurences like * + ? {}

// Inverse table of the above (given a character like '*', 'C', '\' get the appropriate command). We use a look up table for all possible ASCII characters
static const Cmd* get_cmd_byChar[128];
static int        isInitialized = 0;								// Was the look up table above initialized

/**
 * @brief Defines a table of commands representing regex rules.
 * 
 * The `cmd_tbl` structure is an array of `Cmd` objects, each representing a specific command 
 * or rule in regular expression processing. This table maps characters and regex constructs 
 * to their corresponding types and processing functions. It is essential for interpreting 
 * and executing the commands in a regex pattern.
 *
 * Each `Cmd` structure in the table contains:
 * - `id`: A character representing the regex command or a special character.
 * - `type`: A bitmask indicating the type of command (e.g., TYPE_OPEN, TYPE_CLOSE, TYPE_SUFFIX, TYPE_PREFIX, TYPE_CHAR).
 * - `f`: A pointer to the processing function specific to the command.
 *
 * Rules of commands:
 * - TYPE_CLOSE commands follow TYPE_OPEN commands immediately in the table.
 * - The table includes various regex constructs such as grouping '(', '|', '[', quantifiers '*', '+', '?', 
 *   as well as special handling for escape sequences ('\\') and any character ('.').
 * - The last entry in the table with id `0` is the default character processing command.
 *
 * This table is a central component of the regex processing mechanism, enabling the regex engine 
 * to efficiently map characters in a pattern to their respective handling logic.
 *
 * Example Usage:
 *   // The regex engine uses this table to find the appropriate command and function
 *   // for each character or construct in a regex pattern.
 */
// Define the table of command (regex rules). For each id, it's length, type of command and processing function
// Rules of commands: TYPE_CLOSE follows TYPE_OPEN immediately in command table
static const Cmd cmd_tbl[] = {
    { '(', TYPE_OPEN|TYPE_RECURSION,     (void*)c_group },
    { ')', TYPE_CLOSE|TYPE_RECURSION,    (void*)NULL },
    { '|', TYPE_CLOSE|TYPE_RECURSION,    (void*)NULL },
    { '[', TYPE_OPEN,                    (void*)c_option },
    { ']', TYPE_CLOSE,                   (void*)NULL },
    { '{', TYPE_SUFFIX|TYPE_OPEN,        (void*)c_multi },
    { '}', TYPE_CLOSE,                   (void*)NULL },
    { '*', TYPE_SUFFIX,                  (void*)c_multi },
    { '+', TYPE_SUFFIX,                  (void*)c_multi },
    { '?', TYPE_SUFFIX,                  (void*)c_multi },
    { '\\', TYPE_PREFIX,                (void*)c_extended },
    { '.', TYPE_CHAR,                   (void*)c_any },
    { 0, TYPE_CHAR,                     (void*)c_achar },
};

/**
 * @brief Custom implementation of the `strcpy_s` function for safe string copying.
 * 
 * This function is a custom implementation of the `strcpy_s` function, which is a safer version 
 * of the standard `strcpy` function. It is designed to prevent buffer overflows by checking 
 * the size of the destination buffer before copying the string. The function copies the source 
 * string to the destination buffer if there is enough space and returns a status code indicating 
 * the outcome of the operation.
 *
 * @param dest Pointer to the destination buffer where the string is to be copied.
 * @param destSize Size of the destination buffer in bytes.
 * @param src Pointer to the source string to be copied.
 * 
 * @return int Returns a status code indicating the result of the copy operation:
 *         - `STRCPY_S_SUCCESS` if the copy is successful.
 *         - `STRCPY_S_INVALID_ARGUMENT` if the destination pointer is null or the size is zero.
 *         - `STRCPY_S_BUFFER_TOO_SMALL` if the source string size exceeds the destination buffer size.
 *
 * The function operates as follows:
 * - Checks if the destination buffer is valid and has a non-zero size.
 * - Determines the length of the source string.
 * - Checks if the source string can fit into the destination buffer.
 * - If it fits, copies the source string to the destination buffer.
 * - If not, returns an error code indicating the buffer is too small.
 *
 * This function provides a more secure alternative to `strcpy` for copying strings in C++, 
 * especially in situations where buffer sizes are not guaranteed.
 *
 * Example Usage:
 *   char buffer[10];
 *   int result = strcpy_s(buffer, sizeof(buffer), "Hello");
 *   if (result == STRCPY_S_SUCCESS) {
 *       // String copied successfully
 *   }
 */
/****************************************************************************/
/****************************** Aux functions *******************************/
/****************************************************************************/
// Custom implementation of strcpy_s
static inline int strcpy_s(char* dest, size_t destSize, const char* src) {
    if (dest == nullptr || destSize == 0) {
        return STRCPY_S_INVALID_ARGUMENT;
    }

    size_t srcLength = strlen(src);

    if (srcLength >= destSize) {
        return STRCPY_S_BUFFER_TOO_SMALL;
    }

    strcpy(dest, src);

    return STRCPY_S_SUCCESS;
}


/**
 * @brief Initializes the regex mechanism's command lookup table.
 * 
 * This static inline function is responsible for setting up the `get_cmd_byChar` lookup table used 
 * in regex processing. It populates the table with pointers to command structures (`Cmd`) based on 
 * their identifiers. This initialization is required for the efficient execution of regex commands 
 * during pattern matching. The function ensures that it only initializes the table once.
 *
 * The function operates as follows:
 * - Checks if the lookup table has already been initialized to avoid redundant initialization.
 * - Iterates over the `cmd_tbl` array to find the end of the command list.
 * - Initializes the entire lookup table with pointers to the default command, which handles character processing.
 * - Iterates over the command list again, updating the lookup table with specific commands based on their identifiers.
 *
 * This initialization is a critical step in setting up the regex processing system, ensuring that commands 
 * are quickly accessible during pattern matching operations.
 *
 * Note: The function uses a flag `isInitialized` to ensure that the lookup table is initialized only once.
 *
 * Example Usage:
 *   // Before processing any regex patterns
 *   init_regex_mechanism_private();
 */
// Initialize the get_cmd_byChar[] look up table
static inline void init_regex_mechanism_private(void){
    if (isInitialized) return;
    const Cmd* cmd = cmd_tbl, *end = cmd_tbl;
    while (end->id) end++; // Find the last default command (character processing) and store it in 'end'

    for (int i = 0; i < 128; i++)
        get_cmd_byChar[i] = end; // Set the whole lookup table to point to the default command

    for (; cmd < end; cmd++) {
        // Cast cmd->id to int before using it as an index
        get_cmd_byChar[static_cast<unsigned char>(cmd->id)] = cmd; // For all the real commands set an entry in the lookup table
    }

    isInitialized = TRUE;
}

/**
 * @brief Finds the end of a null-terminated string.
 * 
 * This static inline function scans through a given string to locate its end, 
 * defined as the null character ('\0') that terminates standard C strings. 
 * It iteratively moves through the string until it encounters this null character.
 *
 * @param str Pointer to the beginning of the string.
 * 
 * @return const char* Returns a pointer to the null character at the end of the string.
 *
 * The function operates as follows:
 * - Iterates through the string character by character.
 * - Continues until it reaches the null character that signifies the end of the string.
 * - Returns a pointer to this null character, effectively marking the string's end.
 *
 * This function is particularly useful in contexts where the length of the string is not known in advance 
 * and needs to be determined, such as in various string processing or manipulation tasks.
 *
 * Example Usage:
 *   const char* str = "Hello World";
 *   const char* end = endOfString(str);
 *   // 'end' points to the null character at the end of "Hello World"
 */
// Find the end of the string.
static inline const char* endOfString(const char *str){
	while (*str) str++;
	return str;
}

/**
 * @brief Finds the first occurrence of a specified character in a string.
 * 
 * This static inline function searches through a given string to find the first occurrence of 
 * a specified character. It iterates over the string until it finds the character or reaches the 
 * end of the string.
 *
 * @param str Pointer to the beginning of the string in which the search is performed.
 * @param c The character to search for in the string.
 * 
 * @return const char* Returns a pointer to the first occurrence of the character in the string. 
 *         If the character is not found, the function returns NULL.
 *
 * The function operates as follows:
 * - Iterates through the string character by character.
 * - Compares each character in the string to the specified character.
 * - If the specified character is found, returns a pointer to its location in the string.
 * - If the end of the string is reached without finding the character, returns NULL.
 *
 * Example Usage:
 *   const char* str = "Hello World";
 *   char findChar = 'W';
 *   const char* position = findFirstCinS(str, findChar);
 *   if (position) {
 *       // Character found
 *   } else {
 *       // Character not found
 *   }
 */
// Find first occurence of character in the string. Returns the poitner in 'str' starting at 'c' or NULL if not found
static inline const char* findFirstCinS(const char *str, const char c){
	while ((*str)&&(*str-c)) str++;
	return (*str) ? str : NULL;
}

/**
 * @brief Finds the end of a regex expression enclosed within parentheses, including nested structures.
 * 
 * This static inline function is used to locate the end of an expression within parentheses 
 * in a regex pattern. It handles nested parentheses by tracking the depth of nesting, ensuring 
 * the correct identification of the enclosing right parenthesis. The function is designed to work 
 * with uncompiled regex patterns ('_uc' suffix), reading and processing the pattern in real-time.
 *
 * @param p Pointer to the beginning of the expression (at the left parenthesis 'lp').
 * @param rp Right parenthesis character that marks the end of the expression group.
 * 
 * @return const char* Returns a pointer to the position right after the matching right parenthesis. 
 *         Returns NULL if the matching right parenthesis is not found, indicating an imbalance or malformed expression.
 *
 * The function operates as follows:
 * - Starts just after the left parenthesis and iterates through the pattern.
 * - Adjusts the depth of nesting based on the occurrence of left and right parentheses, 
 *   taking into account escaped characters to avoid misinterpretation.
 * - Continues until it finds the matching right parenthesis at the top level of nesting.
 * 
 * Note: This function is part of a larger regex processing system and is essential for handling complex patterns with nested structures.
 *
 * Example Usage:
 *   const char* pattern = "([a-z]*(3+5)[xy])end";
 *   const char* current = pattern;
 *   const char* end = findExpressionEnd_uc(current, ')');
 *   // Process the expression within the parentheses
 */
// Given an expression starting with left parentheses 'p' return a pointer after the end of this expression (right parentheses 'rp').
// Example: given p=[a*(1+3)[z@][aa]]tp5 returns pointer to: 'tp5' by skipping
//          the [a*(1+3)[z@][aa]] expression.
// Note: handles nested parentheses by remembring the parentheses depth. Example: (((X)Y)Z)A  - X is in depth 3, Z - in depth 1, A in depth zero.
// Extension _uc means uncompiled. needs to read forward in real time to find the end of the expression.
static inline const char* findExpressionEnd_uc(const char *p, const char rp){
	char	lp  = *p;															// left parethesis
	int		depth  = 1;															// amount of '('- amount of ')'.
	MYBOOL	isValidCommand = TRUE;												//  \\[ doesn't count as valid parentheses since it should be treated as part of the text.
	for (p++; is_int_notZero(*p)&is_int_notZero(depth); p++){
		depth += isValidCommand * (is_int_equal(*p,lp) - is_int_equal(*p,rp));	// Update the depth only if parenthesis is valid. each 'lp' causes +1, rp causes -1
		isValidCommand = is_int_notEqual(*p,'\\');			// If current charachter == '\' than the parentheses become invalid
	}
	return (depth==0) ? p : NULL;							// (depth==0) -> Expresion parentheses was matched, otherwise end of string reached and Parentheses are not balanced
}

/**
 * @brief Finds the end of a regex expression or a sub-expression within a union set.
 * 
 * This static inline function is an extension of the standard regex parsing mechanism and 
 * is specifically designed to handle union sets (alternations) in regex patterns. It treats 
 * the '|' symbol as a delimiter for sub-expressions within a union set, enabling the parsing 
 * of each alternative expression within parentheses. The '_uc' suffix indicates that this 
 * function operates on uncompiled patterns, reading them in real-time to identify expression boundaries.
 *
 * @param p Pointer to the beginning of the expression or sub-expression (after the opening parenthesis or '|').
 * @param lp Left parenthesis character that marks the beginning of an expression group.
 * @param rp Right parenthesis character that marks the end of an expression group.
 * 
 * @return const char* Returns a pointer to the end of the current expression or sub-expression within a union set.
 *         Returns NULL if the end is not found, indicating a malformed expression.
 *
 * The function operates as follows:
 * - Iterates through the pattern starting just after the opening parenthesis or the last '|'.
 * - Counts the depth of nested parentheses to correctly identify the end of the top-level expression.
 * - Treats '|' as a delimiter for sub-expressions, but only at the top level of nesting.
 * - Skips escaped characters to prevent misinterpretation of special symbols.
 * - Returns the position right after the closing parenthesis of the current expression or sub-expression.
 * 
 * Note: This function is part of a larger regex processing system and is tailored for extended POSIX-style regex patterns.
 *
 * Example Usage:
 *   const char* pattern = "(A|B|C)";
 *   const char* current = pattern;
 *   const char* end = findExpressionEnd_UnionSet_uc(current, '(', ')');
 *   // Process the first expression within the union set
 */
// Same as findExpressionEnd_uc() but supports the the set-union extended POSIX defenition.
// This method considers the symbol '|' as closing parentheses. So (aab|ccd|ef) will return 'aab' as first expression.
//          On the second execution will return 'ccd' and on the third will return 'ef'.
// Extension _uc means uncompiled. needs to read forward in real time to find the end of the expression.
static inline const char* findExpressionEnd_UnionSet_uc(const char *p, const char lp, const char rp){
	int		depth  = 1;						
	MYBOOL	isValidCommand = TRUE;			
	for (p++; (*p!='\0')&&(depth!=0); p++){
		depth += isValidCommand * (is_int_equal(*p,lp) - is_int_equal(*p,rp));
		depth -= is_int_equal(depth,1)&is_int_equal(*p,'|');					// '|' affects only on top level. For example: (XXX(a|B|)DDD) the '|' are not possible alternatives
		isValidCommand = is_int_notEqual(*p,'\\');			
	}
	return (depth==0) ? p : NULL;			
}

/**
 * @brief Advances the pointer to the next unit (expression) in a regex pattern.
 * 
 * This static function is used to navigate through a regular expression pattern by identifying 
 * and moving to the next distinct unit or expression within the pattern. It handles different regex 
 * constructs, such as character classes, quantifiers, and groupings.
 *
 * @param cur Pointer to the current position in the regex pattern.
 * 
 * @return const char* Returns a pointer to the start of the next unit (expression) in the pattern.
 *
 * The function operates as follows:
 * - Retrieves the command associated with the current character in the pattern.
 * - If the current position represents the start of a group or a class (e.g., '[', '(', '{'), 
 *   it finds the corresponding closing character and returns a pointer just past it.
 * - For other types of regex commands, it advances the pointer by the length of the current command.
 * 
 * Note: This function is part of a larger regex processing system and relies on other components 
 * like 'get_cmd' and 'findExpressionEnd_uc' for its operation.
 *
 * Example Usage:
 *   const char* pattern = "[a-z]+(ABC|DEF)*";
 *   const char* current = pattern;
 *   while (*current != '\0') {
 *       current = goToNextPat_uc(current);
 *       // Process the current unit of the pattern
 *   }
 */
// Find next unit of pattern
static const char* goToNextPat_uc(const char* cur){   	
    const Cmd* cmd = get_cmd(*cur);						// Get the command	
    if isOpen(cmd)										// If this is open command: [,(,{ than search for closeing character Otherwise just advance forward
		return findExpressionEnd_uc(cur,(cmd+1)->id);	// Find the closing parentheses of cur.	
	return cur + cmdLength(cmd);						// Just skip the command
}

/**
 * @brief Compiles a regular expression pattern for later use in matching.
 * 
 * This member function of the 'tCompiledRegex' class takes a regular expression pattern as input, 
 * initializes the regex mechanism, and processes the pattern for efficient matching. It calculates the length 
 * of each expression in the pattern and handles special constructs like union sets in parentheses (e.g., '(A|B|C)'). 
 * The function assumes that the provided pattern is legal and correctly formatted.
 *
 * @param pat Pointer to the null-terminated string containing the regex pattern to be compiled.
 * 
 * @return const char* Returns a pointer to the end of the pattern if the compilation is successful, 
 *         or NULL if there is a failure in the compilation process.
 *
 * The function operates as follows:
 * - Initializes internal structures needed for regex processing.
 * - Iterates through the pattern to calculate the length of each expression, considering special cases like nested parentheses.
 * - Processes union sets within parentheses, calculating the length and position of alternatives.
 * - Marks the end of the pattern for reference in later matching operations.
 * - The current implementation has a complexity that may reach O(n^2) in the worst case, with potential optimization noted.
 * 
 * Note: This function is part of a larger regular expression matching system and is intended for internal use within this system.
 *
 * Example Usage:
 *   tCompiledRegex regex;
 *   const char* pattern = "(A|B)*C";
 *   const char* end = regex.compile(pattern);
 *   if (end) {
 *       // Compilation successful
 *   }
 */
/****************************************************************************/
/****************************** Compilation *********************************/
/****************************************************************************/
// Assumes the pattern is legal. Compiles it into 'C'. Returns the end of the pattern on success or NULL on failure.
const char* tCompiledRegex::compile(const char *pat){
	init_regex_mechanism_private();
	start = pat;																			// Pointer to the pattern
	// For each 'OPEN' rules calculate the length of the expression. Todo: make it O(n) instead of O(n^2) for worst case of "((((((((((A))))))))))"
	for (int i = 0; *pat; pat++, i++)
		exprLen[i]   = (UCHAR)(goToNextPat_uc(pat) - pat);									// 'i' is alwyas equals to (pat - start). Initialize the length of current 
	end = pat;
	
	// For each '(' ')' rules calculate union set if relevant. Like (A(z*)A|BB|CC)
	memset(unionLen,0,sizeof(*unionLen)*(end-start));
	pat = start;
	for (int i = 0; *pat; pat++, i++){
		if ((*pat!='|')||(pat[-1]=='\\'))													// We don't care about non unions.
			unionLen[i] = exprLen[i];
		else if (unionLen[i]==0){															// If we already calculated the length for current union, skip it.
			// We are by definition at the first union. Example: For (AA|BB|CC), We are at |BB|CC). 
			int open;
			for (open = i-1; start + open + exprLen[open] <= pat; open--);					// Go backwards until we find the '(' of the current union. 
			// OK, now start+open points exactly to the '(' that opened a union. Moreover 'pat' points to the first '|'
			const char *next = pat, *cur = start+open;										// Iterate over all the '|' and for each store the length until the next '|'
			while (*next =='|'){
				unionLen[cur-start] = (UCHAR)(next - cur + 1);								// Mark the current '|'
				cur = next;																	// Advance to the next '|' or the terminating ')'
				next = findExpressionEnd_UnionSet_uc(next, '(',')') -1;
			}
			unionLen[cur-start] = (UCHAR)(next - cur + 1);									// MArk for the last '|' the length until the terminating ')'
		} 
	}
	return end;
}

static const tCompiledRegex* compiledRegexPtr = NULL;	// Pointer to the current regex-used. This line ruins multi-threading capabilities. Feel free to change the system architecture to support mutli-threading
static const char*	 		 EOS = NULL;				// Pointer to the end of current processed sample. Same reason as above

/****************************************************************************/
/**************************** Command Handlers ******************************/
/****************************************************************************/
// Main method. Matches pattern to sample string. Returns the number of used characters
// Or NO_MATCH if impossible to match.
// 'endp' is pointer to the end of the pattern.
// Declaration of the main mehtod is needed since it is recursivly called.
static int match(const char* pat, const char* sam, const char* endp);

/**
 * @brief Always returns a successful match for any single character comparison.
 * 
 * This static inline function is part of a regular expression matching system and is used 
 * to handle the '.' character in regex patterns, which matches any single character. 
 * The function ignores the actual characters at 'pat' and 'sam' and always returns a success code.
 *
 * @param pat Pointer to the current position in the regex pattern (not used in this function).
 * @param sam Pointer to the current character in the string being matched (not used in this function).
 * 
 * @return int Always returns 1, indicating a successful match of any single character.
 *
 * The operation of this function is straightforward:
 * - It does not perform any actual comparison between 'pat' and 'sam'.
 * - Regardless of the input, it returns 1, representing a successful match.
 * - This is in line with the behavior of the '.' character in regular expressions, which matches any character.
 *
 * Example Usage:
 *   const char* pattern = "."; // represents any single character
 *   char sampleChar = 'a';
 *   int match = c_any(pattern, &sampleChar);
 *   // match will always be 1 for any sampleChar
 */
// Any char comparison is always true
static inline int c_any(	const char* pat, const char* sam){
	(void)pat;
	(void)sam;
	return 1;
}

/**
 * @brief Compares a single character from a regex pattern with a character from a string.
 * 
 * This static inline function is designed for the simple task of comparing a single character 
 * from a regex pattern ('pat') with a single character from a string ('sam'). It's used in the 
 * context of regular expression processing where more complex patterns are not involved, and a 
 * straightforward character-to-character comparison is needed.
 *
 * @param pat Pointer to the character in the regex pattern to be matched.
 * @param sam Pointer to the character in the string to be compared against the pattern character.
 * 
 * @return int Returns 1 if the characters match, or NO_MATCH if they do not.
 *
 * The function operation is very simple:
 * - It directly compares the character from the pattern with the character from the string.
 * - If the characters are the same, it returns 1, indicating a match.
 * - If the characters are different, it returns NO_MATCH.
 *
 * Example Usage:
 *   const char* pattern = "a";
 *   char sampleChar = 'a';
 *   int match = c_achar(pattern, &sampleChar);
 *   if (match != NO_MATCH) {
 *       // Character matches
 *   } else {
 *       // Character does not match
 *   }
 */
// Single char comparison. Match uses one charactr. Wrong returns NO_MATCH.
static inline int c_achar(const char* pat, const char* sam){
	return (*pat == *sam) ? 1 : NO_MATCH;
}

/**
 * @brief Matches a string segment against a group of alternatives in a regular expression pattern.
 * 
 * This static inline function handles matching a segment of a string ('sam') against a group of 
 * alternatives in a regular expression pattern enclosed within parentheses and separated by '|'. 
 * It iteratively attempts to match each alternative in the group until a successful match is found.
 *
 * @param pat Pointer to the beginning of the regex group pattern (right after the opening parenthesis).
 * @param sam Pointer to the segment of the string to be matched against the group.
 * 
 * @return int The number of characters consumed from 'sam' if a successful match is found for any of the alternatives,
 *         otherwise returns NO_MATCH.
 *
 * The function operates as follows:
 * - Iteratively processes each alternative in the group, delimited by '|'.
 * - For each alternative, it uses the 'match' function to attempt a match against the string segment.
 * - If a match is found for any alternative, it returns the number of characters matched.
 * - Continues to the next alternative if the current one does not match.
 * - Returns NO_MATCH if none of the alternatives in the group match the string segment.
 * 
 * Note: The function assumes 'compiledRegexPtr' is already set and uses 'getExpressionEnd_UnionSet' to find 
 * the end of each alternative in the group.
 *
 * Example Usage:
 *   const char* pattern = "(cat|dog|bird)";
 *   const char* sample = "doghouse";
 *   int matchLength = c_group(pattern, sample);
 *   if (matchLength != NO_MATCH) {
 *       // Process the match
 *   }
 */
static inline int c_group(	const char* pat, const char* sam){
	const char *close = compiledRegexPtr->getExpressionEnd_UnionSet(pat);
    if (!close)  return NO_MATCH;						// Could not match the paretheses. Wrong expresion. Exit
	int nCharsMatched;
	while (close[-1]=='|'){
		nCharsMatched = match(pat+1, sam, close-1);		// +1 and -1 remove the parentheses
		if (nCharsMatched >= 0)
			return nCharsMatched;
		pat   = close-1;								// Advance to the next alternative
		close = compiledRegexPtr->getExpressionEnd_UnionSet(pat);
		if (!close)  return NO_MATCH;					// Could not match the paretheses. Wrong expresion. Exit
	}
    return match(pat+1, sam, close-1);					// Execute the final alternative.
}

/**
 * @brief Matches a string character against predefined regular expression abbreviations.
 * 
 * This static inline function handles the matching of a single character in a string ('sam') against 
 * a set of predefined regular expression abbreviations (e.g., '\d' for digits, '\w' for word characters). 
 * These abbreviations represent common character classes in regex. The function expands these abbreviations 
 * into their full regex form and then performs the match.
 *
 * @param pat Pointer to the regex pattern, positioned at the abbreviation character (e.g., 'd' in '\d').
 * @param sam Pointer to the character in the string to be matched against the abbreviation.
 * 
 * @return int Returns the result of the match function, which is the number of characters matched (usually 1 or 0),
 *         or returns NO_MATCH in case of failure. If the abbreviation is unknown, it falls back to character comparison.
 *
 * The function operates as follows:
 * - Expands the abbreviation (e.g., '\d' to '[0-9]') into a character class pattern.
 * - Uses the 'match' function to perform the actual matching against the expanded pattern.
 * - Handles various standard regex abbreviations like '\d', '\w', '\s', and their negated forms '\D', '\W', '\S'.
 * - If the abbreviation is unknown, it defaults to a basic character comparison.
 * 
 * Note: The function assumes the use of other components like 'endOfString' and 'c_achar' for processing.
 *
 * Example Usage:
 *   const char* pattern = "\\d"; // represents a digit
 *   char sampleChar = '5';
 *   int match = c_extended(pattern, &sampleChar);
 *   if (match != NO_MATCH) {
 *       // Process the match
 *   }
 */
// All possible abbreviations
static inline int c_extended(	const char* pat, const char* sam){
	#define ABB_LENGTH  (32)
    char abbr[ABB_LENGTH] = "";    
    switch (*++pat){
        case 'd':	strcpy_s(abbr, ABB_LENGTH, "[0-9]");			break;	// Digit
        case 'D':   strcpy_s(abbr, ABB_LENGTH, "[^0-9]");			break;  // Non-digit
        case 'x':   strcpy_s(abbr, ABB_LENGTH, "[0-9A-Fa-f]");		break;	// Hex digit
        case 'X':	strcpy_s(abbr, ABB_LENGTH, "[^0-9A-Fa-f]");		break;	// Non Hex
        case 'w':   strcpy_s(abbr, ABB_LENGTH, "[0-9A-Za-z_]");		break;	// Word character
        case 'W':	strcpy_s(abbr, ABB_LENGTH, "[^0-9A-Za-z_]");	break;
        case 'h':   strcpy_s(abbr, ABB_LENGTH, "[0-9A-Za-z]");		break;	// head of word character
        case 'H':	strcpy_s(abbr, ABB_LENGTH, "[^0-9A-Za-z]");		break;
        case 'a':   strcpy_s(abbr, ABB_LENGTH, "[A-Za-z]");			break;	// Alphabetic character
        case 'A':	strcpy_s(abbr, ABB_LENGTH, "[^A-Za-z]");		break;
        case 'l':	strcpy_s(abbr, ABB_LENGTH, "[a-z]");			break;	// Lowercase character
        case 'L':	strcpy_s(abbr, ABB_LENGTH, "[^a-z]");			break;
        case 'u':   strcpy_s(abbr, ABB_LENGTH, "[A-Z]");			break;	// Uppercase character
        case 'U':	strcpy_s(abbr, ABB_LENGTH, "[^A-Z]");			break;
        case 's':   strcpy_s(abbr, ABB_LENGTH, "[ \t\r\n\v\f]");	break;	// Whitespace characters
        case 'S':	strcpy_s(abbr, ABB_LENGTH, "[^ \t\r\n\v\f]");	break;
    }

    if (*abbr)	return match(abbr, sam, endOfString(abbr));
    else		return c_achar(pat,sam);						// Unknown abbreviation. Just assume that it is a character comparison
}

/**
 * @brief Matches a single character in a string against a set of characters or ranges defined in a regex pattern.
 * 
 * This static inline function is designed to handle character class expressions in regex, 
 * where a character in a string is matched against a set of allowed (or disallowed) characters 
 * specified within square brackets in the pattern (e.g., '[a-z]', '[^0-9]'). It supports ranges, 
 * individual characters, and negation.
 *
 * @param pat Pointer to the beginning of the character class pattern segment (just after the opening '[').
 * @param sam Pointer to the character in the string to be matched against the character class.
 * 
 * @return int Returns 1 if the character matches the pattern, or NO_MATCH if it doesn't. 
 *         In case of a negation pattern (e.g., '[^a-z]'), returns 1 if the character does not match the pattern.
 *
 * The function works as follows:
 * - Parses the character class pattern to identify ranges (e.g., 'a-z'), individual characters, 
 *   and the presence of a negation operator ('^') at the start.
 * - Iteratively checks if the character from the string ('sam') matches any of the specified ranges 
 *   or characters in the pattern.
 * - Handles escaped characters (e.g., '\-') within the pattern.
 * - Implements negation logic: if the pattern starts with '^', the match criteria are inverted.
 * 
 * Note: The function assumes 'compiledRegexPtr' is already set to point to the current regular expression object.
 *
 * Example Usage:
 *   const char* pattern = "[a-zA-Z]";
 *   char sampleChar = 'b';
 *   int match = c_option(pattern, &sampleChar);
 *   if (match != NO_MATCH) {
 *       // Process the match
 *   }
 */
// Chose one of the options in []. Like [\\-0-9$_#]
static inline  int c_option(	const char* pat, const char* sam){
    const char *from  = NULL;									// If we have [a-z]		'from' is 'a', 'to' is 'z'
    const char *to	  = NULL;									// If we have [qQ]		'from' is 'q' and 'Q', 'to' is not needed
	const char *close = compiledRegexPtr->getExpressionEnd(pat);// Extract the expression inside the [] parentheses
	pat++; close--;												// +1 and -1 remove the parentheses	
    int negationOp = ((*pat == '^') ? NO_MATCH : 1);			// Check for negation flag. Invert character [^a-z], representing negation operator
	if (negationOp<0)
		pat++;

    while (pat < close){		
        if (*pat == '-' && from){								// Check for range selection. Like 0-9, where we already have the from
			to = pat + 1;										// Find the 'to'
            if (*to == '\\')  to++;								// Comparison with reserved character. like \-  or \*			
			// Test for range			
            if is_int_inRange(*sam,*from,*to)				
                return negationOp;								// We have found a match. If 'not' is active than this is a violation of the pattern			
            pat = to + 1;										// So *sam didn't match the current range, try the next range. Like a-z and A-Z
            continue;
        }

        from = pat;												// Beggining of the pattern. Initialize 'from'
        if (*from == '\\'){ 
			from++; pat++;										// Comparison with reserved character. like \\* or \\?			
		}
		
        if (*sam == *from)
            return negationOp;									// Comparison of single letter. Like [a-ZAB]
        pat++;
    }
    return -negationOp;											// We tested all the options and nothing was mathing.  
}

/**
 * @brief Matches a pattern with multiple occurrences of a character in a string.
 * 
 * This static function attempts to match a part of a regular expression pattern, specifically
 * one that involves multiple occurrences of a character (e.g., '*', '+', '?', '{n}', '{min,max}'), 
 * with a segment of a given string. It calculates the minimum and maximum number of repetitions 
 * allowed for the pattern and tries to match accordingly.
 *
 * @param pat Pointer to the beginning of the regex pattern segment to be matched.
 * @param sam Pointer to the segment of the string against which the pattern is to be matched.
 * @param endp Pointer to the end of the regex pattern segment.
 * 
 * @return int The number of characters consumed from 'sam' if a successful match is found,
 *         otherwise returns NO_MATCH.
 *
 * The function operates as follows:
 * - It calculates the minimum and maximum repetitions allowed for the pattern.
 * - Tries to match the pattern with the string starting from minimum repetitions and going up to the maximum.
 * - If a part of the pattern requires matching the rest of the string, it checks for that condition.
 * - Continues to match further occurrences until it reaches the maximum allowed repetitions or the end of the string.
 * - Returns the number of characters matched if successful, otherwise returns NO_MATCH.
 * 
 * Note: This function is a part of a larger regex matching system and relies on other components like 'compiledRegexPtr'.
 *
 * Example Usage:
 *   const char* pattern = "a{2,4}";
 *   const char* sample = "aaaabc";
 *   int matchLength = c_multi(pattern, sample, pattern + strlen(pattern));
 *   if (matchLength != NO_MATCH) {
 *       // Process the match
 *   }
 */
// Multiple occurence of a character
static inline int c_multi(	const char* pat, const char* sam, const char* endp){
    const Cmd* cmd = get_cmd(*pat);
    int   nCharsMatched;														// How many characters the multi repitition consumed.
    int   nRestCharsMatched = NO_MATCH;											// How many characters the rest of the pattern consumes (if it exists).
    int   nRepitions;															// Counter, how many repititions were made up to now in a loop
    const char* start_sam = sam;
    const char *ends      = EOS;												// Get the end of the sample (stored in cache, instead of recalculation)
	const char *foundMatchAt = NULL;											// We already found a match but want to try and find a longer matching string
	const char *multi    = compiledRegexPtr->getExpressionEnd(pat);				// Multi occurence pattern: {}, *,?,+
    const char *next_pat = compiledRegexPtr->getExpressionEnd(multi);			// The rest of the pattern.

	// Calculate Min/Max numbers of needed occurences
    int min = 0, max = 1;														// Initialization not really needed. Just in case.	
    switch (*multi){
        case '{':  // For range of repetition: {4} or {4-8}
            {
                const char* comma = findFirstCinS(multi, ',');
				const char* rEnd  = findFirstCinS(multi, '}');
				// Read the minimum value
                min = ft_fast_atoi(multi+1);
				// If comma exists inside {} than read also the maximum value
				if (comma){
					if       (comma <  rEnd-1)	max = MAX(ft_fast_atoi(comma + 1),1);	// Read Max, Max must be at least 1;
					else if  (comma == rEnd-1)  max = (1<<30);					// Max does not exists: '{min,}', assume 1 billion is enough. Can use MAX_INT instead. Daniel did not want a dependency on <limits.h>
				}
				else                            max = MAX(min,1);				// No range: Like {4}, Max must be at least 1;
            } 
			break;
        case '+':		min = 1;		max = (1<<30);		break;
        case '?':		min = 0;		max = 1;			break;
        case '*':		min = 0;		max = (1<<30);		break;
    }

	// If (min==0), we first try to match the rest of pattern
	if ((min==0)&&(*next_pat)){
		nRestCharsMatched = match(next_pat, sam, endp);
		if (nRestCharsMatched>=0)			
			foundMatchAt = start_sam + nRestCharsMatched;	// Yes! The rest of the sample string matches the rest of the pattern. But maybe we can do more repititions and still be fine. like '.*b' matched to 'ab' but can also match  'abccqqb'
		// Note: if nRestCharsMatched==0 than the rest of the pattern can be matched to an empty string. Success is guaranteed. Now we want to match as much repititions as we can. Like 'a*b?' was matched to first character of 'aaaz' but can be matched to 'aaa'.
	}

	// OK. We need to take at least one repitiotion. Enter the loop
	nRepitions = 0;
    while (sam < ends){
        nCharsMatched = ((Stand_Func)cmd->f)(pat, sam);									// Find the pattern for the i'th time.
        if (nCharsMatched < 0){			
			// No more repetitions are possible
			if (nRepitions < min) return NO_MATCH;										// We need at least 'min' but failed

			if  (*next_pat){
				// If (nRestCharsMatched < 0)
				//      We have enough iterations but we already know that the rest of pattern can't be matched. If we found a good solution earlier return it. Otherwise no solution for matching
				// Else We have found a good solution right now and no more iterations possible. Return the good solution.
				return (foundMatchAt) ? (int)(foundMatchAt-start_sam) : NO_MATCH; 
			}
			return (int)(sam-start_sam);												// Macth found. Use 'nRepitions'
		}        
        sam += nCharsMatched;															// Found 'i' repitiotions. Advance pointers
        nRepitions++;

        if (nRepitions < min) continue;													// If we still havent reached the minimal amount of repititions than continue to gather more repetitions.
		
		// OK, we have at least 'min' iterations, Time to check the if the rest of the 
		// pattern can be matched. If not we will look for more occurences.
		// Otherwise we will use the current amount of occurences.
        if (*next_pat){
            nRestCharsMatched = match(next_pat, sam, endp);
            if (nRestCharsMatched>=0)
				foundMatchAt = sam + nRestCharsMatched;									// See explanation of the code line 'foundMatchAt = start_sam + nRestCharsMatched;' above
        }
		
	    if (nRepitions == max){															// Check the maximal limit of repititions.
			if  (*next_pat)
				return (foundMatchAt) ? (int)(foundMatchAt-start_sam) : NO_MATCH;		// See explanation for this exact code line above 
			return (int)(sam-start_sam);												// Macth found. Use maximal possible amounts of repitions.
		}
	}

    // None of the iterations yielded a consistent match. We exited the loop due to end of sample string.
    if (nRepitions < min) return NO_MATCH;												// Sample string terminated and we didn't get our minimal amount.

	if ((*next_pat)&&(nRestCharsMatched < 0))
		return (foundMatchAt) ? (int)(foundMatchAt-start_sam) : NO_MATCH;				// Sample string terminated and the rest of the pattern cannot be matched to an empty string. If we found a good solution return it. Otherwise no solution for matching	
    return (int)(sam-start_sam);														// No following patterns that require aditional characters and we got enough iterations. like a*z? on a string of 'aaaa'
}


/**
 * @brief Attempts to match a given regular expression pattern to a string from its beginning.
 * 
 * This function matches a regular expression pattern against the beginning of a string ('sam'). 
 * It handles various regex constructs including standard characters, special characters, and suffixes 
 * like '*', '+', '?', and '{x,y}'. The function returns the number of characters from 'sam' that match 
 * the pattern. A return value of 0 indicates a successful match with zero occurrences (e.g., 'a?' matched to 'bc').
 *
 * @param pat Pointer to the beginning of the regex pattern.
 * @param sam Pointer to the beginning of the string to be matched against the pattern.
 * @param endp Pointer to the end of the regex pattern.
 * 
 * @return int The number of characters consumed from 'sam' if the match is successful, 
 *         otherwise returns NO_MATCH (defined constant) to indicate failure.
 *
 * The function works as follows:
 * - It iteratively processes each part of the pattern, handling suffix expressions and standard characters.
 * - If it encounters a suffix (like '*', '?', '{x,y}', '+'), it applies the respective logic.
 * - For standard characters, it matches them against the corresponding characters in 'sam'.
 * - The function handles invalid or null patterns by returning NO_MATCH.
 * - It also accounts for complex scenarios like nested patterns or recursion (future enhancements mentioned).
 * 
 * Note: The function assumes 'compiledRegexPtr' is already set to point to the current regular expression object.
 *
 * Example Usage:
 *   const char* pattern = "a*b";
 *   const char* sample = "aaab";
 *   int matchLength = match(pattern, sample, pattern + strlen(pattern));
 *   if (matchLength != NO_MATCH) {
 *       // Process the match
 *   }
 */

// Match pattern to the 'sam' string from its beginning. 
// Returns the amount of consumed characters if match was successfull. Otherwise returns NO_MATCH.
// Note: 0 means successfull match. For example 'a?' is matched to 'bc' with zero occurences of 'a'
static int match(const char* pat, const char* sam, const char* endp){
    const Cmd* cmd;
    int  nCharsMatched;
    const char* start_sam = sam, *next_pat;

	if (!pat)
		return NO_MATCH;												// NULL pattern is illegal
    while (pat < endp){		
        next_pat  = compiledRegexPtr->getExpressionEnd(pat);			// Find next pattern to see if it is a suffix like *, {x,y},?,+
		if (next_pat==NULL)
			return NO_MATCH;											// Wrong regular expression. For example '(A)))'
		cmd = get_cmd(*next_pat);										// Check the next command if it is a suffix
		if (isSuffix(cmd)){												// 'cmd' is indeed a suffix. like 'z{3,7}'. Activate {3,7} on pattern 'z'.
			int matchedLen = ((SuffixFunc)cmd->f)(pat, sam, endp);		// Execute the suffix
			return (matchedLen>=0) ? (int)(sam-start_sam) + matchedLen : NO_MATCH;
        }
        else{															// No suffix
			cmd = get_cmd(*pat);
			// if (cmd->attr&TYPE_RECURSION){ To do: Handle the the case of the bug (.*)AB is not matched to 'ZAB' because .* consumes 3 letters
            nCharsMatched = ((Stand_Func)cmd->f)(pat, sam);
            if (nCharsMatched < 0) 
				return NO_MATCH;										// If matching failed return NO_MATCH.
			
            sam += nCharsMatched;										// Advance to next pattern
            pat = next_pat;
        }
    }
    return (int)(sam-start_sam);
}

/**
 * @brief Performs a regular expression search within a given string using a specified pattern.
 * 
 * This function is a high-level API method that facilitates regular expression searching. It compiles the provided 
 * regex pattern and then searches for it within the specified string. The function handles the compilation 
 * of the regex pattern and then delegates the searching to the compiled regex object's search method.
 *
 * @param pattern Pointer to the null-terminated string containing the regex pattern to be compiled and searched.
 * @param sampleString Pointer to the null-terminated string within which the regex search will be performed.
 * @param resLen A pointer to an integer where the length of the matched string will be stored. If no match is found,
 *               the value pointed to by resLen will be set to -1.
 * 
 * @return const char* A pointer to the start of the matched substring in the sample string. If there is no match,
 *         the function returns NULL.
 *
 * The function performs the following steps:
 * - It initializes a 'tCompiledRegex' object.
 * - Compiles the provided regex pattern using this object.
 * - Calls the 'search' method of the compiled regex object to perform the actual search.
 *
 * Usage Example:
 *   const char* sample = "Hello World";
 *   int matchLength;
 *   const char* match = regex_search("World", sample, &matchLength);
 *   if (match) {
 *       // Process the match
 *   }
 */
/****************************************************************************/
/******************************** API methods *******************************/
/****************************************************************************/
const char* regex_search(const char* pattern, const char* sampleString, int* resLen){
	tCompiledRegex builtInCompiledRegex;
	builtInCompiledRegex.compile(pattern);
	return builtInCompiledRegex.search(sampleString, resLen);
}

/**
 * @brief Searches for a match of the compiled regular expression within a given string.
 * 
 * This method performs a regular expression search using the compiled pattern 
 * within a given sample string. The method supports patterns starting with '^', 
 * indicating that the match must start at the beginning of the string. It handles 
 * empty patterns and searches through the entire string for a potential match.
 *
 * @param sampleString The string in which to search for the pattern.
 * @param resLen A pointer to an integer where the length of the matched string will be stored.
 *               If no match is found, the value pointed to by resLen will be set to -1.
 * 
 * @return const char* A pointer to the start of the matched substring in the sample string.
 *         If the pattern is empty, it returns the original sample string.
 *         If there is no match, it returns NULL.
 *
 * The search process involves:
 * - Handling an empty pattern, which is always a zero-length match.
 * - Considering patterns starting with '^', to match only at the beginning of the string.
 * - Iteratively checking for a match at each position in the sample string.
 * - Finally, attempting to match an empty string if no match was found throughout.
 * 
 * Usage Example:
 *   const char* sample = "Hello World";
 *   int matchLength;
 *   const char* match = regex.search(sample, &matchLength);
 *   if (match) {
 *       // Process the match
 *   }
 */
const char* tCompiledRegex::search(const char* sampleString, int* resLen) const{
	compiledRegexPtr	   = this;										// Store 'this' as current regex
	const char* pattern    = start;
	const char* endPattern = end;
	const char* endOfSearch= EOS = endOfString(sampleString);			// When comparint the pattern to sample string we will search the entire sample.
	if (pattern == endPattern){											// Empty pattern is matched with zero length matching		
		*resLen = 0;
		return sampleString;
	}
	
	if (pattern[0]=='^'){												// Check if first characters forces a matching to beggining of the string
		pattern++;														// Skip the '^'
		endOfSearch = sampleString+1;									// Allow match only for first position.
	}

	// Try to match from every possible place in the sample string
    for (;sampleString != endOfSearch; sampleString++){
        *resLen = match(pattern, sampleString, endPattern);
        if (*resLen > 0)												// Note the >0 comparison and not >=. We do not allow empty string match.
            return sampleString;										// Full match was found. Return the current location in sample string.
    }
	
	sampleString = EOS;													// No match was found through the entire search.
	*resLen = match(pattern, sampleString, endPattern);					// Try to match pattern to empty string (since we didn't allow it before).
     return (*resLen>= 0) ? sampleString : NULL;						// Empty sample string was matched to pattern (like a*b*c*) (if==0). Otherwise no match
};
