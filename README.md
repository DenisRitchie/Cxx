# Native Design Patterns

## Patrones de diseños en código nativo C++

1. *`ServiceLocator`*
   * Helpers
     * *`Optional<TValue>`*
     * *`Reference<TValue>`*
     * TypeTraits Header
   * Tipos
     * *`ServiceLocator Locator`* Definido como una instancia
     * *`ServiceLocator::Default()`* Como singletón seguro para hilos
     * *`ServiceLocator::Value<ServiceType>`* Como instancia estática singletón genérica

2. *`SemanticValue<TBase>`* Polimorfismo sin punteros

3. *`Generator<TValue>`* Usando por el operador ***co_yield***

4. *`auto operator->*(Object, MethodOrPropertyExtender)`* Para simular propiedades/métodos extensores :point_right: [*Referencia C#*](https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/extension-methods)

### Ejemplo: Registrando un servicio

#### `ServiceLocator::Default()`

```cpp
struct IService
{
  virtual std::string_view GetValue() const noexcept = 0;
};

struct Service : public IService
{
  constexpr Service(const std::string_view Value) noexcept
    : Value{ Value }
  {
  }

  virtual std::string_view GetValue() const noexcept override
  {
    return Value;
  }

  std::string_view Value;
};

// Registrar un tipo usando su clase base y derivada

// Forma 1
ServiceLocator::Default().Register<IService>(Service{ "Test" });

// Forma 2
ServiceLocator::Default().Register<IService, Service>("Test");

// Forma 3
ServiceLocator::Default().Register<IService>([] { return Service{ "Test" }; });

// Forma 4
ServiceLocator::Default().InvokeFactory([]() -> SemanticValue<IService> { return Service{ "Test" }; });

// Forma 5
// Usando un CustomType (Objecto Función)
struct ServiceFactory
{
  SemanticValue<IService> operator()() const noexcept
  {
    return Service{ "Test" };
  };
};

ServiceLocator::Default().InvokeFactory<ServiceFactory>();

// Forma 6
// Usando un MethodFactory
SemanticValue<IService> ServiceMethodFactory() noexcept
{
  return Service{ "Test" };
}

ServiceLocator::Default().InvokeFactory(ServiceMethodFactory);

// Forma 7
// Combinando MethodFactory y TypeFactory (Objecto Función)
ServiceLocator::Default().InvokeFactory<TypeFactory1, TypeFactory2, TypeFactory3, TypeFactory4, ...>(MethodFactory1, MethodFactory2, MethodFactory3, MethodFactory4, LambdaFactory1, LambdaFactory2, LambdaFactory3, ...);
```

#### `ServiceLocator::Value<ServiceType>`

```cpp
// Registrar un tipo usando su clase base y derivada

// Forma 1
ServiceLocator::Value<IService> = Service{ "Test" };

// Forma 2
ServiceLocator::UseFactory([]() -> SemanticValue<IService> { return Service{ "Test" }; });

// Forma 3
// Usando un CustomType
ServiceLocator::UseFactory<ServiceFactory>();

// Forma 4
// Usando un MethodFactory
ServiceLocator::UseFactory(ServiceMethodFactory);

// Forma 5
// Combinando MethodFactory y TypeFactory (Objecto Función)
ServiceLocator::UseFactory<TypeFactory1, TypeFactory2, TypeFactory3, TypeFactory4, ...>(MethodFactory1, MethodFactory2, MethodFactory3, MethodFactory4, LambdaFactory1, LambdaFactory2, LambdaFactory3, ...);
```

## ServiceLocator API

```cpp
class ServiceLocator
{
public:
  ServiceLocator()                                 = default;
  ServiceLocator(const ServiceLocator&)            = delete;
  ServiceLocator(ServiceLocator&&)                 = delete;
  ServiceLocator& operator=(const ServiceLocator&) = delete;
  ServiceLocator& operator=(ServiceLocator&&)      = delete;
  ~ServiceLocator()                                = default;

  template <typename ServiceType, typename ServiceValue, typename... Arguments>
  ServiceLocator& Register(Arguments&&... Args);

  template <typename ServiceType, typename ServiceValue>
  ServiceLocator& Register(ServiceValue&& Value);

  template <typename... FactoryTypes, typename... FactoryFunctions>
  ServiceLocator& InvokeFactory(FactoryFunctions&&... Factory);

  template <typename ServiceType, typename Self>
  decltype(auto) GetService(this Self&& This);

  template <typename ServiceType, typename Self>
  Optional<Reference<ServiceType>> Resolve(this Self&& This) noexcept;

public:
  static ServiceLocator& Default() noexcept;

  template <typename ServiceType>
  static inline Optional<SemanticValue<ServiceType>> Value;

  template <typename... FactoryTypes, typename ...FactoryFunctions>
  static inline void UseFactory(FactoryFunctions &&... Factory);
};
```

## SemanticValue API

```cpp
template <typename BaseType>
class SemanticValue
{
public:
  using type            = BaseType;
  using element_type    = BaseType;
  using value_type      = std::remove_cv_t<BaseType>;
  using reference       = element_type&;
  using pointer         = element_type*;
  using const_reference = const element_type&;
  using const_pointer   = const element_type*;

  template <typename DerivedType>
  constexpr SemanticValue(DerivedType&& value) noexcept;

  template <typename DerivedType>
  SemanticValue(const SemanticValue<DerivedType>& other) noexcept;

  template <typename DerivedType>
  SemanticValue(SemanticValue<DerivedType>&& other) noexcept;

  template <typename DerivedType>
  SemanticValue<BaseType>& operator=(const SemanticValue<DerivedType>& other) noexcept;

  template <typename DerivedType>
  SemanticValue<BaseType>& operator=(SemanticValue<DerivedType>&& other) noexcept;

  ~SemanticValue() noexcept = default;

  template <typename Self>
  inline constexpr decltype(auto) operator*(this Self&& self) noexcept;

  template <typename Self>
  inline constexpr decltype(auto) operator->(this Self&& self) noexcept;
};
```

## Optional API

```cpp
template <typename Type>
struct Optional : public std::optional<Type>
{
  using BaseType = std::optional<Type>;

  using BaseType::optional;
  using BaseType::operator=;

  template <typename Self, std::integral... Indexes>
  constexpr auto&& operator[](this Self&& This, const Indexes... Index);

  template <typename Self>
  constexpr auto&& operator*(this Self&& This);

  template <typename Self>
  constexpr auto&& operator->(this Self&& This);
};
```

## Reference API

```cpp
template <typename Type>
class Reference : public std::reference_wrapper<Type>
{
public:
  using ValueType = std::remove_cv_t<Type>;
  using BaseType  = std::reference_wrapper<Type>;

  using BaseType::reference_wrapper;

  template <typename Self>
  constexpr decltype(auto) operator->(this Self&& This) noexcept;
};
```

## Generator API

```cpp
template <typename Type>
class Generator
{
public:
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type
  {
    promise_type& get_return_object();

    std::suspend_always initial_suspend();
    std::suspend_always final_suspend() noexcept;
    std::suspend_always yield_value(Type const& other);

    void return_void();
    void unhandled_exception();
    void rethrow_if_failed();

    template <typename Expression>
    Expression&& await_transform(Expression&& expression);
  };

  struct iterator
  {
    using iterator_category = std::input_iterator_tag;
    using value_type        = Type;
    using difference_type   = ptrdiff_t;
    using pointer           = const Type*;
    using reference         = const Type&;

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;

    iterator  operator++(int) = delete;
    iterator& operator++()

    const Type& operator*() const;
    const Type* operator->() const;
  };

  Generator()                            = default;
  Generator(const Generator&)            = delete;
  Generator& operator=(const Generator&) = delete;

  Generator(promise_type& promise);
  Generator(Generator&& other);
  Generator& operator=(Generator&& other);
  ~Generator();

  iterator begin();
  iterator end();
};
```

## Extension Methods API

```cpp
template <typename ValueType, typename ExtensionType>
constexpr decltype(auto) operator->*(ValueType&& Value, ExtensionType&& Extension);

namespace NativeDesignPatterns::LINQ::MethodSyntax
{
  /*
    inline constexpr PropertyDetails::LengthImplementation Count;
    inline constexpr PropertyDetails::LengthImplementation Size;
    inline constexpr PropertyDetails::LengthImplementation Length;
  */

  // Ejemplo: Read-Only Properties (Count, Size, Length)

  std::string Value = "Denis West";

  std::cout << "Count  Property: " << Value->*Count  << std::endl; // Count  Property: 10
  std::cout << "Size   Property: " << Value->*Size   << std::endl; // Size   Property: 10
  std::cout << "Length Property: " << Value->*Length << std::endl; // Length Property: 10

  ////////////////////////////////////////////////////////////////////////////////////////////////////

  /*
    inline constexpr PropertyDetails::UpperCaseImplementation UpperCase;
    inline constexpr PropertyDetails::LowerCaseImplementation LowerCase;
  */

  // Ejemplo: Read-Only Properties (UpperCase, LowerCase)
  // Nota: UpperCase y LowerCase modifica el objeto actual sin importar que sea constante

  std::string Value = "Denis West";
  std::cout << "Value: " << Value->*LowerCase << std::endl; // Value: denis west
  std::cout << "Value: " << Value->*UpperCase << std::endl; // Value: DENIS WEST

  // La sentencia anterior actualiza el objeto real, en esta caso todas a mayúsculas
  std::cout << "Value: " << Value << std::endl; // Value: DENIS WEST

  std::cout << "Value: " << Value->*LowerCase->*Count  << " Letras" << std::endl; // Value: 10 Letras
  std::cout << "Value: " << Value->*UpperCase->*Length << " Letras" << std::endl; // Value: 10 Letras

  ////////////////////////////////////////////////////////////////////////////////////////////////////

  /*
    inline constexpr PropertyDetails::BooleanStringImplementation BooleanString;
  */

  // Ejemplo: Read-Only Properties (BooleanString)
  // Referencia:
  //  Línea 74: TEST(ExtensionMethodTest, BooleanStringPropertyExtension)
  //  Archivo:  ExtensionMethodTests.cpp

  ////////////////////////////////////////////////////////////////////////////////////////////////////

  /*
    template <std::ranges::range RangeType>
    inline constexpr PropertyDetails::ToImplementation<RangeType> To;

    template <std::ranges::range RangeType>
    auto As();

    template <typename FunctionType>
    auto Select(FunctionType&& Selector);

    template <typename FunctionType>
    auto Map(FunctionType&& Function);

    auto ToString();
  */

  // Ejemplo: Extension Methods (As, Select, Map, ToString)
  // Referencia:
  //  Línea 118: TEST(ExtensionMethodTest, MethodExtension)
  //  Archivo:   ExtensionMethodTests.cpp
}
```
