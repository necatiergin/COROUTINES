#### _co_await_ ifadesi ne yapar ve _awaitable_ bir tür ne anlama gelir?
_co_await_ anahtar sözcüğü _unary_ bir operatör olarak görev yapar. 
Yani tek bir operand alır. 
_co_await_'in operandı olan nesnenin bazı gereklilikleri yerine getirmesi gerekir.<br>
_coroutine_ fonksiyonumuzda _co_await_ operatörünü kullandığımızda, bizim için hazır olabilecek veya olmayabilecek bir şeyi beklediğimizi ifade etmiş oluruz.
Eğer beklediğimiz şey hazır değilse, _co_await_ o anda çalışmakta olan _coroutine_'i askıya alır, yani onun çalışmasını durdurur ve kontrolü _coroutine_'i çağıran koda geri verir. 
Asenkron görev tamamlandığında, çağıran fonksiyon, kontrolü başlangıçta görevin bitmesini bekleyen _coroutine_'e geri aktarmalıdır.<br> 
Aşağıdaki gibi bir ifade olsun:

```cpp
co_await X{};
```
Bu kodun derlenebilmesi için _X_'in _awaitable_ bir tür olması gerekir. <br> 
#### awaitable nedir?
Derleyici _co_await_ operatörünün operandı olan ifadeden yola çıkarak bir _awaiter_ nesnesi oluşturmak zorundadır. 
Oluşturamaz ise sentaks hatası olur.
- Derleyici önce _promise_type_ sınıfının _await_transform_ isimli bir fonksiyonu olup olmadığını kontrol eder. 
Eğer _promise_type_ sınıfının _await_transform_ isimli bir fonksiyonu varsa derleyici _co_await_ operatörünün operandı olan ifadeyi bu fonksiyona argüman olarak göndererek bu fonksiyonu çağırır. 
Bu fonksiyonun geri dönüş değerini _awaitable_ olarak kullanır. 
Yoksa _co_await_ operatörünün operandı olan ifade doğrudan _awaitable_ olarak kullanılır.
- Bu kez derleyici buradan elde edilen _awaitable_ olarak kullanılacak sınıfın üye operator _co_await_ fonksiyonunun bulunup bulunmadığına bakar. 
Eğer _awaitable_ sınıfın böyle bir fonsiyonu varsa derleyici bu fonksiyonun geri dönüş değerinden _awaiter_ nesnesini oluşturur. 
Eğer böyle bir üye fonksiyon yok ise derleyici bu kez uygun bir parametreye sahip global _operator co_await_ fonksiyonunun olup olmadığına bakar. 
Varsa bu fonksiyona _awaitable_ nesnesini argüman olarak gönderir. 
Böyle bir fonksiyon yok ise bu durumda derleyici doğrudan _awaitable_ nesnesini kullanır. (bu durumda _awaitable_ nesnesi bir _awaiter_ olmak zorundadır.)

Standart kütüphane bize
_std::suspend_always_ ve _std::suspend_never_ _awaiter_ sınıflarını hazır olarak sunmaktadır. 

#### awaiter nedir?
Aşağıda listelenen üç üye işlevi doğrudan uygulayan veya alternatif olarak bu üye işlevlerle bir nesne üretmek için _co_await()_ operatör fonksiyonunu tanımlayan herhangi bir tür, _awaiter_ bir türdür:

##### _await_ready()_
_await_ready_ fonksiyonu sonucun hazır olup olmadığını _(true)_ veya geçerli _coroutine_'i askıya alıp sonucun hazır olmasını beklemek gerekip gerekmediğini ifade eden _bool_ türden bir değer döndürür. 
Bu fonksiyonun _coroutine_ askıya alınmadan (durdurulmadan) hemen önce çağrılır.<br>
Askıya almayı (geçici olarak) tamamen kapatmak için kullanılabilir. <br>
_await_ready_ fonksiyonu _true_ döndürürse, askıya alma isteği reddedilmiş demektir. 
Yani fonksiyonun _true_ değer döndürmesi _coroutine_'i askıya almadan devam etmeye "hazırız" anlamına gelir.<br>
Genellikle, bu fonksiyon yalnızca _false_ değer döndürür ("hayır, herhangi bir askıya alma işleminden kaçınmayın/engellemeyin anlamında").<br> 
Ancak bu fonksiyonu bir koşula bağlı olarak olarak _true_ değer döndürebilir (örneğin, askıya alma bazı verilerin mevcut olmasına bağlıysa).<br>

