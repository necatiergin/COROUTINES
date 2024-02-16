_coroutine_'in implementasyonunbda kullanılan iki tür var:

- promise türü
Bu tür, _coroutine_'in kullanılmasında _custamization_ noktaları sunuyor. Bu türü kullanarak coroutine'in belirli noktalarda nasıl davranacağını belirleyebiliyoruz.
Bu türün üye fonksiyonları belirli durumlarda çağrılan _callback_'ler olarak kullanılıyorlar.

- std::coroutine_handle<> türü
coroutin başlatıldığında promise  sınıfının üye fonksiyomnlarından biri kullanılarak bu türden bir nesne oluşturuluıyor
This object is created when a coroutine is called (using one of the standard callbacks of the promise
type above). 
Bu tü crtin kontrolü için alt sevişyeli bir arayüz sağlıyor.
 -crt'in çalışmasınındevam ettirilmesi
-crt'in çalışmasının sonlandırılması


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


