_co_await_ ifadesi ne yapar ve _awaitable_ bir tür ne anlama gelir?<br>
_co_await_ anahtar sözcüğü _unary_ bir operatör olarak görev yapar. Yani tek bir operand alır. _co_await_'e verdiğimiz operandın bazı gereklilikleri yerine getirmesi gerekir.<br>
Kodumuzda _co_await_ ifadesini kullandığımızda, bizim için hazır olabilecek veya olmayabilecek bir şeyi beklediğimizi ifade etmiş oluruz.
Eğer beklediğimiz şey hazır değilse, _co_await_ o anda çalışmakta olan _coroutine_'i askıya alır (onun çalışmasını durdurur) ve kontrolü _coroutine_'i çağıran koda geri verir. <br>
Asenkron görev tamamlandığında, çağıran fonksiyon kontrolü başlangıçta görevin bitmesini bekleyen _coroutine_'e geri aktarmalıdır.<br> 
Aşağıdaki gibi bir ifade olsun:

```cpp
co_await X{};
```

Bu kodun derlenebilmesi için _X_'in _awaitable_ bir tür olması gerekir. <br> 
Şimdiye kadar sadece basit _awaitable_ türleri kullandık:<br> 
_std::suspend_always_ ve _std::suspend_never_. 
Aşağıda listelenen üç üye işlevi doğrudan uygulayan veya alternatif olarak bu üye işlevlerle bir nesne üretmek için _co_await()_ operatör fonksiyonunu tanımlayan herhangi bir tür, _awaitable_ bir türdür:

##### _await_ready()_
_await_ready_ fonksiyonu sonucun hazır olup olmadığını _(true)_ veya geçerli _coroutine_'i askıya alıp sonucun hazır olmasını beklemek gerekip gerekmediğini ifade eden _bool_ türden bir değer döndürür.
ayrıntılı açıklama: <br>
Bu fonksiyonun çağrıldığı _coroutine_ askıya alınmadan (durdurulmadan) hemen önce çağrılır.<br>
Askıya almayı (geçici olarak) tamamen kapatmak için kullanılabilir. <br>
_await_ready_ fonksiyonu _true_ döndürürse, askıya alma isteği reddedilmiş demektir. Yani fonksiyonun _true_ değer döndürmesi _coroutine_'i askıya almadan devam etmeye "hazırız" anlamına gelir.<br>
Genellikle, bu fonksiyon yalnızca _false_ değer döndürür ("hayır, herhangi bir askıya alma işleminden kaçınmayın/engellemeyin anlamında").<br> 
Ancak fonksiyonumuz bir koşula bağlı olarak olarak _true_ değer döndürebilir (örneğin, askıya alma bazı verilerin mevcut olmasına bağlıysa).<br>
Geri dönüş türüyle await_suspend() fonksiyonu da , coroutine'in askıya alınmasını kabul etmeme sinyali de verebilir (true ve false'un burada zıt anlama sahip olduğuna dikkat edin: await_suspend() içinde true döndürerek, askıya alma kabul edilir. <br> 
_await_ready()_ fonksiyonu ile askıya almayı kabul etmemek, programın _coroutine_'in askıya alınmasını başlatma maliyetinden tasarruf etmesini sağlar.<br>
Bu fonksiyonun içinde, çağrıldığı coroutine'in henüz askıya alınmadığını unutmayın. 
Bu fonksiyon içinde _resume()_ ya da destroy() işlevleri (dolaylı olarak) çağrılmamalıdır.<br> 
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
Snucu (veya hatayı) coroutine'e geri paketlemekten sorumlu fonksiyondur. <br>
_await_suspend()_ tarafından başlatılan çalışma sırasında bir hata meydana gelmişse, bu işlev yakalanan hatayı yeniden atabilir veya bir hata kodu döndürebilir. 
Tüm co_await ifadesinin değeri _await_resume()_ fonksiyonunun döndürdüğü değerdir.


_await_suspend() _burada anahtar fonksiyondur. Bu fonksiyonun arametre değişkeni ve geri dönüş değeri aşağıdaki gibi olabilir:<br>
_await_suspend()_ fonksiyonunun parametresi şunlar olabilir:<br>
_Coroutine_'in kendi handle türü :
std::coroutine_handle<PrmType>
Tüm coroutine handle'ları için kullanılabilen temel bir tür:
std::coroutine_handle<void> (veya sadece std::coroutine_handle<>)

Bu durumda, promise nesnesine erişilemez.
Burada parametre için auto sepcifier kullanılarak derleyicinin  tür çıkarımı yapması sağlanabilir.
await_suspend() işlevinin geri dönüş türü ise şunlar olabilir:await_suspend() içindeki deyimlerin yürütülmesinden sonra sonra askıya alma işlemine devam etmek ve coroutine'i çağırana geri dönmek için void. <br>
bool türü : askıya almanın gerçekten gerçekleşip gerçekleşmeyeceğini bildirmek için. Burada false "askıya alma (artık)" anlamına gelir (await_ready() işlevinin Boolean dönüş değerlerinin tersidir).<br>
- std::coroutine_handle<> yerine başka bir coroutine'i devam ettirmek için. <br>
Bu await_suspend() kullanımına simetrik aktarım (symmetric transfer) denir ve daha sonra ayrıntılı olarak ele alacağız.<br>
Bu durumda, bir noop coroutine herhangi bir coroutine'i devam ettirmeme sinyali vermek için kullanılabilir (fonksiyonun false döndürmesi ile aynı şekilde).<br>
Ek olarak, aşağıdakilere dikkat edilmelidir:<br>
Üye fonksiyonlar, awaiter'ın değiştirilen bir üyeye sahip olduğu durumlar haricinde genellikle const'tır (örneğin, korutin tanıtıcısını await_suspend() içinde saklayarak yeniden başlatma sırasında kullanılabilir hale getirmek gibi). <br>
Üye fonksiyonlar genellikle noexcept'tir (final_suspend() içinde kullanıma izin vermek için bu gereklidir). <br>
Üye fonksiyonlar _constexpr_ olabilir. <br>

