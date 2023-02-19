# Linux Lenguaje C/C++ Código Fuentes

* [#include &lt;string.h&gt;](https://codebrowser.dev/linux/linux/include/linux/string.h.html)

  * [strtok_r.c](https://codebrowser.dev/glibc/glibc/string/strtok_r.c.html)
  * [String.c](https://codebrowser.dev/linux/linux/lib/string.c.html)
    * [strncasecmp](https://codebrowser.dev/linux/linux/lib/string.c.html#strncasecmp)
    * [strcasecmp](https://codebrowser.dev/linux/linux/lib/string.c.html#strcasecmp)
    * [strncpy](https://codebrowser.dev/linux/linux/lib/string.c.html#strncpy)
    * [strlcpy](https://codebrowser.dev/linux/linux/lib/string.c.html#strlcpy)
    * [strcat](https://codebrowser.dev/linux/linux/lib/string.c.html#strcat)
    * [strncat](https://codebrowser.dev/linux/linux/lib/string.c.html#strncat)
    * [strcmp](https://codebrowser.dev/linux/linux/lib/string.c.html#strcmp)
    * [strncmp](https://codebrowser.dev/linux/linux/lib/string.c.html#strncmp)
    * [strchr](https://codebrowser.dev/linux/linux/lib/string.c.html#strchr)
    * [strspn](https://codebrowser.dev/linux/linux/lib/string.c.html#strspn)
    * [strcspn](https://codebrowser.dev/linux/linux/lib/string.c.html#strcspn)
    * [strpbrk](https://codebrowser.dev/linux/linux/lib/string.c.html#strpbrk)
    * [strsep](https://codebrowser.dev/linux/linux/lib/string.c.html#strsep)
    * [strstr](https://codebrowser.dev/linux/linux/lib/string.c.html#strstr)
    * [strnstr](https://codebrowser.dev/linux/linux/lib/string.c.html#strnstr)
    * [memset](https://codebrowser.dev/linux/linux/lib/string.c.html#608)
    * [memcpy](https://codebrowser.dev/linux/linux/lib/string.c.html#695)
    * [memmove](https://codebrowser.dev/linux/linux/lib/string.c.html#716)

# Api de Comparación de C++

* [#include &lt;compare&gt;](https://cpprefjp.github.io/reference/compare.html)

  * Objeto función de comparación de 3 vías
    * [std::compare_three_way](https://cpprefjp.github.io/reference/compare/compare_three_way.html)

  * Funciones como sustitutos del operador de comparación de 3 vías en caso de no proveer uno
    * [std::partial_order](https://cpprefjp.github.io/reference/compare/partial_order.html)
    * [std::strong_order](https://cpprefjp.github.io/reference/compare/strong_order.html)
    * [std::weak_order](https://cpprefjp.github.io/reference/compare/weak_order.html)

  * Customization Point Object Types (CPO): Objeto funciones que ejecutan la comparación de 3 vías que el objeto tenga disponible: &lt;=&gt;, std::xxx_order, (&lt;, ==)
    * [std::compare_partial_order_fallback](https://cpprefjp.github.io/reference/compare/compare_partial_order_fallback.html)
    * [std::compare_strong_order_fallback](https://cpprefjp.github.io/reference/compare/compare_strong_order_fallback.html)
    * [std::compare_weak_order_fallback](https://cpprefjp.github.io/reference/compare/compare_weak_order_fallback.html)

  * Conceptos de verificación, si el objeto se puede comparar por las 3 vías
    * [std::three_way_comparable](https://cpprefjp.github.io/reference/compare/three_way_comparable.html)

  * Ragos de tipos
    * [std::compare_three_way_result](https://cpprefjp.github.io/reference/compare/compare_three_way_result.html)
    * [std::common_comparison_category](https://cpprefjp.github.io/reference/compare/common_comparison_category.html)

  * Distintos tipos de retorno que especifican el tipo de comparación realizado por algún objeto
    * [std::partial_ordering](https://cpprefjp.github.io/reference/compare/partial_ordering.html)
    * [std::strong_ordering](https://cpprefjp.github.io/reference/compare/strong_ordering.html)
    * [std::weak_ordering](https://cpprefjp.github.io/reference/compare/weak_ordering.html)

  * Funciones de comparación con nombres
    * [std::is_eq, std::is_neq, std::is_lt, std::is_lteq, std::is_gt, std::is_gteq](https://cpprefjp.github.io/reference/compare/named_comparison_functions.html)

# Conceptos de C++

* [std::boolean_testable](https://cpprefjp.github.io/reference/concepts/boolean-testable.html)

# C++ coroutines

* [C++20 Coroutines](https://www.andreasbuhr.de/files/2020_10_29_AndreasBuhr_IntroductionToCoroutines.pdf)
* [My tutorial and take on C++20 coroutines](https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html)
* [Understanding C# async/await - The Awaitable-Awaiter Pattern](https://weblogs.asp.net/dixin/understanding-c-sharp-async-await-2-awaitable-awaiter-pattern)

* [Asymmetric Transfer](https://lewissbaker.github.io/)
  * [Coroutine Theory](https://lewissbaker.github.io/2017/09/25/coroutine-theory)
  * [C++ Coroutines: Understanding operator co_await](https://lewissbaker.github.io/2017/11/17/understanding-operator-co-await)
  * [C++ Coroutines: Understanding the promise type](https://lewissbaker.github.io/2018/09/05/understanding-the-promise-type)
  * [C++ Coroutines: Understanding Symmetric Transfer](https://lewissbaker.github.io/2020/05/11/understanding_symmetric_transfer)
  * [C++ Coroutines: Understanding the Compiler Transform](https://lewissbaker.github.io/2022/08/27/understanding-the-compiler-transform)

* [PanicSoftware](https://blog.panicsoftware.com/)
  * [Your first coroutine](https://blog.panicsoftware.com/your-first-coroutine/)
  * [co_awaiting coroutines](https://blog.panicsoftware.com/co_awaiting-coroutines/)

* Examples
  * [A C++20 Coroutine Example](https://mariusbancila.ro/blog/2020/06/22/a-cpp20-coroutine-example/)
  * [Continuation](https://github.com/andreaspfaffenbichler/Continuation)
    * [Continuation Wandbox](https://wandbox.org/permlink/OGjmtFWsgjnn3GxX)

# C++ Libraries

* [Proxy: A Polymorphic Programming Library R5](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p0957r5.pdf)
* [Proxy: A Polymorphic Programming Library R6](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p0957r6.pdf)
* [Proxy: A Polymorphic Programming Library R7](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p0957r7.pdf)
* [Proxy: A Polymorphic Programming Library R7](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p0957r8.pdf)
* [Proxy: A Polymorphic Programming Library R9](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p0957r9.pdf)
  * [Proxy: Easy Polymorphism in C++](https://github.com/microsoft/proxy)
  * [[Boost::ext].TE](https://github.com/boost-ext/te)
* [Implementing a better views::split](https://brevzin.github.io/c++/2020/07/06/split-view/)

# Patrones de Diseño

* Type Erasure
  * [Type erasure - Part I](https://akrzemi1.wordpress.com/2013/11/18/type-erasure-part-i/)
  * [Type erasure - Part II](https://akrzemi1.wordpress.com/2013/12/06/type-erasure-part-ii/)
  * [Type erasure - Part III](https://akrzemi1.wordpress.com/2013/12/11/type-erasure-part-iii/)
  * [Type erasure - Part IV](https://akrzemi1.wordpress.com/2014/01/13/type-erasure-part-iv/)

* [Customization Point Functions](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1292r0.html)
  * [Suggested Design for Customization Points](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html)

# Aspect-Oriented Programming

* [Aspect-Oriented Programming with the RealProxy Class](https://learn.microsoft.com/en-us/archive/msdn-magazine/2014/february/aspect-oriented-programming-aspect-oriented-programming-with-the-realproxy-class)

# Guías de Arquitectura

* [Arquitectura de aplicaciones de ASP.NET Core](https://dotnet.microsoft.com/es-es/learn/aspnet/architecture#ebook-grpc-for-wcf-devs-swimlane)
* [Guías de arquitectura de .NET](https://dotnet.microsoft.com/es-es/learn/dotnet/architecture-guides)
* [Guía de arquitectura de microservicios de .NET](https://dotnet.microsoft.com/es-es/learn/aspnet/microservices-architecture)
* [DevOps para aplicaciones .NET](https://dotnet.microsoft.com/es-es/learn/aspnet/devops)
* [Libros electrónicos de .NET y Azure](https://dotnet.microsoft.com/es-es/learn/azure/architecture)

# Locale Names, Languages, And Country/Region Strings

* [UCRT Locale names, Languages, and Country/Region strings](https://learn.microsoft.com/en-us/cpp/c-runtime-library/locale-names-languages-and-country-region-strings?view=msvc-170)
* [setlocale, _wsetlocale](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-170)
