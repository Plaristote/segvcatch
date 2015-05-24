## About ##
This is a crossplatform C++ library  designed to convert a hardware exceptions, such as segmentation fault, or floating point errors, into a software language exceptions, which can be handled later with a try/catch construction.

Other words, it's a crossplatform structured exception handling (SEH).

For example, this code is working fine:

```
try
{
    *(int*) 0 = 0;
}
catch (std::exception& e)
{
    std::cerr << "Exception catched : " << e.what() << std::endl;
}
```

[Here](http://www.visualdata.ru/blog/109-segv-signal.html) is an article about this library in russian.

[Online documentation](http://www.visualdata.ru/ext/segvcatch-doc)

Contributors and testers is wanted to this project. Write to me (navrocky at visualdata ru) about it.