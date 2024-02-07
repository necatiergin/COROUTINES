- Bu tür, bir _coroutine_ ile işlem yapmak için belirli _"customization"_ noktalarını tanımlamak için kullanılır. 
- Belirli durumlarda çağrılan _call_back_ fonksiyonları tanımlar.
- _coroutine_'in dönüş değerinin nasıl oluşturulacağını veya alınacağını tanımlar (genellikle _coroutine handle_'ın oluşturulmasını da sağlar.)
- _coroutine_'in çalışmasının başında ya da sonunda _suspend_ edilip edilmeyeceğini (durdurulup durdurulmayacağını) belirler.
- _coroutine_'i çağıran ile _coroutine_ arasındaki veri alış verişini sağlar.
- işlenmemiş hata nesneleriyle _(unhandled exception)_ ilgili yapılacakları belirler.

#### promise_type'ın implementasyonu 
- Derleme zamanında bir hata olmaması için _promise_type_'ın aşağıdaki fonksiyonarı tanımlaması gerekmektedir. (çağrıldıkları  sıra ile)  

#### _get_return_object()_ 

bu fonksiyon derleyicinin ürettiği kod tarafından _coroutine_ arayüzünü oluşturması için çağrılır. _coroutine arayüz_ nesnesini oluşturur. Oluşturulan nesne _coroutine_'i çağıran koda _coroutine_'in geri dönüş değeri ile iletilir. _coroutine_ arayüz nesnesi tipik olarak _std::coroutine_handle_ sınıfının _static_ üye fonksiyonu olan _from_promise_ tarafından oluşturulur.

#### initial_suspend();
bu fonksiyon, _coroutine_ başlatıldıktan hemen sonra ilk kez _suspend_ edilsin mi sorusunun cevabını veriyor. derleyicinin ürettiği kod tarafından aşağıdaki gibi çağrılıyor:

```cpp
co_await prm.initial_suspend();
```

bu yüzden fonksiyonun geri dönüş türü bir _"awaitable"_ tür olmalı.<br>
eğer _coroutine_'in başlar başlamaz ilk kez _suspend_ edilmesini istiyorsak bu fonksiyonun geri dönüş değeri _std::suspect_always_ olmalı.
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
eğer _coroutine_'in başlar başlamaz ilk kez _suspend_ edilmesini istemiyorsak bu fonksiyonun geri dönüş değeri _std::suspect_never_ olmalı.

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

Tabi bu fonksiyon, _coroutine_ başlatıldığında suspend edilip edilmeyeceği kararının çalışma zamanında değerlendirilen koşullara göre verilmesini de sağlayabilir. Yine bu fonksiyon içinde _promise_type_ nesnesinin veri elemanlarına atama yapabiliriz.

#### _final_suspend() noexcept;_

bu fonksiyon _coroutine_'in son kez _suspend_ edilebileceği noktayı belirler ve derleyicinin ürettiği kodda aşağıdaki gibi çağrılır:<br>
_prm_ bir _promise_type_ nesnesi olmak üzere

```cpp
co_await prm.final_suspend();
```
Bu fonksiyon _coroutine frame_ tarafından _coroutine_ gövdesini içine alan _try_ bloğunun dışında ve _return_void()_, _return_value()_ ya da _unhandled_exception()_ fonksiyonları çağrıldıktan sonra çağrılır. Bu yüzden _noexcept_ olmalıdır.

#### _unhandled_exception()_

#### return_void ()
_coroutine_ çalışmasının sonucunda çağıran koda bir değer üretmeyecek ise bu fonksiyonun tanımlanması gerekiyor.

#### return_value()
_coroutine_ çalışmasının sonucunda çağıran koda bir değer üretecek ise bu fonksiyonun tanımlanması gerekiyor.




