_promise_ nesnesi _coroutine_'in durumunu yöneten, _coroutine_'in davranışını belirleyen ve _coroutine_'in durumunun izlenmesine olanak sağlayan nesnedir.
_coroutine_'in yürütülmesi sırasında belirli noktalarda ilgili _promise_ nesnesinin belirli üye fonksiyonları çağrılır. _promise_ arayüzünde _coroutine_'in davranışını özelleştiren üye fonksiyonlar bulunur. _coroutine_ kütüphanesinin yazarı, _coroutine_ çağrıldığında ne olacağını, _coroutine_'in çalışması sonlandığunda (doğal yolla ya da işlenmeyen bir _exception_ durumunda) ne olacağını belirler. Ayrıca _coroutine_ içinde bulunan _co_await_ ve _co_yield_ ifadelerinin davranışlarını özelleştirir.
_coroutine_ fonksiyon her çağrıldığında _coroutine frame_ içinde bir _promise_ nesnesi oluşturulur.

#### _promise_type_ türü
- Bu tür, bir _coroutine_ ile işlem yapmak için belirli _"customization"_ noktalarını tanımlamak için kullanılır. 
- Belirli durumlarda çağrılan _call_back_ fonksiyonları tanımlar.
- _coroutine_'in dönüş değerinin nasıl oluşturulacağını veya alınacağını tanımlar (genellikle _coroutine handle_'ın oluşturulmasını da sağlar.)
- _coroutine_'in çalışmasının başında ya da sonunda _suspend_ edilip edilmeyeceğini (durdurulup durdurulmayacağını) belirler.
- _coroutine_'i çağıran ile _coroutine_ arasındaki veri alış verişini sağlar.
- İşlenmemiş hata nesneleriyle _(unhandled exception)_ ilgili yapılacakları belirler.

#### _promise_type_'ın implementasyonu 
Derleme zamanında bir hata olmaması için _promise_type_'ın aşağıdaki fonksiyonları tanımlaması gerekmektedir. (çağrıldıkları  sıra ile)  

#### _constructor_
Bir _coroutine_ başlatıldığında _promise_ nesnesini oluşturmak için _coroutine frame_ tarafından _promise_ sınıfının _constructor_'ı çağrılır. 
Bu yüzden _promise_type_'ın geçerli olarak çağrılabilir bir _constructor_'a sahip olması gerekir. Basit senaryolar için sınıfın _default ctor_'ı yeterli olabilir.
_Constructor_, derleyici tarafından _coroutine_'i bazı argümanlarla başlatmak için kullanılabilir. Bunun için, _constructor_ imzasının, çağrıldığında _coroutine_'e aktarılan argümanlarla eşleşmesi gerekir. Bu teknik özellikle _coroutine_traits_ tarafından kullanılır. 



#### _get_return_object()_ 
Bu fonksiyon derleyicinin ürettiği kod tarafından _coroutine_ arayüzünü oluşturması için çağrılır. 
Derleyicinin ürettiği kodda bu fonksiyonun çağrılmasıyla _coroutine arayüz_ nesnesi oluşturulur. 
Oluşturulan nesne _coroutine_'i çağıran koda _coroutine_'in geri dönüş değeri ile iletilir. 
_coroutine_ arayüz nesnesi tipik olarak _std::coroutine_handle_ sınıfının _static_ üye fonksiyonu olan _from_promise_ tarafından oluşturulur.

#### _initial_suspend()_
Bu fonksiyon, _coroutine_ başlatıldıktan hemen sonra ilk kez _suspend_ edilsin mi sorusunun cevabını verir. 
Derleyicinin ürettiği kod tarafından, _prm_ promise nesnesi olmak üzere, aşağıdaki gibi çağrılır:

```cpp
co_await prm.initial_suspend();
```

Bu yüzden fonksiyonun geri dönüş türü bir _"awaitable"_ tür olmalıdır.<br>
Eğer _coroutine_'in başlar başlamaz ilk kez _suspend_ edilmesini isteniyorsa bu fonksiyonun geri dönüş değeri _std::suspect_always_ olmalıdır.

```cpp
struct promise_type {
	//...
	auto initial_suspend() 
	{ 
		return std::suspend_always{}; 
	}
	//...
}
```
Eğer _coroutine_'in başlar başlamaz ilk kez _suspend_ edilmesi istenmiyor ise bu fonksiyonun geri dönüş değeri _std::suspect_never_ olmalıdır.

```cpp
struct promise_type {
	//...
	auto initial_suspend() 
	{ 
		return std::suspend_never{}; 
	}
	//...
}
```

Tabi bu fonksiyon, _coroutine_ başlatıldığında _suspend_ edilip edilmeyeceği kararının çalışma zamanında değerlendirilen koşullara göre verilmesini de sağlayabilir. Yine bu fonksiyon içinde _promise_type_ nesnesinin veri elemanlarına atama yapılabilir.

#### _final_suspend() noexcept;_
Bu fonksiyon _coroutine_'in son kez _suspend_ edilebileceği noktayı belirler ve derleyicinin ürettiği kodda aşağıdaki gibi çağrılır:<br>
_prm_ bir _promise_type_ nesnesi olmak üzere

