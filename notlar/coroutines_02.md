![relations](https://github.com/necatiergin/COROUTINES/blob/main/notlar/coroutine_realations.png)

Yukarıdaki resmi açıklayalım: <br>

_Caller_ ve _Coroutine_, normalde uygulama kodumuzda uygulayacağımız gerçek fonksiyonlardır.<br>
`Return Object`, _coroutine_'in döndürdüğü türdür ve tipik olarak, örneğin jeneratörler veya asenkron görevler gibi bazı özel kullanım durumları için tasarlanmış genel bir sınıf şablonudur. <br>
_Caller_, _Coroutine_'i devam ettirmek ve _Coroutine_'den gönderilen değerleri almak için _Return Object_ ile etkileşime girer.<br> 
_Return Object_ genellikle tüm çağrılarını _coroutine handle_'a yönlendirir.<br>
_Coroutine Handle_, _Coroutine State_'e ait olmayan bir _handle_'dır.<br> 
_Coroutine Handle_ aracılığıyla _Coroutine_ durumunu devam ettirebilir ve yok edebiliriz. <br>
_Coroutine State_, _coroutine frame_ olarak adlandırılan varlıktır. Opak bir nesnedir, yani boyutunu bielmeyiz ve handle'ı kullanmadan başka bir şekilde erişemeyiz. _Coroutine State_'e  erişmenin tek yolu _Coroutine Handle_'dır.<br>
_Coroutine State_, _coroutine_'i en son askıya alındığı _(durdurulduğu- suspend edildiği)_ yerden devam ettirmek için gerekli olan tüm bilgileri saklar. <br>
_Coroutine State_ ayrıca _Promise_ nesnesini de içerir.
Promise nesnesi, coroutine'in co_await, co_yield ve co_return anahtar sözcükleri aracılığıyla dolaylı olarak iletişim kurduğu şeydir. 
Değerler veya hatalar coroutine'den gönderilirse, önce promise nesnesine ulaşırlar. 
Söz nesnesi, coroutine ile çağıran arasında bir kanal görevi görür, ancak her ikisinin de söze doğrudan erişimi yoktur.
Kuşkusuz, bu ilk bakışta oldukça yoğun görünebilir. Eksiksiz ancak minimal bir örnek, farklı parçaları biraz anlamanıza yardımcı olacaktır
