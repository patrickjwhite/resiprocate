#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "DnsUtil.hxx"
#include "Logger.hxx"

#define RESIPROCATE_SUBSYSTEM resip::Subsystem::UTIL

using namespace resip;
using namespace std;

list<Data> 
DnsUtil::lookupARecords(const Data& host)
{
   list<Data> names;

   if (DnsUtil::isIpV4Address(host))
   {
      names.push_back(host);
      return names;
   }

   struct hostent* result=0;
   int ret=0;
   int herrno=0;

#if defined(__linux__)
   struct hostent hostbuf; 
   char buffer[8192];
   ret = gethostbyname_r( host.c_str(), &hostbuf, buffer, sizeof(buffer), &result, &herrno);
   assert (ret != ERANGE);
#elif defined(__QNX__) || defined(__SUNPRO_CC)
   struct hostent hostbuf; 
   char buffer[8192];
   result = gethostbyname_r( host.c_str(), &hostbuf, buffer, sizeof(buffer), &herrno );
#else
#error "need to define some version of gethostbyname for your arch"
#endif
   
   if (ret != 0 || result == 0)
   {
      string msg;
      switch (herrno)
      {
         case HOST_NOT_FOUND:
            msg = "host not found: ";
            break;
         case NO_DATA:
            msg = "no data found for: ";
            break;
         case NO_RECOVERY:
            msg = "no recovery lookup up: ";
            break;
         case TRY_AGAIN:
            msg = "try again: ";
            break;
      }
      msg += host.c_str();
      throw Exception("no dns resolution", __FILE__, __LINE__);
   }
   else
   {
      assert(result);
      assert(result->h_length == 4);
      DebugLog (<< "DNS lookup of " << host << ": canonical name: " << result->h_name);
      char str[256];
      for (char** pptr = result->h_addr_list; *pptr != 0; pptr++)
      {
         inet_ntop(result->h_addrtype, (u_int32_t*)(*pptr), str, sizeof(str));
         names.push_back(str);
      }
      return names;
   }
}
      
Data 
DnsUtil::getHostByAddr(const Data& ipAddress)
{
   if (!DnsUtil::isIpV4Address(ipAddress))
   {
      return ipAddress;
   }
       
#if defined(__GLIBC__)
#endif
   struct in_addr addrStruct;
   int ret = inet_aton(ipAddress.c_str(), &addrStruct);
   if (ret == 0)
   {
      throw Exception("Not a valid ip address.", __FILE__, __LINE__);
   }

   struct hostent h;
   char buf[8192];
   int localErrno;

#if defined(__GLIBC__)
   hostent* pres;
   ret = gethostbyaddr_r (&addrStruct,
                          sizeof(addrStruct),
                          AF_INET,
                          &h,
                          buf,
                          8192,
                          &pres,
                          &localErrno);
#else
#if !defined(WIN32)
   ret = gethostbyaddr_r ((char *)(&addrStruct),
                          sizeof(addrStruct),
                          AF_INET,
                          &h,
                          buf,
                          8192,
                          &localErrno);
#else
#error no implementation for critical function
#endif
#endif      
   if (ret != 0)
   {
      throw Exception("getHostByAddr failed to lookup PTR", __FILE__, __LINE__);
   }
   return Data(h.h_name);
}

Data 
DnsUtil::getLocalHostName()
{
   char buffer[256];
   if (gethostname(buffer,256) == -1)
   {
      throw Exception("Could not get local host name.", __FILE__,__LINE__);
   }
   return buffer;
}

Data
DnsUtil::getLocalIpAddress() 
{
   return lookupARecords(getLocalHostName()).front();
}

bool 
DnsUtil::isIpV4Address(const Data& ipAddress)
{
   // ok, this is fairly monstrous but it works. 
   unsigned int p1,p2,p3,p4;
   int count=0;
   int result = sscanf( ipAddress.c_str(), 
                        "%u.%u.%u.%u%n",
                        &p1, &p2, &p3, &p4, &count );

   if ( (result == 4) && (p1 <= 255) && (p2 <= 255) && (p3 <= 255) && (p4 <= 255) && (count == int(ipAddress.size())) )
   {
      return true;
   }
   else
   {
      return false;
   }
}


Data
DnsUtil::getIpAddress(const struct in_addr& addr)
{
   char str[256];
   inet_ntop(AF_INET, (u_int32_t*)(&addr), str, sizeof(str));
   return Data(str);
}
