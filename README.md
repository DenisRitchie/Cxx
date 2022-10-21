# Native Design Patterns

## Patrones de diseños en código nativo C++

1. Service Locator
   * Helpers
     * Optional
     * Reference
     * TypeTraits
   * Tipos
     * `ServiceLocator Locator;` Definido como una instancia
     * `ServiceLocator::Default();` Como singletón seguro para hilos
     * `ServiceLocator::Value<ServiceType>;` Como instancia estática singletón genérica

2. Semantic Value `std::any` Polimorfismo sin punteros

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
