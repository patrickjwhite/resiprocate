#include <sipstack/ParserCategory.hxx>
#include <sipstack/HeaderFieldValue.hxx>
#include <sipstack/UnknownParameter.hxx>
#include <iostream>

using namespace Vocal2;

ParserCategory::ParserCategory()
   : mHeaderField(new HeaderFieldValue),
     mIsParsed(true)
{}

ParserCategory::ParserCategory(const ParserCategory& rhs)
   : mIsParsed(rhs.mIsParsed)
{
   if (rhs.mHeaderField)
   {
      mHeaderField = new HeaderFieldValue(*rhs.mHeaderField);
   }
}

ParserCategory*
ParserCategory::clone(HeaderFieldValue* hfv) const
{
   ParserCategory* ncthis = const_cast<ParserCategory*>(this);
   
   HeaderFieldValue* old = ncthis->mHeaderField;
   // suppress the HeaderFieldValue copy
   ncthis->mHeaderField = 0;
   ParserCategory* pc = clone();
   ncthis->mHeaderField = old;
   // give the clone the 
   pc->mHeaderField = hfv;

   return pc;
}

UnknownParameter&
ParserCategory::operator[](const Data& param)
{
   checkParsed();
   return *mHeaderField->get(param);
}

void
ParserCategory::parseParameters(const char* start)
{
   mHeaderField->parseParameters(start);
}

std::ostream&
Vocal2::operator<<(std::ostream& stream, const ParserCategory& category)
{
   return category.encode(stream);
}

