Normal fonksiyonlar çağrıldığında kodlarının tamamı çalıştırılır. Fonksiyonun çalışmasının sonlanması için
- Kodlarının tamamının çalıştırılması _(void fonksiyonlar)_
- bir _return_ deyiminin yürütülmesi
- fonksiyonun kodundan bir _exception_ gönderilmesi yoluyla çıkılması gerekir.

Oysa _coroutine_'ler kodları birden fazla adımda çalışabilen fonksiyonlardır.
Belirli anlarda, bir _coroutine_'in çalışmasını askıya alabiliriz, yani çalışmasını daha sonra tekrar başlatmak üzere durdurabiliriz. (İngilizcede bu anlamda _suspend_ fiili kullanılıyor, isim hali _suspension_) Sonra çalışmasını tekrar devam ettirebiliriz.
Bir _coroutine_ fonksiyonunun çalışmasını örneğin, 
- bir olayı beklemesi gerektiği için,
- yapılacak başka (daha önemli) işler olduğu için,
- koda iletmemiz gereken bir ara sonuç olduğu için, 
daha sonra tekrar başlatmak üzere durdurabiliriz.

Bu nedenle bir _coroutine_'in başlatılması, bir kısmı tamamlanıncaya kadar çalıştırılmak üzere bir fonksiyonun çalışmasının başlatılması anlamına gelir. 
Çağıran fonksiyon ve _coroutine_, iki yürütme yolu arasında gidip gelerek çalışır. 
Burada iki fonksiyonun kodu paralel olarak çalıştırılmamaktadır. Kontrol akışını ayarlayarak adeta ping-pong oynuyoruz:
- Bir fonksiyon, bir _coroutine_'in mevcut kontrol akışını başlatmaya ya da devam ettirmeye _coroutine_'in deyimlerini başlatarak ya da devam ettirerek karar verebilir.
- Daha sonra bir _coroutine_ çalıştığında, _coroutine_ yürütmesini duraklatmaya veya sonlandırmaya kendisi karar verebilir. Bu da _coroutine_'i başlatan veya devam ettiren fonksiyonun kontrol akışına devam ettiği anlamına gelir.

- Bir _coroutine_'in en basit şeklinde, hem ana kontrol akışı hem de _coroutine_'in kontrol akışı aynı _thread_'de çalışır. 
- Birden fazla _thread_ kullanmak zorunda değiliz ve eşzamanlı erişimle _(synchronosied access)_ uğraşmak zorunda değiliz. Ancak, _coroutine_'leri farklı _thread_'lerde çalıştırmak mümkündür. 
- Hatta bir _coroutine_'i daha önce askıya alındığı yerden farklı bir _thread_'de devam ettirebiliriz.
coroutine'ler ortogonal bir özelliktir, ancak birden fazla _thread_ birlikte kullanılabilir.
-bir _coroutine_ kullanmak, arka planda zaman zaman başlattığınız ve devam ettirdiğimiz bir fonksiyona sahip olmak gibidir. 
Bununla birlikte, bir _coroutine_'in ömrü iç içe geçmiş kapsamların ötesine geçtiğinden, bir _coroutine_ aynı zamanda durumunu bir bellekte saklayan ve bununla başa çıkmak için bir API sağlayan bir nesnedir.

Bir fonksiyonun _coroutine_ olarak ele alınabilmesi için tanımmında aşağıdaki anahtar sözcüklerden en az birine sahip olması gerekir.
- co_await
- co_yield
- co_return

Bir _coroutine_ içinde bu anahtar sözcüklerden hiçbirinin gerekli olmaması durumunda, sonunda açıkça bir 
```
co_return; 
```
deyimi yazmamız gerekir.

_Coroutine_'ler bir _stack_ yapısında değiller _(stackless coroutine)_. Bir _coroutine_'i bir _coroutine_ içinden çalıştırıp onu çalıştıran _coroutine_'i _suspend_ etmeden, _suspend_ edemeyiz.<br>
_coroutine_ fonksiyonlar için aşağıdaki kısıtlamalar söz konusudur:
- _main_ fonksiyonu bir _coroutine_ olamaz.
- _variadic_ bir fonksiyon bir _coroutine_ olamaz.
- Sınıfların _constructor_'ları _coroutine_ olamaz.
- Bir sınıfın _destructor_'ı coroutine olamaz.
- _coroutine_ bir _constexpr_ fonksiyon olamaz.
- _coroutine_ bir _consteval_ fonksiyon olamaz.
- _coroutine_'ler _auto return type_  kullanamazlar.
- _coroutine_'ler normal _return_ deyimlerini kullanamazlar.