[Geri dönüş türüyle _await_suspend()_ fonksiyonu da , coroutine'in askıya alınmasını kabul etmeme sinyali de verebilir (_true_ ve _false_ geri dönüş değerlerinin burada zıt anlama sahip olduğuna dikkat edin: _await_suspend()_ fonksiyonunun _true_ değer döndürmesi ile askıya alma kabul edilmiş olur.] <br>
_await_ready()_ fonksiyonu ile askıya almayı kabul etmemek, programın _coroutine_'in askıya alınmasını başlatma maliyetinden tasarruf edilmesini sağlar.<br>
Bu fonksiyonun kodu çalışırken _coroutine_'in henüz askıya alınmadığını (durdurulmadığını) unutmayın. 
Yani bu fonksiyon içinde _resume()_ ya da destroy() işlevleri (dolaylı olarak) çağrılmamalıdır.
Bu fonksiyon içişnde askıya alınan _coroutine_ için _resume()_ veya _destroy()_ çağrısı yapılmadığından emin olunduğu sürece daha karmaşık işleri gerçekleştirecek fonksiyonlar bile burada çağrılabilir.
<br>

#### _await_suspend (coroutine_handle)_ 
_await_ready()_ fonksiyonu _false_ döndürürse, bu fonksiyon _co_await_'i çalıştıran _coroutine_'in handle'ı ile çağrılır. <br>
Bu fonksiyon bize asenkron çalışmayı başlatma ve görev bittiğinde tetiklenecek bir bildirim için abone olma ve ardından _coroutine_'i devam ettirme fırsatı verir.<br>
ayrıntılı açıklama <br>
Bu fonksiyon, _coroutine_ askıya alındıktan hemen sonra çağrılır. 
Fonksiyon parametresi olan _awaitHdl_ askıya alma işleminin talep edildiği _coroutine_'e erişimi sağlayan _handle_'dır.<br>
Bekleyen _coroutine_'in _handle_'ının türündendir: _std::coroutine_handle<PromiseType>_. <br>
Burada, askıya alınan _coroutine_'i veya bekleyen _coroutine_'i hemen devam ettirmek ve diğerini daha sonra devam ettirmek için zamanlamak da dahil olmak üzere bir sonraki adımda ne yapılacağı belirlenebilir. 
Bunu desteklemek için özel geri dönüş türleri kullanılabilir (bu durum aşağıda ele alınacaktır).<br>
Hatta burada _coroutine_ _destroy_ edilebilir.  Ancak bu durumda _coroutine_'in başka bir yerde kullanılmadığından emin olmanız gerekir (örneğin bir _coroutine_ arayüzünde _done()_ çağrısı yapmak gibi). <br>

#### _await_resume()_
Sonucu (veya hatayı) _coroutine_'e iletmekten sorumlu fonksiyondur. <br>
_await_suspend()_ tarafından başlatılan çalışma sırasında bir hata meydana gelmişse bu işlev yakalanan exception'ı yeniden throw edebilir veya bir hata kodu döndürebilir. 
Tüm _co_await_ ifadesinin değeri _await_resume()_ fonksiyonunun döndürdüğü değerdir.

_await_suspend() _burada anahtar fonksiyondur. 
Bu fonksiyonun parametre değişkeni ve geri dönüş değeri aşağıdaki gibi olabilir:<br>
_await_suspend()_ fonksiyonunun parametresi şunlar olabilir:<br>
_Coroutine_'in kendi handle türü :
_std::coroutine_handle<PrmType>_
Tüm _coroutine handle'ları _için kullanılabilen temel bir tür: _std::coroutine_handle<void>_ (veya sadece _std::coroutine_handle<>_)<br>
Bu durumda, _promise_ nesnesine erişilemez.
Burada parametre için _auto sepcifier_ kullanılarak derleyicinin  tür çıkarımı yapması sağlanabilir.

_await_suspend()_ işlevinin geri dönüş türü ise şunlar olabilir:<br>
_await_suspend()_ içindeki deyimlerin yürütülmesinden sonra sonra askıya alma işlemine devam etmek ve _coroutine_'i çağırana geri dönmek için void. <br>
_bool_ türü : askıya almanın gerçekten gerçekleşip gerçekleşmeyeceğini bildirmek için. Burada false "askıya alma (artık)" anlamına gelir. (_await_ready()_ işlevinin _bool_ dönüş değerlerinin tersidir).<br>
_std::coroutine_handle<>_ türü<br>
yerine başka bir _coroutine_'i devam ettirmek için. <br>
_Bu await_suspend()_ kullanımına simetrik aktarım _(symmetric transfer)_ denir ve daha sonra ayrıntılı olarak ele alacağız.<br>
Bu durumda, bir noop coroutine herhangi bir _coroutine_'i devam ettirmeme sinyali vermek için kullanılabilir (fonksiyonun _false_ döndürmesi ile aynı şekilde).<br>
Ek olarak, aşağıdakilere dikkat edilmelidir:<br>
- Üye fonksiyonlar, _awaiter_'ın değiştirilen bir üyeye sahip olduğu durumlar haricinde genellikle _const_'tır (örneğin, coroutine handle'ını await_suspend() içinde saklayarak yeniden başlatma sırasında kullanılabilir hale getirmek gibi). <br>
- Üye fonksiyonlar genellikle _noexcept_'tir (final_suspend() fonksiyonu noexcept olduğu için zaten bu fonksiyon içinde kullanıma izin vermek için bu gereklidir). <br>
- Üye fonksiyonlar _constexpr_ olabilir. <br>

