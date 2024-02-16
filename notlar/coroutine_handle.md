<!--
coroutine handle bir "callable object".
Standart kütüphane coroutine handle türünü generic bir tür olarak sunuyor.

```cpp
std::coroutine_handle<>
// ya da
std::coroutine_handle<void>
``` 

ya da 

```cpp
std::coroutine_handle<promise_type>
``` 


_coroutine_handle_ nesnesi fonksiyon çağrı operatörünün operandı olduğunda _coroutine_'in durdurulduğu noktadan çalışmaya tekrar devam etmesini sağlıyor.


