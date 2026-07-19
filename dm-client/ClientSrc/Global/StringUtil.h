#pragma once

#include "Global/Globaldefine.h"

typedef vector<string> VString;
typedef list<string>   ListString;
typedef map<DWORD,string> IDStringMap;

/** Utility class for manipulating Strings.  */
class StringUtil
{
public:
	typedef std::ostringstream StrStreamType;

	static void replace(string& str,const string& chars,char to);
	static void replace2(string& str,const string& from,const string to);

	static void replace_all(string& str,const string& old_value,const string& new_value); 
	static void erasechar(string& str,char c);

	// Removes any whitespace characters, be it standard space or TABs and so on.
	static void trim(string& str, bool left = true, bool right = true );
	
	// Removes any whitespace characters, be it standard space or TABs and so on.
	static void trim(char *str);

	// Returns a StringVector that contains all the substrings delimited
	//by the characters in the passed <code>delims</code> argument.
	static VString split(const string& str, const string& delims = "\t\n ", unsigned int maxSplits = 0);

	// Upper-cases all the characters in the string.
	static void toLowerCase( string& str );

	// Lower-cases all the characters in the string.
	static void toUpperCase( string& str );

	// Converts the contents of the string to a Real.
	static float toReal( const string& str );

	// Returns whether the string begins with the pattern passed in.
	static bool startsWith(const string& str, const string& pattern, bool lowerCase = true);

	// Returns whether the string ends with the pattern passed in.
	static bool endsWith(const string& str, const string& pattern, bool lowerCase = true);

	// Method for standardising paths - use forward slashes only, end with slash. 
	static string standardisePath( const string &init);

	// Method for splitting a fully qualified filename into the base name and path. 
	static void splitFilename(const string& qualifiedName, 
		string& outBasename, string& outPath);

	// Simple pattern-matching routine allowing a wildcard pattern. 
	static bool match(const string& str, const string& pattern, bool caseSensitive = true);

	/** Converts a Integer to a string. */
	static string format(char* fmt,...);

	static string toStr(const string& str,int &iStart,char cEndChar = '/');
	static int  toInt(const string& str,int &iStart,char cEndChar = '/');
	static UINT toUInt(const string& str,int &iStart,char cEndChar = '/');
	static float toFloat(const string& str,int &iStart,char cEndChar = '/');
	static int  toInt(const string& val);

	static void AutoCut(string& str,int nMaxLength);
	static void AutoCut(char *str, int nMaxLength);

	static bool isSpace(string& str);
	static bool hasInvalidChar(string& str);

	static string GetCommerInt(int iInput, int iCommer, int iBase);

	static string UrlEncoding(const string &sIn);
	static string Gb2312ToUtf8(const string &sIn);
	static BYTE toHex(const BYTE &x);

	/// Constant blank string, useful for returning by ref where local does not exist
	static const string BLANK;
};

