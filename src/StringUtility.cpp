#include "StringUtility.h"

//------------------------------------------------------------------------------
// std::string Replace(const std::string &str, const std::string &from,
//                     const std::string &to)
//------------------------------------------------------------------------------
/*
 * Replaces all occurenece of <from> string to <to> string.
 *
 */
//------------------------------------------------------------------------------
std::string Replace(const std::string &str, const std::string &from,
                                    const std::string &to)
{
   std::string str1 = str;
   std::string::size_type pos = str1.find(from);

   // if string not found, just return same string
   if (pos == str1.npos)
      return str1;

   // if input string is the same as string to replace, just return <to> string
   if (str == from)
      return to;

   bool done = false;
   std::string::size_type start = 0;

   while (!done)
   {
      pos = str1.find(from, start);
	  
      if (pos != str1.npos)
      {
         str1.replace(pos, from.size(), to);
         start = pos + to.size();
      }
      else
      {
         break;
      }
   }

   return str1;
}

//------------------------------------------------------------------------------
// std::string ToUpper(const std::string &str)
//------------------------------------------------------------------------------
std::string ToUpper(const std::string &str)
{
	int len = str.length();

	if (len == 0)
		return str;

	std::string upper = str;
	for (int i=0; i<len; i++)
		upper[i] = toupper(str[i]);

	return upper;
}

//------------------------------------------------------------------------------
// std::string Trim(const std::string &str, StripType stype = BOTH,
//                  bool removeSemicolon = false, bool removeEol = false)
//------------------------------------------------------------------------------
/*
 * Removs leading or/and trailing blanks.  On option it removes end-of-line first
 * before removing semicoln.
 *
 * @param  str  input string to be trimmed
 * @param  stype  option of trimming front, end, or both (default is BOTH)
 * @param  removeSemicolon  option of removing trailing semicoln
 * @param  removeEol  option of removing end-of-line character first before
 *                    removing semicoln
 */
//------------------------------------------------------------------------------
std::string Trim(const std::string &str, StripType stype/* = BOTH*/,
	bool removeSemicolon/* = false*/, bool removeEol/* = false*/)
{

	if (str == "")
		return str;

	std::string str2;
	std::string whiteSpace = " \t";

	std::string::size_type index1 = str.find_first_not_of(whiteSpace);
	std::string::size_type index2 = str.find_last_not_of(whiteSpace);

	if (index1 == str.npos)
		index1 = 0;

	if (stype == LEADING)
		str2 = str.substr(index1);
	else if (stype == TRAILING)
		str2 = str.substr(0, index2 + 1);
	else if (stype == BOTH)
		str2.assign(str.substr(index1, index2-index1+1));

	// remove trailing end-of-lines (\r or \n) or blanks or tabs (\t)
	// before removing semicolns
	if (removeSemicolon)
	{
		if (str2.size() > 0)
		{
			// if option is to remove eol
			if (removeEol)
			{
				// replace all occurance of tab with a space
				str2 = Replace(str2, "\t", " ");

				// remove trailing \r and \n
				while (str2[str2.size()-1] == '\n' || str2[str2.size()-1] == '\r')
					str2.erase(str2.size()-1, 1);

				// remove trailing blanks
				while (str2[str2.size()-1] == ' ')
					str2.erase(str2.size()-1, 1);
			}

			// remove trailing semicolns
			while (str2[str2.size()-1] == ';')
				str2.erase(str2.size()-1, 1);
		}
	}
	return str2;
}

//------------------------------------------------------------------------------
// bool GetLine(std::istream *is, std::string &line)
//------------------------------------------------------------------------------
/*
 * Reads a platform independent line from the input stream.
 *
 * @param  is    The input stream pointer
 * @param  line  The line read from the input stream
 *
 * @return  true if a line from the input stream was read successfully.
 */
//------------------------------------------------------------------------------
bool GetLine(std::istream *is, std::string &line)
{
   if (is == NULL)
      return false;
   
   char ch;
   std::string result;
   
   while (is->get(ch) && ch != '\r' && ch != '\n' && ch != '\0' &&
          !is->eof()) 
      result += ch;
   
   line = result;
   return true;
}

//------------------------------------------------------------------------------
// std::string RemoveInlineComment(const std::string &str, const std::string &comStr)
//------------------------------------------------------------------------------
/*
 * Removes inline comments from the input string.
 * ex) "Create String srt1; % str2" will return "Create String str1;"
 *
 * @param  str  Input string to remove given enclosnig string
 * @param  cmStr The inline comment string to use for removing
 *
 * @return string with inline comments removed
 */
//------------------------------------------------------------------------------
std::string RemoveInlineComment(const std::string &str,const std::string &cmStr)
{
   std::string str1 = str;
   std::string::size_type index = str.find(cmStr);
   if (index == str1.npos)
      return str1;
   else
      return str1.substr(0, index);
}

//------------------------------------------------------------------------------
// bool StartsWith(const std::string &str, const std::string &value)
//------------------------------------------------------------------------------
/*
 * Returns true if string starts with value and false if it does not.
 */
//------------------------------------------------------------------------------
bool StartsWith(const std::string &str, const std::string &value)
{
   return (str.size() >= value.size()) &&
      (str.substr(0, value.size()) == value);
}

//------------------------------------------------------------------------------
// bool EndsWith(const std::string &str, const std::string &value)
//------------------------------------------------------------------------------
/*
 * Returns true if string ends with value and false if it does not.
 */
//------------------------------------------------------------------------------
bool EndsWith(const std::string &str, const std::string &value)
{
   return (str.size() >= value.size()) &&
          (str.substr(str.size() - value.size(), value.size()) == value);
}

bool IsCommentLine(const std::string &str)
{
	std::string tstr = Trim(str);
	return StartsWith(tstr,"%") || StartsWith(tstr,"#");
}

//------------------------------------------------------------------------------
// std::string RemoveAll(const std::string &str, char ch, Integer start = 0)
//------------------------------------------------------------------------------
std::string RemoveAll(const std::string &str, char ch,int start/* = 0*/)
{
	std::string str2 = str;

	std::string::iterator iter = str2.begin();
	for (int i=0; i<start; i++)
		iter++;

	while (iter != str2.end())
	{
		if (*iter == ch)
			str2.erase(iter);
		else
			++iter;
	}

	return str2;
}


//------------------------------------------------------------------------------
// bool IsBlank(const std::string &text, bool ignoreEol = false)
//------------------------------------------------------------------------------
/*
 * Checks if text has only blank spaces.
 *
 * @param  text  input text
 * @param  ignoreEol  Set this to true if end-of-line char to be ignored
 *
 * @return true if text has only blank spaces
 */
//------------------------------------------------------------------------------
bool IsBlank(const std::string &text, bool ignoreEol/* = false*/)
{
   std::string str = Trim(text, BOTH);

   if (ignoreEol)
   {
      str = RemoveAll(str, '\n');
      str = RemoveAll(str, '\r');
   }

   if (str == "")
      return true;
   else
      return false;
}

/**
读取一行，跳过注释，读到之后返回name和value
*/
bool ReadLine(std::istream* file,std::string& name,std::string& value)
{
	std::string line;
	while(!file->eof()){
		if(GetLine(file,line))	{
			line = Trim(line);
			if(IsCommentLine(line) || IsBlank(line)) continue;
			int index = line.find("=");
			name = Trim(line.substr(0,index));
			value = Trim(line.substr(index+1));
			return 1;
		}
	}

	return 0;
}