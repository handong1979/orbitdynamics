#pragma once

#include <string>
#include "Config.h"

//------------------------------------------------------------------------------
// std::string Replace(const std::string &str, const std::string &from,
//                     const std::string &to)
//------------------------------------------------------------------------------
/*
 * Replaces all occurenece of <from> string to <to> string.
 *
 */
//------------------------------------------------------------------------------
ORBITDYN_API std::string Replace(const std::string &str, const std::string &from,
                                    const std::string &to);

//------------------------------------------------------------------------------
// std::string ToUpper(const std::string &str)
//------------------------------------------------------------------------------
ORBITDYN_API std::string ToUpper(const std::string &str);

enum StripType
{
	LEADING = 1,
	TRAILING = 2,
	BOTH = 3,
};

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
ORBITDYN_API std::string Trim(const std::string &str, StripType stype = BOTH,
	bool removeSemicolon = false, bool removeEol = false);

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
ORBITDYN_API bool GetLine(std::istream *is, std::string &line);

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
ORBITDYN_API std::string RemoveInlineComment(const std::string &str,const std::string &cmStr);

//------------------------------------------------------------------------------
// bool StartsWith(const std::string &str, const std::string &value)
//------------------------------------------------------------------------------
/*
 * Returns true if string starts with value and false if it does not.
 */
//------------------------------------------------------------------------------
ORBITDYN_API bool StartsWith(const std::string &str, const std::string &value);

//------------------------------------------------------------------------------
// bool EndsWith(const std::string &str, const std::string &value)
//------------------------------------------------------------------------------
/*
 * Returns true if string ends with value and false if it does not.
 */
//------------------------------------------------------------------------------
ORBITDYN_API bool EndsWith(const std::string &str, const std::string &value);

ORBITDYN_API bool IsCommentLine(const std::string &str);

//------------------------------------------------------------------------------
// std::string RemoveAll(const std::string &str, char ch, Integer start = 0)
//------------------------------------------------------------------------------
ORBITDYN_API std::string RemoveAll(const std::string &str, char ch,int start = 0);


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
ORBITDYN_API bool IsBlank(const std::string &text, bool ignoreEol = false);

/**
读取一行，跳过注释，读到之后返回name和value
*/
ORBITDYN_API bool ReadLine(std::istream* file,std::string& name,std::string& value);


//////////////////////////////////////////////////////////////////////////
// 读取设置文件
#define BEGIN_READ(filename)	{fstream file(filename,ios::in);      \
								if(!file.is_open())                   \
									throw (string("Can't open file") + filename);      \
								std::string name,value;                \
								while(!file.eof()){                    \
									if(ReadLine(&file,name,value))	{
#define READVAR(var,format)		if(name == #var){                      \
									sscanf(value.c_str(),format,&var); \
								}else
#define READPARAM2(var,P1,P2)    if(name == #var){                      \
									std::stringstream sline(value); 	\
									sline >> P1 >> P2;					\
								}else
#define READPARAM3(var,P1,P2,P3) if(name == #var){                      \
									std::stringstream sline(value); 	\
									sline >> P1 >> P2 >> P3;            \
								}else
#define READPARAM4(var,P1,P2,P3,P4) if(name == #var){                   \
									std::stringstream sline(value); 	\
										sline >> P1 >> P2 >> P3 >> P4;  \
									}else
#define READPARAM5(var,P1,P2,P3,P4,P5) if(name == #var){                  \
										std::stringstream sline(value);   \
											sline >> P1 >> P2 >> P3 >> P4 >> P5;\
										}else

#define READPARAM6(var,P1,P2,P3,P4,P5,P6) if(name == #var){                  \
										std::stringstream sline(value);   \
											sline >> P1 >> P2 >> P3 >> P4 >> P5 >> P6;\
										}else
#define READSTR(str)			if(name == #str){                       \
										str=value;                      \
									}else
#define END_READ               cerr << "can't fine name " << name << endl;  \
								}} file.close();}