```cpp
co_await prm.final_suspend();
```
Bu fonksiyon _coroutine frame_ tarafından _coroutine_ gövdesini içine alan _try_ bloğunun dışında ve _return_void()_, _return_value()_ ya da _unhandled_exception()_ fonksiyonları çağrıldıktan sonra çağrılır. Bu yüzden _noexcept_ olmalıdır.
Bu üye fonksiyonun adı biraz yanıltıcı olabilir. Programın akışı _coroutine_'in sonuna ulaştıktan sonra bir kez daha yeniden başlatmaya zorlamak için _std::suspend_never{}_ döndürülebileceği gibi yanlış bir izlenimini verebilir. Ancak, son _suspension_ noktasında gerçekten durdurulmuş bir _coroutine_'i devam ettirmek tanımsız davranıştır.
Bu fonksiyonda durdurulmuş bir _coroutine_ ile yapabileceğiniz tek şey onu _"destroy"_ etmektir. Bu nedenle, bu üye fonksiyonun gerçek amacı, üretilen bir sonucu aktarmak, tamamlanmış olduğu sinyalini vermek veya başka bir yerde bir devamı devam ettirmek gibi bazı mantıkları yürütmektir. _coroutine_'lerin mümkün olduğunca bu noktada durdurulacak şekilde yapılandırılması önerilir.
<!-- //Bunun bir nedeni, derleyicinin coroutine çerçevesinin yaşam süresinin coroutine'i çağıranın içinde ne zaman iç içe geçtiğini belirlemesini çok daha kolay hale getirmesidir; bu da derleyicinin coroutine çerçevesinin yığın bellek tahsisini atlama olasılığını artırır. -->
Bu nedenle, aksi yönde bir karar almak için bir nedeniniz yoksa, _final_suspend()_ fonksiyonu her zaman _std::suspend_always{}_ döndürmelidir. Örneğin:

```cpp
struct promise_type {
	//...
	auto final_suspend() noexcept 
	{ 
		//...
		return std::suspend_always{};
	}
};
```
#### _unhandled_exception()_
_co_yield_ veya _co_return_'ün kullanılıp kullanılmadığına ve nasıl kullanıldığına bağlı olarak, aşağıdaki fonksiyonlardan bazılarının _promise_ sınıfı tarafından tanımlanması gerekir. _coroutin_'in sonunda çağrılmak üzere aşağıdaki fonksiyonlardan birinin tanımlanması gerekir:

#### _return_void();_
_coroutine_ çalışmasının sonucunda çağıran koda bir değer üretmeyecek ise bu fonksiyonun tanımlanması gerekir. _coroutine_ sona ulaştığında (gövdesinin sonuna ulaştığında ya da argümansız bir _co_return_ ifadesine ulaştığında) çağrılır.

#### _return_value()_;
_coroutine_ çalışmasının sonucunda çağıran koda bir değer üretecek ise bu fonksiyonun tanımlanması gerekir. _coroutine_ bir argümanlı bir _co_return_ ifadesine ulaşırsa çağrılır. Aktarılan argüman belirtilen türde olmalı veya bu türe dönüştürülmelidir.
_coroutine_'nin geri bir noktada geri dönüş değeri üretmesi fakat bir başka sonlanma noktasında geri dönüş değeri üretmemesi tanımsız davranıştır.

```cpp
ResultType corofunc(/* */ )
{
	if ( /* */ ) {
		co_return 42;
	}
}
```

Yukarıdaki _coroutine_ geçerli değildir. 
Hem _return_void()_ hem de _return_value()_ fonksiyonunun çağrılması doğru değildir. üstelik 
```cpp
return_value(int);
```
gibi bir fonksiyonun tanımlanması durumunda derleyici yukarıdaki kod için bir bir uyarı mesajı da vermeyebilir.

Farklı imzaya sahip birden fazla _return_value_ fonksiyonu tanımlanabilir yani _return_value_ fonksiyonu _overload_ edilebilir. Hatta _return_value_ fonksiyonu _generic_ olarak da tanımlanabilir.

```cpp
struct promise_type {
	//...
	void return_value(int val) 
	{ 
		//...	
	}

	void return_value(std::string val) 
	{ 
		//...
	}
};
```

Bu durumda _coroutine_ birden fazla ve farklı türden ifadelere sahip _co_return statement_'a sahip olabilir.

```cpp
ReturnType corofunc()
{
	int val{0};
	if ( /*...*/ ) 
	{
		co_return "error";
	}
	
	co_return val;
}
```
#### _yield_value(Type)_
_Coroutine_ içinde programın akışı bir _co_yield_ deyimine geldiğinde _yield_value_ fonksiyonu çağrılır. Bu fonksiyonun geri dönüş değeri _co_await_ operatörünün operandı yapılır:

```cpp
struct promise_type {
	//...
	auto yield_value(int val) 
	{ 
		return std::suspend_always{}; // - suspend coroutine
	}

	auto yield_value(std::string val) 
	{ 
		return std::suspend_always{}; // - suspend coroutine
	}
};
```

<!--
Promises can also be used to define some optional operations that define special behavior of coroutines, where normally some default behavior is used.
await_transform()
await_transform() can be defined to map values from co_await to awaiters.
operator new() and operator delete()
operator new() and operator delete() allow programmers to define a different way memory is allocated for the coroutine state.
These functions may also be used to ensure that coroutines do not accidentally use heap memory.
get_return_object_on_allocation_failure()
get_return_object_on_allocation_failure() allows programmers to define how to react to exceptionless failures of memory allocation for coroutines.
-->



