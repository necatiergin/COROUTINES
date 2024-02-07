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

_await_ready()_, sonucun hazır olup olmadığını (true) veya geçerli coroutine'i askıya alıp sonucun hazır olmasını beklemek gerekip gerekmediğini ifade eden bool türden bir değer döndürür.<br>
await_suspend (coroutine_handle) - await_ready() fonksiyonu false döndürürse, bu fonksiyon co_await'i çalıştıran coroutine'in handle'ı ile çağrılır. Bu fonksiyon bize asenkron çalışmayı başlatma ve görev bittiğinde tetiklenecek bir bildirim için abone olma ve ardından coroutine'i devam ettirme fırsatı verir.<br>
await_resume(), sonucu (veya hatayı) coroutine'e geri paketlemekten sorumlu fonksiyondur. <br>
await_suspend() tarafından başlatılan çalışma sırasında bir hata meydana gelmişse, bu işlev yakalanan hatayı yeniden atabilir veya bir hata kodu döndürebilir. Tüm co_await ifadesinin sonucu await_resume() fonksiyonunun döndürdüğü sonuçtur.

