# segvcatch
This is a crossplatform C++ library designed to convert a hardware exceptions, such as segmentation fault, or floating point errors, into a software language exceptions, which can be handled later with a try/catch construction.

Other words, it's a crossplatform structured exception handling (SEH).

For example, this code is working fine:

```c++
try
{
    *(int*) 0 = 0;
}
catch (std::exception& e)
{
    std::cerr << "Exception catched : " << e.what() << std::endl;
}
```
