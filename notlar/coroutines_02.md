![relations](https://github.com/necatiergin/COROUTINES/blob/main/notlar/coroutine_realations.png)

Yukarıdaki resmi açıklayalım: <br>

`Caller` ve `Coroutine`, normalde uygulama kodumuzda uygulayacağımız gerçek fonksiyonlardır.<br>
`Return Object`, `Coroutine`'in döndürdüğü türdür ve tipik olarak, örneğin jeneratörler veya asenkron görevler gibi bazı özel kullanım durumları için tasarlanmış genel bir sınıf şablonudur. <br>
`Caller`, _Coroutine_'i devam ettirmek ve _Coroutine_'den gönderilen değerleri almak için _Return Object_ ile etkileşime girer.<br> 
`Return Object_ genellikle tüm çağrılarını _coroutine handle_'a yönlendirir.<br>
`Coroutine Handle`, _Coroutine State_'e ait olmayan bir _handle_'dır.<br> 
`Coroutine Handle` aracılığıyla `Coroutine State`i devam ettirebilir ve yok edebiliriz. <br>
`Coroutine State`, _coroutine frame_ olarak adlandırılan varlıktır. Opak bir nesnedir, yani boyutunu bielmeyiz ve handle'ı kullanmadan başka bir şekilde erişemeyiz. `Coroutine State`'e  erişmenin tek yolu _Coroutine Handle_'dır.<br>
`Coroutine State`, _coroutine_'i en son askıya alındığı _(durdurulduğu- suspend edildiği)_ yerden devam ettirmek için gerekli olan tüm bilgileri saklar. <br>
`Coroutine State` ayrıca _Promise_ nesnesini de içerir.
`Promise` nesnesi, _Coroutine_'in `co_await`, `co_yield` ve `co_return` ifadeleri aracılığıyla dolaylı olarak iletişim kurduğu varlıktır. <br>
_Değerler_ veya _exception_'lar `Coroutine`'den gönderildiğinde önce `Promise` nesnesine ulaşırlar. <br> 
`Promise` nesnesi, `Coroutine` ile `Caller` arasında bir kanal görevi görür, ancak her ikisinin de Promise nesnesine doğrudan erişimi yoktur.<br>
