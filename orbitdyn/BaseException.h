//------------------------------------------------------------------------------
//                              BaseException
//------------------------------------------------------------------------------
/**
 * This class provides base exception class, from which all exceptions must 
 * be derived.
 */
//------------------------------------------------------------------------------
#ifndef BaseException_hpp
#define BaseException_hpp

#define USE_STD_EXCEPTION 0

#if USE_STD_EXCEPTION
#include <exception>
#endif //USE_STD_EXCEPTION

#include "Config.h"
#include <string>

#pragma warning(disable:4251)

#if USE_STD_EXCEPTION
class ORBITDYN_API BaseException : public std::exception
#else
class ORBITDYN_API BaseException
#endif //USE_STD_EXCEPTION
{
public:
   virtual std::string GetFullMessage() const 
      {return theMessage + theDetails;};
   virtual std::string GetDetails() const 
      {return theDetails;};
   virtual void SetMessage(const std::string &message)  
      {theMessage = message;};
   virtual void SetDetails(const std::string &details)  
      {theDetails = details;};
   const BaseException& operator=(const std::string &newMessage) 
      {theMessage = newMessage;  return *this;};

//#if USE_STD_EXCEPTION
	const char * what() const
	{ return (theMessage + theDetails).c_str();   }
//#endif //USE_STD_EXCEPTION

   virtual void SetDetails(const char *details, ...);
#if _MSC_VER > 1300   
   static const int MAX_MESSAGE_LENGTH = 3000;
#else
	static const int MAX_MESSAGE_LENGTH;
#endif // _MSC_VER
   
protected:
   BaseException(const std::string& message = "", const std::string &details = "") 
      {theMessage = message; theDetails = details;};
   BaseException(const BaseException& baseException) 
      {theMessage = baseException.theMessage; theDetails = baseException.theDetails;};
   virtual ~BaseException() 
      {};
   const BaseException& operator=(const BaseException& baseException) 
      {theMessage = baseException.theMessage; theDetails = baseException.theDetails;
      return *this;};
   
private:
   std::string theMessage;
   std::string theDetails;
};

#endif // BaseException_hpp

