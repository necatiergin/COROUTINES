_coroutine_'in implementasyonunbda kullanılan iki tür var:

- promise türü<br>
Bu tür, _coroutine_'in kullanılmasında _customization_ noktaları sunuyor. Bu türü kullanarak _coroutine_'in belirli noktalarda nasıl davranacağını belirleyebiliyoruz.
Bu türün üye fonksiyonları belirli durumlarda çağrılan _callback_'ler olarak kullanılıyorlar.

- _std::coroutine_handle<>_ türü <br>
_coroutine_ başlatıldığında _promise_ sınıfının üye fonksiyonlarından biri kullanılarak bu türden bir nesne oluşturuluyor. <br>
Bu tür _coroutine_'in kontrolü için alt seviyeli bir arayüz sağlıyor. <br>
- _coroutine_'in çalışmasının devam ettirilmesi _(resume)_
- _coroutine_'in çalışmasının sonlandırılması.

_coroutine handle_ bir _"callable object"._
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


