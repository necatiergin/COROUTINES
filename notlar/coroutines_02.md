![relations](https://github.com/necatiergin/COROUTINES/blob/main/notlar/coroutine_realations.png)

Caller ve coroutine, normalde uygulama kodumuzda uygulayacağımız gerçek fonksiyonlardır.<br>
`Return Object`, _coroutine_'in döndürdüğü türdür ve tipik olarak, örneğin jeneratörler veya asenkron görevler gibi bazı özel kullanım durumları için tasarlanmış genel bir sınıf şablonudur. <br>
_Caller_, _coroutine_'i devam ettirmek ve _coroutine_'den gönderilen değerleri almak için _Return Object_ ile etkileşime girer. 
Return Object genellikle tüm çağrılarını _coroutine handle_'a yönlendirir.<br>
_Coroutine Handle_, Coroutine State'e ait olmayan bir _handle_'dır.<br> Coroutine tanıtıcısı aracılığıyla coroutine durumunu devam ettirebilir ve yok edebiliriz.
_Coroutine State_, coroutine frame olarak adlandırılan varlıktır.<br> 
Opak bir nesnedir, yani boyutunu bilmeyiz ve tanıtıcıdan başka bir şekilde erişemeyiz. 
Coroutine state, coroutine'i en son askıya alındığı yerden devam ettirmek için gerekli olan her şeyi saklar. 
Coroutine state ayrıca Promise'i de içerir.
Promise nesnesi, coroutine'in co_await, co_yield ve co_return anahtar sözcükleri aracılığıyla dolaylı olarak iletişim kurduğu şeydir. 
Değerler veya hatalar coroutine'den gönderilirse, önce promise nesnesine ulaşırlar. 
Söz nesnesi, coroutine ile çağıran arasında bir kanal görevi görür, ancak her ikisinin de söze doğrudan erişimi yoktur.
Kuşkusuz, bu ilk bakışta oldukça yoğun görünebilir. Eksiksiz ancak minimal bir örnek, farklı parçaları biraz anlamanıza yardımcı olacaktır
