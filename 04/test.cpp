#include <sstream>
#include <cassert>

#include "serializer.h"

using namespace std;

struct Data {
    bool a, b, c;
    uint64_t d, e, f;
    bool g;
    uint64_t h, i;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c, d, e, f, g, h, i);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c, d, e, f, g, h, i);
    }
        
}; 

int main()
{ 
    Data x = {false, true, true, 100500, 322, 12345678, true, 0, 2147483647};
    Data y = {true, true, false, 1, 2, 3, false, 0, 9};
    assert(x.a != y.a);
    
    std::stringstream str1;
    Serializer ser1(str1);  
    Deserializer deser1(str1);
  
    assert(ser1.save(x) == Error::NoError);
    assert(deser1.load(y) == Error::NoError);
  
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
    assert(x.d == y.d);
    assert(x.e == y.e);
    assert(x.f == y.f);
    assert(x.g == y.g);
    assert(x.h == y.h);
    assert(x.i == y.i);

    //not enough tokens
    std::stringstream str2("");
    std::stringstream str3("true");
    std::stringstream str4("34 false");

    //negative number
    std::stringstream str5("false true true -100500 322 12345678, true 0 2147483647");
    //too large number
    std::stringstream str6("false true true 1005000000000000000000000000000000 322 12345678, true 0 2147483647");

    //invalid string token
    std::stringstream str7("false true qwerty 100500 322 4 true 0 2147483647");

    //wrong order
    std::stringstream str8("false true 100500 false 322 12345678 true 0 2147483647");

    //all OK
    std::stringstream str9("false true true 101 14 28 true 230 19");

    Deserializer deser2(str2);   
    Deserializer deser3(str3);   
    Deserializer deser4(str4);   
    Deserializer deser5(str5);  
    Deserializer deser6(str6); 
    Deserializer deser7(str7);   
    Deserializer deser8(str8);   
    Deserializer deser9(str9);   

    assert(deser2.load(y) == Error::CorruptedArchive);
    assert(deser3.load(y) == Error::CorruptedArchive);
    assert(deser4.load(y) == Error::CorruptedArchive);
    assert(deser5.load(y) == Error::CorruptedArchive);
    assert(deser6.load(y) == Error::CorruptedArchive);
    assert(deser7.load(y) == Error::CorruptedArchive);
    assert(deser8.load(y) == Error::CorruptedArchive);
    assert(deser9.load(y) == Error::NoError);

    assert(y.a == false);
    assert(y.b == true);
    assert(y.c == true);
    assert(y.d == 101);
    assert(y.e == 14);
    assert(y.f == 28);
    assert(y.g == true);
    assert(y.h == 230);
    assert(y.i == 19);

    return 0;
}
