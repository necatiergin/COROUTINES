_coroutine_'in gerçekleştirilmesinde önemli rol oynayan iki tür var:

- promise türü<br>
Bu tür, _coroutine_'in kullanılmasında _customization_ noktaları sunuyor. 
Bu türü kullanarak _coroutine_'in belirli noktalarda nasıl davranacağını belirlenebilir.
Bu türün üye fonksiyonları belirli durumlarda çağrılan _callback_'ler olarak kullanılır.

- _std::coroutine_handle<>_ türü <br>

_coroutine_ başlatıldığında _promise_ sınıfının üye fonksiyonlarından biri kullanılarak _std::coroutine_handle_ türünden bir nesne oluşturuluyor. <br>
Bu tür _coroutine_'in kontrolü için alt seviyeli bir arayüz sağlar. 
- _coroutine_'in çalışmasının devam ettirilmesi _(resume)_
- _coroutine_'in çalışmasının sonlandırılması.

_coroutine handle_ bir _"callable object"._ Standart kütüphane _coroutine_handle_ türünü _generic_ bir tür olarak sunar:

```cpp
std::coroutine_handle<>
// ya da
std::coroutine_handle<void>
``` 
ya da 

```cpp
std::coroutine_handle<promise_type>
``` 
_coroutine_handle_ nesnesi fonksiyon çağrı operatörünün operandı olduğunda _coroutine_'in durdurulduğu noktadan çalışmaya tekrar devam etmesini sağlar.<br>

_coroutine'in_ geri dönüş türü olan _coroutine interface_ bu türlere erişim olanağı sunar:
- Nasıl bir _promise_ türü kullanılacak? _promise_ türü tipik olarak bir içsel tür _(nested type)_ yapılır.
- _coroutine_handle_ nesnesi nerede saklanacak? Tipik olarak _coroutine_handle_ sınıfın bir veri elemanı yapılır.
- _coroutine_interface_ türü _coroutine_'in kullanılabilmesi için müşteri kodlara kontrol olanağı veren bir arayüz sağlar.
Standart kütüphanenin sağladığı _coroutine_handle<>_ türü çalışmakta ya da durdurulmuş olan bir _coroutine_'e erişmek için kullanılır. Template parametresi olan türü _coroutine_'in  _promise_type_ türü. Bu türde çeşitli değerler tutan veri elemanları ya da çeşitli operasyonlar sağlayan üye fonksiyonlar bulunabilir.

_coroutine_handle<>_ türünün üye fonksiyonlarına bakalım: 

- _coroutine_handle<promise_type>::from_promise(prm)_ <br>
_promise_ nesnesinden bir _handle_ oluşturur.

- _CoroHandleType{} <br>_
_default constructor_. Henüz bir _coroutine_ ile ilişkilendirilmemiş bir _handle_ oluşturur.

- CoroHandleType{nullptr} 
Henüz bir _coroutine_ ile ilişkilendirilmemiş bir _handle_ oluşturur.

- _CoroHandleType{hdl}_ <br> 
Bir _handle_'dan yeni bir _handle_ nesnesini oluşturur. Bu durumda her iki _handle_ da aynı coroutine'e işaret etmektedir.

- _hdl = hdl2_ <br>
Bir _handle_'a yeni bir _handle_ nesnesini atar. Bu durumda her iki _handle_ da aynı _coroutine_'e işaret etmektedir.

- _if (hdl)_ <br> 
_handle_'ın bir _coroutine_'i gösterip göstermediğini sınar.

- _==, !=_ <br>
iki _handle_'ın aynı _coroutine_'i gösterip göstermediğini sınar.

- _<, <=, >, >=, <=>_ <br>
_handle_'ların sıralanması için gerekli karşılaştırma işlemlerini sağlar.

- _hdl.resume()_ <br>
coroutine_'in durdurulduğu noktadan tekrar çalıştırılmasını _(resume)_ sağlar.

- _hdl()_ <br>
_coroutine_'in durdurulduğu noktadan tekrar çalıştırılmasını _(resume)_ sağlar.

