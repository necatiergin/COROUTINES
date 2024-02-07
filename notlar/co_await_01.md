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
Bu fonksiyonun çağrıldığı _coroutine_ askıya alınmadan (durdurulmadan) hemen önce çağrılır.
Askıya almayı (geçici olarak) tamamen kapatmak için kullanılabilir. 
_await_ready_ fonksiyonu _true_ döndürürse, askıya alma isteği reddedilmiş demekti. Yani fonksiyonun _true_ değer döndürmesi _coroutine_'i askıya almadan devam etmeye "hazırız" anlamına gelir.<br>
Genellikle, bu fonksiyon yalnızca _false_ değer döndürür ("hayır, herhangi bir askıya alma işleminden kaçınmayın/engellemeyin anlamında").<br> 
Ancak fonksiyonumuz bir koşula bağlı olarak olarak _true_ değer döndürebilir (örneğin, askıya alma bazı verilerin mevcut olmasına bağlıysa).<br>
Geri dönüş türüyle await_suspend(), coroutine'in askıya alınmasını kabul etmeme sinyali de verebilir (true ve false'un burada zıt anlama sahip olduğuna dikkat edin: await_suspend() içinde true döndürerek,
askıya alma kabul edilir). 
await_ready() ile askıya almayı kabul etmemek, programın coroutine'in askıya alınmasını başlatma maliyetinden tasarruf etmesini sağlar.
Bu fonksiyonun içinde, çağrıldığı coroutine'in henüz askıya alınmadığını unutmayın. 
Burada resume() veya destroy() işlevlerini (dolaylı olarak) çağırmayın. 
Bu mantığın burada askıya alınan coroutine için resume() veya destroy() çağrısı yapmadığından emin olduğunuz sürece daha karmaşık iş mantıklarını bile burada çağırabilirsiniz.
<br>

#### _await_suspend (coroutine_handle)_ 
_await_ready()_ fonksiyonu _false_ döndürürse, bu fonksiyon _co_await_'i çalıştıran _coroutine_'in handle'ı ile çağrılır. <br>
Bu fonksiyon bize asenkron çalışmayı başlatma ve görev bittiğinde tetiklenecek bir bildirim için abone olma ve ardından _coroutine_'i devam ettirme fırsatı verir.<br>


#### _await_resume()_
Snucu (veya hatayı) coroutine'e geri paketlemekten sorumlu fonksiyondur. <br>
_await_suspend()_ tarafından başlatılan çalışma sırasında bir hata meydana gelmişse, bu işlev yakalanan hatayı yeniden atabilir veya bir hata kodu döndürebilir. 
Tüm co_await ifadesinin değeri _await_resume()_ fonksiyonunun döndürdüğü değerdir.

