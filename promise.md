- Bu tür, bir _coroutine_ ile işlem yapmak için belirli _"customization"_ noktalarını tanımlamak için kullanılır. 
- Belirli durumlarda çağrılan _call_back_ fonksiyonları tanımlar.
- _coroutine_'in dönüş değerinin nasıl oluşturulacağını veya alınacağını tanımlar (genellikle _coroutine handle_'ın oluşturulmasını da sağlar.)
- _coroutine_'in çalışmasının başında ya da sonunda _suspend_ edilip edilmeyeceğini (durdurulup durdurulmayacağını) belirler.
- _coroutine_'i çağıran ile _coroutine_ arasındaki veri alış verişini sağlar.
- İşlenmemiş hata nesneleriyle _(unhandled exception)_ ilgili yapılacakları belirler.

#### promise_type'ın implementasyonu 
- Derleme zamanında bir hata olmaması için _promise_type_'ın aşağıdaki fonksiyonarı tanımlaması gerekmektedir. (çağrıldıkları  sıra ile)  

#### constructor
Bir _coroutine_ başlatıldığında _promise_ nesnesini oluşturmak için _coroutine frame_ tarafından _promise_ sınıfının _constructor_'ı çağrılır. _Constructor_, derleyici tarafından _coroutine_'i bazı argümanlarla başlatmak için kullanılabilir. Bunun için, _constructor_ imzasının, çağrıldığında _coroutine_'e aktarılan argümanlarla eşleşmesi gerekir. Bu teknik özellikle _coroutine_traits_ tarafından kullanılır.

#### _get_return_object()_ 
Bu fonksiyon derleyicinin ürettiği kod tarafından _coroutine_ arayüzünü oluşturması için çağrılır. _coroutine arayüz_ nesnesini oluşturur. Oluşturulan nesne _coroutine_'i çağıran koda _coroutine_'in geri dönüş değeri ile iletilir. _coroutine_ arayüz nesnesi tipik olarak _std::coroutine_handle_ sınıfının _static_ üye fonksiyonu olan _from_promise_ tarafından oluşturulur.

#### _initial_suspend();_
Bu fonksiyon, _coroutine_ başlatıldıktan hemen sonra ilk kez _suspend_ edilsin mi sorusunun cevabını veriyor. Derleyicinin ürettiği kod tarafından aşağıdaki gibi çağrılıyor:

```cpp
co_await prm.initial_suspend();
```

Bu yüzden fonksiyonun geri dönüş türü bir _"awaitable"_ tür olmalı.<br>
Eğer _coroutine_'in başlar başlamaz ilk kez _suspend_ edilmesini istiyorsak bu fonksiyonun geri dönüş değeri _std::suspect_always_ olmalı.
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
Eğer _coroutine_'in başlar başlamaz ilk kez _suspend_ edilmesini istemiyorsak bu fonksiyonun geri dönüş değeri _std::suspect_never_ olmalı.

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
``


#### _unhandled_exception()_

#### _return_void();_
_coroutine_ çalışmasının sonucunda çağıran koda bir değer üretmeyecek ise bu fonksiyonun tanımlanması gerekiyor.

#### _return_value()_;
_coroutine_ çalışmasının sonucunda çağıran koda bir değer üretecek ise bu fonksiyonun tanımlanması gerekiyor.