- _hdl.done()_ <br>
_coroutine_'in çalışmasının tamamlanmış olup olmadığını sınar. BU fonksiyon _true_ değer döndürür ise artık ilgili _crt_ için _resume_ fonksiyonu çağrılmamalıdır.

- _hdl.destroy()_ <br>
_coroutine_ nesnesini sonlandırır.

- _hdl.promise()_ <br>
_coroutine_'in _promise_ nesnesine eriştirir.

- _hdl.address()_ <br>
_coroutine_ verilerinin tutulduğu içsel adresi döndürür.

- _coroutine_handle<PrmT>::from_address(addr)_ <br>
argüman olan adresle ilişkili bir _handle_ nesnesi oluşturur.

Sınıfın _static_ üye fonksiyonu olan _from_promise()_ bir _coroutine_'den bir _handle_ oluşturması için çağrılır. 
Bu fonksiyon aslında _promise_ nesnesinin adresini _handle_ nesnesi içinde saklar.
_prm_ bir _promise_ nesnesi olmak üzere aşağıdaki gibi çağrılabilir:

```cpp
auto hdl = std::coroutine_handle<decltype(prm)>::from_promise(prm);
```

_from_promise()_ tipik olarak _promise_type_ sınıfının _get_return_object_ fonksiyonu içinde çağrılır:

```cpp
class CoroFace{
public:
    struct promise_type {
        auto get_return_object() 
        { 
            return CoroFace{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        //...
    };
    //...
};
```

_default ctor_. ya da _std::nullptr_t_ parametreli _ctor_. ile oluşturulan bir _handle_ nesnesi boştur, hiçbir _coroutine_'i göstermez. Bu durumdaki bir _handle_ nesnesi için çağrılan _operator bool_ fonksiyonu _false_ değer döndürür.

```cpp
std::coroutine_handle<promise_type> hdl1{};
std::coroutine_handle<promise_type> hdl2{nullptr};

if (hdl) // false
```
_handle_ nesnelerinin birbirine kopyalanması ya da atanması maliyeti düşüktür. 
Kopyalanan _handle_ nesnesinin sarmaladığı bir pointer değişkendir.
Bu yüzden _coroutine_handle_ fonkiyonlara tipik olarak _call by value_ olarak gönderilir.
Kopyalamaya izin verildiği için birden fazla _handle_ aynı _coroutine_'i gösterebilir.
Eğer başka bir _handle_ _coroutine_'i yok etmişse _handle_'ın _resume_ ya da _destroy_ fonksiyonlarını çağırmak tanımsız davranıştır _(dangling pointer)_.<br>
Sınıfın _address()_ fonksiyonu sarmalanan _pointer_ değişkenin değerini _void*_ türden bir adres olarak döndürür. 
Böylece bu fonksiyondan elde ettiğimiz pointer değişkeni başka bir bağlamda kullanarak o bağlamda sınıfın _from_address_ isimli _static_ üye fonksiyonuna çağrı yaparak aynı _coroutine_'i gösteren bir başka _handle_ nesnesi oluşturabiliriz:
```cpp
    auto handle1 = std::coroutine_handle<decltype(prm)>::from_promise(prm);
    //...
    void* handle_ptr = handle.address();
    //...
    auto handle2 = std::coroutine_handle<decltype(prm)>::from_address(handle_ptr);
    handle1 == handle2 // true
```
_coroutine_ yok edildikten sonra bu adres kullanılmamalıdır.

Tüm _coroutine_handle_ türlerinden _std::coroutine<void>_ sınıfı türüne örtülü tür dönüşümü vardır.

```cpp
namespace std {
	template<typename Promise>
	struct coroutine_handle {
	//...
	// implicit conversion to coroutine_handle<void>:
	constexpr operator coroutine_handle<>() const noexcept;
	//...
	};
}
```

İlk standarda göre tüm _coroutine_handle_ türlerinin _std::coroutine<void>_ türünden kalıtım yoluyla elde dilmesi gerekiyordu. Bu durum http://wg21.link/lwg3460 ile değiştirildi.