- [Bir _coroutine_ genellikle çağıran için _coroutine_ arayüzü _(coroutine interface)_ olarak hizmet veren bir nesne döndürür.](https://github.com/necatiergin/COROUTINES/blob/main/notlar/coroutine_interface.md)
_coroutine_'in amacına ve kullanımına bağlı olarak, bu nesne zaman zaman bağlamı askıya alan veya değiştiren çalışan bir görevi, zaman zaman değer üreten bir üreteci veya tembel olarak _(lazily)_ ve talep üzerine bir veya daha fazla değer döndüren bir fabrikayı temsil edebilir.<br>

Önce makro düzeyde (büyük) resme bakalım<br>
 
![relations](https://github.com/necatiergin/COROUTINES/blob/main/notlar/coroutine_relations.png)

Yukarıdaki resmi açıklayalım: <br>

`Caller` ve `Coroutine`, normalde uygulama kodumuzda uygulayacağımız gerçek fonksiyonlardır.<br>
`Return Object`, `Coroutine`'in döndürdüğü türdür ve tipik olarak, örneğin jeneratörler veya asenkron görevler gibi bazı özel kullanım durumları için tasarlanmış genel bir sınıf şablonudur. <br>
`Caller`, `Coroutine`'i devam ettirmek ve `Coroutine`'den gönderilen değerleri almak için `Return Object` ile etkileşime girer.<br> 
`Return Object` genellikle tüm çağrılarını `coroutine handle`'a yönlendirir.<br>
`Coroutine Handle`, `Coroutine State`'e ait olmayan bir _handle_'dır.<br> 
`Coroutine Handle` aracılığıyla `Coroutine State`i devam ettirebilir ve yok edebiliriz. <br>
`Coroutine State`, _coroutine frame_ olarak adlandırılan varlıktır. Opak bir nesnedir, yani boyutunu bilemeyiz ve `handle`'ı kullanmadan başka bir şekilde erişemeyiz. `Coroutine State`'e  erişmenin tek yolu `Coroutine Handle`'dır.<br>
`Coroutine State`, `Coroutine`'i en son askıya alındığı _(durdurulduğu- suspend edildiği)_ yerden devam ettirmek için gerekli olan tüm bilgileri saklar. <br>
`Coroutine State` ayrıca _Promise_ nesnesini de içerir.
`Promise` nesnesi, `Coroutine`'in `co_await`, `co_yield` ve `co_return` ifadeleri aracılığıyla dolaylı olarak iletişim kurduğu varlıktır. <br>
_Değerler_ veya _exception_'lar `Coroutine`'den gönderildiğinde önce `Promise` nesnesine ulaşırlar. <br> 
`Promise` nesnesi, `Coroutine` ile `Caller` arasında bir kanal görevi görür, ancak her ikisinin de `Promise` nesnesine doğrudan erişimi yoktur.<br>

## coroutine frame

Bir _coroutine_ başlatıldığında şunlar olur:<br>
_Coroutine_ için gerekli bütün verilerin tutulması için bir `coroutine frame` oluşturulur. Bu bellek alanı genel olarak _heap_'ten edinilir. Ancak derleyiciler, yapabiliyorlar ise `coroutine frame`'i stack üzerinde de oluşturabilirler. Bu optimizasyon tipik olarak _coroutine_'in yaşam  süresinin çağıran kodun yaşam süresinin içinde kaldığı ve derleyicinin `coroutine frame` için ne kadar bellek alanı gerektiğini hesaplayabileceği durumlarda yapılabilir.
_coroutine_'in bütün parametreleri `coroutine frame`'e kopyalanır. Referansların kopyalanması referans semantiği ile olur. Yani değerler kopyalanmaz. _Coroutine_'in çalışması devam ettiği sürece referanslar geçerli durunmda kalmalıdır. Geçersiz _(dangling)_ referanslar oluşabileceğinden referans parametrelerden genel olarak kaçınmak gerekir. 
_promise_ nesnesi _coroutine frame_ içinde oluşturulur. BU nesnenin görevi coroutine'in durum bilgisini tutmak ve coroutine çalışırken kullanılacak özelleştirme _(customization)_
noktaları sağlamaktır.
Bir _promise_ nesnesi yoluyla _coroutine_'in durumu _(state)_ kontrol edilebilir ve takip edilebilir.
<!-- 
-->

_coroutine_'in gerçekleştirilmesinde önemli rol oynayan iki tür var:

**promise türü**<br>
Bu tür, _coroutine_'in kullanılmasında _customization_ noktaları sunar. Bu tür kullılanarak _coroutine_'in belirli noktalarda nasıl davranacağını belirlenebilir.
Bu türün üye fonksiyonları belirli durumlarda çağrılan _callback_'ler olarak kullanılır. Bu türden bir nesnenin içinde _coroutine_'in çağıran kod ile veri alış verişini sağlamak için değişkenler tutulabilir.

_**std::coroutine_handle<>**_ türü <br>
_coroutine_ başlatıldığında _promise_ sınıfının üye fonksiyonlarından biri kullanılarak _std::coroutine_handle_ türünden bir nesne oluşturulur. <br>
Bu tür _coroutine_'in kontrolü için alt seviyeli bir arayüz sağlar. _coroutine_handle_ nesnesi kullanılarak _coroutine_frame_'de tutulan _promise_type_ nesnesine erişilebilir. _coroutine_handle_ nesnesi kullanılarak şunlar yapılabilir:

_coroutine_'in çalışması devam ettirilebilir: _(resume)_<br>
_coroutine state_ _(coroutine frame)_ _destroy_ edilebilir.<br>
_coroutine_state_' te tutulan _promise_type_ nesnesine erişilebilir.<br>

_coroutine handle_ bir _"callable object"_ olarak kullanılabilir.  
_coroutine handle_ bir _"nullable_" türdür. _nullptr_ değerinde olabilir. 
henüz bir _coroutine frame_'i göstermeyen _coroutine_handle_ değişkeni _nullptr_ değerindedir. 
Sınıfın _operator bool_ işleviyle ya da _nullptr_ sabitiyle karşılaştırma ile bu durum sınanabilir: 

```
if (crt_handle) { 
	//...
}

if (crt_handle != nullptr) {
	//...
}
```

Standart kütüphane _coroutine_handle_ türünü _generic_ bir tür olarak sunar:

```cpp
std::coroutine_handle<>
// ya da
std::coroutine_handle<void>
``` 
ya da 

```cpp
std::coroutine_handle<promise_type>
``` 
_std::coroutine_handle\<void\>_ türünden bir nesneye herhangi bir türden _coroutine_handle_ nesnesi atanabilir. (_void pointer_ türüne diğer _pointer_ türlerinden atama yapılabilmesi gibi.)
_coroutine_handle_ nesnesi fonksiyon çağrı operatörünün operandı olduğunda _coroutine_'in durdurulduğu noktadan çalışmaya tekrar devam etmesini sağlar.<br>

_coroutine'in_ geri dönüş türü olan _coroutine interface_ bu türlere erişim olanağı sunar:
- Nasıl bir _promise_ türü kullanılacak? _promise_ türü tipik olarak bir içsel tür _(nested type)_ yapılır.
- _coroutine_handle_ nesnesi nerede saklanacak? Tipik olarak _coroutine_handle_ sınıfın bir veri elemanı yapılır.
- _coroutine_interface_ türü _coroutine_'in kullanılabilmesi için müşteri kodlara kontrol olanağı veren bir arayüz sağlar.
Standart kütüphanenin sağladığı _coroutine_handle<>_ türü çalışmakta ya da durdurulmuş olan bir _coroutine_'e erişmek için kullanılır. 
Template parametresi olan tür _coroutine_'in  _promise_type_ türüdür. Bu türde çeşitli değerler tutan veri elemanları ya da çeşitli operasyonlar sağlayan üye fonksiyonlar bulunabilir.

_coroutine_handle<>_ türünün üye fonksiyonlarına bakalım: 

- _coroutine_handle<promise_type>::from_promise(prm)_ <br>
_promise_ nesnesinden bir _handle_ oluşturur.

- _CoroHandleType{} <br>_
_default constructor_. Henüz bir _coroutine_ ile ilişkilendirilmemiş bir _handle_ oluşturur.

- _CoroHandleType{nullptr}_<br>
Henüz bir _coroutine_ ile ilişkilendirilmemiş bir _handle_ oluşturur.

- _CoroHandleType{hdl}_ <br> 
Bir _handle_'dan yeni bir _handle_ nesnesini oluşturur. Bu durumda her iki _handle_ da aynı _coroutine_'e işaret etmektedir.

- _hdl = hdl2_ <br>
Bir _handle_'a yeni bir _handle_ nesnesini atar. Bu durumda her iki _handle_ da aynı _coroutine_'e işaret etmektedir.

- _if (hdl)_ <br> 
_handle_'ın bir _coroutine_'i gösterip göstermediğini sınar.

- _==, !=_ <br>
iki _handle_'ın aynı _coroutine_'i gösterip göstermediğini sınar.

- _<, <=, >, >=, <=>_ <br>
_handle_'ların sıralanması için gerekli karşılaştırma işlemlerini sağlar.

- _hdl.resume()_ <br>
_coroutine_'in durdurulduğu noktadan tekrar çalıştırılmasını _(resume)_ sağlar.

- _hdl()_ <br>
_coroutine_'in durdurulduğu noktadan tekrar çalıştırılmasını _(resume)_ sağlar.

- _hdl.done()_ <br>
_coroutine_'in çalışmasının tamamlanmış olup olmadığını sınar. Bu fonksiyon _true_ değer döndürür ise artık ilgili _crt_ için _resume_ fonksiyonu çağrılmamalıdır.

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

_promise_ nesnesi _coroutine_'in durumunu yöneten, _coroutine_'in davranışını belirleyen ve _coroutine_'in durumunun izlenmesine olanak sağlayan nesnedir.
_coroutine_'in yürütülmesi sırasında belirli noktalarda ilgili _promise_ nesnesinin belirli üye fonksiyonları çağrılır. _promise_ arayüzünde _coroutine_'in davranışını belirleyen üye fonksiyonlar bulunur. _coroutine_ kütüphanesinin yazarı, _coroutine_ çağrıldığında ne olacağını, _coroutine_'in çalışması sonlandığunda (doğal yolla ya da işlenmeyen bir _exception_ durumunda) ne olacağını belirler. Ayrıca _coroutine_ içinde bulunan _co_await_ ve _co_yield_ ifadelerinin davranışlarını belirler.
_coroutine_ fonksiyon her çağrıldığında _coroutine frame_ içinde bir _promise_ nesnesi oluşturulur.

Bazı detayları şimdilik göz ardı edersek, bir _coroutine_ çağrıldığında aşağıdaki kodların çalıştığını düşünebiliriz:

1. _operator new_ fonksiyonun çağrılmasıyla _coroutine frame_ için bir bellek alanı elde edilir. (Burada, derleyici koşullar uygunsa dinamik bir bellek alanının kullanılmaması biçiminde bir optimizasyon gerçekleştirebilir.)   
2. Fonksiyon parametreleri _coroutine frame_'e kopyalanır.
3. _promise_type_ türünden _promise_ nesnesinin oluşturulması için _promise_type_ sınıfının ilgili _constructor_'ı çağrılır.
4. _promise_ nesnesinin _get_return_object_ fonksiyonu çağrılarak _coroutine_ fonsiyonun geri dönüş değeri elde edilir.
Sonuç bir yerel değişkende saklanır ve _coroutine_ ilk _suspend_ edildiğinde bu değer çağıran koda iletilir.
5. _promise_ nesnesinin _initial_suspend()_ üye fonksiyonu çağrılır. Bu fonksiyonun geri dönüş değeri _co_await_ operatörünün operandı yapılarak _co_await_ deyimi yürütülür.
6. _co_await promise.initial_suspend()_ ifadesi yürütüldükten sonra sonucu _coroutine_ _resume_ edildiğinde _coroutine_ gövdesine yazılan tüm kodlar çalıştırılır.

Programın akışı _co_return_ deyimine geldiğinde ise şunlar olur:

1. _promise_ nesnesinin _return_void()_ ya da _return_value(<expr>)_ fonksiyonu çağrılır.
2. Tüm yerel değişkenlerin hayatı hayata geldikleri sırayla ters sırada sonlandırılır.
3. _promise_ nesnesinin _final_suspend()_ fonksiyonu çağrılır ve geri dönüş değeri _co_await_ operatörünün operandı yapılır. 

#### _promise_type_ türü
- Bu tür, bir _coroutine_ ile işlem yapmak için belirli _"customization"_ noktalarını tanımlamak ve oluşturmak için kullanılır. 
- Belirli durumlarda çağrılan _call_back_ fonksiyonları tanımlar.
- _coroutine_'in dönüş değerinin nasıl oluşturulacağını veya alınacağını tanımlar (genellikle _coroutine handle_'ın oluşturulmasını da sağlar.)
- _coroutine_'in çalışmasının başında ya da sonunda _suspend_ edilip edilmeyeceğini (durdurulup durdurulmayacağını) belirler.
- _coroutine_'i çağıran kod ile _coroutine_ arasındaki veri alış verişini sağlar.
- İşlenmemiş hata nesneleriyle _(unhandled exception)_ ilgili yapılacakları belirler.

#### _promise_type_'ın implementasyonu 
Derleme zamanında bir hata olmaması için _promise_type_ sınıfının aşağıdaki fonksiyonları tanımlaması gerekmektedir. (çağrıldıkları  sıra ile)  

#### _constructor_
Bir _coroutine_ başlatıldığında _promise_type_ nesnesini oluşturmak için _coroutine frame_ tarafından _promise_ sınıfının _constructor_'ı çağrılır. 
Bu yüzden _promise_type_'ın geçerli olarak çağrılabilir bir _constructor_'a sahip olması gerekir. 
Basit senaryolar için sınıfın _default ctor_'ı yeterli olabilir.
_Constructor_, derleyici tarafından _coroutine_'i bazı argümanlarla başlatmak için kullanılabilir. 
Bunun için, _constructor_ imzasının, çağrıldığında _coroutine_'e aktarılan argümanlarla eşleşmesi gerekir. 
Bu teknik özellikle _coroutine_traits_ tarafından kullanılır. 

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

Tabi bu fonksiyon, _coroutine_ başlatıldığında _suspend_ edilip edilmeyeceği kararının çalışma zamanında değerlendirilen koşullara göre verilmesini de sağlayabilir. 
Yine bu fonksiyon içinde _promise_type_ nesnesinin veri elemanlarına atama yapılabilir.

#### _final_suspend() noexcept;_
Bu fonksiyon _coroutine_'in son kez _suspend_ edilebileceği noktayı belirler ve derleyicinin ürettiği kodda aşağıdaki gibi çağrılır:<br>
_prm_ bir _promise_type_ nesnesi olmak üzere

```cpp
co_await prm.final_suspend();
```
Bu fonksiyon _coroutine frame_ tarafından _coroutine_ gövdesini içine alan _try_ bloğunun dışında ve _return_void()_, _return_value()_ ya da _unhandled_exception()_ fonksiyonları çağrıldıktan sonra çağrılır. Bu yüzden _noexcept_ olmalıdır.
Bu üye fonksiyonun adı biraz yanıltıcı olabilir. Programın akışı _coroutine_'in sonuna ulaştıktan sonra bir kez daha yeniden başlatmaya zorlamak için _std::suspend_never{}_ döndürülebileceği gibi yanlış bir izlenimini verebilir. Ancak, son _suspension_ noktasında gerçekten durdurulmuş bir _coroutine_'i devam ettirmek tanımsız davranıştır.
Bu fonksiyonda durdurulmuş bir _coroutine_ ile yapabileceğiniz tek şey onu _"destroy"_ etmektir. 
Bu nedenle, bu üye fonksiyonun gerçek amacı, üretilen bir sonucu aktarmak, tamamlanmış olduğu sinyalini vermek veya başka bir yerde bir devamı devam ettirmek gibi bazı mantıkları yürütmektir. _coroutine_'lerin mümkün olduğunca bu noktada durdurulacak şekilde yapılandırılması önerilir.
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






