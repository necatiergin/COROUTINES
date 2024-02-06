- Bu tür, bir _coroutine_ ile işlem yapmak için belirli _"customization"_ noktalarını tanımlamak için kullanılır. 
- Belirli durumlarda çağrılan _call_back_ fonksiyonları tanımlar.
#### promise_type'ın implementasyonu 
- _coroutine_'in dönüş değerinin nasıl oluşturulacağını veya alınacağını tanımlar (genellikle _coroutine handle_'ın oluşturulmasını da sağlar)
- _coroutine_'in çalışmasının başında ya da sonunda _suspend_ edilip edilmeyeceğini (durdurulup durdurulmayacağını) belirler.
- _coroutine_'i çağıran ile _coroutine_ arasındaki veri alış verişini sağlar.
- işlenmemiş hata nesneleriyle _(unhandled exception)_ ilgili yapılacakları belirler.

- Derleme zamanında bir hata olmaması için promis_type'ın aşağıdaki fonksiyonarı tanımlaması gerekmektedir. (çağprıldıkları  sıra ile)  
- _get_return_object()_ fonksiyonu _coroutine_ arayüzünü oluşturması için çağrılır.Bu fonksiyon _coroutine arayüz_ nesnesini oluşturur. Oluşturulan nesne _coroutine_'i çağıran koda coroutine geri dönüş değeri ile iletilir.

