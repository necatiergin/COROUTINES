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
